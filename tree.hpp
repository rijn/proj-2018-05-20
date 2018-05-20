#pragma once

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
    void optimize();

    void print();

   private:
    void updateDepthHelper( Node*, int );
    void updateIsLeafHelper( Node* );
    int  updateNumLeavesHelper( Node* );
    void printHelper( Node*, int );
};
