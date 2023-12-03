#include <iostream>
#include <vector>
#include <stack> 
#include <limits.h>
#include <chrono>
#include <string.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))

using namespace std::chrono;
using namespace std;

const int EASY_MODE = 2;
const int MEDIUM_MODE = 6;
const int HARD_MODE = 8;

const int EMPTY_TOKEN = 0;
const int PLAYER1_TOKEN = 1;
const int IA_TOKEN = 2;

char EMPTY_SKIN = '-';
char PLAYER1_SKIN = 'X';
char IA_SKIN = 'Y';

const int TOTAL_ROW = 6;
const int TOTAL_COLUMN = 7;

class GameBoard {
private:
    char getSkinToken(int valueToken) {
        if(valueToken == PLAYER1_TOKEN) {
            return PLAYER1_SKIN;
        }

        if(valueToken == IA_TOKEN) {
            return IA_SKIN;
        }

        return EMPTY_SKIN;
    }
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

                cout << this->getSkinToken(this->board[i][j]) << " ";
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

class NodeMiniMax{
private:
    GameBoard currentGameBoard;
    int currentDepth;
    bool isMaximizing;
    int currentToken;

public:
    NodeMiniMax(GameBoard gameBoard, int depth, bool maximizingPlayer, int token) {
        this->currentGameBoard = gameBoard;
        this->currentDepth = depth;
        this->isMaximizing = maximizingPlayer;
        this->currentToken = token;
    }

    int heuristicScore(int correct, int incorrect, int empties) {
        int score = 0;
    
        if (correct == 4) { score += 501; } 
        else if (correct == 3 && empties == 1) { score += 50; }
        else if (correct == 2 && empties == 2) { score += 5; }
        else if (incorrect == 2 && empties == 2) { score -= 6; } 
        else if (incorrect == 3 && empties == 1) { score -= 51; }
        else if (incorrect == 4) { score -= 500; }
    
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

    int execute(bool useAlphaBetaPruning, int alpha, int beta) {
        if(this->currentDepth == 0 || this->currentGameBoard.isFull()) {
            int score = this->evaluateBoard();
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

                NodeMiniMax newChild(copyGameboard, this->currentDepth -1, false, PLAYER1_TOKEN);
                int currentScore = newChild.execute(useAlphaBetaPruning, alpha, beta);
                
                if(currentScore > value) {
                    value = max(value, currentScore);
                    columnOption = i;
                }

                if(!useAlphaBetaPruning) {
                    continue;
                }

                alpha = max(alpha, value);
                
                if(alpha >= beta) {
                    break;
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

                NodeMiniMax newChild(copyGameboard, this->currentDepth -1, true, IA_TOKEN);
                int currentScore = newChild.execute(useAlphaBetaPruning, alpha, beta);
                if(currentScore < value) {
                    value = min(value, currentScore);
                    columnOption = i;
                }

                if(!useAlphaBetaPruning) {
                    continue;
                }

                beta = min(beta, value);
                
                if(alpha >= beta) {
                    break;
                }
            }
            return columnOption;
        }
    }
};

class IAAgent {
private:
    int maxDepth;
public:
    IAAgent(int depth) {
        this->maxDepth = depth;
    }

    int getBestOption(GameBoard gameBoard, int maxDepth, bool useAlphaBetaPruning) {
        cout << "THE IA IS COMPUTING THE BEST OPTION" << endl;
        NodeMiniMax rootNode(gameBoard, maxDepth, true, IA_TOKEN);
        auto start = high_resolution_clock::now();
        int option = rootNode.execute(useAlphaBetaPruning, INT_MIN, INT_MAX);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "TOTAL TIME TO CHOOSE AN OPTION: " <<  duration.count() << " ms " << endl;
        return option;
    }
};

string getNameToken(int token) {
    if(token == PLAYER1_TOKEN) {
        return "PLAYER";
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

bool useAlphaBetaPruningInput() {
    string optionUser;
    cout<<"Do you want to use alpha beta pruning: (yes or no)"<<endl;
    cin >> optionUser;


    while(optionUser != "yes" && optionUser != "no") {
        cout<<"[Retry] Do you want to use alpha beta pruning: (yes or no)"<<endl;
        cin >> optionUser; 
    }

    return optionUser == "yes";
}

int selectGameDifficult() {
    int difficultOption;
    cout<<"Please select the difficult to play: (0 = EASY, 1 = MEDIUM and 2 = HARD)"<<endl;
    cin >> difficultOption;

    while(difficultOption < 0 || difficultOption >= 3) {
        cout << "The previous value is not valid" << endl;
        cout<<"[RETRY]Please select the difficult to play: (0 = EASY, 1 = MEDIUM and 2 = HARD)"<<endl;
        cin >> difficultOption; 
    }

    if(difficultOption == 0) { 
        cout << "The EASY MODE is active! No trophies available" << endl;
        return EASY_MODE; 
    }

    if(difficultOption == 1) { 
        cout << "The MEDIUM MODE is active!" << endl;
        return MEDIUM_MODE; 
    }
    
    cout << "The HARD MODE is active!" << endl;
    return HARD_MODE;
}

void changePlayersSkins() {
    int changeSkinOption;
    cout<<"Do you want to change the skin for the player: (1 = YES, 0 = NO)"<<endl;
    cin >> changeSkinOption;

    while(changeSkinOption < 0 || changeSkinOption > 1) {
        cout << "The previous value is not valid" << endl;
        cout<<"[RETRY]Do you want to change the skin for the player: (1 = YES, 0 = NO)"<<endl;
        cin >> changeSkinOption;
    }

    if(changeSkinOption == 0) {
        return;
    }

    char newPlayer1Skin;
    cout<<"Please enter the new skin for the player: (just one char please, like A , C or M)"<<endl;
    cin >> newPlayer1Skin;

    while(newPlayer1Skin == EMPTY_SKIN || newPlayer1Skin == IA_SKIN) {
        cout << "The option " << EMPTY_SKIN << " and " << IA_SKIN  << " are reserved tokens, please select type other option" << endl;
        cout<<"[RETRY]Please enter the new skin for the player: (just one char please, like A , C or M)"<<endl;
        cin >> newPlayer1Skin;
    }

    PLAYER1_SKIN = newPlayer1Skin;

    cout << "The skin was changed correctly" << endl;
    return;
}

int main() {
    GameBoard gameBoard;
    IAAgent miniMaxAgent(EASY_MODE);

    int columnSelected = 0;
    int currentPlayer = PLAYER1_TOKEN;

    changePlayersSkins();

    bool useAlphaBetaPruning = useAlphaBetaPruningInput();

    if(useAlphaBetaPruning) {
        cout << "ALPHA BETA PRUNING ACTIVED!" << endl;
    }

    int difficultSelected = selectGameDifficult();

    gameBoard.print();

    while (true)
    {
        if(currentPlayer == PLAYER1_TOKEN) {
            columnSelected = userInput(gameBoard);
        } else {
            columnSelected = miniMaxAgent.getBestOption(gameBoard, difficultSelected, useAlphaBetaPruning);
        }

        gameBoard.placePiece(currentPlayer, columnSelected);

        gameBoard.print();

        if(gameBoard.checkWinning(currentPlayer)) {
            cout << getNameToken(currentPlayer) << " WINS" << endl;
            break;
        }

        currentPlayer = currentPlayer == PLAYER1_TOKEN ? IA_TOKEN : PLAYER1_TOKEN;
    }

    cout << "APP ENDED!" << endl;

    return 0;
}
