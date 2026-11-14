# 22주 — Cluster

### 공부

```text
Sharding

Hash Slot

Hashing

Gossip

Node Discovery

Failure Detection

Leader Election

Resharding

Redirection
```

### Valkey 소스 읽기

```text
src/cluster.c
src/cluster.h
src/crc16.c
```

### 구현

작은 Sharding Simulator를 만듭니다.

```text
key
 ↓
CRC / Hash
 ↓
slot
 ↓
node
```

### 추가 공부

```text
MOVED
ASK
Cluster Bus
Gossip
Hash Slot Migration
```

---

# Phase 7. 성능 엔지니어링과 실제 기여
