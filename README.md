# openmp-number-analysis

This C program demonstrates the performance difference between sequential and parallel processing using OpenMP. It processes an array of `n` integers using `N` threads and performs the following tasks:

- **Array generation** (0 to n-1)
- **Classifying each number** as deficient, perfect, or abundant
- **Counting how many numbers belong to each category**
- **Sorting the array** so that deficient numbers come first, then perfect, then abundant

Each step is executed twice:
- Once **sequentially**
- Once using **OpenMP parallelization**

Execution time and speedup are measured and displayed for each phase.


## Note: 


A number is called perfect if the sum of its proper divisors equals the number itself (e.g., 28 is perfect because 28 = 1 + 2 + 4 + 7 + 14).
A number is called abundant if the sum of its proper divisors is greater than the number (e.g., 36 is abundant because 1+2+3+4+6+9+12+18=55 > 36).
A number is called deficient if the sum of its proper divisors is less than the number (e.g., 8 is deficient because 1+2+4=7 < 8).


## Build and Run

You can compile the program with or without optimization:

### With optimization (recommended for performance testing)
gcc -fopenmp -O2 source.c -o openmp-demo

### Or without optimization (easier for debugging, but slower)
gcc -fopenmp source.c -o openmp-demo

### Run the program (regardless of how it was compiled)
./openmp-demo <number_of_elements> <number_of_threads>

