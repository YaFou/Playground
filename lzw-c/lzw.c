#include "lzw.h"
#include <stdbool.h>
#include <unistd.h>

const cw_t NO_ENTRY = DICTFULL;
const cw_t NULL_CW = DICTFULL;
const cw_t FIRST_CW = 256;
const int CW_MIN_WIDTH = 16;

struct dict_t dict;
cw_t inverse_table[DICTFULL][256];

void initialize_dictionary() {
    dict.next_available_cw = FIRST_CW;
    dict.cw_width = CW_MAX_WIDTH;
    for (int i = 0; i < FIRST_CW; i++) {
        dict_entry_t entry = {.pointer = NULL_CW, .byte = i};
        dict.data[i] = entry;
    }
}

cw_t lookup(cw_t cw, byte_t byte) {
    cw_t code = inverse_table[cw][byte];

    if (code >+ dict.next_available_cw) {
        return NO_ENTRY;
    }

    dict_entry_t entry = dict.data[code];

    if (entry.pointer == cw && entry.byte == byte) {
        return code;
    }

    return NO_ENTRY;
    return code < dict.next_available_cw ? code : NO_ENTRY;
}

void build_entry(cw_t cw, byte_t byte) {
    if (dict.next_available_cw == DICTFULL) {
        return;
    }

    dict_entry_t entry = {.pointer = cw, .byte = byte};
    dict.data[dict.next_available_cw] = entry;
    inverse_table[cw][byte] = dict.next_available_cw;
    dict.next_available_cw++;
}

void mock_compress(FILE* input_file, FILE* output_file) {
    cw_t dernier = NULL_CW;
    char c = getc(input_file);
    while (EOF != c) {
        if (dernier == NULL_CW) {
            dernier = (int) c;
            c = getc(input_file);
        } else {
            cw_t pointer = lookup(dernier, (int) c);

            if (pointer == NO_ENTRY) {
                build_entry(dernier, (int) c);
                fprintf(output_file, "%d ", dernier);
                dernier = NULL_CW;
            } else {
                dernier = pointer;
                c = getc(input_file);
            }
        }
    }

    if(dernier != NULL_CW) {
        cw_t pointer = lookup(dernier, (int) c);

        if (pointer == NO_ENTRY) {
            build_entry(dernier, (int) c);
            fprintf(output_file, "%d ", dernier);
            dernier = NULL_CW;
        }
    }
}

byte_t decode_cw(FILE* fp, cw_t cw) {
    byte_t* motif = (byte_t*) malloc(sizeof(byte_t) * DICTFULL);
    int i = 0;
    dict_entry_t entry = dict.data[cw];
    
    while(entry.pointer != NULL_CW) {
        motif[i] = entry.byte;
        entry = dict.data[entry.pointer];
        i++;
    }
    motif[i] = entry.byte;

    for (int j = i; j >= 0; j--) {
        putc(motif[j], fp);
    }
    
    byte_t x = motif[i];
    free(motif);
    return x;
}

byte_t get_first_byte(cw_t cw) {
    dict_entry_t entry = dict.data[cw];
    while (entry.pointer != NULL_CW) {
        entry = dict.data[entry.pointer];
    }
    return entry.byte;
}

void mock_decompress(FILE* input_file, FILE* output_file) {
    cw_t code_courant;
    fscanf(input_file, "%d ", &code_courant);
    byte_t c = decode_cw(output_file, code_courant);
    cw_t dernier_code = code_courant;

    while(EOF != fscanf(input_file, "%d ", &code_courant)) {
        if (code_courant < dict.next_available_cw) {
            c = decode_cw(output_file, code_courant);
        } else if (code_courant == dict.next_available_cw) {
            c = decode_cw(output_file, dernier_code);
            putc(c, output_file);
        }
        build_entry(dernier_code, c);
        dernier_code = code_courant;
    }
}

const int BUFFER_WIDTH = 64;
const int BYTE_WIDTH = 8;
#define MASK(width) (1 << width) - 1
const uint64_t BYTE_MASK = MASK(BYTE_WIDTH);
const int START_WIDTH = 9;

struct bit_file { 
    FILE *fp;
    uint64_t buffer;
    int buffer_length;
};

typedef struct bit_file bit_file;

bit_file *bin_initialize(FILE *fp){
    bit_file *bf = malloc(sizeof(bit_file));
    bf->fp = fp;
    bf->buffer = 0;
    bf->buffer_length = 0;
    return bf;
}

void print_bits(int data) {
    if (data == 0) {
        return;
    }
    print_bits(data >> 1);
    printf(data & 1 == 1 ? "1" : "0");
}

void output_bits(bit_file* bf, uint64_t data, int width, bool flush) {
    bf->buffer += (data & MASK(width)) << bf->buffer_length;
    bf->buffer_length += width;

    while(bf->buffer_length >= BYTE_WIDTH) {
        byte_t x = bf->buffer & MASK(BYTE_WIDTH);
        putc(x, bf->fp);
        bf->buffer_length -= BYTE_WIDTH;
        bf->buffer = bf->buffer >> BYTE_WIDTH;
    }

    if (flush) {
        putc(bf->buffer, bf->fp);
    }
}

uint64_t input_bits(bit_file* bf, int width, bool* eof) {
    while (bf->buffer_length < width) {
        int c = getc(bf->fp);

        if (c == EOF && feof(bf->fp)) {
            *eof = true;
            return 0;
        } else if (c == EOF) {
            c = 24;
        }

        bf->buffer += + ((byte_t) c) << bf->buffer_length;
        bf->buffer_length += BYTE_WIDTH;
    }

    uint64_t x = bf->buffer & MASK(width);
    bf->buffer = bf->buffer >> width;
    bf->buffer_length -= width;
    *eof = false;
    return x;
}

void compress(FILE* input_file, FILE* output_file) {
    bit_file* bf = bin_initialize(output_file);
    cw_t dernier = getc(input_file);
    char c = getc(input_file);
    int width = START_WIDTH;

    while (EOF != c) {
        if (dernier == NULL_CW) {
            dernier = (int) c;
            c = getc(input_file);
        } else {
            cw_t pointer = lookup(dernier, (int) c);

            if (pointer == NO_ENTRY) {
                build_entry(dernier, (int) c);
                output_bits(bf, dernier, width, false);
                dernier = NULL_CW;

                if (dict.next_available_cw == MASK(width)) {
                    width++;
                }
            } else {
                dernier = pointer;
                c = getc(input_file);
            }
        }
    }

    if (dernier != NULL_CW) {
        cw_t pointer = lookup(dernier, (int) c);

        if (pointer == NO_ENTRY) {
            build_entry(dernier, (int) c);
            output_bits(bf, dernier, width, true);
            dernier = NULL_CW;
        }
    }
}

void decompress(FILE* input_file, FILE* output_file) {
    bit_file* bf = bin_initialize(input_file);
    bool eof;
    cw_t code_courant = input_bits(bf, START_WIDTH, &eof);
    if (eof) {
        return;
    }
    byte_t c = decode_cw(output_file, code_courant);
    cw_t dernier_code = code_courant;
    int width = START_WIDTH;
    code_courant = input_bits(bf, width, &eof);

    while(!eof) {
        if (code_courant < dict.next_available_cw) {
            c = decode_cw(output_file, code_courant);
        } else if (code_courant == dict.next_available_cw) {
            c = decode_cw(output_file, dernier_code);
            putc(c, output_file);
        }
        build_entry(dernier_code, c);
        dernier_code = code_courant;
        if (dict.next_available_cw == MASK(width) - 1) {
            width++;
        }
        code_courant = input_bits(bf, width, &eof);
    }
}