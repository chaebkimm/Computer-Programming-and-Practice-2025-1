# PNG 파일 만들기

변경사항: 
* fopen_s가 visual studio 외의 환경에서 에러를 발생시켜 fopen으로 복구했습니다. 
* visual studio의 sln 파일에서 _CRT_SECURE_NO_WARNINGS 전처리기를 추가해서 fopen을 써도 에러가 없게 만들었습니다.
