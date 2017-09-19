Lab 5 : Probabilistic Inference
===============================

Introduction
------------
This project implements two techniques for drawing inference from a Bayes Network.
• Exact inference using variable elimination
• Approximate inference using rejection sampling

* Please refer to [Report.pdf](Report.pdf) for detailed analysis.
* Please refer to [lab.pdf](lab.pdf) for about the project.

Directory Structure
-------------------

---code

	|
	|---main.py
	|---script.sh
	|---b1.txt
	|---b2.txt
	|---b3.txt
	|---q1.txt
	|---q2.txt
	|---q3.txt


---[lab.pdf](lab.pdf)

---README.md

---[Report.pdf](Report.pdf)

To Run
------

1. Change directory to 'code'.

2. Run the program using
python main.py <bayes_net_input> <query_input> [No of samples for rejection sampling]

e.g. to run test case 3
python main.py b3.txt q3.txt

e.g. to run test case 3 with no of sample for rejection sampling as 10000
python main.py b3.txt q3.txt 10000

To automate the task over all test cases  in  a shell script has been added. To use the shell script use
./script.sh

Note
----
1. No of samples for rejection sampling is optional
2. By default no of samples for rejection sampling is 100000
3. Output is printed to console by default
4. To make script.sh executable use

chmod 755 script.sh


Developed By
============
Naman Goyal (2015CSB1021)
