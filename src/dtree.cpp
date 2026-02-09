#include "dtree.h"
#include <iostream>

int classify(node* root, const Table& dtable, int idx) {
    node* ptr = root;
    while (std::holds_alternative<test>(ptr->val)) {
        test t = std::get<test>(ptr->val);
        int attr_value = dtable.data.at(idx).at(t.a);
        ptr = (attr_value == t.v) ? ptr->right : ptr->left;
    }
    return std::get<int>(ptr->val);
}

void free_tree(node* root) {
    if (root == nullptr) return;
    free_tree(root->left);
    free_tree(root->right);
    delete root;
}

void print_tree(node* root, int depth = 0) {
    if (root == nullptr) return;
    for (int i = 0; i < depth; ++i) {
        std::cout << "  ";
    }
    if (std::holds_alternative<test>(root->val)) {
        test t = std::get<test>(root->val);
        std::cout << "Test(Attribute: " << t.a << ", Value: " << t.v << ")\n";
        print_tree(root->left, depth + 1);
        print_tree(root->right, depth + 1);
    } else {
        int class_label = std::get<int>(root->val);
        std::cout << "Class: " << class_label << "\n";
    }
}

int height(node* root) {
    if (root == nullptr) return -1;
    int left_height = height(root->left);
    int right_height = height(root->right);
    return 1 + std::max(left_height, right_height);
}

int tree_size(node* root) {
    if (root == nullptr) return 0;
    return 1 + tree_size(root->left) + tree_size(root->right);
}
