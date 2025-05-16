// UMBC - CMSC 341 - Fall 2024 - Proj2
#include "swarm.h"
Swarm::Swarm(){
    m_root = nullptr;
    m_type = NONE;
}

Swarm::Swarm(TREETYPE type){
    m_type = type;
    m_root = nullptr;
}

Swarm::~Swarm(){
    clear();
}

void Swarm::clear(){
    m_type = NONE;
    removeALL(m_root);
    m_root = nullptr;      
}

void Swarm::removeALL(Robot *currentNode){
    //recursively traverses through the tree and deletes all the nodes
    if(currentNode != nullptr){
        removeALL(currentNode->m_left);
        removeALL(currentNode->m_right);
        delete currentNode;
        currentNode = nullptr;
    }
}

void Swarm::insert(const Robot& robot){
    Robot * newRobot;
    if(m_root != nullptr){
        //creates a new node object and uses the helper function to place it on the tree
        newRobot = new Robot(robot);
        insertHelper(m_root, robot.getID(), newRobot);

    }else{
        //set m_root to the new node
        newRobot = new Robot(robot);
        m_root = newRobot;
    }

    heightSetter();
    if(m_type == AVL){
        //checks the height and rotates if needed
        heightChecker();
    }
    
    if(m_type == SPLAY){
        //if the tree is a splay tree, the inserted node is rotated to the top using the splayHelper function
        if(m_root != newRobot){
            Robot * parentRobot = parentFinder(newRobot->m_id, m_root);
            splayHelper(newRobot, parentRobot);
        }
    }
}

void Swarm::insertHelper(Robot *curr, int id, Robot *robot){
    Robot *emptyRobot = nullptr;
    //if the id is greater, move to the right
    if(id > curr->getID()){
        //if the current node has a right child, call the function at the current's right child
        if(curr->getRight() != nullptr){
            insertHelper(curr->getRight(), id, robot);
        //if the current node doesnt have a right child, set the right to the inserted node
        
        }else{
            //set the node to the right
            curr->setRight(robot);
        }   
    //if the id is less, move to the left
    
    }else if(id < curr->getID()){
        //if the current node has a left child, call the function at the current's left child
        if(curr->getLeft() != nullptr){
            insertHelper(curr->getLeft(), id, robot);
        }else{
            //set the node to the left
            curr->setLeft(robot);
        }
    }
}

void Swarm::remove(int id){
    //remove only works for avl and bst trees
    if(m_type == AVL || m_type == BST){
        //calls the nodeFinderHelper to make sure the id has a node
        bool hasNode = nodeFinderHelper(id, m_root);
        if(hasNode){
            Robot *removeNode = nullptr;
            Robot *parentNode = nullptr;
            //sets removeNode to the node to be removed
            removeNode = removeNodeHelper(m_root, id);
            //if the node isn't the root, use the parentFinder function to set the parentNode
            if(m_root != removeNode){
                parentNode = parentFinder(removeNode->m_id, m_root);
            }

            //case if removeNode is a leaf
            if(removeNode->m_left == nullptr && removeNode->m_right == nullptr){
                if(m_root != removeNode){
                    if(parentNode->m_right == removeNode){
                        parentNode->m_right = nullptr;
                    }else if(parentNode->m_left == removeNode){
                        parentNode->m_left = nullptr;
                    }
                }else{
                    m_root = nullptr;
                }
                delete removeNode;
            }

            //case if removeNode only has a right child
            else if(removeNode->m_left == nullptr && removeNode->m_right !=nullptr){
                if(removeNode != m_root){
                    //sets the parentNode node to now point at removeNode's child node
                    if(parentNode->m_right == removeNode){
                        parentNode->m_right = removeNode->m_right;
                    }else if(parentNode->m_left == removeNode){
                        parentNode->m_left = removeNode->m_right;
                    }
                //if removeNode is the root, set the new root to the right child
                }else{
                    m_root = removeNode->m_right;
                }
                delete removeNode;
            }

            //case if removeNode only has a left child
            else if(removeNode->m_left != nullptr && removeNode->m_right ==nullptr){
                if(removeNode != m_root){
                    //sets the parentNode node to now point at removeNode's child node
                    if(parentNode->m_right == removeNode){
                        parentNode->m_right = removeNode->m_left;
                    }else if(parentNode->m_left == removeNode){
                        parentNode->m_left = removeNode->m_left;
                    }
                }else{
                    m_root = removeNode->m_left;
                }
                delete removeNode;
            }

            //case if removeNode has both a left and right child
            else if(removeNode->m_right !=nullptr && removeNode->m_left != nullptr){
                if(m_root != removeNode){
                    //if the removeNode is the parentNode's right child
                    if(parentNode->m_right == removeNode){
                        parentNode->m_right = removeNode->m_left;
                        if(removeNode->m_left->m_right == nullptr){
                            removeNode->m_left->m_right = removeNode->m_right;
                        }else{
                            //keep traversing right until it the filler Node no longer has a right child
                            Robot *fillerNode = removeNode->m_left->m_right;
                            while (fillerNode->m_right != nullptr){
                                fillerNode = fillerNode->m_right;
                            }
                            fillerNode->m_right = removeNode->m_right;
                        }
                    //if the removeNode is the parentNode's left child
                    }else if(parentNode->m_left == removeNode){
                        parentNode->m_left = removeNode->m_right;
                        if(removeNode->m_right->m_left == nullptr){
                            removeNode->m_right->m_left = removeNode->m_left;
                        }else{
                            //keep traversing right until it the filler Node no longer has a left child
                            Robot *fillerNode = removeNode->m_right->m_left;
                            while(fillerNode->m_left !=nullptr){
                                fillerNode = fillerNode->m_left;
                            }
                            fillerNode->m_left = removeNode->m_left;
                        }
                    }
                //if the removeNode is the root and has two children
                
                }else{
                    m_root = removeNode->m_left;
                    if(removeNode->m_right->m_left == nullptr){
                        removeNode->m_right->m_left = removeNode->m_left->m_right;
                    }else{
                        Robot *fillerNode = removeNode->m_right->m_left;
                        while(fillerNode->m_left != nullptr){
                            fillerNode = fillerNode->m_left;
                        }
                        fillerNode->m_left = removeNode->m_left->m_right;
                    }
                    removeNode->m_left->m_right = removeNode->m_right;
                }
                delete removeNode;
            }
            //sets the height of all the nodes
            heightSetter();
            //rebalances the tree if the tree is an AVL
            if(m_type == AVL){
                heightChecker();
            }
        }
    }
}

Robot* Swarm::removeNodeHelper(Robot * curr, int id){
    if(curr!=nullptr){
        //keeps traversing until the id of the node to remove is found
        if(curr->m_id == id){
            return curr;
        
        }else{
            //if the current id is greater than the target id, move to the left child
            if(curr->m_id > id){
                return removeNodeHelper(curr->m_left, id);
            }else if(curr->m_id < id){
                return removeNodeHelper(curr->m_right, id);
            }
        }
    }
    return curr;
}

bool Swarm::nodeFinderHelper(int id, Robot * curr){
    bool foundLeft = false;
    bool foundRight = false;
    if(curr != nullptr){
        //return true if we found the id of the node
        if(curr->m_id == id){
            return true;
        }else{
            //check left and right for the id of the node to find
            foundLeft = nodeFinderHelper(id, curr->m_left);
            foundRight = nodeFinderHelper(id, curr->m_right);
        }
    }
    //if the node has been found further down the tree, keep returning true
    if(foundLeft || foundRight){
        return true;
    }
    return false;
}

const Swarm & Swarm::operator=(const Swarm & rhs){
    clear();
    m_type = rhs.m_type;
    //if the tree has a root, call the overloadedHelper to make a deep copy of the tree
    if(rhs.m_root != nullptr){
        overloadedHelper(rhs.m_root, rhs);
    }
    return *this;
}

void Swarm::overloadedHelper(Robot* curr,const Swarm & rhs){
    if(curr != nullptr){
        //create a robot object using the attributes from the original tree
        Robot robot(curr->m_id,curr->m_type);
        //inserts the node into the copy tree
        insert(robot);      
        overloadedHelper(curr->m_left, rhs);
        overloadedHelper(curr->m_right, rhs);
    }
}

TREETYPE Swarm::getType() const{
    return m_type;
}

void Swarm::setType(TREETYPE type){
    m_type = type;
    //if the type is an AVL, rebalance the tree
    if(type == AVL){
        heightChecker();
    //if the type is none, clear the tree of all nodes
    }else if(type == NONE){
        clear();
    }
}

void Swarm::dumpTree() const 
{
    dump(m_root);
}

void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}

void Swarm::heightSetter(){
    //Checks and sets the height of the nodes
    heightHelper(m_root);
}

void Swarm::heightHelper(Robot * robot){
    if(robot != nullptr){
        //if the node isn't nullptr, check the right then left
        heightHelper(robot->m_right);
        heightHelper(robot->m_left);
        
        //if the node has a left and right child
        if(robot->m_left != nullptr && robot->m_right != nullptr){
            //whatever height is higher between the left and right child, the parent has that height plus one
            if(robot->m_left->m_height >= robot->m_right->m_height){
                robot->m_height = robot->m_left->m_height+1;
            }else{
                robot->m_height = robot->m_right->m_height+1;
            }
        
        //if the node has no children
        }else if(robot->m_left == nullptr && robot->m_right == nullptr){
            robot->m_height = 0;
        //if the node has only one child
        }else if(robot->m_left == nullptr && robot->m_right != nullptr){
            robot->m_height = robot->m_right->m_height+1;
        }else if(robot->m_left != nullptr && robot->m_right == nullptr){
            robot->m_height = robot->m_left->m_height+1;
        }
    }
}

void Swarm::heightChecker(){
    bool balanced = balanceChecker(m_root);
    //while the tree isn't balanced, keep rotating where needed
    while(balanced == false){
        heightCheckerHelper(m_root);
        heightSetter();
        balanced = balanceChecker(m_root);
    }
}

void Swarm::heightCheckerHelper(Robot* robot){
    int difference;
    if(robot != nullptr){
        heightCheckerHelper(robot->m_left);
        heightCheckerHelper(robot->m_right);
        
        //if the node has both children
        if(robot->m_left != nullptr && robot->m_right != nullptr){
            difference = robot->m_left->m_height - robot->m_right->m_height;
            //unbalance on the left
            if(difference > 1){
                int childDifference;
                if(robot->m_left->m_right != nullptr && robot->m_left->m_left != nullptr){
                    childDifference = robot->m_left->m_left->m_height - robot->m_left->m_right->m_height;
                }else if(robot->m_left->m_right == nullptr){
                    childDifference = 1;
                }else if(robot->m_left->m_left == nullptr){
                    childDifference = -1;
                }
                //rotates left then right based on the height of the children of the left child
                if(childDifference < 0){
                    leftRotate(robot->m_left);
                    rightRotate(robot);
                //rotates right
                }else if(childDifference >= 0){
                    rightRotate(robot);
                }
                //resets the height of each node
                heightSetter();
            
            //unbalance on the right
            }else if(difference < -1){
                int childDifference;
                if(robot->m_right->m_right != nullptr && robot->m_right->m_left != nullptr){
                    childDifference = robot->m_right->m_left->m_height - robot->m_right->m_right->m_height;
                }else if(robot->m_right->m_right == nullptr){
                    childDifference = 1;
                }else if(robot->m_right->m_left == nullptr){
                    childDifference = -1;
                }
                //rotates right then left
                if(childDifference > 0){
                    rightRotate(robot->m_right);
                    leftRotate(robot);
                //rotates left
                }else if(childDifference <= 0){
                    leftRotate(robot);
                }
                
            }
        
        //checks for inbalance if there is only a right child
        }else if(robot->m_left == nullptr && robot->m_right != nullptr){
                if(robot->m_right->m_height > 2){
                    if(robot->m_right->m_left !=nullptr){
                        rightRotate(robot->m_right);
                        leftRotate(robot);
                    }else{
                        leftRotate(robot);
                    }
                }
        
        //checks for inbalance if there is only a left child
        }else if(robot->m_left != nullptr && robot->m_right == nullptr){
            if(robot->m_left->m_height > 2){
                if(robot->m_left->m_right !=nullptr){
                        leftRotate(robot->m_left);
                        rightRotate(robot);
                    }else{
                        rightRotate(robot);
                    }
            }
        }
    }
}

void Swarm:: leftRotate(Robot *robot){
    Robot * parent;
    //y is the right child of the robot node
    Robot * rightChild = robot->m_right;
    Robot * grandChild;
    
    //if the right child has a left child, set the left child to z
    if(rightChild->m_left != nullptr){
        grandChild = rightChild->m_left;
    }
    
    //if the robot node is the root, set the right child to the new root
    if(m_root == robot){
        m_root = rightChild;
    
    }else{
        //finds the parent of the node
        parent = parentFinder(robot->m_id, m_root);
        if(parent->m_left == robot){
            parent->m_left = rightChild;
        }else if(parent->m_right == robot){
            parent->m_right = rightChild;
        }
    }
    
    if(rightChild->m_left != nullptr){
        robot->m_right = grandChild;
        }else{
            robot->m_right = nullptr;
        }
    rightChild->m_left = robot;
    //resets the height
    heightSetter();
}

void Swarm:: rightRotate(Robot *robot){
    Robot * parent;
    Robot * leftChild = robot->m_left;
    Robot * grandChild;
    
    //if the left child has a right child, it is set to the grandChild
    if(leftChild->m_right != nullptr){
        grandChild = leftChild->m_right;
    }
    
    //if the robot node passed in isnt the root
    if(m_root != robot){
        parent = parentFinder(robot->m_id, m_root);
        if(parent->m_left == robot){
            parent->m_left = leftChild;
        }else if(parent->m_right == robot){
            parent->m_right = leftChild;
        }
    }
    
    //if the robot node was the root, the left child is now the new root
    if(m_root->m_id == robot->m_id){
        m_root = leftChild;
    }
    
    if(leftChild->m_right != nullptr){
    robot->m_left = grandChild;
    //if the node passed in is the root
    }else{
        robot->m_left = nullptr;
    }
    leftChild->m_right = robot;
    //resets the height
    heightSetter();
}

Robot* Swarm::parentFinder(int id, Robot * curr){
    //checks the current nodes right child
    if(curr->m_right != nullptr){
        if(curr->m_right->m_id == id){
            return curr;
        }
    }

    //checks the current nodes left child
    if(curr->m_left != nullptr){
        if(curr->m_left->m_id == id){
            return curr;
        }
    }

    //traverses to the left child
    if(curr->m_id > id){
        return parentFinder(id, curr->m_left);
    }

    //traveres to the right child
    return parentFinder(id, curr->m_right);
}

void Swarm::splayHelper(Robot *robot, Robot *parent){
    if(m_root != robot){
        //if the node is the parents right child, rotate right
        if(parent->m_id > robot->m_id){
            rightRotate(parent);
        }
        //if the node is the parents left child, rotate left
        if(parent->m_id < robot->m_id){
            leftRotate(parent);
        }
    }
    if(m_root !=robot){
        //set the new parent and call the function again
        parent = parentFinder(robot->m_id, m_root);
        splayHelper(robot,parent);
    }
}

bool Swarm::balanceChecker(Robot* robot){
    bool balance = true;
    bool leftBalance = true;
    bool rightBalance = true;
    bool totalBalance = true;
    
    if(robot != nullptr){
        //if the node has two children
        if(robot->m_left != nullptr && robot->m_right != nullptr){
            int difference = robot->m_left->m_height - robot->m_right->m_height;
            if(difference > 1 || difference < -1){
                balance = false;
            }
        //if the node only has a right child
        }else if(robot->m_left == nullptr && robot->m_right != nullptr){
            if(robot->m_right->m_height > 2){
                balance = false;
            }
        //if the node only has a left child
        }else if(robot->m_left != nullptr && robot->m_right == nullptr){
            if(robot->m_left->m_height > 2){
                balance = false;
            }
        }
        //checks the balance of the left and right child
        leftBalance = balanceChecker(robot->m_left);
        rightBalance = balanceChecker(robot->m_right);
    }
    
    if(balance == false || leftBalance == false || rightBalance == false){
        totalBalance = false;
    }
    return totalBalance;
}