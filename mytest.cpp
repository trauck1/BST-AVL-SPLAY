// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std;
enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(){}
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }
    void init(int min, int max){
        m_min = min;
        m_max = max;
        m_type = UNIFORMINT;
        m_generator = std::mt19937(10);// 10 is the fixed seed value
        m_unidist = std::uniform_int_distribution<>(min,max);
    }
    void getShuffle(vector<int> & array){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // this function provides a list of all values between min and max
        // in a random order, this function guarantees the uniqueness
        // of every value in the list
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }

    string getRandString(int size){
        // the parameter size specifies the length of string we ask for
        // to use ASCII char the number range in constructor must be set to 97 - 122
        // and the Random type must be UNIFORMINT (it is default in constructor)
        string output = "";
        for (int i=0;i<size;i++){
            output = output + (char)getRandNum();
        }
        return output;
    }
    
    int getMin(){return m_min;}
    int getMax(){return m_max;}
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution

};

class Tester{
    public:
        //this function inserts 300 nodes into an avl tree
        //returns true if 300 nodes were inserted and the tree is correctly balanced
        //checks balance and number of nodes by calling balanceChecker and nodeCounter
        bool avlTreeInsertAndBalanceTest();
        //This function creates a bst tree object and inserts 20 nodes
        //then tests the bst properties by traversing to each node and making sure the left node is less and 
        //the right node is greater
        bool bstPropertyTest();
        //This function creates a splay tree object and inserts 10 nodes
        //this function tests if all the nodes inserted were rotated to the root
        //also tests the values of each nodes children
        bool splayPropertyTest();
        //this function tests the height of a splay tree object
        //traveres through each node in the tree making sure at least one 
        //of the children's height is one less than the parent's height
        bool splayHeightTest();
        //this function tests the remove function by creating a BST with multiple nodes
        //then trying different cases for the remove function like removing a leaf, a node with children, 
        //and removing the root. Tests the amount of nodes, the height of each node and 
        //checks to see if the node is no longer in the tree
        bool removeBstTest();
        //this function tests the remove function on edge cases
        //these edge cases are trying to remove a node not in the 
        bool edgeCaseRemovalTest();
        //this function adds 300 nodes into an avl tree, checks the balance,
        //then removes 300 and checks if the tree is still balanced
        bool avlBalanceAfterRemovalTest();
        //This functions adds 20 nodes into an avl tree and a bst tree, then 
        //tests the balance of the avl and the bst properties of the bst
        //then removes 10 nodes and test if the avl is still balanced and the bst maintains its properties
        //the properties for BST are that the left nodes of a parent are always less than and the right nodes are always greater than
        //also tests the height of both the AVL and BST trees
        bool propertyTest();
        //this function creates a BST tree with 10 nodes, then makes a copy of that BST tree
        //then tests to make sure the trees have the same attributes at different locations
        bool assignmentOperatorNormalTest();
        //this function tests what happens when the copy constructor is called on an empty tree
        //this function tests the type of the trees and if the tree is still empty
        bool assignmentOperatorErrorTest();
    private:
        //this is a helper function that recursively checks the balance of each node
        //by comparing the height of the children
        bool balanceChecker(Robot *curr);
        //this is a helper function that recursively traverses through the tree counting each node
        //and returning the total
        int nodeCounter(int count, Robot *curr);
        //this is a helper function that checks if the tree has bst properties
        //it tests the bst properties by making sure each nodes left child is less than and 
        //each right child is greater than
        bool bstPropertyHelper(Robot* curr);
        //this is a helper function that traverses through the tree making sure there isn't any gaps in the height
        //for example, if the nodes height is 2, at least one of it's children has to have a height of 1.
        bool heightHelper(Robot * curr);
        //recursively traverses from the given node and below, looking for a specific node returning true if it is found
        //and false if not found
        bool nodeFinder(int id, Robot * curr);
        //traverses through two trees comparing the nodes in the trees
        bool assignmentOperatorHelper(Robot * copyCurr, Robot * originalCurr);

};

int main(){
    Random idGen(MINID,MAXID);
    Tester tester;
    const int testTotal = 10;
    int passCounter = 0;
    if(tester.avlTreeInsertAndBalanceTest()){
        cout << "AVL balance test has passed!" << endl;
        passCounter++;
    }else{
        cout << "AVL balance test has failed..." << endl;
    }
    if(tester.bstPropertyTest()){
        cout << "BST property test has passed!" << endl;
        passCounter++;
    }else{
        cout << "BST property test has failed..." << endl;
    }
    if(tester.splayPropertyTest()){
        cout << "Splay property test has passed!" << endl;
        passCounter++;
    }else{
        cout << "Splay property test has failed..." << endl;
    }
    if(tester.splayHeightTest()){
        cout << "Splay height test has passed!" << endl;
        passCounter++;
    }else{
        cout << "Splay height test has failed..." << endl;
    }
    if(tester.removeBstTest()){
        cout << "BST removal test has passed!" << endl;
        passCounter++;
    }else{
        cout << "BST removal test has failed..." << endl;
    }
    if(tester.edgeCaseRemovalTest()){
        cout << "Edge case removal test has passed!" << endl;
        passCounter++;
    }else{
        cout << "Edge case removal test has failed..." << endl;
    }
    if(tester.avlBalanceAfterRemovalTest()){
        cout << "AVL balance after removal test has passed!" << endl;
        passCounter++;
    }else{
        cout << "AVL balance after removal test has failed..." << endl;
    }
    if(tester.propertyTest()){
        cout << "AVL and BST property and balance after removal test has passed!" << endl;
        passCounter++;
    }else{
        cout << "AVL and BST property and balance after removal test has failed..." << endl;
    }
    if(tester.assignmentOperatorNormalTest()){
        cout << "Assignment operator normal test has passed!" << endl;
        passCounter++;
    }else{
        cout << "Assignment operator normal test has failed..." << endl;
    }
    if(tester.assignmentOperatorErrorTest()){
        cout << "Assignment operator error test has passed!" << endl;
        passCounter++;
    }else{
        cout << "Assignment operator error test has failed..." << endl;
    }
    //if all the tests have passed
    if(passCounter == testTotal){
        cout << "**All tests have passed**" << endl;
    }else{
        cout << "some test has failed.." << endl;
    }
    
    
    return 0;
}


bool Tester::avlTreeInsertAndBalanceTest(){
    Random idGen(MINID,MAXID);
    Swarm avlTree(AVL);
    int randomID;
    int nodesVisited;
    //inserts 300 nodes into an avl tree, balances after each insertion
    for(int i = 0; i < 300; i++){
        randomID = idGen.getRandNum();
        Robot robot(randomID,DEFAULT_TYPE);
        avlTree.insert(robot);
    }
    //calls the balanceChecker helper function to make sure the AVL tree is balanced
    bool balanceAndCorrectNumberOfNodes = balanceChecker(avlTree.m_root);
    //calls the node counter helper function to check if the correct amount of nodes have been inserted
    nodesVisited = nodeCounter(nodesVisited, avlTree.m_root);
    //if the tree has the correct amount of nodes and is balanced, return true
    if(nodesVisited == 300 && balanceAndCorrectNumberOfNodes == true){
        return true;
    }
    return false;
}

bool Tester::balanceChecker(Robot *curr){
    bool balance = true;
    if(curr != nullptr){
        //if the function has a left and right child compare the height difference of the two
        if(curr->getRight() != nullptr && curr->getLeft() != nullptr){
            int difference = curr->getLeft()->getHeight() - curr->getRight()->getHeight();
            //if the difference isn't in the range -1 to 1, invalid height difference
            if(difference >1 || difference < -1){
                balance = false;
            }
            if(balance == true){
                return true;
            }else{
                return false;
            }
        //if the node only has a right child
        }else if(curr->getLeft() == nullptr && curr->getRight() != nullptr){
            //if the nodes height is greater than two and there is no left child, the tree is imbalanced
            if(curr->getRight()->getHeight() > 2){
                return false;
            }
        //same as the last if else statement, except if the node only has a left child
        }else if(curr->getLeft() != nullptr && curr->getRight() == nullptr){
            if(curr->getLeft()->getHeight() > 2){
                return false;
            }
        }
        //checks the balance of the left nodes then the right nodes
        bool leftBalance = balanceChecker(curr->getLeft());
        bool rightBalance = balanceChecker(curr->getRight());
        if(rightBalance == false || leftBalance == false){
            balance = false;
        }
    }
    return true;
}

int Tester::nodeCounter(int count, Robot *curr){
    //if the node exists, add one to the count, then check the children
    if(curr != nullptr){
        count++;
        count = nodeCounter(count, curr->getLeft());
        count = nodeCounter(count, curr->getRight());
    }
    return count;
}

bool Tester::bstPropertyTest(){
    Swarm bstTree(BST);
    Random idGen(MINID,MAXID);
    int randomID;
    int nodesVisited = 0;
    //inserts 20 random nodes
    for(int i = 0; i < 20; i++){
        randomID = idGen.getRandNum();
        Robot robot(randomID,DEFAULT_TYPE);
        bstTree.insert(robot);
    }
    //counts the nodes in the tree
    nodesVisited = nodeCounter(nodesVisited, bstTree.m_root);
    //checks the height of all the nodes
    bool validBstTree = bstPropertyHelper(bstTree.m_root);
    if(validBstTree && nodesVisited == 20){
        return true;
    }
    return false;
}

bool Tester::bstPropertyHelper(Robot* curr){
    bool validNode = true;
    if(curr !=nullptr){
        if(curr->getLeft() != nullptr){
            //checks the left node is less than the current
            if(curr->getID() < curr->getLeft()->getID()){
                validNode = false;
            }
        }
        if(curr->getRight() != nullptr){
            //checks if the right node is greater than the current
            if(curr->getID() > curr->getRight()->getID()){
                validNode = false;
            }
        }
        //traverses to the left nodes, then the right nodes
        bool validLeftNode = bstPropertyHelper(curr->getLeft());
        bool validRightNode = bstPropertyHelper(curr->getRight());
        
        if(validNode && validLeftNode && validRightNode){
            return true;
        }else{
            return false;
        }
    }
    return true;
}

bool Tester::splayPropertyTest(){
    Swarm splayTree(SPLAY);
    Random idGen(MINID,MAXID);
    int randomID;
    bool insertAtRoot = true;
    bool hasBstProperties = true;
    bool hasBstPropertiesHelper = true;
    //inserts 10 nodes into the splay tree
    for(int i = 0; i < 10; i++){
        randomID = idGen.getRandNum();
        Robot robot(randomID,DEFAULT_TYPE);
        splayTree.insert(robot);
        //checks that the newly inserted node is the new root
        if(splayTree.m_root->getID() != randomID){
            insertAtRoot = false;
        }
        //checks if the left node is less and right node is greater
        hasBstPropertiesHelper = bstPropertyHelper(splayTree.m_root);
        if(hasBstPropertiesHelper == false){
            hasBstProperties = false;
        }
    }
    if(hasBstProperties && insertAtRoot){
        return true;
    }
    return false;
}

bool Tester::splayHeightTest(){
     Swarm splayTree(SPLAY);
    Random idGen(MINID,MAXID);
    int randomID;
    //inserts 50 nodes into a splay tree
    for(int i = 0; i < 50; i++){
        randomID = idGen.getRandNum();
        Robot robot(randomID,DEFAULT_TYPE);
        splayTree.insert(robot);
    }
    //checks if the nodes have valid heights
    bool validHeights = heightHelper(splayTree.m_root);
    if(validHeights){
        return true;
    }
    return false;
}

bool Tester::heightHelper(Robot * curr){
    bool leftCheck = true;
    bool rightCheck = true;
    bool totalCheck = true;
    bool leftRecursion;
    bool rightRecursion;
    if(curr != nullptr){
        //if the node is a leaf, the height has to be zero
        if(curr->getRight() == nullptr && curr->getLeft() == nullptr){
            if(curr->getHeight() == 0){
                return true;
            }else{
                return false;
            }
        }else{
            //only has left child
            if(curr->getRight() ==nullptr){
                if(curr->getHeight() != curr->getLeft()->getHeight() +1){
                    totalCheck = false;
                    return false;
                }
            //only has right child
            }else if(curr->getLeft() == nullptr){
                if(curr->getHeight() != curr->getRight()->getHeight() +1){
                    totalCheck = false;
                    return false;
                }
            //has both children
            }else{
                if(curr->getHeight() != curr->getLeft()->getHeight() +1){
                    leftCheck = false;
                }
                if(curr->getHeight() != curr->getRight()->getHeight() +1){
                    rightCheck = false;
                }
                //if the node has two children, only one of the heights have to be one less than the parent
                if(!leftCheck && !rightCheck){
                    totalCheck = false;
                    return false;
                }
            }
        }
        leftRecursion = heightHelper(curr->getLeft());
        rightRecursion = heightHelper(curr->getRight());
        if(!leftRecursion || !rightRecursion){
            return false;
        }
    }
    return true;
}

bool Tester::removeBstTest(){
    Swarm bstTree(BST);
    Random idGen(MINID,MAXID);
    int randomID;
    int nodesVisited = 0;
    bool leafNodeValidRemoval;
    bool nodeStillHere;
    bool validHeight;
    bool twoChildrenValidRemoval;
    bool rootValidRemoval;
    //inserts 10 random nodes
    for(int i = 0; i < 10; i++){
        randomID = idGen.getRandNum();
        Robot robot(randomID,DEFAULT_TYPE);
        bstTree.insert(robot);
    }
    nodesVisited = nodeCounter(nodesVisited, bstTree.m_root);
    //checks to see if the tree has the correct starting amount
    if(nodesVisited == 10){
        //removes a leaf node
        bstTree.remove(62498);
        //resets nodesVisited and checks to see if a node has been removed
        nodesVisited = 0;
        nodesVisited = nodeCounter(nodesVisited, bstTree.m_root);
       //checks to see if the right node has been removed
        nodeStillHere = nodeFinder(62498, bstTree.m_root);
        //checks if the height of each node is valid after removal
        validHeight = heightHelper(bstTree.m_root);
        if(nodesVisited == 9 && !nodeStillHere && validHeight){
            leafNodeValidRemoval = true;
        }
        //if the first remval is valid, go onto the next removal
        if(leafNodeValidRemoval){
            //test the case of removing a node with two children
            bstTree.remove(54513);
            nodesVisited = 0;
            //same tests as the last removal
            nodesVisited = nodeCounter(nodesVisited, bstTree.m_root);  
            nodeStillHere = nodeFinder(54513, bstTree.m_root);
            validHeight = heightHelper(bstTree.m_root);
            if(nodesVisited == 8 && !nodeStillHere && validHeight){
                twoChildrenValidRemoval = true;
            }
            if(twoChildrenValidRemoval){
                //tests what happens if the root is removed
                bstTree.remove(79418);
                nodesVisited = 0;
            nodesVisited = nodeCounter(nodesVisited, bstTree.m_root);  
            nodeStillHere = nodeFinder(79418, bstTree.m_root);
            validHeight = heightHelper(bstTree.m_root);
            //does the same checks as the other removals, but also checks if the root is different
            if(nodesVisited == 7 && !nodeStillHere && validHeight && bstTree.m_root->getID() != 79418){
                rootValidRemoval = true;
                return true;
            }
            }
        }    
    }
    return false;
}

bool Tester::nodeFinder(int id, Robot * curr){
    bool foundLeft = false;
    bool foundRight = false;
    if(curr != nullptr){
        if(curr->getID() == id){
            return true;
        }else{
            foundLeft = nodeFinder(id, curr->getLeft());
            foundRight = nodeFinder(id, curr->getRight());
        }
    }
    if(foundLeft || foundRight){
        return true;
    }
    return false;
}

bool Tester::edgeCaseRemovalTest(){
    Swarm bstTree(BST);
    Robot robot(23000,DEFAULT_TYPE);
    bstTree.insert(robot);
    int nodesVisited = nodeCounter(nodesVisited, bstTree.m_root);
    if(nodesVisited == 1){
        //invalid removal, nothing should happen
        bstTree.remove(10000);
        nodesVisited = 0;
        nodesVisited = nodeCounter(nodesVisited, bstTree.m_root);
        //if node changes have been made, return true
        if(nodesVisited = 1){
            //removing the last node
            bstTree.remove(23000);
            //reset nodesVisited
            nodesVisited = 0;
            nodesVisited = nodeCounter(nodesVisited, bstTree.m_root);
            //if there are no more nodes and the root is set to nullptr, valid removal
            if(nodesVisited == 0 && bstTree.m_root == nullptr){
                return true;
            }
        }
    }

    return false;
}

bool Tester::avlBalanceAfterRemovalTest(){
    Random idGen(MINID,MAXID);
    Swarm avlTree(AVL);
    int randomID;
    int nodesVisited;
    int addedID[300];
    //inserts 300 nodes into an avl tree, balances after each insertion
    for(int i = 0; i < 300; i++){
        randomID = idGen.getRandNum();
        addedID[i] = randomID;
        Robot robot(randomID,DEFAULT_TYPE);
        avlTree.insert(robot);
    }
    //checks if 300 nodes have been inserted and that the tree is balanced
    nodesVisited = nodeCounter(nodesVisited, avlTree.m_root);
    bool balancedTree = balanceChecker(avlTree.m_root);
    //removes 150 nodes if the avl tree is valid with 300 nodes
    if(nodesVisited == 300 && balancedTree){
        for(int j = 0; j < 150; j++){
            avlTree.remove(addedID[j]);
        }
        nodesVisited = 0;
        //checks for balance after removal
        nodesVisited = nodeCounter(nodesVisited, avlTree.m_root);
        balancedTree = balanceChecker(avlTree.m_root);
        if(balancedTree && nodesVisited == 150){
            return true;
        }
    }
    return false;
}

bool Tester::propertyTest(){
    Random idGen(MINID,MAXID);
    int randomID;
    Swarm bstTree(BST);
    Swarm avlTree(AVL);
    int addedID[20];
    bool balancedTree;
    bool bstProperties;
    bool avlHeight;
    bool bstHeight;
    //add 20 nodes into a BST and AVL tree
    for(int i = 0; i < 20; i++){
        randomID = idGen.getRandNum();
        addedID[i] = randomID;
        Robot robot(randomID,DEFAULT_TYPE);
        avlTree.insert(robot);
        bstTree.insert(robot);
    }
   //check the balance and height of the AVL tree while checking the properties and height of the BST tree 
    balancedTree = balanceChecker(avlTree.m_root);
    bstProperties = bstPropertyHelper(bstTree.m_root);
    avlHeight = heightHelper(avlTree.m_root);
    bstHeight = heightHelper(bstTree.m_root);
    //remove 10 nodes from both trees
    if(balancedTree && bstProperties && avlHeight && bstHeight){
        for(int j = 0; j < 10; j++){
            avlTree.remove(addedID[j]);
            bstTree.remove(addedID[j]);
        }
    }
    //check the same properties of both trees after removal
    balancedTree = balanceChecker(avlTree.m_root);
    bstProperties = bstPropertyHelper(bstTree.m_root);
    avlHeight = heightHelper(avlTree.m_root);
    bstHeight = heightHelper(bstTree.m_root);
    if(balancedTree && bstProperties && avlHeight && bstHeight){
        return true;
    }
    return false;
}

bool Tester::assignmentOperatorNormalTest(){
    Random idGen(MINID,MAXID);
    Swarm bstTree(BST);
    Swarm bstCopyTree(BST);
    int randomID;
    int originalCount = 0;
    int copyCount = 0;
    int copiedTree;
    //creates a BST tree
    for(int i = 0; i < 10; i++){
        randomID = idGen.getRandNum();
        Robot robot(randomID,DEFAULT_TYPE);
        bstTree.insert(robot);
    }
    originalCount = nodeCounter(originalCount, bstTree.m_root);
    //copies the bst tree
    bstCopyTree = bstTree;
    copyCount = nodeCounter(copyCount, bstCopyTree.m_root);
    //checks if the trees have the same amount of nodes
    if(copyCount == originalCount){
        //checks to see if the copied tree is a deep copy
        copiedTree = assignmentOperatorHelper(bstCopyTree.m_root, bstTree.m_root);
    }
    //if the tree is a deep copy, return true
    if(copiedTree){
        return true;
    }
    return false;
}

bool Tester::assignmentOperatorHelper(Robot * copyCurr, Robot * originalCurr){
    bool leftCopy;
    bool rightCopy;
    if(originalCurr != nullptr){
        //the ids  should be the same
        if(originalCurr->getID() != copyCurr->getID()){
            return false;
        }
        if(originalCurr == copyCurr){
            return false;
        }
        leftCopy = assignmentOperatorHelper(copyCurr->getLeft(), originalCurr->getLeft());
        rightCopy = assignmentOperatorHelper(copyCurr->getRight(), originalCurr->getRight());
        if(!leftCopy || !rightCopy){
            return false;
        }
    }
    return true;
}

bool Tester::assignmentOperatorErrorTest(){
    Swarm emptyTree(BST);
    Swarm copiedTree(NONE);
    copiedTree = emptyTree;
    //the copied tree should still be empty
    if(copiedTree.m_root == nullptr){
        //the copiedTree's type should change from NONE to BST
        if(copiedTree.getType() == BST){
            return true;
        }
    }
    return false;
}