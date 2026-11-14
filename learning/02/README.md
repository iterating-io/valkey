# 2주차 — C 시스템 프로그래밍

## 목표

- `static`과 `extern`이 이름의 가시성과 linkage에 미치는 영향을 설명한다.
- `const`, `volatile`, `restrict`의 역할과 한계를 구분한다.
- 함수 포인터를 callback으로 전달할 수 있다.
- macro와 conditional compilation이 전처리 단계에서 어떻게 동작하는지 확인한다.
- header/source 분리, object file, linker, static library의 관계를 명령으로 관찰한다.
- 작은 정수 배열 라이브러리의 첫 구성 요소를 경고 없이 빌드하고 실행한다.

## 이 README를 사용하는 방법

각 실습은 다음 순서로 진행한다.

1. **기초 설명 읽기**: 처음 나오는 용어와 코드의 역할을 먼저 확인한다.
2. **코드 연결하기**: 설명에 해당하는 선언·정의·호출을 파일에서 찾는다.
3. **예측하기**: 이제 실행 전 출력과 `length`/`capacity`, callback 호출 횟수를 적는다.
4. **컴파일·실행하기**: 경고를 확인한 뒤 실행한다. 경고를 무시하지 않는다.
5. **관찰하기**: 예측과 실제 결과가 다른 이유를 코드와 연결한다.
6. **직접 수정하기**: `직접 해보기`의 한 가지 변경만 적용하고 다시 설명·예측·실행한다.
7. **완료 확인**: 완료 조건을 자신의 말로 설명할 수 있는지 점검한다.

저장소 루트는 `/home/taek/git/valkey`라고 가정한다. 모든 C 코드는 다음 옵션을 사용한다.

```text
-std=c11 -Wall -Wextra -g
```

## 파일 구성

```text
learning/02/
├── array.h       공개 타입·함수 선언, macro, extern 선언
├── array.c       라이브러리 구현, static helper, extern 정의
├── main.c        사용 예제, callback, const/restrict 관찰
├── Makefile       compile → object → link 과정을 자동화
└── README.md
```

## 시작 전에: 이번 주 도구와 용어

지난주에는 한 파일 안에서 C 코드를 컴파일했다. 이번 주에는 코드를 여러 파일로 나누고, 컴파일러와 linker가 각 파일을 어떻게 하나의 프로그램으로 만드는지 본다.

- **선언(declaration)**: “이 이름과 타입이 존재한다”고 알린다. 예를 들어 `int array_push(...);`만으로도 다른 파일이 함수를 호출할 수 있다.
- **정의(definition)**: 실제 함수 본문이나 전역 객체의 저장 공간을 제공한다. 함수 본문 `{ ... }`과 `const char array_library_name[] = ...`이 정의다.
- **translation unit**: 한 `.c` 파일과 그 파일에 `#include`된 내용을 전처리한 단위다. `array.c`와 `main.c`는 보통 서로 다른 translation unit이다.
- **header**: 여러 `.c` 파일이 함께 사용할 선언을 모아 둔 `.h` 파일이다. `#include "array.h"`는 header 내용을 그 위치에 펼친다.
- **object file**: `.c`를 컴파일했지만 아직 다른 파일의 함수와 연결되지 않은 `.o` 파일이다.
- **linker**: 여러 `.o` 파일에서 선언만 보이던 이름과 실제 정의를 연결해 실행 파일을 만든다.
- **linkage**: 같은 이름이 다른 translation unit에서도 같은 대상을 가리킬 수 있는지에 대한 규칙이다. 파일 내부에서만 쓰는 `static` 함수와 여러 파일에서 연결되는 외부 함수가 그 차이를 보여 준다.

### 작은 라이브러리의 실행 흐름

`main.c`는 `array.h`를 읽고 `array_push`를 호출한다. 함수 본문은 `array.c`에 있다. 두 파일을 각각 컴파일하면 `main.o`에는 “`array_push`가 어딘가에 있다”는 미해결 참조가 남고, linker가 `array.o`의 정의와 연결한다.

```text
array.c + array.h ──compile──> array.o ─┐
                                        ├─link──> array_demo
main.c  + array.h ──compile──> main.o  ─┘
```

## 실습 1 — header/source와 linkage 읽기

파일: `array.h`, `array.c`, `main.c`

### 먼저 알아둘 내용

`array.h`는 라이브러리 사용자에게 공개하는 “사용 설명서”와 비슷하다. 함수의 본문까지 header에 넣는 대신 이름, 매개변수 타입, 반환 타입만 선언한다. 그러면 `main.c`는 구현을 몰라도 올바르게 호출할 수 있고, 구현은 `array.c`에서 바꿀 수 있다.

`array.c`의 `array_grow` 앞에 붙은 `static`은 이 함수의 이름을 `array.c` 안에서만 사용할 수 있게 한다. 반대로 `array_push`에는 `static`이 없으므로 header의 선언과 연결되어 다른 파일에서 호출할 수 있다. `extern`은 “이 전역 객체의 정의는 다른 곳에 있다”는 선언이다. 실제 정의가 하나는 있어야 linker가 프로그램을 완성할 수 있다.

이번 주에 함께 보는 타입 한정자도 먼저 구분해 둔다. `const int *source`는 이 포인터를 통해 source 원소를 바꾸지 않겠다는 뜻이다. `int *restrict destination`과 `const int *restrict source`는 두 포인터가 같은 배열을 겹쳐 가리키지 않는다는 약속으로, 컴파일러가 더 자유롭게 최적화할 수 있게 한다. `volatile`은 하드웨어 레지스터처럼 프로그램 밖에서 값이 바뀔 수 있는 객체의 접근을 컴파일러가 임의로 생략하지 않도록 할 때 쓰며, 이 예제에는 직접 사용하지 않는다. `volatile`은 여러 thread의 실행 순서를 안전하게 맞추는 도구가 아니다.

### 읽고 찾기

1. `array.h`에서 `array_push` 선언과 `extern array_library_name` 선언을 찾는다.
2. `array.c`에서 `array_push`와 `array_library_name`의 정의를 찾는다.
3. `array.c`의 `static array_grow`를 찾고, `main.c`에는 이 함수의 선언이나 호출이 없는 것을 확인한다.

### 이제 예측하기

1. `array.h`에 함수 본문이 없는 상태에서도 `main.c`가 컴파일될 수 있는 이유를 적는다.
2. `static array_grow`를 `main.c`에서 호출하려고 하면 어떤 단계에서 문제가 생길지 예측한다.
3. 실행 파일의 첫 줄이 `library=learning-02-array`가 되는 이유를 설명한다.

### 컴파일·실행하기

4. 아래 명령으로 실행한다.

   ```bash
   make -C learning/02
   ./learning/02/array_demo
   ```

5. 결과를 예측과 비교한다. `array.c`의 `static`을 제거해도 현재 출력은 같지만 이름의 공개 범위가 어떻게 달라지는지 설명한다. 수정은 확인 후 원상 복구한다.

### 핵심 개념

- header는 여러 translation unit이 공유할 선언과 공개 계약을 둔다.
- source는 그 계약의 구현을 둔다.
- `static` 함수는 해당 source 파일 안에서만 이름을 사용할 수 있다.
- `extern` 선언은 다른 translation unit에 정의가 있음을 알린다. 선언만 있고 정의가 없으면 linker 오류가 난다.
- `const`는 해당 식을 통한 변경을 막는 한정자이지, 객체가 항상 물리적으로 read-only라는 뜻은 아니다.
- `volatile`은 외부 요인으로 값이 바뀔 수 있어 접근을 생략하거나 합치는 최적화를 제한한다. thread 동기화 도구는 아니다.
- `restrict`는 해당 포인터를 통한 접근이 별칭 규칙을 지킨다는 호출자와의 약속이며, 위반하면 정의되지 않은 동작이 될 수 있다.

## 실습 2 — callback, macro, conditional compilation

### 먼저 알아둘 내용

함수도 메모리에 있는 코드이므로 함수의 주소를 포인터에 저장할 수 있다. `array_foreach`의 `callback` 매개변수는 `int value, size_t index, void *context`를 받는 함수의 주소다. `array_foreach`는 배열 원소마다 그 주소를 호출한다. 따라서 배열 순회 함수는 “각 원소에서 무엇을 할지”를 직접 알 필요가 없고, 호출자가 callback으로 동작을 주입할 수 있다.

`#define`은 전처리기가 이름을 정해진 문자나 식으로 바꾸게 하는 macro다. `ARRAY_INITIAL_CAPACITY`는 배열의 초기 공간을 정하는 숫자 macro이지, 현재 배열에 들어 있는 원소 수가 아니다. 현재 원소 수는 `length`, 확보한 공간의 크기는 `capacity`다.

`#ifdef ARRAY_TRACE`는 컴파일 명령에 `-DARRAY_TRACE`가 있을 때만 그 사이의 코드를 C 코드에 포함한다. 실행 중에 조건을 검사하는 `if`와 달리, 이 선택은 실행 파일을 만들기 전에 일어난다.

### 읽고 찾기

1. `main.c`에서 `print_item`과 `add_item`의 함수 타입을 확인한다.
2. `array.h`에서 `array_foreach`의 callback 매개변수를 확인한다.
3. `array.c`에서 callback이 실제로 호출되는 줄을 찾는다.
4. `array.c`에서 `#ifdef ARRAY_TRACE`와 `#endif` 사이의 코드를 찾는다.

### 이제 예측하기

5. `array_foreach(&array, print_item, NULL)`이 `print_item`을 몇 번 호출할지 적는다.
6. 원소가 5개이고 초기 capacity가 4일 때 최종 capacity를 적는다.
7. `add_item` callback 이후 `sum`의 값을 계산한다.
8. trace 없이 실행했을 때와 trace를 켰을 때 출력이 어느 stream에서 달라질지 예측한다.

### 컴파일·실행하기

9. 먼저 기본 빌드와 실행을 한다.

   ```bash
   make -C learning/02 clean all
   ./learning/02/array_demo
   ```

10. trace를 켜서 다시 빌드하고 차이를 관찰한다.

   ```bash
   make -C learning/02 clean
   make -C learning/02 CFLAGS='-std=c11 -Wall -Wextra -g -DARRAY_TRACE'
   ./learning/02/array_demo
   ```

`ARRAY_TRACE`는 전처리 때 `array.c`에 trace 코드를 포함시킨다. 따라서 trace는 runtime 변수라기보다 compile-time 선택이다.

### 직접 해보기

- `array_push` 호출을 3번, 4번, 5번으로 바꾸기 전에 `length`와 `capacity`를 먼저 계산한다.
- callback을 바꿔 합계를 계산하고, 합계가 `0 + 10 + 20 + 30 + 40`인지 확인한다.
- `array_get`에 범위를 벗어난 index를 전달하고 반환값이 `0`인지 확인한다. 실패할 때 출력 변수는 바뀌지 않아야 한다.
- `array.h`의 `ARRAY_INITIAL_CAPACITY`를 바꾼 뒤 clean build하고 capacity 변화를 설명한다.

## 실습 3 — gcc, clang, object file, linker, library

### 먼저 알아둘 내용

`cc -c array.c -o array.o`의 `-c`는 linker를 아직 실행하지 말고 컴파일 결과인 object file만 만들라는 뜻이다. `array.o`에는 `array_init` 같은 함수의 기계어가 있지만, 그 자체는 보통 실행할 수 없다. 마지막 link 단계에서 `main.o`와 함께 실행 파일이 된다.

static library인 `.a`는 여러 object file을 묶어 둔 archive다. 여기서는 `ar rcs`가 `array.o`를 `libarray.a`에 넣는다. 이후 linker가 `main.o`에서 필요한 심볼을 library에서 찾아 연결한다.

### 읽고 관찰하기

Makefile에서 `array.o`, `main.o`, `array_demo`, `libarray.a`가 각각 어떤 명령으로 만들어지는지 먼저 표시한다. 그 다음 아래 명령을 단계별로 실행한다.

```bash
make -C learning/02 clean
make -C learning/02 array.o main.o
nm learning/02/array.o
cc -std=c11 -Wall -Wextra -g learning/02/array.o learning/02/main.o -o learning/02/manual_demo
./learning/02/manual_demo
```

`array.o`와 `main.o`는 아직 실행 파일이 아니며, 마지막 `cc` 명령의 linker가 두 object file의 참조를 연결한다. static library도 확인한다.

```bash
make -C learning/02 libarray.a
cc -std=c11 -Wall -Wextra -g learning/02/main.o learning/02/libarray.a -o learning/02/library_demo
./learning/02/library_demo
```

가능하면 `make -C learning/02 CC=clang clean all`도 실행해 gcc와 clang의 결과를 비교한다. 컴파일러가 다르더라도 표준을 지킨 코드의 관찰 가능한 결과는 같아야 한다.

## 완료 조건

- [ ] `static` 함수가 다른 source 파일에서 보이지 않는 이유를 설명할 수 있다.
- [ ] `extern` 선언과 실제 정의, 그리고 linker의 역할을 설명할 수 있다.
- [ ] `const`, `volatile`, `restrict`를 각각 언제 쓰는지와 서로 대체할 수 없는 이유를 말할 수 있다.
- [ ] 함수 포인터 callback이 호출되는 흐름을 코드로 설명할 수 있다.
- [ ] macro와 `#ifdef ARRAY_TRACE`가 전처리 단계에서 선택되는 것을 확인했다.
- [ ] `.c → .o → 실행 파일`과 `.a` library의 관계를 설명할 수 있다.
- [ ] Makefile로 경고 없이 빌드하고 실행했다.
