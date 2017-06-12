 /*
 * File:   GameBoard.h
 * Author: smajerci
 *
 * Created on November 18, 2014, 7:24 PM
 *
 *
 */

#ifndef GameBoard_h
#define	GameBoard_h

#include <stdlib.h>
#include <iostream>
#include <vector>

#define DIMENSION 8
#define NUM_TILE_ROWS 3

#define BLK 1
#define RED -1
#define K_WEIGHT 5
#define KBLK 5
#define KRED -5
#define MPT 0

using namespace std;

class GameBoard {
    
public:
    GameBoard();
    GameBoard(int allVal);

    int getAt(int x, int y) {return board.at(x).at(y); }
    int getScore() {return score; }
    void setScore(int newScore) { score = newScore; }
    vector< vector<int> > getBoard() {return board; }
    void setBoard(vector< vector<int> > newBoard);
    
    bool equals(GameBoard theBoard);
    int getVal();

    void print();
    void printV();
    vector<GameBoard> findNextMoves(int player);
    vector<GameBoard> findNextMoves(int r, int c, int thePlayer);
    vector<GameBoard> findJump(int r, int c, int thePlayer);
    
    bool getIsJump() { return isJump; }
    
    int getStartX() { return startX; }
    int getStartY() { return startY; }
    
    int getEndX() { return endX; }
    int getEndY() { return endY; }
    
    void setAt(int r, int c, int theVal) { board.at(r).at(c) = theVal; }
    
private:
//    int DIMENSION = 4;
    vector< vector<int> > board;
    int score;
    bool isJump;
    
    int startX, startY;
    int endX, endY;
    
};

#endif	/* GAMEBOARD_H */

