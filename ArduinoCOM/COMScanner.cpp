#include "ArduinoCOM.h"
#include <iostream>

using namespace std;

int main()
{
	ArduinoCOM scanner;
	cout << "Scanning COM ports..." << endl;
	//cout << "Available COM ports:" << endl;
	for (int i = 0; i < 20; i++)
	{
		if (scanner.isValidPort(i)) cout << "COM" << i << " AVAILABLE" << endl;
		//else cout << " NOT AVAILABLE" << endl;

	}
	return 0;
}