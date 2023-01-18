#include "ArduinoCOM.h"
//#include <iostream> // Using to debug with cout

ArduinoCOM::ArduinoCOM()
{
	
}

ArduinoCOM::~ArduinoCOM()
{
	Disconnect();
}

void ArduinoCOM::Initialize()
{
	// When using asynchronous IO, ReadFile() requires the last argument to be a point to an overlapped object, which is declared here. We are required to set hEvent, Offset, and OffsetHigh, but there are more properties
	/*PortOverlap.hEvent = NULL;		//
	PortOverlap.Offset = 0;			//
	PortOverlap.OffsetHigh = 0;		//
	*/ //Currently not working correctly when using asynchronous IO, switched to synchronous

	// Create a timeout profile with the following parameters
	PortTimeouts.ReadIntervalTimeout = 500;			//	500ms between receiving bytes before timing out
	PortTimeouts.ReadTotalTimeoutConstant = 100;	//	100ms added to (ReadTotalTimeoutMultiplier * bytes requested) before timing out
	PortTimeouts.ReadTotalTimeoutMultiplier = 500;	//	
	PortTimeouts.WriteTotalTimeoutConstant = 1;		//
	PortTimeouts.WriteTotalTimeoutMultiplier = 1;	//
	
	// Set required properties of DCB object
	memset(&Port, 0, sizeof(Port));		//
	Port.DCBlength = sizeof(Port);		//

	LastError = "";	// Start with an empty error
}

int ArduinoCOM::Connect(char* COMPortName, int BaudRate)
{
	Initialize();
	CommDCBParameter = "baud=" + std::to_string(BaudRate) + " parity=n data=8 stop=1";	// Variable to store string containing baud, etc to build DCB with

	try 
	{
		SerialPort = CreateFileA (COMPortName,	// Set COM port to connect to
				GENERIC_READ | GENERIC_WRITE,	// Open read and write
				0,								
				NULL,
				OPEN_EXISTING,					// Opens existing COM port
				/*FILE_FLAG_OVERLAPPED*/0,		// Use FILE_FLAG_OVERLAPPED for asynchronous or 0 for synchronous
				NULL);
		
		if(SerialPort == INVALID_HANDLE_VALUE) throw 'a';						// throw exception if handle is invalid	
		if (!GetCommState(SerialPort, &Port)) throw 'c';						// Gets information about SerialPort to store in DCB
		if (!BuildCommDCB(CommDCBParameter.c_str(), &Port)) throw 'd';			// Build DCB according to CommDCBParameter variable
		if (!SetCommState(SerialPort, &Port)) throw 'e';						// Attempt to set SerialPort according to build parameters from DCB
		if (!SetCommTimeouts(SerialPort, &PortTimeouts)) throw 'b';				// Attempt to set timeout profile for arduino and throw exception if unsuccessful
	}
	catch(char a)
	{
		switch(a)		// Catch exceptions and use SetError() to store the correct error
		{
			case 'a':
				SetError("INVALID_HANDLE_ERROR");
				break;
			case 'b':
				SetError("SET_COMM_TIMEOUT_ERROR");
				break;
			case 'c':
				SetError("GET_COMM_STATE_ERROR");
				break;
			case 'd':
				SetError("DCB_BUILD_ERROR");
				break;
			case 'e':
				SetError("SET_COMM_STATE_ERROR");
				break;
			default:
				SetError("WINDOWS_ERROR: " + GetLastError()); // Use GetLastError() to retrieve any other errors not defined
				break;
		}
		
		CloseHandle(SerialPort);		// Close handle and exit																			
		return 0;						
	}
	catch (...)
	{
		CloseHandle(SerialPort);		// Close handle and exit																			
		return 0;
	}
	Sleep(500);		// Make a slight pause to allow device to get going
	return 1;
}

std::string ArduinoCOM::GetError()
{
	return LastError;
}

void ArduinoCOM::ClearErrors()
{
	LastError = "";
}

void ArduinoCOM::Read()
{
	
}

bool ArduinoCOM::ReadLine(std::string* Line)
{
	bool EndOfFile = false;	// Create an EOF flag to use in this function
	FlushBuffer(); // Clear buffer before we start reading
	DWORD bytesRead; // ReadFile requires a pointer to a DWORD to output bytes read. This will be set back to 0 if EOF is reached 

	try		
	{
		for (int i = 0; !EndOfFile&& i<sizeof(buffer); i++) // Set up loop to read until \0 is found
		{
			if (!ReadFile(SerialPort, &buffer[i], 1, &bytesRead, /*&PortOverlap*/NULL)) // Read one byte at a time from arduino and store in buffer, ReadFile() returns 0 if there is a failure
			{
				COMError = GetLastError();
				i--;
				switch (COMError)
				{
				case ERROR_IO_PENDING:		// We expect to occasionally have IO pending, so ignore and take a pause
					//Sleep(50);
					break;
				case ERROR_HANDLE_EOF:		// We expect to occasionally read to the end of the buffer, so ignore and take a pause
					//Sleep(50);
					break;
				default:					// For all other errors, we should throw an exception
					throw COMError;
					break;

				}

			}

			if (buffer[i] == '\n' && i > 0)
			{
				EndOfFile = true; // Set EOF flag if terminating character is reached
				//std::cout << buffer;		// Used to debug buffer errors
				//if (buffer[0] != '\0')
				{
					//for (int j = 0; j <= i; j++)
					{
						//*Line += buffer[j];
						*Line = std::string(buffer);
					}
					
					FlushBuffer();	// Send the buffer to be set back to \0 in all positions
				}
			}
		}
		
		EndOfFile = false;
	}
	catch (...) //Catch exceptions caused by ReadFile() and exit
	{
		SetError("READ_ERROR " + std::to_string(COMError)); // Output last error and exit 
		CloseHandle(SerialPort);
		return false;
	}
	return true;
}

void ArduinoCOM::Write()
{
	
}

void ArduinoCOM::SetError(std::string NewError)
{
	LastError = NewError;
}

void ArduinoCOM::Disconnect()
{
	CloseHandle(SerialPort);
}

void ArduinoCOM::FlushBuffer()
{
	for (int i = 0; i < sizeof(buffer); i++)
	{
		buffer[i] = '\0';			// Reset all members of buffer to \0
	}

	return;
}