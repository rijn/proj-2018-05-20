#include "tree.hpp"

struct sink {
    int id;
};

#define STree Tree<sink>

int main() {
    auto tree = new STree();

    tree->root                            = new STree::Node( -1 );
    tree->root->left                      = new STree::Node( -1 );
    tree->root->left->left                = new STree::Node( -1 );
    tree->root->left->left->left          = new STree::Node( -1 );
    tree->root->left->left->left->left    = new STree::Node( 1 );
    tree->root->left->left->left->right   = new STree::Node( 2 );
    tree->root->left->left->right         = new STree::Node( -1 );
    tree->root->left->left->right->left   = new STree::Node( 3 );
    tree->root->left->left->right->right  = new STree::Node( 4 );
    tree->root->left->right               = new STree::Node( -1 );
    tree->root->left->right->left         = new STree::Node( 5 );
    tree->root->left->right->right        = new STree::Node( 6 );
    tree->root->right                     = new STree::Node( -1 );
    tree->root->right->left               = new STree::Node( -1 );
    tree->root->right->left->left         = new STree::Node( -1 );
    tree->root->right->left->left->left   = new STree::Node( 7 );
    tree->root->right->left->left->right  = new STree::Node( 8 );
    tree->root->right->left->right        = new STree::Node( -1 );
    tree->root->right->left->right->left  = new STree::Node( 9 );
    tree->root->right->left->right->right = new STree::Node( 10 );
    tree->root->right->right              = new STree::Node( -1 );
    tree->root->right->right->left        = new STree::Node( -1 );
    tree->root->right->right->left->left  = new STree::Node( 11 );
    tree->root->right->right->left->right = new STree::Node( 12 );
    tree->root->right->right->right       = new STree::Node( 13 );

    tree->update();
    tree->optimize(
        []( STree::Node a, STree::Node b ) { return a.id + b.id; },
        []( STree::Node a, STree::Node b ) { return STree::Node( 0 ); } );

    tree->print();

    return 0;
}
