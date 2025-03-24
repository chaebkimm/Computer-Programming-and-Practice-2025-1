/* 배열 타입의 선언 */

/* 문법
 * 타입 이름 [ 식 ] ;
 * 타입 이름 [ 식 ] = { 초기값1 , 초기값2 , }
 * 식은 배열에 해당 타입이 몇개가 있는지를 정하는 식으로, 계산 결과 상수여야 합니다.
 * 초기 값의 개수는 배열 크기와 같거나 더 작아야 합니다.
 *
 * 의미
 * 배열은 메모리에 같은 타입 여러개를 연달아 저장할 수 있는 타입입니다.  
 * 배열에 초기값을 정하려면 { } 안에 초기값을 쉼표로 분리해서 적습니다. 
 */

unsigned int a = 0xaaaaaaaa;
unsigned int b = 0xbbbbbbbb;
unsigned int array[2] = {0xab, 0xc0}; /* To Do: int 배열 array의 크기를 적절하게 바꿔서 메모리 창에서 a와 c, b와 d가 같은 세로줄에 있게 만들어보세요 */
unsigned int c = 0xcccccccc;
unsigned int d = 0xdddddddd;

int main() {
  return 0;
}
