## Performance optimization
Artem Frolov
---
## General guidelines
* Always be profiling
* Do not assume - measure!
* Make it compile, make it right, make it fast
* Set targets
* Concentrate on low-hanging fruit first
* Do not reinvent the wheel

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
(With understanding of CPU and compilation)

* Cache locality
* CPU pipeline
* Vectorization
* Loop-level parallelization
---
## Data serialization
* Verbose textual formats: good for debugging, bad for speed
* Keep data in memory as much as possible
* On-disk
  * Key-value stores (memcached, redis)
  * Relational databases
  * NoSQL databases
  * HDF5
---
## Algorithms and data structures
* Binary search vs. Linear: O(log N) vs. O(N)
* rb-trees and hash maps vs. vectors
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
## Zero-cost abstraction
> What you don't use, you don't pay for. What you use, you couldn't hand code any better
> -- Bjarne Stroustrup
---
## Examples of zero-cost abstractions
* Basic C types
* C++ templates
* Enumerated types
---
## Examples of non-zero-cost abstractions
* Automatic memory management
* Virtual calls
* Value boxing/unboxing
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
inline size_t matrix_idx( double* matrix, size_t n_row_size,
    size_t n_column_size, size_t i, size_t j) {
#if MATRIX_ORDER_ROW_MAJOR
	return i * n_row_size + j;
#else
	return j * n_column_size + i;
#endif
}
inline double matrix_get(double* matrix, size_t n_row_size,
    size_t n_column_size, size_t i, size_t j) {
	return matrix [matrix_idx(matrix, n_row_size, n_column_size, i, j)];
}
inline void matrix_set(double* matrix, size_t n_row_size,
    size_t n_column_size, size_t i, size_t j, double val) {
	matrix [matrix_idx(matrix, n_row_size, n_column_size, i, j)] = val;
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
## Vectorization
```c
float a[8192], b[8192], c[8192];
for (i=0; i<n; i++)
    c[i] = a[i] + b[i];
```
```c
for (i=0; i<n; i+=4) {
	/* vectorized into single instruction */
    c[i + 0] = a[i + 0] + b[i + 0];
    c[i + 1] = a[i + 1] + b[i + 1];
    c[i + 2] = a[i + 2] + b[i + 2];
    c[i + 3] = a[i + 3] + b[i + 3];
}
```	
---

## Unnecessary data copying
* Const reference
---
## CPU Instruction pipeline
* Fetch
* Decode
* Execute
* Memory Access
* Register write back
---
## CPU Instruction pipeline chart

<table border="1">
<tr><th>Instr#</th><th colspan="7" align="center">Stage</th></tr>
<tr><td>1</td> <td>IF</td> <td>ID</td> <td>EX</td> <td bgcolor="green">MEM</td> <td>WB</td> <td> </td> <td> </td></tr>
<tr><td>2</td> <td>  </td> <td>IF</td> <td>ID</td> <td bgcolor="green">EX</td> <td>MEM</td> <td>WB</td> <td> </td></tr>
<tr><td>3</td> <td>  </td> <td>  </td> <td>IF</td> <td bgcolor="green">ID</td> <td>EX</td> <td>MEM</td> <td>WB</td></tr>
<tr><td>4</td> <td>  </td> <td>  </td> <td>  </td> <td bgcolor="green">IF</td> <td>ID</td> <td>EX</td> <td>MEM</td></tr>
<tr><td>Clock cycle</td> <td>1</td> <td>2</td> <td>3</td> <td bgcolor="green">4</td> <td>5</td> <td>6</td> <td>7</td></tr>
</table>
---
## Implications of pipelining
* Data dependence between instructions: reordering improves performance
* On branching instructions CPU has to choose what to pipeline
* CPUs use reasonable heuristics for prediction
* Misprediction causes pipeline restart
---
## Helping CPU with prediction
* Static hints
```c
if (unlikely(v == some_unlikely_value)) {
	/* do something here */
}
```
* Profile-guided optimization (PGO)
* Makes sense for hot loops
---
## OpenMP
* Fine-grained parallelization
