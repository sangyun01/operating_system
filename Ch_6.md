# Limited Direct Execution (LDE)

---

## Mechanism : Limited Direct Execution

**OS needs to share the physical CPU by time sharing.**  
→ OS는 제한된 CPU를 time sharing 기법으로 사용하여야 한다.

### Time sharing method has 2 Issues
1. **Performance** : Overhead를 최소화해야 한다.  
2. **Control** : OS가 제어권을 잃으면 안 된다.

---

## Control

### Direct Execution
> Run the program directly on the CPU — 프로그램을 CPU에 직접적으로 올려 실행하는 상황

#### Process (Direct Execution)
#### Process (Direct Execution)
| # | OS (Kernel Mode) | Hardware | APP (User Mode) | Comments |
|---|------------------|-----------|------------------|-----------|
| 1 | Create entry for process list |  |  | Process list와 PCB를 생성 (kernel mode에서 수행) |
| 2 | Allocate memory for program |  |  | 프로그램을 위한 메모리 공간을 커널이 할당 |
| 3 | Load program into memory | Disk → Memory |  | 디스크에 저장된 프로그램을 메모리로 로드 (code, data 포함) |
| 4 | Set up stack with argc / argv |  |  | Stack, Heap 초기화 및 argc/argv 설정 |
| 5 | Clear registers |  |  | CPU 레지스터 초기화 (이전 실행 정보 제거) |
| 6 | Execute call main |  |  | main 함수 실행 준비 완료 |
| 7 |  |  | Run main | 사용자 코드(main 함수) 실행 시작 |
| 8 |  |  | Execute return from main | main 함수 종료 후 반환 |
| 9 | Free memory of process |  |  | 프로그램 종료 후 커널이 메모리 해제 |
| 10 | Remove from process list |  |  | PCB 및 Process list에서 해당 프로세스 제거 |


> **문제점:**  
> OS는 단순히 프로그램을 실행시키는 라이브러리 역할에 불과 → 제어권(Control)이 없다.

---

### When the OS has not control, problems occur

#### Problem 1 — 잘못된 접근을 OS가 막지 못함

예시:
```c
int *i;
i = 0;      // 대부분의 OS에서 주소 0에는 중요한 data 존재
*i = 1;     // 중요한 data를 1로 변경 → 시스템 고장 유발
```

또는,
> 프로세스가 디스크 I/O 요청, CPU나 메모리에 직접 접근하는 등 restricted operation 수행 시 문제 발생

#### ✅ Solution

**User mode와 Kernel mode를 분리한다.**

| Mode | 설명 |
|------|------|
| **User mode** | Applications do not have full access to hardware resources.<br>→ APP은 제한된 접근만 가능 |
| **Kernel mode** | The OS has access to the full resources of the machine.<br>→ OS는 모든 자원 접근 가능 |

User mode ↔ Kernel mode 전환은 **System Call**을 통해 수행된다.

---

### System Call

#### Trap Instruction : User → Kernel

- **Trap Table**  
  : 예외 상황에 대한 번호와 그에 해당하는 Trap Handler 주소를 저장한 테이블  
  예) 3번 예외 → 0x08 주소의 Trap Handler

- **Trap Handler**  
  : 커널 모드에서 실제 코드를 실행하는 루틴

#### Return from Trap Instruction : Kernel → User

---

## Process (Limited Direct Execution)

---

### @ Boot

| # | OS | HW | APP | Comments |
|---|----|----|-----|-----------|
| 1 | Initialize trap table |  |  | OS에서 trap table 생성 후 HW에 등록 |
| 2 |  | Remember address of ... |  | HW가 trap table 주소를 기억 |
| 3 |  | Syscall handler |  | HW가 시스템 콜 발생 시 호출할 handler 설정 |

---

### @ Run

| # | OS | HW | APP | Comments |
|---|----|----|-----|-----------|
| 1 | Create entry for process list |  |  | PCB, process list 생성 |
| 2 | Allocate memory for Program |  |  | 프로그램을 위한 메모리 할당 |
| 3 | Load program into memory |  |  | Disk → Memory 로 program load |
| 4 | Setup user stack with argv |  |  | Stack, I/O 등 초기화 |
| 5 | Fill kernel stack with reg/PC |  |  | Kernel stack에 reg(0) 값과 PC(main) 저장 |
| 6 | Return-from-trap |  |  | Kernel → User 전환 준비 |
| 7 |  | Restore regs from kernel stack |  | Kernel stack 값 복원 |
| 8 |  | Move to user mode |  | user mode로 전환 |
| 9 |  | Jump to main |  | HW → APP 점프 |
| 10 |  |  | Run main()... | main 함수 실행 |
| 11 |  |  | Call system | System call 발생 |
| 12 |  |  | Trap into OS | user → kernel 진입 |
| 13 |  | Save regs to kernel stack |  | 현재 register 값을 kernel stack에 저장 |
| 14 |  | Move to kernel mode |  | kernel mode로 진입 |
| 15 |  | Jump to trap handler |  | trap table 참조 → handler 주소로 이동 |
| 16 | Handle trap |  |  | trap 처리 |
| 17 | Do work of syscall |  |  | 실제 system call 코드 실행 |
| 18 | Return-from-trap |  |  | kernel → user 복귀 |
| 19 |  | Restore regs from kernel stack |  | 저장된 register 복원 |
| 20 |  | Move to user mode |  | user mode로 복귀 |
| 21 |  | Jump to PC after trap |  | trap 직전의 PC로 복귀 |
| 22 |  |  | Return from main | main 복귀 |
| 23 |  |  | Trap (via exit()) | process 종료를 위해 user → kernel |
| 24 | Free memory of process |  |  | process 메모리 해제 |
| 25 | Remove from process list |  |  | PCB, process list 제거 |


---

## Problem 2 — OS가 제어권(Control)을 돌려받지 못할 수 있다.

예시:
```c
i = -1;
while (i < 0) // inf loop → return하지 않음 → CPU 제어 불가
    do something;
```

---

### ✅ Solution

#### A Cooperative Approach — 자발적 방식
- 프로세스가 주기적으로 CPU를 OS에게 양보 (`yield()` 등)
- OS는 **System Call 발생 시점에만** 제어권을 얻는다.

**문제:**  
비협조적인 프로세스(무한 루프 등)는 제어권을 반환하지 않음.

---

#### A Non-Cooperative Approach — 강압적 방식
- 위 문제를 해결하기 위한 방법  
- OS가 시작 시 **하드웨어 타이머(timer)** 를 설정  
- 일정 시간마다 **Timer Interrupt** 발생 → OS가 제어권을 강제로 회수

---

## Why OS Needs the Control

> Scheduler가 Process A → Process B로 state를 변경하려면?  
> → **Context Switch** 필요

---

## Context Switch

> A low-level piece of assembly code that switches CPU context from one process to another.

1. **save a few register values for the current process onto its kernel stack**
2. **timer interrupt 발생**
3. **Scheduler가 다음 프로세스(B)를 선택**
4. **context switch가 A → B로 전환**

---

## Process (Limited Direct Execution Protocol with Timer Interrupt)

---

### @ Boot

| # | OS | HW | APP | Comments |
|---|----|----|-----|-----------|
| 1 | Initialize trap table |  |  | OS에서 trap table 생성 |
| 2 |  | Remember address of ... |  | HW가 handler 주소 기억 |
| 3 |  | Syscall handler |  | 시스템 콜 처리 루틴 |
| 4 |  | Timer handler |  | timer interrupt 처리 루틴 |
| 5 | Start interrupt timer |  |  | 커널이 timer 시작 |
| 6 |  | Start Timer (HW) |  | 하드웨어에서 주기적 interrupt 발생 |
| 7 |  | Interrupt CPU in X ms |  | Xms마다 인터럽트 발생 |

---

### @ Run

| # | OS | HW | APP | Comments |
|---|----|----|-----|-----------|
| 1 |  |  | Process A | 실행 중 |
| 2 |  | Timer interrupt (HW) 발생 |  | 하드웨어 타이머가 인터럽트 발생 |
| 3 |  | Save regs(A) to k-stack(A) |  | A의 레지스터 값을 커널 스택에 저장 |
| 4 |  | Move to kernel mode |  | User → Kernel 전환 |
| 5 |  | Jump to trap handler |  | trap handler로 이동 |
| 6 | Handle the trap |  |  | trap 처리 |
| 7 | Call switch() routine |  |  | context switch 실행 |
| 7-1 | save regs(A) to proc-struct(A) |  |  | PCB(A)에 레지스터 저장 |
| 7-2 | restore regs(B) from proc-struct(B) |  |  | PCB(B)의 레지스터 복원 |
| 7-3 | switch to k-stack(B) |  |  | 커널 스택을 B의 것으로 변경 |
| 8 | Return-from-trap (into B) |  |  | Kernel → User 복귀 |
| 9 |  | Restore regs(B) from k-stack(B) |  | B의 레지스터 복원 |
| 10 |  | Move to user mode |  | user mode 전환 |
| 11 |  | Jump to B’s PC |  | B의 PC 주소로 점프 |
| 12 |  |  | Process B 실행 | 프로세스 B가 실행됨 |

---

## 🧩 Summary

| 구분 | Cooperative | Non-Cooperative |
|------|--------------|----------------|
| **제어권 반환 방식** | 프로세스가 자발적으로 넘겨줌 (System Call, yield 등) | OS가 하드웨어 타이머로 강제 회수 |
| **장점** | 구현 간단 | 항상 CPU 제어권 보장 |
| **단점** | 무한 루프 시 시스템 정지 가능 | 약간의 하드웨어 오버헤드 |
| **현대 OS** | 거의 사용 안 함 | ✅ 기본 방식 (Preemptive Scheduling) |

---
