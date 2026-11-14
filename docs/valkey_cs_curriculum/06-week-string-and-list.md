# 6주 — String / List

### 공부

```text
Dynamic Array
Linked List
Deque
Memory Fragmentation
Amortized Allocation
Cache Locality
```

### 직접 구현

```text
LPUSH
RPUSH
LPOP
RPOP
```

### Valkey 소스 읽기

```text
src/sds.c
src/quicklist.c
src/listpack.c
src/t_list.c
```

### 핵심 질문

```text
왜 textbook Linked List를 그대로 사용하지 않는가?

왜 메모리를 연속적으로 배치하면 빠를 수 있는가?

Big-O가 같아도 왜 실제 속도가 달라지는가?
```

성능에서는 다음도 중요합니다.

```text
Memory Overhead
Cache Locality
Allocation Count
Pointer Chasing
```

---
