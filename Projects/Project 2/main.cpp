#include "Set.h"
#include <string>
#include <iostream>
#include <cassert>

int main()
{
    
    Set s1; // Test Set constructor
    assert(s1.empty()); // Test empty
    assert(s1.size() == 0); // Test size
    s1.insert("f"); // Test insert
    s1.insert("c");
    s1.insert("a");
    s1.insert("b");
    s1.insert("d");
    s1.insert("e");
    ItemType tempValue = "";
    for(int i = 0; i < s1.size(); i ++)
    {
        ItemType tempValue0;
        s1.get(i, tempValue0); // Test get
        assert(tempValue < tempValue0); // Test insert in an increasing order
        tempValue = tempValue0;
    }
    assert(s1.size() == 6);
    assert(s1.contains("e")); // Test contains
    s1.insert("e"); // Test insert an existing value
    assert(s1.size() == 6);
    Set s2(s1); // Test Copy constructor
    Set s3;
    s3 = s1; // Test Assignment Operator
    assert(s1.size() == s2.size() && s1.size() == s3.size()); // Test Copy constructor and Assignment Operator
    for(int i = 0; i < s1.size(); i ++)
    {
        ItemType tempValue1;
        ItemType tempValue2;
        ItemType tempValue3;
        s1.get(i, tempValue1);
        s2.get(i, tempValue2);
        s3.get(i, tempValue3);
        assert(tempValue1 == tempValue2 && tempValue2 == tempValue3); // Test Copy constructor and Assignment Operator
    }
    assert(!s3.erase("j")); // Test erase a non-existing value
    assert(s3.erase("c") && s3.size() == 5); // Test erase a existing value
    ItemType arr[5];
    arr[0] = "a";
    arr[1] = "b";
    arr[2] = "d";
    arr[3] = "e";
    arr[4] = "f";
    for(int i = 0; i < 5; i ++)
    {
        ItemType tempValue6;
        s3.get(i, tempValue6);
        assert(arr[i] == tempValue6); // Test successfully erased the value
    }
    Set s4;
    s4.insert("a");
    s4.insert("b");
    s4.insert("c");
    s4.insert("x");
    s4.insert("y");
    Set result1;
    unite(s1, s1, result1);
    assert(result1.size() == 6); // Test unite the same Set
    unite(s1, s4, result1);
    assert(result1.size() == 8); // Test unite different Set
    ItemType arr1[8];
    arr1[0] = "a";
    arr1[1] = "b";
    arr1[2] = "c";
    arr1[3] = "d";
    arr1[4] = "e";
    arr1[5] = "f";
    arr1[6] = "x";
    arr1[7] = "y";
    for(int i = 0; i < 8; i ++)
    {
        ItemType tempValue4;
        result1.get(i, tempValue4);
        assert(arr1[i] == tempValue4); // Test unite different Set
    }
    Set result2;
    butNot(s1, s1, result2);
    assert(result2.size() == 0); // Test butNot the same Set
    butNot(s1, s4, result2);
    assert(result2.size() == 3);
    ItemType arr2[3];
    arr2[0] = "d";
    arr2[1] = "e";
    arr2[2] = "f";
    for(int i = 0; i < 3; i ++)
    {
        ItemType tempValue5;
        result2.get(i, tempValue5);
        assert(arr2[i] == tempValue5); // Test unite different Set
    }
   
    std::cerr << "Passed all tests" << std::endl;
    
    return 0;
}
