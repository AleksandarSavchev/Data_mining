#include <iostream>
#include <math.h>
#include <vector>
#include <stack>
#include <utility>
#include <string>

int zeroPos = 0;
int size = 0;

class NodeT {
public:
	NodeT** next;
	NodeT() {
		next = new NodeT * [size];
		for (int i = 0; i < size; i++)
			next[i] = nullptr;
	}
	~NodeT() {
		for (int i = 0; i < size; i++)
			delete[] next[i];
		delete[] next;
	}
};

class Trie {
public:
	NodeT* root;
	Trie() {
		root = new NodeT;
	}

	bool checkIfExists(int** matrix) {
		int side = (int)sqrt(size);
		NodeT* current = root;
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				if (current->next[matrix[i][j]] != nullptr) {
					current = current->next[matrix[i][j]];
				}
				else {
					return false;
				}
			}
		}
		return true;
	}

	void add(int** matrix) {
		NodeT* current = this->root;
		int side = (int)sqrt(size);
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				if (current->next[matrix[i][j]] != nullptr) {
					current = current->next[matrix[i][j]];
				}
				else {
					NodeT* temp = new NodeT;
					current->next[matrix[i][j]] = temp;
					current = temp;
				}
			}
		}
	}
};

int manhatan(int** matrix) {
	int result = 0;
	int realPos = 0;
	int side = (int)sqrt(size);
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			if (matrix[i][j] == 0) {
				continue;
			}
			realPos = matrix[i][j] <= zeroPos ? matrix[i][j] - 1 : matrix[i][j];
			result += abs(i - realPos / side) + abs(j - realPos % side);
		}
	}


	return result;
}

class Node {
public:

	std::vector<Node> children;
	int manhatanVal;
	char way;
	bool visited;

	Node() {}

	Node(int** matrix, char way) {
		this->manhatanVal = manhatan(matrix);
		this->visited = false;
		this->way = way;
	}

	void CreateChildren(int** matrix, char way, Trie* myTree) {
		int side = (int)sqrt(size);
		int x0, y0;
		for (int i = 0; i < side; i++) {
			for (int j = 0; j < side; j++) {
				if (matrix[i][j] == 0) {
					x0 = i;
					y0 = j;
				}
			}
		}
		if (x0 != 0 && way != 'u') {
			SwapAndAdd(x0, y0, x0 - 1, y0, 'd', matrix, myTree);
		}
		if (x0 != side - 1 && way != 'd') {
			SwapAndAdd(x0, y0, x0 + 1, y0, 'u', matrix, myTree);
		}
		if (y0 != 0 && way != 'l') {
			SwapAndAdd(x0, y0, x0, y0 - 1, 'r', matrix, myTree);
		}
		if (y0 != side - 1 && way != 'r') {
			SwapAndAdd(x0, y0, x0, y0 + 1, 'l', matrix, myTree);
		}
	}

	void SwapAndAdd(int x0, int y0, int x1, int y1, char way, int** matrix, Trie* myTree) {
		matrix[x0][y0] = matrix[x1][y1];
		matrix[x1][y1] = 0;
		if (myTree->checkIfExists(matrix)) {
			matrix[x1][y1] = matrix[x0][y0];
			matrix[x0][y0] = 0;
			return;
		}
		myTree->add(matrix);
		Node child(matrix, way);
		children.push_back(child);
		matrix[x1][y1] = matrix[x0][y0];
		matrix[x0][y0] = 0;
	}
};

void ReverseStack(std::stack<Node*> myStack) {
	Node* temp = myStack.top();
	std::string way;
	if (temp->way == 'u')
		way = "Up";
	else if (temp->way == 'd')
		way = "Down";
	else if (temp->way == 'l')
		way = "Left";
	else if (temp->way == 'r')
		way = "Right";
	if (myStack.size() == 1) {
		return;
	}
	else {
		myStack.pop();
		ReverseStack(myStack);
		std::cout << way << std::endl;
	}
}

void Go(int** matrix, char way) {
	if (way == ' ')
		return;
	int side = (int)sqrt(size);
	int x0, y0;
	for (int i = 0; i < side; i++) {
		for (int j = 0; j < side; j++) {
			if (matrix[i][j] == 0) {
				x0 = i;
				y0 = j;
			}
		}
	}

	if (way == 'u') {
		matrix[x0][y0] = matrix[x0 + 1][y0];
		matrix[x0 + 1][y0] = 0;
		return;
	}
	else if (way == 'd') {
		matrix[x0][y0] = matrix[x0 - 1][y0];
		matrix[x0 - 1][y0] = 0;
		return;
	}
	else if (way == 'l') {
		matrix[x0][y0] = matrix[x0][y0 + 1];
		matrix[x0][y0 + 1] = 0;
		return;
	}
	else if (way == 'r') {
		matrix[x0][y0] = matrix[x0][y0 - 1];
		matrix[x0][y0 - 1] = 0;
		return;
	}
}
void Clear(Node* root) {
	root->visited = false;
	for (int i = 0; i < root->children.size(); i++) {
		Clear(&root->children[i]);
	}
}

void ISearch(Node root, int** matrix, Trie* myTree) {
	int threshold = root.manhatanVal;
	std::stack<Node*> myStack;

	while (true) {
		myStack.push(&root);
		root.visited = true;

		while (!myStack.empty()) {

			Node* temp = myStack.top();
			if (temp->children.empty()) {
				temp->CreateChildren(matrix, temp->way, myTree);
			}
			if (temp->children.empty() || temp->children[0].manhatanVal == -1) {
				if (temp->children.empty()) {
					Node* t = new Node;
					t->manhatanVal = -1;
					temp->children.push_back(*t);
				}
				Go(matrix, temp->way == 'r' ? 'l' : temp->way == 'l' ? 'r' : temp->way == 'u' ? 'd' : temp->way == 'd' ? 'u' : ' ');
				myStack.pop();
				continue;
			}
			for (unsigned i = 0; i < temp->children.size(); i++) {
				if (temp->children[i].manhatanVal == 0) {
					std::cout << myStack.size() << std::endl;
					myStack.push(&temp->children[i]);
					ReverseStack(myStack);
					return;
				}
				if (temp->children[i].manhatanVal + (int)myStack.size() <= threshold && !temp->children[i].visited) {
					temp->children[i].visited = true;
					Go(matrix, temp->children[i].way);
					myStack.push(&temp->children[i]);
					break;
				}
				else if (i == temp->children.size() - 1) {
					Go(matrix, temp->way == 'r' ? 'l' : temp->way == 'l' ? 'r' : temp->way == 'u' ? 'd' : temp->way == 'd' ? 'u' : ' ');
					myStack.pop();
				}
			}
		}
		threshold++;
		Clear(&root);
	}
}


int main()
{
	int side;
	std::cin >> size >> zeroPos;
	zeroPos = zeroPos == -1 ? size : zeroPos;
	size++;
	side = (int)sqrt(size);
	int** matrix = new int* [side];
	for (int i = 0; i < side; i++) {
		matrix[i] = new int[side];
		for (int j = 0; j < side; j++) {
			std::cin >> matrix[i][j];
		}
	}
	Trie myTree;
	myTree.add(matrix);
	Node root(matrix, ' ');

	if (!root.manhatanVal) {
		std::cout << "0" << std::endl;
		return 0;
	}
	ISearch(root, matrix, &myTree);

	for (int i = 0; i < side; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;

	return 0;
}