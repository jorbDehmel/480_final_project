#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

int comparisons = 0;
unsigned long long total_bytes_used = 0;


struct Node {
	int data; 
	Node *parent; 
	Node *left; 
	Node *right; 
	int color; // 1 -> Red, 0 -> Black
};

typedef Node *NodePtr;

// class For Red Black Tree
class RBTree {
private:
	NodePtr root;
	NodePtr TNULL;

	// initializes the nodes with appropirate values
	void initializeNULLNode(NodePtr node, NodePtr parent) {
		node->data = 0;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	void preOrder(NodePtr node) {
		if (node != TNULL) {
			cout<<node->data<<" ";
			preOrder(node->left);
			preOrder(node->right);
		} 
	}

	void inOrder(NodePtr node) {
		if (node != TNULL) {
			inOrder(node->left);
			cout<<node->data<<" ";
			inOrder(node->right);
		} 
	}

	void postOrder(NodePtr node) {
		if (node != TNULL) {
			postOrder(node->left);
			postOrder(node->right);
			cout<<node->data<<" ";
		} 
	}

	NodePtr searchTree(NodePtr node, int key) {
		if (node == TNULL || key == node->data) {
			return node;
		}
		comparisons++;
		if (key < node->data) {
			NodePtr leftResult = searchTree(node->left, key);
			return leftResult == TNULL ? nullptr : leftResult;
		} 
		NodePtr rightResult = searchTree(node->right, key);
		return rightResult == TNULL ? nullptr : rightResult;
	}


	// fix the rb tree modified by the delete operation
	void fixDelete(NodePtr x) {
		NodePtr s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				if (s->right->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					} 

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
			} 
		}
		x->color = 0;
	}

	void rbTransplant(NodePtr u, NodePtr v){
		if (u->parent == nullptr) {
			root = v;
		} else if (u == u->parent->left){
			u->parent->left = v;
		} else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	void deleteNode(NodePtr node, int key) {
		// find the node containing key
		NodePtr z = TNULL;
		NodePtr x, y;
		while (node != TNULL){
			if (node->data == key) {
				z = node;
			}

			if (node->data <= key) {
				node = node->right;
			} else {
				node = node->left;
			}
			comparisons++;
		}

		if (z == TNULL) {
			cout<<"Couldn't find key in the tree"<<endl;
			return;
		} 

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		} else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		} else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			} else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z;
		if (y_original_color == 0){
			fixDelete(x);
		}
	}
	
	// fix the red-black tree
	void fixInsert(NodePtr k){
		NodePtr u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;	
				} else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void print(NodePtr root, string indent, bool last) {
	   	if (root != TNULL) {
		   cout<<indent;
		   if (last) {
		      cout<<"R----";
		      indent += "     ";
		   } else {
		      cout<<"L----";
		      indent += "|    ";
		   }
            
           string sColor = root->color?"RED":"BLACK";
		   cout<<root->data<<"("<<sColor<<")"<<endl;
		   print(root->left, indent, false);
		   print(root->right, indent, true);
		}
	}

	size_t calculateMemoryUsage(NodePtr node) {
		if (node == TNULL) {
			return 0;
		}
		// Size of current node + sizes of left and right subtrees
		return sizeof(Node) + calculateMemoryUsage(node->left) + calculateMemoryUsage(node->right);
	}

public:
	RBTree() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	// Pre-Order traversal
	// Node->Left->Right
	void preorder() {
		preOrder(this->root);
	}

	// In-Order traversal
	// Left->Node->Right
	void inorder() {
		inOrder(this->root);
	}

	// Post-Order traversal
	// Left->Right->Node
	void postorder() {
		postOrder(this->root);
	}

	//Search for value k
	NodePtr searchTree(int k) {
		return searchTree(this->root, k);
	}

	// return node with min value
	NodePtr minimum(NodePtr node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// return node with max value
	NodePtr maximum(NodePtr node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// rotate left at node x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position then fix the tree
	void insert(int key) {
		// Ordinary Binary Search Insertion
		NodePtr node = new Node;
		node->parent = nullptr;
		node->data = key;
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red


		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != TNULL) {
			y = x;
			if (node->data < x->data) {
				x = x->left;
			} else {
				x = x->right;
			}
			comparisons++;
		}
		total_bytes_used += sizeof(y);

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (node->data < y->data) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr){
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	NodePtr getRoot(){
		return this->root;
	}

	// delete the node from the tree
	void deleteNode(int data) {
		deleteNode(this->root, data);
	}

	// print the tree structure
	void prettyPrint() {
	    if (root) {
    		print(this->root, "", true);
	    }
	}

	size_t calculateMemoryUsage(){
		return calculateMemoryUsage(this->root);
	}

};


int main() {
	const int ARRAY_SIZE = 10000000;
	RBTree bst;
	int array[ARRAY_SIZE];
	srand(time(NULL));
	for (int i=0; i<ARRAY_SIZE; i++){
		array[i] = rand() % 10000;
	}


	//Insert
	auto begin = chrono::high_resolution_clock::now();
	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		bst.insert(array[i]);
	}
	auto end = chrono::high_resolution_clock::now();
	auto elapsed = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();
	cout << "---------INSERTION-----------------------------------------------" << endl;
	cout << "Time to insert " << ARRAY_SIZE << " elements: 				" << elapsed << " nanoseconds" << endl; 
	cout << "Total Comparisons to insert " << ARRAY_SIZE << " elements: 		" << comparisons << endl;
	cout << "Average Comparisons per element: 			" << float(comparisons)/ARRAY_SIZE << endl;
	cout << "The Theoretical Insetion O(log(n)) is: 			" << log2(float(ARRAY_SIZE)) << endl;
	cout << "Total bytes used for Tree: 				" << bst.calculateMemoryUsage() << endl;
	cout << "Size of each node in Tree: 				" << sizeof(Node) << endl;
	cout << "Theoretical Space complexity O(n) for " << ARRAY_SIZE << " elements:	" << ARRAY_SIZE*sizeof(Node) << endl;
	cout << "-------------------------------------------------------------------" << endl;

	//Search
	int array_jump = ARRAY_SIZE * 0.2;
    int search_array[5] = {array[0],array[array_jump],array[2*array_jump],array[3*array_jump],array[4*array_jump]};
	comparisons = 0;
	NodePtr result;
	begin = chrono::high_resolution_clock::now();
	for(int i = 0; i < 5; i++) {
		result = bst.searchTree(search_array[i]);
		if (result == nullptr) {
			cout << search_array[i] << " Not found in tree" << endl;
		} else {
			//cout << search_array[i] << " Found in tree" << endl;
		}
	}
	end = chrono::high_resolution_clock::now();
	elapsed = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();
	cout << "-----------Searching-------------------------------------------" << endl;
	cout << "Total Time to complete 5 searches:			" << elapsed << " nanoseconds" << endl;
	cout << "Total Comparisons for 5 searches: 			" << comparisons << endl;
	cout << "Average Comparisons per search:				" << float(comparisons)/5 << endl;
	cout << "Theoretical complexity for search(O(logn)):		" << log2(ARRAY_SIZE) << endl;
	cout << "-------------------------------------------------------------------" << endl;


	//Delete
	comparisons = 0;
	begin = chrono::high_resolution_clock::now();
	for(int i = 0; i < 5; i++) {
		result = bst.searchTree(search_array[i]);
		if (result == nullptr) {
			cout << search_array[i] << " Not found in tree" << endl;
		} else {
			//cout << search_array[i] << " Found in tree" << endl;
		}
	}
	end = chrono::high_resolution_clock::now();
	elapsed = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();
	elapsed = chrono::duration_cast<chrono::nanoseconds>(end-begin).count();
	cout << "-----------Deleting-------------------------------------------" << endl;
	cout << "Total Time to complete 5 deletions:			" << elapsed << " nanoseconds" << endl;
	cout << "Total Comparisons for 5 deletions: 			" << comparisons << endl;
	cout << "Average Comparisons per deletions:			" << float(comparisons)/5 << endl;
	cout << "Theoretical complexity for deletions(O(logn)):		" << log2(ARRAY_SIZE) << endl;
	cout << "-------------------------------------------------------------------" << endl;


	//bst.prettyPrint();


	return 0;
}


/* This code is derivative of the code found at this git repo
https://github.com/Bibeknam/algorithmtutorprograms/blob/master/data-structures/red-black-trees/RedBlackTree.cpp
some parts have been implemented independent of this code base but the deletion section comes from this code*/