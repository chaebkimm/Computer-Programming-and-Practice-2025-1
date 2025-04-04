#include<stdio.h>
int main() {
	// test the binary in wikipedia
	FILE *f = fopen("test.png", "wb");
	
	const unsigned char* data = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A" 
		"\x00\x00\x00\x0D" "\x49\x48\x44\x52" "\x00\x00\x00\x01" 
		"\x00\x00\x00\x01" "\x08\x02\x00\x00" "\x00\x90\x77\x53"
		"\xDE\x00\x00\x00" "\x0C\x49\x44\x41" "\x54\x08\xD7\x63"
		"\xF8\xCF\xC0\x00" "\x00\x03\x01\x01" "\x00\x18\xDD\x8D"
		"\xB0\x00\x00\x00" "\x00\x49\x45\x4E" "\x44\xAE\x42\x60\x82";

	int i;

	for (i = 0; i < 69; i++) {
		int j = fputc(data[i], f);
		if (j == data[i]) 
			printf("%d: %02x is written\n", i, data[i]);
		else if (j == EOF) 
			printf("EOF\n");
		else
			printf("error, j:%02x, data:%02x\n", j, data[i]);
	}	
}
