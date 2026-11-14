# 3주차 — CPU와 데이터 표현

## 목표

- binary와 hexadecimal 표기를 서로 바꾸고 bitwise 연산 결과를 계산한다.
- signed/unsigned, two's complement, integer overflow의 서로 다른 규칙을 설명한다.
- endian과 alignment를 메모리 바이트와 주소 차이로 관찰한다.
- cache locality, branch, atomic operation을 과장 없이 C 코드와 연결한다.
- `SETBIT`, `GETBIT`, `COUNTBIT`의 첫 Bitmap 구현을 경고 없이 빌드하고 실행한다.

## 이 README를 사용하는 방법

각 실습은 **설명 읽기 → 코드 찾기 → 실행 전 예측 → 컴파일·실행 → 관찰 → 직접 수정 → 완료 확인** 순서다. 설명을 읽기 전에는 예측을 요구하지 않는다.

저장소 루트는 `/home/taek/git/valkey`라고 가정한다. 모든 C 코드는 다음 옵션으로 빌드한다.

```text
-std=c11 -Wall -Wextra -g
```

## 파일 구성

```text
learning/03/
├── bitmap.h   Bitmap 공개 타입과 함수 선언
├── bitmap.c   SETBIT, GETBIT, COUNTBIT 구현
├── main.c     바이트 순서, 범위, unsigned wrap, atomic 관찰
├── Makefile
└── README.md
```

## 먼저 알아둘 내용

CPU는 값을 bit(0 또는 1)의 묶음으로 다룬다. 8 bit는 1 byte다. binary의 네 자리 `1010`은 hexadecimal 한 자리 `a`이므로, `0x81`은 binary로 `1000 0001`이다. `&`는 둘 다 1인 bit만 남기고, `|`는 하나라도 1이면 1로 만들며, `^`는 서로 다를 때 1이다. `~`는 bit를 뒤집고, `<<`와 `>>`는 bit 위치를 옮긴다.

`uint8_t`는 정확히 8 bit인 **unsigned** 정수다. unsigned 연산의 결과는 해당 타입 범위를 넘으면 2의 거듭제곱을 법으로 정해져서 돌아간다. 이 자료의 `255 + 1`은 명시적으로 `uint8_t`로 변환되어 0이 된다. 반면 signed 정수 overflow는 C에서 정의되지 않은 동작이므로 `INT_MAX + 1`을 실험하지 않는다. two's complement는 오늘날 일반적인 signed 표현으로, 음수 `-x`를 bit 반전 뒤 1을 더해 표현하지만 C 프로그램은 signed overflow에 의존하면 안 된다.

endian은 **여러 byte로 된 값**을 메모리에 놓는 순서다. 한 byte 안의 bit 순서와는 별개다. 이 Bitmap은 endian과 무관하게 `bit 0 = bytes[0]의 최하위 bit`로 약속한다. `0x01020304`의 메모리 출력은 환경이 little-endian이면 `04 03 02 01`, big-endian이면 `01 02 03 04`다.

alignment는 어떤 타입의 객체가 주소 경계에 맞춰 놓이는 성질이다. CPU는 정렬된 접근을 더 쉽게 처리하는 경우가 많고, 구조체는 padding을 가질 수 있다. cache line은 cache가 보통 함께 가져오는 인접 메모리 묶음이며 크기는 CPU마다 다르다. 배열을 앞에서 뒤로 읽는 **spatial locality**, 방금 쓴 값을 다시 쓰는 **temporal locality**는 cache에 유리할 수 있다. 이는 성능 경향이지 모든 경우의 보장은 아니다.

branch는 `if`, loop 조건처럼 흐름을 고르는 명령이다. 데이터가 불규칙하면 CPU의 branch prediction이 빗나가 비용이 생길 수 있다. 먼저 명확한 코드를 쓰고 측정 결과가 있을 때만 branch를 바꾼다.

atomic operation은 여러 실행 흐름이 같은 객체를 다룰 때, 중간 상태 없이 한 번의 연산처럼 보이게 하는 연산이다. `atomic_uint counter`의 두 `fetch_add`는 counter 갱신에는 atomic이다. 그러나 이 예제 Bitmap은 atomic Bitmap이 아니며, 여러 thread가 같은 byte를 동시에 수정하면 별도의 atomic 설계나 lock이 필요하다. atomicity는 signed overflow나 undefined behavior를 고쳐 주는 성질이 아니다.

## 실습 1 — Bitmap의 bit 위치와 SETBIT/GETBIT

### 코드에서 찾기

1. `bitmap.h`의 bit 순서 주석을 읽는다.
2. `bitmap.c`의 `bitmap_bit_location`에서 `bit_offset / CHAR_BIT`와 `bit_offset % CHAR_BIT`를 찾는다.
3. `bitmap_setbit`의 `|=`와 `&= ~mask`, `bitmap_getbit`의 `& mask`를 찾는다.

### 이제 예측하기

1. 빈 `storage[2]`에서 bit 0, 7, 8을 모두 1로 만들면 각 byte는 hexadecimal로 무엇인지 적는다.
2. 그 뒤 bit 7을 0으로 만들면 `bitmap bytes:` 줄이 무엇일지 적는다.
3. bit offset 16이 거절되는 이유를 byte 수와 연결해 적는다.

### 컴파일·실행하기

```bash
make -C learning/03 clean all
./learning/03/bitmap_demo
```

### 관찰하고 연결하기

기본 출력의 `bitmap bytes`는 `01 01`, `count`는 2다. bit 7을 먼저 설정했다가 지우므로 첫 byte에는 bit 0만 남는다. offset 16은 2 byte Bitmap의 범위 `0..15` 밖이다.

### 직접 해보기

- `main.c`에서 bit 3과 bit 12를 설정한다고 가정하고, 실행 전에 두 byte와 count를 계산한다. 그 뒤 호출을 추가해 확인한다.
- `bitmap_setbit`에 value `2`를 전달하면 현재 API가 어떻게 해석하는지 읽고 설명한다. 이 API는 0만 clear, 0 이외는 set으로 정했다.
- `bitmap_countbit`의 `while`을 읽고 `0x81`이 왜 2개로 세어지는지 bit를 하나씩 적는다.

## 실습 2 — byte order, unsigned overflow, alignment

### 코드에서 찾기

1. `main.c`의 `word = 0x01020304`와 `print_bytes(&word, sizeof(word))`를 찾는다.
2. `unsigned_max + 1`을 `uint8_t`로 변환하는 줄을 찾는다.
3. `storage[0]`, `storage[1]` 주소와 포인터 뺄셈을 출력하는 줄을 찾는다.

### 이제 예측하기

1. 현재 컴퓨터가 little-endian이라면 word의 네 byte 순서를 적는다.
2. `uint8_t` 범위의 최댓값에 1을 더한 뒤 `uint8_t`로 바꾸면 무엇인지 적는다.
3. 인접한 `uint8_t` 두 원소의 주소 차이를 적는다.

### 컴파일·실행하기

실습 1의 실행 결과에서 다음 세 줄을 확인한다.

```text
word 0x01020304 stored as: ...
addresses: ... distance=1
uint8: 255 + 1 = 0
```

주소의 절대값은 매 실행마다 달라질 수 있다. `distance=1`만 비교한다. 이 결과만으로 모든 타입의 alignment를 알 수는 없다. 다음을 추가해 `uint32_t`의 주소 간격을 직접 관찰해도 좋다.

```c
uint32_t words[2] = {0, 0};
printf("uint32 distance=%td\n", &words[1] - &words[0]);
```

### 직접 해보기

- `0x81 & 0x0f`, `0x81 | 0x0f`, `0x81 ^ 0x0f`를 종이에 binary 네 묶음으로 먼저 계산한 뒤 출력 코드를 추가해 확인한다.
- signed overflow를 만들기 위해 `INT_MAX + 1` 코드를 추가하지 않는다. 왜 unsigned의 wrap과 달리 결과를 믿을 수 없는지 자신의 말로 설명한다.

## 실습 3 — locality, branch, atomic의 범위

### 코드에서 찾기

1. `bitmap_countbit`의 `for` loop가 byte 배열을 앞에서 뒤로 읽는 것을 찾는다.
2. 같은 함수의 `while (byte != 0)`가 branch인 것을 찾는다.
3. `main.c`의 `atomic_fetch_add_explicit`와 `atomic_load_explicit`을 찾는다.

### 이제 예측하기

1. count가 같은데 한 Bitmap은 `0xff` 한 byte에 몰려 있고 다른 Bitmap은 여러 byte에 흩어져 있다면, `while` 반복 횟수는 어떻게 다른지 적는다.
2. `atomic_fetch_add`를 두 번 호출한 뒤 counter 값이 무엇인지 적는다.
3. atomic counter가 있다고 해서 `storage`를 동시에 수정해도 안전한지 예측한다.

### 컴파일·실행하고 관찰하기

기본 실행의 마지막 줄은 `atomic counter=2`다. 이것은 counter에 대한 두 증가의 원자성만 보여 준다. thread를 만들지 않은 이 프로그램은 data race를 재현하거나 thread 안전성을 증명하지 않는다.

### 직접 해보기

- `bitmap_countbit`의 `count += byte & 1`과 `byte >>= 1`이 각각 무엇을 하는지 주석으로 자신의 말로 작성한 뒤, 이해가 끝나면 주석을 유지하거나 제거한다.
- byte를 순서대로 읽는 코드와 멀리 떨어진 index를 읽는 코드를 섣불리 “빠르다/느리다”로 단정하지 말고, cache line 크기·CPU·입력 크기·측정 방법이 필요하다고 설명한다.

## 완료 조건

- [ ] binary 네 자리와 hexadecimal 한 자리가 대응함을 사용해 bitwise 결과를 계산했다.
- [ ] signed/unsigned와 two's complement를 설명하고, signed overflow를 unsigned wrap과 혼동하지 않는다.
- [ ] 실행 출력에서 endian과 인접 byte의 주소 간격을 확인했다.
- [ ] alignment, cache locality, branch를 성능 보장이 아닌 기초 개념으로 설명할 수 있다.
- [ ] atomic operation의 범위와 Bitmap의 비원자성을 구분할 수 있다.
- [ ] `SETBIT`, `GETBIT`, `COUNTBIT`을 실행했고 bit 순서와 범위 검사를 설명할 수 있다.
