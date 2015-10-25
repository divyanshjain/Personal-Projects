
#include <iomanip>

#include <limits.h> 
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <strings.h>
#include <sstream>
#include <queue>
#include <sstream>
#include <ctime>
#include <stdio.h>
#include <strings.h>
#include <algorithm>
#include <stdlib.h>
#include <algorithm>
#include <ctype.h>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <cmath>
#define MIN 1
#define MAX 0
using namespace std;

class boardprops {
public:
    int algo;
    int myplayer;
    int cod;
    int pits;
    vector<int> boardstate;
};

void printboard(int board[]); // for printing the mancala board
void printboard2(boardprops& game, int board[]);
int askforselection(int board[]);
bool checkboard(boardprops& game, int board[]); // checking the board to make sure that it isn't over
bool takeMove(boardprops& game, int board[], int spotinarray, int human_or_comp); //for actually making the move
int findacross(boardprops& game, int ending, int turn); // find the bin number across
bool checkforone(boardprops& game, int board[], int turn); // check if there is only one move possible.
int findone(boardprops& game, int board[], int turn); // find the one possible move.

typedef struct move {
    int binNum;
    int score;
} Move;
Move minmax(boardprops& game, int board[], int turn, int d, int maxD, int minOrMax, int alpha, int beta, ofstream &outfile, Move& prevm); //mini-max function with alpha, beta pruning.
Move minmax2(boardprops& game, int board[], int turn, int d, int maxD, int minOrMax, ofstream& outfile, Move& prevm);
Move minmax3(boardprops& game, int board[], int turn, int d, int maxD, int minOrMax, ofstream& outfile, Move& prevm);
int computerChooseMove(boardprops& game, int board[], int maxD, int turn, ofstream &outfile); // allow the computer to make a move.
void writeStateToFile(boardprops& game, int board[], ofstream& outfile2);
int evalFunction(int board[], int minOrMax); // Evaluate the board based on who is currently playing.
int evalFunction2(boardprops& game, int board[], int minOrMax);
int checkform(int board[], int choice);
void readprops(ifstream &infile, boardprops& game);
void setboardprops(boardprops& game, int barray[], int length);
void setpits(boardprops& game, int s);
void setmyplayer(boardprops& game, string s);
void setcod(boardprops& game, string s);
void setalgo(boardprops& game, string s);
string intToNode(boardprops& game, int n);
bool repeatchance = false;
bool flag = false;
string posinfi = "Infinity";
string neginfi = "-Infinity";
bool mTempNotCalled = true;
bool retEval = false;
int prevNodeDepth = 0;
string returnstring(int no);
int lineno = 0;
vector<int> movesToState;
int printcount = 0;

int main(int argc,char **argv) {
    srand(time(0));
    int m;
    ifstream infile;
    boardprops game;
    //infile.open("C:/Dj/My Stuff/USC/Fall 15/AI/Homework 2/newinputfiles/input_4.txt");
    infile.open(argv[2]);
    readprops(infile, game);

    ofstream outfile;
    if (game.algo != 1){
      //  outfile.open("C:/Dj/My Stuff/USC/Fall 15/AI/Homework 2/newinputfiles/final4.txt");
        outfile.open("traverse_log.txt");
    }
    int turn = 0; // for switching turns.

    int maxDepth = game.cod;
    int board[(2 * game.pits) + 2];
    int l = (2 * game.pits) + 1;


    for (int i = 0; i <= l; i++) {
        board[i] = game.boardstate[i];
    }
    if (game.myplayer == 2) {
        turn = 1;
    } else {
        turn = 0;
    }

    int choiceofmove = -5;
    bool gameOver = false; 
    bool onlyone = false; 
    bool doesntmatter = false;
    int one;
    while (!flag) {
        if (turn == MIN) {// player2 move
            
         //   cout << endl << "player 2 to play " << endl;

            
            onlyone = false;
            if (onlyone) {
                one = findone(game, board, turn); //find location of move.
                m = one; //set the found location as the move.
                outfile << m << "\r" << endl;
            } else {
                m = computerChooseMove(game, board, maxDepth, turn, outfile); //otherwise use the minmax function to find the best move, given a certain depth.
            }

         //   cout << endl << "board state before final move" << endl;

            doesntmatter = takeMove(game, board, m, turn);

            gameOver = checkboard(game, board); //see if the game is over
            printcount++;
            while (repeatchance && !gameOver) {
           //     outfile << "don't see log" << "\r" << endl;
                m = computerChooseMove(game, board, maxDepth, turn, outfile);
                doesntmatter = takeMove(game, board, m, turn);
                gameOver = checkboard(game, board);
            }
            flag = true;
            if (flag) {
                break;
            } else {
                turn = 0; //switch turn so that the human can go.
            }
        }/*if (turn == MIN) {// player2 move
            //in order to make the computer more efficient, if there is only one move possible
            //there is not need to use minimax to find the move.
           
        //    cout << endl << "player 2 to play " << endl;

           // onlyone = checkforone(game, board, turn);
            onlyone = false;
            if (onlyone) {
                one = findone(game, board, turn); //find location of move.
                m = one; //set the found location as the move.
                outfile<< m<<"\r" << endl;
            } else {
                m = computerChooseMove(game, board, maxDepth, turn,outfile); //otherwise use the minmax function to find the best move, given a certain depth.
            }
            //cout << "Computer moved at "<< m << ", resulting in board below "<< endl;
         //   cout << endl << "board state before final move" << endl;
           
            doesntmatter = takeMove(game, board, m, turn);
            //printboard2(game,board); //print the board.
            gameOver = checkboard(game, board); //see if the game is over
            movesToState.pop_back();
            while(movesToState.size()!=0 && !gameOver){
             //   outfile<<"don't see log"<<"\r"<<endl;
                m = movesToState.back();
                doesntmatter = takeMove(game, board, m, turn);
                movesToState.pop_back();
                gameOver = checkboard(game, board);
            }
            flag = true;
            if (flag) {
                break;
            } else {
                turn = 0; //switch turn so that the human can go.
            }
        }*/
            /*else { //player1 move
                printboard2(game, board);
                cout << endl << "player 1 to play " << endl;
                //onlyone = checkforone(game, board, turn);
                onlyone = false;
                if (onlyone) {
                    one = findone(game, board, turn);
                    m = one;
                } else {
                    m = computerChooseMove(game, board, maxDepth, turn,outfile);
                }
                cout << endl << "board state before final move" << endl;
                printboard2(game, board);
                doesntmatter = takeMove(game, board, m, turn);
                //printboard2(game,board); //print the board.
                gameOver = checkboard(game, board); //see if the game is over
                movesToState.pop_back();
                while(movesToState.size()!=0 && !gameOver){
                    //outfile<<"don't see log"<<"\r"<<endl;
                    m = movesToState.back();
                    doesntmatter = takeMove(game, board, m, turn);
                    movesToState.pop_back();
                    gameOver = checkboard(game, board);
                }
                flag = true;
                if (flag) {
                    break;
                } else {
                    turn = 1;
                }
            }*/
        else { //player1 move
           
            onlyone = false;
            if (onlyone) {
                one = findone(game, board, turn);
                m = one;
            } else {
                m = computerChooseMove(game, board, maxDepth, turn, outfile);
            }
       //     cout << endl << "board state before final move" << endl;

            doesntmatter = takeMove(game, board, m, turn);
            //printboard2(game,board); //print the board.
            gameOver = checkboard(game, board); //see if the game is over
            printcount++;
            while (repeatchance && !gameOver) {
        //        outfile << "don't see log" << "\r" << endl;
                m = computerChooseMove(game, board, maxDepth, turn, outfile);
                doesntmatter = takeMove(game, board, m, turn);
                gameOver = checkboard(game, board);
            }
            flag = true;
            if (flag) {
                break;
            } else {
                turn = 1;
            }
        }
    }

    infile.close();
    outfile.close();
    cout << endl << "finalmove " << endl;
    ofstream outfile2;
    //outfile2.open("C:/Dj/My Stuff/USC/Fall 15/AI/Homework 2/newinputfiles/next_state.txt");
    outfile2.open("next_state.txt");
    writeStateToFile(game, board, outfile2);
    outfile2.close();
    printboard2(game, board); //print resulting board.
}

void remove_carriage_return(std::string& line) {
    if (*line.rbegin() == '\r') {
        line.erase(line.length() - 1);
    }
}

void readprops(ifstream& infile, boardprops& game) {
    string line;
    string player1, player2, mancala1, mancala2;
    //boardprops game;

    while (!infile.eof()) {
        //read first line, algo 
        getline(infile, line);
        remove_carriage_return(line);
        setalgo(game, line);

        //read second line, my player
        getline(infile, line);
        remove_carriage_return(line);
        setmyplayer(game, line);

        //read third line, depth.
        getline(infile, line);
        remove_carriage_return(line);
        setcod(game, line);

        //read fourth,fifth,sixth and seventh line,
        getline(infile, player2);
        remove_carriage_return(player2);
        getline(infile, player1);
        remove_carriage_return(player1);
        getline(infile, mancala2);
        remove_carriage_return(mancala2);
        getline(infile, mancala1);
        remove_carriage_return(mancala1);

        int npits = 1;
        for (int i = 0; i < player1.length(); i++) {
            if (isspace(player1[i]))
                npits++;
        }
        setpits(game, npits);
        int b[2 * npits + 2];
        for (int i = 0; i < npits; i++) {
            b[i] = atoi(player1.substr(0, player1.find(" ")).c_str());
            player1 = player1.substr(player1.find(" ") + 1);
        }
        b[npits] = atoi(mancala1.c_str());

        for (int i = 2 * npits; i > npits; i--) {
            b[i] = atoi(player2.substr(0, player2.find(" ")).c_str());
            player2 = player2.substr(player2.find(" ") + 1);
        }
        b[(2 * npits) + 1] = atoi(mancala2.c_str());

        setboardprops(game, b, (2 * npits) + 2);
        return;
    }
    return;
}

void setpits(boardprops& game, int s) {
    game.pits = s;
}

void setmyplayer(boardprops& game, string s) {
    remove_carriage_return(s);
    game.myplayer = atoi(s.c_str());
}

void setcod(boardprops& game, string s) {
    remove_carriage_return(s);
    game.cod = atoi(s.c_str());
}

void setalgo(boardprops& game, string s) {
    remove_carriage_return(s);
    game.algo = atoi(s.c_str());
}

void setboardprops(boardprops& game, int barray[], int length) {
    // game.boardstate = barray;
    for (int i = 0; i < length; i++) {
        // game.boardstate[i] = barray[i];
        game.boardstate.push_back(barray[i]);
    }

}

string returnstring(int no) {
    if (no == INT_MAX)
        return posinfi;
    else if (no == INT_MIN)
        return neginfi;
    else {
        ostringstream convert;
        convert << no;
        return convert.str();
    }
}

string intToNode(boardprops& game, int n) {

    if (n == -9998) {
        return "root";
    }
    if (n >= 0 && n < game.pits) {
        ostringstream convert;
        convert << n + 2;
        return "B" + convert.str();
    } else if (n > game.pits && n <= 2 * game.pits) {
        ostringstream convert;
        convert << (2 * game.pits) - n + 2;
        return "A" + convert.str();
    } else {
        return "invalid node";
    }
}

bool takeMove(boardprops& game, int board[], int choice, int minOrmax) {
    int avoid; //location to avoid, the other person's mancala
    repeatchance = false;
    bool islast = false;
    if (minOrmax == 1) { //comp playing turn 1
        avoid = game.pits;
    } else {//user playing turn 0
        avoid = (2 * game.pits) + 1;
    }
    int picked = board[choice]; //pick up the marbles and store them
    int next = choice + 1; //find the next bin number
    if (next == avoid) { //if this has to be avoided, be sure to change it
        if (avoid == (2 * game.pits) + 1) {
            next = 0; //avoid comp mancala for human
        } else if (avoid == game.pits) {
            next = game.pits + 1; //avoid human mancala for comp
        }
    }
    board[choice] = 0; //empty bin just picked
    bool changed = false; //if the for loop runs into 13 or 6, this make sure that it doesn't increment twice.
    for (; picked > 0; picked--) { //as long as the number of marbles picked up is less than 0 and decreasing with every iteration
        islast = false;
        if (next == avoid) {//after each increment, this check to make sure that the next bin number is not an avoider
            if (avoid == (2 * game.pits) + 1) {
                changed = true; //next is where 1 marble will be placed. changed is true because it avoids incrementing twice
                next = 0; //since we are avoiding 13, the next bin is 0
            } else {
                changed = true; //next is where 1 marble will be placed. changed is true because it avoids incrementing twice
                next = game.pits + 1; //since we are avoiding 6, the next bin is 7
            }
        }
        int currentinnext = board[next]; //get the current number of marbles in the next bin to be added 1
        board[next] = currentinnext + 1; // add one to the value of the marbles in the next bin
        int test = picked - 1; //to see if there will be no more marbles left to drop.
        if (test >= 0) {
            if (changed) {
                next++; //increment to next
                if (next == (2 * game.pits) + 2) { //make sure that the array loops
                    next = 0;
                }
            } else if (picked == 1 && currentinnext == 0) {
                if ((next == game.pits && minOrmax == 0) || (next == (2 * game.pits) + 1 && minOrmax == 1))
                    islast = true;
                next++;
                if (next == (2 * game.pits) + 2) {
                    next = 0;
                }
                break; //if last has only one to be dropped.
            } else {
                next++;
                if (next == (2 * game.pits) + 2) {
                    next = 0;
                }
            }
        }
    }
    int ending = next - 1; // location of ending bin
    if (ending == -1 && minOrmax == 1) {//player 2
        ending = (2 * game.pits) + 1;
        islast = true;
    }
    if (ending == -1 && minOrmax == 0) { // for player 1
        ending = (2 * game.pits);
    }

    if (ending != (2 * game.pits) + 1 && ending != game.pits) { // does NOT find across if landS in mancala

        if (minOrmax == 0 && board[ending] == 1 && ending > -1 && ending < game.pits) { // for player1
            int across = findacross(game, ending, minOrmax); // find across slot
            if (board[across] >= 0) {
                board[game.pits] = board[game.pits] + 1 + board[across]; // take the piece just landed
                board[ending] = 0;
                board[across] = 0; //empty bin across
                return true;
            } else {
                return false;
            }
        }
        if (minOrmax == 1 && board[ending] == 1 && ending > game.pits && ending < (2 * game.pits) + 1) { // for player 2
            int across = findacross(game, ending, minOrmax); // find across slot
            if (board[across] >= 0) {
                board[(2 * game.pits) + 1] = board[(2 * game.pits) + 1] + 1 + board[across]; // take the piece just landed
                board[ending] = 0; //empty bin on comp side
                board[across] = 0; //empty bin across
                return true; //return true if across value was captured. used in eval
            } else {
                return false;
            }
        }
    } else {
        repeatchance = true;
        return false;
    }
    return false;
}

Move minmax2(boardprops& game, int board[], int turn, int d, int maxD, int minOrMax, ofstream& outfile, Move& prevm) {
    //minOrMax is MAX if the computer is the player and MIN if the human is the player

    Move mTemp, m, m1;
    int board2[(2 * game.pits) + 2];
    bool check; //for checking if one side is empty
    bool doesntmatter; //return bool from takemove, for eval function and doens't matter here.
    check = checkboard(game, board); //check if one side is emtpy.
    if (d == maxD) {
        flag = true;
        m.score = evalFunction2(game, board, minOrMax); //evaluate the current board state and give a score.
        m.binNum = -1; //doesnt matter
        retEval = true;
       // cout << endl << "eval called at d " << d << " with eval value " << m.score << endl;
        return m;
    } else {
        if (minOrMax == MIN && turn == 1) { // MINIMizing player player 2 min
            m.score = INT_MAX;
            m.binNum = -1;
            for (int i = 2 * game.pits; i >= game.pits + 1; i--) { //user turn so only go through 6 bins on user side.
                if (board[i] != 0) {
                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                    int tempBin = prevm.binNum;
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                    if (printcount == 0)
                        outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(m.score) << "\r" << endl;

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move
                    check = checkboard(game, board);
                    if (check) {
                        if (maxD != (d + 1) && !repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "\r" << endl;
                        }
                        if (maxD != (d + 1) && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "\r" << endl;
                        }
                        if (maxD == (d + 1) && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "\r" << endl;
                        }
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "\r" << endl;
                    }

               //     cout << endl << "board when player2 min and move " << i << endl;
                  //  printboard2(game, board);
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                       // cout << endl << "player 2 min gets another chance for " << i << endl;
                        mTemp = minmax2(game, board, turn, d, maxD, MIN, outfile, prevm);
                        mTempNotCalled = false;
                    }

                    if (mTempNotCalled && !check)
                        mTemp = minmax2(game, board, (turn + 1) % 2, d + 1, maxD, MAX, outfile, prevm); //recursive call minmax with m.score(as beta) and alpha
                    prevm.binNum = tempBin;

                    if (retEval) {
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "\r" << endl;
                        prevm.binNum = tempBin;
                        retEval = false;
                    }
                    for (int k = 0; k < (2 * game.pits) + 2; k++) {
                        board[k] = board2[k];
                    }
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;


                    if (mTemp.score < m.score) {
                        m.score = mTemp.score;
                        m.binNum = i;
                    }



                    mTempNotCalled = true;




                }
            }
            if (printcount == 0)
                outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(m.score) << "\r" << endl;
            return m;
        }
        if (minOrMax == MIN && turn == 0) { // MINIMizing player player 1 min
            m.score = INT_MAX;
            m.binNum = -1;
            for (int i = 0; i <= game.pits - 1; i++) { //user turn so only go through 6 bins on user side.
                if (board[i] != 0) {

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                    int tempBin = prevm.binNum;
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                    if (printcount == 0)
                        outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(m.score) << "\r" << endl;

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move
                    check = checkboard(game, board);
                    if (check) {
                        if ((d + 1) != maxD && !repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "\r" << endl;
                        }
                        if ((d + 1) != maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "\r" << endl;
                        }
                        if ((d + 1) == maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "\r" << endl;
                        }
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "\r" << endl;
                    }
                //    cout << endl << "board when player1 min and move " << i << endl;
                  //  printboard2(game, board);
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                     //   cout << endl << "player 1 min gets another chance at" << i << endl;
                        mTemp = minmax2(game, board, turn, d, maxD, MIN, outfile, prevm);
                        mTempNotCalled = false;
                    }


                    if (mTempNotCalled && !check)
                        mTemp = minmax2(game, board, (turn + 1) % 2, d + 1, maxD, MAX, outfile, prevm); //recursive call minmax with m.score(as beta) and alpha
                    prevm.binNum = tempBin;
                    if (retEval) {
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "\r" << endl;
                        prevm.binNum = tempBin;
                        retEval = false;
                    }

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board[j] = board2[j];
                    }

                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;




                    if (mTemp.score < m.score) { //

                        m.score = mTemp.score; //if the found score it less than the kept score, so update score kept.
                        m.binNum = i; //bin number will be current index


                    }

                    mTempNotCalled = true;





                }
            }

            if (printcount == 0)
                outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(m.score) << "\r" << endl;
            return m;
        } else if (minOrMax == MAX && turn == 1) //comp player 2 max
        {

            m.score = INT_MIN;
            m.binNum = -1;
            for (int i = (2 * game.pits); i > game.pits; i--) {
                if (board[i] != 0) {
                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                    int tempBin = prevm.binNum;
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;


                    if (printcount == 0)
                        outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(m.score) << "\r" << endl;
                    //             

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move
                    check = checkboard(game, board);
                    if (check) {
                        if ((d + 1) != maxD && !repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "\r" << endl;
                        }
                        if ((d + 1) != maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "\r" << endl;
                        }
                        if ((d + 1) == maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "\r" << endl;
                        }

                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "\r" << endl;
                    }

                  //  cout << endl << "board when player2 max and move " << i << endl;
                //    printboard2(game, board);
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                     //   cout << endl << "player 2 max gets another chance at " << i << endl;
                        mTemp = minmax2(game, board, turn, d, maxD, MAX, outfile, prevm);
                        mTempNotCalled = false;
                    }

                    if (mTempNotCalled && !check)
                        mTemp = minmax2(game, board, (turn + 1) % 2, d + 1, maxD, MIN, outfile, prevm);
                    prevm.binNum = tempBin;
                    if (retEval) {
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "\r" << endl;
                        prevm.binNum = tempBin;
                        retEval = false;
                    }

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board[j] = board2[j];
                    }

                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;




                    if (mTemp.score > m.score) {

                        m.score = mTemp.score;
                        m.binNum = i;


                    }


                    mTempNotCalled = true;





                }
            }

            if (printcount == 0)
                outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(m.score) << "\r" << endl;
            if (d == 0)
                movesToState.push_back(m.binNum);
            return m;
        }//end of else if player 2 max
        else if (minOrMax == MAX && turn == 0) // player 1 max
        {

            m.score = INT_MIN;
            m.binNum = -1;

            for (int i = 0; i <= game.pits - 1; i++) {
                if (board[i] != 0) {
                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                 //   if (lineno > 30)
                  //      cout << "asasc";
                    int tempBin = prevm.binNum;
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;



                //    cout << lineno++ << endl;
                    if (printcount == 0)
                        outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(m.score) << "\r" << endl;

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move

                    check = checkboard(game, board);
                    if (check) {
                        if ((d + 1) != maxD && !repeatchance) {
                       //     cout << lineno++ << endl;
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "\r" << endl;
                        }

                        if ((d + 1) != maxD && repeatchance) {
                       //     cout << lineno++ << endl;
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "\r" << endl;
                        }
                        if ((d + 1) == maxD && repeatchance) {
                       //     cout << lineno++ << endl;
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "\r" << endl;
                        }
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                    //    cout << lineno++ << endl;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "\r" << endl;
                    }
                 //   cout << endl << "board when player1 max and move " << i << endl;
                //    printboard2(game, board);
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                    //    cout << endl << "player 1 max gets another chance at " << i << endl;
                        mTemp = minmax2(game, board, turn, d, maxD, MAX, outfile, prevm);
                        mTempNotCalled = false;
                    }



                    //if mtemp is set, then don't call.
                    if (mTempNotCalled && !check)
                        mTemp = minmax2(game, board, (turn + 1) % 2, d + 1, maxD, MIN, outfile, prevm); //recursive call minmax with m.score(as alpha) and beta
                    prevm.binNum = tempBin;
                    if (retEval) {
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "\r" << endl;
                        prevm.binNum = tempBin;
                        retEval = false;
                    }
                    for (int k = 0; k < (2 * game.pits) + 2; k++) {
                        board[k] = board2[k];
                    }
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;




                    if (mTemp.score > m.score) {

                        m.score = mTemp.score; //if the found score it greater than the kept score, so update score kept.
                        m.binNum = i; //bin number will be current index

                    }


                    mTempNotCalled = true;

                }
            }
          //  cout << lineno++ << endl;
            if (printcount == 0)
                outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(m.score) << "\r" << endl;
            if (d == 0)
                movesToState.push_back(m.binNum);
            return m;
        } //end of else if player 1 max
    } //end of else
}

Move minmax3(boardprops& game, int board[], int turn, int d, int maxD, int minOrMax, ofstream& outfile, Move& prevm) {

    Move mTemp, m, m1;
    //mTemp.score = -555;
    int board2[(2 * game.pits) + 2];
    bool check;
    bool doesntmatter;
    check = checkboard(game, board);
    if (d == maxD) {
        flag = true;
        m.score = evalFunction2(game, board, minOrMax);
        m.binNum = -1; //doesnt matter
        retEval = true;
   //     cout << endl << "eval called at d " << d << " with eval value " << m.score << endl;
        return m;
    } else {
        if (minOrMax == MIN && turn == 1) { // MINIMizing player player 2 min
            m.score = INT_MAX;
            m.binNum = -1;
            for (int i = 2 * game.pits; i >= game.pits + 1; i--) {
                if (board[i] != 0) {

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                    int tempBin = prevm.binNum;
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move
                    check = checkboard(game, board);
                    if (check) {

                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;

                    }

               //     cout << endl << "board when player2 min and move " << i << endl;
                //    printboard2(game, board);
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                        //  cout << endl << "player 2 min gets another chance for " << i << endl;
                        mTemp = minmax3(game, board, turn, d, maxD, MIN, outfile, prevm);
                        mTempNotCalled = false;
                    }

                    if (mTempNotCalled && !check)
                        mTemp = minmax3(game, board, (turn + 1) % 2, d + 1, maxD, MAX, outfile, prevm); //recursive call minmax with m.score(as beta) and alpha
                    prevm.binNum = tempBin;

                    if (retEval) {

                        prevm.binNum = tempBin;
                        retEval = false;
                    }
                    for (int k = 0; k < (2 * game.pits) + 2; k++) {
                        board[k] = board2[k];
                    }
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;


                    if (mTemp.score < m.score) { //

                        m.score = mTemp.score;
                        m.binNum = i;

                    }



                    mTempNotCalled = true;




                }
            }

            return m;
        }
        if (minOrMax == MIN && turn == 0) { // MINIMizing player player 1 min
            m.score = INT_MAX;
            m.binNum = -1;
            for (int i = 0; i <= game.pits - 1; i++) {
                if (board[i] != 0) {

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                    int tempBin = prevm.binNum;
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move
                    check = checkboard(game, board);
                    if (check) {
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                    }
                //    cout << endl << "board when player1 min and move " << i << endl;
                //    printboard2(game, board);
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                        mTemp = minmax3(game, board, turn, d, maxD, MIN, outfile, prevm);
                        mTempNotCalled = false;
                    }


                    if (mTempNotCalled && !check)
                        mTemp = minmax3(game, board, (turn + 1) % 2, d + 1, maxD, MAX, outfile, prevm); //recursive call minmax with m.score(as beta) and alpha
                    prevm.binNum = tempBin;
                    if (retEval) {
                        prevm.binNum = tempBin;
                        retEval = false;
                    }

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board[j] = board2[j];
                    }

                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;




                    if (mTemp.score < m.score) {

                        m.score = mTemp.score;
                        m.binNum = i;


                    }

                    mTempNotCalled = true;





                }
            }

            return m;
        } else if (minOrMax == MAX && turn == 1) //comp player 2 max
        {

            m.score = INT_MIN;
            m.binNum = -1;
            for (int i = (2 * game.pits); i > game.pits; i--) {
                if (board[i] != 0) {
                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                    int tempBin = prevm.binNum;
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;


                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move
                    check = checkboard(game, board);
                    if (check) {
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                    }
                //    printboard2(game, board);
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                        //   cout << endl << "player 2 max gets another chance at "<<i<< endl;
                        mTemp = minmax3(game, board, turn, d, maxD, MAX, outfile, prevm);
                        mTempNotCalled = false;
                    }

                    if (mTempNotCalled && !check)
                        mTemp = minmax3(game, board, (turn + 1) % 2, d + 1, maxD, MIN, outfile, prevm); //recursive call minmax with m.score(as alpha) and beta
                    prevm.binNum = tempBin;
                    if (retEval) {
                        prevm.binNum = tempBin;
                        retEval = false;
                    }

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board[j] = board2[j];
                    }

                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;



                    if (mTemp.score > m.score) {

                        m.score = mTemp.score;
                        m.binNum = i;


                    }


                    mTempNotCalled = true;





                }
            }

            if (d == 0)
                movesToState.push_back(m.binNum);
            return m;
        }//end of else if player 2 max
        else if (minOrMax == MAX && turn == 0) // player 1 max
        {
            m.score = INT_MIN;
            m.binNum = -1;

            for (int i = 0; i <= game.pits - 1; i++) {
                if (board[i] != 0) {
                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                //    if (lineno > 30)
               //         cout << "asasc";
                    int tempBin = prevm.binNum;
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move

                    check = checkboard(game, board);
                    if (check) {

                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                    }

                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {

                        mTemp = minmax3(game, board, turn, d, maxD, MAX, outfile, prevm);
                        mTempNotCalled = false;
                    }


                    //if mtemp is set, then don't call.
                    if (mTempNotCalled && !check)
                        mTemp = minmax3(game, board, (turn + 1) % 2, d + 1, maxD, MIN, outfile, prevm); //recursive call minmax with m.score(as alpha) and beta
                    prevm.binNum = tempBin;
                    if (retEval) {
                        prevm.binNum = tempBin;
                        retEval = false;
                    }
                    for (int k = 0; k < (2 * game.pits) + 2; k++) {
                        board[k] = board2[k];
                    }
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;




                    if (mTemp.score > m.score) {

                        m.score = mTemp.score;
                        m.binNum = i;

                    }


                    mTempNotCalled = true;

                }
            }
         //   cout << lineno++ << endl;
            if (d == 0)
                movesToState.push_back(m.binNum);
            return m;
        } //end of else if player 1 max
    }
}

Move minmax(boardprops& game, int board[], int turn, int d, int maxD, int minOrMax, int alpha, int beta, ofstream& outfile, Move& prevm) {

    Move mTemp, m, m1;

    int board2[(2 * game.pits) + 2];
    bool check;
    bool doesntmatter;
    check = checkboard(game, board);
    if (d == maxD) {
        flag = true;


        m.score = evalFunction2(game, board, minOrMax);
        m.binNum = -1;
        retEval = true;
        return m;
    } else {
        if (minOrMax == MIN && turn == 1) { // MINIMizing player player 2 min
            m.score = INT_MAX;
            m.binNum = -1;
            int tempscore = m.score;
            for (int i = 2 * game.pits; i >= game.pits + 1; i--) {
                if (board[i] != 0) {
                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }

                    int tempBin = prevm.binNum;
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                //    cout << lineno++ << endl;
                    if (printcount == 0)
                        outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(tempscore) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;

                    doesntmatter = takeMove(game, board, i, turn);
                    check = checkboard(game, board);
                    if (check) {
                        if ((d + 1) != maxD && !repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                        }
                        if ((d + 1) != maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                        }
                        if ((d + 1) == maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                        }
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                   //     cout << lineno++ << endl;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                    }

                //    cout << endl << "board when player2 min and move " << i << endl;
                 //   printboard2(game, board);
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                     //   cout << endl << "player 2 min gets another chance for " << i << endl;
                        mTemp = minmax(game, board, turn, d, maxD, MIN, alpha, beta, outfile, prevm);
                        mTempNotCalled = false;
                    }

                    if (mTempNotCalled && !check)
                        mTemp = minmax(game, board, (turn + 1) % 2, d + 1, maxD, MAX, alpha, beta, outfile, prevm);
                    prevm.binNum = tempBin;

                    if (retEval) {
                    //    cout << lineno++ << endl;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                        prevm.binNum = tempBin;
                        retEval = false;
                    }
                    for (int k = 0; k < (2 * game.pits) + 2; k++) {
                        board[k] = board2[k];
                    }
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                    if (mTemp.score < tempscore) {
                        tempscore = mTemp.score;
                        m.score = tempscore;
                        m.binNum = i;
                    }

                    if (mTemp.score <= alpha) { //if m.score is less than alpha
                    //    cout << endl << "prune the rest of the children" << endl;

                        m.score = mTemp.score;
                        break;
                    }
                    if (mTemp.score < beta) { //

                        m.score = mTemp.score;
                        m.binNum = i; //bin number will be current index
                        beta = m.score;

                    }



                    mTempNotCalled = true;




                }
            }
         //   cout << lineno++ << endl;
            if (printcount == 0)
                outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(tempscore) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
            return m;
        }
        if (minOrMax == MIN && turn == 0) { // MINIMizing player player 1 min
            m.score = INT_MAX;
            m.binNum = -1;
            int tempscore = m.score;
            for (int i = 0; i <= game.pits - 1; i++) {
                if (board[i] != 0) {

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                    int tempBin = prevm.binNum;
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                    if (printcount == 0) {
               //         cout << lineno++ << endl;
                        outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(tempscore) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                    }

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move
                    check = checkboard(game, board);
                    if (check) {
                        if ((d + 1) != maxD && !repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
             //               cout << lineno++ << endl;
                        }
                        if ((d + 1) != maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
              //              cout << lineno++ << endl;
                        }
                        if ((d + 1) == maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
               //             cout << lineno++ << endl;
                        }
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
               //         cout << lineno++ << endl;
                    }
                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                        mTemp = minmax(game, board, turn, d, maxD, MIN, alpha, beta, outfile, prevm);
                        mTempNotCalled = false;
                    }


                    if (mTempNotCalled && !check)
                        mTemp = minmax(game, board, (turn + 1) % 2, d + 1, maxD, MAX, alpha, beta, outfile, prevm); //recursive call minmax with m.score(as beta) and alpha
                    prevm.binNum = tempBin;
                    if (retEval) {
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
            //            cout << lineno++ << endl;
                        prevm.binNum = tempBin;
                        retEval = false;
                    }

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board[j] = board2[j];
                    }

                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;


                    if (mTemp.score < tempscore) {
                        tempscore = mTemp.score;
                        m.score = tempscore;
                        m.binNum = i;
                    }
                    if (mTemp.score <= alpha) { //if m.score is less than alpha

                        m.score = mTemp.score;
                        break; //so break out of for loop
                    }

                    if (mTemp.score < beta) { //
#ifdef PRINT
                        cout << "m.score, which is " << m.score << " is greater than mTemp.score, which is " << mTemp.score << endl;
                        cout << "Setting a new BIN number to " << i << endl;
#endif
                        m.score = mTemp.score; //if the found score it less than the kept score, so update score kept.
                        m.binNum = i; //bin number will be current index

                        beta = m.score;
                    }

                    mTempNotCalled = true;





                }
            }
            if (printcount == 0)
                outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(tempscore) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
      //      cout << lineno++ << endl;
            return m;
        } else if (minOrMax == MAX && turn == 1) //player 2 max
        {
            m.score = INT_MIN;
            m.binNum = -1;
            int tempscore = m.score;
            for (int i = (2 * game.pits); i > game.pits; i--) {
                if (board[i] != 0) {
                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }
                    int tempBin = prevm.binNum;
                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;


                    if (printcount == 0)
                        outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(tempscore) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;

           //         cout << lineno++ << endl;
                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move
                    check = checkboard(game, board);
          //          if (lineno >= 214) {
         //               cout << "check";
                        
           //         }
                    if (check) {
                        if ((d + 1) != maxD && !repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
         //                   cout << lineno++ << endl;
                        }
                        if ((d + 1) != maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
          //                  cout << lineno++ << endl;
                        }
                        if ((d + 1) == maxD && repeatchance) {
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
            //                cout << lineno++ << endl;
                        }
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
            //            cout << lineno++ << endl;
                    }


                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
                        mTemp = minmax(game, board, turn, d, maxD, MAX, alpha, beta, outfile, prevm);
                        mTempNotCalled = false;
                    }

                    if (mTempNotCalled && !check)
                        mTemp = minmax(game, board, (turn + 1) % 2, d + 1, maxD, MIN, alpha, beta, outfile, prevm); //recursive call minmax with m.score(as alpha) and beta
                    prevm.binNum = tempBin;
                    if (retEval) {
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
            //            cout << lineno++ << endl;
                        prevm.binNum = tempBin;
                        retEval = false;
                    }

                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board[j] = board2[j];
                    }

                    if (i <= 2 * game.pits && i >= game.pits + 1 && (tempBin < game.pits || tempBin > 2 * game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;

                    if (mTemp.score > tempscore) {
                        tempscore = mTemp.score;
                        m.score = tempscore;
                        m.binNum = i;
                    }
                    if (mTemp.score >= beta) {

                        m.score = mTemp.score;

                        break; //so break out of for loop
                    }

                    if (mTemp.score > alpha) {
#ifdef PRINT
                        cout << "m.score, which is " << m.score << " is less than or equal to mTemp.score, which is " << mTemp.score << endl;
                        cout << "Setting a new BIN number to " << i << endl;
#endif
                        m.score = mTemp.score; //if the found score it greater than the kept score, so update score kept.
                        m.binNum = i; //bin number will be current index

                        alpha = m.score;
                    }


                    mTempNotCalled = true;





                }
            }

            if (printcount == 0)
                outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(tempscore) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
      //      cout << lineno++ << endl;
            if (d == 0)
                movesToState.push_back(m.binNum);
            return m;
        }//end of else if player 2 max
        else if (minOrMax == MAX && turn == 0) // player 1 max
        {
            m.score = INT_MIN;
            m.binNum = -1;
            int tempscore = m.score;
            for (int i = 0; i <= game.pits - 1; i++) {
                if (board[i] != 0) {
                    for (int j = 0; j < (2 * game.pits) + 2; j++) {
                        board2[j] = board[j];
                    }

                    int tempBin = prevm.binNum;
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;
            //        if (lineno > 26)
            //            cout << "dscd";
            //        cout << lineno++ << endl;
                    if (printcount == 0)
                        outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(tempscore) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;

                    doesntmatter = takeMove(game, board, i, turn); //change board state to next tentative move

                    check = checkboard(game, board);
                    if (check) {
                        if ((d + 1) != maxD && !repeatchance) {
             //               cout << lineno++ << endl;
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MAX) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                        }
                        if ((d + 1) != maxD && repeatchance) {
             //               cout << lineno++ << endl;
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                        }
                        if ((d + 1) == maxD && repeatchance) {
             //               cout << lineno++ << endl;
                            if (printcount == 0)
                                outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(INT_MIN) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                        }
                        mTemp.score = evalFunction2(game, board, minOrMax);
                        mTemp.binNum = i;
              //          cout << lineno++ << endl;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                    }


                    tempBin = prevm.binNum;
                    prevm.binNum = i;
                    if (repeatchance && !check) {
              //          cout << endl << "player 1 max gets another chance at " << i << endl;
                        mTemp = minmax(game, board, turn, d, maxD, MAX, alpha, beta, outfile, prevm);
                        mTempNotCalled = false;
                    }


                    //if mtemp is set, then don't call.
                    if (mTempNotCalled && !check)
                        mTemp = minmax(game, board, (turn + 1) % 2, d + 1, maxD, MIN, alpha, beta, outfile, prevm); //recursive call minmax with m.score(as alpha) and beta
                    prevm.binNum = tempBin;
                    if (retEval) {
              //          cout << lineno++ << endl;
                        if (printcount == 0)
                            outfile << intToNode(game, i) << "," << d + 1 << "," << returnstring(mTemp.score) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
                        prevm.binNum = tempBin;
                        retEval = false;
                    }
                    for (int k = 0; k < (2 * game.pits) + 2; k++) {
                        board[k] = board2[k];
                    }
                    if (i >= 0 && i <= game.pits && (tempBin < 0 || tempBin > game.pits))
                        prevNodeDepth = d;
                    else
                        prevNodeDepth = d + 1;


                    if (mTemp.score > tempscore) {
                        tempscore = mTemp.score;
                        m.score = tempscore;
                        m.binNum = i;
                    }

                    if (mTemp.score >= beta) {
               //         cout << endl << "prune the rest of the children" << endl;

                        m.score = mTemp.score;
                        break; //so break out of for loop
                    }

                    if (mTemp.score > alpha) {
#ifdef PRINT
                        cout << "m.score, which is " << m.score << " is less than or equal to mTemp.score, which is " << mTemp.score << endl;
                        cout << "Setting a new BIN number to " << i << endl;
#endif
                        m.score = mTemp.score; //if the found score it greater than the kept score, so update score kept.
                        m.binNum = i; //bin number will be current index

                        alpha = m.score;

                    }

                    mTempNotCalled = true;

                }
            }
      //      cout << lineno++ << endl;
            if (printcount == 0)
                outfile << intToNode(game, prevm.binNum) << "," << prevNodeDepth << "," << returnstring(tempscore) << "," << returnstring(alpha) << "," << returnstring(beta) << "\r" << endl;
            if (d == 0)
                movesToState.push_back(m.binNum);
            return m;
        } //end of else if player 1 max
    } //end of else
}

int computerChooseMove(boardprops& game, int board[], int maxD, int turn, ofstream &outfile) {
    Move m;
    int alpha = INT_MIN; // alpha = -infinity, for initial call
    int beta = INT_MAX; // beta = infinity, for initial call
    Move tempMove;
    tempMove.score = -9998;
    tempMove.binNum = -9998;

    if (game.algo == 1) {
        if(printcount==0)
        outfile << "Node,Depth,Value" << "\r" << endl;
        lineno++;
     //   cout << lineno++ << endl;
        game.cod = 1;
        maxD = 1;
        m = minmax3(game, board, turn, 0, maxD, MAX, outfile, tempMove);
    }
    if (game.algo == 2) {
        if(printcount==0)
        outfile << "Node,Depth,Value" << "\r" << endl;
        lineno++;
     //   cout << lineno++ << endl;
        m = minmax2(game, board, turn, 0, maxD, MAX, outfile, tempMove);
    } else if (game.algo == 3) {
        if(printcount==0)
        outfile << "Node,Depth,Value,Alpha,Beta" << "\r" << endl;
        lineno++;
    //    cout << lineno << endl;
        m = minmax(game, board, turn, 0, maxD, MAX, alpha, beta, outfile, tempMove); //initial call
    }
    
    return m.binNum; //found the value of best bin
}

int evalFunction(int board[], int minOrMax) { //minORMax arent really needed since we are using evalfunction to benefit comp
    /*int board2[14]; //for getting value lets copy board so it isn't messed up.
    bool checkempty; //check if it results in an empty bin location, if so then add the value of that.
    for (int j = 0; j < 14; j++) {
        board2[j] = board[j]; //copy board 2
    }
    //get all the pieces on the human side, including mancala
    float humanside = (board[0] + board[1] + board[2] + board[3] + board[4] + board[5])*0.45; //multiply by a constant
    //I am multiplying by 0.45 because it gives more variation.
    //this is also why floating point numbers are used, later converted to an int
    float humanmancala = (board[6]) * 3.45; //multiple by a higher constant because these are guaranteed pieces, since in mancala already
    float human = humanside + humanmancala; // find total for human pieces
    //get all the pieces on the comp side, including mancala
    float compside = (board[7] + board[8] + board[9] + board[10] + board[11] + board[12])*0.45; //multiply by a constant
    //I am multiplying by 0.45 because it gives more variation.
    //this is also why floating point numbers are used, later converted to an int
    float compmancala = (board[13]) * 3.45; //multiple by a higher constant because these are guaranteed pieces, since in mancala already
    float comp = compside + compmancala; //find total for comp pieces
    float difference_board = comp - human; //find the difference between the two, negative doesn't make a difference.
    int valueofboard = int(difference_board + 0.5); //this basically rounds the floating point number to the nearest integer.
    for (int i = 0; i <= 5; i++) { //for each possible move for human
        checkempty = takeMove(board2, i, minOrMax); //see if you can land in an empty bin
        if (checkempty) { //if you can land in an empty bin
            int acrosslocation = findacross(i); //find the location of that
            int acrosspieces = board2[acrosslocation]; //find how many pieces are across
            //add the number of pieces across plus the one on your side to the vale of the board
            valueofboard = valueofboard + ((acrosspieces * 0.5)*-1); //since we want the comp to not have to go to this state, we multiple by -1, adding negative
        }
    }
    for (int i = 7; i <= 12; i++) { //for each possible move for comp
        checkempty = takeMove(board2, i, minOrMax); //see if you can land in an empty bin
        if (checkempty) {//if you can land in an empty bin
            int acrosslocation = findacross(i); //find the location of that
            int acrosspieces = board2[acrosslocation]; //find how many pieces are across
            //add the number of pieces across plus the one on your side to the vale of the board
            valueofboard = valueofboard + ((acrosspieces * 0.5)); //we want comp to go to this state, so multiply by constant only
        }
    }
    return valueofboard; //regardless of if you can land in an empty bin, give back the value of the board.
     */
    return 0;
}

int evalFunction2(boardprops& game, int board[], int minOrMax) {
    if (game.myplayer == 2)
        return board[(2 * game.pits) + 1] - board[game.pits];
    else
        return board[game.pits] - board[(2 * game.pits) + 1];
}

bool checkboard(boardprops& game, int board[]) {
    int bottomnumempty = 0; //see if bottom are empty
    for (int i = 0; i < game.pits; i++) {
        if (board[i] == 0) {
            bottomnumempty++; //check each one individual to see if 0
        }
    }
    if (bottomnumempty == game.pits) {
        // if bottom are empty, then take all on comp side and deposit into comp mancala
        //board[13] = board[13] + board[12] + board[11] + board[10] + board[9] + board[8] + board[7];
        for (int i = game.pits + 1; i < (2 * game.pits) + 1; i++) {
            board[(2 * game.pits) + 1] = board[(2 * game.pits) + 1] + board[i]; //empty all on comp side
            board[i] = 0;
        }
        return true; //yes, its empty
    }
    int topnumempty = 0;
    for (int i = game.pits + 1; i < (2 * game.pits) + 1; i++) {//see if top are empty
        if (board[i] == 0) {
            topnumempty++; //check each one individual to see if 0
        }
    }
    if (topnumempty == game.pits) {
        // if top are empty, then take all on human side and deposit into human mancala
        //board[6] = board[6] + board[5] + board[4] + board[3] + board[2] + board[1] + board[0];
        for (int i = 0; i < game.pits; i++) {
            board[game.pits] = board[game.pits] + board[i]; //empty all on comp side
            board[i] = 0;
        }
        return true; //yes, its empty
    }
    return false; //no, its not empty, top 6 or bottom 6
}

bool checkforone(boardprops& game, int board[], int turn) {
    if (turn == 1) {
        int check = 0;
        for (int i = game.pits + 1; i <= (2 * game.pits); i++) {
            if (board[i] > 0) {
                check++; //for each possible move add one
            }
        }
        if (check == 1) { //if there is only one move possible.
            return true; //return true
        } else {
            return false; //else return false.
        }
    }
    if (turn == 0) {
        int check = 0;
        for (int i = 0; i < game.pits; i++) {
            if (board[i] > 0) {
                check++; //for each possible move add one
            }
        }
        if (check == 1) { //if there is only one move possible.
            return true; //return true
        } else {
            return false; //else return false.
        }
    }
}

int findone(boardprops& game, int board[], int turn) {
    if (turn == 1) {
        int check = 0;
        for (int i = game.pits + 1; i <= (2 * game.pits); i++) { //go through all comp bins
            if (board[i] > 0) {
                check = i; //find the bin location of the one possible move for comp
                return check;
            }
        }
        return check;
    }
    if (turn == 0) {
        int check = 0;
        for (int i = 0; i < game.pits; i++) { //go through all comp bins
            if (board[i] > 0) {
                check = i; //find the bin location of the one possible move for comp
                return check;
            }
        }
        return check;
    }
}

int findacross(boardprops& game, int ending, int turn) { //find across bin
    int across = -555;
    if (ending == game.pits || ending == (2 * game.pits) + 1) {
        across = -555;
    } else {
        across = (2 * game.pits) - ending;
    }

    return across;
}

int askforselection(int board[]) {
    int selection;
    cout << "What move would you like to make? Your mancala is to the right." << endl;
    cout << "Please enter either 0,1,2,3,4 or 5." << endl;
    cin >> selection;
    while (selection > 5 || selection < 0 || board[selection] == 0) { //make sure valid selection
        cout << "Wrong selection. Pit is either empty or out of range" << endl;
        cout << "What move would you like to make?" << endl;
        cout << "Please enter the slot number " << endl;
        cin >> selection;
    }
    return selection;
}

void printboard(int board[]) {

    cout << "       |  12  |  11  |  10  |   9  |   8  |   7  |" << endl;
    cout << "________________________________________________________" << endl;
    cout << "|" << "      | (" << std::setfill('0') << std::setw(2) << board[12] << ") | ";
    cout << "(" << std::setfill('0') << std::setw(2) << board[11];
    cout << ") | " << "(" << std::setfill('0') << std::setw(2) << board[10] << ") | ";
    cout << "(" << std::setfill('0') << std::setw(2) << board[9] << ") | ";
    cout << "(" << std::setfill('0') << std::setw(2) << board[8] << ") | ";
    cout << "(" << std::setfill('0') << std::setw(2) << board[7] << ") |      |" << endl;

    cout << "|      |‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾|      |" << endl;

    cout << "|  " << std::setfill('0') << std::setw(2) << board[13] << "  |";
    cout << "                                         ";
    cout << "|  " << std::setfill('0') << std::setw(2) << board[6] << "  |" << endl;
    cout << "|      |_________________________________________|      |" << endl;

    cout << "|" << "      | (" << std::setfill('0') << std::setw(2) << board[0] << ") | ";
    cout << "(" << std::setfill('0') << std::setw(2) << board[1];
    cout << ") | " << "(" << std::setfill('0') << std::setw(2) << board[2] << ") | ";
    cout << "(" << std::setfill('0') << std::setw(2) << board[3] << ") | ";
    cout << "(" << std::setfill('0') << std::setw(2) << board[4] << ") | ";
    cout << "(" << std::setfill('0') << std::setw(2) << board[5] << ") |	|" << endl;

    cout << "‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾" << endl;
    cout << "       |   0  |   1  |   2  |   3  |   4  |   5  |" << endl;
    cout << "\n";
}

void printboard2(boardprops& game, int board[]) {
    cout << endl;
    cout << board[(2 * game.pits) + 1] << "   ";
    for (int i = (2 * game.pits); i > game.pits; i--) {
        cout << " " << board[i];
    }
    cout << endl << "   ";
    for (int i = 0; i < game.pits; i++) {
        cout << " " << board[i];
    }
    cout << "    " << board[game.pits];
    cout << endl;
}

void writeStateToFile(boardprops& game, int board[], ofstream& outfile2) {
    for (int i = (2 * game.pits); i > game.pits + 1; i--) {
        outfile2 << board[i] << " ";
    }
    outfile2 << board[game.pits + 1] << "\r" << endl;
    for (int i = 0; i < game.pits - 1; i++) {
        outfile2 << board[i] << " ";
    }
    outfile2 << board[game.pits - 1] << "\r" << endl;

    outfile2 << board[(2 * game.pits) + 1] << "\r" << endl;
    outfile2 << board[game.pits] << "\r" << endl;
}



