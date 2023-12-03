#include <iostream>
#include <vector>
#include <stack> 
#include <limits.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

using namespace std;

const int EASY_MODE = 2;
const int MEDIUM_MODE = 6;
const int HARD_MODE = 10;

const int EMPTY_TOKEN = 0;
const int PLAYER_TOKEN = 1;
const int IA_TOKEN = 2;

const int TOTAL_ROW = 6;
const int TOTAL_COLUMN = 7;

class GameBoard {
public:
    int board[TOTAL_ROW][TOTAL_COLUMN];
    GameBoard() {
        for (int i = 0; i < TOTAL_ROW; i++) {
            for (int j = 0; j < TOTAL_COLUMN; j++) {
                this->board[i][j] = 0;
            }
        }
    }
    void print() {
        for (int i = 0; i < TOTAL_ROW; i++) {
            for (int j = 0; j < TOTAL_COLUMN; j++) {
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool isFull() {
        for (int i = 0; i < TOTAL_ROW; i++)
        {
            for (int j = 0; j < TOTAL_COLUMN; j++)
            {
                if(this->board[i][j] == EMPTY_TOKEN) {
                    return false;
                }
            }
        }
        return true;        
    }

    bool validColumnToPlacePiece(int column) {
        if(column >= TOTAL_COLUMN || column < 0) {
            return false;
        }

        for (int i = 0; i < TOTAL_ROW; i++)
        {   
            if(this->board[i][column] == 0) {
                return true;
            }
        }
        return false;
    }

    void placePiece(int token, int column) {
        
        for (int i = TOTAL_ROW - 1; i >= 0; i--)
        {
            if (this->board[i][column] == 0)
            {
                this->board[i][column] = token;
                break;
            }
        }
    }

    bool checkWinning(int token) {
        int winSequence = 0; // to count adjacent pieces
        // for horizontal checks
        for ( int c = 0; c < TOTAL_COLUMN - 3; c++) { // for each column
            for ( int r = 0; r < TOTAL_ROW; r++) { // each row
                for (int i = 0; i < 4; i++) { // recall you need 4 to win
                    if (( int)board[r][c + i] == token) { // if not all pieces match
                        winSequence++; // add sequence count
                    }
                    if (winSequence == 4) { return true; } // if 4 in row
                }
                winSequence = 0; // reset counter
            }
        }
        // vertical checks
        for ( int c = 0; c < TOTAL_COLUMN; c++) {
            for ( int r = 0; r < TOTAL_ROW - 3; r++) {
                for (int i = 0; i < 4; i++) {
                    if (( int)board[r + i][c] == token) {
                        winSequence++;
                    }
                    if (winSequence == 4) { return true; }
                }
                winSequence = 0;
            }
        }
        // the below two are diagonal checks
        for ( int c = 0; c < TOTAL_COLUMN - 3; c++) {
            for ( int r = 3; r < TOTAL_ROW; r++) {
                for (int i = 0; i < 4; i++) {
                    if (( int)board[r - i][c + i] == token) {
                        winSequence++;
                    }
                    if (winSequence == 4) { return true; }
                }
                winSequence = 0;
            }
        }
        for ( int c = 0; c < TOTAL_COLUMN - 3; c++) {
            for ( int r = 0; r < TOTAL_ROW - 3; r++) {
                for (int i = 0; i < 4; i++) {
                    if (( int)board[r + i][c + i] == token) {
                        winSequence++;
                    }
                    if (winSequence == 4) { return true; }
                }
                winSequence = 0;
            }
        }
        return false;
    };
    
};

class Node{
private:
    GameBoard currentGameBoard;
    int currentDepth;
    bool isMaximizing;
    int currentToken;

public:
    Node(GameBoard gameBoard, int depth, bool maximizingPlayer, int token) {
        this->currentGameBoard = gameBoard;
        this->currentDepth = depth;
        this->isMaximizing = maximizingPlayer;
        this->currentToken = token;
    }

    int heuristicScore(int correct, int incorrect, int empties) {
        int score = 0;
    
        if (correct == 4) { score += 500001; } 
        else if (correct == 3 && empties == 1) { score += 5000; }
        else if (correct == 2 && empties == 2) { score += 500; }
        else if (incorrect == 2 && empties == 2) { score -= 501; } 
        else if (incorrect == 3 && empties == 1) { score -= 5001; }
        else if (incorrect == 4) { score -= 500000; }
    
        return score;
    }

    int optionScore(vector<int> set) {
        int totalEmpties = 0;
        int totalCorrects = 0;
        int totalIncorrects = 0;

        for (int i = 0; i < set.size(); i++)
        {
            if(set[i] == IA_TOKEN) {
                totalCorrects += 1;
            } else if(set[i] == EMPTY_TOKEN) {
                totalEmpties += 1;
            } else {
                totalIncorrects += 1;
            }
        }
        
        return heuristicScore(totalCorrects, totalIncorrects, totalEmpties);
    }

    int evaluateBoard() {
        int score = 0;
        vector<int> rs(TOTAL_COLUMN);
        vector<int> cs(TOTAL_ROW);
        vector<int> set(4);
        // horizontal
        for (int r = 0; r < TOTAL_ROW; r++) {
            for (int c = 0; c < TOTAL_COLUMN; c++) {
                rs[c] = this->currentGameBoard.board[r][c]; // this is a distinct row alone
            }
            for (int c = 0; c < TOTAL_COLUMN - 3; c++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = rs[c + i]; // for each possible "set" of 4 spots from that row
                }
                score += optionScore(set); // find score
            }
        }
        // vertical
        for (int c = 0; c < TOTAL_COLUMN; c++) {
            for (int r = 0; r < TOTAL_ROW; r++) {
                cs[r] = this->currentGameBoard.board[r][c];
            }
            for (int r = 0; r < TOTAL_ROW - 3; r++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = cs[r + i];
                }
                score += optionScore(set);
            }
        }
        // diagonals
        for (int r = 0; r < TOTAL_ROW - 3; r++) {
            for (int c = 0; c < TOTAL_COLUMN; c++) {
                rs[c] = this->currentGameBoard.board[r][c];
            }
            for (int c = 0; c < TOTAL_COLUMN - 3; c++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = this->currentGameBoard.board[r + i][c + i];
                }
                score += optionScore(set);
            }
        }
        for (int r = 0; r < TOTAL_ROW - 3; r++) {
            for (int c = 0; c < TOTAL_COLUMN; c++) {
                rs[c] = this->currentGameBoard.board[r][c];
            }
            for (int c = 0; c < TOTAL_COLUMN - 3; c++) {
                for (int i = 0; i < 4; i++) {
                    set[i] = this->currentGameBoard.board[r + 3 - i][c + i];
                }
                score += optionScore(set);
            }
        }
        return score;
    }

    int execute() {
        if(this->currentDepth == 0 || this->currentGameBoard.isFull()) {
            int score = this->evaluateBoard();
            cout << "DEPTH: " << this->currentDepth << " - SCORE: " << score << " - full: " << this->currentGameBoard.isFull() << endl;
            return score;
        }

        if(this->isMaximizing){
            int columnOption = -1;
            int value = INT_MIN;
            for (int i = 0; i < TOTAL_COLUMN; i++)
            {
                GameBoard copyGameboard = this->currentGameBoard;

                if(!copyGameboard.validColumnToPlacePiece(i)) {
                    continue;
                }

                copyGameboard.placePiece(this->currentToken, i);

                Node newChild(copyGameboard, this->currentDepth -1, false, PLAYER_TOKEN);
                int currentScore = newChild.execute();
                if(currentScore > value) {
                    value = max(value, currentScore);
                    columnOption = i;
                }
            }
            return columnOption;
        } else {
            int columnOption = -1;
            int value = INT_MAX;
            for (int i = 0; i < TOTAL_COLUMN; i++)
            {
                GameBoard copyGameboard = this->currentGameBoard;

                if(!copyGameboard.validColumnToPlacePiece(i)) {
                    continue;
                }

                copyGameboard.placePiece(this->currentToken, i);

                Node newChild(copyGameboard, this->currentDepth -1, true, IA_TOKEN);
                int currentScore = newChild.execute();
                if(currentScore < value) {
                    value = min(value, currentScore);
                    columnOption = i;
                }
            }
            return columnOption;
        }
    }
};

class MiniMaxAgent {
private:
    int maxDepth;
public:
    MiniMaxAgent(int depth) {
        this->maxDepth = depth;
    }

    int getBestOption(GameBoard gameBoard, int maxDepth) {
        cout << "THE IA IS COMPUTING THE BEST OPTION TO KICK UR ASS" << endl;
        Node rootNode(gameBoard, maxDepth, true, IA_TOKEN);
        int option = rootNode.execute();
        cout << "ia option: " << option << endl;
        return option;
    }
};

string getNameToken(int token) {
    if(token == PLAYER_TOKEN) {
        return "USER";
    }
    return "IA";
}

int userInput(GameBoard gameBoard) {
    int colUser;
    cout<<"Please select the column to play: (0 to 6)"<<endl;
    cin >> colUser;

    while(!gameBoard.validColumnToPlacePiece(colUser)) {
        cout << "The previous value is not valid" << endl;
        cout<<"[Retry] Please select the column to play: (0 to 6)"<<endl;
        cin >> colUser; 
    }

    return colUser;
}

int main() {
    GameBoard gameBoard;
    MiniMaxAgent miniMaxAgent(EASY_MODE);

    gameBoard.print();

    int currentPlayer = PLAYER_TOKEN;
    int columnSelected = 0;
    while (true)
    {
        if(currentPlayer == PLAYER_TOKEN) {
            columnSelected = userInput(gameBoard);
        } else {
            columnSelected = miniMaxAgent.getBestOption(gameBoard, MEDIUM_MODE);
        }

        gameBoard.placePiece(currentPlayer, columnSelected);

        gameBoard.print();

        if(gameBoard.checkWinning(currentPlayer)) {
            cout << getNameToken(currentPlayer) << " WINS" << endl;
            break;
        }

        currentPlayer = currentPlayer == PLAYER_TOKEN ? IA_TOKEN : PLAYER_TOKEN;
    }

    cout << "APP ENDED!" << endl;

    return 0;
}
