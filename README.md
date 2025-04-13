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

`tcgetattr` 함수를 실행하는 데에 문제가 없었으면, tcgetattr ... OK.이 출력됨.

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
  printf("tcgetattr ... OK.\n");

  printf("terminal_setting_local_mode: 0x%04x.\n", terminal_setting.c_lflag);

  return 0;
}
```
cygwin의 sys/termios.h 파일의 코드, 의미는 posix의 `terminos` 구조체 [레퍼런스](https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap11.html#tag_11_02_01)에서 가져옴
> ```c
> /* lflag bits */
> #define ISIG	0x0001   /* ISIG는 Ctrl+C 입력으로 프로그램을 종료하는 신호를 보내는 등의 특별 제어 문자 확인을 가능하게 해준다. */
> #define ICANON	0x0002 /* ICANON은 콘솔에서 엔터를 눌렀을 때 프로그램을 키보드 입력들을 전송하게 한다. */
> #define ECHO	0x0004   /* ECHO는 입력받은 키를 그대로 화면에 출력하게 한다. */
> #define ECHOE	0x0008   /* ECHOE는 백스페이스를 입력했을 때 화면에서도 문자를 지운다. ICANON이 설정되어 있을 때 작동한다. */
> #define ECHOK	0x0010   /* Ctrl_U 입력으로 한 줄을 모두 지우는 것을 허용한다. ICANON이 설정되어 있을 때 작동한다. */
> #define ECHONL	0x0020 /* ECHO가 없을 때도, 엔터키 입력으로 줄을 바꾸는 것을 허용한다. ICANON이 설정되어 있을 때 작동한다. */
> #define NOFLSH	0x0040 /* Ctrl+C를 해도 프로그램으로 전송하지 않은 키보드 입력들을 보존한다. */
> #define TOSTOP	0x0080 /* 백그라운드 프로세스가 화면에 출력하는 것을 방지한다. */
> #define IEXTEN	0x0100 /* 고급 입력 기능을 허용한다. */
> #define FLUSHO	0x0200 /* posix의 확장판인 XSI에서 쓰인다. 모든 출력을 버린다. 옛날 glibc에서 쓰던 _BSD_SOURCE가 정의되어 있을 때 작동한다. */
> #define ECHOKE	0x0400 /* posix의 확장판인 XSI에서 쓰인다. 한 줄을 지울 때 한글자씩 지운다. 옛날 glibc에서 쓰던 _BSD_SOURCE가 정의되어 있을 때 작동한다. */
> #define ECHOCTL	0x0800 /* posix의 확장판인 XSI에서 쓰인다. 특수문자를 ^X 꼴로 표시한다. 옛날 glibc에서 쓰던 _BSD_SOURCE가 정의되어 있을 때 작동한다. */
> ```

테스트 결과 terminal_setting_local_mode: 0x0d1f가 출력되었다면, ECHOCTL, ECHOKE, IEXTEN, ECHOK, ECHOE, ECHO, ICANON, ISIG가 설정된 상태다. 

posix의 `termios` 구조체 [레퍼런스](https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap11.html#tag_11_02_01)
