/*
 * Copyright (C) 2019 Alexander Borisov
 *
 * Author: Alexander Borisov <borisov@lexbor.com>
 */

#include "lexbor/encoding/encoding.h"
#include "lexbor/encoding/multi.h"


#define append_to_file(fc, data, len, codepoint)                               \
    do {                                                                       \
        lxb_char_t *data_to = data;                                            \
                                                                               \
        for (size_t j = 0; j < len; j++) {                                     \
            fprintf(fc, "\\x%02X", (unsigned) data_to[j]);                     \
        }                                                                      \
                                                                               \
        fprintf(fc, " 0x%04X\n", (unsigned) codepoint);                        \
    }                                                                          \
    while (0)


int main(int argc, const char * argv[])
{
    int8_t len;
    size_t size;
    const lxb_encoding_data_t *enc_data;
    const lxb_encoding_multi_index_t *entry;

    lxb_char_t data[8];
    lxb_char_t *ref = data;
    const lxb_char_t *end = data + sizeof(data);

    const char *filepath = "./big5_map_decode.txt";

    enc_data = lxb_encoding_data(LXB_ENCODING_BIG5);

    FILE *fc = fopen(filepath, "w");
    if (fc == NULL) {
        printf("Failed to opening file: %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    fprintf(fc, "#\n"
            "# Copyright (C) 2019 Alexander Borisov\n"
            "#\n"
            "# Author: Alexander Borisov <borisov@lexbor.com>\n"
            "#\n\n");

    fprintf(fc, "#\n"
            "# This file generated by the program\n"
            "# \"utils/lexbor/encoding/big5_map_decode.c\"\n"
            "#\n\n");

    /* Single index */
    size = sizeof(lxb_encoding_multi_index_big5)
           / sizeof(lxb_encoding_multi_index_t);

    for (size_t i = 0; i < size; i++) {
        lxb_encoding_encode_t ctx = {0};

        entry = &lxb_encoding_multi_index_big5[i];

        if (entry->codepoint > LXB_ENCODING_DECODE_MAX_CODEPOINT) {
            continue;
        }

        len = enc_data->encode(&ctx, &ref, end, entry->codepoint);
        if (len < LXB_ENCODING_ENCODE_OK) {
            printf("Failed to encoding code point: %u\n",
                   entry->codepoint);
            return EXIT_FAILURE;
        }

        ref -= len;
        append_to_file(fc, ref, len, entry->codepoint);
    }

    fprintf(fc, "\n# END\n");

    fclose(fc);

    return EXIT_SUCCESS;
}