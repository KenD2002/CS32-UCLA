#ifndef ACTOR_H_
#define ACTOR_H_

#include "GameConstants.h"
#include "GraphObject.h"
#include "GameWorld.h"

class StudentWorld;

class Actor : public GraphObject // Base class for Actor
{
public:
    Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int depth, double size, bool impactable);
    virtual void doSomething() = 0;
    virtual void impact(){ return; }
    StudentWorld* getWorld() const;
    int getMovingDirection() const;
    void setMovingDirection(int movingDirection);
    bool getAlive() const;
    void setAlive(bool aliveStat);
    bool getImpactable() const;
private:
    StudentWorld* m_world;
    bool m_alive;
    int m_movingDirection; // 0 for right, 90 for up, 180 for left, 270 for down
    bool m_impactable; // true for impactable
};


class Avatar : public Actor // Derived class for Peach and Yoshi //unfinished
{
public:
    Avatar(StudentWorld* world, int imageID, double startX, double startY, int whichPlayer);
    virtual void doSomething();
    int getCoins() const;
    void setCoins(int coins);
    int getStars() const;
    void setStars(int stars);
    int getDieRoll() const;
    void setDieRoll(int dieRoll);
    void setAmmunition(bool ammunition); // increment by 1 if ammunition is true, decrement by 1 if ammunition if false
    bool getAmmunition() const;
    void setTeleport(bool teleport);
    bool getTeleport() const;
    void setNew(bool newStats);
    bool getNew() const;
    void setTicks(int ticks);
    int getTicks() const;
    void setWaitingToRoll(bool waitingToRoll);
    bool getWaitingToRoll() const;
    void setBowserActive(bool bowserActive);
    bool getBowserActive() const;
    void setBooActive(bool bowserActive);
    bool getBooActive() const;
    bool land(int squareX, int squareY) const; // return true if landed on (not passing through) a square. squareX/Y are the pixel coordinates of the square
    bool moveOnto(int squareX, int squareY) const; // return true if moved onto (passing through) a square. squareX/Y are the pixel coordinates of the square
private:
    int m_whichPlayer; // 0 for Peach and 1 for Yoshi
    int m_die_roll;
    int m_ticks_to_move;
    bool m_waiting_to_roll; // false for walking and true for waiting to roll
    int m_coins;
    int m_stars;
    bool m_ammunition; // false for having no vortex, true for having
    bool m_teleport; // true if just been teleported, false for not
    bool m_start; // true for it's just initiated, so the user will not be able to choose the direction to go when the game starts
    bool m_new; // true if is a "new" player
    bool m_bowserActive; // true if can be affected by bowser
    bool m_booActive; // true if can be affected by boo
};


// Coin Square class
class CoinSquares : public Actor // Coin Squares
{
public:
    CoinSquares(StudentWorld* world, int imageID, double startX, double startY);
    virtual void doSomething();
};


// Star Square class
class StarSquares : public Actor // Star Squares
{
public:
    StarSquares(StudentWorld* world, int imageID, double startX, double startY);
    virtual void doSomething();
};

// Directional Square class
class DirectionalSquares : public Actor
{
public:
    DirectionalSquares(StudentWorld* world, int imageID, double startX, double startY, int startDirection);
    virtual void doSomething();
};

// Bank Square class
class BankSquares : public Actor
{
public:
    BankSquares(StudentWorld* world, int imageID, double startX, double startY);
    virtual void doSomething();
    void bankDoSomething(Avatar* avatarPtr); // template function for both Peach and Yoshi
};

// Event Square class
class EventSquares : public Actor
{
public:
    EventSquares(StudentWorld* world, int imageID, double startX, double startY);
    virtual void doSomething();
};

// Dropping Square class
class DroppingSquares : public Actor
{
public:
    DroppingSquares(StudentWorld* world, int imageID, double startX, double startY);
    virtual void doSomething();
};

// Baddie Square class
class BaddieSquares : public Actor
{
public:
    BaddieSquares(StudentWorld* world, int imageID, double startX, double startY);
    virtual void doSomething();
    virtual void pausedState_a(Avatar* avatarPtr, Avatar* avatarPtr2) = 0;
    virtual void pausedState_c() = 0;
    virtual void walkingState_e() = 0;
    void setSquareToMive(int squareToMove);
    void setTicksToMove(int ticksToMove);
    virtual void impact();
private:
    bool m_pauseState; // true for paused, false for walking
    int m_pauseCounter;
    int m_travelDist; // in pixels
    int m_squareToMove;
    int m_ticksToMove;
};

// Bowser Square class
class BowserSquares : public BaddieSquares
{
public:
    BowserSquares(StudentWorld* world, int imageID, double startX, double startY);
    virtual void pausedState_a(Avatar* avatarPtr, Avatar* avatarPtr2);
    virtual void pausedState_c();
    virtual void walkingState_e();
};

// Boo Square class
class BooSquares : public BaddieSquares
{
public:
    BooSquares(StudentWorld* world, int imageID, double startX, double startY);
    virtual void pausedState_a(Avatar* avatarPtr, Avatar* avatarPtr2);
    virtual void pausedState_c();
    virtual void walkingState_e();
};


// Vortex Square class
class VortexSquares : public Actor
{
public:
    VortexSquares(StudentWorld* world, int imageID, double startX, double startY, int fireDirection);
    virtual void doSomething();
private:
    bool m_active;
};



// Check if an actor can keep moving in some direction
bool validInDirection(Actor* actor, int movingDirection);

#endif // ACTOR_H_
