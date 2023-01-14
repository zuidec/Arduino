#include <iostream>
#include <fstream>

int main()
{
	HANDLE arduino; // Create new filestream named arduino
	
	try 
	{
		std::cout << "Attempting to connect to COM3..." << std::endl; 	// Attempt to connect to arduino
		arduino = CreateFile ("COM3",
				GENERIC_READ | GENERIC_WRITE,
				0,
				0,
				OPEN_EXISTING,
				FILE_FLAG_OVERLAPPED,
				0);						

		if(arduino == INVALID_HANDLE_VALUE) throw -1;
		else std::cout << "Successfully connected on COM3." << std::endl;
	}
	catch (...)
	{
		std::cout << "Error connecting on COM3, exiting now" << std:: endl;	// Catch exception and exit if
		arduino.close();							// unsuccessful
		return -1;									// 
	}
	
	//arduino.close();	//Terminate arduino filestream before closing
	return 0;
}
