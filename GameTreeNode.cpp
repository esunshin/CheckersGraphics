/* 
 * File:   GameBoard.cpp
 */


#include "GameTreeNode.h"

using namespace std;

GameTreeNode::GameTreeNode() {
    
    GameBoard tempBoard;
    currBoard.setBoard(tempBoard.getBoard());
    currBoard.setScore(tempBoard.getScore());
    
    playerMoved = MPT;
    
}

GameTreeNode::GameTreeNode(GameBoard theBoard) {
    currBoard = theBoard;
    playerMoved = MPT;
}

GameTreeNode::GameTreeNode(GameBoard theBoard, int pMoved) {
    currBoard = theBoard;
    playerMoved = pMoved;
}

vector< GameTreeNode > GameTreeNode::setNextMoves(vector< GameTreeNode > theMoves) {
    nextMoves = theMoves;
    numMoves = nextMoves.size();
    return nextMoves;
}

vector< GameTreeNode > GameTreeNode::addNextMove(GameBoard theMove) {
    GameTreeNode tmpGTN(theMove);
    nextMoves.push_back(tmpGTN);
    numMoves = nextMoves.size();
    return nextMoves;
}

GameTreeNode* GameTreeNode::getNextMoveRef(int index) {
    if(nextMoves.size() != 0)
        return &(*(nextMoves.begin() + index));
    return nullptr;
}

GameTreeNode GameTreeNode::getNextMove(int index) {
    if(nextMoves.size() != 0)
        return nextMoves.at(index);
    GameTreeNode mptNode;
    mptNode.numMoves = -1;
    return mptNode;
    
}

vector< GameTreeNode > GameTreeNode::generateTreeLevel(int depth, int startPlayer, bool isRoot) {
    
    vector< GameTreeNode > mptVect;
    
    if(isRoot) {
        this->playerMoved = 0;
        isRoot = false;
    }
    else
        this->playerMoved = (-1)*startPlayer; // IS THIS RIGHT?
    
    
    
    if(depth <= 0) {
        return mptVect;
    }
    
    vector< GameTreeNode > tmpVect = findNextMoves(startPlayer);
    this->nextMoves = tmpVect;
    
    
    depth--;
    startPlayer = (-1)*startPlayer; //RED, BLK
    
    for(int i = 0; i < tmpVect.size(); i++) {
        this->nextMoves.at(i).nextMoves = this->nextMoves.at(i).generateTreeLevel(depth, startPlayer, isRoot);
    }
    
    return this->nextMoves;
    
}


int GameTreeNode::getScoreSum() {
    
    if(nextMoves.size() == 0)
        return getScore();
    
    int theSummedScores = 0;
    
    for(int i = 0; i < nextMoves.size(); i++) {
        theSummedScores += nextMoves.at(i).getScoreSum();
    }
    return theSummedScores;
}

GameTreeNode GameTreeNode::nextMove(int depth, int player) {
    
    GameTreeNode theTree = *this;
    //    theTree.getCurrBoard().print(); // PRINT!!
    theTree.generateTreeLevel(depth, player, true);
//    cout << theTree.getNextMoves().size() << endl;
    
    //    printTree(theTree);
    
//    cout << theTree.getScoreSum() << endl;
    
    GameTreeNode maxNext = theTree.getNextMove(0);
    int maxNextScore = (player)*theTree.getNextMove(0).getScore();
    
    for(int i = 0; i < theTree.getNextMoves().size(); i++) {
        int tmpScore = (player)*theTree.getNextMove(i).getScoreSum();
        if(tmpScore > maxNextScore) {
            maxNextScore = tmpScore;
            maxNext = theTree.getNextMove(i);
        }
//        theTree.getNextMove(i).getCurrBoard().print();
//        cout << tmpScore << endl << endl;
    }
//    cout << endl;

    return maxNext;
    //    theTree.getCurrBoard().print(); //PRINT!!!
//    cout << endl;
    
    
    
}

// this should return the 'best' next move in the tree for the 'player'
// -> maximizes tree value for curr player
GameTreeNode GameTreeNode::nextMove(int player) { //HERE!!!
    
    GameTreeNode theTree = *this;
    
    GameTreeNode maxNext = theTree.getNextMove(0);
    int maxNextScore = theTree.getNextMove(0).getScoreSum();
    
    if(player == RED) { //RED == -1
        for(int i = 0; i < theTree.getNextMoves().size(); i++) {
            int tmpScore = theTree.getNextMove(i).getScoreSum();

            if(tmpScore < maxNextScore) {
                maxNextScore = tmpScore;
                maxNext = theTree.getNextMove(i);
            }

        }
    }
    else {
        for(int i = 0; i < theTree.getNextMoves().size(); i++) {
            int tmpScore = theTree.getNextMove(i).getScoreSum();
            
            if(tmpScore > maxNextScore) {
                maxNextScore = tmpScore;
                maxNext = theTree.getNextMove(i);
            }
            
        }
    }
    return maxNext;
    
}

bool GameTreeNode::equals(GameTreeNode otherGTN) {
    GameBoard cBoard = currBoard;
    GameBoard oBoard = otherGTN.getCurrBoard();
    bool boardsEqual = cBoard.equals(oBoard);
    return boardsEqual;
}


GameTreeNode GameTreeNode::moveMade(GameBoard newBoard) {
    
    for(int i = 0; i < getNextMoves().size(); i++) {
        if(getNextMove(i).getCurrBoard().equals(newBoard))
            return getNextMove(i);
    }
    
    GameTreeNode mptNode;
    mptNode.setScore(-1);
    return mptNode;

}

void GameTreeNode::addLayer(int player) {
    
    player = (-1)*this->playerMoved;
    
    if(this->getNextMoves().size() > 0) {
        for(int i = 0; i < getNextMoves().size(); i++) {
            getNextMoveRef(i)->addLayer(player);
        }
        return;
    }
    
    this->setNextMoves(player); //player or -player ???
    
}


vector< GameTreeNode > GameTreeNode::setNextMoves(int thePlayer) {
    setNextMoves(currBoard.findNextMoves(thePlayer), thePlayer);
    return nextMoves;
}

vector< GameTreeNode > GameTreeNode::setNextMoves(vector< GameBoard > theMoves, int player) {
    nextMoves.clear();
    for(int i = 0; i < theMoves.size(); i++) {
        GameTreeNode tmpGTN(theMoves.at(i), player);
        nextMoves.push_back(tmpGTN);
    }
    numMoves = nextMoves.size();
    return nextMoves;
}

vector< GameTreeNode > GameTreeNode::findNextMoves(int thePlayer) { //is this even involved???
    vector< GameBoard > theMoves = currBoard.findNextMoves(thePlayer);
    vector< GameTreeNode > theMoveNodes;
    for(int i = 0; i < theMoves.size(); i++) {
        theMoveNodes.push_back(GameTreeNode(theMoves.at(i), thePlayer));
    }

    return theMoveNodes;
}

vector< GameTreeNode > GameTreeNode::setNextMoves(vector< GameBoard > theMoves) {
    nextMoves.clear();
    for(int i = 0; i < theMoves.size(); i++) {
        GameTreeNode tmpGTN(theMoves.at(i));
        nextMoves.push_back(tmpGTN);
    }
    numMoves = nextMoves.size();
    return nextMoves;
}


GameTreeNode GameTreeNode::makeMove(int player) {

    GameTreeNode tmpNode = this->nextMove(player);
    tmpNode.addLayer(player);
    
    return tmpNode;

}

GameTreeNode GameTreeNode::makeMove(GameBoard nextMove, int player) {
    int i;
    for(i = 0; i < nextMoves.size(); i++) {
        if(nextMoves.at(i).currBoard.equals(nextMove)) {
            break;
        }
    }
    
    GameTreeNode tmpNode = nextMoves.at(i);
    
    tmpNode.addLayer(player);
    
    return tmpNode;
    
}


