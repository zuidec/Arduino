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
		void Disconnect();
		int Connect(char* COMPortName, int BaudRate);
		std::string GetError();
		void ClearErrors();
		void Read();
		bool ReadLine(std::string *Line);
		void Write();
		void FlushBuffer();
		//need to implement an isConnected()
		
	private:
		DWORD COMError;
		HANDLE SerialPort;
		DCB Port;
		// OVERLAPPED PortOverlap; // Needed to use asynchronously with ReadFile
		COMMTIMEOUTS PortTimeouts;
		std::string LastError;
		void SetError(std::string NewError);
		void Initialize();
		std::string CommDCBParameter;
		char buffer[64];
};