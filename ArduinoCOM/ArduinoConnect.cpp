#include "ArduinoCOM.h"
#include <iostream>

using namespace std;

int main()
{
	ArduinoCOM arduino;
	string line = "";
	bool isConnected = false;

	cout << "Attempting to connect to arduino on COM3..." << endl;
	if (arduino.Connect("COM3", 115200) == 0) cout << "Error encountered: " << arduino.GetError() << endl;
	else {
		isConnected = true;
		cout << "Connected on COM3, attempting to read from arduino..." << endl << endl;
		//Sleep(600);
	}
while (isConnected)
{
	
	cout << "Enter message: ";
	cin >> line;
	cout << endl;
	//line += '\0';

	if (!arduino.WriteLine(&line))
	{
		cout << endl << "Error encountered: " << arduino.GetError() << endl;
		return 1;
	}
	//Sleep(200);
	line = "";
	
	if (!arduino.ReadLine(&line))
	{
		isConnected = false;
		cout << endl << "Error encountered: " << arduino.GetError() << endl;
		return 1;
	}
	else cout << line;
	
	line = "";
	
}
	cout << "Disconnecting from arduino and exiting." << endl;
	arduino.Disconnect();


	return 0;
}