#pragma once

#include "../include/NIDAQmx.h"
#include "Track.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#define DAQmxErrChk(functionCall) if( DAQmxFailed(error=(functionCall)) ){std::cout << "Error: " << error << std::endl;	char errBuff[2048] = {'\0'}; DAQmxGetExtendedErrorInfo(errBuff, 2048); std::cout << "Extended err: " << errBuff << std::endl; return;}

namespace SLDP
{
	class NIDAQWrapper
	{
	public:
		NIDAQWrapper() {}

		void Initialize()
		{
			int error = 0;
			std::cout << "A" << std::endl;
			std::stringstream s;
			for(int x = 0; x < 8; x++)
			{
				s.clear();
				readHandles.push_back(TaskHandle());
				s << "read" << x;
				DAQmxErrChk(DAQmxCreateTask(s.str().c_str(), &readHandles.back()));
				s.clear();
				s << "Dev1/ai/" << x;
				DAQmxErrChk(DAQmxCreateAIVoltageChan(readHandles.back(), s.str().c_str(), "Voltage_0", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				DAQmxErrChk(DAQmxCfgSampClkTiming(readHandles.back(), "", 1000.0, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 100));
				DAQmxErrChk(DAQmxStartTask(readHandles.back()));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai1", "Voltage_1", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai2", "Voltage_2", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai3", "Voltage_3", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai4", "Voltage_4", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai5", "Voltage_5", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai6", "Voltage_6", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai7", "Voltage_7", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai8", "Voltage_8", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai9", "Voltage_9", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai10", "Voltage_10", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
				//DAQmxErrChk(DAQmxCreateAIVoltageChan(handle, "Dev1/ai11", "Voltage_11", DAQmx_Val_Cfg_Default, -10, 10, DAQmx_Val_Volts, NULL));
			}

			for(int x = 0; x < 2; x++)
			{
				s.clear();
				s << "write" << x;
				writeHandles.push_back(TaskHandle());
				DAQmxErrChk(DAQmxCreateTask(s.str().c_str(), &writeHandles.back()));
				s.clear();
				s << "Dev1/port" << x << "/line0:7";
				DAQmxErrChk(DAQmxCreateDOChan(writeHandles.back(), s.str().c_str(), "", DAQmx_Val_ChanForAllLines));
				DAQmxErrChk(DAQmxCfgSampClkTiming(readHandles.back(), "", 1000.0, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, 100));
				DAQmxErrChk(DAQmxStartTask(writeHandles.back()));
			}
			
			if(error == 0)
				std::cout << "Success!" << std::endl;
			else
			{
				char errBuff[2048] = {'\0'};
				DAQmxGetExtendedErrorInfo(errBuff, 2048);
				std::cout << "Extended err: " << errBuff << std::endl;
			}
			return;
		}

		void ReadTo(Track* track)
		{
		}

		void WriteFrom(Track* track)
		{
		}

		void DisplayValues()
		{
			int error = 0;
			int32 numRead = 0;
			float64 readArray[11000] = {0};
			while(true)
			{
				DAQmxErrChk(DAQmxReadAnalogF64(readHandles[0], DAQmx_Val_Auto, 5, DAQmx_Val_GroupByChannel, readArray, 11000, &numRead, NULL));
				std::cout << "CHAN 1:" << readArray[0];
				std::string test;
				std::cin >> test;
				if(test == "e")
					break;
			}
		}

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
			int error = 0;
			uInt8 data[8] = {0};
			data[Sector%4+Cross] = 0;
			uInt8 off[8] = {0};
			int port = Sector > 4;
			DAQmxErrChk(DAQmxWriteDigitalLines(writeHandles[port], 1,1,10.0,DAQmx_Val_GroupByChannel, data, NULL, NULL));
			Sleep(25);
			DAQmxErrChk(DAQmxWriteDigitalLines(writeHandles[port], 1,1,10.0,DAQmx_Val_GroupByChannel,off, NULL, NULL));
		}

		void Destroy()
		{
			while(true)
			{
				int error;
				for(int x = 0; x < readHandles.size(); x++)
				{
					if(readHandles[x] != 0)
						DAQmxErrChk(DAQmxStopTask(readHandles[x]));
				}
				break;
			}
		}
	private:
		std::vector<TaskHandle> readHandles;
		std::vector<TaskHandle> writeHandles;
	};
}