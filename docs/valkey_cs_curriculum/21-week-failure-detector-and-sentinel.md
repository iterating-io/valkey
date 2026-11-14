# 21주 — Failure Detector / Sentinel

### 공부

```text
Heartbeat

Timeout

Failure Detector

Leader Election

Quorum

Epoch
```

### Valkey 소스 읽기

```text
src/sentinel.c
```

### 핵심 질문

```text
"서버가 죽었다"

vs

"네트워크가 느리다"
```

분산 환경에서는 이 둘을 완벽하게 구분하기 어렵습니다.

이 문제가 Failure Detector의 핵심입니다.

---
