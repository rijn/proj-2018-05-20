#include "tree.hpp"

int main() {
    auto tree = new Tree();

    tree->root                            = new Tree::Node( -1 );
    tree->root->left                      = new Tree::Node( -1 );
    tree->root->left->left                = new Tree::Node( -1 );
    tree->root->left->left->left          = new Tree::Node( -1 );
    tree->root->left->left->left->left    = new Tree::Node( 1 );
    tree->root->left->left->left->right   = new Tree::Node( 2 );
    tree->root->left->left->right         = new Tree::Node( -1 );
    tree->root->left->left->right->left   = new Tree::Node( 3 );
    tree->root->left->left->right->right  = new Tree::Node( 4 );
    tree->root->left->right               = new Tree::Node( -1 );
    tree->root->left->right->left         = new Tree::Node( 5 );
    tree->root->left->right->right        = new Tree::Node( 6 );
    tree->root->right                     = new Tree::Node( -1 );
    tree->root->right->left               = new Tree::Node( -1 );
    tree->root->right->left->left         = new Tree::Node( -1 );
    tree->root->right->left->left->left   = new Tree::Node( 7 );
    tree->root->right->left->left->right  = new Tree::Node( 8 );
    tree->root->right->left->right        = new Tree::Node( -1 );
    tree->root->right->left->right->left  = new Tree::Node( 9 );
    tree->root->right->left->right->right = new Tree::Node( 10 );
    tree->root->right->right              = new Tree::Node( -1 );
    tree->root->right->right->left        = new Tree::Node( -1 );
    tree->root->right->right->left->left  = new Tree::Node( 11 );
    tree->root->right->right->left->right = new Tree::Node( 12 );
    tree->root->right->right->right       = new Tree::Node( 13 );

    tree->update();
    tree->optimize();

    tree->print();

    return 0;
}
