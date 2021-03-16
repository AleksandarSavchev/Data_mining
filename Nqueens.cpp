#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

class Board {
public:
	int size;
	int* q;
	int* c;
	int* d1;
	int* d2;
	Board(int size) {
		this->size = size;
		this->q = new int[size];
		this->c = new int[size];
		for (int i = 0; i < size; i++) {
			this->c[i] = 0;
		}
		this->d1 = new int[2 * size - 1];
		for (int i = 0; i < 2 * size - 1; i++) {
			this->d1[i] = 0;
		}
		this->d2 = new int[2 * size - 1];
		for (int i = 0; i < 2 * size - 1; i++) {
			this->d2[i] = 0;
		}
		for (int i = 0; i < size / 2; i++) {
			int col = 2 * i + 1;
			this->q[i] = col;
			this->c[col]++;
			this->d1[col - i + this->size - 1] ++;
			this->d2[i + col] ++;
		}
		int br = 0;
		for (int i = size / 2; i < size; i++) {
			int col = 2 * br;
			br++;
			this->q[i] = col;
			this->c[col]++;
			this->d1[col - i + this->size - 1] ++;
			this->d2[i + col] ++;
		}
	}

	void print() {
		for (int i = 0; i < this->size; i++) {
			for (int j = 0; j < this->size; j++) {
				if (j == this->q[i]) {
					cout << "*";
				}
				else {
					cout << "-";
				}
			}
			cout << endl;
		}
		cout << endl;
	}
	int mostContLine() {
		int most = 0;
		int current = 0;
		int* max = new int[this->size];
		int br = 0;
		for (int i = 0; i < this->size; i++) {
			current += this->c[this->q[i]] - 1;
			current += this->d1[this->q[i] - i + this->size - 1] - 1;
			current += this->d2[i + this->q[i]] - 1;
			if (current > most) {
				most = current;
				br = 0;
				max[br] = i;
			}
			else if (current == most) {
				br++;
				max[br] = i;
			}
			current = 0;
		}

		int random = rand() % ++br;
		return max[random];
	}
	int LeastContCol(int line) {
		int least = 100000;
		int current = 0;
		this->c[q[line]] --;
		this->d1[q[line] - line + this->size - 1] --;
		this->d2[line + q[line]] --;
		int* min = new int[this->size];
		int br = 0;
		for (int i = 0; i < this->size; i++) {
			current += this->c[i];
			current += this->d1[i - line + this->size - 1];
			current += this->d2[line + i];
			if (current < least) {
				least = current;
				br = 0;
				min[br] = i;
			}
			else if (current == least) {
				br++;
				min[br] = i;
			}
			current = 0;
		}

		int random = rand() % ++br;
		return min[random];
	}
	bool place(int line, int col) {
		this->q[line] = col;
		this->c[col] ++;
		this->d1[col - line + this->size - 1] ++;
		this->d2[line + col] ++;

		int current = 0;

		for (int i = 0; i < this->size; i++) {
			current += this->c[this->q[i]] - 1;
			current += this->d1[this->q[i] - i + this->size - 1] - 1;
			current += this->d2[i + this->q[i]] - 1;
			if (current > 0) {
				return false;
			}
		}
		return true;
	}
};

void findPlace(Board queens) {
	while (true) {
		int line = queens.mostContLine();
		int col = queens.LeastContCol(line);
		if (queens.place(line, col))
			break;
	}
	queens.print();
	return;
}


int main()
{
	int size;
	cin >> size;
	Board queens(size);
	findPlace(queens);
	return 0;
}