# 🧩 MLFQ (Multi-Level Feedback Queue) Scheduling

## 🧠 Overview
**MLFQ**는 **프로세스의 행동(behavior)** 에 따라 우선순위를 동적으로 조정하여,  
**turnaround time 최적화**와 **response time 최소화**를 동시에 달성하는 스케줄링 기법이다.

---

## ⚙️ Assumptions

MLFQ를 도입하기 전, 다음과 같은 4가지 가정을 세운다.

1. Each job runs the same amount of time — 모든 job의 실행 시간이 동일하다.  
2. All jobs arrive at the same time — 모든 job이 동시에 도착한다.  
3. All jobs only use CPU — CPU만 사용한다.  
4. Run-time of each job is known — 각 job의 종료 시점을 알고 있다.

이전 단계에서는 (1), (2), (3)을 relax(완화)하였고,  
마지막으로 (4)를 relax하기 위해 MLFQ를 적용한다.

---

## 🎯 Purpose
**MLFQ의 목표:** 사전 정보 없이도 아래 두 가지를 동시에 달성하기 위함.

- **Optimize turnaround time** — 전체 작업 완료 시간을 최소화  
- **Minimize response time** — 사용자 반응 시간을 최소화  

---

## 🧩 MLFQ Rules

### 🧷 Rule 1  
If `Priority(A) > Priority(B)` → **A runs**

→ 더 높은 우선순위 큐에 있는 프로세스부터 실행된다.

---

### 🧷 Rule 2  
If `Priority(A) = Priority(B)` → **A & B run in Round Robin (RR)**

→ 동일 우선순위라면 **time slice** 단위로 **Round Robin** 방식으로 실행된다.

---

### 🧷 Rule 3  
When a job enters the system, it is placed at the **highest priority**.

→ 새로 들어온 job은 항상 최상위 큐에서 시작한다.  
(이전 행동 기반으로 priority를 재조정하기 위함)

---

### 🧷 Rule 4  
Once a job uses up its **time allotment** at a given level  
(regardless of how many times it has given up the CPU),  
its priority is reduced.  
→ **시간 allotment를 초과한 경우 priority 강등**

#### 세부 규칙
- If a job uses up **entire time slice** → **priority down** (Rule 4a)  
- If a job gives up CPU **before** time slice ends → **priority 유지** (Rule 4b)

---

### 🧷 Rule 5 (Priority Boost)
After some time period **S**, move all jobs to the **topmost queue**.

→ **Starvation** 및 **behavior change 문제**를 방지하기 위한 reset 규칙.  
→ 모든 프로세스의 priority를 주기적으로 초기화한다.

---

## 🔍 Rule 1 & 2 상세 설명
- 높은 priority의 큐부터 실행되며, 한 큐의 프로세스가 모두 끝나야 다음 큐 실행 가능.  
- 동일 priority의 프로세스들은 **Round Robin** 방식으로 time slice를 공유한다.  
- **한 프로세스는 오직 하나의 큐에만 존재한다.**

---

## 🔍 Rule 3 & 4 상세 설명

### Priority Selection (행동 기반 선택)
- **I/O 중심 (Interactive)** → High priority  
  (빠른 사용자 응답 필요)
- **CPU 중심 (Batch job)** → Low priority  
  (response보다 throughput 중심)

#### 예시 1
```
8-Level Queue / 4EA Process
Q8 -> A -> B
Q7
..
Q4 -> C
..
Q1 -> D
```
A & B가 RR으로 동작 후, C → D 순으로 완료된다.

#### 예시 2
```
if A: I/O 중심, B: CPU 중심으로 변화
Q8 -> A
Q7 -> B
..
Q4 -> C
..
Q1 -> D
```
→ A → B → C → D 순서로 실행된다.

---

### Better Accounting (Rule 4 확장)
> 주어진 level에서 CPU 점유 시간이 **time allotment**보다 크면  
> priority를 한 단계 낮춘다.

#### 예시 3 — A Single Long-running Job
```
3 Level Queue, 1EA Process, time slice = 10ms
Q2 | run(10ms) |
Q1 | run(10ms) |
Q0 | run(10ms) run(10ms) run(10ms) ...
```
→ Q2 → Q1 → Q0 순으로 강등되며 실행된다.

---

#### 예시 4 — Along Came a Short Job
```
3 Level Queue, 2EA Process
time slice = 10ms
A: Long-running CPU job
B: Short interactive job (20ms runtime, arrive T=100ms)
```

```
Q2 | A(10ms) | ... | B(10ms) |
Q1 | A(10ms) | ... | B(10ms) |
Q0 | A(80ms) | ... | A(...)
```
→ 100ms에 B가 들어오면 Rule 3에 따라 Q2로 올라와 먼저 실행된다.  
→ B가 완료되면 다시 Q0의 A가 실행된다.

---

#### 예시 5 — I/O 중심 프로세스
```
3 Level Queue, 2EA Process
A: Long CPU job → Q0
B: Interactive I/O job → Q2
```
- B는 매번 **time slice 전부를 쓰지 않음** → Q2 유지  
- 결과: B → A → B → A … 반복 실행

---

## ⚠️ Problem

| 문제 | 설명 |
|------|------|
| **Starvation** | I/O-bound job이 많으면 CPU-bound job은 거의 실행되지 않음 |
| **Game the Scheduler** | 99%만 쓰고 time slice를 살짝 남겨서 priority 유지 → 독점 가능 |
| **Behavior Change** | CPU job이 나중에 I/O-bound로 바뀌어도 priority가 자동으로 회복되지 않음 |

---

## 🧭 Rule 5: Priority Boost
> 위 문제들을 해결하기 위한 방법

**After some time period S, move all jobs to the topmost queue.**

- 모든 job의 priority를 최고로 초기화  
- starvation 방지  
- CPU → I/O behavior 변화 대응  

---

## 🧮 Tuning MLFQ & Additional Issues

| 구분 | 내용 |
|------|------|
| **Low-level Queues (CPU-bound)** | 긴 run-time → **time slice 길게 설정** (overhead 최소화) |
| **High-level Queues (I/O-bound)** | 짧은 run-time → **time slice 짧게 설정** (응답성 향상) |

### 결론
- **High-priority queue** → Short time slices  
- **Low-priority queue** → Longer time slices  

---

## 🧩 Summary Table

| 구분 | 특징 |
|------|------|
| **장점** | Response 빠름, Interactive 환경에 적합 |
| **단점** | Starvation, Scheduling 조작 가능성 |
| **해결책** | Priority Boost |
| **핵심 개념** | Behavior-based Dynamic Priority Scheduling |
