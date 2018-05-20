#pragma once

#include <deque>

class Tree {
   public:
    class Node {
       public:
        int   id    = 0;
        Node* left  = nullptr;
        Node* right = nullptr;

        int  depth     = 0;
        bool isLeaf    = false;
        int  numLeaves = 0;

        Node( int _id ) : id( _id ){};
    };

    Node* root = nullptr;

    void update();
    void optimize( int numLeaves = 4 );

    void print();

   private:
    void updateDepthHelper( Node*, int );
    void updateIsLeafHelper( Node* );
    int  updateNumLeavesHelper( Node* );
    void printHelper( Node*, int );
    void printNode( Node* node );
    Tree::Node* findPermutationAndReplaceByEquivalentNode( Node* node );
    std::deque<Tree::Node*> findAllNodes(
        Node* node, std::function<bool( Node* )> const& filter );
    std::deque<Tree::Node*> findAllLeaves( Node* node );
    std::deque<Tree::Node*> findAllNodesWithNumLeaves( Node* node,
                                                       int   numLeaves );
};
