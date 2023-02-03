#include "ArduinoCOM.h"
#include <iostream>

using namespace std;

int main()
{
	ArduinoCOM arduino;
	arduino.COMPortName = "COM6";
	arduino.BaudRate = 9600;
	string line = "";
	//bool isConnected = false;

	cout << "Attempting to connect to arduino on " << arduino.COMPortName << "..." << endl;
	if (arduino.Connect() == 0) cout << "Error encountered: " << arduino.GetError() << endl;
	else {
		//isConnected = true;
		cout << "Connected on " <<arduino.COMPortName << ", attempting to read from arduino..." << endl << endl;
		//Sleep(600);
	}
while (arduino.isConnected())
{
	
//	cout << "Enter message: ";
	//std::getline(std::cin, line);
//	cout << endl;
	//line += '\0';
	//cout << line;
//	if (!arduino.WriteLine(&line))
	{
//		cout << endl << "Error encountered: " << arduino.GetError() << endl;
//		return 1;
	}
//	Sleep(100);
	line = "";
	
	if (!arduino.ReadLine(&line))
	{
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