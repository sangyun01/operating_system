-   MultiProcessor Scheduling

when \~ ch.9 only consider the singleprocessor (CPU 1EA) \*\* multicore
processor -\> multicore : Multiple CPU cores are packed onto a single
chip. -\> singleprocessor에 비해 속도가 증가, but It not means cpu개수
만큼 배율에 맞추어 속도가 증가함을 의미하지는 않는다.

    - Process is only allocate one CPU
        -> Other case is using thread

\*\* hierarchical Structure reg -\> cache -\> main memory -\> Back up
str(Disk) cost ↑ cost ↓ speed ↑ speed ↓ capacity ↓ capacity ↑

cache small, fast memories hold copies of popular data that is found in
the main memory -\> CPU가 자주 쓰는 data를 cache에 저장한다. utilize
temporal and spatial locality temporal : ex loop spatial locality : ex
array -\> HW issue 일수도, spatial locality일수도 있지만, 주변의 값도
같이 오게 된다. when using the data where reg, data path is cache to
reg, not main memory to reg It means Disk -\> main memory -\> cache
memory -\> reg

\*\* Problem 1. Cache Coherence when occur Process A 동작을 CPU0 진행
main memory -\> cache로 D를 가져옴. Process A 동작을 CPU1로 migration
-\> cache에 data 필요해서 다시 main memory -\> cache로 D를 가져옴.
CPU1에서 D-\>D'로 변경 Process A가 CPU1에서 CPU0로 migration -\> CPU0
cache memory에는 D가 있다. D'가 아님 how to solve Using
write_through_method & bus Snoopng write_through_method : Data가
바뀔때마다 main memory에 값 변경 시간 측면에서 안좋음 bus Snoopng :
bus를 관찰하면서 변화가 오면 invalid 신호 및 업데이트를 한다. Using
write_back_method cache memory에서 값을 제거할 때 업데이트를 한다. 시간
측명에서는 좋지만, 그래도 데이터 문제는 있다.

    2. synchronization
        mutual exclusion primitives -> 여러 process에서 공유자원에 동시에 접근하는 것을 방지한다.
        ex)
            Process A가 Process B보다 먼저 같은 Queue에 먼저 OS에 요청함
            OS는 queue의 키를 A에게 줌. -> B는 접근을 할 수 없음
            A에서 free & unlock을 하면 B에서 접근을 할 수 있다. -> 문제가 발생하지 않음

\*\* cache Affinity Keep a process on the same CPU if at all possible
-\> 가능한 동일한 CPU에서 동작하려고 한다. -\> Coherence를 방지할 수
있다.

\*\* SQMS (Single Queue MultiProcessor Scheduling) put all jobs that
need to be scheduled into a single queue. -\> queue를 하나만 사용한다.
문제점 Lack of scalability -\> wait으로 인해 CPU가 많아도 제성능을
못한다. -\> MQMS를 적용한다. Cache Affinity가 지켜지지 않는다. -\>
pinning으로 일부 해결이 가능하다. but 구현이 어렵다 -\> scalability는
해결이 되지 않는다.

\*\* MQMS (Multi Queue MultiProcessor Scheduling) -\> information
sharing과 synchronization를 해결하였다. 각 Queue끼리 information을
share하지 않음 scalability가 향상 -\> 그렇다고 아예 X는 아니다. CPU끼리
서로 다른 정책(RR, FIFO 등)을 적용해도 된다.

    문제점
    ex)
        process 4개 중 1개가 먼저 끝나서, 하나의 CPU에는 1개, 하나의 CPU에는 2개가 남아있음. or 애초에 process가 3개
        이 중 하나만 남은 CPU의 작업도 끝난다.
            -> migration을 통해 process를 보낸다. : workload balance allocate

    ex) tricky case : monopolize를 방지하고 싶음
        using Stealing
            Source와 target을 두고, 더 적은 곳이 Source, 많은 곳이 target이다.
            이때 target에서 Source로 process를 Stealing 한다.
                -> target queue에 접근하기 위해서는 마찬가지로 synchronization으로 인하여 scalability가 발생한다.
                    -> 그래도 SQMS에 비해 적다
