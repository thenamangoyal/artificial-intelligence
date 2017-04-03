# search.py
# ---------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearchIterative(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:

    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
    """
    frontier = util.Stack() # Frontier is LIFO queue
    frontier.push(problem.getStartState()) # Add intial state to frontier
    explored = [] # Initialize empty explored states
    parent = {} # Dictionary mapping successor to it's parent state and action

    while True:
        if frontier.isEmpty():
            return None # No Solution not found

        state = frontier.pop() # Get a element from frontier

        if state not in explored: # Only expand in not in explored
            if problem.isGoalState(state):
                ans = [] # Initialise Final answer of list of actions

                """Build solution"""
                while state != problem.getStartState():
                    x, y = parent[state] # x = parent state, y = action leading to state
                    ans.insert(0,x)
                    state = y
                return ans # Return Final answer of list of actions

            explored.append(state) # Goal test failed, add the node to explored states

        """Generate all successors"""
        for successor, action, stepCost in  problem.getSuccessors(state):
            if successor not in explored: # Add to frontier if not in explored
                frontier.push(successor)
                parent[successor] = (action, state)
                
def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.
    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.
    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:
    print "Start:", problem.getStartState()
    print "Is the start a goal?", problem.isGoalState(problem.getStartState())
    print "Start's successors:", problem.getSuccessors(problem.getStartState())
    """
    explored = [] # Initialise the explored states
    return dfsrecur(problem.getStartState(), problem, explored)

def dfsrecur(state, problem, explored):

    if problem.isGoalState(state): # Goal Test
        return [] # Returns empty list because the actions gets inserted when function call stacks returns
        
    explored.append(state) # Goal test failed, add the node to explored states

    """Generate all successors"""
    for successor, action, stepCost in  problem.getSuccessors(state):
            if successor not in explored:
                result = dfsrecur(successor,problem,explored)
                if result != None:
                    return [action] + result # Return the updated action list

def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""

    frontier = util.Queue() # Frontier is FIFO queue
    frontier.push(problem.getStartState()) # Add intial state to frontier
    explored = [] # Initialize empty explored states
    parent = {} # Dictionary mapping successor to it's parent state and action

    while True:

        if frontier.isEmpty():
            return None # No Solution not found

        state = frontier.pop() # Get a element from frontier

        if problem.isGoalState(state): # If goal tests passes return list of actions

            ans = [] # Initialise Final answer of list of actions

            """Build solution"""
            while state != problem.getStartState():
                x, y = parent[state] # x = parent state, y = action leading to state
                ans.insert(0,x)
                state = y
            return ans # Return Final answer of list of actions

        explored.append(state) # Goal test failed, add the node to explored states

        """Generate all successors"""
        for successor, action, stepCost in  problem.getSuccessors(state):
            if successor not in explored + frontier.list: # Add to frontier if not in explored or frontier
                frontier.push(successor)
                parent[successor] = (action, state)

def uniformCostSearch(problem):
    """Search the node of least total cost first."""
    
    frontier = util.PriorityQueue() # Frontier is priority queue
    cost = {problem.getStartState(): 0} # Dictionary mapping states to cost
    explored = [] # Initialize empty explored states
    parent = {} # Dictionary mapping successor to it's parent state and action

    frontier.push(problem.getStartState(), cost[problem.getStartState()]) # Add intial state to frontier
    
    while True:

        if frontier.isEmpty(): # No solution found
            return None

        state = frontier.pop() # Get a node from frontier

        if problem.isGoalState(state): # If Goal test passes
            ans = [] # Initialise Final answer of list of actions

            """Build solution"""
            while state != problem.getStartState():
                x, y = parent[state] # x = parent state, y = action leading to state
                ans.insert(0,x)
                state = y
            return ans # Return Final answer of list of actions

        explored.append(state) # Goal test failed, add the node to explored states

        """Generate all successors"""
        for successor, action, stepCost in  problem.getSuccessors(state):

            if successor not in explored + [x[2] for x in frontier.heap]: # Add to frontier if not in explored or frontier
                cost[successor] = cost[state] + stepCost
                parent[successor] = (action, state)
                frontier.push(successor, cost[successor])


            elif successor in [x[2] for x in frontier.heap] and cost[successor] > cost[state] + stepCost: # Updates nodes in frontier with lower path cost
                cost[successor] = cost[state] + stepCost # Update actual path cost
                parent[successor] = (action, state)
                frontier.update(successor, cost[successor])

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""

    frontier = util.PriorityQueue() # Frontier is priority queue
    gcost = {problem.getStartState(): 0} # Dictionary mapping states to gcost
    explored = [] # Initialize empty explored states
    parent = {} # Dictionary mapping successor to it's parent state and action

    frontier.push(problem.getStartState(), gcost[problem.getStartState()] + heuristic(problem.getStartState(), problem)) # Add intial state to frontier
    
    while True:

        if frontier.isEmpty(): # No Solution found
            return None

        state = frontier.pop() # Get a node from Frontier

        if problem.isGoalState(state): # If goal test passes
            ans = [] # Initialise Final answer of list of actions

            """Build solution"""
            while state != problem.getStartState():
                x, y = parent[state] # x = parent state, y = action leading to state
                ans.insert(0,x)
                state = y
            return ans # Return Final answer of list of actions

        explored.append(state) # Goal test failed, add the node to explored states

        """Generate all successors"""
        for successor, action, stepCost in  problem.getSuccessors(state):

            if successor not in explored + [x[2] for x in frontier.heap]: # Add to frontier if not in explored or frontier
                gcost[successor] = gcost[state] + stepCost
                parent[successor] = (action, state)
                frontier.push(successor, gcost[successor] + heuristic(successor, problem))


            elif successor in [x[2] for x in frontier.heap] and gcost[successor] > gcost[state] + stepCost: # Updates nodes in frontier with lower path cost
                gcost[successor] = gcost[state] + stepCost # Update g-cost the actual path ocst
                parent[successor] = (action, state)
                frontier.update(successor, gcost[successor] + heuristic(successor, problem))


# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
