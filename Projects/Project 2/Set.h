#ifndef Set_h
#define Set_h

#include <string>

using ItemType = std::string;

class Set
{
public:
    Set();
    ~Set();
    Set(const Set& other);
    Set &operator= (const Set& other);
    bool empty() const;
    int size() const;
    bool insert(const ItemType& value);
    bool erase(const ItemType& value);
    bool contains(const ItemType& value) const;
    bool get(int pos, ItemType& value) const;
    void swap(Set& other);
    
private:
    struct Node
    {
        ItemType m_value;
        Node *nextNode;
        Node *prevNode;
    };
    Node* head;
    Node* tail;
    int m_size;
};

void unite(const Set& s1, const Set& s2, Set& result);
void butNot(const Set& s1, const Set& s2, Set& result);


#endif /* Set_h */
