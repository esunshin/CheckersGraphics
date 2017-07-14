# CheckersGraphics

Checkers game with OpenGL graphics

User is Red, computer is Black.

Computer will play first.

Uses a relatively shallow but wide tree to calculate 'best' computer move, and possible human moves.


Usage:

$ make

$ ./checkersGraphics

'p' to start the game

'q' to quit

point/click to move your pieces




"TREE_DEPTH" in mainGraphics.cpp changes the tree depth. Anything above 5 makes the game unreasonably slow. 3-4 is a good balance.




*Base code (GameBoard and OpenGL in main) borrowed from Bowdoin Professors Toma and Majercik. Thank you!*


Known Issues:

Even TREE_DEPTH values may lead to computer playing to human's advantage.

    -> More investigation needed
    
Nothing beyond double jumps are found.

Not too hard to beat computer. Possible causes:

    Tree not deep enough.
    
    'Scoring' (valuation) of boards could be improved.
    
    No weight given to what the human is most likely to do.

To do:

Recursive jumping function. (To solve first issue)

Better/more complex board valuation function.





