Sudoku Solver
=============

To Run
------

1. Change directory to 'p1'.

2. Make the code using make

3. Run the program using

---For generic backtracking search (BS)
bin/bs <input_file_name> <output_file_name>

---For BS + minimum remaining value heuristic (MRV)
bin/bsI <input_file_name> <output_file_name>

---For BS + MRV + least constraining value heuristic (LCV)
bin/bsII <input_file_name> <output_file_name>

---For BS + MRV + LCV + MAC algorithm
bin/bsMAC <input_file_name> <output_file_name>

To automate the task over p.txt a shell script has been added. To use the shell script use ./script.sh

All generated output is in 'output' folder and statistics in 'stat' folder

Note
----

*To make script.sh executable use

 chmod 755 script.sh

*If running takes too long using g++ optimsizing tools like -O3

i.e. 
g++ -O3 bs.cpp -o bin/bs
g++ -O3 bsI.cpp -o bin/bsI
g++ -O3 bsII.cpp -o bin/bsII
g++ -O3 bsMAC.cpp -o bin/bsMAC

then run using same format
