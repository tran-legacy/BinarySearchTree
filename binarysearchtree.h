// Yusuf Pisan pisan@uw.edu
// 15 Jan 2018

/**********************************
* Tran Le
* CSS343: Assignment #2 BinarySearchTree
* Last edited: 1/28/2018
***********************************/

// BinarySearchTree class
// Creates a BST to store values
// Uses BinaryNode which holds the data
// Uses templates to store any type of data
// binarysearchtreee.cpp file is included at the bottom of the .h file
/***EDIT (by Tran Le): binarysearchtree class implementation is at the bottom of the class declaration***/
/*** This is a more accurate way to implement a template class ***/
/*** my binarysearchtree.cpp will be turned in empty ***/
// binarysearchtreee.cpp is part of the template, cannot be compiled separately

#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include "binarynode.h"

using namespace std;

template<class ItemType>
class BinarySearchTree {
public:
	// constructor, empty tree
	BinarySearchTree();
	// constructor, tree with root
	explicit BinarySearchTree(const ItemType& rootItem);
	// copy constructor
	BinarySearchTree(const BinarySearchTree<ItemType>& bst);
	// destructor
	virtual ~BinarySearchTree();
	// true if no nodes
	bool isEmpty() const;
	// 0 if empty, 1 if only root, otherwise
	// max height of subtrees + 1
	int getHeight() const;
	// number of nodes and data in tree
	int getNumberOfNodes() const;
	// add a new item
	// return true if succesasfull
	// false if item already in tree or some other reason to fail
	bool add(const ItemType& item);
	// true if item is in tree
	bool contains(const ItemType& item) const;
	// contain helper function
	
	// dispaly a sideways ascii representation of tree
	void displaySideways() const;
	// inorder traversal: left-root-right
	// takes a function that takes a single parameter of ItemType
	// the function can modify the data in tree, but the
	// tree structure is not changed
	void inorderTraverse(void visit(ItemType&)) const;
	// create dynamic array, copy all the items to the array
	// and then read the array to re-create this tree
	void rebalance();
	// delete all nodes in tree and set root to nullptr
	void clear();
	// given an array of length n
	// create this tree to have all items in that array
	// with the minimum height
	// ASSUMPTION (Tran Le): the array is sorted
	bool readTree(ItemType arr[], int n);
	// trees are equal if they have the same structure
	// AND the same item values at all the nodes
	bool operator==(const BinarySearchTree<ItemType> &) const;
	// not == to each other
	bool operator!=(const BinarySearchTree<ItemType> &) const;
private:
	// root of the tree
	BinaryNode<ItemType>* rootPtr{ nullptr };
	int numOfNodes = 0; 
	ItemType* arr; 
	int currentIndex = 0; 

	// add a new node, helper function for add
	BinaryNode<ItemType>* placeNode(BinaryNode<ItemType>* subTreePtr, BinaryNode<ItemType>* newNodePtr);
	// find the node with given vale, helper function for contains
	bool findNode(BinaryNode<ItemType>* subTreePtr, const ItemType& target) const;
	// helper function for displaySideways to work recursively
	void sideways(BinaryNode<ItemType>* current, int level) const;
	// helper function for getHeight, returns the height
	int getHeightHelper(BinaryNode<ItemType>* rootPtr) const; 
	// helper function for operator==(), takes in 2 trees rootPtr
	bool operatorEEHelper(const BinaryNode<ItemType>* otherRootPtr, const BinaryNode<ItemType>* thisRootPtr) const; 
	// Copy constructor recursive helper, takes in the root of the tree we're trying to copy from
	BinaryNode<ItemType>* copyConstructorHelper(const BinaryNode<ItemType>* otherPtr); 
	// Destructor helper recursive helper
	void destructorHelper(BinaryNode<ItemType>* thisPtr);
	// inorderTraversal recursive helper
	void inorderHelper(void visit(ItemType&), BinaryNode<ItemType>* subTreePtr) const;
	// readTree recursive helper 
	void readTreeHelper(ItemType arr[], int startIndex, int endIndex);
	// Helper functions for rebalance
	void insertIntoArray(ItemType& item);  // will use if i figure out how to do the fancy way
	void rebalanceHelper(BinaryNode<ItemType>* subTreePtr);
};

/********************CONSTRUCTORS************************/
template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree() {}

template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree(const ItemType& rootItem) {
	// BinaryNode class automatically make left and right child null
	rootPtr = new BinaryNode<ItemType>(rootItem);
	++numOfNodes;
}
/////////////////////COPY CONSTRUCTOR AND ITS HELPER////////////////////////
template<class ItemType>
BinarySearchTree<ItemType>::BinarySearchTree(const BinarySearchTree<ItemType>& otherTree) {
	this->numOfNodes = otherTree.numOfNodes; 
	rootPtr = copyConstructorHelper(otherTree.rootPtr);
	/*this->numOfNodes = bst.numOfNodes;
	this->rootPtr = new BinaryNode<ItemType>();
	copyConstructorHelper(this->rootPtr, bst.rootPtr);*/ 
}
template<class ItemType> 
BinaryNode<ItemType>* BinarySearchTree<ItemType>::copyConstructorHelper(const BinaryNode<ItemType>* otherPtr) {
	// Use preorder traversal  to copy elements 
	BinaryNode<ItemType>* newTreePtr = NULL; 
	if (otherPtr) {
		newTreePtr = new BinaryNode<ItemType>(otherPtr->getItem()); 
		newTreePtr->setLeftChildPtr(copyConstructorHelper(otherPtr->getLeftChildPtr())); 
		newTreePtr->setRightChildPtr(copyConstructorHelper(otherPtr->getRightChildPtr()));
	}
	return newTreePtr; 
	//if (otherPtr != NULL) {
	//	thisPtr = new BinaryNode<ItemType>(otherPtr->getItem());
	//	cout << "OK ADDING In " << otherPtr->getItem() << endl;
	//	copyConstructorHelper(thisPtr->getLeftChildPtr(), otherPtr->getLeftChildPtr()); 
	//	copyConstructorHelper(thisPtr->getRightChildPtr(), otherPtr->getRightChildPtr()); 
	//}
}
///////////////////////////////////////////////////////////////////////////
/******************DESTRUCTOR AND ITS HELPER************************/
template<class ItemType>
BinarySearchTree<ItemType>::~BinarySearchTree() {
	clear();
}
template<class ItemType>
void BinarySearchTree<ItemType>::destructorHelper(BinaryNode<ItemType>* thisPtr) {
	// Implement it in postorder 
	if (thisPtr) {
		destructorHelper(thisPtr->getLeftChildPtr());
		destructorHelper(thisPtr->getRightChildPtr());
		delete thisPtr;
	}
}
/*******************GETTER FUNCTIONS*****************/
template<class ItemType>
bool BinarySearchTree<ItemType>::isEmpty() const {
	return !rootPtr;
}

//////////////////////////GETHEIGHT AND ITS HELPER FUNCTION ////////////////////////
template<class ItemType>
int BinarySearchTree<ItemType>::getHeight() const {
	return getHeightHelper(rootPtr); 
}
template<class ItemType> 
int BinarySearchTree<ItemType>::getHeightHelper(BinaryNode<ItemType>* currentNode) const {
	if (currentNode == NULL) 
		return 0; 
	int leftHeight = getHeightHelper(currentNode->getLeftChildPtr());
	int rightHeight = getHeightHelper(currentNode->getRightChildPtr());
	return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);  // Pick the bigger height and add 1 for the root 
}
///////////////////////////////////////////////////////////////////////////////////////

template<class ItemType>
int BinarySearchTree<ItemType>::getNumberOfNodes() const {
	return numOfNodes;;
}

////////////////////////ADD AND ITS HELPER FUNCTION/////////////////////////////////////
template<class ItemType>
bool BinarySearchTree<ItemType>::add(const ItemType& item) {
	if (contains(item)) return false;  // if the item is in the tree, return false
	BinaryNode<ItemType>* newNodePtr = new BinaryNode<ItemType>(item);  // make new node
	if (rootPtr == NULL) {
		rootPtr = newNodePtr;
		++numOfNodes; 
		return true;
	} 
	return placeNode(rootPtr, newNodePtr); // place node in tree
}
template<class ItemType>
BinaryNode<ItemType>* BinarySearchTree<ItemType>::placeNode(BinaryNode<ItemType>* subTreePtr, BinaryNode<ItemType>* newNodePtr) {
	// Base Case
	if (!subTreePtr)  // If root is null, set the newNode in
		return subTreePtr;

	// Recursive steps
	if (subTreePtr->getItem() < newNodePtr->getItem()) {  // If root is smaller than item
		if (placeNode(subTreePtr->getRightChildPtr(), newNodePtr) == NULL) {  // If the right child
			subTreePtr->setRightChildPtr(newNodePtr);
			++numOfNodes; 
		}
	} else if (subTreePtr->getItem() > newNodePtr->getItem()) { // If the root is bigger than item
		if (placeNode(subTreePtr->getLeftChildPtr(), newNodePtr) == NULL) {
			subTreePtr->setLeftChildPtr(newNodePtr);
			++numOfNodes; 
		}
	}
	return subTreePtr;
}
/////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////CONTAINS AND ITS HELPER FUNCTION////////////////////////////////////
template<class ItemType>
bool BinarySearchTree<ItemType>::contains(const ItemType& item) const {
	if (rootPtr == NULL) return false;
	return findNode(rootPtr, item); 
}
template<class ItemType>
bool BinarySearchTree<ItemType>::findNode(BinaryNode<ItemType>* subTreePtr, const ItemType & target) const {
	if (subTreePtr == NULL) {
		return false;
	} else if (subTreePtr->getItem() == target) {  // If it's the item, return true
		return true;
	} else if (subTreePtr->getItem() < target) {  // If this item is smaller then item, search the right branch
		return findNode(subTreePtr->getRightChildPtr(), target);
	} else if (subTreePtr->getItem() > target) {  // If this item is bigger than item, search the left branch
		return findNode(subTreePtr->getLeftChildPtr(), target);
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////INORDER TRAVERSE AND ITS HELPER///////////////////////////////////////
template<class ItemType> 
void BinarySearchTree<ItemType>::inorderTraverse(void visit(ItemType&)) const {
	inorderHelper(visit, this->rootPtr); 
}
template<class ItemType> 
void BinarySearchTree<ItemType>::inorderHelper(void visit(ItemType&), BinaryNode<ItemType>* subTreePtr) const {
	if (subTreePtr) {
		inorderHelper(visit, subTreePtr->getLeftChildPtr());  // go left
		ItemType item = subTreePtr->getItem();   // get this current
		visit(item);
		inorderHelper(visit, subTreePtr->getRightChildPtr());  // go right
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////REBALANCE AND ITS HELPER///////////////////////////////////////////////
template<class ItemType>
void BinarySearchTree<ItemType>::rebalance() {
	// Dynamically declare array 
	int size = this->numOfNodes;  // needs to keep size variable because clear will set numOfNodes to 0
	this->arr = new ItemType[size];

	// Will insert values of this tree into arr
	rebalanceHelper(this->rootPtr); 
	//this->inorderTraverse(insertIntoArray);
	///***************FOR TESTING PURPOSES****************/
	//cout << "THE ARRAY IS LIKE: "; 
	//for (int i = 0; i < this->numOfNodes; ++i) {  // so the array looks correct
	//	cout << arr[i] << " "; 
	//}
	///**************************************************/
	// Clear this tree before reading in from array to rebalance
	this->clear();  
	this->readTree(this->arr, size);
	// Delete the array and reset index 
	delete[] this->arr; 
	this->arr = NULL;
	this->currentIndex = 0; 
}
template<class ItemType>
void BinarySearchTree<ItemType>::insertIntoArray(ItemType& item) {
	this->arr[this->currentIndex] = item; 
	++currentIndex;
}
template<class ItemType>
void BinarySearchTree<ItemType>::rebalanceHelper(BinaryNode<ItemType>* subTreePtr) {
	if (subTreePtr) {
		rebalanceHelper(subTreePtr->getLeftChildPtr());  // go left
		this->arr[this->currentIndex] = subTreePtr->getItem();
		++currentIndex; 
		rebalanceHelper(subTreePtr->getRightChildPtr());  // go right
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////
template<class ItemType>
void BinarySearchTree<ItemType>::clear() {
	destructorHelper(this->rootPtr);  // Recursive helper method
	this->rootPtr = NULL;
	this->numOfNodes = 0;
}

/////////////////////////////////READTREE AND ITS HELPER//////////////////////////////////////
template<class ItemType>
bool BinarySearchTree<ItemType>::readTree(ItemType arr[], int n) {
	if (arr) {
		readTreeHelper(arr, 0, n-1);
		return true; 
	}
	return false; 
}
template<class ItemType> 
void BinarySearchTree<ItemType>::readTreeHelper(ItemType arr[], int startIndex, int endIndex) {
	int midPoint = (endIndex + startIndex) / 2;
	if (endIndex < startIndex) {
		return;
	} else {
		this->add(arr[midPoint]); // Add midpoint into tree
		readTreeHelper(arr, startIndex, midPoint - 1);  // Insert left branch 
		readTreeHelper(arr, midPoint + 1, endIndex);  // Insert right branch
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////Operator== AND ITS HELPER///////////////////////////////////////////
template<class ItemType>
bool BinarySearchTree<ItemType>::operator==(const BinarySearchTree<ItemType>& tree) const {
	// If not same number of nodes or same height, return false
	if (tree.getNumberOfNodes() != this->getNumberOfNodes() || tree.getHeight() != this->getHeight())
		return false;
	else
		return operatorEEHelper(tree.rootPtr, this->rootPtr);
}

template<class ItemType>
bool BinarySearchTree<ItemType>::operatorEEHelper(const BinaryNode<ItemType>* otherRootPtr, const BinaryNode<ItemType>* thisRootPtr) const {
	bool isEqual = true; 
	
	if (otherRootPtr == NULL && thisRootPtr == NULL) {
		return true;
	}
	// If one is NULL and the other is not NULL, or the items aren't equal to each other 
	if ((otherRootPtr == NULL && thisRootPtr != NULL) || (otherRootPtr != NULL && thisRootPtr == NULL) ||  
		(otherRootPtr->getItem() != thisRootPtr->getItem())) {
		return false; 
	} else {
		isEqual = operatorEEHelper(otherRootPtr->getLeftChildPtr(), thisRootPtr->getLeftChildPtr()) &&
			operatorEEHelper(otherRootPtr->getRightChildPtr(), thisRootPtr->getRightChildPtr()); 
	}
	return isEqual; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////
template<class ItemType>
bool BinarySearchTree<ItemType>::operator!=(const BinarySearchTree<ItemType>& tree) const {
	// If number of nodes or heights aren't equal to each other, return true
	if ((tree.getNumberOfNodes() != this->getNumberOfNodes()) || tree.getHeight() != this->getHeight())
		return true;
	else 
		return !operatorEEHelper(tree.rootPtr, this->rootPtr);
}

/******************TEACHER PROVIDED METHODS*************/
// based on code from Zander
// display ascii version of tree sideways
template<class ItemType>
void BinarySearchTree<ItemType>::displaySideways() const {
	sideways(rootPtr, 0);
}

// display ascii version of tree sideways recursively
template<class ItemType>
void BinarySearchTree<ItemType>::sideways(BinaryNode<ItemType>* current, int level) const {
	if (current != NULL) {
		level++;
		sideways(current->getRightChildPtr(), level);

		// indent for readability, 4 spaces per depth level
		for (int i = level; i >= 0; i--) {
			cout << "    ";
		}

		cout << current->getItem() << endl;        // display information of object
		sideways(current->getLeftChildPtr(), level);
	}
}


#endif // BINARYSEARCHTREE_H