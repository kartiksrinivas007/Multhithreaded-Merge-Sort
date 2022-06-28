# Multithreaded-Merge-Sort
Analysis of Multi threaded And single threaded Merge Sort Algorithms On a 6 core Machine.


This program has several functions.Each is explained in detail in the report file.
The C program contains 2 parts, Namley Input analysis and Time Analysis. 
The Input Analysis accepts the number of threads from user and performs a Multithreaded merge sort on the array given by the user.
Time Analysis generates Random arrays of huge size and then rins both sequential and multithreaded merge sort and finds the time taken by each of them to sort the array.

# Method 
---
Compilation on UNIX OS : 


Compilation : 
```
gcc -o assgn2 Assgn2_ES15.c -lrt -pthread
```

Running(UNIX OS ON TERMINAL) : 

```
./assgn2
```
Input `1` for accepting the number of threads and testing the program 
Input `2` for time Analysis on randomly generated Arrays


the file `SortedArrays.txt` will be generated in your directory in case 1 containing the sorted array

---
