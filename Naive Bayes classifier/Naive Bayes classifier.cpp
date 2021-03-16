#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;
class Person {
public:
	char chars[17];
	Person() {
		for (int i = 0; i < 17; i++) {
			chars[i] = '?';
		}
	}

	void print() {
		for (int i = 0; i < 17; i++) {
			cout << chars[i] << " ";
		}
		cout << endl;
	}
};

class Party {
public:
	vector<Person> people;
	int numR, numD;
	double propRY[17], propRN[17], propDY[17], propDN[17];
	Party() {
		numR = 0;
		numD = 0;
		for (int i = 0; i < 16; i++) {
			propRY[i] = 0;
			propRN[i] = 0;
			propDY[i] = 0;
			propDN[i] = 0;
		}
	}

	void add(Person* person) {
		if (person->chars[0] == 'r')
			numR++;
		else numD++;
		people.push_back(*person);
	}

	void calcProp() {
		int yesR[16], noR[16], yesD[16], noD[16];
		for (int i = 0; i < 16; i++) {
			yesR[i] = 0;
			noR[i] = 0;
			yesD[i] = 0;
			noD[i] = 0;
		}
		for (int i = 0; i < people.size(); i++) {
			for (int j = 1; j < 17; j++) {
				if (people[i].chars[0] == 'r' && people[i].chars[j] == 'y')
					yesR[j - 1]++;
				else if (people[i].chars[0] == 'r' && people[i].chars[j] == 'n')
					noR[j - 1]++;
				else if (people[i].chars[0] == 'd' && people[i].chars[j] == 'y')
					yesD[j - 1]++;
				else if (people[i].chars[0] == 'd' && people[i].chars[j] == 'n')
					noD[j - 1]++;
			}
		}
		propDY[0] = numD / (double)people.size();
		propRY[0] = numR / (double)people.size();
		for (int i = 1; i < 17; i++) {
			propRY[i] = yesR[i - 1] / (double)numR;
			propRN[i] = noR[i - 1] / (double)numR;
			propDY[i] = yesD[i - 1] / (double)numD;
			propDN[i] = noD[i - 1] / (double)numD;
		}

	}

	void print() {
		for (int i = 0; i < 16; i++) {
			cout << propRY[i] << " " << propRN[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < 16; i++) {
			cout << propRY[i] << " " << propRN[i] << " ";
		}
		cout << endl;
	}
};


int main()
{
	Party party[20];
	string line;
	ifstream myfile("house-votes-84.data");
	for (int k = 0; k >= 0; k++)
	{
		if (!getline(myfile, line))
			break;
		Person* temp = new Person;
		int i;
		temp->chars[0] = line[0];
		if (line[0] == 'r') {
			i = 11;
		}
		else i = 9;
		for (int j = 1; j < 17; j++) {
			temp->chars[j] = line[i];
			i += 2;
		}
		party[k % 10].add(temp);
		for (int l = 10; l < 20; l++) {
			if (l - 10 == k % 10) continue;
			party[l].add(temp);
		}
	}
	myfile.close();
	for (int i = 0; i < 20; i++) {
		party[i].calcProp();
	}

	double sum = 0.0;
	for (int i = 10; i < 20; i++) {
		int num = 0;
		int correct = 0;
		int j = i - 10;
		for (int k = 0; k < party[j].people.size(); k++) {
			double rep = log(party[i].propRY[0]);
			double dem = log(party[i].propDY[0]);
			for (int l = 1; l <= 16; l++) {
				rep += log(party[j].people[k].chars[l] == 'y' ? party[i].propRY[l] : party[j].people[k].chars[l] == 'n' ? party[i].propRN[l] : 1 - (party[i].propRN[l] + party[i].propRY[l]));
				dem += log(party[j].people[k].chars[l] == 'y' ? party[i].propDY[l] : party[j].people[k].chars[l] == 'n' ? party[i].propDN[l] : 1 - (party[i].propDN[l] + party[i].propDY[l]));
			}
			num++;
			if (rep >= dem && party[j].people[k].chars[0] == 'r') {
				correct++;
			}
			else if (rep < dem && party[j].people[k].chars[0] == 'd') {
				correct++;
			}
		}
		double accuracy = correct / (double)num;
		sum += accuracy;
		accuracy = ceil(accuracy * 100.0);
		cout << "Test " << j + 1 << " has accuracy of: " << accuracy << "%" << endl;
	}
	cout << "Mean: " << ceil((sum / 10.0) * 100.0) << "%" << endl;
	return 0;
}
