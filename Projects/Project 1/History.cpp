#include "History.h"
#include "Arena.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols):
    m_rows(nRows), m_cols(nCols)
{
    for(int i = 0; i < MAXROWS; i ++)
    {
        for(int j = 0; j < MAXCOLS; j ++)
        {
            m_gridRecords[i][j] = '!';
        }
    }
    for(int i = 0; i < m_rows; i ++)
    {
        for(int j = 0; j < m_cols; j ++)
        {
            m_gridRecords[i][j] = 'A' - 1;
        }
    }
}


bool History::record(int i, int j)
{
    i--;
    j--;
    if(i < 0 || j < 0 || i >= MAXROWS || j >= MAXCOLS)
    {
        return false;
    }
    else
    {
        if(m_gridRecords[i][j] != 'Z')
        {
            m_gridRecords[i][j] ++;
        }
        return true;
    }
}

void History::display() const
{
    clearScreen();
    for(int i = 0; i < m_rows; i ++)
    {
        for(int j = 0; j < m_cols; j ++)
        {
            if(m_gridRecords[i][j] == '!')
            {
            }
            else if(m_gridRecords[i][j] == 'A' - 1)
            {
                std::cout << '.';
            }
            else
            {
                std::cout << m_gridRecords[i][j];
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
