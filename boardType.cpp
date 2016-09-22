//Julian Bushi
//2 April 2016
//CSC210 - Program 4
//Implementation for Board class

#include <iostream>
#include <cstddef>
#include "boardType.h"

Board::Board()  //default constructor
{
    boardSize = 10;

    board = new char*[boardSize];   //creates default board

    for (int i=0; i < boardSize; i++)   //creates dynamic array
    board[i] = new char[boardSize];

    for (int i = 0; i < boardSize; i++) //fills array with ~ chars
    {
        for (int j = 0; j < boardSize; j++)
        {
            board[i][j] = '~';
        }
    }

}

Board::Board(int x) //parameter constructor
{
    boardSize = x;

    board = new char*[boardSize];   //creates board based on size

    for (int i=0; i < boardSize; i++)   //creates dynamic array
    board[i] = new char[boardSize];

    for (int i = 0; i < boardSize; i++) //fills array with ~ chars
    {
        for (int j = 0; j < boardSize; j++)
        {
            board[i][j] = '~';
        }
    }

}

Board::Board(const Board& obj)  //copy constructor
{
    boardSize = obj.boardSize;

    board = new char*[boardSize];   //creates board same size as other board

    for (int i=0; i < boardSize; i++)   //creates dynamic array
    board[i] = new char[boardSize];

    for (int i = 0; i < boardSize; i++) //fills array with chars of other board
    {
        for (int j = 0; j < boardSize; j++)
        {
            board[i][j] = obj.board[i][j];
        }
    }
}

Board::~Board() //destructor
{
    for(int i = 0; i < boardSize; i++)   //deallocates board
		{
    		delete [] board[i];
		}
			delete [] board;
}

void Board::update(int y, int x, char ch)   //updates one location (y,x) of board with the character
{
    board[y][x] = ch;

    return;
}

void Board::print() const   //displays board on screen
{
    std::cout << "   ";
    for (int i = 0; i < boardSize; i++)     //displays number of column in top row
    {
        if(i<9)
            std::cout << '0' << i+1 << " ";
		else if(i>=9)
            std::cout << i+1 << " ";
    }

    std::cout << std::endl;

    for (int i = 0; i < boardSize; i++) //loops through each array position
    {
		if(i<9)
            std::cout << '0' << i+1 << " "; //displays number of row in first column
		else if(i>=9)
            std::cout << i+1 << " ";

		for (int j = 0; j < boardSize; j++)
        {
			std::cout<< board[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    return;
}

int Board::GetBoardSize() const //returns board size
{
    return boardSize;
}

char Board::CheckBoard(int y, int x) const  //returns char in one location on board (y,x) to see if it is occupied
{
    return board[y][x];
}



