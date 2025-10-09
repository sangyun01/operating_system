# Scheduling : Introduction

---

## Assumptions

We set **4 Assumptions** and then relax each of them:

1. **Each job runs same amount of time** → 모든 jobs의 process 시간이 동일하다.  
2. **All jobs arrive at the same time** → 동시에 jobs들이 도착한다.  
3. **All jobs only use CPU** → CPU만 사용한다.  
4. **Run-time of each job is known** → 언제 끝나는지 알고 있다.

---

## Concept

### Turnaround Time
The time at which the job completes minus the time at which the job arrived in the system.  
→ 완료된 시간 - 도착한 시간 = 동작시간  
→ **T_turnaround = T_completion - T_arrival**

### Fairness
Performance and fairness are often at odds in scheduling.

### Response Time
The time from when the job arrives to the first time it is scheduled.  
→ 하나의 job이 처음 실행된 시간  
→ **T_response = T_firstrun - T_arrival**

---

## FIFO (First In First Out) / FCFS (First Come First Served)

→ Very simple and easy to implement

### Example (with all assumptions)
A, B, C → each job runs for 10s & order (A→B→C)

| Job | Completion Time | Arrival | Turnaround |
|-----|-----------------|----------|-------------|
| A | 10s | 0s | 10s |
| B | 20s | 0s | 20s |
| C | 30s | 0s | 30s |

**Average Turnaround time** = (10+20+30)/3 = **20s**  
**Average Response time** = (0+10+20)/3 = **10s**

---

### Relax 1st Assumption (Each job runs same amount of time)

A=100s, B=10s, C=10s & order (A→B→C)

| Job | Completion Time | Arrival | Turnaround |
|-----|-----------------|----------|-------------|
| A | 100s | 0s | 100s |
| B | 110s | 0s | 110s |
| C | 120s | 0s | 120s |

**Average Turnaround time** = (100+110+120)/3 = **110s**  
**Average Response time** = (0+100+110)/3 = **70s**  
→ Too long Turnaround time

---

## SJF (Shortest Job First)

→ Run the shortest job first, then the next shortest, and so on.

A=100s, B=10s, C=10s → order: B→C→A

| Job | Completion Time | Arrival | Turnaround |
|-----|-----------------|----------|-------------|
| A | 120s | 0s | 120s |
| B | 10s | 0s | 10s |
| C | 20s | 0s | 20s |

**Average Turnaround time** = (120+10+20)/3 = **50s**  
**Average Response time** = (20+0+10)/3 = **10s**

---

### Relax 2nd Assumption (All jobs arrive at the same time)

A=100s, B=10s, C=10s  
T_A,arrive=0s / T_B,arrive=10s / T_C,arrive=10s  
→ order: A→B→C

| Job | Completion | Arrival | Turnaround |
|-----|-------------|----------|-------------|
| A | 100s | 0s | 100s |
| B | 110s | 10s | 100s |
| C | 120s | 10s | 110s |

**Average Turnaround time** = (100+100+110)/3 = **103.3s**  
**Average Response time** = (0+100+110)/3 = **70s**  
→ Too long Turnaround time

---

## STCF (Shortest Time to Completion First)

→ When a new job enters the system → compare remaining jobs and new job.

A=100s, B=10s, C=10s  
T_A,arrive=0s / T_B,arrive=10s / T_C,arrive=10s  
→ order: A → B(A stop) → C → A'

| Job | Completion | Arrival | Turnaround |
|-----|-------------|----------|-------------|
| A | 120s | 0s | 120s |
| B | 20s | 10s | 10s |
| C | 30s | 10s | 20s |

**Average Turnaround time** = (120+10+20)/3 = **50s**  
**Average Response time** = (0+10+20)/3 = **10s**  
→ Considers Response time as well

---

## RR (Round Robin)

→ Run a job for a time slice and then switch to the next job in the run queue until the jobs are finished.  
→ 실행 중인 job을 slice 단위로 나누어 실행한다.

---

### Compare SJF vs RR

Example: A, B, C → each job runs for 5s, all arrive at 0s

#### SJF
| Job | Completion | First Run | |
|-----|-------------|------------|
| A | 5s | 0s |
| B | 10s | 5s |
| C | 15s | 10s |

**Average Turnaround time** = (5+10+15)/3 = **10s**  
**Average Response time** = (0+5+10)/3 = **5s**

#### RR (Time slice = 1s)
| Job | Completion | First Run | |
|-----|-------------|------------|
| A | 13s | 0s |
| B | 14s | 1s |
| C | 15s | 2s |

**Average Turnaround time** = (13+14+15)/3 = **14s**  
**Average Response time** = (0+1+2)/3 = **1s**

→ Response time and time slice length are trade-offs.  
→ When switching between A→B or B→C, a **context switch overhead** occurs.

---

### Relax 3rd Assumption (All jobs arrive at the same time)

A=50s, B=50s  
Order: A→B, T_A,arrive=0s / T_B,arrive=0s

A runs for 10s and then issues an I/O request (each I/O = 10s)  
B runs CPU 50s only

| Job | Completion | Turnaround |
|------|-------------|-------------|
| A | 90s (0→10, 20→30, 40→50, 60→70, 80→90) | 90s |
| B | 140s (90→140) | 140s |

**Average Turnaround time** = (90+140)/2 = **115s**  
**Average Response time** = (0+90)/2 = **45s**

→ Using Overlap (Incorporating I/O)

---

### Incorporating I/O (Block State)

| Job | Completion | Turnaround |
|------|-------------|-------------|
| A | 90s (0→10, 20→30, 40→50, 60→70, 80→90) | 90s |
| B | 100s (10→20, 30→40, 50→60, 70→80, 90→100) | 100s |

**Average Turnaround time** = (90+100)/2 = **95s**  
**Average Response time** = (0+10)/2 = **5s**
