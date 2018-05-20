#include <deque>
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
        printNode( p );
        if ( p->left ) {
            printHelper( p->left, 0 );
        }
        if ( p->right ) {
            printHelper( p->right, 0 );
        }
    }
#endif
}

void Tree::printNode( Node* node ) {
    std::cout << node->id << ", depth = " << node->depth
              << ", isLeaf = " << node->isLeaf
              << ", numLeaves = " << node->numLeaves << std::endl;
}

#define DEBUG

void Tree::optimize( int numLeaves ) {
    auto q = findAllNodesWithNumLeaves( root, numLeaves );

#ifdef DEBUG
    for ( auto& node : q ) {
        printNode( node );
    }
#endif

    while ( !q.empty() ) {
    }
}

Tree::Node* Tree::findPermutationAndReplaceByEquivalentNode( Node* node ) {
    auto leaves = findAllLeaves( node );

    struct computeNode {
        std::deque<Tree::Node> nodes;
        int                    leftPointer;
        int                    rightPointer;
    };

    return (Node*)nullptr;
}

std::deque<Tree::Node*> Tree::findAllNodes(
    Node* node, std::function<bool( Node* )> const& filter ) {
    std::deque<Tree::Node*> q;

    if ( node == nullptr ) return q;

    if ( filter( node ) ) {
        q.push_back( node );
    }

    auto q_left = findAllNodes( node->left, filter );
    q.insert( q.end(), q_left.begin(), q_left.end() );

    auto q_right = findAllNodes( node->right, filter );
    q.insert( q.end(), q_right.begin(), q_right.end() );

    return q;
}

std::deque<Tree::Node*> Tree::findAllLeaves( Node* node ) {
    return findAllNodes( node, []( Node* node ) { return node->isLeaf; } );
}

std::deque<Tree::Node*> Tree::findAllNodesWithNumLeaves( Node* node,
                                                         int   numLeaves ) {
    return findAllNodes( node, [numLeaves]( Node* node ) {
        return node->numLeaves == numLeaves;
    } );
}
