#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>

int main()
{
	HANDLE arduino; // Create new filestream named arduino
	
	try 
	{
		std::cout << "Attempting to connect to COM3..." << std::endl; 	// Attempt to connect to arduino
		arduino = CreateFile ("COM3",			// COM port 3
				GENERIC_READ | GENERIC_WRITE,	// Open read and write
				0,
				0,
				CREATE_ALWAYS,					// Alwayts create a new instance
				FILE_FLAG_OVERLAPPED,			// Use overlapping
				0);						

		if(arduino == INVALID_HANDLE_VALUE) throw -1;
		else std::cout << "Successfully connected on COM3." << std::endl;
	}
	catch (...)
	{
		DWORD comError = GetLastError();
		std::cout << "Error connecting on COM3, exiting now, error number: "<< comError << std:: endl;	// Catch exception and exit if
		CloseHandle(arduino);																								// unsuccessful
		return -1;									 
	}
	std::string buffer = "";
	while (1) /// Enter into main loop for receiving data from arduino
	{
		//Read a line from arduino and store into the buffer
		std::cin.getline(buffer,25,'\r') << arduino.;
		
		std::cout << std::endl << "\rThe current light level is: " << buffer;
	}

	CloseHandle(arduino);
	return 0;
}
