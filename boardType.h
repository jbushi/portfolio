//Julian Bushi
//2 April 2016
//CSC210 - Program 4
//Header for Board class

#ifndef H_Board
#define H_Board

class Board
{
public:
    Board();    //default constructor
    Board(int); //parameter constructor
    Board(const Board&); //copy constructor
    ~Board();   //destructor
    void update(int, int, char); //updates one location (y,x) of board with the character

    void print() const; //displays board on screen
    int GetBoardSize() const; //returns board size
    char CheckBoard(int, int) const; //returns char in one location on board (y,x) to see if it is occupied

private:
    int boardSize;
    char **board;
};

#endif // H_Board
