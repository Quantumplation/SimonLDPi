#include "../include/curses.h"
#include "../include/panel.h"
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>
#include <Windows.h>

int CALLBACK WinMain( __in  HINSTANCE hInstance, __in  HINSTANCE hPrevInstance, __in  LPSTR lpCmdLine, __in  int nCmdShow )
{
    WINDOW *winSensors, *winLog, *winSwitch, *winTrack;
    chtype save[80], ch;
    int width, height, w, x, y, i, j, seed;


    initscr();
	resize_term(25,66);
		
    start_color();

    cbreak();
    noecho();

    curs_set(0);

    noecho();

    /* refresh stdscr so that reading from it will not cause it to 
       overwrite the other windows that are being created */

    refresh();

    /* Create a drawing window */

    winSensors = newwin(10, 33, 0, 0);
	winLog = newwin(5, 66, 25-5, 0);
	winSwitch = newwin(10,33,0,33);
	winTrack = newwin(10, 40, 10, 15);

    if (winSensors == NULL || winLog == NULL || winSwitch == NULL || winTrack == NULL)
    {
        endwin();
        return 1;
    }
    
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_BLACK, COLOR_RED);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
    wbkgd(winSensors, COLOR_PAIR(1));
	wbkgd(winLog, COLOR_PAIR(2));
	wbkgd(winSwitch, COLOR_PAIR(1));
    werase(winSensors);
	werase(winLog);
	werase(winSwitch);
	werase(winTrack);

	init_pair(4, COLOR_WHITE, COLOR_WHITE);
	init_pair(5, COLOR_BLUE, COLOR_BLUE);
	wattrset(winSensors, COLOR_PAIR(4));
	wattrset(winSwitch, COLOR_PAIR(4));
	box(winSensors, '-', ' ');
	box(winLog, ' ', '-');
	box(winSwitch, '-', ' ');

	init_pair(6, COLOR_RED, COLOR_WHITE);
	wattrset(winSensors, COLOR_PAIR(6));
	wattrset(winSwitch, COLOR_PAIR(6));
	mvwaddstr(winSensors, 0, 1, " Sensors ");
	mvwaddstr(winSwitch, 0, 1, " Switches ");
	mvwaddstr(winLog, 0, 1, " Log:  " );

	init_pair(7, COLOR_WHITE, COLOR_RED);
	wattrset(winLog, COLOR_PAIR(7));
	mvwaddstr(winLog, 1, 1, "Acquiring Read Channel Dev1/ai0 with name Voltage_0...");
	mvwaddstr(winLog, 2, 1, "Setting timer...");
	mvwaddstr(winLog, 3, 1, "Registering continuous sample callbacks...");

	wattrset(winSensors, COLOR_PAIR(1));
	mvwaddstr(winSensors, 2, 2, "A:    -1.0000");	mvwaddstr(winSensors, 2, 18, "B:    -1.0000");
	mvwaddstr(winSensors, 3, 2, "C:    -1.0000");	mvwaddstr(winSensors, 3, 18, "D:    -1.0000");
	mvwaddstr(winSensors, 4, 2, "E:    -1.0000");	mvwaddstr(winSensors, 4, 18, "F:    -1.0000");
	mvwaddstr(winSensors, 5, 2, "G:    -1.0000");	mvwaddstr(winSensors, 5, 18, "H:    -1.0000");
	mvwaddstr(winSensors, 6, 2, "I:    -1.0000");	mvwaddstr(winSensors, 6, 18, "J:    -1.0000");
	mvwaddstr(winSensors, 7, 2, "K:    -1.0000");	mvwaddstr(winSensors, 7, 18, "L:    -1.0000");

	wattrset(winSwitch, COLOR_PAIR(1));
	mvwaddstr(winSwitch, 3, 2, "1:   Cross");	mvwaddstr(winSwitch, 3, 17, "2:   Cross");
	mvwaddstr(winSwitch, 4, 2, "3:   Straight");mvwaddstr(winSwitch, 4, 17, "4:   Cross");
	mvwaddstr(winSwitch, 5, 2, "5:   Cross");	mvwaddstr(winSwitch, 5, 17, "6:   Straight");
	mvwaddstr(winSwitch, 6, 2, "7:   Corss");	mvwaddstr(winSwitch, 6, 17, "8:   Straight");

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
				  (char)216,//SCLOSE:   43
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
	wrefresh(winLog);
	wrefresh(winSwitch);
	wrefresh(winTrack);
	getch();
	endwin();
}