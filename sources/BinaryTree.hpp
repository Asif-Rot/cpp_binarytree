#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stack>
#include <queue>

using namespace std;

namespace ariel {

    template<class T>
    class BinaryTree {

        struct Node {
            T data;
            Node* left = nullptr;
            Node* right = nullptr;
            Node(T a): data(a), left(nullptr), right(nullptr) {}
        };
        Node *root = nullptr;
        
        public:
            BinaryTree() : root(nullptr) {}

            ~BinaryTree() {
                if (root != nullptr) {
                    delete root;
                }
            }

            void copyTree(Node &newroot, const Node &thisroot) {
                if (thisroot.left != nullptr) {
                    newroot.left = new Node(thisroot.left->data);
                    copyTree(*newroot.left, *thisroot.left);
                }

                if (thisroot.right != nullptr) {
                    newroot.right = new Node(thisroot.right->data);
                    copyTree(*newroot.right, *thisroot.right);
                }
            }

            BinaryTree(BinaryTree &other) {
                root = new Node(other.root->data);
                copyTree(*root, *other.root);
            }

            BinaryTree(BinaryTree &&other) noexcept {
                root = other.root;
                other.root = nullptr;
            }   

            BinaryTree &operator=(BinaryTree other) {
                if (this != &other) {
                    delete root;
                    root = new Node(other.root->data);
                    copyTree(*root, *other.root);
                }
                
                return *this;
            }

            BinaryTree &operator=(BinaryTree &&other) noexcept {
                delete root;
                root = other.root;
                other.root = nullptr;
            }

            BinaryTree &search_parent(const T& parent, const T& child, bool isleft) {
                if (root == NULL) {
                    throw invalid_argument("No such root");
                }

                stack<Node*> nodeStack_search;
                Node *node = root;
            
                while (node != nullptr || !nodeStack_search.empty()) {
                    while (node != nullptr) {
                        nodeStack_search.push(node);
                        node = node->left;
                    }
                    node = nodeStack_search.top();
                    nodeStack_search.pop();

                    if (node->data == parent) {
                        if (isleft) {
                            if(node->left == nullptr) {
                                node->left = new Node(child);
                            } else {
                                node->left->data = child;
                            }
                        }
                        else {
                            if(node->right == nullptr) {
                                node->right = new Node(child);
                            } else {
                                node->right->data = child;
                            }
                        }
                        return *this;
                    }

                    node = node->right;
                }
                throw invalid_argument("There is no such parent");
            }


            BinaryTree &add_root(const T& data) {
                if (root == nullptr) {
                    root = new Node(data);
                } else {
                    root->data = data;
                }
                return *this;
            }

            BinaryTree &add_left(const T& rootdata, const T& leftdata) {
                search_parent(rootdata, leftdata, true);
                return *this;
            }

            BinaryTree &add_right(const T& rootdata, const T& rightdata) {
                search_parent(rootdata, rightdata, false);
                return *this;
            }

            class iterator{

                private:
                    int orderof;
                    stack<Node*> nodeStack;
                    queue<Node*> q_print;
                    stack<Node*> s_print;
                    Node* p_node;

                public:

                    iterator(int order, Node* ptr=nullptr) : p_node(ptr) {
                        orderof = order;
                        if(p_node != nullptr){
                            
                            if(order == 0){ //preorder
                                if(p_node->right != nullptr){
                                    nodeStack.push(p_node->right);
                                }
                                if(p_node->left!=nullptr){
                                    nodeStack.push(p_node->left);
                                }
                            }
                            else if(order == 1){ //inorder
                                Node *tmp = p_node;

                                while (tmp != NULL || !nodeStack.empty()) {
                                    while (tmp !=  NULL) {
                                        nodeStack.push(tmp);
                                        tmp = tmp->left;
                                    }
                    
                                    tmp = nodeStack.top();
                                    
                                    q_print.push(tmp);
                                    nodeStack.pop();
                            
                                    tmp = tmp->right;
                                }
                                p_node = q_print.front();
                                q_print.pop();
                            }                            
                            
                            else { // postorder
                                nodeStack.push(p_node); 
                                while (!nodeStack.empty()){
                                    Node* tmp = nodeStack.top();
                                    nodeStack.pop();
                                    s_print.push(tmp);
                                    if (tmp->left) {
                                        nodeStack.push(tmp->left);
                                    }
                                    if (tmp->right) {
                                        nodeStack.push(tmp->right);
                                    }
                                }
                                p_node = s_print.top();
                                s_print.pop();
                            }    
                        }
                    }

                    T& operator*() const {
                        return p_node->data;
                    }

                    T* operator->() const {
                        return &(p_node->data);
                    }

                    // ++i;
                    iterator& operator++() {
                        if (!nodeStack.empty() || !q_print.empty() || !s_print.empty()){

                            if (orderof == 0){ // preorder
                                p_node = nodeStack.top();
                                nodeStack.pop();
                                if (p_node->right != nullptr) {
                                    nodeStack.push(p_node->right);
                                }
                                if (p_node->left != nullptr) {
                                    nodeStack.push(p_node->left);
                                }
                                return *this;
                            }

                            if (orderof == 1) { // inorder
                                p_node = q_print.front();
                                q_print.pop();
                                return *this;
                            }

                            if (orderof == 2) { // postorder
                                p_node = s_print.top();
                                s_print.pop();
                                return *this;
                            }
                        }

                        p_node = nullptr;
                        return *this;
                    }
                        
                    // i++;
                    iterator operator++(int) {
                        iterator tmp = *this;
                            if (!nodeStack.empty() || !q_print.empty() || !s_print.empty()){

                            if(orderof == 0) { // preorder
                                p_node = nodeStack.top();
                                nodeStack.pop();
                                if (p_node->right != nullptr){
                                    nodeStack.push(p_node->right);
                                }
                                if (p_node->left != nullptr){
                                    nodeStack.push(p_node->left);
                                }
                                return tmp;
                            }

                            if (orderof == 1) { // inorder
                                p_node = q_print.front();
                                q_print.pop();
                                return tmp;
                            }

                            if (orderof == 2) { // postorder
                                p_node = s_print.top();
                                s_print.pop();
                                return tmp;
                            }
                        }

                        p_node = nullptr;
                        return tmp;
                    }

                    bool operator==(const iterator& rhs) const {
                        return p_node == rhs.p_node;
                    }

                    bool operator!=(const iterator& rhs) const {
                        return p_node != rhs.p_node;
                    }
            };

            iterator begin_preorder() {
                return iterator{0, root};
            }

            iterator end_preorder() {
                return iterator{0, nullptr};
            }

            iterator begin_inorder() {
                return iterator{1, root};
            }
            
            iterator end_inorder() {
                return iterator{1, nullptr};
            }

            iterator begin_postorder() {
                return iterator{2, root};
            }
            
            iterator end_postorder() {
                return iterator{2, nullptr};
            }

            iterator begin() { // Need to change to inorder
                return iterator{1, root};
            }
            
            iterator end() { // Need to change to inorder
                return iterator{1, nullptr};
            }

            friend std::ostream& operator<< (std::ostream& os, const BinaryTree& b) {
                if (b.root == NULL) {
                    return os;
                }
 
                stack<Node*> nodeStack;
                nodeStack.push(b.root);
            
                while (nodeStack.empty() == false) {
                    Node* node = nodeStack.top();
                    os << node->data << " ";
                    nodeStack.pop();
            
                    if (node->right) {
                        nodeStack.push(node->right);
                    }
                    if (node->left) {
                        nodeStack.push(node->left);
                    }
                }

                return os;
            }

    };
};