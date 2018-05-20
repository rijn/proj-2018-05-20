#include <iomanip>
#include <iostream>

#include "tree.hpp"

void Tree::update() {
    updateDepthHelper( root, 0 );
    updateIsLeafHelper( root );
    updateNumLeavesHelper( root );
}

void Tree::updateDepthHelper( Node* node, int depth ) {
    if ( node == nullptr ) return;
    node->depth = depth;
    updateDepthHelper( node->left, depth + 1 );
    updateDepthHelper( node->right, depth + 1 );
}

void Tree::updateIsLeafHelper( Node* node ) {
    if ( node == nullptr ) return;
    if ( node->left == nullptr && node->right == nullptr ) {
        node->isLeaf = true;
    } else {
        node->isLeaf = false;
        updateIsLeafHelper( node->left );
        updateIsLeafHelper( node->right );
    }
}

int Tree::updateNumLeavesHelper( Node* node ) {
    if ( node == nullptr ) return 0;
    if ( node->isLeaf ) {
        node->numLeaves = 1;
    } else {
        node->numLeaves = updateNumLeavesHelper( node->left ) +
                          updateNumLeavesHelper( node->right );
    }
    return node->numLeaves;
}

void Tree::print() {
    printHelper( root, 0 );
}

void Tree::printHelper( Node* p, int indent = 0 ) {
#if 0
    if(p != nullptr) {
        if(p->right) {
            printHelper(p->right, indent+4);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
        std::cout<< p->id << "\n ";
        if(p->left) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            printHelper(p->left, indent+4);
        }
    }
#else
    if ( p != nullptr ) {
        std::cout << p->id << ", depth = " << p->depth
                  << ", isLeaf = " << p->isLeaf
                  << ", numLeaves = " << p->numLeaves << std::endl;
        if ( p->left ) {
            printHelper( p->left, 0 );
        }
        if ( p->right ) {
            printHelper( p->right, 0 );
        }
    }
#endif
}
