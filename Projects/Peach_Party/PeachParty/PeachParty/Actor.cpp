#include "Actor.h"
#include "StudentWorld.h"
#include <ctime>
#include <cstdlib>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


// Actor Implementation, base class
Actor::Actor(StudentWorld* world, int imageID, double startX, double startY, int startDirection, int depth, double size, bool impactable)
:GraphObject(imageID, startX, startY, startDirection, depth, size), m_world(world), m_alive(true), m_movingDirection(0), m_impactable(impactable)
{}


StudentWorld* Actor::getWorld() const
{
    return m_world;
}


int Actor::getMovingDirection() const
{
    return (m_movingDirection);
}

void Actor::setMovingDirection(int movingDirection)
{
    m_movingDirection = movingDirection;
}

bool Actor::getAlive() const
{
    return m_alive;
}

void Actor::setAlive(bool aliveStat)
{
    m_alive = aliveStat;
}

bool Actor::getImpactable() const
{
    return m_impactable;
}

// Player Implementation, derived class for Peach and Yoshi from Actor
Avatar::Avatar(StudentWorld* world, int imageID, double startX, double startY, int whichPlayer)
:Actor(world, imageID, startX, startY, right, 0, 1, false), m_ticks_to_move(0), m_waiting_to_roll(true), m_die_roll(0), m_coins(0), m_stars(0), m_ammunition(false), m_teleport(false), m_start(true), m_new(true), m_whichPlayer(whichPlayer), m_booActive(false), m_bowserActive(false)
{}

void Avatar::doSomething()
{
    if(m_waiting_to_roll) // If waiting_to_roll
    {
        // a
        if(m_teleport)
        {
            if(!validInDirection(this, this->getMovingDirection()))
            {
                time_t present_time;
                time(&present_time);
                srand(unsigned(present_time));
                int randomNewDirection = (this->getMovingDirection() + 90 * (rand() % 3 + 1)) % 360;
                while(!validInDirection(this, randomNewDirection)) // pick a random direction
                {
                    randomNewDirection = (randomNewDirection + 90) % 360;
                }
                setMovingDirection(randomNewDirection);
                if(randomNewDirection == 180)
                {
                    setDirection(180);
                }
                else
                {
                    setDirection(0);
                }
            }
        }
        
        
        // b
        switch (getWorld()->getAction(m_whichPlayer))
        {
            case ACTION_ROLL: //c
                time_t present_time;
                time(&present_time);
                srand(unsigned(present_time) + rand() * m_whichPlayer); // to make the dice rolls different for 2 players if they roll in the same second
                m_die_roll = rand() % 10 + 1; // die_roll = random number from 1 to 10, inclusive
                m_ticks_to_move = 8 * m_die_roll;

                m_waiting_to_roll = false; //Change the Avatar's state to the walking state.
                break;
            case ACTION_FIRE: //d
                if(m_ammunition == false)
                {
                    return;
                }
                getWorld()->getActors().push_back(new VortexSquares(getWorld(), IID_VORTEX, getX(), getY(), getMovingDirection()));
                this->getWorld()->playSound(SOUND_PLAYER_FIRE);
                this->setAmmunition(false);
                break;
            default: //e
                return;
        }
    }
    
    if(!m_waiting_to_roll) // If walking
    {
        if(getX() % 16 == 0 && getY() % 16 == 0) // Avatar exactly on a square
        {
            //a on a direction square
            Board::GridEntry squareContent = getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16);
            if(squareContent == Board::up_dir_square || squareContent == Board::down_dir_square || squareContent == Board::left_dir_square || squareContent == Board::right_dir_square)
            {
                switch(squareContent)
                {
                    case Board::up_dir_square:
                        setMovingDirection(90);
                        setDirection(0);
                        break;
                    case Board::down_dir_square:
                        setMovingDirection(270);
                        setDirection(0);
                        break;
                    case Board::left_dir_square:
                        setMovingDirection(180);
                        setDirection(180);
                        break;
                    case Board::right_dir_square:
                        setMovingDirection(0);
                        setDirection(0);
                        break;
                    default:
                        break;
                }
            }
            //b on a fork
            else if(!m_start && !m_teleport && (validInDirection(this, getMovingDirection()) + validInDirection(this, (getMovingDirection() + 90) % 360) + validInDirection(this, (getMovingDirection() + 270) % 360) > 1)) // more than 1 path available and Avatar has at least moved once
            {
                switch (getWorld()->getAction(m_whichPlayer))
                {
                    case ACTION_UP:
                        if(validInDirection(this, 90))
                        {
                            setMovingDirection(90);
                            setDirection(0);
                        }
                        break;
                    case ACTION_DOWN:
                        if(validInDirection(this, 270))
                        {
                            setMovingDirection(270);
                            setDirection(0);
                        }
                        break;
                    case ACTION_LEFT:
                        if(validInDirection(this, 180))
                        {
                            setMovingDirection(180);
                            setDirection(180);
                        }
                        break;
                    case ACTION_RIGHT:
                        if(validInDirection(this, 0))
                        {
                            setMovingDirection(0);
                            setDirection(0);
                        }
                        break;
                    default:
                        return;
                }
            }
            //c can't continue moving forward
            else if(!validInDirection(this, this->getMovingDirection()))
            {
                switch(getMovingDirection())
                {
                    case 0: // moving right or left, prefer up over down
                    case 180:
                        if(validInDirection(this, 90))
                        {
                            setMovingDirection(90);
                        }
                        else
                        {
                            setMovingDirection(270);
                        }
                        break;
                    case 90: // moving up or down, prefer right over left
                    case 270:
                        if(validInDirection(this, 0))
                        {
                            setMovingDirection(0);
                        }
                        else
                        {
                            setMovingDirection(180);
                        }
                    default:
                        break;
                }

                if(getMovingDirection() == 180)
                {
                    this->setDirection(180); // spirit direction
                }
                else
                {
                    this->setDirection(0); // spirit direction
                }
            }
        }
        
        m_start = false;
        m_new = true;
        m_bowserActive = true;
        m_booActive = true;
        
        //d
        switch(getMovingDirection())  // move in updated direction
        {
            case 0: // moving right
                moveTo(getX() + 2, getY());
                break;
            case 90: // moving up
                moveTo(getX(), getY() + 2);
                break;
            case 180: // moving left
                moveTo(getX() - 2, getY());
                break;
            case 270: // moving down
                moveTo(getX(), getY() - 2);
                break;
            default:
                break;
        }
        //e
        m_ticks_to_move --;
        m_die_roll = (m_ticks_to_move + 7) / 8; // for text display
        //f
        if(m_ticks_to_move == 0)
        {
            m_teleport = false;
            m_waiting_to_roll = true;
        }
    }
}

int Avatar::getCoins() const
{
    return m_coins;
}

void Avatar::setCoins(int coins)
{
    m_coins += coins;
}

int Avatar::getStars() const
{
    return m_stars;
}

void Avatar::setStars(int stars)
{
    m_stars += stars;
}

int Avatar::getDieRoll() const
{
    return m_die_roll;
}

void Avatar::setDieRoll(int dieRoll)
{
    m_die_roll = dieRoll;
}

void Avatar::setBowserActive(bool bowserActive)
{
    m_bowserActive = bowserActive;
}

bool Avatar::getBowserActive() const
{
    return m_bowserActive;
}

void Avatar::setBooActive(bool bowserActive)
{
    m_booActive = bowserActive;
}

bool Avatar::getBooActive() const
{
    return m_booActive;
}

void Avatar::setAmmunition(bool ammunition)
{
    m_ammunition = ammunition;
}

bool Avatar::getAmmunition() const
{
    return m_ammunition;
}

void Avatar::setTeleport(bool teleport)
{
    m_teleport = teleport;
}

bool Avatar::getTeleport() const
{
    return m_teleport;
}

void Avatar::setNew(bool newStats)
{
    m_new = newStats;
}

bool Avatar::getNew() const
{
    return m_new;
}

int Avatar::getTicks() const
{
    return m_ticks_to_move;
}

void Avatar::setTicks(int ticks)
{
    m_ticks_to_move = ticks;
}

bool Avatar::getWaitingToRoll() const
{
    return m_waiting_to_roll;
}

void Avatar::setWaitingToRoll(bool waitingToRoll)
{
    m_waiting_to_roll = waitingToRoll;
}

bool Avatar::land(int squareX, int squareY) const
{
    if(getX() == squareX && getY() == squareY && m_waiting_to_roll == true)
    {
        return true;
    }
    return false;
}

bool Avatar::moveOnto(int squareX, int squareY) const
{
    if(getX() == squareX && getY() == squareY && m_waiting_to_roll == false)
    {
        return true;
    }
    return false;
}


// CoinSquares Implementation, derived class from Actor
CoinSquares::CoinSquares(StudentWorld* world, int imageID, double startX, double startY)
:Actor(world, imageID, startX, startY, 0, 1, 1, false)
{}

void CoinSquares::doSomething()
{
    if(!this->getAlive())
    {
        return;
    }
    
    bool coinType = getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::blue_coin_square || getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::player || getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::bowser || getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::boo; // true for blue, false for red
    for(int i = 0; i < 2; i ++)
    {
        Avatar* avatarPtr = getWorld()->getAvatars()[i];
        int numCoins = 0;
        int soundToPlay = 0;
        if(avatarPtr->land(getX(), getY()) && avatarPtr->getNew())
        {
            if(coinType) //blue
            {
                numCoins = 3;
                soundToPlay = SOUND_GIVE_COIN;
            }
            else
            {
                if(avatarPtr->getCoins() < 3)
                {
                    numCoins = -avatarPtr->getCoins();
                }
                else
                {
                    numCoins = -3;
                }
                soundToPlay = SOUND_TAKE_COIN;
            }

            avatarPtr->setCoins(numCoins);
            getWorld()->playSound(soundToPlay);
            avatarPtr->setNew(false);
        }
    }
}

// StarSquares Implementation, derived class from Actor
StarSquares::StarSquares(StudentWorld* world, int imageID, double startX, double startY)
:Actor(world, imageID, startX, startY, 0, 1, 1, false)
{}

void StarSquares::doSomething()
{
    for(int i = 0; i < 2; i ++)
    {
        Avatar* avatarPtr = getWorld()->getAvatars()[i];
        if(avatarPtr->moveOnto(getX(), getY()) || (avatarPtr->land(getX(), getY()) && avatarPtr->getNew()))
        {
            if(avatarPtr->getCoins() < 20)
            {
                return;
            }
            else
            {
                avatarPtr->setCoins(-20);
                avatarPtr->setStars(1);
                getWorld()->playSound(SOUND_GIVE_STAR);
                avatarPtr->setNew(false);
            }
        }
    }
}


// DirectionalSquares Implementation, derived class from Actor
DirectionalSquares::DirectionalSquares(StudentWorld* world, int imageID, double startX, double startY, int startDirection)
:Actor(world, imageID, startX, startY, startDirection, 1, 1, false)
{}

void DirectionalSquares::doSomething()
{
    // implemented inside the Avatar class
}


// BankSuqares Implementation, derived class from Actor
BankSquares::BankSquares(StudentWorld* world, int imageID, double startX, double startY)
:Actor(world, imageID, startX, startY, 0, 1, 1, false)
{}

void BankSquares::doSomething()
{
    for(int i = 0; i < 2; i ++)
    {
        Avatar* avatarPtr = getWorld()->getAvatars()[i];
        if(avatarPtr->land(getX(), getY()) && avatarPtr->getNew())
        {
            avatarPtr->setCoins(avatarPtr->getWorld()->getBank());
            getWorld()->setBank(-getWorld()->getBank());
            getWorld()->playSound(SOUND_WITHDRAW_BANK);
            avatarPtr->setNew(false);
        }
        else if(avatarPtr->moveOnto(getX(), getY()))
        {
            if(avatarPtr->getCoins() < 5)
            {
                getWorld()->setBank(avatarPtr->getCoins());
                avatarPtr->setCoins(-avatarPtr->getCoins());
            }
            else
            {
                getWorld()->setBank(5);
                avatarPtr->setCoins(-5);
            }
            getWorld()->playSound(SOUND_DEPOSIT_BANK);
        }
    }
}


// EventSuqares Implementation, derived class from Actor
EventSquares::EventSquares(StudentWorld* world, int imageID, double startX, double startY)
:Actor(world, imageID, startX, startY, 0, 1, 1, false)
{}

void EventSquares::doSomething()
{
    time_t present_time;
    time(&present_time);
    srand(unsigned(present_time));
    int eventNum = rand() % 3 + 1;
    for(int i = 0; i < 2; i ++)
    {
        Avatar* avatarPtr = getWorld()->getAvatars()[i];
        if(avatarPtr->land(getX(), getY()) && avatarPtr->getNew())
        {
            // option 1
            if(eventNum == 1)
            {
                avatarPtr->setTeleport(true);
                int teleX = rand() % 16;
                int teleY = rand() % 16;
                while(getWorld()->getBoard().getContentsOf(teleX, teleY) == Board::empty)
                {
                    teleX = rand() % 16;
                    teleY = rand() % 16;
                }
                avatarPtr->moveTo(teleX * 16, teleY * 16);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            }
            // option 2
            if(eventNum == 2)
            {
                Avatar* avatarPtr2 = getWorld()->getAvatars()[(i + 1) % 2];
                avatarPtr->setTeleport(true);
                avatarPtr2->setTeleport(true);
                avatarPtr2->setNew(false);
                //i
                int tempX = avatarPtr->getX();
                int tempY = avatarPtr->getY();
                avatarPtr->moveTo(avatarPtr2->getX(), avatarPtr2->getY());
                avatarPtr2->moveTo(tempX, tempY);
                //ii
                int tempTicks = avatarPtr->getTicks();
                int tempDieRoll = avatarPtr->getDieRoll();
                avatarPtr->setTicks(avatarPtr2->getTicks());
                avatarPtr2->setTicks(tempTicks);
                avatarPtr->setDieRoll(avatarPtr2->getDieRoll());
                avatarPtr2->setDieRoll(tempDieRoll);
                //iii
                int tempMovingDirection = avatarPtr->getMovingDirection();
                avatarPtr->setMovingDirection(avatarPtr2->getMovingDirection());
                avatarPtr2->setMovingDirection(tempMovingDirection);
                //iv
                int tempSpiritDirection = avatarPtr->getDirection();
                avatarPtr->setDirection(avatarPtr2->getDirection());
                avatarPtr2->setDirection(tempSpiritDirection);
                //v
                bool tempWaitingToRoll = avatarPtr->getWaitingToRoll();
                avatarPtr->setWaitingToRoll(avatarPtr2->getWaitingToRoll());
                avatarPtr2->setWaitingToRoll(tempWaitingToRoll);
                getWorld()->playSound(SOUND_PLAYER_TELEPORT);
            }
            // option 3
            else
            {
                avatarPtr->setAmmunition(true);
                getWorld()->playSound(SOUND_GIVE_VORTEX);
                avatarPtr->setNew(false);
            }
        }
    }
}


// DroppingSuqares Implementation, derived class from Actor
DroppingSquares::DroppingSquares(StudentWorld* world, int imageID, double startX, double startY)
:Actor(world, imageID, startX, startY, 0, 1, 1, false)
{}

void DroppingSquares::doSomething()
{
    time_t present_time;
    time(&present_time);
    srand(unsigned(present_time));
    int droppingNum = rand() % 2 + 1;
    for(int i = 0; i < 2; i ++)
    {
        Avatar* avatarPtr = getWorld()->getAvatars()[i];
        if(avatarPtr->getX() == getX() && avatarPtr->getY() == getY() && avatarPtr->getWaitingToRoll() && avatarPtr->getNew())
        {
            getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
            if(droppingNum == 1 || avatarPtr->getStars() < 1) //option 1
            {
               if(avatarPtr->getCoins() < 10)
               {
                   avatarPtr->setCoins(-avatarPtr->getCoins());
               }
               else
               {
                   avatarPtr->setCoins(-10);
               }
            }
            else // option 2
            {
               avatarPtr->setStars(-1);
            }
            avatarPtr->setNew(false);
        }
    }
}


// BaddieSquares Implementation, class for baddies, derived from Actor
BaddieSquares::BaddieSquares(StudentWorld* world, int imageID, double startX, double startY)
:Actor(world, imageID, startX, startY, 0, 0, 1, true), m_pauseState(true), m_travelDist(0), m_pauseCounter(180), m_squareToMove(0)
{
    setMovingDirection(right);
}

void BaddieSquares::setSquareToMive(int squareToMove)
{
    m_squareToMove = squareToMove;
}

void BaddieSquares::setTicksToMove(int ticksToMove)
{
    m_ticksToMove = ticksToMove;
}

void BaddieSquares::doSomething()
{
    time_t present_time;
    time(&present_time);
    srand(unsigned(present_time));
    if(m_pauseState == true) // pause state
    {
        //a
        for(int i = 0; i < 2; i ++)
        {
            Avatar* avatarPtr = getWorld()->getAvatars()[i];
            Avatar* avatarPtr2 = getWorld()->getAvatars()[(i + 1) % 2];
            if(avatarPtr->land(getX(), getY()))
            {
                pausedState_a(avatarPtr, avatarPtr2);
            }
        }
        //b
        m_pauseCounter --;
        //c
        if(m_pauseCounter == 0)
        {
            pausedState_c();
            m_ticksToMove = 8 * m_squareToMove;
            
            int randomNewDirection = (90 * (rand() % 4)) % 360; // pick a random direction
            while(!validInDirection(this, randomNewDirection)) // check if it's valid
            {
                randomNewDirection = (randomNewDirection + 90) % 360; // if not valid use another perpendicular direction
            }
            setMovingDirection(randomNewDirection);
            if(randomNewDirection == 180)
            {
                setDirection(180);
            }
            else
            {
                setDirection(0);
            }
            m_pauseState = false;
        }
    }
    
    if(m_pauseState == false) // walking state
    {
        // a on a fork and exactly on a square
        if(getX() % 16 == 0 && getY() % 16 == 0 && validInDirection(this, getMovingDirection()) + validInDirection(this, (getMovingDirection() + 90) % 360) + validInDirection(this, (getMovingDirection() + 270) % 360) > 1)
        {
            // pick a random direction
            int randomNewDirection = (90 * (rand() % 4)) % 360;
            while(!validInDirection(this, randomNewDirection)) // check if it's valid
            {
                randomNewDirection = (randomNewDirection + 90) % 360; // if not valid use another perpendicular direction
            }
            setMovingDirection(randomNewDirection);
            if(randomNewDirection == 180)
            {
                setDirection(180);
            }
            else
            {
                setDirection(0);
            }
        }
        // b can't continue moving forward
        else if(getX() % 16 == 0 && getY() % 16 == 0 && !validInDirection(this, this->getMovingDirection()))
        {
            switch(getMovingDirection())
            {
                case 0: // moving right or left, prefer up over down
                case 180:
                    if(validInDirection(this, 90))
                    {
                        setMovingDirection(90);
                    }
                    else
                    {
                        setMovingDirection(270);
                    }
                    break;
                case 90: // moving up or down, prefer right over left
                case 270:
                    if(validInDirection(this, 0))
                    {
                        setMovingDirection(0);
                    }
                    else
                    {
                        setMovingDirection(180);
                    }
                default:
                    break;
            }
            
            if(getMovingDirection() == 180)
            {
                this->setDirection(180); // spirit direction
            }
            else
            {
                this->setDirection(0); // spirit direction
            }
        }
        
        // c
        switch(getMovingDirection())  // move in updated direction
        {
            case 0: // moving right
                moveTo(getX() + 2, getY());
                break;
            case 90: // moving up
                moveTo(getX(), getY() + 2);
                break;
            case 180: // moving left
                moveTo(getX() - 2, getY());
                break;
            case 270: // moving down
                moveTo(getX(), getY() - 2);
                break;
            default:
                break;
        }
        
        // d
        m_ticksToMove --;
        
        // e
        if(m_ticksToMove == 0)
        {
            m_pauseState = true;
            m_pauseCounter = 180;
            walkingState_e();
        }
    }
}

void BaddieSquares::impact()
{
    time_t present_time;
    time(&present_time);
    srand(unsigned(present_time));
    int teleX = rand() % 16;
    int teleY = rand() % 16;
    while(getWorld()->getBoard().getContentsOf(teleX, teleY) == Board::empty)
    {
        teleX = rand() % 16;
        teleY = rand() % 16;
    }
    this->moveTo(teleX * 16, teleY * 16);
    setDirection(0);
    setMovingDirection(0);
    m_pauseState = true;
    m_pauseCounter = 180;
}



// Bowser Implementation, class for bowser, derived from BaddieSquares
BowserSquares::BowserSquares(StudentWorld* world, int imageID, double startX, double startY)
:BaddieSquares(world, imageID, startX, startY)
{}

void BowserSquares::pausedState_a(Avatar* avatarPtr, Avatar* avatarPtr2)
{
    time_t present_time;
    time(&present_time);
    srand(unsigned(present_time));
    if(avatarPtr->getBowserActive() && rand() % 2 == 1)
    {
        avatarPtr->setCoins(-avatarPtr->getCoins());
        avatarPtr->setStars(-avatarPtr->getStars());
        getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
    }
    avatarPtr->setBowserActive(false);
}

void BowserSquares::pausedState_c()
{
    time_t present_time;
    time(&present_time);
    srand(unsigned(present_time));
    setSquareToMive(rand() % 10 + 1);
}

void BowserSquares::walkingState_e()
{
    if(rand() % 4 == 1) // 25% chance
    {
        if(getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::blue_coin_square || getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::red_coin_square || getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::player || getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::bowser || getWorld()->getBoard().getContentsOf(getX() / 16, getY() / 16) == Board::boo)
        {
            for(int i = 0; i < getWorld()->getActors().size(); i ++)
            {
                if(getWorld()->getActors()[i] == this)
                {
                    continue;
                }
                else if(getWorld()->getActors()[i]->getX() == getX() && getWorld()->getActors()[i]->getY() == getY())
                {
                    getWorld()->getActors()[i]->setAlive(false);
                    getWorld()->getActors().push_back(new DroppingSquares(getWorld(), IID_DROPPING_SQUARE, getX(), getY()));
                    getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
                    break;
                }
            }
        }
    }
}

// Boo Implementation, class for bowser, derived from BaddieSquares
BooSquares::BooSquares(StudentWorld* world, int imageID, double startX, double startY)
:BaddieSquares(world, imageID, startX, startY)
{}

void BooSquares::pausedState_a(Avatar* avatarPtr, Avatar* avatarPtr2)
{
    time_t present_time;
    time(&present_time);
    srand(unsigned(present_time));
    if(avatarPtr->getBooActive() && rand() % 2 == 1)
    {
        int tempCoins = avatarPtr->getCoins();
        avatarPtr->setCoins(avatarPtr2->getCoins() - tempCoins);
        avatarPtr2->setCoins(tempCoins - avatarPtr2->getCoins());
    }
    else
    {
        int tempStars = avatarPtr->getStars();
        avatarPtr->setStars(avatarPtr2->getStars() - tempStars);
        avatarPtr2->setStars(tempStars - avatarPtr2->getStars());
    }
    getWorld()->playSound(SOUND_BOO_ACTIVATE);
    avatarPtr->setBooActive(false);
}

void BooSquares::pausedState_c()
{
    time_t present_time;
    time(&present_time);
    srand(unsigned(present_time));
    setSquareToMive(rand() % 3 + 1);
}

void BooSquares::walkingState_e()
{
    return;
}


// Vortex Implementation, class for vortex, derived from Actor
VortexSquares::VortexSquares(StudentWorld* world, int imageID, double startX, double startY, int fireDirection)
:Actor(world, imageID, startX, startY, 0, 0, 1, false), m_active(true)
{
    setMovingDirection(fireDirection);
}

void VortexSquares::doSomething()
{
    if(!m_active)
    {
        return;
    }
    
    switch(getMovingDirection())  // move in updated direction
    {
        case 0: // moving right
            moveTo(getX() + 2, getY());
            break;
        case 90: // moving up
            moveTo(getX(), getY() + 2);
            break;
        case 180: // moving left
            moveTo(getX() - 2, getY());
            break;
        case 270: // moving down
            moveTo(getX(), getY() - 2);
            break;
        default:
            break;
    }
    
    if(getX() < 0 || getX() > VIEW_WIDTH || getY() < 0 || getY() > VIEW_HEIGHT)
    {
        m_active = false;
        setAlive(false);
    }
    
    for(int i = 0; i < getWorld()->getActors().size(); i ++)
    {
        if(getWorld()->getActors()[i]->getImpactable() && getX() <= getWorld()->getActors()[i]->getX() + 16 && getX() >= getWorld()->getActors()[i]->getX() - 16 && getY() <= getWorld()->getActors()[i]->getY() + 16 && getY() >= getWorld()->getActors()[i]->getY() - 16)
        {
            getWorld()->getActors()[i]->impact();
            setAlive(false);
            m_active = false;
            getWorld()->playSound(SOUND_HIT_BY_VORTEX);
            break;
        }
    }
}


    
// Check if an actor can keep moving in some direction
bool validInDirection(Actor* actor, int movingDirection)
{
    int gridNewX = 0;
    int gridNewY = 0;
    actor->getPositionInThisDirection(movingDirection, 16, gridNewX, gridNewY); // newX, newY in pixels
    if(gridNewX % 16 != 0 || gridNewY % 16 != 0)
    {
        return true;
    }
    
    gridNewX /= 16;
    gridNewY /= 16;
    if(actor->getWorld()->getBoard().getContentsOf(gridNewX, gridNewY) == Board::empty) // If the new square is empty
    {
        return false;
    }
    return true;
}



