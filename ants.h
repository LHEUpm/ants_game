#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <ncursesw/ncurses.h>

#define NB_LINES_SIM		40
#define NB_COL_SIM			80
#define NB_LINES_MSG		29
#define NB_COL_MSG			49
#define NB_LINES_TOOLS 		9
#define NB_COL_TOOLS 		49

#define EMPTY				0				
#define OBSTACLE			1				
#define ANT					2


typedef struct case_tag {
	int element;
	pthread_t *ant;
	pthread_mutex_t mutex;
} case_t;

typedef struct coord_tag {
	int y;
	int x;
} coord_t;

WINDOW *win_sim;
WINDOW *win_msg;

case_t map[NB_LINES_SIM][NB_COL_SIM];	
int nb_ants;
pthread_mutex_t mutex_nb_ants;