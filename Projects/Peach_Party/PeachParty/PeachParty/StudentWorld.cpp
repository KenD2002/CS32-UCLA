#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), m_bank(0)
{}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

void StudentWorld::cleanUp()
{
    vector<Avatar*>::iterator j;
    j = Avatars.begin();
    while (j != Avatars.end())
    {
        delete (*j);
        j = Avatars.erase(j);
    }
    
    vector<Actor*>::iterator i;
    i = Actors.begin();
    while (i != Actors.end())
    {
        delete (*i);
        i = Actors.erase(i);
    }
}

int StudentWorld::init()
{
    std::string boardName = assetPath() + "/board0" + std::to_string(getBoardNumber()) + ".txt";
    m_Board.loadBoard(boardName);

    // create Actors
    for(int i = 0; i < BOARD_WIDTH; i ++)
    {
        for(int j = 0; j < BOARD_HEIGHT; j ++)
        {
            switch(m_Board.getContentsOf(i, j))
            {
                case Board::player:
                    Avatars.push_back(new Avatar(this, IID_PEACH, i * 16, j * 16, 1)); // Player Peach
                    Avatars.push_back(new Avatar(this, IID_YOSHI, i * 16, j * 16, 2)); // Player Yoshi
                    Actors.push_back(new CoinSquares(this, IID_BLUE_COIN_SQUARE, i * 16, j * 16));
                case Board::blue_coin_square:
                    Actors.push_back(new CoinSquares(this, IID_BLUE_COIN_SQUARE, i * 16, j * 16));
                    break;
                case Board::red_coin_square:
                    Actors.push_back(new CoinSquares(this, IID_RED_COIN_SQUARE, i * 16, j * 16));
                    break;
                case Board::star_square:
                    Actors.push_back(new StarSquares(this, IID_STAR_SQUARE, i * 16, j * 16));
                    break;
                case Board::right_dir_square:
                    Actors.push_back(new DirectionalSquares(this, IID_DIR_SQUARE, i * 16, j * 16, 0));
                    break;
                case Board::up_dir_square:
                    Actors.push_back(new DirectionalSquares(this, IID_DIR_SQUARE, i * 16, j * 16, 90));
                    break;
                case Board::left_dir_square:
                    Actors.push_back(new DirectionalSquares(this, IID_DIR_SQUARE, i * 16, j * 16, 180));
                    break;
                case Board::down_dir_square:
                    Actors.push_back(new DirectionalSquares(this, IID_DIR_SQUARE, i * 16, j * 16, 270));
                    break;
                case Board::bank_square:
                    Actors.push_back(new BankSquares(this, IID_BANK_SQUARE, i * 16, j * 16));
                    break;
                case Board::event_square:
                    Actors.push_back(new EventSquares(this, IID_EVENT_SQUARE, i * 16, j * 16));
                    break;
                case Board::bowser:
                    Actors.push_back(new CoinSquares(this, IID_BLUE_COIN_SQUARE, i * 16, j * 16));
                    Actors.push_back(new BowserSquares(this, IID_BOWSER, i * 16, j * 16));
                    break;
                case Board::boo:
                    Actors.push_back(new CoinSquares(this, IID_BLUE_COIN_SQUARE, i * 16, j * 16));
                    Actors.push_back(new BooSquares(this, IID_BOO, i * 16, j * 16));
                    break;
                default:
                    break;
            }
        }
    }
    
    startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds
    return GWSTATUS_CONTINUE_GAME;
}



// Whether Add String VOR
std::string VOR(bool ammunition)
{
    if(ammunition)
    {
        return " VOR";
    }
    return "";
}

int StudentWorld::move()
{
    vector<Actor*>::iterator i;
    i = Actors.begin();
    while (i != Actors.end())
    {
        if((*i)->getAlive())
        {
            (*i)->doSomething();
        }
        
        if(!(*i)->getAlive())
        {
            delete (*i);
            i = Actors.erase(i);
        }
        else
        {
            i ++;
        }
    }
    
    Avatars[0]->doSomething(); // Peach can't be dead
    Avatars[1]->doSomething(); // Yoshi can't be dead
    
    setGameStatText("P1 Roll: " + std::to_string(Avatars[0]->getDieRoll()) + " Stars: " + std::to_string(Avatars[0]->getStars()) + " $$: " + std::to_string(Avatars[0]->getCoins()) + VOR(Avatars[0]->getAmmunition()) + " | Time: " + std::to_string(this->timeRemaining()) + " | Bank: " + std::to_string(m_bank) + " | P2 Roll: " + std::to_string(Avatars[1]->getDieRoll()) + " Stars: " + std::to_string(Avatars[1]->getStars()) + " $$: "  + std::to_string(Avatars[1]->getCoins()) + VOR(Avatars[1]->getAmmunition()));
    
    if (timeRemaining() <= 0)
    {
        playSound(SOUND_GAME_FINISHED);
        if(Avatars[1]->getStars() > Avatars[0]->getStars()) // Yoshi wins for more stars
        {
            setFinalScore(Avatars[1]->getStars(), Avatars[1]->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else if(Avatars[1]->getStars() < Avatars[0]->getStars()) // Peach wins for more stars
        {
            setFinalScore(Avatars[0]->getStars(), Avatars[0]->getCoins());
            return GWSTATUS_PEACH_WON;
        }
        else // Yoshi and Peach have the same amount of stars
        {
            if(Avatars[1]->getCoins() > Avatars[0]->getCoins()) // Yoshi wins for more coins
            {
                setFinalScore(Avatars[1]->getStars(), Avatars[1]->getCoins());
                return GWSTATUS_YOSHI_WON;
            }
            else if(Avatars[1]->getCoins() < Avatars[0]->getCoins()) // Yoshi wins for more coins
            {
                setFinalScore(Avatars[0]->getStars(), Avatars[0]->getCoins());
                return GWSTATUS_PEACH_WON;
            }
            else // Game tied
            {
                time_t present_time;
                time(&present_time);
                srand(unsigned(present_time));
                if(rand() % 2) // randomly pick a winner
                {
                    setFinalScore(Avatars[1]->getStars(), Avatars[1]->getCoins());
                    return GWSTATUS_YOSHI_WON;
                }
                else
                {
                    setFinalScore(Avatars[0]->getStars(), Avatars[0]->getCoins());
                    return GWSTATUS_PEACH_WON;
                }
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

Board StudentWorld::getBoard() const
{
    return m_Board;
}

int StudentWorld::getBank() const
{
    return m_bank;
}

void StudentWorld::setBank(int bank)
{
    m_bank += bank;
}

std::vector <Avatar*>& StudentWorld::getAvatars()
{
    return Avatars;
}

std::vector <Actor*>& StudentWorld::getActors()
{
    return Actors;
}

