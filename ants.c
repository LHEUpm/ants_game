#include "ants.h"

void ncurses_init() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	refresh();
	curs_set(0);	
	nodelay(stdscr, TRUE);
	mousemask(BUTTON1_CLICKED, NULL);
}

void ncurses_stop() {
	endwin();
}

void simulation_init() {
	int i, j;
	
	nb_ants = 0;
	pthread_mutex_init(&mutex_nb_ants, NULL);

	for (i = 0; i < NB_LINES_SIM; i++) {
		for (j = 0; j < NB_COL_SIM; j++) {
			map[i][j].element = EMPTY;
			map[i][j].ant = NULL;
			pthread_mutex_init(&map[i][j].mutex, NULL);
		}
	}
}

WINDOW *create_window_box_sim() {

	WINDOW *win_box_sim;
	
	win_box_sim = newwin(NB_LINES_SIM + 2, NB_COL_SIM + 2, 0, 0);
	box(win_box_sim, 0, 0);
	mvwprintw(win_box_sim, 0, (NB_COL_SIM + 2) / 2 - 5, "SIMULATION");	
	wrefresh(win_box_sim);
	
	return win_box_sim;
}

WINDOW *create_window_sim() {

	WINDOW *win_sim;
	
	win_sim = newwin(NB_LINES_SIM, NB_COL_SIM, 1, 1);
	
	return win_sim;
}

WINDOW *create_window_box_msg() {

	WINDOW *win_box_msg;
	
	win_box_msg = newwin(NB_LINES_MSG + 2, NB_COL_MSG + 2, 0, NB_COL_SIM + 2);
	box(win_box_msg, 0, 0);
	mvwprintw(win_box_msg, 0, (NB_COL_MSG + 2) / 2 - 4, "MESSAGES");
	wrefresh(win_box_msg);
	
	return win_box_msg;
}

WINDOW *create_window_msg() {

	WINDOW *win_msg;
	
	win_msg = newwin(NB_LINES_MSG, NB_COL_MSG, 1, NB_COL_SIM + 3);
	scrollok(win_msg, TRUE);
	
	return win_msg;
}

WINDOW *create_window_box_tools() {

	WINDOW *win_box_tools;
	
	win_box_tools = newwin(NB_LINES_TOOLS + 2, NB_COL_TOOLS + 2, NB_LINES_MSG + 2 , NB_COL_SIM + 2);
	box(win_box_tools, 0, 0);
	mvwprintw(win_box_tools, 0, (NB_COL_TOOLS + 2) / 2 - 3, "TOOLS");
	wrefresh(win_box_tools);
	
	return win_box_tools;
}

WINDOW *create_window_tools() {

	WINDOW *win_tools;
	
	win_tools = newwin(NB_LINES_TOOLS, NB_COL_TOOLS, NB_LINES_MSG + 3, NB_COL_SIM + 3);
	mvwprintw(win_tools, 0, 3, "# : Obstacle\n");
	mvwprintw(win_tools, 1, 3, "@ : Ant");
	mvwprintw(win_tools, 0, 1, "X");
	wrefresh(win_tools);
	
	return win_tools;
}

void *procedure_ant(void *arg) {
	coord_t *coord = (coord_t *) arg;
	int random;
	wprintw(win_msg, "Number of ants : %d\n", nb_ants);
	wrefresh(win_msg);
	wprintw(win_msg, "Ant was created at the position %d %d\n", coord->y, coord->x);
	wrefresh(win_msg);
	while (1) {	
		random = rand() % 4;	
        switch(random){
            case 0:
                if(coord->y - 1 >= 0 && map[coord->y - 1][coord->x].element == EMPTY){
                    map[coord->y + 1][coord->x].element = ANT;
					map[coord->y][coord->x].element = EMPTY;
					mvwprintw(win_sim,coord->y,coord->x," ");
					wrefresh(win_sim);
					mvwprintw(win_sim,coord->y - 1,coord->x,"@");
					coord->y = coord->y - 1;
					wrefresh(win_sim);
                }
                break;
            case 1:
                if(coord->y + 1 < 40 && map[coord->y + 1][coord->x].element == EMPTY ){
                	map[coord->y - 1][coord->x].element = ANT;
					map[coord->y][coord->x].element = EMPTY;
					mvwprintw(win_sim,coord->y,coord->x," ");
					wrefresh(win_sim);
					mvwprintw(win_sim,coord->y + 1,coord->x,"@");
					coord->y = coord->y + 1;
					wrefresh(win_sim);
                }
                break;
            case 2:
                if(coord->x - 1 >= 0 && map[coord->y][coord->x - 1].element == EMPTY){
                    map[coord->y][coord->x -1].element = ANT;
					map[coord->y][coord->x].element = EMPTY;
					mvwprintw(win_sim,coord->y,coord->x," ");
					wrefresh(win_sim);
					mvwprintw(win_sim,coord->y,coord->x-1,"@");
					coord->x = coord->x-1;
					wrefresh(win_sim);
                }
                break;
            case 3:
                if(coord->x + 1 < 80 && map[coord->y][coord->x + 1].element == EMPTY){
                    map[coord->y][coord->x+1].element = ANT;
					map[coord->y][coord->x].element = EMPTY;
					mvwprintw(win_sim,coord->y,coord->x," ");
					wrefresh(win_sim);
					mvwprintw(win_sim,coord->y,coord->x+1,"@");
					coord->x = coord->x+1;
					wrefresh(win_sim);
                }
                break;	
        }
        sleep(1);
    }
	free(coord);
	return NULL;
}


int main(int argc, char *argv[]) {
	WINDOW *win_box_sim, *win_box_msg, *win_box_tools, *win_tools;
	MEVENT event;
	int ch, current_item = OBSTACLE;
	coord_t *coord;

	srand(time(NULL));

	ncurses_init();
	simulation_init();
	
	win_box_sim = create_window_box_sim();
	win_sim = create_window_sim();
	win_box_msg = create_window_box_msg();
	win_msg = create_window_msg();
	win_box_tools = create_window_box_tools();
	win_tools = create_window_tools();
	
	mvprintw(LINES - 1, 0, "Press F2 to exit");
	wrefresh(stdscr);
	wprintw(win_msg, "Current tool : Obstacle\n");
	wrefresh(win_msg);
	while((ch = getch()) != KEY_F(2)) {
		switch(ch) {
			case KEY_MOUSE :
				if (getmouse(&event) == OK) {
					wrefresh(win_msg);
					if (event.y == 32 && event.x >= 82 && event.x <= 98) {
						current_item = OBSTACLE;
						mvwprintw(win_tools, 0, 1, "X");
						mvwprintw(win_tools, 1, 1, " ");
						wrefresh(win_tools);
						wprintw(win_msg, "Current tool : Obstacle\n");
						wrefresh(win_msg);
					}
					else if (event.y == 33 && event.x >=82 && event.x <= 98) {
						current_item = ANT;
						mvwprintw(win_tools, 0, 1, " ");
						mvwprintw(win_tools, 1, 1, "X");
						wrefresh(win_tools);
						wprintw(win_msg, "Current tool : Ant\n");
						wrefresh(win_msg);
					}
					else if (event.y > 0 && event.y < NB_LINES_SIM + 1 && event.x > 0 && event.x < NB_COL_SIM + 1) {
						switch (current_item) {
							case OBSTACLE :
								pthread_mutex_lock(&map[event.y - 1][event.x - 1].mutex);
								if (map[event.y - 1][event.x - 1].element == EMPTY) {
									map[event.y - 1][event.x - 1].element = OBSTACLE;
									mvwprintw(win_sim, event.y - 1, event.x - 1, "#");
								}
								wrefresh(win_sim);
								wrefresh(win_msg);
								pthread_mutex_unlock(&map[event.y - 1][event.x - 1].mutex);
								break;
							case ANT :
								pthread_mutex_lock(&map[event.y - 1][event.x - 1].mutex);
								if (map[event.y - 1][event.x - 1].element == EMPTY) {									
									map[event.y - 1][event.x - 1].element = ANT;
									map[event.y - 1][event.x - 1].ant = (pthread_t *) malloc(sizeof(pthread_t));
									coord = (coord_t *) malloc(sizeof(coord_t));
									coord->y = event.y - 1;
									coord->x = event.x - 1;
									pthread_create(map[event.y - 1][event.x - 1].ant, NULL, procedure_ant, (void *) coord);
									mvwprintw(win_sim, event.y - 1, event.x - 1, "@");
									pthread_mutex_lock(&mutex_nb_ants);
									nb_ants++;
									pthread_mutex_unlock(&mutex_nb_ants);
								}
								wrefresh(win_sim);
								wrefresh(win_msg);
								pthread_mutex_unlock(&map[event.y - 1][event.x - 1].mutex);
								break;
						}
					}
				}
			break;
		}
	}

	delwin(win_box_sim);
	delwin(win_sim);
	delwin(win_box_msg);
	delwin(win_msg);
	delwin(win_box_tools);
	delwin(win_tools);
	ncurses_stop();

	return 0;
}