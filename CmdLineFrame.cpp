/*
  CmdLineFrame.cpp

*/
#include "CmdLineFrame.h"

CmdLineFrame::CmdLineFrame(cmdFunc showMenuFunc,
		cmdEntry *availableCmdsPtr,
		uint32_t baud)
{
	_cnt = 0;
	_showMenuFunc = showMenuFunc;
	_availableCmdsPtr = availableCmdsPtr;

	//_uartPtr = uartPtr;
	Serial.begin(baud);

	while(Serial.available() > 0) {
		Serial.read();
	}
}

void CmdLineFrame::parseCmd()
{
	char 			command[CMDLNFR_CMD_LENGTH];
	char 			argument[CMDLNFR_ARGUMENT_LENGTH];
	char    		*token, *tk, *str;
	unsigned char   i, arg = 0u, k = 0u, cmd_found = 0u, funcReturn;
	char    		endOf[] = "endnow";

	str = (char *)_cmdLineBuffer;
	if ((token = strtok(str, " ")) != NULL)
	{
		do {
			tk = token;
			i = 0u;
			while (*tk != '\0')  {
				if (arg == 0) {
					command[i] = *tk;
					i++;
				} else {
					argument[k] = *tk;
					k++;
				}
				tk++;
			}
			if (arg == 0) {
				arg++;
				command[i] = '\0';
			} else {
				argument[k] = ' ';
				k++;
			}
		} while ((token = strtok(NULL, " ")) != NULL);

		argument[k] = '\0';
		if (strcmp((char *)argument, "\0") == 0) {
			command[i-1] = '\0';
		}
	}

	i = 0;
	while (strcmp((char *)endOf, (char *)_availableCmdsPtr[i].cmdPtr) != 0) {
		if (strcmp((char *)command, (char *)_availableCmdsPtr[i].cmdPtr) == 0) {
			funcReturn = (*_availableCmdsPtr[i].functionPtr)(argument);
			if (funcReturn != 0) {
				return;
			}
			cmd_found = 1;
			break;
		}
		i++;
	}

	if (cmd_found == 0) {
		Serial.print("Unknown command (");
		Serial.print(command);
		Serial.println(")");
		Serial.println("");
		(*_showMenuFunc)(NULL);
	}

	Serial.println("");
	Serial.print("arduino> ");
}

bool CmdLineFrame::readCmdLine()
{
	while (Serial.available()) {
		char c = Serial. read();
		Serial. print(c);
		_cmdLineBuffer[_cnt++] = c;
		if ((c == '\r') || (c == '\n') || (_cnt >= sizeof(_cmdLineBuffer)-1)) {
			_cmdLineBuffer[_cnt] = '\0';
			_cnt = 0;
			Serial.println("");
			return true;
		}
	}
	return false;
}

void CmdLineFrame::service()
{
	int byteIn = 0;

	if (readCmdLine()) {
		parseCmd();
	}
}
