#pragma once
#include <functional>
#include <iostream>

template<typename T>
struct AVLTreeNode {
    T key;
    AVLTreeNode* left;
    AVLTreeNode* right;
    int height;

    AVLTreeNode(T k) : key(k), left(nullptr), right(nullptr), height(1) {}
};

template<typename T>
class AVLTree {
public:
    AVLTreeNode<T>* root;

    AVLTree() : root(nullptr) {}

    void insert(T key) {
        root = insertion(root, key);
    }
    void deletion(T key)
    {
        root = deleteNode(root, key);
    }
    int height(AVLTreeNode<T>* node) {
        if (node == nullptr)
        {
            return 0;
        }
        return node->height;
    }
    int balance_factor(AVLTreeNode<T>* node) {
        if (node == nullptr)
        {
            return 0;
        }
        return height(node->left) - height(node->right);
    }

    AVLTreeNode<T>* rotateRight(AVLTreeNode<T>* node) {
        if (node == nullptr || node->left == nullptr) {
            return node; // Rotation not possible, return the node as is
        }

        AVLTreeNode<T>* A = node->left;
        AVLTreeNode<T>* B = A->right;

        A->right = node;
        node->left = B;

        node->height = 1 + std::max(height(node->left), height(node->right));
        A->height = 1 + std::max(height(A->left), height(A->right));

        return A;
    }

    AVLTreeNode<T>* rotateLeft(AVLTreeNode<T>* node) {
        if (node == nullptr || node->right == nullptr) {
            return node; // Rotation not possible, return the node as is
        }
        AVLTreeNode<T>* A = node->right;
        AVLTreeNode<T>* B = A->left;

        A->left = node;
        node->right = B;

        node->height = 1 + std::max(height(node->left), height(node->right));
        A->height = 1 + std::max(height(A->left), height(A->right));

        return A;
    }

    AVLTreeNode<T>* insertion(AVLTreeNode<T>* node, T data) {
        if (node == nullptr) {
            return new AVLTreeNode<T>(data);
        }

        if (data < node->key) {
            node->left = insertion(node->left, data);
        }
        else if (data > node->key) {
            node->right = insertion(node->right, data);
        }
        else {
            return node;
        }

        node->height = 1 + std::max(height(node->left), height(node->right));

        int balance = balance_factor(node);

        // LL
        if (balance == 2 && balance_factor(node->left) == 1) {
            return rotateRight(node);
        }

        // RR
        if (balance == -2 && balance_factor(node->left) == -1) {
            return rotateLeft(node);
        }

        // LR
        if (balance == -2 && balance_factor(node->left) == 1) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RL
        if (balance == 2 && balance_factor(node->left) == -1) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLTreeNode<T>* deleteNode(AVLTreeNode<T>* node, T data) {
        if (node == nullptr) {
            return node;
        }

        if (data < node->key) {
            node->left = deleteNode(node->left, data);
        }
        else if (data > node->key) {
            node->right = deleteNode(node->right, data);
        }
        else {
            if (node->left == nullptr || node->right == nullptr) {
                AVLTreeNode<T>* temp = node->left ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else
                    *node = *temp;
                delete temp;
            }
            else {
                AVLTreeNode<T>* temp = minValueNode(node->right);
                node->key = temp->key;
                node->right = deleteNode(node->right, temp->key);
            }
        }

        if (node == nullptr) {
            return node;
        }

        node->height = 1 + std::max(height(node->left), height(node->right));

        int balance = balance_factor(node);

        // LL
        if (balance > 1 && balance_factor(node->left) >= 0)
            return rotateRight(node);

        // LR
        if (balance > 1 && balance_factor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        // RR
        if (balance < -1 && balance_factor(node->right) <= 0)
            return rotateLeft(node);

        // RL
        if (balance < -1 && balance_factor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    AVLTreeNode<T>* minValueNode(AVLTreeNode<T>* node) {
        AVLTreeNode<T>* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    void inorder() {
        inorderTraversal(root);
    }

    void inorder(std::function<void(T&)> func) {
        inorderTraversal(root, func);
    }

    void inorderTraversal(AVLTreeNode<T>* node) {
        if (node == nullptr) return;
        inorderTraversal(node->left);
        std::cout << node->key << " ";
        inorderTraversal(node->right);
    }

    void inorderTraversal(AVLTreeNode<T>* node, std::function<void(T&)> func) {
        if (node != nullptr) {
            inorderTraversal(node->left, func);
            func(node->key);
            inorderTraversal(node->right, func);
        }
    }
};
