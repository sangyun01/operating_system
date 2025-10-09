# Operating System Overview

---

## OS (Operating System)
> **OS operates between Hardware (HW) and Application (APP).**

---

## CPU Virtualization

- **OS can promote the illusion that many virtual CPUs exist.**  
  → CPU가 여러 개로 존재하는 것처럼 보이게 한다.  

- **How?** Using the **Time Sharing** method  
  → 아주 짧은 순간마다 CPU를 여러 프로세스가 번갈아 사용하므로 사람이 보기에는 동시에 실행되는 것처럼 느껴진다.  

- **Time and order decision:** Determined by the **Scheduling Policy**.  
  → CPU를 시간 단위로 나누어 쓰기 때문에 성능(Performance) 저하가 발생할 수 있다.

---

## Process : Running Program

| 구성 요소 | 설명 |
|------------|------|
| **Memory** | Instructions, Data Section |
| **Registers** | PC (Program Counter), SP (Stack Pointer), GPIO 등 |

---

## Process API

> These APIs are available on any modern OS.  
> 대부분의 OS에서 이름은 다를 수 있지만, 기능적으로 동일한 5가지 기능을 가진다.

1. **Create**
2. **Destroy**
3. **Wait**
4. **Miscellaneous Control**
5. **Status**

---

## Create Process (Detailed Steps)

### **Order**
**Load → Stack → Heap → Other initialization → Start Program**

#### 1. Load
- Processor는 Disk에 직접 접근 불가.  
- 따라서 Disk의 실행 파일이 **Code + Data** 형태로 **Memory**로 올라온다.  
- 모든 파일이 한 번에 올라오는 것이 아니라, **필요한 부분만(lazily)** Load된다.  
  → 전체를 한 번에 올리면 실행이 지연됨.

#### 2. Stack
- 프로그램 종료 시 사라지는 **지역 변수(local variables)**, **함수 매개변수(function parameters)** 저장.  
- Stack이 할당될 때, `argc`, `argv` 배열이 초기화되어 main() 함수의 인자로 전달된다.

#### 3. Heap
- 사용자가 얼마나 공간을 사용할지 모를 때 **동적 할당 영역**.  
- `malloc()`으로 공간을 확보하고, `free()`로 해제.

#### 4. Other Initialization Tasks
- I/O 요청 등 나머지 초기화 수행.  
- 각 Process는 **input, output, error** 3개의 descriptor를 가짐.

#### 5. Start Program
- Stack의 `argc`, `argv` 값을 이용하여 **main() 함수 실행**.

---

### **Program Execution Flow**
프로그램은 실행되기 전 **Disk**에 존재하며 **executable format(실행 파일 형태)** 로 저장되어 있다.

$$
\text{Disk → Memory → CPU}
$$

| 단계 | 설명 |
|------|------|
| **Disk** | 실행 전 Code, Data가 존재 (예: SSD에 저장된 executable) |
| **Memory** | 프로그램 실행 시 Code, Data가 Memory로 Load됨 |
| **Memory** | Stack과 Heap이 생성됨 |
| **CPU (Processor)** | Memory의 내용을 Fetch하여 Register에 저장 |

---

## Process States

A process can be in one of **three states**:

| 상태 | 설명 |
|------|------|
| **Running** | 프로세스가 CPU에서 실행 중 |
| **Ready** | 실행 가능하지만, CPU를 아직 배정받지 않음 |
| **Blocked** | I/O 요청 등으로 CPU를 사용하지 못하는 상태 |

### **예시: I/O 요청**
- CPU는 Disk에 직접 접근 불가.  
- Disk I/O 요청 시, Disk → Memory로 데이터를 불러오는 동안 **다른 Process를 실행**시켜 CPU 활용도를 높임.  
- 그렇지 않으면 CPU 자원이 낭비됨.

---

## Process State Transition

```
Running ↔ Ready     : Scheduler
Running → Blocked → Ready → Running
Running → Blocked   : I/O initiate
Blocked → Ready     : I/O done
```

---

## Data Structures

### **PCB (Process Control Block)**
- 각 프로세스의 정보를 포함.  
- 주요 항목:
  - PID (Process ID)
  - PC (Program Counter)
  - Process Name
  - Location
  - **Register Context:** 프로세스 상태를 정의하는 레지스터 집합

### **Process List**
- Ready Processes  
- Blocked Processes  
- Current Running Process
