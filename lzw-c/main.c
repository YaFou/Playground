// QUESTION 1
// Compression : dictionnaire et table inverse
// Décompression : dictionnaire

// QUESTION 2
// 8 + 65536 * (32 + 8) + 32 = 2621480

// QUESTION 3
// 65536 * 256 = 16777216

// QUESTION 9
// O(nd)

// QUESTION 13
// 4 codes

#include "lzw.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("arguments invalides\n");
        return 0;
    }

    FILE* in = stdin;
    FILE* out = stdout;

    if (argc >= 3) {
        in = fopen(argv[2], "r");
    }
    if (argc >= 4) {
        out = fopen(argv[3], "w");
    }
    
    char commande = argv[1][0];
    initialize_dictionary();

    if (commande == 'c') {
        compress(in, out);
    } else if (commande == 'C') {
        mock_compress(in, out);
    } else if (commande == 'd') {
        decompress(in, out);
    } else if (commande == 'D') {
        mock_decompress(in, out);
    } else {
        printf("arguments invalides\n");
    }

    fclose(in);
    fclose(out);

    return 0;
}