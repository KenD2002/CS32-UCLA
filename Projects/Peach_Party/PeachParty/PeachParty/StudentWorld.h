#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Board getBoard() const;
    int getBank() const;
    void setBank(int bank);
    std::vector <Avatar*>& getAvatars();
    std::vector <Actor*>& getActors();
private:
    std::vector <Avatar*> Avatars; // index 0 for Peach, index 2 for Yoshi
    std::vector <Actor*> Actors;
    Board m_Board;
    int m_bank;
};

#endif // STUDENTWORLD_H_
