# 19주 — Replication

### 공부

```text
Primary / Replica

Replication Log

Offset

Full Sync
Partial Sync

Backlog
```

### Valkey 소스 읽기

```text
src/replication.c
```

### 직접 구현

```text
mini-valkey-primary
          │
          │ replication stream
          ▼
mini-valkey-replica
```

---
