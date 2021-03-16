#include <iostream>
#include <vector>

using namespace std;

bool checkIt(char board[3][3]) {
	for (int i = 0; i < 3; i++) {
		if ((board[i][0] == board[i][1] && board[i][0] == board[i][2] && board[i][0] != ' ') ||
			(board[0][i] == board[1][i] && board[0][i] == board[2][i] && board[0][i] != ' ')) {
			return true;
		}
	}
	if ((board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[1][1] != ' ') ||
		(board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[1][1] != ' ')) {
		return true;
	}
	else {
		return false;
	}
}

class Node {
public:
	int value;
	char state[3][3] = { { ' ', ' ', ' ' }, { ' ', ' ', ' ' }, { ' ', ' ', ' ' } };
	vector<Node> child;

	Node() { value = 0; }
	Node(char board[3][3], int x, int y, char sym) {
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				this->state[i][j] = board[i][j];
			}
		}
		this->state[x][y] = sym;
		if (checkIt(state)) {
			value = 1;
		}
		else value = 0;
	}

	void createChildren(char sym) {
		if (this->child.size() > 0) return;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				if (this->state[i][j] == ' ') {
					Node temp(this->state, i, j, sym);
					this->child.push_back(temp);
				}
			}
		}
	}

};

void print(char board[3][3]) {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << board[i][j];
			if (j < 2) cout << "|";
		}
		cout << endl;
		cout << "------" << endl;
	}
}

Node* min(Node* current, int depth, char sym, char next, int alpha, int beta);

Node* max(Node* current, int depth, char sym, char next, int alpha, int beta) {
	if (current->value == 1) {
		current->value = 0 - (20 - depth);
		return current;
	}
	current->createChildren(sym);
	if (current->child.size() == 0) {
		return current;
	}
	current->value = -100;
	Node* temp = new Node;
	for (int i = 0; i < current->child.size(); i++) {
		Node* temp1 = min(&current->child[i], depth + 1, next, sym, alpha, beta);
		if (temp1->value > current->value) {
			temp = &current->child[i];
			current->value = temp1->value;
			alpha = current->value;
			if (temp1->value >= beta) break;
		}
	}
	return temp;
}

Node* min(Node* current, int depth, char sym, char next, int alpha, int beta) {
	if (current->value == 1) {
		current->value = 20 - depth;
		return current;
	}
	current->createChildren(sym);
	if (current->child.size() == 0) {
		return current;
	}
	current->value = 100;
	Node* temp = new Node;
	for (int i = 0; i < current->child.size(); i++) {
		Node* temp1 = max(&current->child[i], depth + 1, next, sym, alpha, beta);
		if (temp1->value < current->value) {

			temp = &current->child[i];
			current->value = temp1->value;
			beta = current->value;
			if (temp1->value <= alpha) break;
		}
	}
	return temp;
}


int main()
{
	char board[3][3] = { { ' ', ' ', ' ' },{ ' ', ' ', ' ' },{ ' ', ' ', ' ' } };
	int choose = 0;
	while (true)
	{
		cout << "Type 1 for first and 2 for second: ";
		cin >> choose;
		if (choose == 1 || choose == 2) break;
	}
	int x, y;
	int turn = 0;
	Node* temp = new Node;
	if (choose == 1) {
		while (true) {
			turn++;
			Node* temp = new Node;
			while (true) {
				cout << "Enter coordinates for your play(eg, 1 1): ";
				cin >> x >> y;
				if (board[--x][--y] == ' ') {
					board[x][y] = 'x';
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							temp->state[i][j] = board[i][j];
						}
					}
					if (checkIt(board)) {
						print(board);
						cout << "Player has won!" << endl;
						return 0;
					}
					break;
				}
				cout << "Wront coordinates!" << endl;
			}
			print(board);
			if (turn == 5) {
				cout << "Draw!" << endl;
				return 0;
			}
			cout << endl;

			temp = max(temp, 0, 'o', 'x', -100, 100);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					board[i][j] = temp->state[i][j];
				}
			}
			print(board);
			if (checkIt(board)) {
				cout << "Computer has won!" << endl;
				return 0;
			}
		}
	}
	else {
		while (true) {
			turn++;

			temp = max(temp, 0, 'o', 'x', -100, 100);
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					board[i][j] = temp->state[i][j];
				}
			}
			print(board);
			if (checkIt(board)) {
				cout << "Computer has won!" << endl;
				return 0;
			}

			if (turn == 5) {
				cout << "Draw!" << endl;
				return 0;
			}
			cout << endl;
			temp = new Node;
			while (true) {
				cout << "Enter coordinates for your play(eg, 1 1): ";
				cin >> x >> y;
				if (board[--x][--y] == ' ') {
					board[x][y] = 'x';
					for (int i = 0; i < 3; i++) {
						for (int j = 0; j < 3; j++) {
							temp->state[i][j] = board[i][j];
						}
					}
					if (checkIt(board)) {
						print(board);
						cout << "Player has won!" << endl;
						return 0;
					}
					break;
				}
				cout << "Wront coordinates!" << endl;
			}
			print(board);
		}
	}
	return 0;
}