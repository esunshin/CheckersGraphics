//
//  main.cpp
//  
//
//  Created by Ezra Sunshine on 6/5/17.
//
//

#include <stdio.h>
#include "GameTreeNode.h"



void printPossMoves(vector< GameBoard > possibleMoves);
int getZeroDepth(GameTreeNode theRoot);
vector< GameTreeNode > isLegalStart(GameTreeNode theTree, int x, int y);
int isLegalEnd(vector< GameTreeNode > theStarts, int x, int y);
void printWithIndexes(GameBoard theBoard);

int main() {
    
    
//    GameBoard testBoard;
//    
//    printWithIndexes(testBoard);
    
    
    GameTreeNode theTree;
    theTree.print();
    
//    theTree = theTree.generateTreeLevel(5, BLK, true);
    theTree.generateTreeLevel(5, BLK, true);
    
    for(int i = 0; i < 50; i++) { //arbitrary
    
//        cout << "Pre-Depth: " << getZeroDepth(theTree) << endl;
//        cout << "Post-Depth: " << getZeroDepth(theTree) << endl;

        theTree = theTree.makeMove(BLK); //make computer move
//        theTree.print();
        
        printWithIndexes(theTree.getCurrBoard());
        
        
        int x, y = -1;
        vector< GameTreeNode > startMoves = isLegalStart(theTree, x, y);
        
        while(x <= 0 or y <= 0 or startMoves.size() == 0) {
            cout << "Start X: ";
            cin >> x;
            cout << "Start Y: ";
            cin >> y;
            
            startMoves = isLegalStart(theTree, x-1, y-1);
        }
        cout << endl;
        
        x = -1;
        y = -1;
        int endLegal = -1;
        while(x <= 0 or y <= 0 or endLegal == -1) {
            cout << "End X: ";
            cin >> x;
            cout << "End Y: ";
            cin >> y;
            
            endLegal = isLegalEnd(startMoves, x-1, y-1);
        }
        cout << endl;
        
        GameBoard userMove = startMoves.at(endLegal).getCurrBoard();
        
        theTree = theTree.makeMove(userMove, BLK);
        theTree.print();
    }
        
        /*  USE IF YOU WANT TO SHOW ALL POSSIBLE MOVES
         vector< GameBoard > possMoves;
        for(int i = 0; i < theTree.getNextMoves().size(); i++) {
            possMoves.push_back(theTree.getNextMove(i).getCurrBoard());
        }
        printPossMoves(possMoves);
    
        int nextMove = -1;
        while(nextMove <= 0 or nextMove > possMoves.size()) {
            cout << "What move do you want to make?  ";
            cin >> nextMove;
        }
        
        
        theTree = theTree.makeMove(possMoves.at(nextMove-1), BLK);
        
        theTree.print();
         
         */
}



void printPossMoves(vector< GameBoard > possibleMoves) {
    
    for (int row = 0; row < DIMENSION; row++) {
        for(int i = 0; i < possibleMoves.size(); i++) {
            cout << "|";
            for (int col = 0; col < DIMENSION; col++) {
                if(possibleMoves.at(i).getAt(row, col) == RED) {
                    //                system("Color 1A");
                    cout << "-|";
                }
                else if(possibleMoves.at(i).getAt(row, col) == BLK)
                    cout << "+|";
                else if(possibleMoves.at(i).getAt(row, col) == KRED)
                    cout << "=|";
                else if(possibleMoves.at(i).getAt(row, col) == KBLK)
                    cout << "*|";
                else
                    cout << " |";
            }
            if(row == 0)
                cout << possibleMoves.at(i).getScore() << "   ";
            else if(row == DIMENSION-1)
                cout << i+1 << "   ";
            else
                cout << "    ";
        }
        cout << endl;
    }
}


int getZeroDepth(GameTreeNode theRoot) {
    
    if(theRoot.getNextMoves().size() == 0)
        return 0;
    
    return 1 + getZeroDepth(theRoot.getNextMove(0));
    
}


vector< GameTreeNode > isLegalStart(GameTreeNode theTree, int x, int y) {
    
    vector< GameTreeNode > foundStartMoves;
    
    int startX;
    int startY;
    
    for(int i = 0; i < theTree.getNextMoves().size(); i++) {
        startX = theTree.getNextMove(i).getCurrBoard().getStartX();
        startY = theTree.getNextMove(i).getCurrBoard().getStartY();
        
        if(startX == x and startY == y) {
            foundStartMoves.push_back(theTree.getNextMove(i));
        }
    }
    
    return foundStartMoves;
    
}

int isLegalEnd(vector< GameTreeNode > theStarts, int x, int y) {
    
    for(int i = 0; i < theStarts.size(); i++) {
        if(theStarts.at(i).getCurrBoard().getEndX() == x) {
            if(theStarts.at(i).getCurrBoard().getEndY() == y) {
                return i;
            }
        }
    }

    return -1;
}

void printWithIndexes(GameBoard theBoard) {
    cout << "'";
    for(int i = 0; i < DIMENSION; i++) {
        cout << i+1 << "'"; //top row numbers
    }
    cout << endl;
    
    for (int row = 0; row < DIMENSION; row++) {
        cout << "|";
        for (int col = 0; col < DIMENSION; col++) {
            if(theBoard.getAt(row, col) == RED) {
                cout << "-|";
            }
            else if(theBoard.getAt(row, col) == BLK)
                cout << "+|";
            else if(theBoard.getAt(row, col) == KRED)
                cout << "=|";
            else if(theBoard.getAt(row, col) == KBLK)
                cout << "*|";
            else
                cout << " |";
        }
        cout << row+1; //down-the-side numbers
        if(row == DIMENSION-1) {
            cout << " " << theBoard.getScore() << endl;
        }
        else {
            cout << endl;
        }
    }
    
}




