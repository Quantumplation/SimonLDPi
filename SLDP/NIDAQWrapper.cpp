#include "NIDAQWrapper.h"
#include "Edge.h"
#include "Node.h"

namespace SLDP
{
	int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
	{
		int32       read=0;
		float64     data[120] = {0};

		Read(taskHandle, data, 10, 12);

		NIDAQWrapper* wrapper = (NIDAQWrapper*)callbackData;

		for(int x = 0; x < 12; x++)
		{
			float avg = 0;
			for(int y = 0; y < 10; y++)
			{
				avg += data[x*10+y];
			}
			avg /= 10;
			wrapper->setValue(x, avg);
		}
		if(!wrapper->frozen)
			wrapper->refreshConsole();
		return 0;
	}

	void NIDAQWrapper::BuildUI()
	{
		initscr();
		resize_term(20,46);
		
		start_color();

		cbreak();
		noecho();

		curs_set(0);

		noecho();

		/* refresh stdscr so that reading from it will not cause it to 
		   overwrite the other windows that are being created */

		refresh();

		/* Create a drawing window */

		winSwitch = newwin(5,22,10,0);
		winSensors = newwin(5,22,10,24);
		winLogBox = newwin(5, 46, 15, 0);
		winLog = subwin(winLogBox, 3,44,17,1);
		winTrack = newwin(10, 0, 0, 5);
		
		scrollok(winLog, true);

		if (winSensors == NULL || winLogBox == NULL || winLog == NULL || winSwitch == NULL || winTrack == NULL)
		{
			endwin();
			return;
		}
    
		init_pair(1, COLOR_WHITE, COLOR_BLUE);
		init_pair(2, COLOR_WHITE, COLOR_RED);
		wbkgd(winSensors, COLOR_PAIR(1));
		wbkgd(winSwitch, COLOR_PAIR(1));
		wbkgd(winLog, COLOR_PAIR(2));
		wbkgd(winLogBox, COLOR_PAIR(2));
		werase(winSensors);
		werase(winLog);
		werase(winLogBox);
		werase(winSwitch);
		werase(winTrack);

		wattrset(winSensors, COLOR_PAIR(1));
		wattrset(winSwitch, COLOR_PAIR(1));
		wattrset(winSensors, COLOR_PAIR(1));
		wattrset(winSwitch, COLOR_PAIR(1));

		mvwaddstr(winSensors, 0, 1, "Obstacles");
		mvwaddstr(winSwitch, 0, 1, "Sensors");
		mvwaddstr(winLogBox, 0, 1, "Log:" );

		wattrset(winLog, COLOR_PAIR(2));

		wattrset(winSensors, COLOR_PAIR(1));
		mvwaddstr(winSensors, 2, 1, "A:");	mvwaddstr(winSensors, 2, 6, "B:");  mvwaddstr(winSensors, 2, 11, "C:");	mvwaddstr(winSensors, 2, 16, "D:"); 
		mvwaddstr(winSensors, 3, 1, "E:");	mvwaddstr(winSensors, 3, 6, "F:");	mvwaddstr(winSensors, 3, 11, "G:");	mvwaddstr(winSensors, 3, 16, "H:");	
		mvwaddstr(winSensors, 4, 1, "I:");	mvwaddstr(winSensors, 4, 6, "J:");	mvwaddstr(winSensors, 4, 11, "K:");	mvwaddstr(winSensors, 4, 16, "L:");

		wattrset(winSwitch, COLOR_PAIR(1));
		mvwaddstr(winSwitch, 2, 1, "1:");	mvwaddstr(winSwitch, 2, 8, "2:");	mvwaddstr(winSwitch, 2, 15, "3:");
		mvwaddstr(winSwitch, 3, 1, "4:");	mvwaddstr(winSwitch, 3, 8, "5:");	mvwaddstr(winSwitch, 3, 15, "6:");	
		mvwaddstr(winSwitch, 4, 1, "7:");	mvwaddstr(winSwitch, 4, 8, "8:");

		mvwaddstr(winTrack, 0,0, track);
		wrefresh(winSensors);
		wrefresh(winLogBox);
		wrefresh(winLog);
		wrefresh(winSwitch);
		wrefresh(winTrack);
		halfdelay(1);
	}

	void NIDAQWrapper::Initialize()
	{
		BuildUI();
		// Setup our read channel
		readHandle = GetTask("readTask", winLog);
		for(int x = 0; x < 12; x++)
		{
			std::stringstream s;
			std::string name;
			s << "Voltage_" << x;
			s >> name;
			GetReadChannel(readHandle, name.c_str(), x, winLog);
		}
		StartTask(readHandle, EveryNCallback, this, winLog);

		// Setup our write channels
		writeHandles.push_back(TaskHandle());
		writeHandles[0] = GetTask("write0");
		GetWriteChannel(writeHandles[0], "DigitalOut_0", 0, winLog);
		writeHandles.push_back(TaskHandle());
		writeHandles[1] = GetTask("write1");
		GetWriteChannel(writeHandles[1], "DigitalOut_1", 1, winLog);

		StartTask(writeHandles[0], NULL, NULL, winLog);
		StartTask(writeHandles[1], NULL, NULL, winLog);

		wrefresh(winLog);

		return;
	}

	void NIDAQWrapper::GetPhysical(Track* track)
	{		
		std::string labels[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
		std::string nodeSwitch[] = {"One", "Two", "R", "C", "Five", "U", "Seven", "D"};
		for(int x = 0; x < 12; x++)
			track->getFirstEdge(labels[x])->setFlags((values[x] > 4) ? SLDP::EDGE_IMPASSABLE : SLDP::EDGE_NONE);
		for(int x = 0; x < 8; x++)
			track->getFirstNode(nodeSwitch[x])->setCurrentEdge((x == 5 || x == 7) ? LEFT : RIGHT, switches[x] ? 1 : 0);
	}
	void NIDAQWrapper::MeHearYourBodyTalk(Track* track)
	{
		std::string nodeSwitch[] = {"One", "Two", "R", "C", "Five", "U", "Seven", "D"};
		for(int x = 0; x < 8; x++)
			if(track->getFirstNode(nodeSwitch[x])->edgeNotDefault((x == 5 || x == 7) ? LEFT : RIGHT))
				PulseSwitch(x+1, true);
			else
				PulseSwitch(x+1, false);
	}

	void NIDAQWrapper::GetPhysical(GUITrack* track)
	{
		std::string labels[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L"};
		std::string nodeSwitch[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight"};
		std::vector<GUIEdge*> edges = track->getEdges();
		for(int x = 0; x < 12; x++)
		{
			for(size_t y = 0; y < edges.size(); y++)
			{
				if(edges[y]->getLabel() == labels[x] && ((edges[y]->isBlocked() && values[x] < 4) || !edges[y]->isBlocked() && values[x] > 4)) edges[y]->alterBlocked();
			}
		}
		std::vector<GUISwitch*> guiswitches = track->getSwitches();
		for(int x = 0; x < 8; x++)
		{
			if(x == 2 || x == 3 || x == 5 || x == 7)
			{
				if(track->getFirstNode(nodeSwitch[x])->isSwitchableLeft()) track->getFirstNode(nodeSwitch[x])->setSwitchedLeft(switches[x]);
				if(track->getFirstNode(nodeSwitch[x])->isSwitchableRight()) track->getFirstNode(nodeSwitch[x])->setSwitchedRight(switches[x]);
			}
			else
			{
				for(size_t y = 0; y < guiswitches.size(); y++)
				{
					if(guiswitches[y]->getLabel() == nodeSwitch[x]) guiswitches[y]->switchAll(switches[x]);
				}
			}
		}
	}
	void NIDAQWrapper::MeHearYourBodyTalk(GUITrack* track)
	{
		std::string nodeSwitch[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight"};
		std::vector<GUISwitch*> guiswitches = track->getSwitches();
		for(int x = 0; x < 8; x++)
		{
			if(x == 2 || x == 3 || x == 5 || x == 7)
			{
				if(track->getFirstNode(nodeSwitch[x])->isSwitchableLeft()) PulseSwitch(x+1, track->getFirstNode(nodeSwitch[x])->isSwitchedLeft());
				if(track->getFirstNode(nodeSwitch[x])->isSwitchableRight()) PulseSwitch(x+1, track->getFirstNode(nodeSwitch[x])->isSwitchedRight());
			}
			else
			{
				for(size_t y = 0; y < guiswitches.size(); y++)
				{
					if(guiswitches[y]->getLabel() == nodeSwitch[x])
					{
						PulseSwitch(x+1, guiswitches[y]->getNodes()[0]->isSwitchedRight());
					}
				}
			}
		}
	}



	void NIDAQWrapper::PulseSwitch(int Sector, bool Cross)
	{
		if(Sector < 0 || Sector > 8)
			return;
		if(Sector == 7 || Sector == 8)
		{
			// Cludgey workaround for their labelling
			Sector = (Sector == 7) ? 8 : 7;
		}
		int port = Sector > 4;
		int channel = ((Sector-1)%4)*2 + Cross;
		Pulse(writeHandles[port], port, channel, winLog);
		if(Sector == 7 || Sector == 8)
		{
			//Cludgey workaround for their labelling
			Sector = (Sector == 7) ? 8 : 7;
		}
		switches[Sector-1] = Cross;
	}

	void NIDAQWrapper::Destroy()
	{
		FreeTask(readHandle, winLog);
		FreeTask(writeHandles[0], winLog);
		FreeTask(writeHandles[1], winLog);
	}
}