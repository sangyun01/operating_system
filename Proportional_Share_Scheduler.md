# Scheduling : Proportional Share Scheduler

---

## **Proportional Share Scheduler**

- Fair-share scheduler이다.  
  **HOW ?** → Using **Tickets (Tix)**  

### 종류
- Lottery Scheduling  
- Stride Scheduling  

---

## **Tickets**

- Represents the share of a resource that a process should receive.  
- The percentage of tickets represents its share of the system resource.  
  → 즉, 자원 할당량을 **tickets**으로 가중치처럼 표현한다.

**예시**

| Process | Tickets | CPU Share |
|----------|----------|------------|
| A | 75 | 75% |
| B | 25 | 25% |
| **Total** | **100** | **100%** |

---

## **Lottery Scheduling**

- The scheduler **picks a winning ticket** at random.  
  → 해당 ticket을 가진 **process**를 실행시킨다.  

**예시**
```
Total tickets = 100  
A: 0~74  
B: 75~99  
```

- **Sampling with replacement**을 사용 → 뽑은 ticket을 다시 집어넣어 확률을 유지한다.  
  → 즉, 각 프로세스는 **매번 동일한 확률**로 선택될 수 있다.  

**주의사항**
- Ticket 분배 비율과 실제 실행 시간은 완전히 일치하지 않을 수 있다.  
  - Run time이 길수록 → 평균적으로 **비율에 수렴**  
  - Run time이 짧을수록 → **unfairness** 발생 가능  

---

### **Ticket Mechanisms**

#### 1️⃣ Ticket Currency
- 사용자는 자신의 하위 job들에 ticket을 **임의의 단위(currency)** 로 나눠줄 수 있다.  
- 시스템은 이를 **global ticket 값**으로 변환한다.  

**예시**

| Process | Global Tickets | Sub-process | Local Tickets | Converted Global |
|----------|----------------|--------------|----------------|------------------|
| A | 100 | A1 | 50 | 500 |
|  |  | A2 | 50 | 500 |
| B | 100 | - | - | 100 |

→ 내부 단위를 조정함으로써 **세밀한 가중치 조절** 가능.

---

#### 2️⃣ Ticket Transfer
- 한 프로세스가 자신의 티켓을 **일시적으로 다른 프로세스에게 양보**할 수 있다.  
  → 보통 **의존 관계(dependency)** 가 있을 때 사용된다.  

**예시**
- Process A가 I/O 요청을 처리해야 하는 Process B에 의존한다면,  
  A는 자신의 티켓을 B에 양도하여 B가 먼저 실행되도록 한다.  
  이후 B가 완료되면 다시 돌려받는다.

---

#### 3️⃣ Ticket Inflation
- 프로세스가 일시적으로 자신이 가진 티켓 수를 **증가 또는 감소**시킬 수 있다.  
  → 필요 시 발행 / 불필요 시 취소  

- 단, **과도한 발행(독점)** 을 방지하기 위해 OS가 제약을 둔다.  

---

## **U : Unfairness Metric**

\[
U = \frac{\text{First job completion time}}{\text{Second job completion time}}
\]

**예시**
- 두 job의 실행시간이 각각 10이라면,  
  - 첫 번째 job 완료 : 10  
  - 두 번째 job 완료 : 20  
  \[
  U = \frac{10}{20} = 0.5
  \]
  → **U가 1에 가까울수록 공정함 (Fair)**
  
**해석**
- Job이 길수록 → 더 많은 추첨(draw)이 일어나므로 **U → 1**  
- Job이 짧을수록 → 추첨 횟수가 적어 **Unfair** 할 수 있다.

---

## **Stride Scheduling**

### **Stride 정의**
\[
\text{Stride} = \frac{\text{A Large Number}}{\text{Number of Tickets}}
\]

**예시**

| Process | Tickets | Stride |
|----------|----------|---------|
| A | 100 | 100 |
| B | 50 | 200 |

(large number = 10000 기준)

---

### **Algorithm (Pseudo Code)**

```cpp
current = remove_min(queue);        // 가장 낮은 pass value 선택
schedule(current);                  // 해당 process 실행
current->pass += current->stride;   // pass 값 갱신
insert(queue, current);             // 다시 queue에 삽입
```

---

### **Stride Scheduling Example**

Assume:
- Large number = 10000  
- Tix_A = 100, Tix_B = 50, Tix_C = 250  
→ Stride A = 100, Stride B = 200, Stride C = 40  

| Pass(A) | Pass(B) | Pass(C) | Who Runs | Comment |
|----------|----------|----------|-----------|----------|
| 0 | 0 | 0 | A | all 0 → leftmost(A) |
| 100 | 0 | 0 | B | B, C=0 → leftmost(B) |
| 100 | 200 | 0 | C | smallest pass(C) |
| 100 | 200 | 40 | C | smallest pass(C) |
| 100 | 200 | 80 | C | smallest pass(C) |
| 100 | 200 | 120 | A | smallest pass(A) |
| 200 | 200 | 120 | C | smallest pass(C) |
| 200 | 200 | 160 | C | smallest pass(C) |
| 200 | 200 | 200 | A | 반복 |

→ 실행 횟수: A=2, B=1, C=5  
→ Ticket 수에 비례함.

---

### **Stride Scheduling 특징**

- **Stride가 작을수록** 자주 실행된다.  
- **Stride가 클수록** 드물게 실행된다.  
- 단점: 새로운 process가 들어올 경우,  
  해당 process의 **pass value가 기존 프로세스와 같아질 때까지 독점** 가능.  

**예시**
- A와 C의 경우, U = 7/9 로 계산됨.  

---
