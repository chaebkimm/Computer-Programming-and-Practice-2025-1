# PNG 파일 만들기

```c
/* add_pixel 함수는 red, green, blue에 해당하는 색깔을 가진 픽셀을 데이터 모음(배열)에 저장합니다. */
/* red, green, blue는 빛의 3원색으로, 0이 가장 어두운 값이고, 255가 가장 밝은 값입니다. */
void add_pixel(int red, int green, int blue);

/* make_png 함수는 가로, 세로 픽셀 개수를 정해주면 지금까지 저장된 픽셀 데이터를 가지고 png 파일을 만듭니다. */
/* width가 가로 픽셀 개수, height가 세로 픽셀 개수입니다. */
void make_png(int width, int height);
```

변경사항: 
* fopen_s가 visual studio 외의 환경에서 에러를 발생시켜 fopen으로 복구했습니다. 
* visual studio의 sln 파일에서 _CRT_SECURE_NO_WARNINGS 전처리기를 추가해서 fopen을 써도 에러가 없게 만들었습니다.

# 레퍼런스
* [위키피디아 PNG 설명 페이지](https://en.wikipedia.org/wiki/PNG)
* [PNG 문서](https://www.rfc-editor.org/rfc/rfc2083)
* [zlib 호프만 코딩 문서](https://www.rfc-editor.org/rfc/rfc1951)
* [zlib 압축 방법](https://www.rfc-editor.org/rfc/rfc1950)
