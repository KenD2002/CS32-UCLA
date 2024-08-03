#include <iostream>
#include <string.h>
#include <stack>

class Coord
        {
          public:
            Coord(int r, int c) : m_row(r), m_col(c) {}
            int r() const { return m_row; }
            int c() const { return m_col; }
          private:
            int m_row;
            int m_col;
        };


bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    std::stack<Coord> CoordStack;
    CoordStack.push(Coord(sr, sc));
    maze[sr][sc] = 'a';
    while (!CoordStack.empty())
    {
        Coord curCoord = CoordStack.top();
        int curR = curCoord.r();
        int curC = curCoord.c();
        CoordStack.pop();
        if(curR == er && curC == ec)
        {
            return true;
        }
        if(maze[curR][curC + 1] == '.') // Move East?
        {
            Coord newCoord(curR, curC + 1);
            CoordStack.push(newCoord);
            maze[curR][curC + 1] = 'a';
        }
        if(maze[curR - 1][curC] == '.') // Move North?
        {
            Coord newCoord(curR - 1, curC);
            CoordStack.push(newCoord);
            maze[curR - 1][curC] = 'a';
        }
        if(maze[curR][curC - 1] == '.') // Move West?
        {
            Coord newCoord(curR, curC - 1);
            CoordStack.push(newCoord);
            maze[curR][curC - 1] = 'a';
        }
        if(maze[curR + 1][curC] == '.') // Move South?
        {
            Coord newCoord(curR + 1, curC);
            CoordStack.push(newCoord);
            maze[curR + 1][curC] = 'a';
        }
    }
    
    return false;
}
