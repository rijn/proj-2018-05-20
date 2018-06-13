#pragma once

#include <deque>
#include <functional>

template <class T>
class Tree {
   public:
    class Node {
       public:
        int   id     = 0;
        Node* left   = nullptr;
        Node* right  = nullptr;
        Node* parent = nullptr;

        int  depth     = 0;
        bool isLeaf    = false;
        int  numLeaves = 0;

        T data;

        bool enabled   = true;

        Node( int _id ) : id( _id ){};
        Node( const Node& other ) {
            id        = other.id;
            left      = other.left;
            right     = other.right;
            parent    = other.parent;
            depth     = other.depth;
            isLeaf    = other.isLeaf;
            numLeaves = other.numLeaves;
            data      = other.data;
            enabled   = other.enabled;
        };

        void updateParentHelper( Node* parent );
    };

    Node* root = nullptr;

    void update();
    void optimize(
        std::function<float( typename Tree<T>::Node&, typename Tree<T>::Node& )>
            distance = 0,
        std::function<typename Tree<T>::Node( typename Tree<T>::Node,
                                              typename Tree<T>::Node )>
            merge     = 0,
        int numLeaves = 4 );

    void print();

    std::deque<typename Tree<T>::Node*> findAllNodes(
        std::function<bool( Node* )> const& filter );

   private:
    void updateDepthHelper( Node*, int );
    void updateIsLeafHelper( Node* );
    int  updateNumLeavesHelper( Node* );
    int  printHelper( Node*, int = 0 );
    void printNode( Node* node );
    Tree<T>::Node* findPermutationAndReplaceByEquivalentNode(
        Node* node,
        std::function<float( typename Tree<T>::Node&, typename Tree<T>::Node& )>
            distance,
        std::function<typename Tree<T>::Node( typename Tree<T>::Node,
                                              typename Tree<T>::Node )>
            merge );
    std::deque<typename Tree<T>::Node*> findAllNodes(
        Node* node, std::function<bool( Node* )> const& filter,
        bool stopAtDisabledNode = true );
    void updateNumLeavesUpward( Node* node, int diffNumLeaves );
    void pushPossibleNodes( std::deque<typename Tree<T>::Node*>& q, Node* node,
                            int numLeaves );
    std::deque<typename Tree<T>::Node*> findAllLeaves( Node* node );
    std::deque<typename Tree<T>::Node*> findAllNodesWithNumLeaves(
        Node* node, int numLeaves );
};

#include "tree.tpp"
