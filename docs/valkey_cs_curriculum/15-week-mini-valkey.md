# 15주 — mini-Valkey 구현

지금까지 배운 것을 하나로 합칩니다.

### 지원 명령

```text
PING

SET key value
GET key
DEL key

INCR key

LPUSH
LPOP

EXPIRE
TTL
```

### Architecture

```text
TCP Server
   ↓
epoll
   ↓
Input Buffer
   ↓
RESP Parser
   ↓
Command Table
   ↓
HashMap
   ↓
Response Buffer
   ↓
TCP
```

처음에는 Single-thread Event Loop 방식으로 구현합니다.

### 테스트

```text
1000 Clients
Pipeline
Large Request
Partial Packet
Partial Write
Disconnect
Invalid Protocol
```

### Phase 4 통과 기준

다음 요청이 들어왔을 때 전체 흐름을 설명할 수 있어야 합니다.

```text
SET foo bar
```

```text
socket
→ event loop
→ read
→ protocol parsing
→ command lookup
→ hash table
→ reply generation
→ write
```

---

# Phase 5. Database Internals
