// This is the .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "TileList.h"

void TileList::removeIndex(int index){
    for (int i = index; i < length; ++i){
        tileArray[i] = tileArray[i+1];
    }
}

TileList::TileList()
{

}

TileList::~TileList()
{
    delete[] tileArray;
}

void TileList::addTile(Tile tile)
{
    if (length == capacity){
        Tile* tileArrayNew = new Tile[capacity*2];
        for (int i = 0; i < length; ++i){
            tileArrayNew[i] = tileArray[i];
        }
        delete[] tileArray;
        tileArray = tileArrayNew;
        capacity *= 2;
    }
    tileArray[length] = tile;
    length++;
}

void TileList::drawAll(QGraphicsScene* scene)
{
    for(int i = 0; i < length; ++i){
        tileArray[i].draw(scene);
    }
}

int TileList::indexOfTopTile(int x, int y)
{
    for (int i = length-1; i >= 0; --i){
        if (tileArray[i].contains(x,y)) return i;
    }
    return -1;
}

void TileList::raise(int x, int y)
{
    int index = indexOfTopTile(x,y);
    if (index != -1){
        Tile topTile = tileArray[index];
        removeIndex(index);
        tileArray[length-1] = topTile;
    }
}

void TileList::lower(int x, int y)
{
    int index = indexOfTopTile(x,y);
    if (index != -1){
        Tile topTile = tileArray[index];
        for (int i = index; i > 0; --i){
            tileArray[i] = tileArray[i-1];
        }
        tileArray[0] = topTile;
    }
}

void TileList::remove(int x, int y)
{
    int index = indexOfTopTile(x,y);
    if (index != -1){
        removeIndex(index);
        length--;
    }
}

void TileList::removeAll(int x, int y)
{
    int counter = 0;
    for (int i = 0; i < length; ++i){
        if (tileArray[i].contains(x,y)) counter++;
    }
    for (int i = 0; i < counter; ++i){
        remove(x,y);
    }
}

