Sudoku Solver
=============

Introduction
------------
This project contains two problems

1. Implement a Sudoku solver that treats the puzzle as a constraint satisfaction problem. Sudoku is a logic puzzle involving placement of digits in squares satisfying certain constraints. Induce intelligence into the backtracking search, by integrating the minimum remaining value heuristic. Let us call this search as BS-I. Implement and integrate the least constraining value heuristic into the BS-I search. Let us call this BS-II. Finally, implement and integrate the MAC algorithm into BS-II. Call this algorithm BS-MAC.

2. Formulate Sudoku as a Boolean satisfiability problem and solve using MiniSAT solver.

* Please refer to [Report.pdf](Report.pdf) for detailed analysis.
* Please refer to [lab.pdf](lab.pdf) for about the project.


Directory Structure
-------------------

---code

	[p1/bs.cpp](code/p1/bs.cpp)
	[p1/bsI.cpp](code/p1/bsI.cpp)
	[p1/bsII.cpp](code/p1/bsII.cpp)
	[p1/bsMAC.cpp](code/p1/bsMAC.cpp)
	[p2/main.cpp](code/p2/main.cpp)

---[lab.pdf](lab.pdf)

---README.md

---[Report.pdf](Report.pdf)

Problem 1 (Constraint Satisfaction Problem)
===========================================

To Run
------

1. Change directory to '[p1](code/p1)'.

2. Make the code using
make

3. Run the program using

---For generic backtracking search (BS)
bin/bs <input_file_name> <output_file_name>

---For BS + minimum remaining value heuristic (MRV)
bin/bsI <input_file_name> <output_file_name>

---For BS + MRV + least constraining value heuristic (LCV)
bin/bsII <input_file_name> <output_file_name>

---For BS + MRV + LCV + MAC algorithm
bin/bsMAC <input_file_name> <output_file_name>

To automate the task over p.txt a shell script has been added. To use the shell script use
./script.sh

All generated output is in 'output' folder and statistics in 'stat' folder

Note
----
To make script.sh executable use

chmod 755 script.sh


If runnign takes too long using g++ optimsizing tools like -O3

i.e.
g++ -O3 bs.cpp -o bin/bs

g++ -O3 bsI.cpp -o bin/bsI

g++ -O3 bsII.cpp -o bin/bsII

g++ -O3 bsMAC.cpp -o bin/bsMAC

then run using same format




Problem 2 (Boolean Satisfiability Problem)
===========================================

1. Change directory to '[p2](code/p2)'.

2. Make the code using
make

3. Run the program using
./sat <input_file_name> <output_file_name>

To automate the task over p.txt a shell script has been added. To use the shell script use
./script.sh

All generated output is in 'output' folder 

Note
----
Make sure to make minstat and script.sh executable use

chmod 755 minstat script.sh

Make sure to first locate to 'p2' directory while executing the script


Developed By
============
Naman Goyal (2015CSB1021)
