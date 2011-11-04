#pragma once

#include "../../include/NIDAQmx.h"
#include "../../include/curses.h"

#ifdef NIDAQINTERFACE_EXPORTS
#define NDAPI __declspec(dllexport)
#else
#define NDAPI __declspec(dllimport)
#endif

#define DAQmxErrChkVoid(functionCall) if( DAQmxFailed(error=(functionCall)) ){printf("Error: %d", error); char errBuff[2048] = {'\0'}; DAQmxGetExtendedErrorInfo(errBuff, 2048); printf("Extended err:%s", errBuff); return;}
#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ){printf("Error: %d", error); char errBuff[2048] = {'\0'}; DAQmxGetExtendedErrorInfo(errBuff, 2048); printf("Extended err:%s", errBuff);}

extern NDAPI TaskHandle GetTask(const char* taskName, WINDOW* window = NULL);
extern NDAPI void StartTask(TaskHandle task, DAQmxEveryNSamplesEventCallbackPtr callback = NULL, WINDOW* window = NULL);
extern NDAPI void FreeTask(TaskHandle handle, WINDOW* window = NULL);
extern NDAPI void GetReadChannel(TaskHandle handle, const char* name, int channel, WINDOW* window = NULL);
extern NDAPI void GetWriteChannel(TaskHandle handle, const char* name, int channel, WINDOW* window = NULL);
extern NDAPI void Read(float64* data, int count);
extern NDAPI void Pulse(TaskHandle handle, unsigned port, unsigned chan, WINDOW* window = NULL);