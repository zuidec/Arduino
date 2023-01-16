///
///	ArduinoCOM is a class used to connect to Arduino and Read/Write data over the serial port
///

#include <string>
#include <windows.h>

class ArduinoCOM
{
	public:
		ArduinoCOM();
		void Disconnect();
		int Connect(char* COMPortName, int BaudRate);
		std::string GetError();
		void ClearErrors();
		void Read();
		void Write();
		
		
	private:
		DWORD COMError;
		HANDLE SerialPort;
		DCB Port;
		OVERLAPPED PortOverlap;
		COMMTIMEOUTS PortTimeouts;
		std::string LastError;
		void SetError(std::string ArduinoError);
		int Initialize();
};