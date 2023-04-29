#include "Game.h"
#include "Player.h"

#include "Board.h"

#include <iostream>
#include <string>
using namespace std;

bool addStandardShips(Game& g)
{
    return g.addShip(5, 'A', "aircraft carrier")  &&
           g.addShip(4, 'B', "battleship")  &&
           g.addShip(3, 'D', "destroyer")  &&
           g.addShip(3, 'S', "submarine")  &&
           g.addShip(2, 'P', "patrol boat");
}

int main()
{
    const int NTRIALS = 100;

    cout << "Select one of these choices for an example of the game:" << endl;
    cout << "  1.  A mini-game between two mediocre players" << endl;
    cout << "  2.  A mediocre player against a human player" << endl;
    cout << "  3.  A " << NTRIALS
         << "-game match between a good and a mediocre player, with no pauses"
         << endl;
    cout << "Enter your choice: ";
    string line;
    getline(cin,line);
    if (line.empty())
    {
        cout << "You did not enter a choice" << endl;
    }
    else if (line[0] == '1')
    {
        Game g(2, 3);
        g.addShip(2, 'R', "rowboat");
        Player* p1 = createPlayer("mediocre", "Popeye", g);
        Player* p2 = createPlayer("mediocre", "Bluto", g);
        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '2')
    {
        Game g(10, 10);
        addStandardShips(g);
        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
        Player* p2 = createPlayer("human", "Shuman the Human", g);
        g.play(p1, p2);
        delete p1;
        delete p2;
    }
    else if (line[0] == '3')
    {
        float nGoodWins = 0;
//        int nMediocreWins = 0;

        for (int k = 1; k <= NTRIALS; k++)
        {
            cout << "============================= Game " << k
                 << " =============================" << endl;
            Game g(10, 10);
            addStandardShips(g);
            Player* p1 = createPlayer("good", "Good Gary", g); //
            Player* p2 = createPlayer("mediocre", "Mediocre Mimi", g);
            Player* winner = (k % 2 == 1 ?
                                g.play(p1, p2, false) : g.play(p2, p1, false));
            if (winner == p1)
                nGoodWins++;
            delete p1;
            delete p2;
        }
        cout << "The good player won " << (nGoodWins*100/NTRIALS) << "% of games." << endl;
          // We'd expect a mediocre player to win most of the games against
          // an awful player.  Similarly, a good player should outperform
          // a mediocre player.
    }
    else
    {
       cout << "That's not one of the choices." << endl;
    }
}






//
//
////#include "Game.h"
////#include "Player.h"
////#include <iostream>
////#include <string>
////
////// delete
////#include "Board.h"
////
////using namespace std;
////
////bool addStandardShips(Game& g)
////{
////    return g.addShip(5, 'A', "aircraft carrier")  &&
////           g.addShip(4, 'B', "battleship")  &&
////           g.addShip(3, 'D', "destroyer")  &&
////           g.addShip(3, 'S', "submarine")  &&
////           g.addShip(2, 'P', "patrol boat");
////}
////
//////int main()
//////{
//////    Game g(10, 10);
//////    addStandardShips(g);
//////    Board b(g);
//////
//////    for (int k = 0; k < g.nShips(); k++)
//////        b.placeShip(Point(k,0), k, HORIZONTAL);
//////
//////    b.display(true);
//////
////////    for (int k = 0; k < g.nShips(); k++)
////////        b.unplaceShip(Point(k,0), k, HORIZONTAL);
////////    b.display(true);
////////    assert(b.allShipsDestroyed() == true);
//////
//////    bool shotHit, shipDestroyed;
//////    int shipId = 10;
//////
//////    assert(b.attack(Point(10,10), shotHit, shipDestroyed, shipId) == false);
//////    assert(b.attack(Point(-1,5), shotHit, shipDestroyed, shipId) == false);
//////    assert(b.attack(Point(5,5), shotHit, shipDestroyed, shipId) == true
//////           && shotHit == false && shipDestroyed == false && shipId == 10);
//////    assert(b.attack(Point(2,0), shotHit, shipDestroyed, shipId) == true); assert(shotHit == true); assert(shipDestroyed == false); assert(shipId == 10);
//////    assert(b.attack(Point(3,0), shotHit, shipDestroyed, shipId) == true
//////           && shotHit == true && shipDestroyed == false && shipId == 10);
//////    assert(b.attack(Point(4,0), shotHit, shipDestroyed, shipId) == true
//////           && shotHit == true && shipDestroyed == false && shipId == 10);
//////    assert(b.attack(Point(4,1), shotHit, shipDestroyed, shipId) == true
//////           && shotHit == true && shipDestroyed == true && shipId == 4);
//////
//////    b.attack(Point(0,0), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(0,1), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(0,2), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(0,3), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(0,4), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(1,0), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(1,1), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(1,2), shotHit, shipDestroyed, shipId);
//////    b.display(true);
//////    b.display(false);
//////    b.attack(Point(1,3), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(2,2), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(2,1), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(2,0), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(2,3), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(3,0), shotHit, shipDestroyed, shipId);
//////    b.attack(Point(3,1), shotHit, shipDestroyed, shipId);
//////    assert(b.attack(Point(3,2), shotHit, shipDestroyed, shipId) == true);
//////    assert(b.attack(Point(3,2), shotHit, shipDestroyed, shipId) == false);
//////    assert(b.allShipsDestroyed() == true);
//////
//////    cout << "success" << endl;
//////
//////}
////
////int main()
////{
//////    const int NTRIALS = 10;
//////
//////    cout << "Select one of these choices for an example of the game:" << endl;
//////    cout << "  1.  A mini-game between two mediocre players" << endl;
//////    cout << "  2.  A mediocre player against a human player" << endl;
//////    cout << "  3.  A " << NTRIALS
//////         << "-game match between a mediocre and an awful player, with no pauses"
//////         << endl;
//////    cout << "Enter your choice: ";
//////    string line;
//////    getline(cin,line);
//////    if (line.empty())
//////    {
//////        cout << "You did not enter a choice" << endl;
//////    }
//////    else if (line[0] == '1')
//////    {
//////        Game g(2, 3);
//////        g.addShip(2, 'R', "rowboat");
//////        Player* p1 = createPlayer("mediocre", "Popeye", g);
//////        Player* p2 = createPlayer("mediocre", "Bluto", g);
//////        cout << "This mini-game has one ship, a 2-segment rowboat." << endl;
//////        g.play(p1, p2);
//////        delete p1;
//////        delete p2;
//////    }
//////    else if (line[0] == '2')
//////    {
//////        Game g(10, 10);
//////        addStandardShips(g);
//////        Player* p1 = createPlayer("mediocre", "Mediocre Midori", g);
//////        Player* p2 = createPlayer("human", "Shuman the Human", g);
//////        g.play(p1, p2);
//////        delete p1;
//////        delete p2;
//////    }
//////    else if (line[0] == '3')
//////    {
//////        int nMediocreWins = 0;
////
//////        for (int k = 1; k <= NTRIALS; k++)
//////        {
//////            cout << "============================= Game " << k
//////                 << " =============================" << endl;
////            Game g(10, 10);
////            addStandardShips(g);
////
//////    Player* p1 = createPlayer("mediocre", "Alex", g);
//////    Player* p2 = createPlayer("mediocre", "Brian", g);
//////
//////    Board b1(g);
//////    Board b2(g);
//////
//////    cout << "b1 board before: " << endl;
//////    b1.display(false);
//////    p1->placeShips(b1);
//////    cout << "b1 board afer: " << endl;
//////    b1.display(false);
////
////
//////    p2->placeShips(b2);
////
////
//////    Point point2 = p2->recommendAttack();
//////    bool shotHit = true, shipDestroyed = true;
//////    int shipId = 0;
//////    b1.attack(point2, shotHit, shipDestroyed, shipId = 10);
//////    b1.display(false);
////
//////            Player* p1 = createPlayer("human", "Shuman", g);
//////            Player* p2 = createPlayer("awful", "Mediocre Mimi", g);
//////
//////            g.play(p1, p2, false);
////////            Player* winner = (k % 2 == 1 ?
////////                                g.play(p1, p2, false) : g.play(p2, p1, false));
////////            if (winner == p2)
////////                nMediocreWins++;
//////            delete p1;
//////            delete p2;
////
//////        }
//////        cout << "The mediocre player won " << nMediocreWins << " out of "
//////             << NTRIALS << " games." << endl;
////          // We'd expect a mediocre player to win most of the games against
////          // an awful player.  Similarly, a good player should outperform
////          // a mediocre player.
//////    }
//////    else
//////    {
//////       cout << "That's not one of the choices." << endl;
//////    }
////}
