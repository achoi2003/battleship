#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>

using namespace std;

//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
  public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
 : Player(nm, g), m_lastCellAttacked(0, 0){}

bool AwfulPlayer::placeShips(Board& b)
{
      // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if ( ! b.placeShip(Point(k,0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */,
                                     bool /* shotHit */, bool /* shipDestroyed */,
                                     int /* shipId */)
{
    // AwfulPlayer completely ignores the result of any attack
    
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
    // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************

bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
  public:
    HumanPlayer(string nm, const Game& g);
    virtual bool isHuman() const;
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
};

HumanPlayer::HumanPlayer(string nm, const Game& g)
: Player(nm, g){}

bool HumanPlayer::isHuman() const {return true;}

bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " must place " << game().nShips() << " ships." << endl;
    
    for(int i = 0; i < game().nShips(); i++)
    {
        b.display(false);
        char input = ' ';
        Direction dir;
        
        while(input != 'h' && input != 'v')
        {
            cout << "Enter h or v for direction of " << game().shipName(i)
            << " (length " << game().shipLength(i) << "): ";
            cin >> input;
            cin.clear();
            cin.ignore(10000, '\n');
        }
        
        input == 'h' ? dir = HORIZONTAL : dir = VERTICAL;

        int r, c;
        cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
        getLineWithTwoIntegers(r, c);

        while(!b.placeShip(Point(r, c), i, dir))
        {
            cout << "The ship can not be placed there." << endl;
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            getLineWithTwoIntegers(r, c);
        }
    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    int r, c;
    cout << "Enter the row and column to attack (e.g., 3 5): ";
    while(!getLineWithTwoIntegers(r, c))
    {
        cout << "You must enter two integers." << endl;
        cout << "Enter the row and column to attack (e.g., 3 5): ";
    }
    
    if(!game().isValid(Point(r, c)))
        cout << name() << " wasted a shot at (" << r << "," << c << ").";

    return Point(r, c);
}

void HumanPlayer::recordAttackResult(Point p, bool validShot, bool shotHit,
                                bool shipDestroyed, int shipId){}

void HumanPlayer::recordAttackByOpponent(Point p){}

//*********************************************************************
//  MediocrePlayer
//*********************************************************************

class MediocrePlayer : public Player
{
  public:
    MediocrePlayer(string nm, const Game& g);
    bool mediocrePlacing(Board& b, int shipId, int depth);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
  private:
    int m_state;
    bool m_shotHit, m_shipDestroyed;
    Point m_point;
    char m_arr[MAXROWS][MAXCOLS];
};

MediocrePlayer::MediocrePlayer(string nm, const Game& g)
: Player(nm, g), m_state(1), m_shotHit(false), m_shipDestroyed(false)
{
    for(int r = 0; r < game().rows(); r++)
        for(int c = 0; c < game().cols(); c++)
            m_arr[r][c] = '.';
}

bool MediocrePlayer::mediocrePlacing(Board& b, int shipId, int depth)
{
    if(shipId >= game().nShips())
        return true;
    
    // limit on depth of recursion
    if(depth == 50)
        return false;
    
    for(int r = 0; r < game().rows(); r++)
    {
        for(int c = 0; c < game().cols(); c++)
        {
            if(b.placeShip(Point(r,c), shipId, HORIZONTAL) ||
               b.placeShip(Point(r,c), shipId, VERTICAL))
            {
                if(mediocrePlacing(b, shipId+1, depth+1))
                    return true;
                else
                {
                    b.unplaceShip(Point(r,c), shipId, HORIZONTAL);
                    b.unplaceShip(Point(r,c), shipId, VERTICAL);
                }
            }
        }
    }
    return false;
}

bool MediocrePlayer::placeShips(Board &b)
{
    b.block();
    if(mediocrePlacing(b, 0, 0))
    {
        b.unblock();
        return true;
    }
    b.unblock();
    return false;
}

Point MediocrePlayer::recommendAttack()
{
    Point p;
    bool exitLoop = false;
    while(!exitLoop)
    {
        switch(m_state)
        {
            case 1: {
                if(m_shotHit && !m_shipDestroyed)
                {
                    m_state = 2;
                    break;
                }
                else if(m_shotHit && m_shipDestroyed)
                    m_shipDestroyed = false;
                
                p = game().randomPoint();
                while(m_arr[p.r][p.c] != '.')
                    p = game().randomPoint();
                exitLoop = true;
                break;
            }
            case 2: {
                if(m_shipDestroyed)
                {
                    m_shipDestroyed = false;
                    m_shotHit = false;
                    m_state = 1;
                    break;
                }
                
                vector<Point> valid_points;
                
                for(int c = m_point.c-4; c <= m_point.c+4; c++)
                {
                    Point p(m_point.r, c);
                    if(game().isValid(p) && m_arr[p.r][p.c] == '.')
                        valid_points.push_back(p);
                }
                for(int r = m_point.r-4; r <= m_point.r+4; r++)
                {
                    Point p(r, m_point.c);
                    if(game().isValid(p) && m_arr[p.r][p.c] == '.')
                        valid_points.push_back(p);
                }
                
                if(valid_points.empty())
                {
                    m_shotHit = false;
                    m_shipDestroyed = false;
                    m_state = 1;
                    break;
                }
                
                return valid_points.at(randInt(valid_points.size()));
            }
        }
    }
    return p;
}

void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    m_point = p;
    m_shotHit = shotHit;
    m_shipDestroyed = shipDestroyed;
    
    if(validShot && shotHit)
        m_arr[p.r][p.c] = 'X';
    else if(validShot)
        m_arr[p.r][p.c] = 'o';
}

void MediocrePlayer::recordAttackByOpponent(Point p) {}

//*********************************************************************
//  GoodPlayer
//*********************************************************************

class GoodPlayer : public Player
{
  public:
    GoodPlayer(string nm, const Game& g);
    virtual ~GoodPlayer();
    bool goodPlacing(Board& b, int shipId, int depth);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
                                                bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    
    
    void generateDensity();
    void clearDensity();
    Point findGreatest(bool searchAll);
    
  private:
    int m_state;
    bool m_shotHit, m_shipDestroyed;
    Point m_point;
    char m_arr[MAXROWS][MAXCOLS];

    int* ship_sizes;
    int density_arr[MAXROWS][MAXCOLS];
};

GoodPlayer::GoodPlayer(string nm, const Game& g)
: Player(nm, g), m_state(1), m_shotHit(false), m_shipDestroyed(false)
{
    clearDensity();
    ship_sizes = new int[game().nShips()];
    for(int i = 0; i < game().nShips(); i++)
        ship_sizes[i] = 0;
    for(int r = 0; r < game().rows(); r++)
        for(int c = 0; c < game().cols(); c++)
            m_arr[r][c] = '.';
}

GoodPlayer::~GoodPlayer() {delete[] ship_sizes;}

bool GoodPlayer::goodPlacing(Board& b, int shipId, int depth)
{
    if(shipId >= game().nShips())
        return true;
    
    // limit on depth of recursion
    if(depth == 50)
        return false;
    
    for(int r = 0; r < game().rows(); r++)
    {
        for(int c = 0; c < game().cols(); c++)
        {
            if(b.placeShip(Point(r,c), shipId, HORIZONTAL) ||
               b.placeShip(Point(r,c), shipId, VERTICAL))
            {
                if(goodPlacing(b, shipId+1, depth+1))
                {
                    ship_sizes[shipId] = game().shipLength(shipId);
                    return true;
                }
                else
                {
                    b.unplaceShip(Point(r,c), shipId, HORIZONTAL);
                    b.unplaceShip(Point(r,c), shipId, VERTICAL);
                }
            }
        }
    }
    return false;
}

void GoodPlayer::generateDensity()
{
    clearDensity();
    for(int i = 0; i < game().nShips(); i++)
    {
        bool canBePlaced;
        int size = ship_sizes[i];
        if(size == 0)
            continue;
        
        const int nGameRows = game().rows();
        const int nGameCols = game().cols();
        
        for(int r = 0; r < nGameRows; r++)
        {
            for(int c = 0; c < nGameCols; c++)
            {
                canBePlaced = true;
                // try placing vertically
                if(r + size <= game().rows())
                {
                    for(int r2 = r, i = 0; i < size; r2++, i++)
                    {
                        if(m_arr[r2][c] == 'o')
                        {
                            canBePlaced = false;
                            break;
                        }
                    }
                    if(canBePlaced)
                    {
                        for(int r2 = r, j = 0; j < size; r2++, j++)
                        {
                            density_arr[r2][c]++;
                        }
                    }
                }
                canBePlaced = true;
                if(c + size <= nGameCols)
                {
                    for(int c2 = c, i = 0; i < size; c2++, i++)
                    {
                        if(m_arr[r][c2] == 'o')
                        {
                            canBePlaced = false;
                            break;
                        }
                    }
                    if(canBePlaced)
                    {
                        for(int c2 = c, j = 0; j < size; c2++, j++)
                        {
                            density_arr[r][c2]++;
                        }
                    }
                }
            }
        }
    }
}

void GoodPlayer::clearDensity()
{
    for(int r = 0; r < MAXROWS; r++)
    {
        for(int c = 0; c < MAXCOLS; c++)
            density_arr[r][c] = 0;
    }
}

Point GoodPlayer::findGreatest(bool searchAll = true)
{
    int bigr = 0, bigc = 0;
    
    if(searchAll)
    {
        for(int r = 0; r < MAXROWS; r++)
        {
            for(int c = 0; c < MAXCOLS; c++)
            {
                if(density_arr[r][c] > density_arr[bigr][bigc]
                   && m_arr[r][c] != 'X' && m_arr[r][c] != 'o')
                {
                    bigr = r;
                    bigc = c;
                }
            }
        }
    }
    else
    {
        bool noneFound = true;
        if(noneFound)
        {
            for(int c = m_point.c-4; c <= m_point.c+4; c++)
            {
                Point p(m_point.r, c);
                if(game().isValid(p) && m_arr[p.r][p.c] == '.')
                {
                    bigr = p.r;
                    bigc = p.c;
                    noneFound = false;
                    break;
                }
            }
        }
        if(noneFound)
        {
            for(int r = m_point.r-4; r <= m_point.r+4; r++)
            {
                Point p(r, m_point.c);
                if(game().isValid(p) && m_arr[p.r][p.c] == '.')
                {
                    bigr = p.r;
                    bigc = p.c;
                    noneFound = false;
                    break;
                }
            }
        }
        if(noneFound)
            return Point(-1,-1);

        for(int c = m_point.c-4; c <= m_point.c+4; c++)
        {
            Point p(m_point.r, c);
            if(game().isValid(p) && m_arr[p.r][p.c] == '.')
            {
                if(density_arr[p.r][p.c] > density_arr[bigr][bigc])
                {
                    bigr = p.r;
                    bigc = p.c;
                }
            }
        }
        for(int r = m_point.r-4; r <= m_point.r+4; r++)
        {
            Point p(r, m_point.c);
            if(game().isValid(p) && m_arr[p.r][p.c] == '.')
            {
                if(density_arr[p.r][p.c] > density_arr[bigr][bigc])
                {
                    bigr = p.r;
                    bigc = p.c;
                }
            }
        }
    }
    return Point(bigr, bigc);
}

bool GoodPlayer::placeShips(Board &b)
{
    b.block();
    if(goodPlacing(b, 0, 0))
    {
        b.unblock();
        return true;
    }
    b.unblock();
    return false;
}

Point GoodPlayer::recommendAttack()
{
    Point p;
    bool exitLoop = false;
    while(!exitLoop)
    {
        generateDensity();
        switch(m_state)
        {
            case 1: {
                if(m_shotHit && !m_shipDestroyed)
                {
                    m_state = 2;
                    break;
                }
                else if(m_shotHit && m_shipDestroyed)
                    m_shipDestroyed = false;
                
                p = findGreatest();
                exitLoop = true;
                break;
            }
            case 2: {
                if(m_shipDestroyed)
                {
                    m_shipDestroyed = false;
                    m_shotHit = false;
                    m_state = 1;
                    break;
                }
                
                p = findGreatest(true);
                
                if(p.r == -1 || p.c == -1)
                {
                    m_shotHit = false;
                    m_shipDestroyed = false;
                    m_state = 1;
                    break;
                }
                
                exitLoop = true;
                break;
            }
        }
    }
    return p;
}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{
    m_point = p;
    m_shotHit = shotHit;
    m_shipDestroyed = shipDestroyed;
    
    if(shipId < game().nShips() && shipId >= 0)
        ship_sizes[shipId] = 0;
    
    if(validShot && shotHit)
        m_arr[p.r][p.c] = 'X';
    else if(validShot)
        m_arr[p.r][p.c] = 'o';
}

void GoodPlayer::recordAttackByOpponent(Point p) {}


//*********************************************************************
//  createPlayer
//*********************************************************************

Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };
    
    int pos;
    for (pos = 0; pos != sizeof(types)/sizeof(types[0])  &&
                                                     type != types[pos]; pos++)
        ;
    switch (pos)
    {
      case 0:  return new HumanPlayer(nm, g);
      case 1:  return new AwfulPlayer(nm, g);
      case 2:  return new MediocrePlayer(nm, g);
      case 3:  return new GoodPlayer(nm, g);
      default: return nullptr;
    }
}
