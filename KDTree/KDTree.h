/**
 * File: KDTree.h
 * Author: (your name here)
 * ------------------------
 * An interface representing a kd-tree in some number of dimensions. The tree
 * can be constructed from a set of data and then queried for membership and
 * nearest neighbors.
 */

#ifndef KDTREE_INCLUDED
#define KDTREE_INCLUDED

#include "Point.h"
#include "BoundedPQueue.h"
#include <stdexcept>
#include <cmath>

// "using namespace" in a header file is conventionally frowned upon, but I'm
// including it here so that you may use things like size_t without having to
// type std::size_t every time.


template <size_t N, typename ElemType>
struct Node{
    Node(Point<N> pt, ElemType data,int level){
        this->point = pt;
        this->data = data;
        this->level = level;
    }

    Point<N> point;
    ElemType data;
    int level = 0;
    Node* left = nullptr;
    Node* right = nullptr;

    double levelCoord(){
        return point[level%N];
    }
};

using namespace std;

template <size_t N, typename ElemType>
class KDTree {
public:
    // Constructor: KDTree();
    // Usage: KDTree<3, int> myTree;
    // ----------------------------------------------------
    // Constructs an empty KDTree.
    KDTree();
    
    // Destructor: ~KDTree()
    // Usage: (implicit)
    // ----------------------------------------------------
    // Cleans up all resources used by the KDTree.
    ~KDTree();
    
    // KDTree(const KDTree& rhs);
    // KDTree& operator=(const KDTree& rhs);
    // Usage: KDTree<3, int> one = two;
    // Usage: one = two;
    // -----------------------------------------------------
    // Deep-copies the contents of another KDTree into this one.
    KDTree(const KDTree& rhs);
    KDTree& operator=(const KDTree& rhs);
    
    // size_t dimension() const;
    // Usage: size_t dim = kd.dimension();
    // ----------------------------------------------------
    // Returns the dimension of the points stored in this KDTree.
    size_t dimension() const;
    
    // size_t size() const;
    // bool empty() const;
    // Usage: if (kd.empty())
    // ----------------------------------------------------current->levelCoord()
    // Returns the number of elements in the kd-tree and whether the tree is
    // empty.
    size_t size() const;
    bool empty() const;
    
    // bool contains(const Point<N>& pt) const;
    // Usage: if (kd.contains(pt))
    // ----------------------------------------------------
    // Returns whether the specified point is contained in the KDTree.
    bool contains(const Point<N>& pt) const;
    
    // void insert(const Point<N>& pt, const ElemType& value);
    // Usage: kd.insert(v, "This value is associated with v.");
    // ----------------------------------------------------
    // Inserts the point pt into the KDTree, associating it with the specifiedcurrent->levelCoord()
    // value. If the element already existed in the tree, the new value will
    // overwrite the existing one.
    void insert(const Point<N>& pt, const ElemType& value);
    
    // ElemType& operator[](const Point<N>& pt);
    // Usage: kd[v] = "Some Value";
    // ----------------------------------------------------
    // Returns a reference to the value associated with point pt in the KDTree.
    // If the point does not exist, then it is added to the KDTree using the
    // default value of ElemType as its key.
    ElemType& operator[](const Point<N>& pt);
    
    // ElemType& at(const Point<N>& pt);
    // const ElemType& at(const Point<N>& pt) const;
    // Usage: cout << kd.at(v) << endl;
    // ----------------------------------------------------
    // Returns a reference to the key associated with the point pt. If the point
    // is not in the tree, this function throws an out_of_range exception.
    ElemType& at(const Point<N>& pt);
    const ElemType& at(const Point<N>& pt) const;
    
    // ElemType kNNValue(const Point<N>& key, size_t k) const
    // Usage: cout << kd.kNNValue(v, 3) << endl;
    // ----------------------------------------------------
    // Given a point v and an integer k, finds the k points in the KDTree
    // nearest to v and returns the most common value associated with those
    // points. In the event of a tie, one of the most frequent value will be
    // chosen.
    ElemType kNNValue(const Point<N>& key, size_t k) const;

private:
    void recKNNValue(const Point<N>&, BoundedPQueue<Node<N,ElemType>* >&, Node<N,ElemType>*) const;
    Node<N,ElemType>* root;
    void traverseAndDelete(Node<N,ElemType>*);
    Node<N,ElemType>* findNode(Point<N>) const;
    Node<N,ElemType>* insertNode(const Point<N>& pt, const ElemType& value);
    void copyRec(Node<N,ElemType>*,Node<N,ElemType>*);
    size_t sizeT = 0;
};

/** KDTree class implementation details */

template <size_t N, typename ElemType>
KDTree<N, ElemType>::KDTree() {
    root = nullptr;
}

template <size_t N, typename ElemType>
KDTree<N, ElemType>::~KDTree() {
    traverseAndDelete(root);
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::dimension() const {
    return N;
}

template <size_t N, typename ElemType>
size_t KDTree<N, ElemType>::size() const {
    return sizeT;
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::empty() const {
    return sizeT == 0;
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::insert(const Point<N>& pt, const ElemType& value){
    insertNode(pt,value);
}

template <size_t N, typename ElemType>
bool KDTree<N, ElemType>::contains(const Point<N>& pt) const {
    return findNode(pt) != nullptr;
}

template <size_t N, typename ElemType>
ElemType& KDTree<N,ElemType>::operator[](const Point<N>& pt){
    Node<N,ElemType>* node = findNode(pt);
    if (node == nullptr){
        ElemType data = ElemType();
        Node<N,ElemType>* newNode = insertNode(pt,data);
        return newNode->data;
    }
    return node->data;

}

template <size_t N, typename ElemType>
KDTree<N,ElemType>::KDTree(const KDTree& rhs){
    Node<N,ElemType>* currentT = rhs.root;
    root = new Node<N,ElemType>(currentT->point,currentT->data,currentT->level);
    sizeT++;
    copyRec(root,currentT);
}

template<size_t N,typename ElemType>
void KDTree<N,ElemType>::copyRec(Node<N,ElemType>* currentO,Node<N,ElemType>* currentT){
    if (currentT->left != nullptr){
        currentO->left = new Node<N,ElemType>(currentT->left->point,currentT->left->data,currentT->left->level);
        copyRec(currentO->left,currentT->left);
        sizeT++;
    }
    if (currentT->right != nullptr){
        currentO->right = new Node<N,ElemType>(currentT->right->point,currentT->right->data,currentT->right->level);
        copyRec(currentO->right,currentT->right);
        sizeT++;
    }
}

template <size_t N, typename ElemType>
KDTree<N,ElemType>& KDTree<N,ElemType>::operator=(const KDTree& rhs){
    Node<N,ElemType>* currentT = rhs.root;
    root = new Node<N,ElemType>(currentT->point,currentT->data,currentT->level);
    sizeT++;
    copyRec(root,currentT);
    return *this;
}

template <size_t N, typename ElemType>
Node<N,ElemType>* KDTree<N,ElemType>::insertNode(const Point<N>& pt, const ElemType& value){
    if (root == nullptr){
        root = new Node<N, ElemType>(pt,value,0);
        sizeT++;
        return root;
    }
    Node<N, ElemType>* current = root;
    int level = 0;
    while(true){
        if (current->point == pt){
            current->data=value;
            return current;
        }
        else{
            if (pt[level%N] < current->levelCoord()){
                if (current->left == nullptr){
                    current->left = new Node<N,ElemType>(pt,value,level+1);
                    sizeT++;
                    return current->left;
                }
                else
                    current = current->left;
            }
            else{
                if (current->right == nullptr){
                    current->right = new Node<N,ElemType>(pt,value,level+1);
                    sizeT++;
                    return current->right;
                }
                else
                    current = current->right;
            }
        }
        level++;
    }
}

template <size_t N, typename ElemType>
ElemType& KDTree<N,ElemType>::at(const Point<N>& pt){
    Node<N,ElemType>* node = findNode(pt);
    if (node == nullptr)
        throw out_of_range("Point does not exist in tree");
    else
        return node->data;
}

template <size_t N, typename ElemType>
const ElemType& KDTree<N,ElemType>::at(const Point<N>& pt) const{
    Node<N,ElemType>* node = findNode(pt);
    if (node == nullptr)
        throw out_of_range("Point does not exist in tree");
    else
        return node->data;
}

template <size_t N, typename ElemType>
ElemType KDTree<N,ElemType>::kNNValue(const Point<N>& pt, size_t k) const{
    BoundedPQueue<Node<N,ElemType>* > Q(k);
    recKNNValue(pt,Q,root);
    map<ElemType,int> amountOfNodes;
    ElemType bestData;
    int highestNumber = 0;
    while (!Q.empty()){
        Node<N,ElemType>* node = Q.dequeueMin();
        if (amountOfNodes.find(node->data) != amountOfNodes.end()){
            amountOfNodes[node->data]++;
            if (amountOfNodes[node->data] > highestNumber){
                highestNumber = amountOfNodes[node->data];
                bestData = node->data;
            }
        }
        else{
            if (highestNumber == 0){
                highestNumber = 1;
                bestData = node->data;
            }
            amountOfNodes[node->data] = 1;
        }
    }
    return bestData;
}

template <size_t N, typename ElemType>
void KDTree<N,ElemType>::recKNNValue(const Point<N>& pt, BoundedPQueue<Node<N,ElemType>* >& Q, Node<N,ElemType>* current) const{
    if (current == nullptr)
        return;
    Q.enqueue(current,Distance(current->point,pt));
    if (pt[current->level%N] < current->levelCoord())
        recKNNValue(pt,Q,current->left);
    else
        recKNNValue(pt,Q,current->right);

    if ((Q.size() != Q.maxSize()) || (abs(current->levelCoord()-pt[current->level%N]) < Q.worst())){
        if (pt[current->level%N] < current->levelCoord())
            recKNNValue(pt,Q,current->right);
        else
            recKNNValue(pt,Q,current->left);
    }
}


template <size_t N, typename ElemType>
Node<N, ElemType>* KDTree<N,ElemType>::findNode(Point<N> pt) const{
    if (root == nullptr)
        return root;
    Node<N, ElemType>* current = root;
    int level = 0;
    while(true){
        if (current->point == pt){
            return current;
        }
        else{
            if (pt[level%N] < current->levelCoord()){
                if (current->left == nullptr){
                    return current->left;
                }
                else
                    current = current->left;
            }
            else{
                if (current->right == nullptr){
                    return current->right;
                }
                else
                    current = current->right;
            }
        }
        level++;
    }
}

template <size_t N, typename ElemType>
void KDTree<N, ElemType>::traverseAndDelete(Node<N, ElemType>* node){
    if (node == nullptr)
        return;
    if (node->left != nullptr)
        traverseAndDelete(node->left);
    if (node->right != nullptr)
        traverseAndDelete(node->right);
    delete node;
}

// TODO: finish the implementation of the rest of the KDTree class

#endif // KDTREE_INCLUDED
