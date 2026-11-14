# 20주 — 장애와 Consistency

### 공부

```text
Network Partition

Consistency

Availability

Durability

CAP

Split Brain

Failover

Quorum
```

### 사고 실험

```text
Client
   │
   ▼
Primary
   │
   X network failure
   │
Replica
```

다음을 고민합니다.

```text
Primary가 Client에게 OK를 보낸 직후 죽으면?

Replica가 Write를 아직 받지 못했다면?

Replica가 새로운 Primary로 승격되면?

어떤 Write가 유실될 수 있는가?
```

### 핵심 개념

비동기 Replication에서는 장애 시점에 따라 이미 ACK된 Write도 유실될 수 있습니다.

---
