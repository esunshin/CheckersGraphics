//
//  main.cpp
//  
//
//  Created by Ezra Sunshine on 6/5/17.
//
//

#include <stdio.h>
#include "GameTreeNode.h"
#include <math.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


GLfloat red[3] = {1.0, 0.0, 0.0};
GLfloat green[3] = {0.0, 1.0, 0.0};
GLfloat blue[3] = {0.0, 0.0, 1.0};
GLfloat black[3] = {0.0, 0.0, 0.0};
GLfloat white[3] = {1.0, 1.0, 1.0};
GLfloat gray[3] = {0.5, 0.5, 0.5};
GLfloat yellow[3] = {1.0, 1.0, 0.0};
GLfloat magenta[3] = {1.0, 0.0, 1.0};
GLfloat cyan[3] = {0.0, 1.0, 1.0};

GLint fillmode = 0;

const int WINDOWSIZE = 500;
const int OFFSET = 10;
const float DEG2RAD = 3.14159/180;
const float HEAVY = 2;

//coordinates of last mouse click
double mouse_x=-10, mouse_y=-10;
//initialized to a point outside the window

void display(void);
void keypress(unsigned char key, int x, int y);
void mousepress(int button, int state, int x, int y);
void timerfunc();
void drawShape();
void drawDot(int row, int col, int player);
void drawDot(int row, int col, int player, float weight);
void drawFilledDot(int row, int col, int player);
void drawBoard(GameBoard theBoard);
void drawBoard(GameBoard theBoard, int newX, int newY);

//void drawShape(); // Draw something..

void printPossMoves(vector< GameBoard > possibleMoves);
int getZeroDepth(GameTreeNode theRoot);
void printWidths(GameTreeNode theRoot);
vector< GameTreeNode > isLegalStart(GameTreeNode theTree, int x, int y);
int isLegalEnd(vector< GameTreeNode > theStarts, int x, int y);
void printWithIndexes(GameBoard theBoard);
GameBoard mergeBoards(GameBoard currBoard, vector< GameBoard > theNextMoves);


int getColFromClick(int clickX);
int getRowFromClick(int clickY);
int checkForWin(GameBoard theBoard);

bool isUsersMove = false;
bool isPaused = true;
bool hasGeneratedMerged = false;
bool gameOver = false;

GameTreeNode theGame;
GameBoard mergedBoard;
vector< GameBoard > nextMoves;
vector< int > nextMovesRow;
vector< int > nextMovesCol;

int prevClickX = -1;
int prevClickY = -1;

/* ****************************** */
void display(void) {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity(); //clear the matrix
    
    
    /* The default GL window is [-1,1]x[-1,1] with the origin in the
     center.
     
     Our system of coordinates (in which we generate our points) is
     (0,0) to (WINSIZE,WINSIZE), with the origin in the lower left
     corner.
     
     We need to map the points to [-1,1] x [-1,1]
     
     Assume we are the local coordinate system.
     
     First we scale down to [0,2] x [0,2] */
    glScalef(2.0/WINDOWSIZE, 2.0/WINDOWSIZE, 1.0);
    /* Then we translate so the local origin goes in the middle of teh
     window to (-WINDOWSIZE/2, -WINDOWSIZE/2) */
    glTranslatef(-WINDOWSIZE/2, -WINDOWSIZE/2, 0);
    //now we draw in our local coordinate system (0,0) to
    //(WINSIZE,WINSIZE), with the origin in the lower left corner.
    
    
    drawShape();

    drawBoard(theGame.getCurrBoard());


    if(hasGeneratedMerged) {
        drawBoard(mergedBoard);
    }
    
    /* execute the drawing commands */
    glFlush();
}


/* ****************************** */
void keypress(unsigned char key, int x, int y) {
    switch(key) {
        
        case 'q':
            exit(0);
            break;
            
        case 'p':
            isPaused = !isPaused;
            glutPostRedisplay();
            break;
            
    }
    
}


/*
 Usage
 
 void glutMouseFunc(void (*func)(int button, int state, int x, int y));
 
 Description
 
 glutMouseFunc sets the mouse callback for the current window. When a
 user presses and releases mouse buttons in the window, each press and
 each release generates a mouse callback. The button parameter is one
 of GLUT_LEFT_BUTTON, GLUT_MIDDLE_BUTTON, or GLUT_RIGHT_BUTTON. For
 systems with only two mouse buttons, it may not be possible to
 generate GLUT_MIDDLE_BUTTON callback. For systems with a single mouse
 button, it may be possible to generate only a GLUT_LEFT_BUTTON
 callback. The state parameter is either GLUT_UP or GLUT_DOWN
 indicating whether the callback was due to a release or press
 respectively. The x and y callback parameters indicate the window
 relative coordinates when the mouse button state changed. If a
 GLUT_DOWN callback for a specific button is triggered, the program can
 assume a GLUT_UP callback for the same button will be generated
 (assuming the window still has a mouse callback registered) when the
 mouse button is released even if the mouse has moved outside the
 window.
 */

void mousepress(int button, int state, int x, int y) {
    
    
    if(state == GLUT_DOWN) {
//
        mouse_x = x;
        mouse_y = y;
        
        
        //(x,y) are in wndow coordinates, where the origin is in the upper
        //left corner; our reference system has the origin in lower left
        //corner, this means we have to reflect y
        mouse_y = WINDOWSIZE - mouse_y;

        int col = getColFromClick(mouse_x);
        int row = getRowFromClick(mouse_y);
        cout << "Col: " << col << endl;
        cout << "Row: " << row << endl;
        
        if(col == prevClickX and row == prevClickY) {
            cout << "same click (181)" << endl;
            return;
        }
        else {
            prevClickX = col;
            prevClickY = row;
        }
        
        if(isUsersMove and hasGeneratedMerged) {
            bool clickedNextMove = false;
            int i;
            //COULD REPLACE W/ CHECK AGAINST A PREMADE VECT! ->Don't think this will be faster
            for(i = 0; i < nextMoves.size(); i++) {
                if(nextMoves.at(i).getEndX() == col) {
                    if(nextMoves.at(i).getEndY() == row) {
                        clickedNextMove = true;
                        break;
                        cout << "CLICKED NEXT MOVE" << endl;
                    }
                }
            }
            if(clickedNextMove) {
                theGame = theGame.makeMove(nextMoves.at(i), RED);
                nextMoves.clear();
                hasGeneratedMerged = false;
                isUsersMove = false;
                return;
            }
            else
                nextMoves.clear();
        }
        
        if(isUsersMove) {
            cout << "USER's MOVE" << endl;
            
            vector< GameTreeNode > startMoves = isLegalStart(theGame, col, row);
            
            if(startMoves.size() > 0)
                cout << "Is Legal, size = " << startMoves.size() << endl;
            else
                cout << "Not Legal" << endl;
            
//            vector< GameBoard > nextMoves;
            
            for(int i = 0; i < startMoves.size(); i++) {
                
                nextMoves.push_back(startMoves.at(i).getCurrBoard());
                
            }
            
            cout << nextMoves.size() << endl;
            
            mergedBoard = mergeBoards(theGame.getCurrBoard(), nextMoves);
            cout << 313 << endl;
            hasGeneratedMerged = true;
//            isUsersMove = false;
//            isPaused = true; //unpause to require 'p' for computer move!!!

        }
        
//        if(isUsersMove and hasGeneratedMerged) {
//            cout << nextMoves.size();
//        }
        
    }
    
    glutPostRedisplay();
}



/* Handler for window re-size event. Called back when the window first appears and
 whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    
    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);
    
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    gluOrtho2D(0.0, (GLdouble) width, 0.0, (GLdouble) height);
}





void timerfunc() {
    
    if(!isUsersMove and !isPaused and !gameOver) {
        
        int winCheck = checkForWin(theGame.getCurrBoard());
        if(winCheck != 0) {
            cout << "GAME OVER!!!" << endl;
            gameOver = true;
            if(winCheck == BLK)
                cout << "Black won!" << endl;
            else if(winCheck == RED)
                cout << "Red won!" << endl;
            return;
            //exit(1);
        }
        
        prevClickX = -1;
        prevClickY = -1;
        
        hasGeneratedMerged = false;
        
        theGame = theGame.makeMove(BLK);
        
     
        theGame.print();
        
        cout << 364 << endl;
        glFlush();
        glutPostRedisplay();
        
        isUsersMove = !isUsersMove;
        
//        cout << "DEPTH: " << getZeroDepth(theGame) << endl;
//        cout << "Widths: " << endl;
//        printWidths(theGame);
        
        winCheck = checkForWin(theGame.getCurrBoard());
        if(winCheck != 0) {
            cout << "GAME OVER!!!" << endl;
            if(winCheck == BLK)
                cout << "Black won!" << endl;
            else if(winCheck == RED)
                cout << "Red won!" << endl;
            return;
            //exit(1);
        }
    }
    
}



void drawShape() {
    
    glColor3fv(black);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBegin(GL_LINES);

    for(int i = 0; i <= DIMENSION; i++) {
        
        glVertex2f(OFFSET, OFFSET + (i * ((WINDOWSIZE-2*OFFSET)/DIMENSION)));
        glVertex2f(WINDOWSIZE-OFFSET, OFFSET + (i * ((WINDOWSIZE-2*OFFSET)/DIMENSION)));
        
        glVertex2f(OFFSET + (i * ((WINDOWSIZE-2*OFFSET)/DIMENSION)), OFFSET);
        glVertex2f(OFFSET + (i * ((WINDOWSIZE-2*OFFSET)/DIMENSION)), WINDOWSIZE-OFFSET);
        
    }
        
        glEnd();
    
}

void drawDot(int row, int col, int player) {
    
    // WILL NEED TO DECOMPOSE INTO TRIANGLES TO FILL!!!
    
    if(player == RED)
        glColor3fv(red);
    else if(player == BLK)
        glColor3fv(black);
    else if(player == KRED) {
        drawFilledDot(row, col, player);
        return;
    }
    else if(player == KBLK) {
        drawFilledDot(row, col, player);
        return;
    }
    else if(player == RED*2)
        glColor3fv(cyan);
    else if(player == BLK*2)
        glColor3fv(gray);
    else if(player == KRED*2) {
        drawFilledDot(row, col, player);
        return;
    }
    else if(player == KBLK*2) {
        drawFilledDot(row, col, player);
        return;
    }

    
    float radius = (WINDOWSIZE-2*OFFSET)/(2*DIMENSION);
    
    float x = OFFSET + radius*((2*col)+1);
    float y = WINDOWSIZE - OFFSET - radius*((2*row)+1);
    
    radius -= 5;
    
    glBegin(GL_LINE_LOOP);
    
    for (int i=0; i < 360; i += 15)
    {
        float degInRad = i*DEG2RAD;
        glVertex2f(cos(degInRad)*radius + x, sin(degInRad)*radius + y);
    }
    
    glEnd();
    
    
}


void drawDot(int row, int col, int player, float weight) {
    glLineWidth(weight);
    drawDot(row, col, player);
    glLineWidth(1);
    return;
}


void drawFilledDot(int row, int col, int player) {
    
    if(player == KRED)
        glColor3fv(red);
    else if(player == KBLK)
        glColor3fv(black);
    else if(player == KRED*2)
        glColor3fv(cyan);
    else if(player == KBLK*2)
        glColor3fv(gray);
    
    float radius = (WINDOWSIZE-2*OFFSET)/(2*DIMENSION);
    
    float x = OFFSET + radius*((2*col)+1);
    float y = WINDOWSIZE - OFFSET - radius*((2*row)+1);
    
    radius -= 5;
    
//    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//    glLineWidth(3);
    glBegin(GL_TRIANGLES);
    
    for (int i=0; i < 360; i += 15)
    {
        
        float degInRad = i*DEG2RAD;
        glVertex2f(cos(degInRad)*radius + x, sin(degInRad)*radius + y);
        int j = i+15;
        degInRad = j*DEG2RAD;
        glVertex2f(cos(degInRad)*radius + x, sin(degInRad)*radius + y);
        glVertex2f(x, y);
                   
    }
    
    glEnd();
    
    
    
}


int getColFromClick(int clickX) {
    
    float radius = (WINDOWSIZE-2*OFFSET)/(2*DIMENSION);
    int col = round((((clickX - OFFSET)/radius)-1)/2);
    return col;
    
}

int getRowFromClick(int clickY) {
    
    float radius = (WINDOWSIZE-2*OFFSET)/(2*DIMENSION);
    int row = round((((WINDOWSIZE - OFFSET - clickY)/radius)-1)/2);
    return row;
    
}

void drawBoard(GameBoard theBoard) {
    drawBoard(theBoard, 0, 0);
    return;
    for(int i = 0; i < DIMENSION; i++) {
        for(int j = 0; j < DIMENSION; j++) {
            
            if(theBoard.getAt(i, j) == 0)
                continue;
            else(drawDot(i, j, theBoard.getAt(i, j)));
        }
    }
    
    
}

void drawBoard(GameBoard theBoard, int newX, int newY) {
    newX = theBoard.getEndX();
    newY = theBoard.getEndY();
    for(int i = 0; i < DIMENSION; i++) {
        for(int j = 0; j < DIMENSION; j++) {
            
            if(theBoard.getAt(i, j) == 0)
                continue;
            else if(j == newX and i == newY)
                drawDot(i, j, theBoard.getAt(i, j), HEAVY);
            else
                drawDot(i, j, theBoard.getAt(i, j));
        }
    }
    
}

/* ****************************** */
int main(int argc, char** argv) {
    
    
    theGame.generateTreeLevel(5, BLK, true);
    isPaused = true;
    
    /* initialize GLUT  */
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WINDOWSIZE, WINDOWSIZE);
    glutInitWindowPosition(100,100);
    glutCreateWindow(argv[0]);
    
    /* register callback functions */
    glutDisplayFunc(display);
    glutKeyboardFunc(keypress);
    glutMouseFunc(mousepress);
    //  if (argc > 1) {
    glutIdleFunc(timerfunc);
    //  }
    
    /* init GL */
    /* set background color black*/
    glClearColor(1, 1, 1, 1);
    /* here we can enable depth testing and double buffering and so
     on */
    
    
    /* give control to event handler */
    glutMainLoop();
    return 0;
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

void printWidths(GameTreeNode theRoot) {
    
    if(theRoot.getNextMoves().size() == 0)
        return;

    cout << theRoot.getNextMoves().size() << endl;
    
    int maxWidthIndex = 0;
    int maxWidth = theRoot.getNextMoves().at(maxWidthIndex).getNextMoves().size();
    for(int i = 0; i < theRoot.getNextMoves().size(); i++) {
        if(theRoot.getNextMoves().at(i).getNextMoves().size() > maxWidth) {
            maxWidthIndex = i;
            maxWidth = theRoot.getNextMoves().at(i).getNextMoves().size();
        }
    }

    
    printWidths(theRoot.getNextMove(maxWidthIndex));
    
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

GameBoard mergeBoards(GameBoard currBoard, vector< GameBoard > theNextMoves) {
    
    //cout << "Next Moves Size: " << theNextMoves.size() << endl;
    for(int i = 0; i < theNextMoves.size(); i++) {
        int newRow = theNextMoves.at(i).getEndY();
        int newCol = theNextMoves.at(i).getEndX();
        //    cout << "newRow: " << newRow << "newCol: " << newCol;
        int player = theNextMoves.at(i).getAt(newRow, newCol);
        //!?!?
        cout << " Player: " << player << endl;
        currBoard.setAt(newRow, newCol, 2*player);
    }
    return currBoard;
    
}


int checkForWin(GameBoard theBoard) {
    
    int pos = 0;
    int neg = 0;
    
    for (int row = 0; row < DIMENSION; row++) {
        for (int col = 0; col < DIMENSION; col++) {
            if(theBoard.getAt(row, col) == 0)
                continue;
            else if(theBoard.getAt(row, col) > 0)
                pos++;
            else if(theBoard.getAt(row, col) < 0)
                neg++;
        }
        if(pos > 0 and neg > 0)
            return 0;
    }
    
    if(pos > 0 and neg == 0)
        return BLK;
    else if(neg > 0 and pos == 0)
        return RED;
    else return MPT;
    
}




