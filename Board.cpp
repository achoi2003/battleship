#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <vector>

#include <cassert>

using namespace std;

class BoardImpl
{
  public:
    BoardImpl(const Game& g);
    ~BoardImpl();
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

  private:
    const Game& m_game;
    char m_arr[MAXROWS][MAXCOLS];
    char* ships_placed; // index = shipId
};

BoardImpl::BoardImpl(const Game& g)
 : m_game(g)
{
    ships_placed = new char[g.nShips()];
    for(int i = 0; i < g.nShips(); i++)
        ships_placed[i] = ' ';
    
    clear();
}

BoardImpl::~BoardImpl()
{
    delete[] ships_placed;
}

void BoardImpl::clear()
{
    for(int r = 0; r < m_game.rows(); r++)
        for(int c = 0; c < m_game.cols(); c++)
            m_arr[r][c] = '.';
}

void BoardImpl::block()
{
    int count = (m_game.rows() * m_game.cols())/2;
    while(count > 0)
    {
        Point p = m_game.randomPoint();
        if(m_arr[p.r][p.c] != ' ')
        {
            m_arr[p.r][p.c] = ' ';
            count--;
        }
    }
}

void BoardImpl::unblock()
{
    for(int r = 0; r < m_game.rows(); r++)
        for(int c = 0; c < m_game.cols(); c++)
            if(m_arr[r][c] == ' ')
                m_arr[r][c] = '.';
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    // invalid shipId or point
    if(shipId < 0 || shipId >= m_game.nShips() || !m_game.isValid(topOrLeft))
        return false;
    
    // that shipId has already been placed
    if(ships_placed[shipId] != ' ')
        return false;
    
    // check if ship would overlap another ship or blocked position
    // or be partly/fully outside the board
    if(dir == VERTICAL)
    {
        if(topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
                return false;
        for(int r = topOrLeft.r, i = 0; i < m_game.shipLength(shipId); r++, i++)
        {
            if(m_arr[r][topOrLeft.c] != '.')
                return false;
        }
        for(int r = topOrLeft.r, i = 0; i < m_game.shipLength(shipId); r++, i++)
            m_arr[r][topOrLeft.c] = m_game.shipSymbol(shipId);
    }
    else if(dir == HORIZONTAL)
    {
        if(topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())
                return false;
        for(int c = topOrLeft.c, i = 0; i < m_game.shipLength(shipId); c++, i++)
        {
            if(m_arr[topOrLeft.r][c] != '.')
                return false;
        }
        for(int c = topOrLeft.c, i = 0; i < m_game.shipLength(shipId); c++, i++)
            m_arr[topOrLeft.r][c] = m_game.shipSymbol(shipId);
    }
    
    ships_placed[shipId] = m_game.shipSymbol(shipId);
    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    // invalid shipId or point
    if(shipId < 0 || shipId >= m_game.nShips() || !m_game.isValid(topOrLeft))
        return false;
    
    // that shipId has not been placed
    if(ships_placed[shipId] == ' ')
        return false;
    
    // check if board contains entire ship at indicated location
    if(dir == VERTICAL)
    {
        if(topOrLeft.r + m_game.shipLength(shipId) > m_game.rows())
                return false;
        for(int r = topOrLeft.r, i = 0; i < m_game.shipLength(shipId); r++, i++)
        {
            if(m_arr[r][topOrLeft.c] != m_game.shipSymbol(shipId))
                return false;
        }
        for(int r = topOrLeft.r, i = 0; i < m_game.shipLength(shipId); r++, i++)
            m_arr[r][topOrLeft.c] = '.';
    }
    else if(dir == HORIZONTAL)
    {
        if(topOrLeft.c + m_game.shipLength(shipId) > m_game.cols())
                return false;
        for(int c = topOrLeft.c, i = 0; i < m_game.shipLength(shipId); c++, i++)
        {
            if(m_arr[topOrLeft.r][c] != m_game.shipSymbol(shipId))
                return false;
        }
        for(int c = topOrLeft.c, i = 0; i < m_game.shipLength(shipId); c++, i++)
            m_arr[topOrLeft.r][c] = '.';
    }
    
    ships_placed[shipId] = ' ';
    return true;
}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for(int c = 0; c < m_game.cols(); c++)
        cout << c;
    cout << endl;
        
    for(int r = 0; r < m_game.rows(); r++)
    {
        cout << r << " ";
        for(int c = 0; c < m_game.cols(); c++)
        {
            if(shotsOnly && m_arr[r][c] != 'X' && m_arr[r][c] != '.' &&
               m_arr[r][c] != 'o')
                cout << '.';
            else
                cout << m_arr[r][c];
        }
        cout << endl;
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    char& coor = m_arr[p.r][p.c];
    
    shotHit = false;
    shipDestroyed = false;
    shipId = -1;
    
    // invalid point
    if(!m_game.isValid(p) || coor == 'X' || coor == 'o')
        return false;
    
    if(coor == '.')
        coor = 'o';
    else
    {
        shotHit = true;
        
        for(int c = 0; c < m_game.cols(); c++)
        {
            if(m_arr[p.r][c] == coor && c != p.c)
            {
                coor = 'X';
                return true;
            }
        }
        for(int r = 0; r < m_game.rows(); r++)
        {
            if(m_arr[r][p.c] == coor && r != p.r)
            {
                coor = 'X';
                return true;
            }
        }
        
        shipId = 0;
        for(; shipId < m_game.nShips(); shipId++)
        {
            if(ships_placed[shipId] == coor)
               break;
        }

        ships_placed[shipId] = ' ';
        shipDestroyed = true;
        coor = 'X';
    }
    return true;
}

bool BoardImpl::allShipsDestroyed() const
{
    for(int i = 0; i < m_game.nShips(); i++)
    {
        if(ships_placed[i] != ' ')
            return false;
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
