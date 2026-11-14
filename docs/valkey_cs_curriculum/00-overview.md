# Valkey를 통해 배우는 컴퓨터공학 커리큘럼

> 목표: Valkey를 단순히 사용하는 개발자가 아니라, **Valkey 내부 동작을 소스 코드 수준에서 이해하고 수정할 수 있는 시스템 개발자**가 되는 것.

---

## 1. Valkey를 공부하는 이유

Valkey는 단순한 Key-Value Cache가 아니라 다음 요소가 한 코드베이스에 모여 있는 시스템 소프트웨어입니다.

- In-Memory Database
- Cache
- Messaging / Pub-Sub
- Stream Processing
- Persistence
- Replication
- High Availability
- Distributed Cluster

따라서 Valkey를 깊게 공부하면 다음 컴퓨터공학 분야를 실제 코드와 연결해서 학습할 수 있습니다.

- C / 시스템 프로그래밍
- 컴퓨터 구조
- 자료구조와 알고리즘
- 운영체제
- 컴퓨터 네트워크
- 데이터베이스 시스템
- 분산 시스템
- 성능 엔지니어링

핵심 학습 축은 다음과 같습니다.

```text
C
+
자료구조
+
운영체제
+
네트워크
+
데이터베이스
+
분산 시스템
```

---

# 2. Valkey 기능과 필요한 컴퓨터공학

| Valkey 기능 | 공부할 컴퓨터공학 |
|---|---|
| String / Hash / List / Set | 자료구조, 알고리즘 |
| Sorted Set | 정렬, 탐색, Ordered Data Structure |
| Bitmap / Bitfield | 비트 연산, 데이터 표현 |
| HyperLogLog / Bloom Filter 계열 | 확률적 자료구조, 확률론 |
| TTL / Expiration | 자료구조, 시간 관리 |
| LRU / LFU Eviction | 캐시 알고리즘 |
| RESP 프로토콜 | 컴퓨터 네트워크, 직렬화 |
| TCP Client/Server | Socket Programming |
| Event Loop | 운영체제, Non-blocking I/O |
| 많은 동시 연결 | I/O Multiplexing, epoll/kqueue |
| MULTI / EXEC / WATCH | DB Transaction, 동시성 제어 |
| RDB | Snapshot, 파일 시스템 |
| AOF | WAL / Log 기반 복구 |
| Replication | 분산 시스템 |
| Sentinel | 장애 감지, Failover |
| Cluster | Sharding, Hash Slot, Gossip |
| Pub/Sub | Messaging |
| Streams | Event Log, Consumer Group |
| Lua / Functions | Embedded Runtime |
| Module API | Plugin Architecture |
| ACL / TLS | 보안, 인증, 암호화 |
| Valkey Search | 검색 알고리즘, Vector Search, HNSW |

Valkey를 공부할 때 중요한 것은 기능 자체를 외우는 것이 아니라 다음 질문을 반복하는 것입니다.

```text
이 기능은 왜 이런 자료구조를 사용할까?

시간복잡도는 어떻게 될까?

메모리는 얼마나 사용할까?

CPU cache locality에는 어떤 영향을 줄까?

동시 접속이 많으면 어떻게 처리할까?

서버가 죽으면 데이터는 어떻게 복구할까?

Primary가 죽으면 Replica는 어떻게 처리할까?

Network Partition이 발생하면 어떻게 될까?
```

---

# 3. 전체 학습 목표

최종적으로 다음 흐름을 소스 코드 수준에서 설명할 수 있어야 합니다.

```text
Client
  ↓ TCP
Socket
  ↓
Event Loop
  ↓
RESP Parser
  ↓
Command
  ↓
Data Structure
  ↓
Memory
  ├── RDB
  ├── AOF
  └── Replication
         ↓
      Replica
         ↓
      Cluster
```

그리고 다음 작업을 직접 할 수 있는 수준을 목표로 합니다.

```text
1. Valkey Source Build
2. gdb로 요청 하나 추적
3. 새로운 Command 구현
4. 테스트 작성
5. Benchmark 수행
6. 작은 성능/기능 개선
7. 실제 Valkey PR 이해
8. Valkey 코드에 기여
```

---

# 4. 24주 전체 커리큘럼

기준 학습량:

- 주당 약 8~10시간
- 기간보다 단계 순서가 더 중요
- 이론만 공부하지 않고 반드시 구현과 소스코드 읽기를 병행

전체 흐름:

```text
C
 ↓
자료구조
 ↓
운영체제
 ↓
Network
 ↓
mini-Valkey
 ↓
Persistence
 ↓
Replication
 ↓
Distributed Systems
 ↓
Performance Engineering
 ↓
Valkey Contribution
```

---

# Phase 1. C와 컴퓨터 구조

# 5. Valkey 소스코드 읽는 순서

처음부터 `server.c` 전체를 읽는 것은 추천하지 않습니다.

작은 모듈부터 올라가는 방식이 좋습니다.

---

## Level 1 — 작은 자료구조

```text
sds.c
dict.c
adlist.c
intset.c
listpack.c
```

배울 것:

```text
Memory Layout
Dynamic String
Hash Table
Linked Structure
Compact Encoding
```

---

## Level 2 — Event Loop

```text
ae.c
ae_epoll.c
ae_kqueue.c
ae_select.c
```

배울 것:

```text
Event Loop
I/O Multiplexing
File Descriptor
Callback
```

---

## Level 3 — Network

```text
connection.c
socket.c
networking.c
```

배울 것:

```text
TCP
Connection
Buffer
RESP Parsing
Client State
```

---

## Level 4 — Commands

```text
t_string.c
t_hash.c
t_list.c
t_set.c
t_zset.c
```

배울 것:

```text
Command Implementation
Data Structure Mapping
Complexity
Validation
```

---

## Level 5 — Database

```text
db.c
object.c
expire.c
evict.c
```

배울 것:

```text
Key Space
Object Representation
TTL
Expiration
Eviction
```

---

## Level 6 — Persistence

```text
rdb.c
aof.c
rio.c
```

배울 것:

```text
Snapshot
Serialization
AOF
Crash Recovery
I/O Abstraction
```

---

## Level 7 — Server

```text
server.c
```

이 시점부터 `server.c`가 훨씬 읽기 쉬워집니다.

배울 것:

```text
Initialization
Command Processing
Configuration
Server Lifecycle
Main Loop
```

---

## Level 8 — Replication

```text
replication.c
```

배울 것:

```text
Primary / Replica
Synchronization
Replication Offset
Backlog
Reconnect
```

---

## Level 9 — HA / Distributed

```text
sentinel.c
cluster.c
cluster.h
```

배울 것:

```text
Failure Detection
Failover
Quorum
Gossip
Hash Slot
Sharding
Cluster State
```

---

# 6. 가장 중요한 프로젝트 6개

24주 동안 모든 내용을 동일한 비중으로 공부할 필요는 없습니다.

아래 6개 프로젝트를 반드시 완성하는 것을 핵심 목표로 둡니다.

---

## Project 1 — C 자료구조 라이브러리

```text
HashMap
Dynamic Array
Linked List
Skip List
Bitmap
```

배우는 것:

```text
Memory
Pointer
Complexity
Allocation
Cache Locality
```

---

## Project 2 — epoll 기반 TCP Server

```text
Socket
Non-blocking I/O
epoll
Event Loop
Connection Management
```

---

## Project 3 — RESP Parser

지원:

```text
Simple String
Error
Integer
Bulk String
Array
```

배우는 것:

```text
Protocol
Serialization
Incremental Parser
Buffer
State Machine
```

---

## Project 4 — mini-Valkey

지원:

```text
PING

SET
GET
DEL
INCR

LPUSH
LPOP

EXPIRE
TTL
```

Architecture:

```text
TCP
 ↓
Event Loop
 ↓
RESP
 ↓
Command
 ↓
HashMap
```

---

## Project 5 — mini-Valkey Persistence + Replication

추가:

```text
Snapshot
AOF
Crash Recovery
Primary
Replica
Replication Stream
```

---

## Project 6 — 실제 Valkey 코드 수정

진행:

```text
Valkey Build
 ↓
Source Analysis
 ↓
gdb
 ↓
Code Change
 ↓
Test
 ↓
Benchmark
 ↓
PR 수준 Patch
```

---

# 7. CS 과목별 우선순위

전체 과정의 우선순위는 다음과 같습니다.

```text
★★★★★ C
★★★★★ 자료구조
★★★★★ 운영체제
★★★★★ Network

★★★★☆ Database Internals
★★★★☆ Distributed Systems

★★★☆☆ Computer Architecture
★★★☆☆ Performance Engineering

★★☆☆☆ 일반적인 SQL / 관계형 DB 이론
```

Valkey 개발이 목표라면 관계형 데이터베이스의 SQL, Relational Algebra, Query Optimizer를 가장 먼저 깊게 공부할 필요는 없습니다.

먼저 다음 기반을 만드는 것이 중요합니다.

```text
C
OS
Network
Data Structures
```

---

# 8. 분야별 핵심 공부 목록

## C / 시스템 프로그래밍

```text
Pointer
Struct
Function Pointer
Macro

Stack / Heap

malloc
calloc
realloc
free

Memory Alignment

Compiler
Linker

Make
CMake

gdb

ASan
UBSan
TSan
```

---

## 자료구조

```text
Dynamic Array

Linked List
Deque

Hash Table

Skip List

Tree

Heap

Bitmap

Probabilistic Data Structure
```

---

## 알고리즘

```text
Big-O

Amortized Analysis

Hashing

Binary Search

Sampling

Approximation

LRU

LFU
```

---

## 운영체제

```text
Process
Thread

Context Switching

Virtual Memory

Page
Page Fault

fork
Copy-on-Write

File Descriptor

Socket

Non-blocking I/O

select
poll
epoll
kqueue

Memory Allocator

Page Cache

fsync
```

---

## 네트워크

```text
TCP

Socket

Connection

Buffer

Byte Stream

Protocol

Serialization

Request / Response

Pipelining

RTT

Backpressure
```

---

## 데이터베이스 시스템

```text
Durability

Write Ahead Log

Logging

Checkpoint

Snapshot

Crash Recovery

Atomicity

Transaction

Optimistic Locking

CAS
```

---

## 분산 시스템

```text
Replication

Sharding

Partitioning

Hashing

Gossip Protocol

Failure Detector

Leader Election

Network Partition

Consistency

Availability

CAP

Split Brain

Failover

Distributed State
```

---

## 성능 엔지니어링

```text
Latency

Throughput

p50
p95
p99
p999

CPU Profiling

Cache Miss

Memory Allocation

System Call

Context Switch

Benchmark Design
```

---

# 9. 공부할 때 반복할 학습 사이클

이론을 전부 공부한 뒤 Valkey를 시작하면 안 됩니다.

다음 사이클을 반복하는 것이 좋습니다.

```text
개념 공부
  ↓
작게 직접 구현
  ↓
Valkey에서 같은 문제 찾기
  ↓
Valkey 코드 읽기
  ↓
왜 구현이 다른지 분석
  ↓
Benchmark
  ↓
다시 개선
```

예:

```text
Hash Table 이론
 ↓
HashMap 직접 구현
 ↓
Valkey dict.c 읽기
 ↓
구현 차이 분석
 ↓
rehash 실험
 ↓
성능 측정
```

또 다른 예:

```text
epoll 공부
 ↓
Echo Server 구현
 ↓
Event Loop 구현
 ↓
Valkey ae.c 읽기
 ↓
networking.c 연결
```

---

# 10. Valkey 요청 하나를 추적하는 연습

예를 들어:

```text
SET foo bar
```

가 들어왔다고 가정합니다.

최종적으로 다음 전체 흐름을 소스 수준에서 추적할 수 있어야 합니다.

```text
Client
  ↓
TCP
  ↓
Socket
  ↓
File Descriptor
  ↓
Event Loop
  ↓
Readable Event
  ↓
Network Buffer
  ↓
RESP Parser
  ↓
Command Lookup
  ↓
SET Command
  ↓
Dictionary
  ↓
Object / String
  ↓
Memory
  ↓
Reply Creation
  ↓
Output Buffer
  ↓
Writable Event
  ↓
TCP
  ↓
Client
```

Persistence가 켜져 있다면:

```text
SET
 ↓
Memory Update
 ↓
AOF
```

Replication이 켜져 있다면:

```text
SET
 ↓
Primary
 ↓
Replication Stream
 ↓
Replica
```

Cluster라면:

```text
Key
 ↓
Hash
 ↓
Hash Slot
 ↓
Node
```

까지 이어집니다.

---

# 11. 24주 후 최종 체크리스트

다음을 설명할 수 있으면 목표에 상당히 가까워진 것입니다.

## C

- Pointer를 자유롭게 읽을 수 있다.
- Stack / Heap을 설명할 수 있다.
- Memory Leak과 Use-after-free를 찾을 수 있다.
- gdb를 사용해 Call Stack을 추적할 수 있다.

## 자료구조

- Hash Table을 직접 구현할 수 있다.
- Rehashing을 설명할 수 있다.
- Skip List가 무엇인지 설명할 수 있다.
- Cache Locality가 왜 중요한지 설명할 수 있다.

## 운영체제

- Process와 Thread 차이를 설명할 수 있다.
- Virtual Memory를 설명할 수 있다.
- fork와 Copy-on-Write를 설명할 수 있다.
- File Descriptor를 설명할 수 있다.
- epoll이 필요한 이유를 설명할 수 있다.

## 네트워크

- TCP가 Byte Stream이라는 의미를 설명할 수 있다.
- Partial Read / Write를 처리할 수 있다.
- Non-blocking Socket Server를 구현할 수 있다.
- RESP Parser를 직접 구현할 수 있다.

## 데이터베이스

- Snapshot과 WAL/AOF 차이를 설명할 수 있다.
- fsync 정책과 Durability Trade-off를 설명할 수 있다.
- Crash Recovery를 구현해 봤다.
- Optimistic Locking 개념을 설명할 수 있다.

## 분산 시스템

- Async Replication의 데이터 유실 가능성을 설명할 수 있다.
- Full Sync / Partial Sync 차이를 설명할 수 있다.
- Network Partition을 설명할 수 있다.
- Failure Detector의 한계를 설명할 수 있다.
- Gossip이 왜 필요한지 설명할 수 있다.
- Hash Slot 기반 Sharding을 설명할 수 있다.

## 성능

- Latency와 Throughput 차이를 설명할 수 있다.
- 평균 대신 p95/p99가 중요한 이유를 설명할 수 있다.
- perf/strace를 이용해 병목을 찾을 수 있다.
- Benchmark 결과를 해석할 수 있다.

## Valkey

- Valkey를 직접 Build/Test할 수 있다.
- 요청 하나를 gdb로 추적할 수 있다.
- 주요 자료구조 구현 위치를 찾을 수 있다.
- Event Loop 코드를 읽을 수 있다.
- Persistence 코드를 읽을 수 있다.
- Replication 코드를 읽을 수 있다.
- Cluster 코드를 읽을 수 있다.
- 작은 기능을 수정하고 테스트할 수 있다.

---

# 12. 최종 목표 프로젝트 구조

전체 학습 결과물을 하나의 프로젝트로 묶는다면 다음과 같습니다.

```text
mini-valkey/
│
├── src/
│   ├── main.c
│   ├── server.c
│   ├── event_loop.c
│   ├── networking.c
│   ├── resp.c
│   ├── command.c
│   ├── hashmap.c
│   ├── list.c
│   ├── object.c
│   ├── expire.c
│   ├── rdb.c
│   ├── aof.c
│   └── replication.c
│
├── include/
│
├── tests/
│
├── benchmark/
│
├── Makefile
│
└── README.md
```

지원 기능:

```text
TCP Server

Event Loop

RESP

SET
GET
DEL
INCR

LPUSH
LPOP

EXPIRE
TTL

Snapshot

AOF

Primary / Replica
```

최종적으로는 다음 순서가 하나의 긴 프로젝트가 됩니다.

```text
자료구조
   ↓
TCP Server
   ↓
Event Loop
   ↓
RESP
   ↓
mini-Valkey
   ↓
Persistence
   ↓
Replication
   ↓
Valkey Source
   ↓
Valkey Contribution
```

---

# 13. 권장 학습 원칙

## 1. 이론만 공부하지 않는다

```text
이론
→ 구현
→ 실제 코드
```

순서를 반복합니다.

## 2. 완벽한 구현보다 작은 구현을 먼저 만든다

예:

```text
Thread Pool까지 구현한 서버
```

를 처음부터 만드는 대신:

```text
Blocking Echo Server
 ↓
Non-blocking Server
 ↓
epoll
 ↓
Event Loop
```

순서로 발전시킵니다.

## 3. Big-O만 보지 않는다

항상 다음을 같이 봅니다.

```text
CPU
Memory
Allocation
Cache Locality
System Call
Network RTT
```

## 4. Valkey 코드를 정답으로 외우지 않는다

항상 질문합니다.

```text
왜 이렇게 구현했을까?

더 단순하게 구현하면 어떤 문제가 있을까?

이 설계는 어떤 Trade-off를 선택했을까?
```

## 5. 반드시 Benchmark한다

시스템 프로그래밍에서는 느낌보다 측정이 중요합니다.

```text
구현
 ↓
가설
 ↓
측정
 ↓
분석
 ↓
개선
```

---

# 14. 최종 학습 방향

Valkey를 중심으로 컴퓨터공학을 공부하는 가장 좋은 흐름은 다음과 같습니다.

```text
C
 ↓
Memory
 ↓
Data Structure
 ↓
Operating System
 ↓
Socket
 ↓
TCP
 ↓
Event Loop
 ↓
RESP
 ↓
mini-Valkey
 ↓
Persistence
 ↓
Transaction
 ↓
Replication
 ↓
Failure Detection
 ↓
Cluster
 ↓
Performance
 ↓
Valkey Source Contribution
```

이 과정을 끝내면 Valkey만 이해하는 것이 아니라 다음 시스템들을 공부할 기반도 함께 생깁니다.

```text
Redis / Valkey

Database Engine

Message Broker

Proxy

Web Server

Storage Engine

Distributed Cache

Distributed Database
```

핵심은 Valkey를 하나의 제품으로만 보는 것이 아니라, **컴퓨터공학의 여러 분야가 실제로 만나는 시스템 소프트웨어 교재**로 사용하는 것입니다.

---

# 참고 자료

- Valkey 공식 사이트: https://valkey.io/
- Valkey 공식 GitHub: https://github.com/valkey-io/valkey
- Valkey Introduction: https://valkey.io/topics/introduction/
- Persistence: https://valkey.io/topics/persistence/
- Replication: https://valkey.io/topics/replication/
- Cluster Specification: https://valkey.io/topics/cluster-spec/
- Transactions: https://valkey.io/topics/transactions/
- Pipelining: https://valkey.io/topics/pipelining/
- Benchmark: https://valkey.io/topics/benchmark/
