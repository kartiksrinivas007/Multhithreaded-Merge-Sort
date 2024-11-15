# Multithreaded-Merge-Sort
## Analysis of Multi threaded merge sort on a 6 core Machine.
---

This is a simple implementation and analysis of the merge sort algorithm with multiple threads.
There are two modes of execution for analyzing the effects of multiple threads -- the first mode takes an input from the user and performs a multithreaded merge sort. The second generates random arrays of variable size and reports statistics on the amount of time taken.

Please see `report.pdf` for the analysis.

Compilation on UNIX OS : 
Compilation : 
```
gcc -o merge MergeSort.c -lrt -pthread
```

Running(UNIX OS ON TERMINAL) : 
```
./merge
```
Input `1` for accepting the number of threads and testing the program 
Input `2` for time Analysis on randomly generated Arrays

the file `SortedArrays.txt` will be generated in your directory in case 1 containing the sorted array

---
