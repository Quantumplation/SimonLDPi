#pragma once
#include "../include/curses.h"
#include "../NIDAQInterface/src/nidaq.h"
#include "Track.h"
#include "GUITrack.h"
#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

namespace SLDP
{
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
		NIDAQWrapper() : winSwitch(NULL), winSensors(NULL), winLog(NULL), winLogBox(NULL), winTrack(NULL), frozen(false) {}

		void BuildUI();
		void Initialize();

		void GetPhysical(Track* track); // GET PHYSICAL, PHYSICAL
		void MeHearYourBodyTalk(Track* track);

		void GetPhysical(GUITrack* track);
		void MeHearYourBodyTalk(GUITrack* track);

		void PulseSwitch(int Sector, bool Cross);

		void Destroy();
		void refreshConsole()
		{
			for(int x = 0; x < 12; x++)
			{
				mvwprintw(winSensors, x/4+2, (x%4)*5 + 3, values[x] > 4 ? "Y" : "N");
			}
			char open[] = {(char)(192), '\0', (char)(193), '\0', (char)(217), '\0', (char)(218), '\0'};
			mvwprintw(winTrack, 3, 7, open + (values[0]>4)*2);   // A
			mvwprintw(winTrack, 5, 7, open + (values[1]>4)*2);   // B
			mvwprintw(winTrack, 5, 13, open + (values[2]>4)*2);  // C
			mvwprintw(winTrack, 7, 13, open + (values[3]>4)*2);  // D
			mvwprintw(winTrack, 1, 17, open + (values[4]>4)*2);  // E
			mvwprintw(winTrack, 3, 17, open + (values[5]>4)*2);  // F
			mvwprintw(winTrack, 5, 22, open + (values[6]>4)*2);  // G
			mvwprintw(winTrack, 9, 20, open + (values[7]>4)*2);  // H
			mvwprintw(winTrack, 3, 24, open + (values[8]>4)*2);  // I
			mvwprintw(winTrack, 1, 30, open + (values[9]>4)*2);  // J
			mvwprintw(winTrack, 5, 30, open + (values[10]>4)*2);  // K
			mvwprintw(winTrack, 8, 29, open + (values[11]>4)*2 + 4);  // L

			for(int x = 0; x < 8; x++)
			{
				mvwprintw(winSwitch, (x/3)+2, (x%3)*7+3, switches[x] ? "X" : "S");
			}
			char cross[] = {(char)177, '\0', (char)178, '\0', (char)179, '\0', (char)180, '\0', (char)216, '\0', (char)215, '\0'};
			char straight[] = {(char)203, '\0', (char)204, '\0', (char)205, '\0', (char)206, '\0', (char)219, '\0', (char)220, '\0'};
			mvwprintw(winTrack, 3,3, switches[0] ? cross : straight); // 1
			mvwprintw(winTrack, 3,5, switches[0] ? (cross+6) : (straight+6) );
			mvwprintw(winTrack, 5,3, switches[0] ? (cross+4) : (straight+4) );
			mvwprintw(winTrack, 5,5, switches[0] ? (cross+2) : (straight+2) );
			
			mvwprintw(winTrack, 3,18, switches[4] ? cross : straight); // 5
			mvwprintw(winTrack, 3,20, switches[4] ? (cross+6) : (straight+6));
			mvwprintw(winTrack, 5,18, switches[4] ? (cross+4) : (straight+4));
			mvwprintw(winTrack, 5,20, switches[4] ? (cross+2) : (straight+2));
			
			mvwprintw(winTrack, 5,9, switches[1] ? (cross) : (straight)); // 2
			mvwprintw(winTrack, 5,11, switches[1] ? (cross+6) : (straight+6));
			mvwprintw(winTrack, 7,9, switches[1] ? (cross+4) : (straight+4));
			mvwprintw(winTrack, 7,11, switches[1] ? (cross+8) : (straight+8));
			
			mvwprintw(winTrack, 5,24, switches[6] ? (cross) : (straight)); // 7
			mvwprintw(winTrack, 5,26, switches[6] ? (cross+6) : (straight+6));
			mvwprintw(winTrack, 7,24, switches[6] ? (cross+4) : (straight+4));
			mvwprintw(winTrack, 7,26, switches[6] ? (cross+2) : (straight+2));

			mvwprintw(winTrack, 1,13, switches[3] ? (cross) : (straight)); //4
			mvwprintw(winTrack, 7,10, switches[2] ? (cross+10) : (straight+10));//3
			mvwprintw(winTrack, 1,28, switches[7] ? (cross+6) : (straight + 6));//6
			mvwprintw(winTrack, 7,23, switches[5] ? (cross+6) : (straight+6));//8
			wrefresh(winSensors);
			wrefresh(winSwitch);
			wrefresh(winTrack);
			getch();
		}

		void setValue(int index, float64 value)
		{
			values[index] = value;
		}

		~NIDAQWrapper()
		{
			Destroy();
		}

		
		bool frozen;
	private:
		TaskHandle readHandle;
		std::vector<TaskHandle> writeHandles;

		float64 values[12];
		bool switches[8];


		WINDOW* winSwitch;
		WINDOW* winSensors;
		WINDOW* winLog, *winLogBox;
		WINDOW* winTrack;
	};
}