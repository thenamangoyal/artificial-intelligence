Lab 2 - Local & Adversarial Search
==================================


Introduction
------------
This project contains two problems

1. Experimenting with different local search algorithms. Take a complex problem, learn to formulate it as an AI single agent search problem and solve it using local search algorithms.

2. This part has been adopted from Berkeley Pacman projects. Design agents for the classic version of Pacman, including ghosts. Implement both minimax and expectimax search and with evaluation function design.

* Please refer to [Report.pdf](Report.pdf) for detailed analysis.
* Please refer to [l2.pdf](l2.pdf) for about the project.

Directory Structure
-------------------

---code
	[coalblockauction/main.cpp](code/coalblockauction/main.cpp)
	[multiagent/multiAgents.py](code/multiagent/multiAgents.py)

---[l2.pdf](l2.pdf)

---README.md

---[Report.pdf](Report.pdf)

Problem 1 (Coal Block Auction)
==============================



To Run
------

1. Locate to '[coalblockauction](code/coalblockauction)' directory.

2. Make the code using
make

3. Run the test case
./auction < Test_case.txt

e.g. to run test file 1.txt use
./auction < 1.txt

To automate the task over 1.txt to 20.txt a shell script has been added. To use the sheel script use
./script.sh

Note
----

The result outputs bids selected where the first value is total value of selection.
All values for company id, blocks id are assumed to be 1-indexed.
Bids are 1 indexed


Problem 2 (PacMan Multi Agent)
==============================

1. Locate to '[multiagent](code/multiagent)' directory.

2. Run using
python autograder.py


Developed By
============
Naman Goyal (2015CSB1021)
Chaudhari Milan Jayeshbhai (2015CSB1010)
