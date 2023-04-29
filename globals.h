#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

#include <random>

//
#include <chrono>

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double,std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};
//

const int MAXROWS = 10;
const int MAXCOLS = 10;

enum Direction {
    HORIZONTAL, VERTICAL
};

class Point
{
  public:
    Point() : r(0), c(0) {}
    Point(int rr, int cc) : r(rr), c(cc) {}
    int r;
    int c;
};

  // Return a uniformly distributed random int from 0 to limit-1
inline int randInt(int limit)
{
    static std::random_device rd;
    static std::mt19937 generator(rd());
    if (limit < 1)
        limit = 1;
    std::uniform_int_distribution<> distro(0, limit-1);
    return distro(generator);
}

#endif // GLOBALS_INCLUDED
