## Low level performance optimization
Artem Frolov
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
* Intrinsics
* Vectorization
* Loop-level parallelization
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
* Enumerrated types
---
## Examples of non-zero-cost abstractions
* Automatic memory management
* Virtual calls
* Value boxing/unboxing
---
## Cache locality
* Memory is cached by chunks (cache lines)
* Accesses better be bunched together in time and space
* Arrays: successive > random
* Data structures: less pointers
* Lists and hash tables: need chunking optimization
* Temporal locality: recently referenced are more likely to be referenced in future
* Spatial locality: items closely together tend ti be referenced colse together in time
---
## Data layout
* Array of structures (AOS)
* Structure of arrays (SOA)
---
## Array of structures and structure of Arrays
```c
struct pixel_t {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct pixel_t ArrayOfStructures[N];

struct struct_of_arrays_t {
	uint8_t r[N];
	uint8_t g[N];
	uint8_t b[N];	
} StructureOfArrays;
```
	
---
## Unnecessary data copying
* Const reference
---
## Branch prediction
---
## OpenMP
* Fine-grained parallelization
---
## Vectorization
---
## Intrinsics
---
## Micro/Macro Fusion