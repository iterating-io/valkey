# 1주 — C 메모리 모델

### 공부

- Pointer
- Pointer Arithmetic
- Array
- Struct
- Union
- Enum
- `sizeof`
- Stack / Heap
- `malloc`
- `calloc`
- `realloc`
- `free`
- Alignment
- Padding
- Lifetime
- Ownership 개념

### 직접 구현

```text
Dynamic Array
String
Linked List
```

예를 들어 다음 구조를 자연스럽게 다룰 수 있어야 합니다.

```c
typedef struct {
    char *buf;
    size_t len;
    size_t capacity;
} String;
```

### 목표

다음 코드가 나와도 막히지 않는 수준입니다.

```text
char *
char **
void *
struct *
pointer to pointer
```

---
