#pragma once
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

//NodeT class
template <class T1, class T2>
class NodeT
{
public:
    T1 key;
    T2 value;
    NodeT* left;
    NodeT* right;
    NodeT* parent;
    bool isBlack;

    //Constructor
    NodeT(T1 targetKey, T2 targetValue): key(targetKey), value(targetValue), left(nullptr), right(nullptr), parent(nullptr), isBlack(false) {};
};

template <class T1, class T2>
class RedBlackTree
{
    public:
    //Default Constructor
    RedBlackTree();

    //Copy Constructor
    RedBlackTree(const RedBlackTree & rbt);

    //Assignment Operatoy
    RedBlackTree & operator=(const RedBlackTree & rbt);

    //Destructure
    ~RedBlackTree();

    //Mutators
    bool insert(T1 targetKey, T2 targetValue);

    bool remove(T1 targetKey);

    //Accessors
    bool search(T1 targetKey) const;

    vector<T2> search(T1 targetKey1, T1 targetKey2) const;

    vector<T2> values() const;

    vector<T1> keys() const;

    int size() const;

    NodeT<T1, T2>* getRoot() const;

    private:
    //Attributes
    NodeT<T1, T2>* root;
    int treeSize;
    
    //Helper Methods
    void copyTree(NodeT<T1, T2>* original, NodeT<T1, T2>* dst);
    void clear();
    void clear(NodeT<T1, T2>* current);
    NodeT<T1, T2>* findNode(T1 targetKey, NodeT<T1, T2>* & Parent) const;
    void bstRemove(T1 targetKey);
    NodeT<T1, T2>* getSuccessor(NodeT<T1, T2>* target, NodeT<T1, T2>* & sParent) const;
    void rbtInsertFix(NodeT<T1, T2>* & targetNode);
    void rbtRemoveFix(NodeT<T1, T2>* & targetNode);
    void leftRotate(NodeT<T1, T2>* targetNode);
    void rightRotate(NodeT<T1, T2>* targetNode);
    void inOrderKeyPushBack(NodeT<T1, T2>* current, vector<T1> &usingVecotr) const;
    void inOrderValuePushBack(NodeT<T1, T2>* current, vector<T2> &usingVecotr) const;
    void pushValueBack(NodeT<T1, T2>* current, T1 targetKey1, T1 targetKey2, vector<T2> &usingVecotr) const;    
};

//RedBlackTree methods implementation

//Default Constructor
template <class T1, class T2>
RedBlackTree<T1, T2>::RedBlackTree()
{
    root = nullptr;   
    treeSize = 0;
}

//Copy Constructor
template <class T1, class T2>
RedBlackTree<T1, T2>::RedBlackTree(const RedBlackTree & rbt)
{
    root = nullptr;
    if (rbt.root != nullptr)
    {
        //Copy root of rbt
        root = new NodeT<T1, T2>(rbt.root->key, rbt.root->value);
        root->isBlack = true;
        //Copy rest of parameter rbt
        copyTree(rbt.root, root);
    }
}

//Overloaded Assignment Operator
template <class T1, class T2>
RedBlackTree<T1, T2> & RedBlackTree<T1, T2>::operator=(const RedBlackTree & rbt)
{
    if (this != &rbt)
    {
        clear();
        RedBlackTree temp(rbt);
        root = temp.root;   
        temp.root = nullptr;
    } 
    return *this;
}

//Destructure
template <class T1, class T2>
RedBlackTree<T1, T2>::~RedBlackTree()
{
    clear();
}

//PRE: tree will not contain duplicated keys
template <class T1, class T2>
bool RedBlackTree<T1, T2>::insert(T1 targetKey, T2 targetValue)
{
    //check if the tree contain targetKey
    if (search(targetKey))
    {
        return false;
    }

    //bst insert
    else
    {
	    NodeT<T1, T2>* Parent = root;
	    NodeT<T1, T2>* next = root;        
        NodeT<T1, T2>* targetNode = new NodeT<T1, T2>(targetKey, targetValue);

	    if(root==NULL)
	    {
	        root=targetNode;
	        targetNode->parent=NULL;
	    }
        
	    else
	    {
	        while(Parent!=NULL)
	        {
	            next=Parent;
	            if(Parent->key < targetNode->key)
	            {	
	                Parent=Parent->right;
	            }    
	            else
	            {	
	                Parent=Parent->left;
	            }    
	        }
	        //targetNode->parent=next;
	        if(next->key < targetNode->key)
	        {	
	            next->right=targetNode;
	            targetNode->parent = next;
	        }
	        else
	        {	
	            next->left=targetNode;
	            targetNode->parent = next;
	    	}
	    }
        	
        targetNode->isBlack = false;

        while (targetNode != root && targetNode->parent->isBlack == false)
        {
            rbtInsertFix(targetNode); 
        }
        root->isBlack=true;

	    //Increment the size of the tree
	    treeSize++;
	    return true; 	
    }	     
}

template <class T1, class T2>
bool RedBlackTree<T1, T2>::remove(T1 targetKey)
{
    //chech if the tree is empty or if the tree does not contain targetKey
    if (root == nullptr || !search(targetKey))
    {
        return false;
    }
    else
    {
        //Remove key from tree maintaining bst property 
        bstRemove(targetKey);

        //removed node
        NodeT<T1, T2>* y;

        if (y->isBlack == true)
        {
            //rbtRemoveFix(targetNode);  //targetNode could be null  
        }
       
        //Decrement the size of the tree
        treeSize--;
        return true;
    }
}

template <class T1, class T2>
bool RedBlackTree<T1, T2>::search(T1 targetKey) const
{
    NodeT<T1, T2>* current = root;
    while (current != nullptr)
    {
        if (targetKey == current->key)
        {
            return true;
        }
        else if(targetKey < current->key)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }        
    }
    return false;
}

//POST: returns a vector containing the values between targetKey1 and targetKey2(in ascending order of the keys)
template <class T1, class T2>
vector<T2> RedBlackTree<T1, T2>::search(T1 targetKey1, T1 targetKey2) const
{
    vector<T2> vValues;

    if (targetKey1 <= targetKey2)
    {
        pushValueBack(root, targetKey1, targetKey2, vValues);      
    }
    else //targetKey1 > targetKey2
    {
        pushValueBack(root, targetKey2, targetKey1, vValues);        
    } 
    return vValues;          
}

//POST: returns a vector containing all values in the tree(in ascending order of the keys) 
template <class T1, class T2>
vector<T2> RedBlackTree<T1, T2>::values() const
{
    vector<T2> vValues;
    //if tree is empty
    if (root == nullptr)
    {
        return vValues;
    }
    else  //non-emoty
    {
        inOrderValuePushBack(root, vValues);
        return vValues;
    }
}

//POST:: returns a vector containing all keys in the tree(in a acsending order of thre keys)
template <class T1, class T2>
vector<T1> RedBlackTree<T1, T2>::keys() const
{ 
    vector<T1> vKeys; 
    //if tree is empty
    if (root == nullptr)
    {
        return vKeys;
    }
    else   //non-emoty
    {
        inOrderKeyPushBack(root, vKeys);
        return vKeys;
    }
}

//POST: return number of items stored in the tree
template <class T1, class T2>
int RedBlackTree<T1, T2>::size() const
{
    return treeSize;
}

template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::getRoot() const
{
    return root;
}

// Helper methods implementation

//PARAM: original - pointer to the tree that we are copying from
//    dst(destination) - pointer to the tree that we are making
//POST: Copy rest of nodes of the tree
template <class T1, class T2>
void RedBlackTree<T1, T2>::copyTree(NodeT<T1, T2>* original, NodeT<T1, T2>* dst)
{
    if (original != nullptr)
    {
        if (original->left != nullptr)
        {
            dst->left = new NodeT<T1, T2>(original->left->key, original->left->value);
            //copy the colour of nodes
            if (original->left->isBlack == true)
            {
                dst->left->isBlack = true;
            }
            else
            {
                dst->left->isBlack = false;
            }            
        }
        if (original->right != nullptr)
        {
            dst->right = new NodeT<T1, T2>(original->right->key, original->right->value);
            //copy the colour of nodes
            if (original->right->isBlack == true)
            {
                dst->right->isBlack = true;
            }
            else
            {
                dst->right->isBlack = false;
            }            
        }
        copyTree(original->left, dst->left);
        copyTree(original->right, dst->right); 
    } 
}

// Helper method that Empties tree
// PRE:
// POST: Removes all nodes from tree, deallocates dynamic memory
template <class T1, class T2>
void RedBlackTree<T1, T2>::clear()
{
	clear(root);
	root = nullptr;
}

// Helper method for clear
// PRE:
// POST: Removes all nodes from tree, deallocates dynamic memory
template <class T1, class T2>
void RedBlackTree<T1, T2>::clear(NodeT<T1, T2>* current)
{
	if (current != nullptr)
    {
		clear(current->left);
		clear(current->right);
		delete current;
	}
}

//PARAM: target = value to be found
//       Parent = Parent of target
template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::findNode(T1 targetKey, NodeT<T1, T2>* & Parent) const
{
    NodeT<T1, T2>* current = root;
    Parent = nullptr;
    while (current != nullptr)
    {
        if (targetKey == current->key)
        {
            current->parent = nullptr;
            return current;
        }
        Parent = current;
        if (targetKey < current->key)   
        {
            current = current->left;
            current->parent = Parent;
        }
        else
        {
            current = current->right;
            current->parent = Parent;
        } 
    }
    return current;
}

//correct the black height property of the tree since a new node has been inserted
template <class T1, class T2>
void RedBlackTree<T1, T2>::rbtInsertFix(NodeT<T1, T2>* & targetNode)
{
    NodeT<T1, T2>* grandParent = targetNode->parent->parent;
    if (targetNode->parent == grandParent->left)
    {
        NodeT<T1, T2>* uncle = grandParent->right;
        if (uncle != nullptr && uncle->isBlack == false)
        {
            targetNode->parent->isBlack = true;
            uncle->isBlack = true;
            grandParent->isBlack = false;
            targetNode = grandParent;
        }
        else    //if uncle == nullptr
        {
            if (targetNode == targetNode->parent->right)
            {
                targetNode = targetNode->parent;
                leftRotate(targetNode);
            }
            targetNode->parent->isBlack = true;
            grandParent->isBlack = false;
            rightRotate(grandParent);
        }   
    }

    else    //(targetNode->parent == grandParent->right)
    {
        NodeT<T1, T2>* uncle = grandParent->left;
        if (uncle != nullptr && uncle->isBlack == false)
        {
            targetNode->parent->isBlack = true;
            uncle->isBlack = true;
            grandParent->isBlack = false;
            targetNode = grandParent;
        }
        else    //if uncle == nullptr
        {
            if (targetNode == targetNode->parent->left)
            {
                targetNode = targetNode->parent;
                rightRotate(targetNode);
            }
            targetNode->parent->isBlack = true;
            grandParent->isBlack = false;
            leftRotate(grandParent);
        }           
    }
}

//used by remove
//Remove key from tree maintaining bst property 
template <class T1, class T2>
void RedBlackTree<T1, T2>::bstRemove(T1 targetKey)
{
    //Find node to remove
    NodeT<T1, T2>* Parent = nullptr;
    NodeT<T1, T2>* target = findNode(targetKey, Parent);

    //target has one or no children
    if (target->left == nullptr || target->right == nullptr)
    {
        //Identify child (if any)
        NodeT<T1, T2>* child = target->left;
        if (child == nullptr)
        {
            child = target->right;  //may be null
        }
        //Attach child to parent or make child the new root
        if (Parent != nullptr)
        {
            if (target == Parent->left)
            {
                Parent->left = child;
                if (child != nullptr)
                {
                    //update patent of the child  
                    child->parent = Parent->left;                         
                }
            }
            else
            {
                Parent->right = child;
                if (child != nullptr)
                {
                    //update parent of the child
                    child->parent = Parent->right;
                }
            }  
        }
        else  //target is root
        {
            root = child;
            //update parent of the child
            root->parent = nullptr;
        } 
    }

    //target has two children - replace target with successor
    else
    {
        //Find successor
        NodeT<T1, T2>* successorParent = nullptr;
        NodeT<T1, T2>* successor = getSuccessor(target, successorParent);

        //Attach target's left child to the successor
        if (successor->parent != target)
        {
            successor->left = target->left;
            if (target->left != nullptr)
            {
                //update parent's of target's left child
                target->left->parent = successor;            
            }            
        }

        //Attach successor to the target's parent (it the target is in the tree) or make new root
        if (Parent != nullptr) //target is in the tree
        {
            if (target == Parent->left)
            {
                Parent->left = successor;
            }
            else
            {
                Parent->right = successor;
            }
            
            //update successor's parent
            successor->parent = Parent;  
        }

        else //target is root
        {
            root = successor;
            //update parent of successor
            root->parent = nullptr;            
        }

        //when successor is not target's right child
        if (successor != target->right)
        {
            if (successorParent != target)
            {
                successorParent->left = successor->right;
                if (successor->right != nullptr)
                {
                    //update successor right's parent
                    successor->right->parent = successorParent;            
                }

                successor->right = target->right;
                
                if (target->right != nullptr)
                {
                    //update target right's parent
                    target->right->parent = successor;                
                }                
            }
        }   
    }
    delete target;   
}

//PARAM: target - node to find the successor of
//       sParent - parent of successor
//POST: returns the successor of target, sets successor's parent
template <class T1, class T2>
NodeT<T1, T2>* RedBlackTree<T1, T2>::getSuccessor(NodeT<T1, T2>* target, NodeT<T1, T2>* & sParent) const
{
    //Successor is the left most node of the right subtree
    NodeT<T1, T2>* current = target->right;
    sParent = target;

    while (current->left != nullptr)
    {
        sParent = current;
        current = current->left;
    }
    return current;   
}

//correct the black height property of the tree since a black node hase been removed
template <class T1, class T2>
void RedBlackTree<T1, T2>::rbtRemoveFix(NodeT<T1, T2>* & targetNode)
{
    while (targetNode != root && targetNode->isBlack == true)
    {
        if (targetNode == targetNode->parent->left)  //targetNode is left child
        {
            NodeT<T1, T2>* sibiling = targetNode->parent->right;   //targetNode's sibiling
            if (sibiling->isBlack == false)
            {
                sibiling->isBlack = true;
                targetNode->parent->isBlack = false;    //parent was black
                leftRotate(targetNode->parent);
                sibiling = targetNode->parent->right;
            }
            if (sibiling->left->isBlack == true && sibiling->right->isBlack == true)
            {
                sibiling->isBlack = true;
            }
            else
            {
                if (sibiling->right->isBlack == true)
                {
                    sibiling->left->isBlack = true;
                    sibiling->isBlack = false;
                    rightRotate(sibiling);
                    sibiling = targetNode->parent->right;
                }
                //change the colour of the sibiling, to the colour of parent's of targetNode-
                if (sibiling->isBlack == true)
                {
                    targetNode->parent->isBlack = true;
                }
                else
                {
                    targetNode->parent->isBlack = false;
                }
                //y->isBlack = targetNode->parent->isBlack  //aya khode in ro nemishe bejaye az khate chnge color ta inja gozasht?
                targetNode->parent->isBlack = true;
                sibiling->right->isBlack = true;
                leftRotate(targetNode->parent);
                targetNode = root;
            } 
        }

        else   //symmetric of if //targetNode is right child
        {
            NodeT<T1, T2>* sibiling = targetNode->parent->left;   //targetNode's sibiling
            if (sibiling->isBlack == false)
            {
                sibiling->isBlack = true;
                targetNode->parent->isBlack = false;    //parent was black
                rightRotate(targetNode->parent);
                sibiling = targetNode->parent->left;
            }
            if (sibiling->right->isBlack == true && sibiling->left->isBlack == true)
            {
                sibiling->isBlack = true;
            }
            else
            {
                if (sibiling->left->isBlack == true)
                {
                    sibiling->right->isBlack = true;
                    sibiling->isBlack = false;
                    leftRotate(sibiling);
                    sibiling = targetNode->parent->left;
                }
                //change the colour of the sibiling, to the colour of parent's of targetNode-
                if (sibiling->isBlack == true)
                {
                    targetNode->parent->isBlack = true;
                }
                else
                {
                    targetNode->parent->isBlack = false;
                }
                //y->isBlack = targetNode->parent->isBlack  //aya khode in ro nemishe bejaye az khate chnge color ta inja gozasht?
                targetNode->parent->isBlack = true;
                sibiling->left->isBlack = true;
                rightRotate(targetNode->parent);
                targetNode = root;
            } 
        }
    }

    targetNode->isBlack = true;     //inja bayad bashe ya balatari jaii?
}

//used by rbtInsertFix and rbtRemoveFix
template <class T1, class T2>
void RedBlackTree<T1, T2>::leftRotate(NodeT<T1, T2>* targetNode)
{
    if(targetNode->right==NULL)
    {
        return ;
    } 
     
    else
    {
        NodeT<T1, T2>* child = targetNode->right;
        targetNode->right = child->left;        
        if(child->left != nullptr)
        {
            child->left->parent = targetNode;
        }

        if(targetNode->parent != nullptr)
        {
            child->parent = targetNode->parent;  
        }  

        if(targetNode->parent == nullptr)
        {
            root = child;
            child->parent = nullptr;
        }    
        else    //targetNode->parent != nullptr
        {
            if(targetNode == targetNode->parent->left)
            {
                targetNode->parent->left = child;
                child->parent = targetNode->parent;
            }
            else
            {
                targetNode->parent->right = child;
                child->parent = targetNode->parent;
            }    
        }
                
        child->left = targetNode;
        targetNode->parent = child;
    }  
}

//used by rbtInsertFix and rbtRemoveFix
template <class T1, class T2>
void RedBlackTree<T1, T2>::rightRotate(NodeT<T1, T2>* targetNode)   
{ 
    if(targetNode->left == nullptr)
    {	
        return ;
    }

    else
    {  
        NodeT<T1, T2>* child = targetNode->left;
        targetNode->left = child->right;        
        if(child->right != nullptr)
        {
            child->right->parent = targetNode;
        }

        if(targetNode->parent != nullptr)
        {
            child->parent = targetNode->parent;
        } 

        if(targetNode->parent == nullptr)
        {
            root = child;
        }    
        else
        {
            if(targetNode == targetNode->parent->left)
            {
                targetNode->parent->left = child;
                child->parent = targetNode->parent;
            }
            else
            {
                targetNode->parent->right = child;
                child->parent = targetNode->parent;              
            }    
        }
        child->right = targetNode;
        targetNode->parent = child;
    }
}

//used by search
//POST: add values between targetKey1 and targetKey2 to the vector (inorder of the key)
template <class T1, class T2>
void RedBlackTree<T1, T2>::pushValueBack(NodeT<T1, T2>* current, T1 targetKey1, T1 targetKey2, vector<T2> &usingVecotr) const
{
    //base case
    if (current == nullptr)
    {
        return;
    }
    //if root is greater than targetKey1 
    if (targetKey1 < current->key)
    {     
        pushValueBack(current->left, targetKey1, targetKey2, usingVecotr);  
    }
    //if root is between targetKey1 & targetKey2
    if (targetKey1 <= current->key && current->key <= targetKey2) 
    {
        usingVecotr.push_back(current->value);             
    }
    //if root is smaller than targetKey2
    if (targetKey2 > current->key) 
    {
        pushValueBack(current->right, targetKey1, targetKey2, usingVecotr);
    } 
}

//used by values
//POST: add value of the current node to the vector (inorder of the key)
template <class T1, class T2>
void RedBlackTree<T1, T2>::inOrderValuePushBack(NodeT<T1, T2>* current, vector<T2> &usingVecotr) const
{ 
    if (current != nullptr)
    {
		inOrderValuePushBack(current->left, usingVecotr);
        usingVecotr.push_back(current->value);
		inOrderValuePushBack(current->right, usingVecotr);        
    }    
}

//used by keys
//POST: add key of the current node to the vector (in order of the key)
template <class T1, class T2>
void RedBlackTree<T1, T2>::inOrderKeyPushBack(NodeT<T1, T2>* current, vector<T1> &usingVecotr) const
{ 
    if (current != nullptr)
    {
		inOrderKeyPushBack(current->left, usingVecotr);
        usingVecotr.push_back(current->key);
		inOrderKeyPushBack(current->right, usingVecotr);        
    }    
}