#include "doctest.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

#include "BinaryTree.hpp"
using namespace ariel;


TEST_CASE("Good BinaryTree code") {
    BinaryTree<string> b_string;
    CHECK_NOTHROW(b_string.add_root("1"));
    CHECK_NOTHROW(b_string.add_root("2")); // root is now 2
    CHECK_NOTHROW(b_string.add_left("2", "8"));
    CHECK_NOTHROW(b_string.add_left("2", "7")); // root 2: left is now 7
    CHECK_NOTHROW(b_string.add_right("2", "5"));
    CHECK_NOTHROW(b_string.add_left("7", "9"));
    CHECK_NOTHROW(b_string.add_right("7", "4")); // parent 7: right is 4
    /**
     *         2
     *       /   \
     *      7     5
     *     / \
     *    9   4
     */

    string preorderArr[] = {"2", "7", "9", "4", "5"};
    string inorderArr[] = {"9", "7", "4", "2", "5"};
    string postorderArr[] = {"9", "4", "7", "5", "2"};

    int i = 0;
    for (auto it=b_string.begin_preorder(); it!=b_string.end_preorder(); ++it) {
        CHECK((*it) == preorderArr[i]);
        i++;
    }

    i = 0;
    for (auto it=b_string.begin_preorder(); it!=b_string.end_preorder(); it++) {
        CHECK((*it) == preorderArr[i]);
        i++;
    }

    i = 0;
    for (auto it=b_string.begin_inorder(); it!=b_string.end_inorder(); ++it) {
        CHECK((*it) == inorderArr[i]);
        i++;
    }

    i = 0;
    for (auto it=b_string.begin_postorder(); it!=b_string.end_postorder(); ++it) {
        CHECK((*it) == postorderArr[i]);
        i++;
    }

    // demonstrate the arrow operator:
    i = 0;
    for (auto it=b_string.begin_postorder(); it!=b_string.end_postorder(); ++it) {
        CHECK(it->size() == postorderArr[i].size());
        i++;
    }

    i = 0;
    for (const string& element: b_string) {  // this should work like inorder
        CHECK(element == inorderArr[i]);
        i++;
    }  
    
}


TEST_CASE("Bad BinaryTree code") {
    BinaryTree<int> b_int;
    CHECK_NOTHROW(b_int.add_root(1));
    CHECK_NOTHROW(b_int.add_root(1)); // same value twice
    CHECK_THROWS(b_int.add_left(2, 8)); // 2 is not the root
    CHECK_NOTHROW(b_int.add_right(1, 5));
    CHECK_THROWS(b_int.add_right(4, 2)); // 4 is not a parent in the tree
    CHECK_NOTHROW(b_int.add_root(4)); // Change root to 4
    CHECK_THROWS(b_int.add_left(1, 8)); // 1 is not the root anymore
    CHECK_THROWS(b_int.add_right(1, 3)); // 1 is not the root anymore
    CHECK_NOTHROW(b_int.add_right(4, 2)); // Can change to 2!
    CHECK_THROWS(b_int.add_right(5, 6)); // no 5
    CHECK_THROWS(b_int.add_left(19, 42));// no 19
    CHECK_NOTHROW(b_int.add_root(7));
    CHECK_NOTHROW(b_int.add_left(7, 6));

    /**
     *          7
     *        /   \
     *       6     2
     */

    int postorderArr[] = {6, 2, 7};

    int i = 0;
    bool c;
    for (auto it=b_int.begin_preorder(); it!=b_int.end_preorder(); ++it) {
        CHECK_FALSE((*it) == postorderArr[i]);
        i++;
    }

    CHECK_THROWS(b_int.add_right(4, 6)); // 4 is not the root anymore
    CHECK_NOTHROW(cout << b_int << endl);

}