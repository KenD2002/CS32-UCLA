#include "newSet.h"
#include <cstdlib>
#include <iostream>
#include <string>

Set::Set(int max)
    :m_size(0), m_max(max)
{
    if(max < 0)
    {
        std::exit(1);
    }
    this->m_arr = new ItemType[max];
}

Set::~Set()
{
    delete [] m_arr;
}

Set::Set(const Set& other)
{
    this->m_max = other.m_max;
    this->m_size = other.m_size;
    this->m_arr = new ItemType [other.m_max];
    
    for(size_t i = 0; i < other.m_size; i++)
    {
        this->m_arr[i] = other.m_arr[i];
    }
}

Set& Set::operator=(const Set& other)
{
    if(&other != this)
    {
        delete [] this->m_arr;
        
        this->m_arr = new ItemType[other.m_max];
        this->m_size = other.m_size;
        this->m_max = other.m_max;
        
        for(size_t i = 0; i < other.m_size; i++)
        {
            this->m_arr[i] = other.m_arr[i];
        }
    }
    return *this;
}

bool Set::empty() const
{
    if(m_size == 0)
    {
        return true;
    }
    return false;
}

int Set::size() const
{
    return this->m_size;
}

bool Set::insert(const ItemType& value)
{
    if(contains(value) || this->m_size == m_max)
    {
        return false;
    }
    
    this->m_size++;
    this->m_arr[m_size - 1] = value;
    for(size_t i = 0; i < m_size; i++)
    {
        if(m_arr[i] > m_arr[m_size - 1])
        {
            ItemType tempValue = m_arr[i];
            m_arr[i] = m_arr[m_size - 1];
            m_arr[m_size - 1] = tempValue;
        }
    }
    return true;
}

bool Set::erase(const ItemType& value)
{
    if(!contains(value))
    {
        return false;
    }
    //The if statement above guarantees that value is included in m_arr.
    //Case 1: value appears at position m_size - 1, and this case covers the situation that m_size = 1:
    if(m_arr[m_size] == value)
    {
        m_size--;
        return true;
    }
    //Case 2: value does not appear at the last position.
    for(size_t i = 0; i < this->m_size - 1; i++)
    {
        if(m_arr[i] == value)
        {
            m_arr[i] = m_arr[i + 1];
            m_arr[i + 1] = value;
        }
    }
    m_size--;
    return true;
}

bool Set::contains(const ItemType& value) const
{
    for(size_t i = 0; i < this->m_size; i++)
    {
        if(m_arr[i] == value)
        {
            return true;
        }
    }
    return false;
}

bool Set::get(int i, ItemType &value) const
{
    if (i < 0 || i >= m_size)
        return false;

    value = m_arr[i];
    return true;
}

/*
// previous version of get, used bobble sort. Modified insert so dont have to sort when call get.
bool Set::get(int i, ItemType& value) const
{
    if(0 <= i && i < this->size())
    {
        ItemType* tempArr;
        tempArr = new ItemType[m_max];
        for(int j = 0; j < this->m_size; j++)
        {
            tempArr[j] = this->m_arr[j];
        }//copy this->m_arr into tempArr.
        for(int k = 0; k < this->m_size; k++)
        {
            for(int m = 0; m < this->m_size - 1; m++)
            {
                if(tempArr[m] > tempArr[m+1])
                {
                    ItemType tempItem = tempArr[m];
                    tempArr[m] = tempArr[m + 1];
                    tempArr[m + 1] = tempItem;
                }
            }
        }//bobble sort for tempArr.
        value = tempArr[i];
        return true;
    }
    return false;
}
 */

void Set::swap(Set& other)
{
    int tempSize = other.m_size;
    other.m_size = this->m_size;
    this->m_size = tempSize;
    
    int tempMax = other.m_max;
    other.m_max = this->m_max;
    this->m_max = tempMax;
    
    ItemType* tempm_arr = this->m_arr;
    this->m_arr = other.m_arr;
    other.m_arr = tempm_arr;
}
