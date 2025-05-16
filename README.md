# BST-AVL-SPLAY

Swarm Binary Search Tree Implementation
UMBC - CMSC 341 - Fall 2024 - Project 2
Overview
This project implements a binary search tree (BST) system to manage robot identifiers in a swarm. 
The implementation supports three different tree types:

Standard Binary Search Tree (BST)
AVL Tree (self-balancing)
Splay Tree (recently accessed elements move to root)

Features

Dynamic Tree Type: Switch between BST, AVL, and Splay tree implementations at runtime
Robot Management: Insert and remove robots by unique IDs
Self-Balancing: AVL implementation maintains balanced heights
Recently-Used Access Pattern: Splay implementation optimizes for frequently accessed robots
Tree Visualization: Dump functionality to display the current tree structure

Implementation Details
BST

Standard binary search tree where nodes are arranged by ID
Each node has 0-2 children, with left child < parent < right child
No balancing guarantees

AVL

Self-balancing BST that maintains height property
No leaf is more than one level different from siblings
Implements rotations (left, right) to maintain balance after operations
Height information is tracked for each node

Splay

Recently accessed nodes are rotated to the root
Improves performance for frequently accessed items
Implements zig-zag and zig-zig rotations


Time Complexity

BST:

Average: O(log n) for insert/remove/search
Worst: O(n) for unbalanced trees


AVL:

O(log n) for all operations (guaranteed)


Splay:

Amortized O(log n) for all operations
Individual operations may take O(n) time


