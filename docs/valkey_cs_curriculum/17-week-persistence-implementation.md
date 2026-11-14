# 17주 — Persistence 직접 구현

mini-Valkey에 Persistence를 추가합니다.

### Snapshot

```text
SAVE
```

동작:

```text
Memory
  ↓
Serialize
  ↓
data.rdb
```

### AOF

예를 들어 다음 명령을 로그에 기록합니다.

```text
SET foo 10
INCR foo
DEL bar
```

재시작 시:

```text
AOF
 ↓ replay
Memory
```

### 실험

다음 정책을 비교합니다.

```text
fsync every write

vs

fsync every second

vs

no fsync
```

측정:

```text
Throughput
Latency
Durability
Data Loss Window
```

---
