Lab1 - Search : Pacman
======================

Introduction
------------

Experimenting with different AI search techniques in a Pacman environment. This is part of the Pacman projects developed at [UC Berkeley](http://ai.berkeley.edu/project_overview.html). The Pacman agent needs to find paths through the maze world, both to reach a location and to collect food efficiently.


* Please refer to [Report.pdf](Report.pdf) for detailed analysis.
* Please refer to [lab.pdf](lab.pdf) for about the project.

Directory Structure
-------------------

---code
	[search.py](code/search.py)
	[searchAgent.py](code/searchAgent.py)

---[lab.pdf](lab.pdf)

---README.md

---[Report.pdf](Report.pdf)


Executing
---------

Then run the autograder using

$python autograder.py

It took around 20 seconds to grade and gave a score a 26/25 (bonus for solving Q7 by just expanding 255 nodes). The code is commented but feel free to email at 2015csb1021@iitrpr.ac.in for any clarification.

Question specific commands

Question 1
----------
$python pacman.py -l tinyMaze -p SearchAgent
$python pacman.py -l mediumMaze -p SearchAgent
$python pacman.py -l bigMaze -z .5 -p SearchAgent

Question 2
----------
$python pacman.py -l mediumMaze -p SearchAgent -a fn=bfs
$python pacman.py -l bigMaze -p SearchAgent -a fn=bfs -z .5
$python eightpuzzle.py

Question 3
----------
$python pacman.py -l mediumMaze -p SearchAgent -a fn=ucs
$python pacman.py -l mediumDottedMaze -p StayEastSearchAgent
$python pacman.py -l mediumScaryMaze -p StayWestSearchAgent

Question 4
----------
$python pacman.py -l bigMaze -z .5 -p SearchAgent -a fn=astar,heuristic=manhattanHeuristic

Question 5
----------
$python pacman.py -l tinyCorners -p SearchAgent -a fn=bfs,prob=CornersProblem
$python pacman.py -l mediumCorners -p SearchAgent -a fn=bfs,prob=CornersProblem

Question 6
----------
$python pacman.py -l mediumCorners -p AStarCornersAgent -z 0.5

Question 7
----------
$python pacman.py -l testSearch -p AStarFoodSearchAgent
$python pacman.py -l trickySearch -p AStarFoodSearchAgent

Question 8
----------
$python pacman.py -l bigSearch -p ClosestDotSearchAgent -z .5

Developed by
Naman Goyal
2015CSB1021
