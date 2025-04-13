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

## 1. 콘솔에서 키보드 하나 입력받기 (Windows Cygwin)
**1) stdio.h 헤더 파일에 fileno 함수가 있고, stdin의 파일 번호가 정상적으로 출력되는지 체크하기**
```c
#include <stdio.h>

int main() {
  int stdin_fileno = fileno(stdin);
  printf("stdin_fileno : %d \n", stdin_fileno); /* 정상적인 경우 0이 출력됨 */
  return 0;
}
```
posix의 `int fileno(FILE* stream);` [레퍼런스](https://pubs.opengroup.org/onlinepubs/9799919799/functions/fileno.html)


**2) 터미널에서 termios.h 헤더 파일이 있는지 체크하기**
```bash
cygcheck -p "sys/termios.h"
```
헤더 파일이 있으면, cygwin-devel-버전 - cygwin-devel: Core development files가 출력됨

cygwin의 sys/termios.h 파일 [github](https://github.com/openunix/cygwin/blob/master/winsup/cygwin/include/sys/termios.h)


**3) 터미널 셋팅을 가져오는 데에 문제가 없는지 확인하기**
```c
#include <stdio.h>
#include <sys/termios.h>

int main() {
   
  int stdin_fileno = fileno(stdin);
  struct termios terminal_setting;

  int error_no = tcgetattr(stdin_fileno, &terminal_setting);
  if (error_no) {
    printf("Error: tcgetattr, error_no is %d.\n", error_no);
    return 1;
  }
  else {
    printf("tcgetattr ... OK.\n");
  }

  return 0;
}
```

posix의 `int tcgetattr(int fildes, struct termios *termios_p);` [레퍼런스](https://pubs.opengroup.org/onlinepubs/9799919799/functions/tcgetattr.html#)


**4) 터미널 셋팅값 확인하기**
```c
#include <stdio.h>
#include <sys/termios.h>

int main() {
   
  int stdin_fileno = fileno(stdin);
  struct termios terminal_setting;

  int error_no = tcgetattr(stdin_fileno, &terminal_setting);
  if (error_no) {
    printf("Error: tcgetattr, error_no is %d.\n", error_no);
    return 1;
  }
  else {
    printf("tcgetattr ... OK.\n");
  }

  return 0;
}
```

posix의 `termios` 구조체 [레퍼런스](https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap11.html#tag_11_02_01)
