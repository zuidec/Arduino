///
///	ArduinoCOM is a class used to connect to Arduino and Read/Write data over the serial port
///

#include <string>
#include <windows.h>

class ArduinoCOM
{
	public:
		ArduinoCOM();
		~ArduinoCOM();

		void Disconnect();								// Basic functionality
		int Connect(char* COMPortName, int BaudRate);	// Basic functionality
		std::string GetError();							// Basic functionality
		void ClearErrors();								// Basic functionality
		void Read();									// Not implemented yet
		bool ReadLine(std::string *Line);				// Basic functionality
		void Write();									// Not implemented yet
		bool WriteLine(std::string *Line);				// Not implemented yet
		void isConnected();								// Not implemented yet
		
	private:
		DWORD COMError;									// Variable to store the results of the windows GetLastError() as needed
		DWORD bytesRead;								// ReadFile requires a pointer to a DWORD to output bytes read. This will be set back to 0 if EOF is reached 
		DWORD bytesWritten;								// WriteFile requires this to output bytes written
		DWORD bytesToWrite;								// Tells WriteFile() how many bytes to write
		HANDLE SerialPort;								// The handle used to access the serial port
		DCB Port;										// Holds parameters used to set up serial port handle
		OVERLAPPED PortOverlap;							// Needed to use asynchronously with ReadFile, not currently used
		COMMTIMEOUTS PortTimeouts;						// Holds all the timeout information for the handle
		std::string LastError;							// Class specific variable to store class defined errors
		std::string CommDCBParameter;					// Contains required data to set up DCB Port, including baud rate
		char ReadBuffer[128];							// Buffer to hold incoming read data
		char WriteBuffer[128];							// Buffer to hold outgoing write data

		void SetError(std::string NewError);			// Basic functionality
		void Initialize();								// Called from Connect() to set up timeouts, overlaps, and DCB
		void FlushBuffer(char* buffer, int bufferSize);	// Basic functionality
};