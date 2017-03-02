/*______________________________________________
 |                                              |
 | txplot v1.0 - a simple tty plotting utility  |
 | Copyright (c) 1994-2002  by  Antoni Sawicki  |
 |______________________________________________|
 |'''''''''''''''''''''''''''''''''''''''''''''
 |
 |   /\                
 |  /  \  ,'.      /\  distributed under      ,' 
 | /    \'   `.  ,'. \    ,           /\  __,'_----
 |    ,' \     `'/  `.\ ,' `.  /\__,_/_---,'
 |  ,'    \ GNU /_____`\/\---`.--,' `.  ,\
 | '_____--\---/          \  / `' \/  `'  \
 |  General \ /   Public   \/  License     \
 |           '                              \
 | 
*/

#include <curses.h>
#include <stdlib.h>
#include <math.h>
#define PLOTCHAR 'O'

int draw_scale(int argc, int argm, int xmax, int ymax);
int draw_line(int x1, int y1, int x2, int y2);

int main(int argc, char *argv[]) {
	char *env;
	int n,argmax=0;

	struct position {
		int x;
		int y;
	} pos[argc]; //this is GCC-only extension

	if(argc<3) usage();

	//find y-scale maximum
	env=getenv("TXPLOT_SCALE");
	if(env!=0)
		argmax=atoi(env);
	else
		for(n=1;n<argc;n++)
			if(atoi(argv[n])>argmax) 
				argmax=atoi(argv[n]);

	//init curses screen
	initscr();

	//draw scale
	draw_scale(argc, argmax, COLS, LINES);

	//build position table and rescale to real screen size
	for(n=1; n<argc; n++) {
		pos[n].x=(n*(COLS-1)/argc);
		pos[n].y=LINES-(atoi(argv[n])*(LINES-4)/argmax)-2;
	}

	//draw plot
	for(n=1; n<argc-1; n++) 
		draw_line(pos[n].x, pos[n].y, pos[n+1].x, pos[n+1].y);

	//paint curses screen and quit
	refresh();
	endwin();
	return 0;
}


int draw_scale(int argc, int argm, int xmax, int ymax) {
	int n;

	//y-scale
	for(n=0; n<ymax; n++)
		mvprintw(n, 0, "|");

	//y-1/4
	for(n=0; n<xmax; n++)
		mvprintw(ymax-ymax/4, n, ".");
	mvprintw(ymax-(ymax/4), 0, "%1.1f", (float) argm/4);
	
	//y-half
	for(n=0; n<xmax; n++)
		mvprintw(ymax-ymax/2, n, ".");
	mvprintw(ymax-(ymax/2), 0, "%1.1f", (float) argm/2);

	//y-3/4
	for(n=0; n<xmax; n++)
		mvprintw(ymax-3*ymax/4, n, ".");
	mvprintw(ymax-(3*ymax/4), 0, "%1.1f", (float) 3*argm/4);

	//y-info
	mvprintw(ymax-(ymax-1), 0, "%d", argm);
	mvprintw(0,0, "^");
	mvprintw(ymax-2,0, "0");

	//x-scale
	for(n=1; n<xmax; n++)
		mvprintw(ymax-1, n, "-");

	//x-info
	mvprintw(ymax-1, (xmax/argc), "1");
	mvprintw(ymax-1, ((argc-1)*xmax/argc), "%d", argc-1);
	mvprintw(ymax-1,xmax-1, ">");


	return 0;
}

int draw_line(int x1, int y1, int x2, int y2) {
	float a,x;
	int b,y;

	a=(float) (y2-y1) / (x2-x1); 
	b=y1-a*x1;

	for(x=x1; x<=x2; x=x+0.1) {
		y=a*x+b;
		mvaddch(y,x,PLOTCHAR);
	}

	return 0;
}

int usage(void) {
	printf(
		"Usage: txplot val val val ...\n"\
		"Example: txplot 95 50 95 100 25 50 10 15 12 10 5\n\n"\
		"For fixed scale, set environment vairable TPLOT_SCALE"\
		"to the desired Y-Max.\n"\
	);
	exit(1);
}

