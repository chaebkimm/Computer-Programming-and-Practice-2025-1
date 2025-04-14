# 편집기 프로젝트

## 1. 콘솔에서 키보드 하나 입력받기 (Windows. C 라이브러리 사용 환경: Universal C Runtime, Microsoft Visual C++ Runtime Library)
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

* MSVC, Visual Studio 2022 디버거 콘솔
* MSVC, Developer Command Prompt for VS2022에서 시작한 VSCode의 터미널
* GCC, MSYS2 UCRT64에서 설치됨 (mingw-w64-ucrt-x86_64-gcc), MSYS2의 터미널 (Mintty)
* GCC, MSYS2 UCRT64에서 설치됨 (mingw-w64-ucrt-x86_64-gcc), Command Prompt에서 시작한 VSCode의 터미널 (mingw-w64-ucrt-x86_64-toolchain 설치됨)
* GCC, MSYS2 MINGW64에서 설치됨 (mingw-w64-x86_64-gcc), MSYS2의 터미널 (Mintty)
* GCC, MSYS2 MINGW64에서 설치됨 (mingw-w64-x86_64-gcc), Command Prompt에서 시작한 VSCode의 터미널 (mingw-w64-x86_64-toolchain 설치됨)
* CLANG, MSYS2 CLANG64에서 설치됨 (mingw-w64-clang-x86_64-gcc), MSYS2의 터미널 (Mintty)
* CLANG, MSYS2 CLANG64에서 설치됨 (mingw-w64-clang-x86_64-gcc), VSCode에서 tasks.json을 변경해서 실행한 같은 창에서의 PowerShell 터미널
* CLANG, MSYS2 CLANG64에서 설치됨 (mingw-w64-clang-x86_64-gcc), VSCode에서 tasks.json을 변경해서 실행한 새로운 창에서의 MSYS2 터미널

## 1. 콘솔에서 키보드 하나 입력받기 (Windows. C 라이브러리 사용 환경: Cygwin)
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

  int error_no = tcgetattr(stdin_fileno, &terminal_setting); /* 터미널 셋팅을 terminal_setting에 저장한다. */
  /* 터미널 셋팅을 가져올 때 에러가 있었는지 확인한다 */
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


**4) 터미널 셋팅값 확인하기 - 로컬모드**
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

  printf("terminal_setting_local_mode: 0x%04x.\n", terminal_setting.c_lflag); /* 터미널 셋팅 중 로컬 모드 관련 값 (int 타입)을 출력한다. */

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
>
> struct termios
> {
>  tcflag_t	c_iflag;
>  tcflag_t	c_oflag;
>  tcflag_t	c_cflag;
>  tcflag_t	c_lflag;
>  char		c_line;
>  cc_t		c_cc[NCCS];
>  speed_t	c_ispeed;
>  speed_t	c_ospeed;
> };
>
> ```

테스트 결과 terminal_setting_local_mode: 0x0d1f가 출력되었다면, ECHOCTL, ECHOKE, IEXTEN, ECHOK, ECHOE, ECHO, ICANON, ISIG가 설정된 상태다. 

posix의 `termios` 구조체 [레퍼런스](https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap11.html#tag_11_02_01)


**5) 터미널 셋팅값 확인하기**
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

  printf("terminal_setting_non_canonical_mode MIN value: %d.\n", terminal_setting.c_cc[VMIN]); /* 터미널 셋팅 값 중 일반 모드가 아닐 때 몇바이트씩 입력을 받을지에 대한 값을 출력한다. */

  printf("terminal_setting_non_canonical_mode TIME value: %d.\n", terminal_setting.c_cc[VTIME]); /* 터미널 셋팅 값 중 일반 모드가 아닐 때 얼마나 오래 입력을 기다릴지를 0.1초 단위로 출력한다. */

  return 0;
}
```
테스트 결과 terminal_setting_non_canonical_mode MIN value: 1.이 출력되었다면, 1바이트씩 stdin에 전달한다는 뜻이다. 

또한, terminal_setting_non_canonical_mode TIME value: 0.이 출력되었다면, 키보드 입력을 무한정 기다린다는 뜻이다.

cygwin의 sys/termios.h 파일의 코드, 의미는 posix의 `terminos` 구조체 [레퍼런스](https://pubs.opengroup.org/onlinepubs/9799919799/basedefs/V1_chap11.html#tag_11_02_06)에서 가져옴
> ```c
> #define VDISCARD  1
> #define VEOL      2
> #define VEOL2     3
> #define VEOF      4
> #define VERASE    5
> #define VINTR     6
> #define VKILL     7
> #define VLNEXT    8
> #define VMIN      9
> #define VQUIT    10
> #define VREPRINT 11
> #define VSTART   12
> #define VSTOP    13
> #define VSUSP    14
> #define VSWTC    15
> #define VTIME    16
> #define VWERASE  17
> 
> #define NCCS     18
> ```

**6) 터미널 셋팅값 변경하기**
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

  printf("terminal_setting_non_canonical_mode MIN value: %d.\n", terminal_setting.c_cc[VMIN]);

  printf("terminal_setting_non_canonical_mode TIME value: %d.\n", terminal_setting.c_cc[VTIME]);

  terminal_setting.c_lflag &= ~ICANON; /* 터미널 셋팅 정보 중 로컬 모드에서 일반 입력 모드 여부를 나타내는 비트를 0으로 설정한다. */ 

  /* 변경된 터미널 셋팅 정보로 터미널 설정을 당장 변경한다. */
  error_no = tcsetattr(stdin_fileno, TCSANOW, &terminal_setting);
  if (error_no) {
    printf("Error: tcsetattr, error_no is %d.\n", error_no);
    return 1;
  }

  printf("tcsetattr ... OK.\n"); /* 터미널 설정 변경시 에러가 없을 경우 출력된다. */

  return 0;
}
```
테스트 결과 tcsetattr ... OK.이 출력되었다면, 로컬 모두 세팅에서 일반적인 경우(Canonical mode) 여부를 나타내는 비트가 0으로 설정된 것으로, 문자 하나씩 입력이 가능해진 것이다. 

posix의 `int tcsetattr(int fildes, int optional_actions, const struct termios *termios_p);` [레퍼런스](https://pubs.opengroup.org/onlinepubs/9799919799/functions/tcsetattr.html#)

**7) 터미널에서 키보드 입력을 받아보기**
```c
#include <stdio.h>
#include <sys/termios.h>

int main() {
   
  int stdin_fileno = fileno(stdin);
  struct termios terminal_setting;

  int error_no = tcgetattr(stdin_fileno, &terminal_setting);
  unsigned char ch_input; /* 키보드 입력을 저장할 문자 변수 */

  if (error_no) {
    printf("Error: tcgetattr, error_no is %d.\n", error_no);
    return 1;
  }
  printf("tcgetattr ... OK.\n");

  printf("terminal_setting_local_mode: 0x%04x.\n", terminal_setting.c_lflag);

  printf("terminal_setting_non_canonical_mode MIN value: %d.\n", terminal_setting.c_cc[VMIN]);

  printf("terminal_setting_non_canonical_mode TIME value: %d.\n", terminal_setting.c_cc[VTIME]);

  terminal_setting &= ~ICANON;

  error_no = tcsetattr(stdin_fileno, TCSANOW, &terminal_setting);
  if (error_no) {
    printf("Error: tcsetattr, error_no is %d.\n", error_no);
    return 1;
  }

  printf("tcsetattr ... OK.\n");

  /* 키보드 입력을 받고, 해당 정수 값을 출력한다. */
  do {
    scanf("%c", &ch_input);
    printf("\nint value of input character is : %d. \n", ch_input);
  while(ch_input != 'q');

  return 0;
}
```
실행 결과
```
tcgetattr ... OK.
terminal_setting_local_mode: 0x0d1d.
terminal_setting_non_canonical_mode MIN value: 1.
terminal_setting_non_canonical_mode TIME value: 0.
tcsetattr ... OK.
a
int value of input character is : 97.
z
int value of input character is : 122.
0
int value of input character is : 48.
9
int value of input character is : 57.
A
int value of input character is : 65.
Z
int value of input character is : 90.
탭키
int value of input character is : 9.
엔터키

int value of input character is : 10.
왼쪽화살표키
int value of input character is : 27.

int value of input character is : 91.

int value of input character is : 68.
ESC키
nt value of input character is : 27.
q
int value of input character is : 113.
```


**8) 터미널에서 키보드 입력을 받아보기 - 에코를 끄기**
```c
#include <stdio.h>
#include <sys/termios.h>

int main() {
   
  int stdin_fileno = fileno(stdin);
  struct termios terminal_setting;

  int error_no = tcgetattr(stdin_fileno, &terminal_setting);
  unsigned char ch_input;

  if (error_no) {
    printf("Error: tcgetattr, error_no is %d.\n", error_no);
    return 1;
  }
  printf("tcgetattr ... OK.\n");

  printf("terminal_setting_local_mode: 0x%04x.\n", terminal_setting.c_lflag);

  printf("terminal_setting_non_canonical_mode MIN value: %d.\n", terminal_setting.c_cc[VMIN]);

  printf("terminal_setting_non_canonical_mode TIME value: %d.\n", terminal_setting.c_cc[VTIME]);

  terminal_setting &= ~ICANON;

  terminal_setting &= ~ECHO; /* 에코에 해당하는 비트를 0으로 만든다. */

  error_no = tcsetattr(stdin_fileno, TCSANOW, &terminal_setting);
  if (error_no) {
    printf("Error: tcsetattr, error_no is %d.\n", error_no);
    return 1;
  }

  printf("tcsetattr ... OK.\n");

  do {
    scanf("%c", &ch_input);
    printf("\nint value of input character is : %d. \n", ch_input);
  while(ch_input != 'q');

  return 0;
}
```
실행 결과
```
tcgetattr ... OK.
terminal_setting_local_mode: 0x0d1d.
terminal_setting_non_canonical_mode MIN value: 1.
terminal_setting_non_canonical_mode TIME value: 0.
tcsetattr ... OK.
a키
int value of input character is : 97.
A키
int value of input character is : 122.
0키
int value of input character is : 48.
9키
int value of input character is : 57.
A키
int value of input character is : 65.
Z키
int value of input character is : 90.
탭키
int value of input character is : 9.
엔터키
int value of input character is : 10.
왼쪽화살표키
int value of input character is : 27.

int value of input character is : 91.

int value of input character is : 68.
ESC키
int value of input character is : 27.
q키
int value of input character is : 113.

```

**9) 터미널에서 키보드 키 입력 하나를 받는 함수 만들기**
```c
#include <stdio.h>
#include <sys/termios.h>

/* 터미널 셋팅을 읽어서 저장하는 함수 */
int get_terminal_setting(struct termios *pointer_to_setting) {
  int stdin_fileno = fileno(stdin);
  int error_no = tcgetattr(stdin_fileno, pointer_to_setting);

  if (error_no) {
    printf("Error: tcgetattr, error_no is %d.\n", error_no);
    return 1;
  }
  printf("tcgetattr ... OK.\n");

  printf("terminal_setting_local_mode: 0x%04x.\n", pointer_to_setting->c_lflag);

  printf("terminal_setting_non_canonical_mode MIN value: %d.\n", pointer_to_setting->c_cc[VMIN]);

  printf("terminal_setting_non_canonical_mode TIME value: %d.\n", pointer_to_setting->c_cc[VTIME]);

  return 0;
}  

/* 입력받은 셋팅값으로 터미널 셋팅을 업데이트하는 함수 */
int set_terminal_setting(struct termios *pointer_to_setting) {
  int stdin_fileno = fileno(stdin);
  int error_no = tcsetattr(stdin_fileno, TCSANOW, pointer_to_setting);
  if (error_no) {
    printf("Error: tcsetattr, error_no is %d.\n", error_no);
    return 1;
  }

  printf("tcsetattr ... OK.\n");

  return 0;
}  

/* 터미널에서 키보드 입력을 한바이트씩 에코 없이 받게 설정값을 바꾸는 함수 */
int setup_terminal() {
  struct termios terminal_setting;
  int error_no = get_terminal_setting(&terminal_setting);

  if (error_no) return 1;

  terminal_setting.c_lflag &= ~ICANON;
  terminal_setting.c_lflag &= ~ECHO;

  error_no = set_terminal_setting(&terminal_setting);

  if (error_no) return 1;
  else return 0;
}

/* 터미널에서 키보드 입력을 원래대로 받고, 에코도 다시 설정하는 함수 */
int restore_terminal() {
  struct termios terminal_setting;
  int error_no = get_terminal_setting(&terminal_setting);

  if (error_no) return 1;

  terminal_setting.c_lflag |= ICANON;
  terminal_setting.c_lflag |= ECHO;

  error_no = set_terminal_setting(&terminal_setting);

  if (error_no) return 1;
  else return 0;
}

/* 키보드 입력을 한바이트 받아서 unsigned char로 반환하는 함수. 주의: 어떤 키는 여러 바이트가 입력됨. */
unsigned char get_byte() {
  unsigned char ch_input;
  scanf("%c", &ch_input);
  return ch_input;
}

/* setup_terminal과 get_byte 테스트 */
int main() {
  unsigned char ch_input; /* 키보드 입력에서 한 바이트씩 읽어서 저장할 변수 */

  setup_terminal();     

  do {
    ch_input = get_byte();
    printf("\nint value of input character is : %d. \n", ch_input);
  } while(ch_input != 'q');

  restore_terminal();
  
  return 0;
}
```
실행결과는 8)과 동일

테스트 환경:

* GCC, MSYS2 MSYS에서 설치됨 (gcc), MSYS2의 터미널 (Mintty)
* ~~GCC, MSYS2 MSYS에서 설치됨 (gcc), VSCode에서 tasks.json을 변경해서 실행한 같은 창에서의 PowerShell 터미널~~
* GCC, MSYS2 MSYS에서 설치됨 (gcc), VSCode에서 tasks.json을 변경해서 실행한 새로운 창에서의 MSYS2 터미널

## 1. 콘솔에서 키보드 하나 입력받기 (Linux. C 라이브러리 사용 환경: glibc)
**터미널에서 키보드 키 입력 하나를 받는 함수 만들기**
```c
#include <stdio.h>
#include <termios.h>

/* read setting values of terminal. save to the structure pointed by pointer_to_setting */
int get_terminal_setting(struct termios *pointer_to_setting) {
  int stdin_fileno = fileno(stdin);
  int error_no = tcgetattr(stdin_fileno, pointer_to_setting);

  if (error_no) {
    printf("Error: tcgetattr, error_no is %d.\n", error_no);
    return 1;
  }
  printf("tcgetattr ... OK.\n");

  printf("terminal_setting_local_mode: 0x%04x.\n", pointer_to_setting->c_lflag);

  printf("terminal_setting_non_canonical_mode MIN value: %d.\n", pointer_to_setting->c_cc[VMIN]);

  printf("terminal_setting_non_canonical_mode TIME value: %d.\n", pointer_to_setting->c_cc[VTIME]);

  return 0;
}  

/* update terminal setting from the values saved in the structure pointed by pointer_to_setting */
int set_terminal_setting(struct termios *pointer_to_setting) {
  int stdin_fileno = fileno(stdin);
  int error_no = tcsetattr(stdin_fileno, TCSANOW, pointer_to_setting);
  if (error_no) {
    printf("Error: tcsetattr, error_no is %d.\n", error_no);
    return 1;
  }

  printf("tcsetattr ... OK.\n");

  return 0;
}  

/* update terminal setting to get keyboard input byte by byte without echo */
int setup_terminal() {
  struct termios terminal_setting;
  int error_no = get_terminal_setting(&terminal_setting);

  if (error_no) return 1;

  terminal_setting.c_lflag &= ~ICANON;
  terminal_setting.c_lflag &= ~ECHO;

  error_no = set_terminal_setting(&terminal_setting);

  if (error_no) return 1;
  else return 0;
}

/* restore original terminal setting, which gets keyboard input after enter and echo is on */
int restore_terminal() {
  struct termios terminal_setting;
  int error_no = get_terminal_setting(&terminal_setting);

  if (error_no) return 1;

  terminal_setting.c_lflag |= ICANON;
  terminal_setting.c_lflag |= ECHO;

  error_no = set_terminal_setting(&terminal_setting);

  if (error_no) return 1;
  else return 0;
}

/* get one byte from keyboard input and returns unsigned char. Caution: Some key results in multiple bytes */
unsigned char get_byte() {
  unsigned char ch_input;
  scanf("%c", &ch_input);
  return ch_input;
}

/* test setup_terminal() and get_byte() */
int main() {
  unsigned char ch_input; /* variable to hold input key value */

  setup_terminal();     

  do {
    ch_input = get_byte();
    printf("\nint value of input character is : %d. \n", ch_input);
  } while(ch_input != 'q');

  restore_terminal();
  
  return 0;
}
```
실행 결과
```
tcgetattr ... OK.
terminal_setting_local_mode: 0x0d1d.
terminal_setting_non_canonical_mode MIN value: 1.
terminal_setting_non_canonical_mode TIME value: 0.
tcsetattr ... OK.
a키
int value of input character is : 97.
A키
int value of input character is : 122.
0키
int value of input character is : 48.
9키
int value of input character is : 57.
A키
int value of input character is : 65.
Z키
int value of input character is : 90.
탭키
int value of input character is : 9.
엔터키
int value of input character is : 10.
왼쪽화살표키
int value of input character is : 27.

int value of input character is : 91.

int value of input character is : 68.
ESC키
int value of input character is : 27.
q키
int value of input character is : 113.

```
테스트 환경:

* GCC, WSL Ubuntu, VSCode에서 실행
* GCC, WSL Ubuntu, 명령어로 실행
* GCC, Redhat (VMware), 명령어로 실행
