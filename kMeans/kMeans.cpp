#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <cmath>

using namespace std;

double distance(pair<double, double> a, pair<double, double> b) {
	double x = (a.first - b.first) * (a.first - b.first);
	double y = (a.second - b.second) * (a.second - b.second);
	return sqrt(x + y);
}

class Cluster {
public:
	vector<pair<double, double>> points;
	pair<double, double> cen;
	Cluster() {}
	void add(pair<double, double> point) {
		points.push_back(point);
	}
	bool calcCen() {
		double x = 0;
		double y = 0;
		for (int i = 0; i < points.size(); i++) {
			x += points[i].first / points.size();
			y += points[i].second / points.size();
		}
		x = ceil(x * 1000.0) / 1000.0;
		y = ceil(y * 1000.0) / 1000.0;
		if (cen.first == x && cen.second == y) return true;
		cout << cen.first << "," << cen.second << " " << x << "," << y << endl;
		cen.first = x;
		cen.second = y;
		return false;
	}
	double completeDeam() {
		double max = 0;
		for (int i = 0; i < points.size(); i++) {
			for (int j = 0; j < points.size(); j++) {
				double curr = distance(points[i], points[j]);
				if (curr > max)
					max = curr;
			}
		}
		return max;
	}
	double cenDeam() {
		double deam = 0;
		for (int i = 0; i < points.size(); i++) {
			deam += distance(points[i], cen);

		}
		return deam / points.size();
	}
	void clear() {
		points.clear();
	}

	void write(ofstream& result, string color) {
		for (int i = 0; i < points.size(); i++) {
			result << points[i].first << " " << points[i].second << " " << color << endl;
		}
	}
};



int main()
{
	string colors[10] = { "Blue", "Red", "Green", "BlueViolet","Cyan", "Magenta", "Orange", "Purple", "Yellow", "Gray" };
	string file;
	int clusterCount;
	cin >> file >> clusterCount;
	ifstream myfile(file);
	Cluster all;
	string line;
	int count = 0;
	while (getline(myfile, line)) {
		count++;
		pair<double, double> temp;
		myfile >> temp.first >> temp.second;
		all.add(temp);
	}
	Cluster* clusters = new Cluster[clusterCount];

	double max = 0;
	double min = 0;
	for (int k = 0; k < 10; k++) {
		for (int i = 0; i < clusterCount; i++) {
			int ran = rand() % count;
			clusters[i].cen = all.points[ran];
		}
		while (true) {
			for (int i = 0; i < count; i++) {
				int best = 0;
				double bestD = 100000;
				for (int j = 0; j < clusterCount; j++) {
					if (bestD > distance(all.points[i], clusters[j].cen)) {
						bestD = distance(all.points[i], clusters[j].cen);
						best = j;
					}
				}
				clusters[best].add(all.points[i]);
			}
			int flag = 0;
			for (int i = 0; i < clusterCount; i++) {
				if (!clusters[i].calcCen())
					flag++;
			}
			if (!flag)
				break;
			for (int i = 0; i < clusterCount; i++) {
				clusters[i].clear();
			}
		}
		double curr = 0;
		double complete = 0;
		for (int i = 0; i < clusterCount; i++) {
			/*for (int j = 0; j < clusterCount; j++) {
				curr += (distance(clusters[i].cen, clusters[j].cen));
			}*/
			complete += clusters[i].cenDeam();
		}
		if (complete < min || k == 0) {
			ofstream result("result.txt");
			for (int i = 0; i < clusterCount; i++) {
				clusters[i].write(result, colors[i]);
			}
			min = complete;
		}
		for (int i = 0; i < clusterCount; i++) {
			clusters[i].clear();
		}
	}
	return 0;
}

