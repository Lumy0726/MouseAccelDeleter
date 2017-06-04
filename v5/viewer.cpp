#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

int main(int arg_num, char * argv[], char * env[]) {
	int initV, tempInt1, tempInt2;
	ifstream fileI("Mouse_Accel_Optimizer.conf");
	ofstream fileO("Mouse_accel_Optimizer_ratio.txt");
	if (!fileI) { return 0; }
	fileO << fixed << setprecision(3);
	fileI >> tempInt1;
	fileO << tempInt1 << endl;
	fileI >> tempInt1 >> tempInt2 >> tempInt1;
	fileI >> tempInt1 >> initV >> tempInt1;
	fileO << setw(4) << 0 << '\t' << setw(7) << (double)initV / tempInt2 << endl;
	fileO << setw(4) << 1 << '\t' << setw(7) << 1.0 << endl;
	while (true) {
		fileI >> tempInt1;
		if (fileI.eof())break;
		fileO << setw(4) << tempInt1 << '\t';
		fileI >> tempInt1;
		fileO << setw(7) << (double)initV / tempInt1 << endl;
		fileI >> tempInt1;
	}
	return 0;
}