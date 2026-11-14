# 1주차 — C 메모리 모델

## 목표

- 포인터와 포인터 연산을 설명할 수 있다.
- 배열과 `struct`의 메모리 배치를 관찰할 수 있다.
- `malloc`/`free`의 기본 사용법과 ownership을 설명할 수 있다.
- 작은 Dynamic Array를 직접 구현할 준비를 한다.

## 이 README를 사용하는 방법

각 실습을 읽기만 하지 말고 다음 순서로 진행한다.

1. **읽기**: 실습의 목표와 핵심 개념을 먼저 읽는다.
2. **예측하기**: 코드를 실행하기 전에 출력, 주소 관계, `length`와 `capacity`의 변화를 종이에 적는다.
3. **실행하기**: 저장소 루트에서 제공된 컴파일 명령을 그대로 실행한다. 경고가 나오면 그냥 넘어가지 않는다.
4. **관찰하기**: 실제 출력과 예측을 비교하고, 주소처럼 매번 달라질 수 있는 값과 항상 유지되어야 하는 관계를 구분한다.
5. **코드 읽기**: 출력에 해당하는 코드와 메모리의 lifetime, ownership이 바뀌는 지점을 찾는다.
6. **질문에 답하기**: 아래 질문에 자신의 말로 답한 뒤 `답변 확인`과 비교한다.
7. **완료 확인**: 실습의 완료 조건을 모두 설명할 수 있을 때 다음 실습으로 넘어간다.

### 준비 사항

- C 컴파일러(`cc` 또는 `gcc`)
- 터미널
- 저장소 루트: `/home/taek/git/valkey`

모든 예제는 다음 공통 옵션으로 컴파일한다.

```bash
-std=c11 -Wall -Wextra -g
```

`-std=c11`은 사용할 C 표준을 지정하고, `-Wall -Wextra`는 실수를 찾기 위한 경고를 켜며, `-g`는 디버거에서 소스 위치를 볼 수 있게 한다. 컴파일 명령에서 경고가 발생하면 먼저 원인을 읽고 해결한다.

## 실습 순서

| 순서 | 주제 | 파일 | 이번 실습의 결과 |
| --- | --- | --- | --- |
| 1 | 포인터와 배열 | `pointer_probe.c` | 배열 이름, 포인터, `sizeof`의 차이를 설명한다. |
| 2 | Heap 메모리 | `memory_probe.c` | `malloc`으로 얻은 메모리의 ownership과 `free` 시점을 설명한다. |
| 3 | Dynamic Array | `dynamic_array.c` | `length`, `capacity`, `realloc`을 사용한 배열을 구현한다. |

## 실습 1 — 포인터와 배열 관찰

파일: `pointer_probe.c`

저장소 루트에서 실행:

```bash
cc -std=c11 -Wall -Wextra -g learning/01/pointer_probe.c -o learning/01/pointer_probe
./learning/01/pointer_probe
```

주소 출력은 실행할 때마다 달라져도 정상이다. 중요한 것은 주소 사이의 관계와 각 값의 크기다.

### 포인터·배열·포인터 연산

### 핵심 용어

- `address`: 객체가 메모리에 놓인 위치다. `&x`는 `x`의 주소를 구한다.
- `object`: 값을 저장하는 메모리 영역이다. 변수와 배열 원소가 객체다.
- `lvalue`: 객체를 식별하는 표현식이다. 따라서 주소를 구하거나 값을 저장할 수 있다.
- `pointer`: 다른 객체의 주소를 저장하는 객체다. `int *p = &x;`처럼 선언한다.
- `dereference`: 포인터가 가리키는 객체에 접근하는 연산이다. `*p`는 주소가 아니라 그 객체의 값이다.
- `pointer arithmetic`: 포인터 연산은 가리키는 타입의 크기를 단위로 이동한다. `p + 1`은 `sizeof(*p)` 바이트 뒤를 가리킨다.
- `array-to-pointer conversion`: 대부분의 식에서 배열 이름은 첫 원소를 가리키는 포인터로 변환된다. 단, `sizeof(array)`와 `&array`에서는 배열 전체 정보가 유지된다.

예를 들어 `int values[3]`에서 `values`, `&values[0]`, `values + 0`은 첫 원소를 가리키며, `*(values + 1)`은 `values[1]`과 같다. `&values[2]`와 `values + 2`도 같은 주소다.

배열을 함수 인자로 전달하면 함수 매개변수의 배열 선언은 포인터로 조정된다. 따라서 배열 본문에서의 `sizeof(values)`는 전체 배열 크기지만, `int *values` 매개변수 안의 `sizeof(values)`는 포인터 크기다. 포인터 크기가 배열 전체 크기와 항상 같은 것은 아니다.

배열의 유효한 인덱스 범위를 벗어나 읽는 것은 메모리가 부족해지는 OOM이 아니라 정의되지 않은 동작(undefined behavior)이다. 잘못된 값을 읽거나, 충돌하거나, 겉보기에는 정상처럼 동작할 수 있다.

### 답변 확인

1. `&x`는 `x`의 주소이고, `*p`는 `p`가 가리키는 `x`의 값이다.
2. `values + 1`은 `int` 한 개 크기만큼 이동한다.
3. `&values[2]`와 `values + 2`는 같은 주소를 가리킨다.
4. 배열 식의 `sizeof(values)`는 전체 배열 크기지만, 포인터 매개변수의 `sizeof(values)`는 포인터 크기다.
5. `*(p + 1)`은 다음 원소의 값에 접근한다.
6. 범위를 벗어난 접근은 정의되지 않은 동작이다.

관찰 실습은 `pointer_probe.c`에서 수행한다.

### 실습 1 진행 순서

1. `pointer_probe.c`를 열고 `values`, `p`, `inspect_array`를 먼저 읽는다.
2. 실행 전 다음을 예측한다.
   - `sizeof(values)`와 `sizeof(values[0])`의 관계
   - `*p`, `*(p + 1)`의 값
   - 배열 바깥에서 `sizeof(values)`를 계산할 때와 함수 안에서 계산할 때의 차이
3. 아래 명령으로 컴파일하고 실행한다.

   ```bash
   cc -std=c11 -Wall -Wextra -g learning/01/pointer_probe.c -o learning/01/pointer_probe
   ./learning/01/pointer_probe
   ```

4. 주소의 숫자 자체를 외우지 말고, 인접한 `int` 원소의 주소 차이가 `sizeof(int)`인지를 확인한다.
5. `inspect_array` 안에서 배열이 포인터로 전달되는 이유를 코드와 연결해 설명한다.

### 실습 1 완료 조건

- `values`, `&values[0]`, `values + 0`이 무엇을 가리키는지 설명할 수 있다.
- `p + 1`이 1바이트가 아니라 `sizeof(*p)`만큼 이동하는 이유를 설명할 수 있다.
- 배열의 `sizeof`와 포인터의 `sizeof`가 다른 이유를 설명할 수 있다.
- 범위를 벗어난 배열 접근이 왜 정의되지 않은 동작인지 설명할 수 있다.

## 실습 2 — `malloc`/`free`, ownership, lifetime

파일: `memory_probe.c`

저장소 루트에서 실행:

```bash
cc -std=c11 -Wall -Wextra -g learning/01/memory_probe.c -o learning/01/memory_probe
./learning/01/memory_probe
```

출력 결과를 바탕으로 `memory_probe.c` 안의 질문에 먼저 답한다.

### 제출 기준

- 파일 안의 질문에 자신의 말로 답한다.
- `sizeof` 결과를 운영체제나 컴파일러가 정한 ABI와 연결해 설명한다.
- `struct`의 필드 사이에 빈 공간이 생길 수 있는 이유를 설명한다.
- `malloc`으로 얻은 메모리의 ownership과 `free` 시점을 표시한다.

### 핵심 개념

- `malloc(bytes)`는 heap에서 `bytes`만큼의 저장 공간을 예약하고 그 시작 주소를 반환한다. 초기화는 하지 않으며, 실패하면 `NULL`을 반환한다.
- `free(pointer)`는 그 `malloc` 계열 호출이 예약한 공간을 다시 사용할 수 있게 한다. 같은 포인터를 두 번 `free`하거나, 이미 해제한 공간을 사용하면 정의되지 않은 동작이다.
- `ownership`은 누가 할당한 공간을 해제할 책임을 갖는지에 대한 규칙이다. 아래 동적 배열에서는 배열 객체가 `data`가 가리키는 버퍼를 소유하므로 `dynamic_array_destroy`가 해제한다.
- `lifetime`은 객체를 유효하게 사용할 수 있는 기간이다. `malloc`으로 얻은 버퍼의 lifetime은 할당 성공 후 시작하고 `free`에서 끝난다. `realloc`이 이동하면 이전 주소는 더 이상 사용하면 안 된다.

### ownership 예시

ownership은 포인터가 어디에 들어 있는지가 아니라, **누가 마지막 `free`를 호출할 책임이 있는지**다.

#### 예시 1 — 만든 쪽이 직접 해제한다

```c
int *number = malloc(sizeof(*number));

if (number == NULL) {
    return 1;
}

*number = 42;
free(number); /* number가 가리킨 메모리를 만든 현재 함수가 해제한다. */
```

`number`가 이 블록의 소유자다. `free(number)` 뒤에는 `*number`를 읽거나 쓰면 안 된다.

#### 예시 2 — 포인터를 복사해도 소유자는 늘지 않는다

```c
int *number = malloc(sizeof(*number));
int *borrowed;

if (number == NULL) {
    return 1;
}

borrowed = number;
*borrowed = 42;
free(number); /* 소유자인 number 쪽의 책임으로 한 번만 해제한다. */
```

`borrowed`와 `number`는 같은 메모리를 가리키지만, `borrowed`는 잠시 사용하는 포인터일 뿐이다. `free(borrowed)`를 추가로 호출하면 double free가 된다.

#### 예시 3 — 소유권을 구조체로 넘긴다

```c
DynamicArray array;

if (!dynamic_array_init(&array)) {
    return 1;
}

dynamic_array_push(&array, 42);
dynamic_array_destroy(&array); /* array가 data 버퍼를 해제한다. */
```

`dynamic_array_init`이 버퍼를 만들지만, 성공 후에는 `array.data`가 그 주소를 보관하고 `DynamicArray`가 해제 책임을 이어받는다. 따라서 호출자는 `free(array.data)`를 따로 호출하지 않는다.

#### 예시 4 — 함수가 새 메모리를 반환하면 호출자가 책임을 받는다

```c
static int *make_number(void) {
    int *number = malloc(sizeof(*number));

    if (number != NULL) {
        *number = 42;
    }
    return number;
}

int *number = make_number();
if (number == NULL) {
    return 1;
}
free(number); /* make_number에서 호출한 malloc의 책임이 호출자에게 넘어왔다. */
```

이처럼 할당한 함수와 해제하는 함수가 달라도 된다. 중요한 것은 반환 규약으로 ownership이 호출자에게 이전되었고, 최종적으로 한 곳에서 정확히 한 번 `free`한다는 점이다.

### length와 capacity

동적 배열은 다음 세 가지 상태를 함께 관리한다.

- `data`: 정수 원소가 저장된 heap 버퍼의 주소
- `length`: 현재 저장된 원소 수
- `capacity`: 재할당 없이 저장할 수 있는 최대 원소 수

`length`는 실제 데이터의 크기이고 `capacity`는 확보한 공간의 크기다. 원소를 추가해 `length == capacity`가 되면 더 큰 버퍼를 `realloc`으로 확보한 뒤 `capacity`를 늘린다. `realloc`이 실패할 수 있으므로 성공 여부를 확인하기 전에는 기존 포인터를 덮어쓰지 않는다.

### 실습 2 진행 순서

1. `memory_probe.c`에서 `heap_value`가 선언되고, 할당되고, 사용되고, 해제되는 순서를 표시한다.
2. 실행 전에 다음 질문에 답을 적는다.
   - `sizeof(values)`와 `sizeof(heap_value)` 중 어느 것이 배열 전체 크기인가?
   - `struct Layout`의 크기가 필드 크기의 합보다 클 수 있는 이유는 무엇인가?
   - `heap_value`를 누가 소유하며, 어느 줄에서 lifetime이 끝나는가?
3. 아래 명령으로 실행한다.

   ```bash
   cc -std=c11 -Wall -Wextra -g learning/01/memory_probe.c -o learning/01/memory_probe
   ./learning/01/memory_probe
   ```

4. 출력의 크기는 현재 ABI에 따른 결과로 기록하고, 주소처럼 실행마다 바뀔 수 있는 값과 구분한다.
5. 코드의 `free(heap_value)`를 찾은 뒤, 그 줄 이후에 `*heap_value`를 읽으면 안 되는 이유를 설명한다.

### 실습 2 완료 조건

- `malloc` 실패 시 `NULL`을 확인해야 하는 이유를 설명할 수 있다.
- heap 버퍼의 ownership을 누가 갖는지와 `free` 책임을 말할 수 있다.
- `free` 이후 포인터를 사용하면 안 되는 이유를 설명할 수 있다.
- struct padding과 배열 범위 초과가 서로 다른 문제임을 구분할 수 있다.

## 실습 3 — 작은 정수 Dynamic Array 구현

파일: `dynamic_array.c`

실습 2에서 배운 개념을 사용해 작은 정수 동적 배열을 구현한다. 다음 명령으로 컴파일하고 실행한다.

```bash
cc -std=c11 -Wall -Wextra -g learning/01/dynamic_array.c -o learning/01/dynamic_array
./learning/01/dynamic_array
```

이 실습에서 **처음에는 코드를 새로 작성하지 않는다.** `dynamic_array.c`를 읽고 실행하면서 동적 배열이 어떻게 동작하는지 확인한다.

- `dynamic_array_push(&array, value)`는 **정수 `value` 하나를 배열의 끝에 추가하는 함수**다.
- 공간이 남아 있으면 값을 저장하고 `length`를 1 늘린다.
- 공간이 없으면 버퍼를 더 크게 만든 뒤 값을 저장한다.
- 성공하면 `1`, 메모리를 늘리지 못하면 `0`을 반환한다.

`main`은 배열을 초기화하고 `dynamic_array_push`를 열 번 호출한 뒤, 마지막에 `dynamic_array_destroy`로 버퍼를 정확히 한 번 해제한다.

### 실습 3 진행 순서

1. `dynamic_array.c`를 열고 `DynamicArray`, `dynamic_array_init`, `dynamic_array_push`, `dynamic_array_destroy`, `main`을 찾는다. **수정하지 말고** 각각의 역할을 한 줄로 적는다.
2. `DynamicArray`의 `data`, `length`, `capacity`가 각각 무엇을 의미하는지 적는다.
3. `main`의 반복문을 읽는다. `dynamic_array_push(&array, (int)(i * 10))`는 `0, 10, 20, ... 90`을 차례대로 추가한다.
4. `INITIAL_CAPACITY`가 4이므로, 4번째·5번째·9번째 값을 넣은 직후의 `length`와 `capacity`를 예측한다.
5. 아래 명령으로 컴파일하고 실행한다.

   ```bash
   cc -std=c11 -Wall -Wextra -g learning/01/dynamic_array.c -o learning/01/dynamic_array
   ./learning/01/dynamic_array
   ```

6. 출력이 `length=10 capacity=16`인지 확인한다. 원소를 추가할 때 capacity는 `4 → 8 → 16`으로 늘어난다.
7. 이제 `dynamic_array_push` 함수 본문을 읽는다. `length == capacity`일 때만 `realloc`하는 이유와, `new_data` 임시 포인터를 쓰는 이유를 설명한다.
8. 마지막 `dynamic_array_destroy`가 ownership과 lifetime을 어떻게 마무리하는지 확인한다.

### 직접 해보기

- `main`의 반복 조건을 `i < 3`, `i < 4`, `i < 5`로 하나씩 바꾸고 `length`와 `capacity`를 예측한 뒤 실행한다.
- `dynamic_array_push`가 호출될 때마다 `length`와 `capacity`를 출력해, 언제 배열이 커지는지 확인한다.
- 다음 단계로 `dynamic_array_get` 함수를 직접 작성한다. `dynamic_array_print`가 이미 이 함수를 호출하도록 작성되어 있다.

  ```c
  static int dynamic_array_get(const DynamicArray *array, size_t index,
                               int *value);
  ```

  `index < array->length`이면 `array->data[index]`를 `*value`에 저장하고 `1`을 반환한다. 그렇지 않으면 `value`를 건드리지 않고 `0`을 반환한다. 함수 본문을 완성한 뒤 다시 컴파일하고 실행한다.

### 실습 3 완료 조건

- `length`와 `capacity`의 차이를 예시와 함께 설명할 수 있다.
- capacity가 부족할 때만 재할당하는 이유를 설명할 수 있다.
- `realloc` 실패 시 기존 버퍼를 보존하는 코드 흐름을 설명할 수 있다.
- 배열의 소유자가 마지막에 정확히 한 번 버퍼를 해제하는 구조를 설명할 수 있다.

## 전체 완료 체크리스트

- [ ] 세 파일을 모두 직접 컴파일하고 실행했다.
- [ ] 실행 전에 결과를 예측하고 실제 결과와 비교했다.
- [ ] 포인터와 배열의 `sizeof` 차이를 설명할 수 있다.
- [ ] `malloc`/`realloc`/`free`의 역할과 실패 가능성을 설명할 수 있다.
- [ ] ownership과 lifetime을 코드의 특정 줄에 표시할 수 있다.
- [ ] `length`와 `capacity`를 사용해 Dynamic Array의 상태를 추적할 수 있다.
- [ ] 코드에 경고가 없는지 확인했다.
