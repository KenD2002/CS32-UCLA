#include "Set.h"
#include <iostream>

using namespace std;

Set::Set()  // Set Constructor
:head(nullptr), tail(nullptr), m_size(0)
{
}

Set::~Set() // Set Destructor
{
    if(head == nullptr)
    {
        return;
    }
    
    Node* p = head;
    while(p != nullptr)
    {
        Node* n = p->nextNode;
        delete p;
        p = n;
    }
}
    
Set::Set(const Set& other) // Copy Constructor
{
    this->m_size = other.m_size;
    
    // Check if other is empty:
    if(other.empty())
    {
        this->head = nullptr;
        this->tail = nullptr;
    }
    else
    {
        // Set for head:
        Node* p;
        p = new Node;
        p->prevNode = nullptr;
        p->nextNode = nullptr;
        p->m_value = other.head->m_value;
        this->head = p;
        // Iterate for the rest Nodes:
        Node* n = other.head;
        while(n->nextNode != nullptr)
        {
            Node* k;
            k = new Node;
            k->m_value = n->nextNode->m_value;
            k->prevNode = p;
            k->nextNode = nullptr;
            p->nextNode = k;
            n = n->nextNode;
            p = k;
        }
        // Now p is already the last Node, so we set tail:
        this->tail = p;
    }
}

Set& Set:: operator= (const Set& other) // Assignment Operator Overload
{
    if(this == &other)
    {
        return *this;
    }
    
    Set tempSet(other);
    swap(tempSet);
    
    return *this;
}

bool Set::empty() const // Check if the Set is empty
{
    if(m_size == 0 && head == nullptr && tail == nullptr)
    {
        return true;
    }

    return false;
}

int Set::size() const // Get m_size
{
    return m_size;
}

bool Set::insert(const ItemType& value) // Insert a new Node
{
    if(this->contains(value))
    {
        return false;
    }
    
    // Check if the linked list is empty:
    if(this->empty())
    {
        Node* p;
        p = new Node;
        p->m_value = value;
        p->nextNode = nullptr;
        p->prevNode = nullptr;
        head = p;
        tail = p;
        this->m_size ++;
        return true;
    }
    
    // Check if the value is going to be added to the front:
    if(head->m_value > value)
    {
        Node* p;
        p = new Node;
        p->m_value = value;
        p->nextNode = head;
        p->prevNode = nullptr;
        head->prevNode = p;
        head = p;
        this->m_size ++;
        return true;
    }
    
    // Then we check if the value is going to be added to the rear:
    if(tail->m_value < value)
    {
        Node* p;
        p = new Node;
        p->m_value = value;
        p->nextNode = nullptr;
        p->prevNode = tail;
        tail->nextNode = p;
        tail = p;
        this->m_size ++;
        return true;
    }
    
    // Now the only situation left is adding to somewhere in the middle:
    Node* pos = head;
    while(pos != nullptr)
    {
        if((pos->nextNode)->m_value > value)
        {
            Node* p;
            p = new Node;
            p->m_value = value;
            p->nextNode = pos->nextNode;
            p->prevNode = pos;
            (pos->nextNode)->prevNode = p;
            pos->nextNode = p;
            this->m_size ++;
            return true;
        }
        pos = pos->nextNode;
    }
    
    return false;
}

bool Set::erase(const ItemType &value) // Remove a Node
{
    if(this->contains(value))
    {
        // Now we guarentee that the linked list is not empty.
        // If there is only one Node in the linked list:
        if(this->m_size == 1)
        {
            Node* p = head;
            head = nullptr;
            tail = nullptr;
            delete p;
            m_size --;
            return true;
        }
        
        // If there is at least two elements in the linked list:
        // If we are removing the first Node:
        if(head->m_value == value)
        {
            Node* p = head;
            (head->nextNode)->prevNode = nullptr;
            head = head->nextNode;
            delete p;
            m_size --;
            return true;
        }
        
        //If we are removing the last Node:
        if(tail->m_value == value)
        {
            Node* p = tail;
            (tail->prevNode)->nextNode = nullptr;
            tail = tail->prevNode;
            delete p;
            m_size --;
            return true;
        }
        
        //If we are removing a middle Node:
        Node* pos = head;
        while(pos != nullptr)
        {
            if(pos->m_value == value)
            {
                (pos->prevNode)->nextNode = pos->nextNode;
                (pos->nextNode)->prevNode = pos->prevNode;
                delete pos;
                this->m_size --;
                return true;
            }
            pos = pos->nextNode;
        }
    }
    
    return false;
}

bool Set::contains(const ItemType &value) const // Check if the linked list contains a value
{
    Node* pos = head;
    for(size_t i = 0; i < m_size; i ++)
    {
        if(pos->m_value == value)
        {
            return true;
        }
        pos = pos->nextNode;
    }
    
    return false;
}

bool Set::get(int pos, ItemType &value) const // Get the pos-th large value in the Set.
{
    if(0 <= pos && pos < m_size)
    {
        Node* p = head;
        for(int i = 0; i < pos; i ++)
        {
            p = p->nextNode;
        }
        value = p->m_value;
        return true;
    }
    
    return false;
}

void Set::swap(Set &other) // Swap two Sets.
{
    Node* tempHead = this->head;
    Node* tempTail = this->tail;
    int tempM_size = this->m_size;
    this->head = other.head;
    this->tail = other.tail;
    this->m_size = other.m_size;
    other.head = tempHead;
    other.tail = tempTail;
    other.m_size = tempM_size;
}

void unite(const Set& s1, const Set& s2, Set& result) // Union of Set
{
    result = s1;
    ItemType tempValue;
    for(int i = 0; i < s2.size(); i ++)
    {
        s2.get(i, tempValue);
        result.insert(tempValue);
    }
}

void butNot(const Set& s1, const Set& s2, Set& result) // Set subtraction
{
    result = s1;
    ItemType tempValue;
    for(int i = 0; i < s2.size(); i ++)
    {
        s2.get(i, tempValue);
        if(result.contains(tempValue))
        {
            result.erase(tempValue);
        }
    }
}
