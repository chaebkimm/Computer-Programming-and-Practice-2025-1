/* 매개변수리스트가 있는 함수 선언 */

/* 문법
 * 함수 선언에서 매개변수 정보를 쓰려면 함수의반환타입 함수이름 ( 매개변수타입 매개변수이름 ) ; 형식으로 선언합니다. 
 * 매개변수를 여러개 쓸 때는 ,로 구분합니다.
 * 매개변수가 없음을 의미할때는 매개변수이름은 없이 void라고만 씁니다.  
 *
 * 의미
 * 해당 이름이 특정 반환타입의 함수를 뜻한다고 정하고, 매개변수의 개수와 각각의 타입도 정합니다. 
 */

/* To Do: 반환 타입이 int이고 이름이 value이고, 매개변수가 int 타입 하나인 함수를 선언해보세요 */
int value(int number);
int main() {
	return value(5);
}

/* 
 * 함수 이름: value
 * 반환 타입: int
 * 매개 변수: int x
 *
 * value 함수는 int 타입 매개 변수의 값을 그대로 반환한다.
 */
int value(int x) {
	return x;
}
