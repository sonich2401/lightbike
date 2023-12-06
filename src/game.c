#include "game.h"
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <limits.h>

#define SHORT_MAX (0xFFFF)

#include "input.h"

#include "debug.h"
#define ACCESS_GLOBALS
#include "globals.h"
#include "colors.h"


static volatile bool is_running = false;
#ifdef GAME_USE_THREADS
	static pthread_t game_thread_id;
#endif


typedef enum{
	TILE_NONE,
	TILE_TRAIL
}tile_t;


static tile_t* game_board;


#define INDEX_BOARD(x,y) game_board[g_game_w * y  + x]



typedef enum

typedef struct{
	COLOR_t color;
	
}Player;



static void* game_thread(void* args){
    (void)args;
    while(!is_running){usleep(1000);} //wait for game to start

    InputEvent_t event;
    while(true){
		//Create delay to not max out CPU
		usleep(1000);
    
        //Check for new input
        event = input_poll_input();
        while(event.packet_type != INPUT_EVENT_NONE){
            
            switch(event.packet_type){
                case INPUT_EVENT_NONE:
                    DFATAL("This should never occur!");
                break;
                case INPUT_EVENT_MOUSE:

                break;

                case INPUT_EVENT_KEYDOWN:
					if(event.KeyDown.key == 'q'){
						return NULL; //exit game
					}         	
                break;
            }
            
            event = input_poll_input(); //Get next input
        }


        if(game_over){
            return NULL;
        }

       	


        //MAIN GAME LOGIC
        
    }
    
    return NULL;
}





void init_game(void){
    size_t byte_count =  g_game_w * g_game_h * sizeof(tile_t);
    game_board = (tile_t*)malloc(byte_count);
    SMART_ASSERT(game_board != NULL, "ran out of memory!\n Tried to allocate %lu, bytes!", byte_count);
    
    for(size_t i = 0; i < g_game_w * g_game_h; i++){
        game_board[i] = TILE_NONE;
    }


    #ifdef GAME_USE_THREADS
	    int return_code;
	    return_code = pthread_create(&game_thread_id, NULL, game_thread, NULL);
	    SMART_ASSERT(return_code == 0, "failed to create game thread");
    #endif
}


int run_game(void){
    
    is_running = true;
    #ifdef GAME_USE_THREADS
    	(void)pthread_join(game_thread_id, NULL);
    #else
    	(void)game_thread(NULL);
    #endif
    
    return EXIT_SUCCESS;
}
