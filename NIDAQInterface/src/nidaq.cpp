#include "nidaq.h"
#include "c_system.h"
#include <cstring>
#include <time.h>

#include <cstdio>

void print(WINDOW* w, const char* str)
{
	if(w)
	{
		wscrl(w, -1);
		mvwaddstr(w, 0, 0, str);
	}
//	else
	{
		printf(str);
		printf("\n");
	}
}

NDAPI TaskHandle GetTask(const char* taskName, WINDOW* w)
{
	int error;
	TaskHandle handle;
	print(w, "Creating task...");
	DAQmxErrChk(DAQmxCreateTask(taskName, &handle));
	return handle;
}

NDAPI void StartTask(TaskHandle handle, DAQmxEveryNSamplesEventCallbackPtr callback, void* callbackData, WINDOW* w)
{
	int error;

	if(callback)
	{
		print(w,"Setting timer...");
		DAQmxErrChkVoid(DAQmxCfgSampClkTiming(handle,"",100.0,DAQmx_Val_Rising,DAQmx_Val_ContSamps,0));
		print(w,"Registering continuous sample callbacks...");
		DAQmxErrChkVoid(DAQmxRegisterEveryNSamplesEvent(handle,DAQmx_Val_Acquired_Into_Buffer,10,0,callback,callbackData));
	}

	print(w,"Starting task...");
	DAQmxErrChkVoid(DAQmxStartTask(handle));
}

NDAPI void GetReadChannel(TaskHandle handle, const char* name, int chan, WINDOW* w)
{
	int error;
	
	char* chanStr = new char[10];
	strcpy(chanStr, "Dev1/ai##");
	if(chan < 10)
	{
		chanStr[7] = '0' + chan; 
		chanStr[8] = '\0';
	}
	else 
	{ 
		chanStr[7] = '0' + (chan / 10); 
		chanStr[8] = '0' + (chan % 10); 
	}
	
	char* output = new char[1024];
	sprintf(output, "Acquiring %s name %s", chanStr, name);
	print(w,output);
	DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, chanStr, name, DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
	delete chanStr;
	delete output;
}

NDAPI void GetWriteChannel(TaskHandle handle, const char* name, int channel, WINDOW* w)
{
	int error;

	char* chanStr = new char[19];
	strcpy(chanStr, "Dev1/port#/line0:7");
	chanStr[9] = '0' + channel;

	char* output = new char[1024];
	sprintf(output, "Acquiring %s name %s", chanStr, name);
	print(w,output);
	DAQmxErrChk(DAQmxCreateDOChan(handle, chanStr, name, DAQmx_Val_ChanForAllLines));
	delete chanStr;
	delete output;
}

NDAPI int Read(TaskHandle handle, float64* data, int sampsPerChan, int numSamps)
{
	int32       error=0;
	int32       read=0;

	DAQmxErrChk(DAQmxReadAnalogF64(handle,sampsPerChan,1.0,DAQmx_Val_GroupByChannel,data,numSamps*sampsPerChan,&read,NULL));
	return read;
}

NDAPI void Pulse(TaskHandle handle, unsigned port, unsigned chan, WINDOW* w)
{
	static time_t lastPulse[16] = {0};
	time_t t;
	time(&t);
	if(difftime(t, lastPulse[port*8 + chan]) < 2)
	{
		print(w, "Pulsing too rapidly.");
		return;
	}
	if(port >= 2 || port < 0)
	{
		print(w,"Invalid port...");
		return;
	}
	if(chan >= 8 || chan < 0)
	{
		print(w,"Invalid Channel...");
		return;
	}
	int error;
	uInt8 data[8] = {0,0,0,0,0,0,0,0};
	data[chan] = 1;
	uInt8 off[8] = {0};
	// Watchdog Timer
	TaskHandle wd;
	{
		char* portStr = new char[14];
		strcpy(portStr, "port#/line0:7");
		portStr[4] = '0' + port;
	//	DAQmxErrChkVoid(DAQmxCreateWatchdogTimerTask("Dev1", "Watchdog", &wd, 40, portStr, 0));
	}

	//print(w, "Creating watchdog task with a timeout of 25ms.  This guarentees pulse completion...");
	//DAQmxErrChkVoid(DAQmxStartTask(wd));
	print(w, "Pulsing 1...");
	printf("Pulsing 1 on port %d line %d", port, chan);
	int32 numSampsWritten;
	DAQmxErrChkVoid(DAQmxWriteDigitalLines(handle, 1,1,10.0,DAQmx_Val_GroupByChannel, data, &numSampsWritten, NULL));
	Sys_Sleep(10);
	print(w, "Pulsing 0...");
	printf("Pulsing 0 on port %d line %d", port, chan);
	DAQmxErrChkVoid(DAQmxWriteDigitalLines(handle, 1,1,10.0,DAQmx_Val_GroupByChannel, off, NULL, NULL));
	//FreeTask(wd, w);
	time(&lastPulse[port*8+chan]);
}

NDAPI void FreeTask(TaskHandle handle, WINDOW* w)
{
	int error;
	if(handle != 0)
	{
		print(w, "Clearing task...");
		DAQmxErrChkVoid(DAQmxClearTask(handle));
	}
}