/*
  CmdLineFrame.h
  .
*/
#ifndef CmdLineFrame_h
#define CmdLineFrame_h

#include "Arduino.h"

#if !defined(CMDLNFR_ARGUMENT_LENGTH)
	#define CMDLNFR_ARGUMENT_LENGTH     70
#endif

#if !defined(CMDLNFR_CMD_LENGTH)
	#define CMDLNFR_CMD_LENGTH         10
#endif

#define CMDLNFR_LENGTH    (CMDLNFR_CMD_LENGTH + CMDLNFR_ARGUMENT_LENGTH)


typedef unsigned char (*cmdFunc) (char *argu);

typedef struct
{
    char	cmdPtr[CMDLNFR_CMD_LENGTH];
    cmdFunc	functionPtr;
} cmdEntry;

class CmdLineFrame
{
  public:
	CmdLineFrame(cmdFunc showMenuFunc,
			cmdEntry *availableCmdsPtr,
			uint32_t baud);
	void service();

  private:
	char _cmdLineBuffer[CMDLNFR_LENGTH];
	int _cnt;
	cmdFunc _showMenuFunc;
	cmdEntry *_availableCmdsPtr;
	//HardwareSerial *_uartPtr;

    void parseCmd();
    bool readCmdLine();
};

#endif
