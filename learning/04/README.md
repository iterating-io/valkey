# 4주차 — 디버깅과 Valkey 빌드

## 목표

- gdb로 멈춤점, 호출 흐름, 지역 변수 변화를 관찰한다.
- core dump가 멈춘 시점의 프로그램 상태라는 점을 이해한다.
- AddressSanitizer(ASan), UndefinedBehaviorSanitizer(UBSan),
  ThreadSanitizer(TSan)가 찾는 문제의 차이를 구분한다.
- 현재 checkout의 Valkey를 빌드하고, 실행한 검증 명령과 결과를 기록한다.
- Phase 1의 pointer, memory, alignment, atomic, compiler/linker 질문에 답한다.

## 이 README를 사용하는 방법

각 실습은 **설명 읽기 → 코드 찾기 → 실행 전 예측 → 컴파일·실행 → 관찰 → 코드 연결 → 완료 확인** 순서다. 설명을 읽기 전에는 예측을 요구하지 않는다.

저장소 루트는 `/home/taek/git/valkey`라고 가정한다. 학습용 C 코드는 다음 옵션으로 빌드한다.

```text
-std=c11 -Wall -Wextra -g
```

`-g`는 gdb가 변수 이름과 소스 줄을 연결할 수 있게 디버그 정보를 넣는다. 이는 프로그램을 고치거나 안전하게 만드는 옵션이 아니다.

## 파일 구성

```text
learning/04/
├── debug_demo.c  gdb로 따라갈 의도적 논리 버그
├── core_demo.c   core dump 관찰용 의도적 abort
├── asan_uaf.c    ASan용 use-after-free
├── ubsan_shift.c UBSan용 잘못된 shift
├── tsan_race.c   TSan용 data race
├── Makefile
└── README.md
```

각 sanitizer 예제에는 의도적인 잘못된 동작이 있다. 학습용 파일에서만 실행하고 Valkey 소스에는 넣지 않는다.

## 실습 1 — gdb의 기본 흐름

gdb는 프로그램을 멈춘 뒤 현재 위치, 호출자, 변수 값을 살펴보는 도구다.

- `break main` 또는 `break calculate_total`: 해당 함수 시작에 멈춤점을 둔다.
- `next` (`n`): 현재 함수 안에서 다음 소스 줄로 간다. 호출은 한 단계로 넘긴다.
- `step` (`s`): 호출한 함수 안으로 들어간다.
- `finish`: 현재 함수를 끝까지 실행하고 호출한 곳으로 돌아온다.
- `backtrace` (`bt`): 현재 함수까지 오게 된 호출 stack을 본다.
- `print total`: 식 또는 변수 값을 출력한다.
- `watch total`: `total` 값이 바뀔 때 멈춘다. 지역 변수라면 그 변수가 살아 있는 함수 안에서 설정한다.
- `continue` (`c`): 다음 멈춤점 또는 프로그램 종료까지 계속 실행한다.

### 코드에서 찾기

1. `debug_demo.c`의 `main`에서 `unit_price`, `quantity`, `total`을 찾는다.
2. `calculate_total`의 `subtotal`, `coupon`과 `apply_coupon` 호출을 찾는다.
3. `main`의 `total == 30` 비교 주석을 찾는다. 이것이 고의로 심어 둔 논리 버그다.

### 이제 예측하기

`unit_price = 10`, `quantity = 3`, coupon이 5일 때 `subtotal`과 `total`을 계산한다. 출력이 어느 분기로 갈지 적는다.

### 컴파일·실행하기

```bash
make -C learning/04 clean all
./learning/04/debug_demo
gdb -q ./learning/04/debug_demo
```

먼저 변수 값의 변화를 관찰한다. gdb 안에서 다음을 한 줄씩 실행한다.

```text
break main
run
next
next
watch total
watch total
next
print total
continue
quit
```

`watch total`은 `total`을 초기화하는 줄에서 설정한다. 다음 `next`가 계산을 마친 뒤 `total`이 25로 바뀌었다고 멈춰야 한다.

이번에는 호출 안으로 들어간다.

```text
break calculate_total
run
backtrace
print unit_price
next
print subtotal
next
step
backtrace
finish
quit
```

두 번째 `next` 뒤에는 `return apply_coupon(...)` 줄에 있다. 여기서 `step`은 `apply_coupon` 안으로 들어간다. `backtrace`에는 `apply_coupon`, `calculate_total`, `main` 순서의 호출 관계가 보여야 한다. `finish` 뒤에는 호출한 함수로 돌아온다.

### 관찰하고 연결하기

정상 계산 결과는 25인데, 고의 버그는 30과 비교한다. gdb는 원인을 자동으로 결정하지 않는다. 변수 값과 call stack이라는 증거를 보여 주고, 사람이 기대값과 비교하게 한다. 이해 후 `total == 25`로 바꿔 재빌드·실행해 고칠 수 있다.

## 실습 2 — core dump

core dump는 비정상 종료 순간의 메모리와 register 등 상태를 운영체제가 남긴 파일이다. 실행 중인 프로세스에 붙는 gdb와 달리, 종료 뒤의 상태를 `gdb 프로그램 core파일`로 볼 수 있다. core에는 민감한 메모리가 포함될 수 있으므로 공유에 주의한다.

### 코드에서 찾기

`core_demo.c`의 `report_fatal_error`와 `abort()`를 찾는다. 이는 학습을 위해 의도적으로 SIGABRT를 일으킨다.

### 이제 예측하기

`abort()`가 호출될 때 프로그램이 정상 종료할지, signal로 끝날지 예측한다. `backtrace`의 위쪽 함수 이름도 적는다.

### 컴파일·실행하고 관찰하기

```bash
make -C learning/04 core_demo
ulimit -c unlimited
./learning/04/core_demo
find . -maxdepth 2 -type f -name 'core*' -print
gdb -q ./learning/04/core_demo ./core
```

gdb에서 `backtrace`와 `quit`을 실행한다. core 파일 이름과 위치는 운영체제 설정에 따라 달라진다. `core`가 보이지 않으면 `ulimit -c`, `/proc/sys/kernel/core_pattern`, 또는 systemd-coredump 설정 때문에 파일이 다른 곳에 저장되었거나 수집만 되었을 수 있다. 이 경우 실패로 꾸미지 말고 환경 제약으로 기록한다.

## 실습 3 — sanitizer의 역할

sanitizer는 compiler가 추가 검사를 넣어 실행 중 특정 종류의 오류를 보고하게 하는 도구다. 모든 bug를 찾거나 release 빌드를 대체하지는 않는다. 같은 프로그램에 sanitizer를 조합할 수 있는지는 compiler와 런타임 조합에 따라 달라진다.

| 도구 | 이 예제가 보이는 문제 | 대표적인 범위 |
| --- | --- | --- |
| ASan | `free` 뒤 pointer 읽기 | use-after-free, out-of-bounds 등 memory access 오류 |
| UBSan | 32-bit 값에서 32칸 shift | 정의되지 않은 동작 일부 |
| TSan | 두 thread가 `counter++`를 동시에 수행 | data race |

`memory leak`은 ASan runtime에 포함될 수 있는 LeakSanitizer가 보고할 수 있지만, 플랫폼·옵션에 따라 다르다. leak은 할당한 heap memory에 더 이상 도달할 방법 없이 `free`도 하지 않은 상태다. use-after-free와 반대가 아니라 서로 다른 오류다.

### 코드에서 찾기

1. `asan_uaf.c`의 `free(number)` 뒤 `*number` 읽기를 찾는다.
2. `ubsan_shift.c`의 `shift = 32`와 `1U << shift`를 찾는다. `uint32_t`의 유효 shift 수는 0부터 31까지다.
3. `tsan_race.c`의 공유 전역 `counter`와 두 `pthread_create` 호출을 찾는다. `counter++`는 읽기·증가·쓰기를 묶은 동작이라 atomic이 아니다.

### 이제 예측하기

각 프로그램을 일반 실행했을 때와 해당 sanitizer를 붙여 실행했을 때 무엇이 달라질지 적는다. 특히 use-after-free가 항상 같은 값처럼 보인다고 해서 올바른 코드가 되는 것은 아니다.

### 컴파일·실행하고 관찰하기

각 명령은 0이 아닌 종료 상태가 **예상되는 학습 실험**이다. 한 명령의 실패가 다음 명령을 막지 않게 따로 실행한다.

```bash
make -C learning/04 asan
./learning/04/asan_uaf

make -C learning/04 ubsan
./learning/04/ubsan_shift

make -C learning/04 tsan
./learning/04/tsan_race
```

ASan은 `heap-use-after-free`, UBSan은 `shift exponent`, TSan은 `data race`를 포함한 진단을 목표로 한다. compiler가 `-fsanitize=thread`를 지원하지 않거나 TSan runtime이 현재 환경에서 실행되지 않는 경우도 있다. 그때는 정확한 compiler 또는 runtime 메시지와 함께 “TSan 관찰 불가”로 기록한다. 프로그램의 정상 종료만으로 race가 없다는 뜻은 아니다.

## 실습 4 — 현재 Valkey 빌드와 가장 작은 검증

Valkey 루트의 `README.md`는 기본 빌드를 `make`, 통합 검증을 `make test`, unit 검증을 `make test-unit`으로 안내한다. 처음에는 현재 checkout을 바꾸지 않는 기본 빌드와, 이미 빌드된 서버를 대상으로 하는 가장 작은 ping 검증부터 한다.

```bash
make
```

그 다음 터미널 1에서 서버를 foreground로 실행한다.

```bash
./src/valkey-server --port 0 --unixsocket /tmp/valkey-learning.sock --logfile ""
```

터미널 2에서 연결과 종료를 확인한다.

```bash
./src/valkey-cli -s /tmp/valkey-learning.sock ping
./src/valkey-cli -s /tmp/valkey-learning.sock shutdown nosave
```

성공 기준은 build가 0으로 끝나고 `ping`이 `PONG`을 출력하며 shutdown 뒤 서버가 unix socket을 제거하고 종료하는 것이다. 더 넓은 검증은 다음처럼 한다.

```bash
make -C src test-unit
make test
```

테스트는 시간이 오래 걸리거나 로컬 의존성에 영향을 받을 수 있다. 실행한 명령, 종료 상태, 핵심 결과를 아래 기록 칸에 그대로 적는다.

```text
날짜 / checkout:
make:
ping 검증:
추가 검증:
환경 제약 또는 실패 원인:
```

## Phase 1 빠른 점검

- Pointer는 객체의 주소를 저장하는 값이다. 유효한 객체 수명과 맞는 타입·정렬로만 역참조해야 한다.
- Stack은 보통 함수 호출의 지역 상태를 위한 자동 저장공간이고 함수가 돌아오면 그 수명이 끝난다. Heap은 `malloc` 등으로 얻고 `free`로 반환하는 동적 저장공간이다.
- Use-after-free는 `free` 뒤에도 그 pointer를 읽거나 쓰는 오류다.
- Memory leak은 더 이상 사용할 수 없는 heap allocation을 반환하지 않아 자원이 남는 오류다.
- Alignment는 타입이 요구하는 주소 경계다. 잘못 정렬된 pointer 역참조는 성능 저하뿐 아니라 C에서 정의되지 않은 동작이 될 수 있다.
- Atomic operation은 특정 객체의 작업이 다른 실행 흐름에 중간 상태 없이 보이게 하는 성질이다. 더 큰 여러 단계의 불변식에는 lock 또는 별도 설계가 필요할 수 있다.
- Compiler는 C 소스를 object code로 번역하고, linker는 object file·library를 연결해 실행 파일을 만든다.

## 완료 조건

- [ ] gdb에서 `break`, `next`, `step`, `finish`, `backtrace`, `watch`, `print`을 실행하고 언제 쓰는지 설명한다.
- [ ] 작은 프로그램에서 `total`의 변화와 `apply_coupon → calculate_total → main` 호출 관계를 관찰했다.
- [ ] core dump가 무엇인지, core 파일이 없을 수 있는 환경 이유를 설명한다.
- [ ] ASan, UBSan, TSan의 목적과 차이를 설명하고 실제 결과 또는 환경 제약을 기록했다.
- [ ] 현재 checkout에서 `make`와 unix socket `PONG` 검증을 실행하고 결과를 기록했다.
- [ ] Phase 1의 일곱 질문에 자신의 말로 답할 수 있다.
