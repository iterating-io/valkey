# 23주 — Performance Engineering

### 공부

```text
Latency
Throughput

Percentile

p50
p95
p99
p999

CPU Profiling

Cache Miss

Memory Allocation

System Call

Context Switch

Network RTT
```

### 도구

```text
valkey-benchmark

perf

strace

gdb

top
htop

/usr/bin/time
```

### 실험

Pipelining을 바꿔가며 측정합니다.

```text
pipeline=1
pipeline=10
pipeline=100
pipeline=1000
```

측정:

```text
Throughput
Average Latency
p95
p99
CPU Usage
System Call Count
```

---
