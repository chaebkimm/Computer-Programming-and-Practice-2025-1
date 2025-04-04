
void add_pixel(int red, int green, int blue); 

void make_png(int width, int height);

void make_small_c_img();

void minimal_img();

void pixel(int red, int green, int blue);

int main() {
	int i = 0;
	for(i = 0; i < 50000; i++) add_pixel(255, 255, 0);
	make_png(250, 200);
}