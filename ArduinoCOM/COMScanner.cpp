#include "ArduinoCOM.h"
#include <iostream>

using namespace std;

int main()
{
	ArduinoCOM scanner;
	cout << "Scanning COM ports..." << endl;
	
	scanner.ScanPorts();

	cout << scanner.availablePorts.size() << " COM ports available:" << endl;

	for (int i = 0; i < scanner.availablePorts.size(); i++)
	{
		cout << scanner.availablePorts[i] << "		";
	}
	cout << endl;

	return 0;
}