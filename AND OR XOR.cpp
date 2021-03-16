#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <string>

using namespace std;

const double m = 0.5;
const double e = 2.71828;

class SimpleGate {
public:
	double weight[5];
	string type;
	double input[4][2] = { { 0.0, 0.0 },{ 1.0, 0.0 },{ 0.0, 1.0 },{ 1.0, 1.0 } };
	double exOutput[4] = { 0.0, 0.0, 0.0, 1.0 };
	double output[4];
	SimpleGate(string type) {
		srand(time(NULL));
		for (int i = 0; i < 5; i++) {
			weight[i] = (rand() % 110 - 50) * 0.001;
		}
		this->type = type;
		if (type == "or") {
			exOutput[1] = 1.0;
			exOutput[2] = 1.0;
		}
		if (type == "nand") {
			exOutput[0] = 1.0;
			exOutput[1] = 1.0;
			exOutput[2] = 1.0;
			exOutput[3] = 0.0;
		}
	}
	double* calc() {
		for (int i = 0; i < 4; i++) {
			double sum = weight[0] + weight[1] * input[i][0] + weight[2] * input[i][1];
			double ok = 1 / (1 + pow(e, sum * -1));
			output[i] = ok;
			double err = (ok + 0.001) * (1.001 - ok) * (exOutput[i] - ok);
			weight[0] = weight[0] + m * err;
			weight[1] = weight[1] + m * err * input[i][0];
			weight[2] = weight[2] + m * err * input[i][1];

		}
		return output;
	}
	void print() {
		cout << "A B " << type << endl;
		for (int i = 0; i < 4; i++) {
			cout << input[i][0] << " " << input[i][1] << " " << output[i] << endl;
		}
	}
};

class MultiGate {
public:
	double weight[3];
	string type;
	double input[4][2] = { { 0.0, 0.0 },{ 1.0, 0.0 },{ 0.0, 1.0 },{ 1.0, 1.0 } };
	double exOutput[4] = { 0.0, 1.0, 1.0, 0.0 };
	double output[4];

	MultiGate() {
		srand(time(NULL));
		for (int i = 0; i < 3; i++) {
			weight[i] = (rand() % 110 - 50) * 0.001;
		}
		this->type = "xor";
	}
	void calc(int num) {
		SimpleGate nand("nand");
		SimpleGate orr("or");
		for (int j = 0; j < num; j++) {
			double* input1 = nand.calc();
			double* input2 = orr.calc();
			for (int i = 0; i < 4; i++) {
				double sum = weight[0] * input1[i] + weight[1] * input2[i] + weight[2];
				double ok = 1 / (1 + pow(e, sum * -1));
				output[i] = ok;
				double err = (ok + 0.001) * (1.001 - ok) * (exOutput[i] - ok);
				weight[0] += m * err * input1[i];
				weight[1] += m * err * input2[i];
				weight[2] += m * err;
			}
		}
	}
	void print() {
		cout << "A B " << type << endl;
		for (int i = 0; i < 4; i++) {
			cout << input[i][0] << " " << input[i][1] << " " << output[i] << endl;
		}
	}
};

int main()
{
	cout << "Enter or, and or xor:";
	string type;
	cin >> type;
	if (type == "xor") {
		MultiGate network;
		network.calc(1000000);
		network.print();
	}
	else if (type == "or" || type == "and") {
		SimpleGate network(type);
		for (int i = 0; i < 100000; i++) {
			network.calc();
		}
		network.print();
	}
	return 0;
}

