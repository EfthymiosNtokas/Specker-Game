# Specker-Game
The description of the game follows:
p players (indexed from 0 to p-1) take part in a game with n heaps (indexed from 0 to h-1).
Player 0 goes first, then player 1 etc.

In each move: The current player selects two heaps indexed i and j, removes k>0 coins
from i and puts m<k coins in j. A player is winning if after his move the heaps are all empty and 
the game ends.

The class Move, is used for the representation of the moves of the game.
The class State, is used for the representation of the states of the game.
The class Player, is used for the rperesentation of the payers of the game.

There are 4 types of players:
Greedy: He always picks the largest heap, removes all of its coins and does not
put any coins back.

Spartan:He always picks the smallest non empty heap, removes one of its coins and does
not put any coins back

Sneaky: He always picks the smallest non empty heap, removes all of its coins and does
not put any coins back

Righteous: He always selects the largest heap, with lets say c coins. He removes
ceiling(c/2) of those and puts ceiling(c/2)-1 to the current smallest heap.

The class Game,is used for the initalisation and respresentation of the game.
