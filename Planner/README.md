Lab 4 : Planning
================

Introduction
------------
This project contains Planners for Blocks World

* Forward (progression) planner using breadth first search
* Forward (progression) planner using A* search
* Goal Stack planner

* Please refer to Report.pdf for detailed analysis.

To Run Planner
--------------

1. Change directory to 'code'.

2. Make the code using
make

3. Run the program using
./planner <input_file_name> <output_file_name> [Optional parameters]

Optional parameters
-h, --help: Help
-i, --inadmissible :  Use inadmissible heuristic
-p, --print : Prints initial and goal state
-f, --bfs : Force to use bfs
-a, --astar : Force to use astar
-g, --goalstack : Force to use goalstack

e.g. to run sample test case 4.txt
./planner input/4.txt output/4.txt

To run with admissible heuristic on case 6.txt
./planner input/6.txt output/6.txt

To run with inadmissible heuristic on case 6.txt
./planner input/6.txt output/6.txt --inadmissible

To run with inadmissible heuristic on case 6.txt and print initial and goal state
./planner input/6.txt output/6.txt --inadmissible --print

To automate the task over all sample test cases  in  a shell script has been added. To use the shell script use
./script.sh

The generated output is in 'output' folder

To Generate New Test Case
-------------------------

1. Change directory to 'code'.

2. Make the code using
make

3. Run the program using
./generate output_file no_of_blocks planner(f|a|g)

Optional parameters
-h, --help: Help

e.g. to generate a 10 blocks problem for goal stack planning in 'input/new.txt' use
./generate input/new.txt 10 g


Directory Structure
-------------------

---README
---code
	|
	|---main.cpp
	|---generate.cpp
	|---script.sh
	|---input
	
		|
		|---1.txt
		|---2.txt
		|---3.txt
		|---4.txt
		|---5.txt
		|---6.txt
		
	|---output
	
		|
		|---1.txt
		|---2.txt
		|---3.txt
		|---4.txt
		|---5.txt
		|---6.txt

---Report.pdf


Note
----
To make script.sh executable use

chmod 755 script.sh


If it takes too long use g++ optimsizing tools like -O3

i.e.
g++ -O3 main.cpp -o planner

By default it uses admissible heurisitic

Developed By
============
Naman Goyal (2015CSB1021)
