//Julian Bushi
//2 April 2016
//CSC210 - Program 4
//An object oriented Battleship game for one player. Ships are objects of Ship class and stored in a linked list.
//Board class is used for the game board. Includes try/catch blocks for exception handling.

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "boardType.h"
#include "shipType.h"

using namespace std;

const int MAX_SHIPSIZE = 5; //largest ship size

Ship* CreateShipList(int); //initializes linked list of ships and returns head of list
void PlaceShips (Board*, Ship*); //creates ship properties using next 3 functions, and writes them into board objects

void InitializeShip (Ship*, int);  //fills a ship object with random properties
bool IsValidPosition (Ship*, int); //checks if position of ship fits in board
bool IsNotOverlap (Board*, Ship*); //checks if ship is overlapping with existing ship

void UserInput(Board*, int&, int&); //allows user to input attack location
bool CheckIfHit (Board*, int, int, bool&); //checks if ship was hit in selected location
void UpdateBoards(Board*, Board*, bool, int, int, int); //updates board objects with result of turn

Ship* FindShip(Board*, Ship*, int, int, int); //finds which ship was hit in a successful attack and returns a pointer to it
void UpdateShips(Ship*&, Ship*); //updates the affected ship objects when hit
void SinkShip(Ship*&, Ship*); //removes ship from list when destroyed

bool CheckEndGame(Ship*); //checks if game is over (when no ships are left)
void CleanUp(Board*, Board*, Ship*); //deallocates dynamic memory

int main()
{
    int numTurns; //tracks number of turns
    int numShips; //number of ships in game
    int boardSize;//dimension of board
    int x,y;  //used to input attack coordinates
    bool hit; //tracks hit on ships
    bool repeat = false;  //tracks repeat input
    bool endGame = false; //set if game is over

    Ship* head = NULL;    //keeps track of start of the ship list
	Ship* currPtr = NULL;
    Ship* prevPtr = NULL;

    string boardExStr = "Error: Board too small. Using default size..";
    string shipExStr = "Error: Too few ships. Using default number...";

    cout << "Welcome to Battleship! " << endl << endl;

    try
    {
        cout << "How big should the board be? (Default: 10) ";  //User enters size of board
        cin >> boardSize;

        if(boardSize < 4)
            throw(boardExStr); //exception if invalid board size entered (uses default value)
    }
    catch(string)
    {
        cout << boardExStr << endl;
        boardSize = 10;
    }

    Board *UserBoard = new Board (boardSize); //user board used to display during game
    Board *CPUBoard = new Board (boardSize); //hidden CPU board used to track ships

    numTurns = (boardSize*boardSize)/2; //set number of turns

    try
    {
        cin.clear();
        cin.ignore(100,'\n');

        cout << "\nHow many ships should there be? (Default: 5) "; //Use enters number of ships
        cin >> numShips;

        if(numShips < 1)
            throw(shipExStr); //exception if invalid number of ships entered (uses default value)
    }
    catch(string)
    {
        cout << shipExStr << endl;
        numShips = 5;
    }

    cout << endl << "Generating board";

    head = CreateShipList(numShips); //creates list of ships objects

    PlaceShips(CPUBoard, head); //fills ship objects with random properties and places them in board objects

    cout << "Done." << endl << endl;

    //*****************MAIN GAME MENU**************************

    for(int turn=1; turn <= numTurns; turn++) //loops for the given amount of turns
    	{
			cout << "Turn " << turn << ":\n\n";    //displays turn number

			do
			{
				UserInput(UserBoard, x, y); //calls userinput, allows user to make input

				repeat = false; //makes sure repeat isn't still set to true from previous input

				hit = CheckIfHit(CPUBoard, x, y, repeat); //checks if the selected spot is a hit, sets hit accordingly. Also sets repeat (call-by-reference) if it detects a repeat

				if(repeat == true)
					cout << "\nYou have already guessed this spot, try again! \n\n"; //if repeat is set to true from the CheckIfHit call, will allow user to make another input

			}while(repeat == true);


			if (hit == true)                      //outputs hit or miss message depending on hit boolean
				cout << "\nHit!\n" << endl;
			else
				cout << "\nMiss\n" << endl;

			UpdateBoards(UserBoard, CPUBoard, hit, x, y, turn); //this function updates the two boards for the current turn

				if (hit == true)		         //if the turn was a hit, runs the functions necessary to change the current ship's status
					{
						prevPtr = FindShip(CPUBoard, head, x, y, turn); //assigns a pointer to keep track of the ship that was hit
						UpdateShips(head, prevPtr);                     //updates the ship status member of the ship that was hit
    				}

			endGame = CheckEndGame(head); //checks if the game is over at the end of each turn (when all ships are destroyed)

    		if(endGame == true) //if the endGame is true, exits the main loop
			{
				UserBoard->print();

				cout << "\nCongratulations! You won!\n\n";
				break;
			}
		}

		if(endGame == false) //if endGame is false after all turns used, outputs losing message
            cout << "You ran out of turns...Play again\n\n";

		return(0);
}

Ship* CreateShipList(int numShips) //creates a linked list of ships (empty)
{
    Ship* head  = NULL;
    Ship* newNodePtr  = NULL;
    Ship* currPtr  = NULL;


    head = new Ship();  //creates first node in list
    currPtr = head;


    for(int i=0; i<numShips; i++) //loop creates new node and places it after the current one
    {
        newNodePtr = new Ship();
        currPtr->setLink(newNodePtr);
        currPtr = newNodePtr;
    }

	currPtr->setLink(NULL); //sets last link to null

	return head;
}

void PlaceShips (Board* CPUBoard, Ship* ptr) //places ships in CPUboard, ptr is pointer to head
{
	bool isNotOverlap = false;

	InitializeShip(ptr, CPUBoard->GetBoardSize());  //fills a ship node with random properties

	if(ptr->GetOrient() == 0)					    //places first ship (dont need to check for overlap yet)
			{
				for(int i=0; i < ptr->GetSize(); i++)
					{
						CPUBoard->update((ptr->GetYCoord() + i), ptr->GetXCoord(), '#');
					}
			}
	if(ptr->GetOrient() == 1)                                         //writes # char to appropriate spot depending on orientation and size
			{
				for(int i=0; i < ptr->GetSize(); i++)
					{
						CPUBoard->update(ptr->GetYCoord(), (ptr->GetXCoord() + i), '#');
					}

			}

	cout <<".";

	ptr = ptr->getLink();

	while(ptr->getLink() != NULL)						//places other ships, checking for overlap
		{
				do
				{
					InitializeShip(ptr, CPUBoard->GetBoardSize());                        //fills a ship node with random properties
					isNotOverlap = IsNotOverlap(CPUBoard, ptr); //checks for overlap
					//cout << "\n Place Ships 6\n";

				}while(isNotOverlap == false);                  //gets a new set of data for the ship until there's no overlap

				cout << ".";

				if(ptr->GetOrient() == 0)                       //writes # char to appropriate spot depending on orientation and size
					{
						for(int i=0; i < ptr->GetSize(); i++)
							{
								CPUBoard->update((ptr->GetYCoord() + i), ptr->GetXCoord(), '#');
							}
					}
				if(ptr->GetOrient() == 1)
					{
						for(int i=0; i < ptr->GetSize(); i++)
							{
								CPUBoard->update(ptr->GetYCoord(), (ptr->GetXCoord() + i), '#');
							}
					}


			ptr = ptr->getLink();

		}
	return;
}

void InitializeShip (Ship* ptr, int boardSize) //function to fill a ship object with random properties, ptr is pointer to the ship
{
	bool isValid = false;
	srand (time(NULL));           //seed rand based on time


	ptr->SetSize(rand()%(MAX_SHIPSIZE - 2)+ 2);                  //sets a random ship size from (2 - MAX_SHIPSIZE)
	ptr->SetOrient(static_cast<Orientation>(rand() % 2));        //sets a random orientation (horizontal or vertical)

	do                                  //loop creates a random position for the ship, and checks if it is a valid position
	{
		ptr->SetCoords((rand() % boardSize), (rand() % boardSize));

		isValid = IsValidPosition(ptr, boardSize); //sends ship to function to check if it is in a valid position
	}while(isValid == false);

	return;
}

bool IsValidPosition (Ship* ptr, int boardSize) //function to check if ship is placed in a valid position, ptr is pointer to the ship
{
	if ((ptr->GetOrient()) == 0)
		{
			if((ptr->GetYCoord() + ptr->GetSize()) > boardSize) //checks to see if position + length of the ship is greater than the size of the board
				return false;
			else
				return true;
		}
	else if ((ptr->GetOrient()) == 1)
		{
			if((ptr->GetXCoord() + ptr->GetSize()) > boardSize)
				return false;
			else
				return true;
		}
}

bool IsNotOverlap (Board* CPUBoard, Ship* ptr) //function to check if ship is overalapping with another ship, ptr is pointer to the ship
{
	if((ptr->GetOrient()) == 0)
		{
			for(int i=0; i < (ptr->GetSize()); i++)
				{
					if( (CPUBoard->CheckBoard((ptr->GetYCoord() + i), ptr->GetXCoord())) == '#')   //if any of the coordinates the ship would take up are already ship # chars, there is overlap
						return false;
				}
			return true;
		}
	if((ptr->GetOrient()) == 1)
		{
			for(int i=0; i < (ptr->GetSize()); i++)
				{
					if( (CPUBoard->CheckBoard(ptr->GetYCoord(), (ptr->GetXCoord() + i))) == '#')
						return false;
				}
			return true;
		}

}

void UserInput (Board* UserBoard, int& x, int& y) //function that allows the user to input their attack coordinates, x and y are by reference so the main can access them
{
	string inputExStr = "Error. You did not enter a number. ";

	UserBoard->print(); //displays board for the user

	cout << endl;

	do
	{
		try
		{
		cin.clear();
        cin.ignore(100,'\n');

		cout << "Enter the x-coordinate for your attack (1-" << UserBoard->GetBoardSize() << "): "; //allows user to input x coordinate
		cin >> x;

		if (cin.fail())
            throw(inputExStr);  //throws exception if invalid input type

		if (x < 1 || x > UserBoard->GetBoardSize())
			cout << "\nInvalid number.\n";
		}
		catch(string)
		{
		    cout << inputExStr << endl << endl;
		    x = 0;
		    cin.clear();
		}


	}while(x < 1 || x > UserBoard->GetBoardSize()); //loops until valid number has been inputted

	do
	{
		try
		{
		cin.clear();
        cin.ignore(100,'\n');

		cout << "Enter the y-coordinate for your attack (1-" << UserBoard->GetBoardSize() << "): "; //allows user to input x coordinate
		cin >> y;

		if (cin.fail())
            throw(inputExStr); //throws exception if invalid input type

		if (y < 1 || y > UserBoard->GetBoardSize())
			cout << "\nInvalid number.\n";
		}
		catch(string)
		{
		    cout << inputExStr << endl << endl;
		    y = 0;
		    cin.clear();
		}


	}while(y < 1 || y > UserBoard->GetBoardSize()); //loops until valid number has been inputted

	return;
}

bool CheckIfHit (Board* CPUBoard, int x, int y, bool& repeat) //function to check if the selection is a hit, repeat used by reference to check if the spot has already been attacked
{
	if( CPUBoard->CheckBoard((y-1),(x-1)) == '#')       //if spot is ship char, there is a hit
		return true;

	else if( CPUBoard->CheckBoard((y-1),(x-1)) == '~') //if spot is ocean char, there is a miss
		return false;

	else                                //otherwise that spot has already been attacked
		repeat = true;                  //sets repeat to true
		return false;
}

void UpdateBoards (Board* UserBoard, Board* CPUBoard, bool hit, int x, int y, int turn)  //updates the two boards to reflect the current turn
{
	if( hit == true)
	{
		UserBoard->update((y-1),(x-1), 'H');    //UserBoard gets a hit char in that spot
		CPUBoard->update((y-1),(x-1), turn);    //CPUBoard gets the value of the current turn, which is used in FindShip to know which ship has been hit that turn
		return;
	}

	else
	{
		UserBoard->update((y-1),(x-1), '.');  //If no hit, the two boards get a miss char
		CPUBoard->update((y-1),(x-1), '.');
		return;
	}

}

Ship* FindShip (Board* CPUBoard, Ship* ptr, int x, int y, int turn) //function to find which ship has been hit, ptr is pointer to head
{
	Ship* prevPtr = NULL;

	while(ptr->getLink() != NULL) //loops through list
	{

		if(ptr->GetOrient() == 0)
			{
				for(int i=0; i < ptr->GetSize(); i++)
					{
						if( CPUBoard->CheckBoard((ptr->GetYCoord() + i), (ptr->GetXCoord())) == turn) //if the value of the current turn is detected where a ship should be, that is the ship that was hit
							{
								return prevPtr;	//previous ptr is passed so the sinkship function can skip a node to delete it
							}
					}
			}
		if(ptr->GetOrient() == 1)
			{
				for(int i=0; i < ptr->GetSize(); i++)
					{
						if(  CPUBoard->CheckBoard((ptr->GetYCoord()), (ptr->GetXCoord()+i)) == turn)
							{
								return prevPtr;
							}
					}
			}

		prevPtr = ptr->getLink();
		ptr = ptr->getLink();
	}
}

void UpdateShips (Ship*& head, Ship* prevPtr) //function to update the ships list, head is by reference in case the first ship in the list is sunk. prevPtr is from the FindShip function
{

	if(prevPtr == NULL)		//case if affected ship is the first node (prevPtr not set to anything)
	{

		head->updateHit(); //increments status member of ship

		if (head->GetStatus() == head->GetSize())  //if the status equals the size of the ship, all of its sections were hit and the ship should be sunk
		{
			cout << "You sunk my battleship!\n\n";
			SinkShip(head, prevPtr);
			return;
		}

		else
			return;
	}

	else					//case if affected ship is any other node
	{

		prevPtr->getLink()->updateHit();   //increments status member of ship

		if (prevPtr->getLink()->GetStatus() == prevPtr->getLink()->GetSize())   //if the status equals the size of the ship, all of its sections were hit and the ship should be sunk
		{
			cout << "You sunk my battleship!\n\n";
			SinkShip(head, prevPtr);
			return;
		}

		else
			return;
	}
}

void SinkShip (Ship*& head, Ship* prevPtr) //function to remove the sunk ship from the list and deallocate it, head is by reference in case the first ship in the list is sunk. prevPtr is from the UpdateShips (originally FindShip) function
{
	Ship* ptr;

	if (prevPtr == NULL)    // deleting head node
        {
    		ptr = head;
			head = head->getLink();         //sets new head
        }
        else				// deleting any other node
        {
        	ptr = prevPtr->getLink();
            prevPtr->setLink(ptr->getLink()); //links list around the node
        }

        delete ptr;  //deallocates memory

    return;

}

bool CheckEndGame(Ship* ptr) //function to check if the game is over (no ships left in list)
{
	int n = 0;

	while(ptr->getLink() != NULL)
	{
		n++;                  //increments n each time a ship is found in the list
		ptr = ptr->getLink();
	}

	if (n == 0)             //if number of ships is 0, game is over
		return true;
	else					//otherwise game goes on
		return false;
}

void CleanUp (Board* UserBoard, Board* CPUBoard, Ship* head) //function to deallocate any remaining ships in the list
{
	Ship* prevPtr;

	delete UserBoard;
	delete CPUBoard;

	while (head != NULL)                //deallocates ships in list
    {
        prevPtr = head;
        head = head->getLink();
        delete prevPtr;
    }

    return;
}
