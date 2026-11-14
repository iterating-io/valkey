# 11주 — I/O Multiplexing

### 공부

```text
select
poll
epoll
kqueue

Level Triggered
Edge Triggered

Event Driven Architecture
```

### Valkey 소스 읽기

```text
src/ae.c
src/ae.h

src/ae_epoll.c
src/ae_kqueue.c
src/ae_select.c
```

### 추적할 흐름

```text
aeMain()
   ↓
aeProcessEvents()
   ↓
aeApiPoll()
```

---
