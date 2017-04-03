# multiAgents.py
# --------------
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


from util import manhattanDistance
from game import Directions
import random, util

from game import Agent

class ReflexAgent(Agent):
    """
      A reflex agent chooses an action at each choice point by examining
      its alternatives via a state evaluation function.

      The code below is provided as a guide.  You are welcome to change
      it in any way you see fit, so long as you don't touch our method
      headers.
    """


    def getAction(self, gameState):
        """
        You do not need to change this method, but you're welcome to.

        getAction chooses among the best options according to the evaluation function.

        Just like in the previous project, getAction takes a GameState and returns
        some Directions.X for some X in the set {North, South, West, East, Stop}
        """
        # Collect legal moves and successor states
        legalMoves = gameState.getLegalActions()

        # Choose one of the best actions
        scores = [self.evaluationFunction(gameState, action) for action in legalMoves]
        bestScore = max(scores)
        bestIndices = [index for index in range(len(scores)) if scores[index] == bestScore]
        chosenIndex = random.choice(bestIndices) # Pick randomly among the best

        "Add more of your code here if you want to"
        return legalMoves[chosenIndex]

    def evaluationFunction(self, currentGameState, action):
        """
        Design a better evaluation function here.

        The evaluation function takes in the current and proposed successor
        GameStates (pacman.py) and returns a number, where higher numbers are better.

        The code below extracts some useful information from the state, like the
        remaining food (newFood) and Pacman position after moving (newPos).
        newScaredTimes holds the number of moves that each ghost will remain
        scared because of Pacman having eaten a power pellet.

        Print out these variables to see what you're getting, then combine them
        to create a masterful evaluation function.
        """
        # Useful information you can extract from a GameState (pacman.py)
        successorGameState = currentGameState.generatePacmanSuccessor(action)
        newPos = successorGameState.getPacmanPosition()
        newFood = successorGameState.getFood()
        newGhostStates = successorGameState.getGhostStates()
        newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

        "*** YOUR CODE HERE ***"
        ans = 0 # Final answer

        # Feature 1: Is successor state a food state
        (nx, ny) = newPos
        isFood=int(currentGameState.getFood()[nx][ny])
        ans += isFood

        # Feature 2: Distance form all foods dots
        foodList = newFood.asList()
        val = len(foodList)
        remainingFoodDistance = 0
        currentFood = newPos

        while len(foodList):
          closestFood = min(foodList, key=lambda x: util.manhattanDistance(x, currentFood))
          remainingFoodDistance += util.manhattanDistance(closestFood, currentFood)
          currentFood = closestFood
          foodList.remove(currentFood)
        
        if (remainingFoodDistance!=0):
          ans += 1.0/remainingFoodDistance

        # Feature 3: Distance form ghosts
        totalGhostDist=0
        if  min([util.manhattanDistance(ghostState.getPosition(), newPos) for ghostState in newGhostStates])<2:
          return -9999999

        # Feature 4: If successor state is stop state
        stopFeature = 0
        if len(currentGameState.getLegalActions()) >1:
          if action == Directions.STOP:
            stopFeature=1

        ans -= stopFeature        

        # Feature 5: Score of game and scared time of ghost
        ans += sum(newScaredTimes) + successorGameState.getScore()
        return ans

def scoreEvaluationFunction(currentGameState):
    """
      This default evaluation function just returns the score of the state.
      The score is the same one displayed in the Pacman GUI.

      This evaluation function is meant for use with adversarial search agents
      (not reflex agents).
    """
    return currentGameState.getScore()

class MultiAgentSearchAgent(Agent):
    """
      This class provides some common elements to all of your
      multi-agent searchers.  Any methods defined here will be available
      to the MinimaxPacmanAgent, AlphaBetaPacmanAgent & ExpectimaxPacmanAgent.

      You *do not* need to make any changes here, but you can if you want to
      add functionality to all your adversarial search agents.  Please do not
      remove anything, however.

      Note: this is an abstract class: one that should not be instantiated.  It's
      only partially specified, and designed to be extended.  Agent (game.py)
      is another abstract class.
    """

    def __init__(self, evalFn = 'scoreEvaluationFunction', depth = '2'):
        self.index = 0 # Pacman is always agent index 0
        self.evaluationFunction = util.lookup(evalFn, globals())
        self.depth = int(depth)

class MinimaxAgent(MultiAgentSearchAgent):
    """
      Your minimax agent (question 2)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action from the current gameState using self.depth
          and self.evaluationFunction.

          Here are some method calls that might be useful when implementing minimax.

          gameState.getLegalActions(agentIndex):
            Returns a list of legal actions for an agent
            agentIndex=0 means Pacman, ghosts are >= 1

          gameState.generateSuccessor(agentIndex, action):
            Returns the successor game state after an agent takes an action

          gameState.getNumAgents():
            Returns the total number of agents in the game
        """
        "*** YOUR CODE HERE ***"
        depth=0        
        agentIndex=1
        return max(gameState.getLegalActions(0), key= lambda x: self.minValue(gameState.generateSuccessor(0, x), agentIndex, depth))
        util.raiseNotDefined()

    def minValue(self, gameState, agentIndex, depth):
      if (len(gameState.getLegalActions(agentIndex))==0):
        return self.evaluationFunction(gameState)
      if agentIndex< gameState.getNumAgents()-1:
        return min([self.minValue(gameState.generateSuccessor(agentIndex, x), agentIndex+1, depth) for x in gameState.getLegalActions(agentIndex)])
        
      else:
        return min([self.maxValue(gameState.generateSuccessor(agentIndex, x), depth+1) for x in gameState.getLegalActions(agentIndex)])

    def maxValue(self, gameState, depth):
      if ((depth >= self.depth) or len(gameState.getLegalActions(0))==0):
        return self.evaluationFunction(gameState)
      return max([self.minValue(gameState.generateSuccessor(0, x),1, depth) for x in gameState.getLegalActions(0)])


class AlphaBetaAgent(MultiAgentSearchAgent):
    """
      Your minimax agent with alpha-beta pruning (question 3)
    """

    def getAction(self, gameState):
        """
          Returns the minimax action using self.depth and self.evaluationFunction
        """
        depth=0
        agentIndex=1

        a=float('-inf')
        b=float('inf')

        value = float('-inf')
        finalaction = None
        for action in gameState.getLegalActions(0):
          value = self.minValue(gameState.generateSuccessor(0, action),agentIndex, depth, a, b)
          if a < value:
            a = value
            finalaction = action
        return finalaction
        util.raiseNotDefined()

    def minValue(self, gameState, agentIndex, depth, a, b):
      if (len(gameState.getLegalActions(agentIndex))==0):
        return self.evaluationFunction(gameState)
      
      value = float('inf')
      for action in gameState.getLegalActions(agentIndex):
        if agentIndex< gameState.getNumAgents()-1:
          value = min(value, self.minValue(gameState.generateSuccessor(agentIndex, action), agentIndex+1, depth, a, b))
        else:
          value = min(value, self.maxValue(gameState.generateSuccessor(agentIndex, action), depth+1, a, b))

        if value < a:
          return value
        b = min(b, value)

      return value
     
    def maxValue(self, gameState, depth, a, b):
      if ((depth >= self.depth) or len(gameState.getLegalActions(0))==0):
        return self.evaluationFunction(gameState)

      value = float('-inf')
      for action in gameState.getLegalActions(0):
        value = max(value, self.minValue(gameState.generateSuccessor(0, action),1, depth, a, b))

        if value > b:
          return value
        a = max(a, value)

      return value

class ExpectimaxAgent(MultiAgentSearchAgent):
    """
      Your expectimax agent (question 4)
    """

    def getAction(self, gameState):
        """
          Returns the expectimax action using self.depth and self.evaluationFunction

          All ghosts should be modeled as choosing uniformly at random from their
          legal moves.
        """
        depth=0        
        agentIndex=1
        return max(gameState.getLegalActions(0), key= lambda x: self.minValue(gameState.generateSuccessor(0, x), agentIndex, depth))
        util.raiseNotDefined()

    def minValue(self, gameState, agentIndex, depth):
      numActions = len(gameState.getLegalActions(agentIndex))
      if (numActions==0):
        return self.evaluationFunction(gameState)
      if agentIndex< gameState.getNumAgents()-1:
        return sum([self.minValue(gameState.generateSuccessor(agentIndex, x), agentIndex+1, depth) for x in gameState.getLegalActions(agentIndex)])/float(numActions)
        
      else:
        return sum([self.maxValue(gameState.generateSuccessor(agentIndex, x), depth+1) for x in gameState.getLegalActions(agentIndex)])/float(numActions)

    def maxValue(self, gameState, depth):
      if ((depth >= self.depth) or len(gameState.getLegalActions(0))==0):
        return self.evaluationFunction(gameState)

      return max([self.minValue(gameState.generateSuccessor(0, x),1, depth) for x in gameState.getLegalActions(0)])

def betterEvaluationFunction(currentGameState):
    """
      Your extreme ghost-hunting, pellet-nabbing, food-gobbling, unstoppable
      evaluation function (question 5).

      DESCRIPTION: <write something here so we know what you did>
    """
    
    Distance=util.manhattanDistance
    newGhostStates = currentGameState.getGhostStates()
    newPos = currentGameState.getPacmanPosition()
    numAgents = currentGameState.getNumAgents()

    ans = 0 # Final answer

    # Feature 1: Distance from Ghost
    if numAgents>1:
      minGhostDist= min([Distance(ghostState.getPosition(), newPos) for ghostState in newGhostStates])
      if minGhostDist<2:
        return -10000
      else:
        ans -= 1.0/minGhostDist

    # Get values for features in current in state
    capsuleList = currentGameState.getCapsules()
    newcapsuleList = capsuleList[:]
    newFood = currentGameState.getFood()    
    newScaredTimes = [ghostState.scaredTimer for ghostState in newGhostStates]

    # Feature 2: Total remaining food distance
    foodList = newFood.asList()
    val = len(foodList)
    remainingFoodDistance = 0
    currentFood = newPos

    while len(foodList):
      closestFood = min(foodList, key=lambda x: Distance(x, currentFood))
      remainingFoodDistance += 1.0/Distance(closestFood, currentFood) # Inverting value
      currentFood = closestFood
      foodList.remove(currentFood)

    ans += remainingFoodDistance

    # Feature 3: Total remaining capsule distance
    currentCapsule = newPos
    remainingCapsuleDistance=0
    countCapsule = value = len(capsuleList)
    while value:
      closestCapsule = min(capsuleList, key=lambda x: Distance(x, currentCapsule))
      remainingCapsuleDistance += 1.0/Distance(closestCapsule, currentCapsule) # Inverting value
      currentCapsule = closestCapsule
      capsuleList.remove(currentCapsule)
      value-=1

    ans += remainingCapsuleDistance

    # Feature 4: Count of remaining food and capsules
    ans -= 2*(currentGameState.getNumFood()+countCapsule)

    # Feture 5: Score of Game and sum of Scared Times of Ghosts
    ans += 5*currentGameState.getScore()+sum(newScaredTimes)
    
    return ans
    util.raiseNotDefined()

# Abbreviation
better = betterEvaluationFunction

