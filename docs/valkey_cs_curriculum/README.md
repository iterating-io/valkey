# Valkey를 통해 배우는 컴퓨터공학 커리큘럼

> 목표: Valkey를 단순히 사용하는 개발자가 아니라, **Valkey 내부 동작을 소스 코드 수준에서 이해하고 수정할 수 있는 시스템 개발자**가 되는 것.

이 디렉터리는 24주 커리큘럼을 주차별로 나눈 문서 모음입니다. 전체 배경과 학습 원칙은 [00. 커리큘럼 소개와 전체 개요](./00-overview.md)에서 확인할 수 있습니다.

## 주차별 커리큘럼

- [00. 커리큘럼 소개와 전체 개요](./00-overview.md)
- [1주 — C 메모리 모델](./01-week-c-memory-model.md)
- [2주 — C 시스템 프로그래밍](./02-week-c-system-programming.md)
- [3주 — CPU와 데이터 표현](./03-week-cpu-and-data-representation.md)
- [4주 — 디버깅과 Valkey 빌드](./04-week-debugging-and-valkey-build.md)
- [5주 — Hash Table](./05-week-hash-table.md)
- [6주 — String / List](./06-week-string-and-list.md)
- [7주 — Tree / Skip List / Ordered Set](./07-week-tree-skip-list-and-ordered-set.md)
- [8주 — 확률적 알고리즘과 Cache](./08-week-probabilistic-algorithms-and-cache.md)
- [9주 — Process / Virtual Memory](./09-week-process-and-virtual-memory.md)
- [10주 — File Descriptor / Socket](./10-week-file-descriptor-and-socket.md)
- [11주 — I/O Multiplexing](./11-week-io-multiplexing.md)
- [12주 — Event Loop 직접 구현](./12-week-event-loop.md)
- [13주 — TCP](./13-week-tcp.md)
- [14주 — RESP와 Protocol Parser](./14-week-resp-and-protocol-parser.md)
- [15주 — mini-Valkey 구현](./15-week-mini-valkey.md)
- [16주 — Persistence / 파일 시스템](./16-week-persistence-and-filesystem.md)
- [17주 — Persistence 직접 구현](./17-week-persistence-implementation.md)
- [18주 — Transaction / Concurrency Control](./18-week-transaction-and-concurrency-control.md)
- [19주 — Replication](./19-week-replication.md)
- [20주 — 장애와 Consistency](./20-week-failure-and-consistency.md)
- [21주 — Failure Detector / Sentinel](./21-week-failure-detector-and-sentinel.md)
- [22주 — Cluster](./22-week-cluster.md)
- [23주 — Performance Engineering](./23-week-performance-engineering.md)
- [24주 — Valkey Contributor 프로젝트](./24-week-valkey-contributor-project.md)

## 학습 흐름

```text
C → 자료구조 → 운영체제 → 네트워크 → mini-Valkey
  → Persistence → Replication → Distributed Systems
  → Performance Engineering → Valkey Contribution
```
