# 편집기 프로젝트

## 1. 콘솔에서 키보드 하나 입력받기 (Windows의 C 라이브러리 사용 환경: Universal C Runtime, Microsoft Visual C++ Runtime Library)
```c
#include <stdio.h>
#include <conio.h>

int main() {
  int ch, count = 0;

  do {
    ch = _getch();
    printf("%d: [%3d, 0x%02x] '%c'\n", count++, ch, ch, ch);
  } while(1);

  return 0;
}
```
`int _getch();`의 [레퍼런스](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-getwch?view=msvc-170)

테스트 환경:

* Visual Studio 2022, 디버거 콘솔
* MSYS2 UCRT64 (mingw-w64-ucrt-x86_64-gcc), Mintty 터미널
* MSYS2 MINGW64 (mingw-w64-x86_64-gcc), Mintty 터미널

## 1. 콘솔에서 키보드 하나 입력받기 (Windows MSYS2)
