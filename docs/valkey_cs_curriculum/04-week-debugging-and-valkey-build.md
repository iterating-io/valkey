# 4주 — 디버깅과 Valkey 빌드

### 공부

```text
gdb

break
next
step
finish
backtrace
watch
print

core dump

AddressSanitizer
UndefinedBehaviorSanitizer
ThreadSanitizer
```

### 실습

Valkey 소스 코드를 내려받아 직접 빌드합니다.

```bash
git clone https://github.com/valkey-io/valkey.git
cd valkey
make
make test
```

### Phase 1 통과 기준

다음을 설명할 수 있어야 합니다.

```text
Pointer란 무엇인가?

Stack과 Heap의 차이는?

Use-after-free란?

Memory Leak이란?

Alignment가 왜 필요한가?

Atomic Operation이란?

Compiler와 Linker의 차이는?
```

---

# Phase 2. 자료구조와 알고리즘
