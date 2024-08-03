#include <stdio.h>
#include <iostream>
#include "Set.h"
#include "CardSet.h"
#include <cassert>

int main()
{
    CardSet cs1;
    assert(cs1.size() == 0);
    assert(cs1.add(123456789));
    assert(cs1.size() == 1);
    assert(!cs1.add(123456789));
    assert(cs1.size() == 1);
    assert(cs1.add(223456789));
    assert(cs1.add(323456789));
    assert(cs1.add(423456789));
    assert(cs1.add(523456789));
    assert(cs1.add(623456789));
    assert(cs1.add(234567890));
    assert(cs1.add(134567890));
    assert(cs1.size() == 8);
    std::cerr << "All cases passed" << std::endl;
    cs1.print();
    
    return 0;
}

