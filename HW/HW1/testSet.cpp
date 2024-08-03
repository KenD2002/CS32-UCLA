#include <iostream>
#include <string>
#include <cassert>
#include "Set.h"

int main()
{
 
    /*
    //Use the following testing codes only if changed ItemType into unsigned long.
    Set s;
    assert(s.empty());
    ItemType x = 9876543;
    assert( !s.get(42, x)  &&  x == 9876543); // x unchanged by get failure
    s.insert(123456789);
    assert(s.size() == 1);
    assert(s.get(0, x)  &&  x == 123456789);
    Set nset1;
    Set nset2;
    assert(nset1.empty() == 1);
    assert(nset1.size() == 0);
    assert(nset1.insert(1) == 1);
    assert(nset1.insert(2) == 1);
    assert(nset1.insert(3) == 1);
    assert(nset1.insert(4) == 1);
    assert(nset1.insert(5) == 1);
    assert(nset1.insert(6) == 1);
    assert(nset1.insert(7) == 1);
    assert(nset1.insert(8) == 1);
    assert(nset1.insert(1) == 0);
    assert(nset1.insert(5) == 0);
    assert(nset1.insert(8) == 0);
    assert(nset1.size() == 8);
    assert(nset1.contains(9) == 0);
    assert(nset1.contains(1) == 1);
    assert(nset1.contains(5) == 1);
    assert(nset1.contains(8) == 1);
    for(int i = 0; i < nset1.size(); i++)
    {
        ItemType element;
        nset1.get(i, element);
        std::cerr << element << " ";
    }
    std::cerr << std::endl;
    assert(nset1.erase(9) == 0);
    assert(nset1.erase(1) == 1);
    assert(nset1.erase(5) == 1);
    assert(nset1.erase(8) == 1);
    assert(nset1.contains(1) == 0);
    assert(nset1.contains(5) == 0);
    assert(nset1.contains(8) == 0);
    assert(nset1.erase(1) == 0);
    assert(nset1.erase(5) == 0);
    assert(nset1.erase(8) == 0);
    assert(nset1.size() == 5);
    
    for(int i = 0; i < nset1.size(); i++)
    {
        ItemType element;
        nset1.get(i, element);
        std::cerr << element << " ";
    }
    std::cerr << std::endl;
    */

    
    //Use the following testing codes only if changed ItemType into std::string.
    Set s;
    assert(s.empty());
    ItemType x1 = "arepa";
    assert( !s.get(42, x1)  &&  x1 == "arepa"); // x unchanged by get failure
    s.insert("chapati");
    assert(s.size() == 1);
    assert(s.get(0, x1)  &&  x1 == "chapati");
    
    Set ss;
    ss.insert("lavash");
    ss.insert("roti");
    ss.insert("chapati");
    ss.insert("injera");
    ss.insert("roti");
    ss.insert("matzo");
    ss.insert("injera");
    assert(ss.size() == 5);  // duplicate "roti" and "injera" were not added
    ItemType x;
    ss.get(0, x);
    assert(x == "chapati");  // "chapati" is greater than exactly 0 items in ss
    ss.get(4, x);
    assert(x == "roti");  // "roti" is greater than exactly 4 items in ss
    ss.get(2, x);
    assert(x == "lavash");  // "lavash" is greater than exactly 2 items in ss
    
    Set ss1;
    ss1.insert("laobing");
    Set ss2;
    ss2.insert("matzo");
    ss2.insert("pita");
    ss1.swap(ss2);
    assert(ss1.size() == 2  &&  ss1.contains("matzo")  &&  ss1.contains("pita")  &&
           ss2.size() == 1  &&  ss2.contains("laobing"));
    
    Set ss3;
    ss3.insert("dosa");
    assert(!ss3.contains(""));
    ss3.insert("tortilla");
    ss3.insert("");
    ss3.insert("focaccia");
    assert(ss3.contains(""));
    ss3.erase("dosa");
    assert(ss3.size() == 3  &&  ss3.contains("focaccia")  &&  ss3.contains("tortilla")  &&
           ss3.contains(""));
    ItemType v;
    assert(ss3.get(1, v)  &&  v == "focaccia");
    assert(ss3.get(0, v)  &&  v == "");
    
    
    Set set1;
    Set set2;
    assert(set1.empty() == 1);
    assert(set1.size() == 0);
    assert(set1.insert("1") == 1);
    assert(set1.insert("2") == 1);
    assert(set1.insert("3") == 1);
    assert(set1.insert("4") == 1);
    assert(set1.insert("5") == 1);
    assert(set1.insert("6") == 1);
    assert(set1.insert("7") == 1);
    assert(set1.insert("8") == 1);
    assert(set1.insert("1") == 0);
    assert(set1.insert("5") == 0);
    assert(set1.insert("8") == 0);
    assert(set1.size() == 8);
    assert(set1.contains("9") == 0);
    assert(set1.contains("1") == 1);
    assert(set1.contains("5") == 1);
    assert(set1.contains("8") == 1);
    for(int i = 0; i < set1.size(); i++)
    {
        ItemType element;
        set1.get(i, element);
        std::cerr << element << " ";
    }
    std::cerr << std::endl;
    assert(set1.erase("9") == 0);
    assert(set1.erase("1") == 1);
    assert(set1.erase("5") == 1);
    assert(set1.erase("8") == 1);
    assert(set1.contains("1") == 0);
    assert(set1.contains("5") == 0);
    assert(set1.contains("8") == 0);
    assert(set1.erase("1") == 0);
    assert(set1.erase("5") == 0);
    assert(set1.erase("8") == 0);
    assert(set1.size() == 5);
    
    for(int i = 0; i < set1.size(); i++)
    {
        ItemType element;
        set1.get(i, element);
        std::cerr << element << " ";
    }
    std::cerr << std::endl;
    
    assert(set2.insert("10") == 1);
    assert(set2.insert("11") == 1);
    assert(set2.insert("12") == 1);
    assert(set2.insert("13") == 1);
    assert(set2.insert("14") == 1);
    assert(set2.insert("15") == 1);
    assert(set2.size() == 6);
    for(int i = 0; i < set2.size(); i++)
    {
        ItemType element;
        set2.get(i, element);
        std::cerr << element << " ";
    }
    std::cerr << std::endl;
    set1.swap(set2);
    for(int i = 0; i < set1.size(); i++)
    {
        ItemType element;
        set1.get(i, element);
        std::cerr << element << " ";
    }
    std::cerr << std::endl;
    for(int i = 0; i < set2.size(); i++)
    {
        ItemType element;
        set2.get(i, element);
        std::cerr << element << " ";
    }
    std::cerr << std::endl;
    
    Set set3;
    for(int i = DEFAULT_MAX_ITEMS; i > 0; i--)
    {
        set3.insert(std::to_string(1000 + i));
    }
    assert(set3.size() == DEFAULT_MAX_ITEMS);
    assert(set3.insert("0") == false);
    for(int i = 0; i < DEFAULT_MAX_ITEMS; i++)
    {
        ItemType value;
        set3.get(i, value);
        assert(value == std::to_string(1001 + i));
    }
    
    return 0;
}
