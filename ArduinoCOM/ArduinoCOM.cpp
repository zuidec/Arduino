#include <ArduinoCOM.h>


ArduinoCOM::ArduinoCOM()
{
	
}
	
int ArduinoCOM::Initialize()
{
	// When using asynchronous IO, ReadFile() requires the last argument to be a point to an overlapped object, which is declared here. We are required to set hEvent, Offset, and OffsetHigh, but there are more properties
	PortOverlap.hEvent = NULL;		//
	PortOverlap.Offset = 0;			//
	PortOverlap.OffsetHigh = 0;		//

	// Create a timeout profile with the following parameters
	PortTimeouts.ReadIntervalTimeout = 1;			//
	PortTimeouts.ReadTotalTimeoutConstant = 1;		//
	PortTimeouts.ReadTotalTimeoutMultiplier = 1;	//
	PortTimeouts.WriteTotalTimeoutConstant = 1;		//
	PortTimeouts.WriteTotalTimeoutMultiplier = 1;	//
	
	// Set required properties of DCB object
	memset(&Port, 0, sizeof(Port));	
	Port.DCBlength = sizeof(Port);
}

int ArduinoCOM::Connect(char* COMPortName, int BaudRate)
{
	Initialize();
	
	try 
	{
		SerialPort = CreateFileA (COMPortName,	// Set COM port to connect to
				GENERIC_READ | GENERIC_WRITE,	// Open read and write
				0,								
				NULL,
				OPEN_EXISTING,					// Opens existing COM port
				FILE_FLAG_OVERLAPPED,			// Arduino uses FILE_FLAG_OVERLAPPED 
				NULL);
				
		if(arduino == INVALID_HANDLE_VALUE) throw 'a'; // throw exception if handle is invalid
		if (!GetCommState(SerialPort, &Port)) throw 'c';
		if (!BuildCommDCB("baud=" + BaudRate + " parity=n data=8 stop=1", &Port)) throw 'd'; // Setup DBC port correctly
		if (!SetCommState(SerialPort, &Port)) throw 'e';
		if (!SetCommTimeouts(SerialPort, &PortTimeouts)) throw 'b'; // Attempt to set timeout profile for arduino and throw exception if unsuccessful
	}
	catch(char a)
	{
		switch(a)
		{
			case a:
				SetError("INVALID_HANDLE_ERROR");
				break;
			case b:
				SetError("SET_COMM_TIMEOUT_ERROR");
				break;
			case c:
				SetError("GET_COMM_STATE_ERROR");
				break;
			case d:
				SetError("DCB_BUILD_ERROR");
				break;
			case e:
				SetError("SET_COMM_STATE_ERROR");
				break;
			default:
				SetError("WINDOWS_ERROR: " + GetLastError());
				break;
		}
		
		CloseHandle(SerialPort);		// Close handle and exit																			
		return 0;	
	}
	
	Sleep(500);
}

std::string ArduinoCOM::GetError()
{
	
}

void ArduinoCOM::ClearErrors()
{
	
}

void ArduinoCOM::Read()
{
	
}

void ArduinoCOM::Write()
{
	
}

void ArduinoCOM::SetError()
{
	
}

void ArduinoCOM::Disconnect()
{
	
}