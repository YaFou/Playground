#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

typedef uint32_t cw_t;
typedef uint8_t byte_t;

struct dict_entry_t {
    cw_t pointer;
    byte_t byte;
};
typedef struct dict_entry_t dict_entry_t;

#define CW_MAX_WIDTH 16
#define DICTFULL (1u << CW_MAX_WIDTH)

struct dict_t {
    cw_t next_available_cw;
    int cw_width;
    dict_entry_t data[DICTFULL];
};

void initialize_dictionary();
void mock_compress(FILE* input_file, FILE* output_file);
void mock_decompress(FILE* input_file, FILE* output_file);
void compress(FILE* input_file, FILE* output_file);
void decompress(FILE* input_file, FILE* output_file);