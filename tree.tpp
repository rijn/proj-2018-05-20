#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stack>

#include "tree.hpp"

template <class T>
void Tree<T>::update() {
    updateDepthHelper( root, 0 );
    updateIsLeafHelper( root );
    updateNumLeavesHelper( root );
    root->updateParentHelper( nullptr );
}

template <class T>
void Tree<T>::updateDepthHelper( Node* node, int depth ) {
    if ( node == nullptr ) return;
    node->depth = depth;
    updateDepthHelper( node->left, depth + 1 );
    updateDepthHelper( node->right, depth + 1 );
}

template <class T>
void Tree<T>::updateIsLeafHelper( Node* node ) {
    if ( node == nullptr ) return;
    if ( node->left == nullptr && node->right == nullptr ) {
        node->isLeaf = true;
    } else {
        node->isLeaf = false;
        updateIsLeafHelper( node->left );
        updateIsLeafHelper( node->right );
    }
}

template <class T>
int Tree<T>::updateNumLeavesHelper( Node* node ) {
    if ( node == nullptr ) return 0;
    if ( node->isLeaf ) {
        node->numLeaves = 1;
    } else {
        node->numLeaves = updateNumLeavesHelper( node->left ) +
                          updateNumLeavesHelper( node->right );
    }
    return node->numLeaves;
}

template <class T>
void Tree<T>::Node::updateParentHelper( Node* parent ) {
    this->parent = parent;
    if ( this->left != nullptr ) this->left->updateParentHelper( this );
    if ( this->right != nullptr ) this->right->updateParentHelper( this );
}

template <class T>
void Tree<T>::print() {
    printHelper( root, 0 );
}

template <class T>
void Tree<T>::printHelper( Node* p, int indent /* = 0 */ ) {
#if 1
    if ( p != nullptr ) {
        if ( p->right ) {
            printHelper( p->right, indent + 4 );
        }
        if ( indent ) {
            std::cout << std::setw( indent ) << ' ';
        }
        if ( p->right ) std::cout << " /\n" << std::setw( indent ) << ' ';
        std::cout << p->id << "\n ";
        if ( p->left ) {
            std::cout << std::setw( indent ) << ' ' << " \\\n";
            printHelper( p->left, indent + 4 );
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

template <class T>
void Tree<T>::printNode( Node* node ) {
    if ( node == nullptr ) {
        std::cout << "nullptr" << std::endl;
        return;
    }
    std::cout << node->id << ", depth = " << node->depth
              << ", isLeaf = " << node->isLeaf
              << ", numLeaves = " << node->numLeaves << std::endl;
}

#define DEBUG

template <class T>
void Tree<T>::optimize(
    std::function<float( Tree::Node, Tree::Node )>      distance,
    std::function<Tree::Node( Tree::Node, Tree::Node )> merge, int numLeaves ) {
    auto q = findAllNodesWithNumLeaves( root, numLeaves );

#ifdef DEBUG
    for ( auto& node : q ) {
        printNode( node );
    }
#endif

    while ( !q.empty() ) {
        auto& node = q.front();

        auto equivalentNode =
            findPermutationAndReplaceByEquivalentNode( node, distance, merge );

        equivalentNode->parent    = node->parent;
        equivalentNode->numLeaves = numLeaves;
        if ( node->parent != nullptr ) {
            if ( node->parent->left == node ) {
                node->parent->left = equivalentNode;
            }
            if ( node->parent->right == node ) {
                node->parent->right = equivalentNode;
            }
        }
        if ( root == node ) {
            root = equivalentNode;
        }

        // TODO: dispose node

        updateNumLeavesUpward( equivalentNode, numLeaves - 1 );

        pushPossibleNodes( q, equivalentNode, numLeaves );

        q.pop_front();

        // std::cout << "tree after merge" << std::endl;
        // printHelper( root );
        // std::cout << "-----" << std::endl;
    }
}

template <class T>
void Tree<T>::pushPossibleNodes( std::deque<Tree::Node*>& q, Node* node,
                                 int numLeaves ) {
    if ( node == nullptr ) return;
    if ( node->numLeaves == numLeaves ) {
        q.push_back( node );
    }
    pushPossibleNodes( q, node->parent, numLeaves );
}

template <class T>
typename Tree<T>::Node* Tree<T>::findPermutationAndReplaceByEquivalentNode(
    Node* node, std::function<float( Tree::Node, Tree::Node )> distance,
    std::function<Tree::Node( Tree::Node, Tree::Node )> merge ) {
    auto leaves = findAllLeaves( node );

    struct computeNode {
        typename std::deque<Tree<T>::Node>           nodes;
        typename std::deque<Tree<T>::Node>::iterator leftIterator;
        typename std::deque<Tree<T>::Node>::iterator rightIterator;
        float                                        cost;
    };

    float minimalCost    = std::numeric_limits<float>::max();
    Node* equivalentNode = nullptr;

    std::stack<computeNode> s;

    computeNode initialNode;
    for ( auto& node : leaves ) {
        initialNode.nodes.push_back( *node );
    }

    s.push( initialNode );

    s.top().leftIterator  = s.top().nodes.begin();
    s.top().rightIterator = s.top().nodes.begin();
    s.top().cost          = 0;

    while ( !s.empty() ) {
        auto& top = s.top();

        top.rightIterator++;

        if ( top.rightIterator == top.nodes.end() ) {
            top.leftIterator++;
            top.rightIterator = top.leftIterator;
            continue;
        }

        if ( top.leftIterator == top.nodes.end() ) {
            if ( top.nodes.size() == 1 ) {
                if ( top.cost < minimalCost ) {
                    minimalCost    = top.cost;
                    equivalentNode = new Node( top.nodes.front() );
                }
            }

            s.pop();

            continue;
        }

        // std::cout << top.nodes.size() << " ["
        //           << std::distance( top.nodes.begin(), top.leftIterator )
        //           << ", "
        //           << std::distance( top.nodes.begin(), top.rightIterator )
        //           << "]" << std::endl;

        computeNode nextNode;
        for ( auto it = top.nodes.begin(); it != top.nodes.end(); it++ ) {
            if ( it != top.leftIterator && it != top.rightIterator ) {
                nextNode.nodes.push_back( *it );
            }
        }

        nextNode.cost =
            top.cost + distance( *top.leftIterator, *top.rightIterator );
        nextNode.nodes.push_back(
            merge( *top.leftIterator, *top.rightIterator ) );

        s.push( nextNode );

        s.top().leftIterator  = s.top().nodes.begin();
        s.top().rightIterator = s.top().nodes.begin();
    }

    return equivalentNode;
}

template <class T>
void Tree<T>::updateNumLeavesUpward( Node* node, int diffNumLeaves ) {
    if ( node == nullptr ) return;
    node->numLeaves -= diffNumLeaves;
    updateNumLeavesUpward( node->parent, diffNumLeaves );
}

template <class T>
std::deque<typename Tree<T>::Node*> Tree<T>::findAllNodes(
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

template <class T>
std::deque<typename Tree<T>::Node*> Tree<T>::findAllLeaves( Node* node ) {
    return findAllNodes( node, []( Node* node ) { return node->isLeaf; } );
}

template <class T>
std::deque<typename Tree<T>::Node*> Tree<T>::findAllNodesWithNumLeaves(
    Node* node, int numLeaves ) {
    return findAllNodes( node, [numLeaves]( Node* node ) {
        return node->numLeaves == numLeaves;
    } );
}
