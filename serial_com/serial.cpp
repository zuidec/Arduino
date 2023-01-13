#include <stdio.h>
#include <bios.h>

void main()
{
	char i = 0, title[] = "Hello world";
	unsigned status;

	status = _bios_serialcom(_COM_INIT, 0, _COM_9600 | _COM_CHR8 | _COM_STOP1 | _COM_NOPARITY);

	if(status &0x100)
		while (title[i])
		{
			_bios_serialcom(_COM_SEND, 0, title[i]);
			putchar(title[i]);
			i++;

		}

}
