#include "../include/curses.h"
#include "../include/panel.h"
#include <stdio.h>
#include <signal.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <Windows.h>

int CALLBACK WinMain( __in  HINSTANCE hInstance, __in  HINSTANCE hPrevInstance, __in  LPSTR lpCmdLine, __in  int nCmdShow )
{
    WINDOW *winSensors, *winLogBox, *winLog, *winSwitch, *winTrack;

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
		return 1;
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

	mvwaddstr(winSensors, 0, 1, "Sensors");
	mvwaddstr(winSwitch, 0, 1, "Switches");
	mvwaddstr(winLogBox, 0, 1, "Log:" );

	wattrset(winLog, COLOR_PAIR(2));
	mvwaddstr(winLog, 2, 0, "Acquiring chan Dev1/ai0 named Voltage_0...");
	wscrl(winLog, 1);
	mvwaddstr(winLog, 2, 0, "Setting timer...");
	wscrl(winLog, 1);
	mvwaddstr(winLog, 2, 0, "Registering continuous sample callbacks...");	
	wscrl(winLog, 1);
	mvwaddstr(winLog, 2, 0, "Doing stuff.");

	wattrset(winSensors, COLOR_PAIR(1));
	mvwaddstr(winSensors, 2, 1, "A:");	mvwaddstr(winSensors, 2, 6, "B:");  mvwaddstr(winSensors, 2, 11, "C:");	mvwaddstr(winSensors, 2, 16, "D:"); 
	mvwaddstr(winSensors, 3, 1, "E:");	mvwaddstr(winSensors, 3, 6, "F:");	mvwaddstr(winSensors, 3, 11, "G:");	mvwaddstr(winSensors, 3, 16, "H:");	
	mvwaddstr(winSensors, 4, 1, "I:");	mvwaddstr(winSensors, 4, 6, "J:");	mvwaddstr(winSensors, 4, 11, "K:");	mvwaddstr(winSensors, 4, 16, "L:");

	wattrset(winSwitch, COLOR_PAIR(1));
	mvwaddstr(winSwitch, 2, 1, "1:");	mvwaddstr(winSwitch, 2, 8, "2:");	mvwaddstr(winSwitch, 2, 15, "3:");
	mvwaddstr(winSwitch, 3, 1, "4:");	mvwaddstr(winSwitch, 3, 8, "5:");	mvwaddstr(winSwitch, 3, 15, "6:");	
	mvwaddstr(winSwitch, 4, 1, "7:");	mvwaddstr(winSwitch, 4, 8, "8:");

	char t[] ={
				  (char)32,//EMPTY	:	0
				  (char)176,//SSS	:	1
			      (char)177,//XDF	:	2
			      (char)178,//XUR	:	3
			      (char)179,//XUF	:	4
			      (char)180,//XDR	:	5
			      (char)181,//UF	:	6
			      (char)182,//DF	:	7
			      (char)183,//X		:	8
			      (char)184,//DF	:	9
			      (char)185,//UR	:	10
			      (char)186,//UF	:	11
			      (char)187,//DR	:	12
			      (char)188,//TLC	:	13
			      (char)189,//BLC	:	14
			      (char)190,//BRC	:	15
			      (char)191,//TRC	:	16
			      (char)192,//OPEN	:	17
			      (char)193,//CLOSED:	18
			      (char)194,//TC	:	19
			      (char)195,//RC	:	20
			      (char)196,//BC	:	21
			      (char)197,//LC	:	22
			      (char)198,//TRCO	:	23
			      (char)199,//BRCO	:	24
			      (char)200,//BLCO	:	25
			      (char)201,//TLCO	:	26
			      (char)202,//ALLC	:	27
			      (char)203,//SDF	:	28
			      (char)204,//SUR	:	29
			      (char)205,//SUF	:	30
			      (char)206,//SDR	:	31
				  (char)207,//STLC	:	32
				  (char)208,//SBLC	:	33
				  (char)209,//SBRC	:	34
				  (char)210,//STRC	:	35
				  (char)211,//SBC   :   36
				  (char)212,//SLC   :   37
				  (char)213,//STC   :   38
				  (char)214,//SRC   :   39
				  (char)215,//DFTC  :   40
				  (char)216,//URBRC :   41
				  (char)217,//SLOPEN:   42
				  (char)218,//SCLOSE:   43
			     };

	char trackSymbolsTwo[] = "░▒▓│┤╡╢╖╕╣║╗╝╜╛┐└┴┬├─┼╞╟╚╔╩╦╠═╬";

	char track[] = {
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

	mvwaddstr(winTrack, 0,0, track);
	wrefresh(winSensors);
	wrefresh(winLogBox);
	wrefresh(winLog);
	wrefresh(winSwitch);
	wrefresh(winTrack);
	int index = 0;
	std::stringstream ss;
	while(getch())
	{
		ss << index++;
		wscrl(winLog, 1);
		mvwaddstr(winLog, 2, 0, ss.str().c_str());
		ss = std::stringstream("");
		wrefresh(winLog);
	}
	endwin();
}