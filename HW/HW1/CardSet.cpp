#include "CardSet.h"
#include "Set.h"
#include <iostream>

CardSet::CardSet()
    :m_CardSet()
{
}

bool CardSet::add(unsigned long cardNumber)
{
    if(m_CardSet.insert(cardNumber))
    {
        return true;
    }
    return false;
}

int CardSet::size() const
{
    return m_CardSet.size();
}

void CardSet::print() const
{
    for(int i = 0; i < this->size(); i++)
    {
        ItemType tempValue;
        this->m_CardSet.get(i, tempValue);
        std::cout << tempValue << std::endl;
    }
}

