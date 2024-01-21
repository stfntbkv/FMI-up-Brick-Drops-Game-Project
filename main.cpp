/**
*
* Solution to course project # <номер на вариант>
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2023/2024
*
* @author Stefan Tabakov
* @idnumber 0MI0600334
* @compiler GCC
*
* Code for game Bricks Drop
*
*/

#include<iostream>
#include<vector>
#include<cmath>
#include<ctime>
#include<fstream>


using namespace std;
const int ROW_SIZE = 10;
const int COL_SIZE = 8;
const int MAX_FIGURE_SIZE = 4;
int PlayerPoints = 0;
const string UsersFile = "Users";
vector<string> Users;
vector<int> Scores;


void IntitalizeField(vector<vector<char> > &field) {
    field.resize(ROW_SIZE);
    for (int i = 0; i < ROW_SIZE; i++) {
        field[i].resize(COL_SIZE);
        for (int j = 0; j < COL_SIZE; j++) {
            field[i][j] = ' ';
        }
    }
}

bool IsInField(const vector<vector<char> > &field, int x, int y) {
    return x >= 0 && x < ROW_SIZE && y >= 0 && y < COL_SIZE;
}

bool IsStartOfBlock(const vector<vector<char> > &field, int row, int col) {
    if (col == 0 || field[row][col] != field[row][col - 1]) {
        if (field[row][col] != ' ') {
            return true;
        }
    }
    return false;
}

bool UserInputValidation(const vector<vector<char> > &field, int leftCordinate, int row, char direction, int steps) {
    if (!IsInField(field, row, leftCordinate)) {
        cout << "Invalid Input! The cordinates that you entered are not in the field!" << endl;
        return false;
    }

    if (field[row][leftCordinate] == ' ') {
        cout << "Invalid Input! There is no block to mov on the cordinates that you entered" << endl;
        return false;
    }
    if (!IsStartOfBlock(field, row, leftCordinate)) {
        cout << "Invalid Input! This is not the left cordinate of a block" << endl;
        return false;
    }
    if (direction != 'r' && direction != 'l') {
        cout << "Invalid Input! The direction that you entered is not correct!" << endl;
        return false;

    }
    if (direction == 'r') {
        if (!IsInField(field, row, leftCordinate + steps)) {
            cout << "Invalid Input! The number os steps will take you out of the field!" << endl;
            return false;
        }
    }
    if (direction == 'l') {
        if (!IsInField(field, row, leftCordinate - steps)) {
            cout << "Invalid Input! The number os steps will take you out of the field!" << endl;
            return false;
        }
    }
    return true;
}

bool UserInput(vector<vector<char> > field, int &leftCordinate, int &row, char &direction, int &steps) {
    cout << "Input the left cordinate of the block you want to move: " << endl;
    cin >> leftCordinate;
    cout << "Input row cordinate: " << endl;
    cin >> row;
    cout << "Input the direction choose between /'r/' and /'l/': " << endl;
    cin >> direction;
    cout << "Input the number of steps you want to move the block " << endl;
    cin >> steps;

    return UserInputValidation(field, leftCordinate - 1, row - 1, direction, steps);

}


void OutputField(const vector<vector<char> > &field) {
    cout << '-';
    for (int i = 0; i < COL_SIZE; ++i) {
        cout << '-';
    }
    cout << '-' << endl;
    for (int i = 0; i < ROW_SIZE; ++i) {
        cout << '|';
        for (int j = 0; j < COL_SIZE; ++j) {
            cout << field[i][j];
        }
        cout << '|';
        cout << endl;
    }
    cout << '-';
    for (int i = 0; i < COL_SIZE; ++i) {
        cout << '-';
    }
    cout << '-' << endl;

    cout << "-----------------" << endl;
    cout << "Player points: ";
    cout << PlayerPoints << endl;
    cout << "------------------" << endl;
}

void GenerateRandomRow(vector<vector<char> > &field, int row, int col) {
    if (col >= COL_SIZE) {
        return;
    }

    int maxFigureSize = min(MAX_FIGURE_SIZE, COL_SIZE - col);
    int randomFigureSize = rand() % maxFigureSize + 1;
    int randomChar = 'a' + rand() % 26;
    int randomSpaces = rand() % maxFigureSize;

    for (int i = col; i < col + randomSpaces && i < COL_SIZE; ++i) {
        field[row][i] = ' ';
    }
    col += randomSpaces;

    for (int i = col; i < col + randomFigureSize && i < COL_SIZE; ++i) {
        field[row][i] = randomChar;
    }
    col += randomFigureSize;
    GenerateRandomRow(field, row, col);
}

int GetBlockSize(const vector<vector<char> > &field, int row, int col) {
    char curCh = field[row][col];
    int blockSize = 0;
    while (field[row][col] == curCh && col < COL_SIZE) {
        blockSize++;
        col++;
    }
    return blockSize;
}


void MoveBlockDown(vector<vector<char> > &field, int row, int col, int blockSize) {
    while (row < ROW_SIZE - 1) {
        bool canMove = true;
        for (int i = 0; i < blockSize; i++) {
            if (field[row + 1][col + i] != ' ') {
                canMove = false;
                break;
            }
        }
        if (!canMove) {
            break;
        }
        for (int i = 0; i < blockSize; i++) {
            field[row + 1][col + i] = field[row][col + i];
            field[row][col + i] = ' ';
        }
        row++;
    }
}

void MoveBlocksDown(vector<vector<char> > &field) {
    for (int row = ROW_SIZE - 2; row >= 0; row--) {
        int col = 0;
        while (col < COL_SIZE) {
            if (field[row][col] != ' ') {
                int blockSize = GetBlockSize(field, row, col);
                MoveBlockDown(field, row, col, blockSize);
                col += blockSize;
            } else {
                col++;
            }
        }
    }
}


void MoveBlock(vector<vector<char> > &field, int row, int startCol, char direction, int steps) {
    int blockSize = GetBlockSize(field, row, startCol);
    bool canMove = true;
    for (int i = 0; i < steps; i++) {
        if (direction == 'l' && startCol ==0) {
            canMove = false;
            break;
        }
        if (direction == 'r' && startCol + blockSize == COL_SIZE) {
            canMove = false;
            break;
        }

        if ((direction == 'l' && field[row][startCol - 1] != ' ') ||
            (direction == 'r' && field[row][startCol + blockSize] != ' ')) {
            canMove = false;
            break;
        }

        if (direction == 'l') {
            field[row][startCol - 1] = field[row][startCol];
            field[row][startCol + blockSize - 1] = ' ';
            startCol--;
        }
        if (direction == 'r') {
            field[row][startCol + blockSize] = field[row][startCol + blockSize - 1];
            field[row][startCol] = ' ';
            startCol++;
        }
    }
    if (canMove) {
        MoveBlocksDown(field);
    }
}

void RemoveFullRows(vector<vector<char> > &field) {
    for (int i = 0; i < ROW_SIZE; i++) {
        bool isFull = true;
        for (int j = 0; j < COL_SIZE; j++) {
            if (field[i][j] == ' ') {
                isFull = false;
                break;
            }
        }
        if (isFull) {
            PlayerPoints += 10;
            for (int row = i; row > 0; row--) {
                field[row] = field[row - 1];
            }
            for (int col = 0; col < COL_SIZE; col++) {
                field[0][col] = ' ';
            }
            MoveBlocksDown(field);
            i--;
        }
    }
}

void MoveFieldOneUp(vector<vector<char> > &field) {
    for (int i = 1; i < ROW_SIZE; i++) {
        for (int j = 0; j < COL_SIZE; j++) {
            field[i - 1][j] = field[i][j];
        }
    }
}

bool IsFieldFull(const vector<vector<char> > &field) {
    for (int i = 0; i < ROW_SIZE; i++) {
        bool isCurRowEmpty = true;
        for (int j = 0; j < COL_SIZE; j++) {
            if (field[i][j] != ' ') {
                isCurRowEmpty = false;
            }
        }
        if (isCurRowEmpty) {
            return false;
        }

    }
    return true;
}

void PlayGame(vector<vector<char> > &field) {
    int leftCordinateCol;
    int rowCordinate;
    char direction;
    int steps;

    IntitalizeField(field);
    GenerateRandomRow(field, 9, 0);
    GenerateRandomRow(field, 8, 0);
    GenerateRandomRow(field, 7, 0);
    bool isFieldFull = IsFieldFull(field);
    MoveBlocksDown(field);
    RemoveFullRows(field);
    OutputField(field);
    while (!isFieldFull) {

        bool validUserInput = UserInput(field, leftCordinateCol, rowCordinate, direction, steps);
        if (!validUserInput) {
            cout << "Try again:" << endl;
            continue;
        }
//        char e;
//        cout << "Exit? ";
//        cin >> e;
//        if (e == 'e') {
//            break;
//        }
        MoveBlock(field, rowCordinate - 1, leftCordinateCol - 1, direction, steps);
        RemoveFullRows(field);
        MoveFieldOneUp(field);

        GenerateRandomRow(field, ROW_SIZE - 1, 0);
        MoveBlocksDown(field);
        RemoveFullRows(field);


        OutputField(field);
        isFieldFull = IsFieldFull(field);
    }
    cout << "Game over!" << endl;
    cout << "Final points: ";
    cout << PlayerPoints << endl;
}

//File
void UsersList(const string &filename, vector<string> &users, vector<int> &scores) {
    fstream fileInput(filename, ios::in);
    if (!fileInput) {
        cout << "File can't be oppened" << endl;
        return;
    }
    string line;
    int rowCounter = 0;
    while (getline(fileInput, line)) {
        if (rowCounter % 2 == 0) {
            users.push_back(line);
        } else {
            scores.push_back(stoi(line));
        }
        rowCounter++;
    }
    fileInput.close();
}

bool AddUser(vector<string> &users, vector<int> &scores, const string &user, int newScore) {
    for (int i = 0; i < users.size(); i++) {
        if (users[i] == user) {
            if (newScore > scores[i]) {
                scores[i] = newScore;
            }
            return true;
        }
    }
    users.push_back(user);
    scores.push_back(newScore);
    return false;
}

void UpdateUsersList(const string &filename, const vector<string> &users, const vector<int> &scores) {
    fstream fileOutput(filename, ios::out);
    if (!fileOutput) {
        cout << "File can't be oppened" << endl;
        return;
    }
    for (int i = 0; i < users.size(); i++) {
        fileOutput << users[i] << endl;
        fileOutput << scores[i] << endl;
    }
    fileOutput.close();
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    vector<vector<char> > field;
    string user;
    cout << "Enter your username: ";
    cin >> user;
    PlayGame(field);
    UsersList(UsersFile, Users, Scores);
    AddUser(Users, Scores, user, PlayerPoints);
    UpdateUsersList(UsersFile, Users, Scores);

    int playerIndex = 0;
    for (int i = 0; i < Users.size(); ++i) {
        if (Users[i] == user) {
            playerIndex = i;
        }
    }
    cout << "Highest Score: " << Scores[playerIndex];
}
