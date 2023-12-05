# COL 216 ASSIGNMENT 3

This is the implementation of the cache simulator for a processor in the c++ language.

## FILES

1. `cache_simulate.cpp` : This file contains the code for the cache simulator.
2. `memory_trace_files` : This folder contains the memory trace files.
3. `makefile` : This file contains the commands to compile and run the code.
4. `combine.py` : This file contains the code to combine the memory trace files plots in the folders `plot1`,`plot2`,`plot3`,`plot4` and `plot5` into a single file `graph_data.txt`.
5. `Report.pdf` : This file contains the report for the assignment.
6. `plot1`,`plot2`,`plot3`,`plot4` and `plot5` : These folders contain the outputs for the memory trace files.

## MAKEFILE
The makefile contains the following commands:
1. `all` : This command compiles the code.
2. `clean` : This command removes the executable file.
To run the code, use the following commands: with the parameters 
- block size
- cache size L1
- associativity L1
- cache size L2
- associativity L2
```bash
make all
./cache_simulate 64 1024 2 65536 8 memory_trace_files/trace1.txt
```