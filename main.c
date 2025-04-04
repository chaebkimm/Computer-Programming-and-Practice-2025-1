/* add_pixel 함수는 red, green, blue에 해당하는 색깔을 가진 픽셀을 데이터 모음(배열)에 저장합니다. */
/* red, green, blue는 빛의 3원색으로, 0이 가장 어두운 값이고, 255가 가장 밝은 값입니다. */
void add_pixel(int red, int green, int blue);

/* make_png 함수는 가로, 세로 픽셀 개수를 정해주면 지금까지 저장된 픽셀 데이터를 가지고 png 파일을 만듭니다. */
/* width가 가로 픽셀 개수, height가 세로 픽셀 개수입니다. */
void make_png(int width, int height);

void make_small_c_img();

void minimal_img();

void pixel(int red, int green, int blue);

int main() {
	/* 가로 250, 세로 200 픽셀 크기의 그림을 모두 노랗게 색칠한 png 파일을 만듭니다. */
	int i = 0;
	for(i = 0; i < 50000; i++) add_pixel(255, 255, 0);
	make_png(250, 200);
}
