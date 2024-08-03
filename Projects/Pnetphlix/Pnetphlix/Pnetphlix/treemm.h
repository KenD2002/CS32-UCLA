#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <vector>
#include <string>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
private:
    struct Node
    {
        Node(const KeyType &myKey, const ValueType &myVal)
        {
            m_key = myKey;
            m_values.push_back(myVal);
            left = nullptr;
            right = nullptr;
        }
        KeyType m_key;
        std::vector<ValueType> m_values;
        Node* left;
        Node* right;
    };
    
  public:
    class Iterator
    {
      public:
        Iterator()
        :m_valid(false)
        {}
        
        Iterator(std::vector<ValueType> &values)
        :m_values_end(values.end()), m_iterator(values.begin()), m_valid(true)
        {}

        ValueType& get_value() const
        {
            return *m_iterator;
        }

        bool is_valid() const
        {
            return m_valid;
        }

        void advance()
        {
            m_iterator++;
            if(m_iterator == m_values_end)
            {
                m_valid = false;
                return;
            }
        }
        

      private:
        typename std::vector<ValueType>::iterator m_values_end;
        typename std::vector<ValueType>::iterator m_iterator;
        bool m_valid;
    };

    TreeMultimap()
    {
        m_root = nullptr;
    }

    ~TreeMultimap()
    {
        freeTreeMultimap(m_root);
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        if(m_root == nullptr)  // if the BST is empty
        {
            m_root = new Node(key, value);
            return;
        }
        
        Node* cur = m_root;
        for(;;)
        {
            if(key == cur->m_key) // the key already exists, then push_back the value into this node
            {
                cur->m_values.push_back(value);
                return;
            }
            
            if(key < cur->m_key)
            {
                if(cur->left != nullptr)
                {
                    cur = cur->left;
                }
                else
                {
                    cur->left = new Node(key, value);
                    return;
                }
            }
            else if(key > cur->m_key)
            {
                if(cur->right != nullptr)
                {
                    cur = cur->right;
                }
                else
                {
                    cur->right = new Node(key, value);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node *ptr = m_root;
        while(ptr != nullptr)
        {
            if(key == ptr->m_key)
            {
                return Iterator(ptr->m_values);
            }
            else if(key < ptr->m_key)
            {
                ptr = ptr->left;
            }
            else if(key > ptr->m_key)
            {
                ptr = ptr->right;
            }
        }
        return Iterator();
    }

  private:
    
    void freeTreeMultimap(Node* cur)
    {
        if(cur == nullptr)
        {
            return;
        }
        freeTreeMultimap(cur->left);
        freeTreeMultimap(cur->right);
        delete cur;
    }
    
    Node* m_root;
    
};

#endif // TREEMULTIMAP_INCLUDED
