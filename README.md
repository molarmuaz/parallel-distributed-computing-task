### Graph File Analysis

I have always found multithreading and synchronization to be my weak spots in computer science, and this task was definitely something I had been dreading. But, in the small time span allotted and my very limited knowledge of this topic, here is what I made:

In this assignment, I implemented a multithreaded solution using the pthreads library in C, to analyze a large graph txt file. The file represents an undirected graph where each line contains two node IDs that indicate an edge between the nodes. 

Link: https://snap.stanford.edu/data/com-Orkut.html

The goal was to compute the total number of nodes and edges efficiently by dividing the work among multiple threads. Each thread processed an equal segment of the file, calculated the degrees of nodes, and kept track of the edge count locally i.e. did not use a shared variable. Synchronization was managed using mutexes to ensure correct updates to shared data.

Despite implementing a somewhat efficient solution, I encountered some minor inaccuracies in the final results that I couldn’t resolve. The calculated values for the total nodes and edges are slightly different from the expected values. These differences are shown below, along with the time taken to process the file. I could have also increased efficiency by playing around with affinity and allocating bigger segments to stronger cores and vice versa.


1 Thread:
![[Pasted image 20250201180332.png]](https://github.com/molarmuaz/parallel-distributed-computing-task/blob/main/img/Pasted%20image%20250201180332.png)

2 Threads:
![[Pasted image 20250201173737.png]](https://github.com/molarmuaz/parallel-distributed-computing-task/blob/main/img/Pasted%20image%20250201173737.png)

4 Threads:
![[Pasted image 20250201173235.png]](https://github.com/molarmuaz/parallel-distributed-computing-task/blob/main/img/Pasted%20image%2020250201173235.png)

8 Threads:
![[Pasted image 20250201174523.png]](https://github.com/molarmuaz/parallel-distributed-computing-task/blob/main/img/Pasted%20image%20250201174523.png)

16 Threads:
![[Pasted image 20250201174752.png]](https://github.com/molarmuaz/parallel-distributed-computing-task/blob/main/img/Pasted%20image%20250201174752.png)
