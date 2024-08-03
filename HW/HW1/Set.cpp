#include "Set.h"
#include <iostream>
#include <string>

Set::Set()
    :m_size(0)
{
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
    if(contains(value) || this->m_size == DEFAULT_MAX_ITEMS)
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

bool Set::get(int i, ItemType& value) const
{
    if(0 <= i && i < this->size())
    {
        ItemType tempArr[DEFAULT_MAX_ITEMS];
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

void Set::swap(Set& other)
{
    int numLarger = 0;
    if(this->size() <= other.size())
    {
        numLarger = other.size();
    }
    else
    {
        numLarger = this->size();
    }
    
    for(int i = 0; i < numLarger; i++)
    {
        ItemType tempElement = this->m_arr[i];
        this->m_arr[i] = other.m_arr[i];
        other.m_arr[i] = tempElement;
    }
    
    int tempSize = other.m_size;
    other.m_size = this->m_size;
    this->m_size = tempSize;
}

