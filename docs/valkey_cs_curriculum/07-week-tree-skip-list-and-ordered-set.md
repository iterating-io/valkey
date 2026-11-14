# 7주 — Tree / Skip List / Ordered Set

### 공부

```text
BST
AVL Tree
Red-Black Tree
Heap
Skip List
Binary Search
Ordered Set
```

### 직접 구현

Sorted Set을 만들어 봅니다.

```text
ZADD
ZRANGE
ZRANK
```

구조는 처음에는 다음과 같이 구성합니다.

```text
HashMap
+
Skip List
```

### Valkey 소스 읽기

```text
src/t_zset.c
```

---
