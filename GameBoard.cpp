/* 
 * File:   GameBoard.cpp
 * Author: smajerci
 * 
 * Created on November 18, 2014, 7:24 PM
 */

#include <stdlib.h>
#include "GameBoard.h"

using namespace std;

//#define DIMENSION 10

// create a GameBoard by assigning random values [0..2]
// inclusive to the board array
GameBoard::GameBoard() {
    
//    int DIMENSION = 10;
    
	for (int row = 0; row < DIMENSION; ++row) {
        
        board.push_back( vector<int>() );
        
		for (int col = 0; col < DIMENSION; ++col) {

            if(row < NUM_TILE_ROWS and (row+col)%2 == 1) {
                board.at(row).push_back(BLK);
            }
            else if(row >= (DIMENSION - NUM_TILE_ROWS) and (row+col)%2 == 1) {
                board.at(row).push_back(RED);
            }
            else
                board.at(row).push_back(MPT);;
		}
	}

    score = 0;
    isJump = false;
    
    startX = -1;
    startY = -1;
    endX = -1;
    endY = -1;
}


// create a GameBoard by assigning the same value to every
// item in the board array; the value is supplied by the
// parameter allVal
GameBoard::GameBoard(int allVal) {

//    for (int r = 0; r < DIMENSION; r++) {
//        for (int c = 0; c < DIMENSION; c++) {
//            board[r][c] = allVal;
//        }
//    }

}



// returns true if the GameBoard passed in is equal to
// the board in this GameBoard object; otherwise, it
// returns false
bool GameBoard::equals(GameBoard theBoard) {

	for (int r = 0; r < DIMENSION; r++) {
		for (int c = 0; c < DIMENSION; c++) {
			if (this->getAt(r, c) != theBoard.getAt(r, c))
				return false;
		}
	}

	return true;
}


// computes the "value" of a board, which is defined to
// be the number of 1s in the array minus the number of
// 2s in the array
int GameBoard::getVal() {
    
    int theScore = 0;
    for (int r = 0; r < DIMENSION; r++) {
        for (int c = 0; c < DIMENSION; c++) {
            theScore += getAt(r, c);
        }
    }
    
    return theScore;
}


// prints a GameBoard as a 2-dimension grid of integers
void GameBoard::print() {
//    for (int col = 0; col < DIMENSION; col++) {
//        cout << "-----";
//    }
//    cout << endl;
	for (int row = 0; row < DIMENSION; row++) {
        cout << "|";
		for (int col = 0; col < DIMENSION; col++) {
            if(getAt(row, col) == RED) {
//                system("Color 1A");
                cout << "-|";
            }
            else if(getAt(row, col) == BLK)
                cout << "+|";
            else if(getAt(row, col) == KRED)
                cout << "=|";
            else if(getAt(row, col) == KBLK)
                cout << "*|";
            else
                cout << " |";
		}
        if(row == DIMENSION-1) {
            cout << " " << score << endl;
        }
        else {
            cout << endl;
        }
//        for (int col = 0; col < DIMENSION; col++) {
//            cout << "-----";
//        }
//		cout << endl;
	}
//    cout << score;
//	cout << endl;
}

void GameBoard::printV() {
    for (int col = 0; col < DIMENSION; col++) {
        cout << "-----";
    }
    cout << endl;
    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
            if(getAt(row, col) >= 0)
                cout << getAt(row, col) << "  | ";
            else
                cout << getAt(row, col) << " | ";
        }
        cout << endl;
        for (int col = 0; col < DIMENSION; col++) {
            cout << "-----";
        }
        cout << endl;
    }
    cout << endl;
}

vector<GameBoard> GameBoard::findNextMoves(int player) {
    
    bool foundJump = false;
    vector<GameBoard> allMoves;
    vector<GameBoard> fewMoves;
    
    int thePlayer = player/abs(player);
    
    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
//            if(getAt(row, col) == player or getAt(row, col) == K_WEIGHT*player) {
            if(getAt(row, col) == thePlayer or getAt(row, col) == K_WEIGHT*thePlayer) {
                fewMoves = findNextMoves(row, col, thePlayer);
                if(fewMoves.size() > 0) { // need this
                    if(foundJump == false) { //if you haven't found a jump yet..
                        if(fewMoves.at(0).isJump == true) {
                            foundJump = true;
                            allMoves.clear();
                            allMoves.insert(allMoves.end(), fewMoves.begin(), fewMoves.end());
                        }
                        else if(fewMoves.at(0).isJump == false) {
                            allMoves.insert(allMoves.end(), fewMoves.begin(), fewMoves.end());
                        }
                    }
                    else if(foundJump == true) {
                        if(fewMoves.at(0).isJump == true) {
                            allMoves.insert(allMoves.end(), fewMoves.begin(), fewMoves.end());
                        }
                        else if(fewMoves.at(0).isJump == false)
                            continue;
                    }
                } // size check
            }
        }
    }
    
    return allMoves;
    
}

vector<GameBoard> GameBoard::findNextMoves(int r, int c, int thePlayer) {
    

    vector<GameBoard> theMoves;
    
    int player = getAt(r, c);
    if(player != thePlayer and player != K_WEIGHT*thePlayer) {
        cout << "Expected " << thePlayer << " , player at r,c " << player << endl;
        return theMoves;
    }
    
    thePlayer = player;
    
    bool isKing = false;
    if(abs(thePlayer) == K_WEIGHT)
        isKing = true;
    
    bool foundJump = false;
    
    for(int cDelt = -1; cDelt <= 1; cDelt++) {
//        int rDelt = thePlayer;
        for(int rDelt = -1; rDelt <= 1; rDelt++) {
            
            int curr = 0;
            if(thePlayer != 0)
                curr = thePlayer/abs(thePlayer);
            
            if(!isKing and rDelt != curr)
                continue;
            
            if(rDelt == 0 or cDelt == 0)
                continue;
        
            GameBoard gB = *this;
            gB.startX = c;  // IS THIS RIGHT???
            gB.startY = r;  // IS THIS RIGHT???
        
            int newR = r + rDelt;
            int newC = c + cDelt;

            if(newR >= DIMENSION or newC >= DIMENSION)
                continue;
            if(newR < 0 or newC < 0)
                continue;
        
            int newLoc = 0;
            if(getAt(newR, newC) != 0)
                newLoc = getAt(newR, newC)/abs(getAt(newR, newC));
            
            if(newLoc == curr) //trying to move into teammate-occupied spot
                continue;
        
            if(newLoc == MPT and !foundJump) { //Move if you can, and you haven't found any jump moves yet
                gB = *this;
                gB.startX = c;  // IS THIS RIGHT???
                gB.startY = r;  // IS THIS RIGHT???
                gB.board.at(r).at(c) = MPT;
                
                if((curr == -1 and newR == 0) or (curr == 1 and newR == DIMENSION-1))
                    gB.board.at(newR).at(newC) = curr*K_WEIGHT;
                else
                    gB.board.at(newR).at(newC) = thePlayer;
                
                gB.endX = newC;  // IS THIS RIGHT???
                gB.endY = newR;  // IS THIS RIGHT???
                
                gB.score = gB.getVal();
                gB.isJump = false;
                theMoves.push_back(gB);
            }
        
            int newRFar = newR + rDelt;
            int newCFar = newC + cDelt;
        
            if(newRFar >= DIMENSION or newCFar >= DIMENSION)
                continue;
            if(newRFar < 0 or newCFar < 0)
                continue;
        
            
            int newLocFar = 0;
            if(getAt(newRFar, newCFar) != 0)
                newLocFar = getAt(newRFar, newCFar)/abs(getAt(newRFar, newCFar));
            
            if((newLoc == (-1)*curr) and newLocFar == MPT) { //can do simple jump...
                if(!foundJump) { //first jump at this location found
                    theMoves.clear();
                    foundJump = true;
                }
                
                gB = *this;
                gB.startX = c;  // IS THIS RIGHT???
                gB.startY = r;  // IS THIS RIGHT???
                gB.board.at(r).at(c) = MPT;
                gB.board.at(newR).at(newC) = MPT;
                
                if((curr == -1 and newRFar == 0) or (curr == 1 and newRFar == DIMENSION-1))
                    gB.board.at(newRFar).at(newCFar) = curr*K_WEIGHT;
                else
                    gB.board.at(newRFar).at(newCFar) = thePlayer;
//                gB.board.at(newRFar).at(newCFar) = thePlayer;
                gB.score = gB.getVal();
                gB.isJump = true;
                
                gB.endX = newCFar;  // IS THIS RIGHT???
                gB.endY = newRFar;  // IS THIS RIGHT???
            
                vector<GameBoard> doubleJumps = gB.findJump(newRFar, newCFar, thePlayer);
                if(doubleJumps.size() == 0)
                    theMoves.push_back(gB); // no double jump found
                else
                    theMoves.insert(theMoves.end(), doubleJumps.begin(), doubleJumps.end()); // double jump(s)
            }
        }
    }
    
//    if(theMoves.size() > 0) {
//        for(int i = 0; i < theMoves.size(); i++) {
//            cout << theMoves.at(i).isJump << " ";
//        }
//        cout << endl;
//    }
    return theMoves;
    
}

vector<GameBoard> GameBoard::findJump(int r, int c, int thePlayer) {
    
    vector<GameBoard> theMoves;
    
    int player = board.at(r).at(c);
    if(player != thePlayer and player != K_WEIGHT*thePlayer) {
        cout << "Expected " << thePlayer << " , player at r,c " << player << endl;
        return theMoves;
    }
    
    thePlayer = player;
    
    bool isKing = false;
    if(abs(thePlayer) == K_WEIGHT)
        isKing = true;
    
    for(int cDelt = -1; cDelt <= 1; cDelt++) {
        for(int rDelt = -1; rDelt <= 1; rDelt++) {
            
            if(!isKing and rDelt != (thePlayer/abs(thePlayer)))
                continue;
            
            if(rDelt == 0 or cDelt == 0)
                continue;
            
    //        int rDelt = thePlayer;
            
            GameBoard gB = *this;
            gB.startX = c;  // IS THIS RIGHT???
            gB.startY = r;  // IS THIS RIGHT???
            
            int newR = r + rDelt;
            int newC = c + cDelt;
            
            //        if(newR == 0 and newC == 0)
            //            continue;
            if(newR >= DIMENSION or newC >= DIMENSION)
                continue;
            if(newR < 0 or newC < 0)
                continue;
            
            if(board.at(newR).at(newC) == thePlayer)
                continue;
            
    //        if(board.at(newR).at(newC) == MPT) {  //SIMPLE MOVE
    //            gB = *this;
    //            gB.board.at(r).at(c) = MPT;
    //            gB.board.at(newR).at(newC) = thePlayer;
    //            gB.score = gB.getVal();
    //            theMoves.push_back(gB);
    //        }
            
            int newRFar = newR + rDelt;
            int newCFar = newC + cDelt;
            
            if(newRFar >= DIMENSION or newCFar >= DIMENSION)
                continue;
            if(newRFar < 0 or newCFar < 0)
                continue;
            
            int curr = 0;
            int newLoc = 0;
            int newLocFar = 0;
            
            if(thePlayer != 0)
                curr = thePlayer/abs(thePlayer);
            if(getAt(newR, newC) != 0)
                newLoc = getAt(newR, newC)/abs(getAt(newR, newC));
            if(getAt(newRFar, newCFar) != 0)
                newLocFar = getAt(newRFar, newCFar)/abs(getAt(newRFar, newCFar));
            
            
            if(newLoc == (-1)*curr and newLocFar == MPT) { //SIMPLE JUMP
                gB = *this;
                gB.startX = c;  // IS THIS RIGHT???
                gB.startY = r;  // IS THIS RIGHT???
                
                gB.board.at(r).at(c) = MPT;
                gB.board.at(newR).at(newC) = MPT;
                
                if((curr == -1 and newRFar == 0) or (curr == 1 and newRFar == DIMENSION-1))
                    gB.board.at(newRFar).at(newCFar) = curr*K_WEIGHT;
                else
                    gB.board.at(newRFar).at(newCFar) = thePlayer;
//                gB.board.at(newR + rDelt).at(newC + cDelt) = thePlayer;
                
                gB.endX = newCFar;  // IS THIS RIGHT???
                gB.endY = newRFar;  // IS THIS RIGHT???
                
                gB.score = gB.getVal();
                gB.isJump = true;
                theMoves.push_back(gB);
            }
        }
    }
    
    return theMoves;
    
}

void GameBoard::setBoard(vector< vector<int> > newBoard) {
    
    board.clear();
    
    for (int row = 0; row < DIMENSION; ++row) {
        
        board.push_back( vector<int>() );
        
        for (int col = 0; col < DIMENSION; ++col) {
            board.at(row).push_back(newBoard.at(row).at(col));
        }
    }
    
    score = getVal();
    
}

//bool isOpponent(



