//Julian Bushi
//2 April 2016
//CSC210 - Program 4
//Header for Ship class

#ifndef H_Ship
#define H_Ship

enum Orientation {Vertical, Horizontal}; //orientation of ship

class Ship
{
public:
    Ship(); //default constructor
    Ship(int,int,int,Orientation);  //parameter constructor
    Ship(const Ship&);  //copy constructor

    int GetXCoord() const;    //returns x-coord
    int GetYCoord() const;    //returns y-coord
    int GetSize() const;      //returns size
    int GetStatus() const;    //returns status
    Orientation GetOrient() const;    //returns orientation

    void SetSize(int);          //sets size
    void SetCoords(int, int);   //sets location
    void SetOrient(Orientation);//sets orientation
    void updateHit();   //updates status

    Ship* getLink() const;            //returns link
    void setLink(Ship* newLink);//sets link


private:
    int size;                 //length of ship
    int status;               //keeps track of number of hits on this ship
    int posX;                 //x-coord of ship
    int posY;                 //y-coord of ship
    Orientation orientation;  //orientation of ship

    Ship* link; //link to next ship node
};

#endif // H_Ship
