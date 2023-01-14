#include <iostream>
#include <fstream>

int main()
{
	std::ifstream arduino; // Create new filestream named arduino
	
	try 
	{
		std::cout << "Attempting to connect to COM3..." << std::endl; 	// Attempt to connect to arduino
		arduino.open("COM3",std::ios::in);						

		if(arduino.fail()) throw -1;
		else if (arduino.is_open()) std::cout << "Successfully connected on COM3." << std::endl;
	}
	catch (...)
	{
		std::cout << "Error connecting on COM3, exiting now" << std:: endl;	// Catch exception and exit if
		arduino.close();							// unsuccessful
		return -1;									// 
	}
	
	arduino.close();	//Terminate arduino filestream before closing
	return 0;
}
