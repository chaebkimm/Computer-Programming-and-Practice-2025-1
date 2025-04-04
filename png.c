#include<stdio.h>
#include<stdlib.h>
#include"huffman.h"
#include"png.h"

static FILE* img = NULL;

static unsigned long crc_table[256];
static unsigned int width;
static unsigned int height;

static void init_crc_table() {
	for (int n = 0; n < 256; n++) {
		unsigned long c = (unsigned long)n;
		for (int k = 0; k < 8; k++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		crc_table[n] = c;
	}
}

static void png_write(const unsigned char* buf, unsigned int len) {
	if (!img) exit(1);
	while (len-- > 0) {
		fputc(*(buf++), img);
	}
}

static void png_write_int(const unsigned int data) {
	if (!img) exit(1);
	for (int i = 24; i >= 0; i -= 8) {
		fputc((data >> i) & 0xff, img);
	}
}

static void png_open(const char* filename, unsigned int png_width, unsigned png_height) {
	img = fopen(filename, "wb");

	if (!img) {
		printf("File open error");
		exit(1);
	}

	width = png_width;
	height = png_height;

	const unsigned char PNG_SIGNITURE[8] = { 137, 'P', 'N', 'G', 13, 10, 26, 10 };
	png_write(PNG_SIGNITURE, 8);

	init_crc_table();
}

static unsigned int encode_char(unsigned char* buf, unsigned int offset, const unsigned char data) {
	buf[offset++] = data & 0xff;
	return offset;
}

static unsigned int encode_int(unsigned char* buf, unsigned int offset, const unsigned int data) {
	for (int i = 24; i >= 0; i -= 8) {
		buf[offset++] = (data >> i) & 0xff;
	}
	return offset;
}

static unsigned long get_crc(unsigned char* buf, const unsigned int size, unsigned long c) {
	c ^= 0xffffffffL;
	for (unsigned int n = 0; n < size; n++) {
		c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	}
	return c ^= 0xffffffffL;
}

static void png_header() {
	const unsigned int n_bits = 13;
	unsigned char type[4] = "IHDR";
	unsigned char* data = malloc(n_bits);

	unsigned int data_offset = 0;
	data_offset = encode_int(data, data_offset, width);
	data_offset = encode_int(data, data_offset, height);

	const unsigned char bit_depth = 8;
	data_offset = encode_char(data, data_offset, bit_depth);
	
	const unsigned char color_type = 2;
	data_offset = encode_char(data, data_offset, color_type);

	const unsigned char compression_method = 0;
	data_offset = encode_char(data, data_offset, compression_method);

	const unsigned char filter_method = 0;
	data_offset = encode_char(data, data_offset, filter_method);

	const unsigned char interlace_method = 0;
	data_offset = encode_char(data, data_offset, interlace_method);

	unsigned long crc = get_crc(type, 4, 0);
	crc = get_crc(data, data_offset, crc);

	png_write_int(data_offset);
	png_write(type, 4);
	png_write(data, data_offset);
	png_write_int(crc & 0xffffffff);
}

static void png_data(const unsigned char* rgbs) {
	const unsigned int n_bits = (width * 3 + 1) * height * 3 / 2 + 5;
	unsigned char type[4] = "IDAT";
	unsigned char* data = malloc(n_bits);

	unsigned int data_offset = 0;

	const unsigned char CM = 0x08;

	unsigned int log_n_bits = (width * 3 * height) >> 7;
	unsigned char CINFO = 0;

	for (; log_n_bits > 0 && CINFO < 7; CINFO++) {
		log_n_bits >>= 1;
	}

	const unsigned char CMF = CM | ((CINFO & 0x0f) << 4);
	data_offset = encode_char(data, data_offset, CMF);

	const unsigned char FLEVEL = 3;
	const unsigned char FDICT = 0;
	unsigned char FCHECK = 0;

	unsigned int CMF_FLG = (CMF << 8) | (FLEVEL << 6) | (FDICT << 5);
	if ((CMF_FLG % 31) > 0) {
		FCHECK = 31 - CMF_FLG % 31;
	}

	const unsigned char FLG = (FLEVEL << 6) | (FDICT << 5) | (FCHECK & 0x1f);


	data_offset = encode_char(data, data_offset, FLG);

	printf("\nhuffman_static_lines\n");

	data_offset += huffman_static_lines(rgbs, width, height, data + data_offset);

	printf("\nget_crc\n");


	unsigned long crc = get_crc(type, 4, 0);
	crc = get_crc(data, data_offset, crc);

	png_write_int(data_offset);
	png_write(type, 4);
	png_write(data, data_offset);
	png_write_int(crc & 0xffffffff);
}

static void png_end() {
	unsigned char type[4] = "IEND";
	unsigned int data_offset = 0;
	unsigned long crc = get_crc(type, 4, 0);

	png_write_int(data_offset);
	png_write(type, 4);
	png_write_int(crc & 0xffffffff);
}

extern int png_save(const char* filename, unsigned int png_width, unsigned png_height, const unsigned char* rgbs) {
	printf("\n\npng_open\n\n");
	png_open(filename, png_width, png_height);

	printf("\n\npng_header\n\n");
	png_header();

	printf("\n\npng_data\n\n");
	png_data(rgbs);

	printf("\n\npng_end\n\n");
	png_end();

	fclose(img);

	img = fopen(filename, "rb");
	
	int c, col = 0;
	while ( (c = fgetc(img)) != EOF) {
		printf("%02x ", (unsigned char) c);
		if ( ++col == 16 ) {
			printf("\n");
			col = 0;
		}
	}
	printf("\n");
	
	fclose(img);

	return 0;
}
