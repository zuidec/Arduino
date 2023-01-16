#include "ArduinoCOM.h"
#include <iostream>

using namespace std;

int main()
{
	ArduinoCOM arduino;
	string line = "";
	bool isConnected = false;

	cout << "Attempting to connect to arduino on COM3..." << endl;
	if (arduino.Connect("COM3", 9600) == 0) cout << "Error encountered: " << arduino.GetError() << endl;
	else {
		isConnected = true;
		cout << "Connected on COM3, attempting to read from arduino..." << endl << endl;
	}

	while (isConnected) 
	{
		if (!arduino.ReadLine(&line))
		{
			isConnected = false;
			cout << endl << "Error encountered: " << arduino.GetError() << endl;
		}
		else cout << "\r                      \rLight level: " << line;
		line = "";
	}

	cout << "Disconnecting from arduino and exiting." << endl;
	arduino.Disconnect();


	return 0;
}