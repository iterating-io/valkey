# 5주 — Hash Table

### 공부

```text
Hash Function
Collision
Chaining
Open Addressing
Load Factor
Rehashing
Incremental Rehashing
Amortized Complexity
```

### 직접 구현

```text
HashMap

SET key value
GET key
DEL key
```

### 실험

10만 개 정도 데이터를 넣으면서 다음을 측정합니다.

```text
Load Factor
Collision Count
Lookup Latency
Memory Usage
```

### Valkey 소스 읽기

```text
src/dict.c
src/dict.h
```

---
