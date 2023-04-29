#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>
#include <vector>

using namespace std;

class GameImpl
{
  public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);
  private:
    struct Ship
    {
        Ship(int length, char symbol, string name);
        int m_length;
        char m_symbol;
        string m_name;
    };
    
    int m_rows;
    int m_cols;
    vector<Ship> ship_vector; // shipId = index
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
: m_rows(nRows), m_cols(nCols){}

// guaranteed valid parameters
GameImpl::Ship::Ship(int length, char symbol, string name)
: m_length(length), m_symbol(symbol), m_name(name){}

int GameImpl::rows() const
{
    return m_rows;
}

int GameImpl::cols() const
{
    return m_cols;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0  &&  p.r < rows()  &&  p.c >= 0  &&  p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    ship_vector.push_back(Ship(length, symbol, name));
    return true;
}

int GameImpl::nShips() const
{
    return ship_vector.size();
}

int GameImpl::shipLength(int shipId) const
{
    return ship_vector.at(shipId).m_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return ship_vector.at(shipId).m_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return ship_vector.at(shipId).m_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause = true)
{
    if(!p1->placeShips(b1) || !p2->placeShips(b2))
        return nullptr;

    bool shotHit = false, shipDestroyed = false, validShot = false,
    p1Win = false, p2Win = false;
    
    int shipId = -1;
    
    // game starts!
    while(!p1Win && !p2Win)
    {
        cout << p1->name() << "'s turn.  Board for " << p2->name() << ":" << endl;
        p1->isHuman() ? b2.display(true) : b2.display(false);
        Point point1 = p1->recommendAttack();
        validShot = b2.attack(point1, shotHit, shipDestroyed, shipId);
        p1->recordAttackResult(point1, validShot, shotHit, shipDestroyed, shipId);
        
        if(validShot)
        {
            cout << p1->name() << " attacked (" << point1.r << "," << point1.c
                                << ") ";
            if(shipDestroyed)
            {
                cout << "and destroyed the " << shipName(shipId);
            }
            else if(shotHit)
                cout << "and hit something";
            else
                cout << "and missed";
            
            cout << ", resulting in: " << endl;
        }
        
        p1->isHuman() ? b2.display(true) : b2.display(false);
        p1Win = b2.allShipsDestroyed();
        if(p1Win) {break;}
        if(shouldPause) {waitForEnter();}
        
        cout << p2->name() << "'s turn.  Board for " << p1->name() << ":" << endl;
        p2->isHuman() ? b1.display(true) : b1.display(false);
        Point point2 = p2->recommendAttack();
        validShot = b1.attack(point2, shotHit, shipDestroyed, shipId);
        p2->recordAttackResult(point2, validShot, shotHit, shipDestroyed, shipId);
        
        if(validShot)
        {
            cout << p2->name() << " attacked (" << point2.r << "," << point2.c
                            << ") ";
            if(shipDestroyed)
            {
                cout << "and destroyed the " << shipName(shipId);
            }
            else if(shotHit)
                cout << "and hit something";
            else
                cout << "and missed";
            
            cout << ", resulting in: " << endl;
        }
        
        p2->isHuman() ? b1.display(true) : b1.display(false);
        p2Win = b1.allShipsDestroyed();
        if(shouldPause) {waitForEnter();}
    }
    
    if(p1Win)
    {
        cout << p1->name() << " wins!" << endl;
        if(p2->isHuman())
            b1.display(false);
        return p1;
    }
    else if(p2Win)
    {
        cout << p2->name() << " wins!" << endl;
        if(p1->isHuman())
            b2.display(false);
        return p2;
    }
    return nullptr;
}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1  ||  nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1  ||  nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows()  &&  length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
             << endl;
        return false;
    }
    if (!isascii(symbol)  ||  !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
             << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X'  ||  symbol == '.'  ||  symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
             << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                 << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0  &&  shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr  ||  p2 == nullptr  ||  nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

