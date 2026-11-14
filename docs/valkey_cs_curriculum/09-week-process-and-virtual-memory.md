# 9주 — Process / Virtual Memory

### 공부

```text
Process
Thread

Virtual Address
Physical Address

Page
Page Table
TLB

Page Fault

mmap

fork
Copy-on-Write
```

### 실험

C 프로그램에서 `fork()` 후 큰 메모리를 할당하고 부모/자식에서 수정하면서 다음을 관찰합니다.

```text
RSS
Page Fault
Memory Usage
Copy-on-Write
```

### 핵심 연결

이 내용은 이후 RDB Snapshot과 Background Save를 이해할 때 중요합니다.

---
