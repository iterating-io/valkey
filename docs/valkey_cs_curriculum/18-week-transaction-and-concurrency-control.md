# 18주 — Transaction / Concurrency Control

### 공부

```text
Atomicity
Isolation

Optimistic Concurrency Control

CAS

Locking

MVCC 개념
```

### 구현

```text
MULTI
SET A 10
SET B 20
EXEC
```

가능하다면 다음도 추가합니다.

```text
WATCH key
```

### 핵심 질문

```text
Transaction 중 다른 Client 명령은 어떻게 처리되는가?

WATCH가 왜 Optimistic Locking과 비슷한가?

Rollback이 없는 Transaction은 어떤 특성을 가지는가?
```

---

# Phase 6. Replication과 분산 시스템
