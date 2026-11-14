# 4주차 검증 기록

검증일: 2026-08-26
checkout: `4733aed65 Add XX option to ZRANGE commands for null/empty distinction (#2978)`

## 학습 예제

- `make -C learning/04 clean all`: 성공.
- `make -C learning/04 asan`, `ubsan`, `tsan`: 모두 compiler와 runtime에서 지원됨.
- ASan: `heap-use-after-free` 진단을 확인함 (의도적인 `asan_uaf.c` 오류, 종료 상태 1).
- UBSan: `shift exponent 32 is too large for 32-bit type` 진단을 확인함 (의도적인 `ubsan_shift.c` 오류, 종료 상태 1).
- TSan: `data race` 진단을 확인함 (의도적인 `tsan_race.c` 오류, 종료 상태 66).
- gdb: `total` watchpoint가 0에서 25로 바뀌는 것을 확인했고, `calculate_total → main` 및 `apply_coupon → calculate_total → main` 호출 stack을 확인함.
- core dump: `core_demo`는 SIGABRT로 종료됨 (상태 134). 이 환경의 `core_pattern`은 systemd-coredump로 전달하는 설정이라 현재 작업 디렉터리에 core 파일이 생성되지 않았음.

## Valkey

- 초기 `make`는 남아 있던 LTO 15.1 object와 현재 GCC 16.0의 불일치로 link에 실패함.
- `make distclean` 뒤 `make -j1` clean rebuild를 완료함.
- 이어서 실행한 `make`가 성공했으며, 빌드 시스템은 `Hint: It's a good idea to run 'make test' ;)`를 출력함.
- `valkey-server --port 0 --unixsocket /tmp/valkey-learning-04.sock --logfile ""`로 foreground 실행 후 `valkey-cli ... ping`의 `PONG`을 확인함.
- `valkey-cli ... shutdown nosave`로 정상 종료했으며 서버 로그에서 unix socket 제거를 확인함.

전체 integration test인 `make test`와 unit suite는 이 단계에서 실행하지 않았다. 이번 단계의 최소 검증은 clean build와 실제 server/client ping이다.
