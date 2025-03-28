#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <iostream>
#include <vector>

template <typename KeyType, typename ValueType>
class TreeMultimap
{
  public:
    class Iterator
    {
      public:
        Iterator()
        {
            i = 0;
            valid = false;
            // Replace this line with correct code.
        }
        Iterator(std::vector<ValueType> *vec) {
            i = 0;
            valid = true;
            v = vec;
        }

        ValueType& get_value() const
        {
            return v->at(i);
        }

        bool is_valid() const
        {
            return valid;
        }

        void advance()
        {
            //used to iterate through
            i++;
            //if its out of bounds, set valid to false indicating its reached the end
            if(i >= v->size()) {
                valid = false;
                return;
            }
        }

      private:
        int i;
        bool valid;
        std::vector<ValueType> *v;
    };

    TreeMultimap()
    {
        root = nullptr;
    }

    ~TreeMultimap()
    {
        //destructor
        for(int i = 0; i < v_treemm.size(); i++) {
            delete v_treemm.at(i);
        }
    }

    void insert(const KeyType& key, const ValueType& value)
    {
        KeyType m_key = key;
        ValueType m_value = value;
        //if it is a nullpointer, then it has no values so add a new node
        if(root == nullptr) {
            root = new Node(m_key, m_value);
            v_treemm.push_back(root);
            return;
        }
        Node *current = root;
        for(;;) {
            //if the key is equal to the current key
            if(key == current->key){
                //add the value and return
                current->values.push_back(value);
                return;
            }
            //if it is less than
            if(key < current->key) {
                //add it to the left if it isn't empty
                if(current->left!=nullptr)
                    current = current->left;
                else {
                    //if it is empty, create a new node and then add it
                    current->left = new Node(m_key, m_value);
                    v_treemm.push_back(current->left);
                    return;
                }
            }
            //if it is greater than
            else if(key > current->key) {
                //if it isn't empty add to right
                if(current->right!=nullptr)
                    current = current->right;
                else {
                    //else if it is, create new and then add
                    current->right = new Node(m_key, m_value);
                    v_treemm.push_back(current->right);
                    return;
                }
            }
        }
    }

    Iterator find(const KeyType& key) const
    {
        Node *current = root;
        for(;;) {
            //if it is nullptr, just return invalid iterator3
            if(current == nullptr) {
                return Iterator();
            }
            //if it is found(when key is current key) then return the iterator of that value
            if(key == current->key) {
                return Iterator(&current->values);
            }
            //if it is less than, move to the left
            else if(key < current->key) {
                current = current->left;
            }
            //if it is greater than, move to the right
            else if(key > current->key) {
                current = current->right;
            }
        }
        return Iterator();  // Replace this line with correct code.
    }

  private:
    struct Node{
        Node(KeyType key, ValueType value) {
            this->key = key;
            values.push_back(value);
        }
        Node *left = nullptr;
        Node *right = nullptr;
        KeyType key;
        std::vector<ValueType> values;
    };
    Node* root;
    std::vector<Node*> v_treemm;
};

#endif // TREEMULTIMAP_INCLUDED
