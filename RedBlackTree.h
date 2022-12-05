using namespace std;
#include <iostream>
#include <vector>

struct TreeNode {
  int numMatching;
  string name;
  TreeNode *left = nullptr;
  TreeNode *right = nullptr;
  TreeNode *parent = nullptr;
  bool color;
};

class
    RedBlackTree { // based on pseudocode from class slides and
                   // help from geeksforgeeks explanation
                   // https://www.geeksforgeeks.org/insertion-in-red-black-tree/
  TreeNode *root;

public:
  RedBlackTree() { root = nullptr; }
  TreeNode *getRoot() { return root; }

  void rotateLeft(TreeNode *node) {
    TreeNode *temp = new TreeNode();
    if (node->right->left) {
      temp->right = node->right->left;
    }

    // temp is "new" version of node
    temp->left = node->left;
    temp->numMatching = node->numMatching;
    temp->name = node->name;
    temp->color = node->color;
    // node becomes its right child
    node->numMatching = node->right->numMatching;
    node->name = node->right->name;
    node->color = node->right->color;
    // attach node and temp
    node->left = temp;
    temp->parent = node;
    // if temp has children, ensure they have temp as parent
    if (temp->left) {
      temp->left->parent = temp;
    }
    if (temp->right) {
      temp->right->parent = temp;
    }

    // since node became its right child, ignore the original right child
    if (node->right->right) {
      node->right = node->right->right;
    } else {
      node->right = nullptr;
    }

    if (node->right) {
      node->right->parent = node;
    }
  }

  void rotateRight(TreeNode *node) {
    TreeNode *temp = new TreeNode();
    if (node->left->right) {
      temp->left = node->left->right;
    }
    // temp is "new" version of node
    temp->right = node->right;
    temp->numMatching = node->numMatching;
    temp->name = node->name;
    temp->color = node->color;
    // node becomes its left child
    node->numMatching = node->left->numMatching;
    node->name = node->left->name;
    node->color = node->left->color;
    // attach node and temp
    node->right = temp;
    temp->parent = node;
    // if temp has children, ensure they have temp as parent
    if (temp->left) {
      temp->left->parent = temp;
    }
    if (temp->right) {
      temp->right->parent = temp;
    }

    // since node became its left child, ignore the original left child
    if (node->left->left) {
      node->left = node->left->left;
    } else {
      node->left = nullptr;
    }

    if (node->left) {
      node->left->parent = node;
    }
  }

  void balance(TreeNode *temp) {

    while (temp->parent->color) { // if parent is red & not null
      TreeNode *grandparent = temp->parent->parent;
      TreeNode *uncle = root;
      if (temp->parent == grandparent->left) { // parent is left, uncle is right
        if (grandparent->right) {              // if there is an uncle
          uncle = grandparent->right;
        }
        if (uncle->color) { // uncle is red, flip colors
          temp->parent->color = false;
          uncle->color = false;
          grandparent->color = true;
          if (grandparent->numMatching != root->numMatching) {
            temp = grandparent; // grandparent isn't root, continue
          } else { // grandparent is root, break and color root black
            break;
          }
        }                                            // uncle is black, rotate
        else if (temp == grandparent->left->right) { // left right
          rotateLeft(temp->parent);
        } else { // left left
          temp->parent->color = false;
          grandparent->color = true;
          rotateRight(grandparent);
          if (grandparent->numMatching != root->numMatching) {
            temp = grandparent; // grandparent isn't root, continue
          } else { // grandparent is root, break and color root black
            break;
          }
        }
      } else {                   // parent is right, uncle is left
        if (grandparent->left) { // if there is an uncle
          uncle = grandparent->left;
        }
        if (uncle->color) { // uncle is red, flip colors
          temp->parent->color = false;
          uncle->color = false;
          grandparent->color = true;
          if (grandparent->numMatching != root->numMatching) {
            temp = grandparent; // grandparent isn't root, continue
          } else { // grandparent is root, break and color root black
            break;
          }
        }                                            // uncle is black, rotate
        else if (temp == grandparent->right->left) { // right left
          rotateRight(temp->parent);
        } else { // right right
          temp->parent->color = false;
          grandparent->color = true;
          rotateLeft(grandparent);
          if (grandparent->numMatching != root->numMatching) {
            temp = grandparent; // grandparent isn't root, continue
          } else { // grandparent is root, break and color root black
            break;
          }
        }
      }
    }
    root->color = false; // ensure root stays black
  }

  void insert(string name, int numMatching) { // iterative insert

    if (root == nullptr) { // doesn't exist, add
      root = new TreeNode();
      root->parent = nullptr;
      root->name = name;
      root->numMatching = numMatching;
      root->color = false;
    } else {
      TreeNode *parent = root;
      TreeNode *temp = new TreeNode();
      temp->numMatching = numMatching;
      temp->name = name;

      while (parent != nullptr) {
        if (numMatching <
            parent->numMatching) { // smaller than current, needs to go left
          if (parent->left != nullptr) { // something to left, keep checking
            parent = parent->left;
          } else { // nothing to left, insert
            parent->left = temp;
            temp->parent = parent;
            temp->color = true;
            break;
          }
        } else { // bigger than/equal to current, needs to go right
          if (parent->right != nullptr) { // something to right, keep checking
            parent = parent->right;
          } else { // nothing to right, insert
            parent->right = temp;
            temp->parent = parent;
            temp->color = true;
            break;
          }
        }
      }
      balance(temp); // fix balance and colors
    }
  }

  void printInorder(TreeNode *node, int &count,
                    vector<pair<string, int>> &top16) {
    if (node == nullptr || count == 16) {
      return;
    }
    // since printing biggest first, reverse inorder
    printInorder(node->right, count, top16);
    if (count < 16) {
      pair<string, int> temp = make_pair(node->name, node->numMatching);
      top16.push_back(temp);
      count++;
    }
    if (count < 16) {
      printInorder(node->left, count, top16);
    }
  }
};