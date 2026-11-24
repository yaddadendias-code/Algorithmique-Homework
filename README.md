# Sorting Algorithms Assignment

Université Badji Mokhtar - Annaba  
Computer Science Department  
Module: Algorithms and Data Structures  
Programming language: C11  
Author: Yaddaden Dias  
Group: [Enter your group]  
Academic year: 2026/2027

## Project overview

This project implements Bubble Sort, Selection Sort, Insertion Sort, Merge Sort and Quick Sort. It contains an interactive menu, manual array input and display, CPU-time measurements, repeated benchmarks, CSV export and a 100,000-element example.

## Libraries

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
```

- `stdio.h`: keyboard input, screen output and CSV files.
- `stdlib.h`: `malloc`, `free`, `rand` and `srand`.
- `string.h`: `memcpy` to copy the same original input before each measurement.
- `stdbool.h`: Boolean values used by input validation and Bubble Sort.
- `math.h`: included as part of the project headers; no mathematical function is currently needed by these sorting algorithms.
- `time.h`: `clock()` and `CLOCKS_PER_SEC` for CPU-time measurement.

## Compile and run

Open the terminal in the project root directory.

### MSYS2 UCRT64 on Windows

```bash
gcc -std=c11 -O2 -Wall -Wextra src/main.c -o sorting.exe
./sorting.exe
```

### Linux

```bash
gcc -std=c11 -O2 -Wall -Wextra src/main.c -o sorting
./sorting
```

The program uses only standard C headers. It does not include Windows-specific headers or POSIX process-memory headers.

## Menu

| Option | Action |
|---:|---|
| 1 | Bubble Sort: manually enter an array, then display the original and sorted arrays. |
| 2 | Selection Sort: manually enter an array, then display the original and sorted arrays. |
| 3 | Insertion Sort: manually enter an array, then display the original and sorted arrays. |
| 4 | Merge Sort: manually enter an array, then display the original and sorted arrays. |
| 5 | Quick Sort: manually enter an array, then display the original and sorted arrays. |
| 6 | Compare all algorithms over five test sizes and export `results/comparison.csv`. |
| 7 | Run Merge Sort and Quick Sort on 100,000 random integers and export `results/big_array.csv`. |
| 0 | Exit. |

For options 1 to 5, the user enters up to 100 integer elements manually. The program displays the original array and its sorted result.

## Comparison sizes

Option 6 uses these five array sizes:

| Array size | Bubble Sort | Selection Sort | Insertion Sort | Merge Sort | Quick Sort |
|---:|---|---|---|---|---|
| 1,000 | Run | Run | Run | Run | Run |
| 5,000 | Run | Run | Run | Run | Run |
| 10,000 | Skipped | Skipped | Skipped | Run | Run |
| 50,000 | Skipped | Skipped | Skipped | Run | Run |
| 100,000 | Skipped | Skipped | Skipped | Run | Run |

The quadratic algorithms are limited to 5,000 elements to avoid excessively long runs. Merge Sort and Quick Sort are measured at all five sizes. The input data is generated with a fixed seed (`2026`), and each algorithm receives the same original array for a given size. Each measurement is repeated five times. The copy operation and sorted-result verification are outside the timed interval.

## Large-array example

Option 7 generates 100,000 random integers using a fixed seed (`2026`). It runs Merge Sort and Quick Sort on separate copies of the same data. It checks that the result is sorted and displays the first and last ten values rather than printing all 100,000 elements. Bubble Sort, Selection Sort and Insertion Sort are skipped for this large example because their average and worst-case time complexities are quadratic.

## CPU-time measurement

The program uses the standard C `clock()` function from `time.h`:

```c
clock_t start = clock();
sortFunction(workArray, size);
clock_t end = clock();

double milliseconds =
    1000.0 * (double)(end - start) / CLOCKS_PER_SEC;
```

The array copy is performed before the start time is recorded. The correctness check and output are performed after the end time is recorded. Five runs are performed and the average CPU time is reported. Very small arrays may display `0.000 ms` because the measured operation can be shorter than the timer resolution.

## Memory reporting limitation

The program tracks the extra heap memory explicitly allocated inside the sorting functions. Merge Sort allocates one temporary array, so its extra heap allocation is tracked with `trackedMalloc()` and `trackedFree()`. The other sorting functions do not explicitly allocate heap arrays. The program also reports the maximum recursion depth observed in Merge Sort and Quick Sort.

This is **not the operating system's total peak process memory**. It does not measure the fixed input arrays, the C runtime, or the exact number of bytes used by recursive stack frames. The assignment asks for peak memory obtained through a language or operating-system API. A full process-level peak-memory measurement therefore remains unavailable in this standard-library-only version; it would require a platform-specific API such as `getrusage()` or the Windows process-memory API. These headers were intentionally left out to keep the project compatible with the requested standard-header set.

## Algorithm descriptions and theoretical complexity

| Algorithm | Best time | Average time | Worst time | Auxiliary space |
|---|---:|---:|---:|---:|
| Bubble Sort with early exit | O(n) | O(n²) | O(n²) | O(1) |
| Selection Sort | O(n²) | O(n²) | O(n²) | O(1) |
| Insertion Sort | O(n) | O(n²) | O(n²) | O(1) |
| Merge Sort | O(n log n) | O(n log n) | O(n log n) | O(n) |
| Quick Sort | O(n log n) | O(n log n) | O(n²) | O(log n) stack depth in this implementation because it recursively processes the smaller partition |

Quick Sort uses the last array element as its pivot. The implementation processes one partition iteratively and recursively processes the smaller partition to keep the call-stack depth at O(log n), while the worst-case running time remains O(n²).

## Results and machine specifications

Run option 6 and option 7 on the computer used for the assignment. The CSV files are generated in the `results/` folder. Add the real outputs and the machine specifications below before submission.

- CPU: [Enter CPU model]
- RAM: [Enter installed RAM]
- Operating system: [Enter OS and version]
- Compiler: [Enter GCC version]

Do not copy benchmark values from another computer. Execution times depend on the processor, compiler, compiler options and operating system.

## GitHub submission checklist

1. Keep the repository public.
2. Upload `src/main.c`, `README.md`, `.gitignore` and the generated CSV result files.
3. Run the program on the target computer and add its actual CPU, RAM, OS and GCC information to this README.
4. Review the CSV results and write a short critical analysis comparing the experimental results with theoretical complexities.
5. Open the repository in a private browser window to confirm that the files are public and accessible.

## Critical analysis template

After running the experiments, complete this section using your own results:

- Which algorithm was fastest for the tested small arrays?
- How did Merge Sort and Quick Sort behave as the array size increased?
- Did the results for Bubble Sort, Selection Sort and Insertion Sort show the expected quadratic growth?
- Did the observed results match the theoretical complexity? Explain any differences.
- What limitations affected the measurements?
