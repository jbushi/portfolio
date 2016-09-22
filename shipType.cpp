//Julian Bushi
//2 April 2016
//CSC210 - Program 4
//Implementation for Ship class

#include <iostream>
#include <cstddef>
#include "shipType.h"

Ship::Ship()    //default constructor
{
    size = 2;       //sets default ship values
    status = 0;
    posX = 0;
    posY = 0;
    orientation = Vertical;

    link = NULL;
}

Ship::Ship(int s, int x, int y, Orientation o)  //parameter constructor
{
    size = s;       //sets passed ship values
    status = 0;
    posX = x;
    posY = y;
    orientation = o;

    link = NULL;
}

Ship::Ship(const Ship& obj) //copy constructor
{
    size = obj.size;       //sets values based on another object
    status = obj.status;
    posX = obj.posX;
    posY = obj.posY;
    orientation = obj.orientation;

    link = obj.link;
}

int Ship::GetXCoord() const   //returns x-coord
{
    return posX;
}

int Ship::GetYCoord() const   //returns y-coord
{
    return posY;
}

int Ship::GetSize() const     //returns size
{
    return size;
}

int Ship::GetStatus() const   //returns status
{
    return status;
}

Orientation Ship::GetOrient() const   //returns orientation
{
    return orientation;
}

void Ship::SetSize(int s)   //sets size
{
    size = s;
    return;
}

void Ship::SetCoords(int x, int y)  //sets location
{
    posX = x;
    posY = y;
    return;
}

void Ship::SetOrient(Orientation o) //sets orientation
{
    orientation = o;
    return;
}

void Ship::updateHit()  //updates status
{
    status = status + 1;
    return;
}

Ship* Ship::getLink() const   //returns link
{
    return link;
}

void Ship::setLink(Ship* newLink)   //sets link
{
    if(newLink == NULL)
        link = NULL;
    else
        link = newLink;
    return;
}


