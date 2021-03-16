#include <iostream>
#include <utility>
#include <math.h>
#include <ctime> 
#include <algorithm>
#include <queue>

using namespace std;

short int sizeP = 0;
const short int maxN = 2;
const short int pop = 30;
const short int maxpop = pop * 3;

double distance(pair<int, int> left, pair<int, int> right) {
	double distance = pow(abs(left.first - right.first), 2);
	distance += pow(abs(left.second - right.second), 2);
	distance = sqrt(distance);
	return distance;
}

double way(pair<int, int>* path) {
	double length = 0;
	for (int i = 1; i < sizeP; i++) {
		length += distance(path[i - 1], path[i]);
	}
	return length;
}

void print(pair<int, int>* path) {
	/*for (int i = 0; i < 4; i++) {
		cout << path[i].first << "," << path[i].second << " ";
	}*/
	//cout << path[0].first << "," << path[0].second << " ";
	cout << way(path);
	cout << endl;
}

class Pqueue {
public:
	pair<int, int>** pool;
	int place;
	Pqueue() {
		pool = new pair<int, int>*[maxpop];
		for (int i = 0; i < maxpop; i++) {
			pool[i] = new pair<int, int>[sizeP];
		}
		place = 0;
	}
	~Pqueue() {
		for (int i = 0; i < maxpop; i++) {
			pool[i] = new pair<int, int>;
			delete[] pool[i];
		}
		delete[] pool;
	}
	bool checkIfIn(pair<int, int>* path) {
		short int br = 0;
		int f = 1;
		for (int i = 0; i < place; i++) {
			for (int j = 0; j < sizeP; j++) {
				if (pool[i][j].first != path[j].first || pool[i][j].second != path[j].second) {
					f = 0;
					break;
				}
			}
			if (f == 1) {
				br++;
				if (br >= maxN) {
					return true;
				}
			}
			f = 1;
		}
		return false;
	}
	bool add(pair<int, int>* path) {
		if (this->checkIfIn(path)) {
			return false;
		}
		for (int i = 0; i < place; i++) {
			if (way(pool[i]) > way(path)) {
				place++;

				for (int j = i; j < place; j++) {
					pair<int, int>* temp = pool[j];
					pool[j] = path;
					path = temp;
				}
				return true;
			}
		}
		pool[place] = path;
		place++;
		return true;
	}
	void mutate() {
		place = 10;
		int br = 0;
		for (int i = 10; i < pop; i++) {
			while (true) {
				short int length = rand() % (sizeP / 10) + 1;
				short int start1 = rand() % (sizeP - length);
				short int start2 = rand() % (sizeP - length);
				br = start2;
				for (int j = start1; j < length; j++) {
					pair<int, int> temp = pool[i][j];
					pool[i][j] = pool[i][br];
					pool[i][br] = temp;
					br++;
				}
				if (this->add(pool[i])) {
					break;
				}
			}
		}
	}
	void printP() {
		for (int i = 0; i < 1; i++) {
			print(pool[i]);
		}
	}
	void clear() {
		place = 0;
	}
};
void mutatex(pair<int, int>* path) {
	short int length = rand() % 3 + 1;
	short int start1 = rand() % (sizeP - length);
	short int start2 = rand() % (sizeP - length);
	int br = start2;
	for (int j = start1; j < length; j++) {
		pair<int, int> temp = path[j];
		path[j] = path[br];
		path[br] = temp;
		br++;
	}

}

pair<int, int>* generate(short int sizeP) {
	pair<int, int>* pairs = new pair<int, int>[sizeP];
	int x, j;
	srand(time(NULL));
	short int done[100][100];
	for (int i = 0; i < 100; i++) {
		for (int j = 0; j < 100; j++) {
			done[i][j] = 0;
		}
	}
	for (short int i = 0; i < sizeP; i++) {
		while (true) {
			x = rand() % 101;
			j = rand() % 101;
			if (done[x][j] == 0) {
				done[x][j] = 1;
				pairs[i].first = x;
				pairs[i].second = j;
				break;
			}
		}
	}
	return pairs;
}

bool checkIfThere(pair<int, int> pairr, pair<int, int>* path, short int start, short int end) {
	for (short int i = start; i < end; i++) {
		if (pairr.first == path[i].first && pairr.second == path[i].second) {
			return true;
		}
	}
	return false;
}

void mix(pair<int, int>* left, pair<int, int>* right, Pqueue* queue) {
	short int length = rand() % (sizeP)+1;
	length /= 2;
	length++;
	short int start = rand() % (sizeP - length);
	pair<int, int>* child[2];
	child[0] = new pair<int, int>[sizeP];
	child[1] = new pair<int, int>[sizeP];
	for (int i = start; i < start + length; i++) {
		child[0][i] = left[i];
		child[1][i] = right[i];
	}
	int brl = 0, brr = 0;
	for (int i = 0; i < start; i++) {
		for (int j = brr; j < sizeP; j++) {
			if (!checkIfThere(right[j], child[0], start, start + length)) {
				brr = j + 1;
				child[0][i] = right[j];
				break;
			}
		}
		for (int j = brl; j < sizeP; j++) {
			if (!checkIfThere(left[j], child[1], start, start + length)) {
				brl = j + 1;
				child[1][i] = left[j];
				break;
			}
		}
	}
	for (int i = length + start; i < sizeP; i++) {
		for (int j = brr; j < sizeP; j++) {
			if (!checkIfThere(right[j], child[0], start, start + length)) {
				brr = j + 1;
				child[0][i] = right[j];
				break;
			}
		}
		for (int j = brl; j < sizeP; j++) {
			if (!checkIfThere(left[j], child[1], start, start + length)) {
				brl = j + 1;
				child[1][i] = left[j];
				break;
			}
		}
	}
	while (queue->checkIfIn(child[0])) {
		mutatex(child[0]);
	}
	while (queue->checkIfIn(child[1])) {
		mutatex(child[1]);
	}
	queue->add(child[0]);
	queue->add(child[1]);
}

void mixUp(Pqueue* queue, int br) {
	int** done = new int* [maxpop];
	for (int i = 0; i < pop; i++) {
		done[i] = new int[maxpop];
		for (int j = 0; j < pop; j++) {
			if (j == i) {
				done[i][j] = 1;
			}
			else done[i][j] = 0;
		}
	}
	int randomi = 0;
	int randomj = 0;
	for (int i = 0; i < br; i++) {
		while (true) {
			randomi = rand() % pop + 1;
			randomi = rand() % randomi;
			randomj = rand() % pop + 1;
			randomj = rand() % randomj;
			if (done[randomi][randomj] != 1) {
				done[randomi][randomj] = 1;
				done[randomj][randomi] = 1;
				break;
			}
		}
		mix(queue->pool[randomi], queue->pool[randomj], queue);
	}
	for (int i = 0; i < pop; i++) {
		delete[] done[i];
	}
	delete[] done;
}

int main()
{
	cin >> sizeP;
	pair<int, int>* pairs = generate(sizeP);
	pair<int, int>** pool = new pair<int, int>*[pop];
	Pqueue* queue = new Pqueue;


	for (int i = 0; i < pop; i++) {
		pool[i] = new pair<int, int>[sizeP];
		for (int j = 0; j < sizeP; j++) {
			pair<int, int> temp = pairs[j];
			pool[i][j] = temp;
		}
		while (true) {
			random_shuffle(pool[i], pool[i] + sizeP);
			if (queue->add(pool[i])) {
				break;
			}
		}
	}


	for (int i = 0; i < 200; i++) {
		mixUp(queue, pop);
		queue->place = pop;
		if (i == 9 || i == 49 || i == 99 || i == 149 || i == 199) {
			cout << i + 1 << ": ";
			queue->printP();
			cout << endl;
		}
	}


	for (int i = 0; i < pop; i++) {
		pool[i] = new pair<int, int>;
		delete[] pool[i];
	}
	delete[] pool;
	delete[] pairs;
	return 0;
}