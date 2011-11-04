#pragma once
#include "../include/curses.h"
#include "../NIDAQInterface/src/nidaq.h"
#include "Track.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

namespace SLDP
{

	int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
	{
		int32       error=0;
		char        errBuff[2048]={'\0'};
		static int  totalRead=0;
		int32       read=0;
		float64     data[120];

		/*********************************************/
		// DAQmx Read Code
		/*********************************************/
		if(DAQmxFailed(error = DAQmxReadAnalogF64(taskHandle,10,1.0,DAQmx_Val_GroupByChannel,data,nSamples*12,&read,NULL)))
		{
			std::cout << "Error: " << error << std::endl;	
			char errBuff[2048] = {'\0'}; 
			DAQmxGetExtendedErrorInfo(errBuff, 2048); 
			std::cout << "Extended err: " << errBuff << std::endl; 
			return -1;
		}
		if( read>0 ) 
		{
	/*		system("cls");
			printf("+-----------------------------------------------------------------------+\n"
				   "|A:\t%f\tB:\t%f\tC:\t%f\t|\n"
				   "|D:\t%f\tE:\t%f\tF:\t%f\t|\n"
				   "|G:\t%f\tH:\t%f\tI:\t%f\t|\n"
				   "|J:\t%f\tK:\t%f\tL:\t%f\t|\n"
				   "+-----------------------------------------------------------------------+\n",
				   data[0], data[10], data[20],
				   data[30], data[40], data[50],
				   data[60], data[70], data[80],
				   data[90], data[100],data[110]);
			fflush(stdout);
		*/}
		return 0;
	}

	int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
	{
		return 0;
	}

	const char track[] = {
		' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','4',' ',' ','E',' ',' ',' ',' ',' ',' ',' ',' ',' ','8',' ',' ','J','\n',
		' ',' ',' ',' ',' ',' ',' ',' ',' ',(char)190,(char)187,(char)176,(char)176,(char)177,(char)208,(char)176,(char)176,(char)193,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)209,(char)180,(char)176,(char)192,(char)176,'R','1','\n',
		' ',' ',' ',' ','1',' ',' ','A',(char)190,(char)181,(char)188,' ',' ',(char)191,(char)182,(char)189,' ','F',' ','5',' ',' ',' ',' ','I',' ',(char)190,(char)181,(char)188,'\n',
		'L','1', (char)176, (char)177,(char)213,(char)180,(char)176,(char)192,(char)186,(char)188,' ',' ',' ',' ',(char)191,(char)185,(char)176,(char)192,(char)177,(char)213,(char)180,(char)176,(char)176,(char)176,(char)192,(char)176,(char)186,(char)188,'\n',
		' ',' ',' ',(char)195,(char)183,(char)197,' ','B',' ',' ','2',' ',' ','C',' ',' ',' ',' ',(char)195,(char)183,(char)197,' ','G',' ',' ','7',' ',' ',' ',' ','K','\n',
		'L','2',(char)176,(char)179,(char)211,(char)178,(char)176,(char)193,(char)176,(char)177,(char)213,(char)180,(char)176,(char)192,(char)176,(char)176,(char)176,(char)176,(char)179,(char)211,(char)178,(char)176,(char)192,(char)176,(char)177,(char)213,(char)180,(char)176,(char)176,(char)176,(char)192,(char)176,'R', '2','\n',
		' ',' ',' ',' ',' ',' ',' ',' ',' ',(char)195,(char)183,(char)197,' ','D',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',(char)195,(char)183,(char)197,'\n',
		' ',' ',' ',' ',(char)190,(char)187,(char)176,(char)176,(char)176,(char)179,(char)215,(char)216,(char)176,(char)192,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)209,(char)180,(char)179,(char)211,(char)178,(char)176,(char)184,(char)189,'L','\n',
		' ',' ',' ',(char)190,(char)181,(char)188,' ',' ',' ','3',(char)191,(char)182,(char)189,' ',' ',' ',' ',' ',' ',' ','H',(char)190,(char)181,(char)188,'6',' ',' ',' ',(char)191,(char)217,(char)189,'\n',
		'L','3',(char)176,(char)186,(char)188,' ',' ',' ',' ',' ',' ',(char)191,(char)185,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)176,(char)192,(char)186,(char)188,' ',' ',' ',' ',' ',' ',(char)191,(char)185,(char)176,'R','3','\n',
		'\0'
	};

	class NIDAQWrapper
	{
	public:
		NIDAQWrapper() {}

		void Initialize()
		{
			initscr();

			// Setup our read channel
			readHandle = GetTask("readTask");
			for(int x = 0; x < 12; x++)
			{
				std::stringstream s;
				std::string name;
				s << "Voltage_" << x;
				s >> name;
				GetReadChannel(readHandle, name.c_str(), x);
			}

			//StartTask(readHandle, EveryNCallback);

			// Setup our write channels
			writeHandles.push_back(TaskHandle());
			writeHandles[0] = GetTask("write0");
			GetWriteChannel(writeHandles[0], "DigitalOut_0", 0);
			writeHandles.push_back(TaskHandle());
			writeHandles[1] = GetTask("write1");
			GetWriteChannel(writeHandles[1], "DigitalOut_1", 1);

			StartTask(writeHandles[0]);
			StartTask(writeHandles[1]);

			return;
		}

		void ReadTo(Track* track)
		{
		}

		void WriteFrom(Track* track)
		{
		}

/*		void DisplayValues()
		{
			int error = 0;
			int32 numRead = 0;
			float64 readArray[11000] = {0};
			while(true)
			{
				DAQmxErrChk(DAQmxReadAnalogF64(readHandle, DAQmx_Val_Auto, 5, DAQmx_Val_GroupByChannel, readArray, 11000, &numRead, NULL));
				std::cout << "CHAN 1:" << readArray[0] << " read: " << numRead;
				std::string test;
				std::cin >> test;
				if(test == "e")
					break;
			}
		}
		*/
		void WriteSwitches()
		{
/*			int error=0;
			uInt8 data[16]={0,0,0,0,0,0,0,0,
							0,0,0,0,0,0,0,0};
			for(int x = 0; x < 8; x++)
			{
				DAQmxErrChk(DAQmxWriteDigitalLines(handle, 1,1,10.0,DAQmx_Val_GroupByChannel,data, NULL, NULL));
				data[x] = 1;
				std::string wait;
				std::cin >> wait;
			}
			if(error == 0)
				std::cout << "Success!" << std::endl;
			else
			{
				char errBuff[2048] = {'\0'};
				DAQmxGetExtendedErrorInfo(errBuff, 2048);
				std::cout << errBuff << std::endl;
			}*/
		}

		void PulseSwitch(int Sector, bool Cross)
		{
			if(Sector < 0 || Sector > 8)
				return;
			int port = Sector > 4;
			int channel = ((Sector-1)%4)*2 + Cross;
			Pulse(writeHandles[port], port, channel);
		}

		void Destroy()
		{
			FreeTask(readHandle);
			FreeTask(writeHandles[0]);
			FreeTask(writeHandles[1]);
		}

		void printConsole()
		{
			
		}

		~NIDAQWrapper()
		{
			Destroy();
		}
	private:
		TaskHandle readHandle;
		std::vector<TaskHandle> writeHandles;

		int values[12];

		WINDOW* wSwitches;
		WINDOW* wSensors;
		WINDOW* wLog;
		WINDOW* wTrack;
	};
}