/* 
 * File:   GameBoard.h
 * Author: smajerci
 *
 * Created on November 18, 2014, 7:24 PM
 *
 *
 */

#ifndef GameTreeNode_h
#define	GameTreeNode_h

#include "GameBoard.h"

using namespace std;

class GameTreeNode {
    
public:
    GameTreeNode();
    GameTreeNode(GameBoard theBoard);
    GameTreeNode(GameBoard theBoard, int pMoved);
    
    vector< GameTreeNode > getNextMoves() { return nextMoves; }
    vector< GameTreeNode >& getNextMovesRef() { return nextMoves; }
    GameTreeNode* getNextMoveRef(int index);
    GameTreeNode getNextMove(int index);
    
    GameBoard getCurrBoard() { return currBoard; }
    
    vector< GameTreeNode > setNextMoves(vector< GameTreeNode > theMoves);
    vector< GameTreeNode > setNextMoves(vector< GameBoard > theMoves);
    vector< GameTreeNode > setNextMoves(vector< GameBoard > theMoves, int player);
//    void addNextMove(GameTreeNode * theMove);
    vector< GameTreeNode > addNextMove(GameBoard theMove);
    
    int getNumMoves() { return numMoves; }
    
    vector< GameTreeNode > setNextMoves(int thePlayer);
    
    vector< GameTreeNode > generateTreeLevel(int depth, int startPlayer, bool isRoot);
    
    vector< GameTreeNode > findNextMoves(int thePlayer);
    
    int getScore() { return currBoard.getScore(); }
    
    void setScore(int index) { currBoard.setScore(index); }
    
    int getScoreSum();
    
    void print() { currBoard.print();
        cout << "Moved: " << playerMoved << endl;
    }
    
    GameTreeNode nextMove(int depth, int player);
    GameTreeNode nextMove(int player); //assumes have already ran generateTreeLevel(depth, player)
    
    bool equals(GameTreeNode otherGTN);
    
    GameTreeNode moveMade(GameBoard newBoard);
    
    void addLayer(int player);
    
    int getPlayerMoved() { return playerMoved; }
    
    GameTreeNode makeMove(int player);
    
    GameTreeNode makeMove(GameBoard nextMove, int player);
    
private:
    GameBoard currBoard;
    vector< GameTreeNode > nextMoves;
    int numMoves;
    int playerMoved;
    
};

#endif	/* GameTreeNode_h */

