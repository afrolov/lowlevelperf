#### TD Tech talks
## Low-level performance optimization: Cache Locality and OpenMP

#### Artem Frolov
Quantitative development, UQL

---
## High level performance optimization

(Largely CPU and language independent)

* Algorithms
* Data structures
* Asynchronous I/O
* Binary serialization
* Database indices
* Task parallelization
---
## Low level performance optimization
(With understanding of CPU, memory, and compilation)

* Cache locality
* CPU pipeline
* Vectorization
* Data parallelization
---
## This talk
* Cache locality
* Parallelization with OpenMP
---
## High-level vs. low-level
* Most code (esp. user facing) is I/O bound
* Low-level makes most sense for CPU-bound and memory-bound code
  * Quantitative libraries
  * Scientific and technical computing
  * System-level code (drivers, DBs, graphical libraries,...)
* Do low-level opt only after high-level opt  
---

## Latency numbers every programmer should know

<table>
<tr><th>operation</th><th>time (ns)</th><th>time (ms)</th><th>comment</th></tr>
<tr><td>L1 cache reference</td><td align='right'>0.5 ns</td><td></td><td></td></tr>
<tr><td>Branch mispredict</td><td align='right'>5 ns</td><td></td><td></td></tr>
<tr><td>L2 cache reference</td><td align='right'>7 ns</td><td></td><td></td></tr>
<tr><td>Mutex lock/unlock</td><td align='right'>25 ns</td><td></td><td></td></tr>
<tr><td>RAM access</td><td align='right'>100 ns</td><td></td><td></td></tr>
<tr><td>Compress 1KB (Zippy)</td><td align='right'>3000 ns</td><td></td><td></td></tr>
</table>
---

## Latency numbers (2/3)

<table>
<tr><th>operation</th><th>time (ns)</th><th>time (ms)</th><th>comment</th></tr>
<tr><td>Send 1K bytes over 1Gbps network</td><td align='right'>10,000 ns</td><td align='right'>0.01 ms</td><td></td></tr>
<tr><td>Read 4K randomly from SSD</td><td align='right'>150,000 ns</td><td align='right'>0.15 ms</td><td></td></tr>
<tr><td>Read 1MB randomly from memory</td><td align='right'>250,000 ns</td><td align='right'>0.25 ms</td><td></td></tr>
<tr><td>Round trip within datacenter</td><td align='right'>500,000 ns</td><td align='right0'>0.5 ms</td><td></td></tr>
</table>
---
## Latency numbers (3/3)
<table>
<tr><th>operation</th><th>time (ns)</th><th>time (ms)</th><th>comment</th></tr>
<tr><td>Read 1 MB sequentially from SSD</td><td align='right'>1,000,000 ns</td><td align='right'>1 ms</td><td></td></tr>
<tr><td>Disk seek</td><td align='right'>10,000,000 ns</td><td align='right'>10 ms</td><td>20X datacenter</td></tr>
<tr><td>Read 1MB sequentially from disk</td><td align='right'>20,000,000 ns</td><td align='right'>20 ms</td><td>80X RAM, 20X SSD</td></tr>
<tr><td>Send packet CA, US -> NL -> CA, US</td><td align='right'>150,000,000 ns</td><td align='right'>150 ms</td><td></td></tr>
</table>

---
## Cache locality
* Memory is cached by chunks (cache lines)
* Accesses better be bunched together in time and space
* Arrays: successive > random, smaller stride
* Data structures: less pointers
* Lists and hash tables: need chunking optimization
* Temporal locality: recently referenced are more likely to be referenced in future
* Spatial locality: items closely together tend ti be referenced close together in time
---
## Data layout
* Matrices: row-major vs column-major
* Array of structures (AOS)
* Structure of arrays (SOA)
---
#### Row-major vs. Column-major

##### Matrix
<table border="1">
<tr bgcolor="red"><td>11</td><td>12</td><td>13</td><td>14</td></tr>
<tr bgcolor="green"><td>21</td><td>22</td><td>23</td><td>24</td></tr>
<tr bgcolor="blue"><td>31</td><td>32</td><td>33</td><td>34</td></tr>
</table>

##### Row-major
<table border="1">
<tr>
<td bgcolor="red">11</td><td bgcolor="red">12</td><td bgcolor="red">13</td><td bgcolor="red">14</td>
<td bgcolor="green">21</td><td bgcolor="green">22</td><td bgcolor="green">23</td><td bgcolor="green">24</td>
<td bgcolor="blue">31</td><td bgcolor="blue">32</td><td bgcolor="blue">33</td><td bgcolor="blue">34</td>
</tr>
</table>

##### Column-major
<table border="1">
<tr>
<td bgcolor="red">11</td><td bgcolor="green">21</td><td bgcolor="blue">31</td>
<td bgcolor="red">12</td><td bgcolor="green">22</td><td bgcolor="blue">32</td>
<td bgcolor="red">13</td><td bgcolor="green">23</td><td bgcolor="blue">33</td>
<td bgcolor="red">14</td><td bgcolor="green">24</td><td bgcolor="blue">34</td>
</tr>
</table>
---
### Testing Pearson correlation
```c
inline size_t matrix_idx(size_t n_row_size,
    size_t n_column_size, size_t i, size_t j) {
#if MATRIX_ORDER_ROW_MAJOR
	return i * n_row_size + j;
#else
	return j * n_column_size + i;
#endif
}
inline double matrix_get(double* mtx, size_t n_row_sz,
    size_t n_col_sz, size_t i, size_t j) {
	return mtx[matrix_idx(matrix, n_row_sz, n_col_sz, i, j)];
}
inline void matrix_set(double* mtx, size_t n_row_sz,
    size_t n_col_sz, size_t i, size_t j, double val) {
	mtx[matrix_idx(matrix, n_row_sz, n_col_sz, i, j)] = val;
}

```
---
### Pearson correlation results

<table>
<tr><th>Series number and length</th><th>Row-major time</th><th>Column-major time</th><th>Factor</th></tr>
<tr><td>256</td><td>0.033</td><td>0.093</td><td>2.8x</td></tr>
<tr><td>512</td><td>0.182</td><td>0.381</td><td>2.0x</td></tr>
<tr><td>1024</td><td>1.445</td><td>13.884</td><td>9.6x</td></tr>
<tr><td>2048</td><td>11.49</td><td>143.66</td><td>12.5x</td></tr>
<tr><td>4096</td><td>93.37</td><td>1409.39</td><td>15x</td></tr>
</table>

---
## Array of structures and structure of Arrays
```c
struct pixel_t {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} ArrayOfStructures[N];

struct struct_of_arrays_t {
	uint8_t r[N];
	uint8_t g[N];
	uint8_t b[N];	
} StructureOfArrays;
```
---
## Cache locality: Demo
---
## OpenMP
* Fine-grained parallelization
* API to support shared memory multiprocessing
* C, C++, and Fortran
* Set of compiler pragmas

---
## OpenMP: how it works
* For each section *master* thread *forks* a number of *slave* threads
* Work sharing constructs specify division of work ans synchronization
* At the end of section *slave* threads *join* into the master thread
---
## OpenMP: simplest example
```c
float a[8192], b[8192], c[8192];
#pragma omp for
for (i=0; i<sizeof(a)/sizeof(a[0]); i++)	
    c[i] = a[i] + b[i];
```
---
## OpenMP: thread-local storage, scheduling, and critical sections
```c
float a[8192], b[8192], c[8192];
double sum = 0.0, loc_sum = 0.0;
#pragma omp parallel private(loc_sum)
{
	#pragma omp for(static, 1)
	for (i=0; i<sizeof(a)/sizeof(a[0]); i++) {
    	c[i] = a[i] * b[i];
   		loc_sum += c[i];
	}
    #pragma omp critical
    sum = sum + loc_sum;
}
```

---
## OpenMP: support for reduction
```c
float a[8192], b[8192], c[8192];
long sum = 0;
 #pragma omp parallel for reduction(+:sum) schedule(static,1)
 for(i = 0; i < N; i++) {      
     sum = sum + a[i]*b[i];
 }
```

---
## OpenMP: dynamic scheduling
```c
#pragma omp parallel private(j,k) 
{  
  #pragma omp for schedule(dynamic, 1)
  for(i = 2; i <= N-1; i++)
     for(j = 2; j <= i; j++)
        for(k = 1; k <= M; k++)
           b[i][j] += a[i-1][j]/k + a[i+1][j]/k;
}
```
---
## OpenMP: Demo
---
## OpenMP

* pros
  * Simple
  * Portable
  * Piece-meal approach to parallelization
* cons
  * Risk of race conditions
  * Maybe difficult to debug
  * Error-handling is compilcated
---
# Thank you!
## Q&amp;A 
Artem.Frolov@tdsecurities.com