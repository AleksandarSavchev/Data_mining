#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <algorithm>
#include <time.h>

using namespace std;

class Stat {
public:
	string name;
	int num, numY, numN;
	Stat(string name) {
		numY = 0;
		numN = 0;
		num = 1;
		this->name = name;
	}

	double E() {
		double pa = numY / (double)num;
		double pb = numN / (double)num;
		return numY == 0 ? 0 : numN == 0 ? 0 : (0 - (pa * log2(pa)) - (pb * log2(pb)));
	}

};
class WholeStat {
public:
	vector<Stat> all;
	WholeStat() {  }
	void add(string name, string type) {
		int f = 0;
		for (int i = 0; i < all.size(); i++) {
			if (all[i].name == name) {
				f = 1;
				all[i].num++;
				if (type == "recurrence-events") {
					all[i].numY++;
				}
				else all[i].numN++;
				break;
			}
		}
		if (f == 0) {
			Stat temp(name);
			if (type == "recurrence-events") {
				temp.numY++;
			}
			else temp.numN++;
			all.push_back(temp);
		}
	}
	int GetNum() {
		int res = 0;
		for (int i = 0; i < all.size(); i++) {
			res += all[i].num;
		}
		return res;
	}
	double E() {
		double res = 0.0;
		for (int i = 0; i < all.size(); i++) {
			res += (all[i].num / (double)GetNum()) * all[i].E();
		}
		return res;
	}
	void print() {
		for (int i = 0; i < all.size(); i++) {
			cout << all[i].name << ": " << all[i].num / (double)GetNum() << " ";
		}
		cout << endl;
	}
};
class Person {
public:
	vector<string> stats;
	Person(string* stats) {
		for (int i = 0; i < 10; i++) {
			this->stats.push_back(stats[i]);
		}
	}

	void print() {
		for (int i = 0; i < stats.size(); i++) {
			cout << stats[i] << " ";
		}
		cout << endl;
	}
};

class Party {
public:
	vector<Person> people;
	vector<WholeStat> all;
	vector<pair<int, double>> res;
	int numY, numN;
	Party() { numY = 0; numN = 0; }

	void add(Person person) {
		if (person.stats[0] == "recurrence-events")
			numY++;
		else numN++;
		people.push_back(person);
		if (all.size() == 0) {
			for (int i = 0; i < people[0].stats.size(); i++) {
				WholeStat* temp = new WholeStat;
				all.push_back(*temp);
			}
		}

		for (int i = 0; i < people[0].stats.size(); i++) {
			all[i].add(person.stats[i], person.stats[0]);
		}
	}

	bool check(int first, int second, vector<pair<int, double>> prio) {
		if (first > prio[0].first) {
			first++;
		}
		if (second > prio[0].first) {
			second++;
		}
		for (int i = 0; i < prio.size(); i++) {
			if (prio[i].first == second)
				return false;
			if (prio[i].first == first)
				return true;
		}
	}
	int BestInfGain(vector<pair<int, double>> prio) {
		double pa = numY / (double)people.size();
		double pb = numN / (double)people.size();
		pair<int, double> temp;
		int flag = 0;
		for (unsigned i = 1; i < people[0].stats.size(); i++) {
			temp.first = i;
			if (numY == 0 || numN == 0) {
				temp.second = 0;
			}
			else temp.second = (0 - (pa * log2(pa)) - (pb * log2(pb)));
			temp.second -= all[i].E();

			for (int j = 0; j < res.size(); j++) {
				if ((temp.second == res[j].second && prio.size() > 1 && check(temp.first, res[j].first, prio)) || temp.second > res[j].second) {
					res.insert(res.begin() + j, temp);
					flag = 1;
					break;
				}
			}
			if (!flag) {
				res.push_back(temp);
			}

			flag = 0;
		}
		return res[0].first;
	}

	bool same() {
		string temp = people[0].stats[0];
		for (unsigned int i = 0; i < people.size(); i++) {

			if (temp != people[i].stats[0]) {
				return false;
			}
		}
		return true;
	}

	void print() {
		for (unsigned i = 0; i < all.size(); i++) {
			all[i].print();
		}
		cout << endl;
	}
};

class Test {
public:
	vector<Person> people;
	Test() {}
	void add(Person* person) {
		people.push_back(*person);
	}
};

class Node {
public:
	Party* party;
	string result;
	string most;
	int index;
	string sname;
	vector<Node*> children;
	Node(Party* party, string sname) {
		this->party = party;
		this->result = "";
		this->most = "";
		this->sname = sname;
	}
	void print() {
		cout << index << " " << sname << " " << children.size() << " " << result << endl;
	}
};

class DecisionTree {
public:
	Node* root;
	DecisionTree(Party* party) {
		this->root = new Node(party, "");
		this->root->result = "";
	}

	void MakeTree(Node* root, int depth, vector<int> done, vector<pair<int, double>> prio) {
		int k = 2;
		if (root->party->same()) {
			root->result = root->party->people[0].stats[0];
			return;
		}
		else {
			if (root->party->all[0].all[0].num / (double)root->party->all[0].GetNum() >= 0.5) {
				root->most = root->party->all[0].all[0].name;
			}
			else root->most = root->party->all[0].all[1].name;
		}
		int index = root->party->BestInfGain(prio);
		if (root->party->all[index].all.size() < k)
			return;
		int br = 0;
		for (int i = 0; i < done.size(); i++) {
			if (br + index >= done[i])
				br++;
			else break;
		}
		done.push_back(br + index);
		sort(done.begin(), done.end());
		root->index = index + br;
		for (unsigned int i = 0; i < root->party->all[index].all.size(); i++) {
			Party* temp = new Party;
			string current = root->party->all[index].all[i].name;
			for (unsigned int j = 0; j < root->party->people.size(); j++) {
				if (root->party->people[j].stats[index] == current) {
					Person tempp = root->party->people[j];
					//tempp.print();
					tempp.stats.erase(tempp.stats.begin() + index);

					temp->add(tempp);
				}
			}
			//cout << endl;
			Node* t = new Node(temp, current);
			if (t->party->all.size() == 1) {
				root->children.push_back(t);
			}
			else {
				root->children.push_back(t);
				MakeTree(t, depth + 1, done, root->party->res);
			}
		}
	}

	string findWhich(Person* person, Node* root) {
		for (int i = 0; i < root->children.size(); i++) {
			if (person->stats[root->index] == root->children[i]->sname) {
				if (root->children[i]->result != "") {
					return root->children[i]->result;
				}
				else return findWhich(person, root->children[i]);
			}
		}
		return root->most;
	}
	void DFSprint(Node* root) {
		for (int i = 0; i < root->children.size(); i++) {
			root->children[i]->party->print();
		}
	}
};


int main()
{
	Party party[10];
	Test tests[10];
	int size = 10;
	string line;
	ifstream myfile("breast-cancer.data");
	for (int k = 0; k >= 0; k++)
	{
		if (!getline(myfile, line))
			break;
		int i = 0;
		string word;
		string stats[10];
		stringstream ss(line);
		while (getline(ss, word, ','))
		{
			stats[i] = word;
			i++;
		}
		Person* temp = new Person(stats);
		tests[k % size].add(temp);
		for (int l = 0; l < size; l++) {
			if (l == k % size) continue;
			party[l].add(*temp);
		}
	}
	myfile.close();
	vector<int> done;
	double sum = 0;
	vector<pair<int, double>> prio;
	for (int j = 0; j < size; j++) {
		int num = 0;
		int correct = 0;
		DecisionTree* tree = new DecisionTree(&party[j]);
		tree->MakeTree(tree->root, 0, done, prio);
		for (int i = 0; i < tests[j].people.size(); i++) {
			if (tree->findWhich(&tests[j].people[i], tree->root) == tests[j].people[i].stats[0])
				correct++;
			num++;
		}
		double accuracy = correct / (double)num;
		sum += accuracy;
		cout << "Test " << j + 1 << " has accuracy of: " << accuracy << endl;
	}
	cout << "Median is " << sum / (double)size << endl;
	return 0;
}
