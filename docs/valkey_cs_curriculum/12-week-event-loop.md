# 12주 — Event Loop 직접 구현

### 구현

작은 Event Loop 라이브러리를 만듭니다.

```text
mini-event-loop

register(fd, READABLE)
register(fd, WRITABLE)

poll()

callback(fd)
```

그 위에 TCP Echo Server를 올립니다.

```text
             Event Loop
                 │
      ┌──────────┼──────────┐
      ▼          ▼          ▼
   client1    client2     client3
```

### Phase 3 통과 기준

```text
왜 Client마다 Thread를 만들지 않아도 되는가?

epoll은 왜 필요한가?

Blocking I/O와 Non-blocking I/O의 차이는?

fork 후 메모리는 실제로 언제 복사되는가?

Event Loop가 CPU를 계속 100% 사용하지 않는 이유는?
```

---

# Phase 4. 네트워크와 mini-Valkey
