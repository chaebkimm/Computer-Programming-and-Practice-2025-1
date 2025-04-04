#include <stdlib.h>
#include <stdio.h>

static unsigned char* buf = NULL;
static unsigned int bits_total = 0;
static unsigned int bit_pos = 0;
static unsigned int index = 0;

unsigned int huffman_bits(unsigned int lit_value) {
    if (lit_value < 144) return 8;
    else if (lit_value < 256) return 9;
    else if (lit_value < 280) return 7;
    else if (lit_value < 288) return 8;
    else exit(1);
}

unsigned int huffman_codes(unsigned int lit_value) {
    if (lit_value < 144) return 0b00110000 + lit_value;
    else if (lit_value < 256) return 0b110010000 + lit_value - 144;
    else if (lit_value < 280) return 0b0000000 + lit_value - 256;
    else if (lit_value < 288) return 0b11000000 + lit_value - 280;
    else exit(1);
}

unsigned int append_bit(unsigned char bit) {
    printf("%d", bit);

    buf[index] = (buf[index] & (~(1 << bit_pos))) | (bit << bit_pos);

    bits_total++;
    bit_pos = bits_total % 8;
    index = bits_total / 8;

    return 1;
}

unsigned int append_bits(unsigned int bits, unsigned int n_bits) {
    printf("\n%d bits: %x ", n_bits, bits);
    for (unsigned int n = 0; n < n_bits; n++) {
        unsigned char bit = (unsigned char)(bits & 0x01);
        append_bit(bit);
        bits >>= 1;
    }
    return n_bits;
}

unsigned int append_bits_reverse(unsigned int bits, unsigned int n_bits) {
    printf("\n%d bits: ", n_bits);
    for (int n = n_bits - 1; n >= 0; n--) {
        unsigned char bit = (unsigned char)((bits >> n) & 0x01);
        append_bit(bit);
    }
    return n_bits;
}

static unsigned int append_bits_int(unsigned int bits) {
    printf("\nbits: %x ", bits);
    for (unsigned int n = 4; n-- > 0; ) {
        append_bits(bits >> (n*8) & 0xff, 8);
    }
    return 32;
}

unsigned long get_adler(unsigned char* buf, unsigned int buf_size) {
    const unsigned long BASE = 65521;

    unsigned long s1 = 1L;
    unsigned long s2 = 0L;

    for (unsigned int n = 0; n < buf_size; n++) {
        s1 = (s1 + buf[n]) % BASE;
        s2 = (s2 + s1) % BASE;
    }

    return (s2 << 16) + s1;
}

unsigned long get_adler_from_lines(const unsigned char* rgbs, int width, int height) {
    const unsigned long BASE = 65521;

    unsigned long s1 = 1L;
    unsigned long s2 = 0L;

    for (int y = 0; y < height; y++) {

        s1 = (s1 + 0) % BASE;
        s2 = (s2 + s1) % BASE;

        for (int x = 0; x < 3 * width; x++) {
            int n = x + y * 3 * width;

            s1 = (s1 + rgbs[n]) % BASE;
            s2 = (s2 + s1) % BASE;
        }
    }

    printf("\nadler: %lx %d %d", (s2 << 16) + s1, width, height);

    return (s2 << 16) + s1;
}

unsigned int huffman_static_lines(const unsigned char* rgbs, int width, int height, unsigned char* huff_codes) {

    buf = huff_codes;

    printf("\n**BFINAL**\n");

    unsigned char bfinal = 1;
    append_bit(bfinal);

    printf("\n**BTYPE**\n");
    unsigned char btype = 1;
    append_bits(btype, 2);

    printf("\n**IMG\n");

    for (int y = 0; y < height; y++) {

	printf("\n**row:%d, FILTERTYPE**\n", y);

        append_bits_reverse(huffman_codes(0), huffman_bits(0));

        for (int x = 0; x < 3 * width; x++) {
            int n = x + y * 3 * width;

            int lit_value = rgbs[n];
            int huff_code = huffman_codes(lit_value);
            int n_huff_bit = huffman_bits(lit_value);

            append_bits_reverse(huff_code, n_huff_bit);
        }
        
        if (y == height - 1) {
            append_bits_reverse(0x0000000, 7);
            if (bits_total % 8) {
                append_bits_reverse(0, 8 - (bits_total % 8));
            }
        }

        printf("\nbits_total: %d\n", bits_total);

    }

    if (bits_total % 8) exit(2);

    unsigned long adler = get_adler_from_lines(rgbs, width, height);

    printf("\nadler: %lx\nadler & 0xffffffff: %lx\n(unsigned int)(adler & 0xffffffff) : %x\n", adler, adler & 0xffffffff, (unsigned int)(adler & 0xffffffff));

    append_bits_int((unsigned int)(adler & 0xffffffff));

    printf("\n");
    int ncol = 4;
    for (unsigned int i = 0; i < bits_total/8/ncol; i++) {
        for (int j = 0; j < ncol; j++) {
            int n = i * ncol + j;
            printf("%02x ", huff_codes[n]);
        }
        for (int j = 0; j < ncol; j++) printf(" ");

        for (int j = 0; j < ncol; j++) {
            int n = i * ncol + j;
            for (int k = 8; k-- > 0;) {
                printf("%d", (huff_codes[n] >> k) & 0x01);
            }
            printf(" ");
        }

        printf("\n");
    }
    printf("\n");

    buf = NULL;

    return bits_total / 8;
}

