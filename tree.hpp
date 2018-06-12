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

        Node( int _id ) : id( _id ){};
        Node( const Node& other ) {
            id        = other.id;
            left      = other.left;
            right     = other.right;
            parent    = other.parent;
            depth     = other.depth;
            isLeaf    = other.isLeaf;
            numLeaves = other.numLeaves;
        };

        void updateParentHelper( Node* parent );
    };

    Node* root = nullptr;

    void update();
    void optimize(
        std::function<float( Tree::Node, Tree::Node )>      distance = 0,
        std::function<Tree::Node( Tree::Node, Tree::Node )> merge    = 0,
        int numLeaves = 4 );

    void print();

   private:
    void updateDepthHelper( Node*, int );
    void updateIsLeafHelper( Node* );
    int  updateNumLeavesHelper( Node* );
    void printHelper( Node*, int = 0 );
    void printNode( Node* node );
    Tree<T>::Node* findPermutationAndReplaceByEquivalentNode(
        Node* node, std::function<float( Tree::Node, Tree::Node )> distance,
        std::function<Tree::Node( Tree::Node, Tree::Node )> merge );
    std::deque<Tree::Node*> findAllNodes(
        Node* node, std::function<bool( Node* )> const& filter );
    void updateNumLeavesUpward( Node* node, int diffNumLeaves );
    void pushPossibleNodes( std::deque<Tree::Node*>& q, Node* node,
                            int numLeaves );
    std::deque<Tree::Node*> findAllLeaves( Node* node );
    std::deque<Tree::Node*> findAllNodesWithNumLeaves( Node* node,
                                                       int   numLeaves );
};

#include "tree.tpp"
