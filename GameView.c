////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// GameView.c: GameView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10   v3.0    Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Game.h"
#include "GameView.h"
#include "Map.h"
#include "Places.h"
// add your own #includes here
#include <string.h>

/*//Structs
struct Dracula {
    int blood_points;
    Trail Trail;

};
// TODO: ADD YOUR OWN STRUCTS HERE

// Linked list for trail where a node points to the next on 
struct Trail {
    int trail_position;
    char location[];
    struct Trail *next;

}; Trail;*/
typedef enum Vampire_State {
    VAMPIRE_TRUE,
    VAMPIRE_FALSE
} Vampire_State;


//Struct for Player Views
typedef struct playerView {
    Player Player_Name;
    int Player_Health;
    PlaceId Player_Location;
    
} playerView;

//Struct for immature vampires
typedef struct Vampire {
    int Vampire_State;
    int Vampire_Round;
    PlaceId Vampire_Location;

} Vampire;


struct gameView {
	//Integer for GameScore
	int GameScore;
	//Integer for round no.
	Round Round_no;
	//Player who has the current turn
	Player Current_Player;
	//Linked-list of players/array
	playerView PlayerList[NUM_PLAYERS]; 
	//Struct for immature vampires (use linked list?)
	Vampire Vampire;
	
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

GameView GvNew(char *pastPlays, Message messages[])
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	GameView new = malloc(sizeof(*new));
	if (new == NULL) {
		fprintf(stderr, "Couldn't allocate GameView!\n");
		exit(EXIT_FAILURE);
	}
	new->GameScore = GAME_START_SCORE;
	new->Round_no = 0;
	new->Current_Player = PLAYER_LORD_GODALMING;
	
	Player PlayerArray[] = {PLAYER_LORD_GODALMING, PLAYER_DR_SEWARD, 
	                        PLAYER_VAN_HELSING,	PLAYER_MINA_HARKER,		
	                        PLAYER_DRACULA};
	                        
	//Initialising the Player Structs
	for (int i = 0; i < NUM_PLAYERS; i++) {
	    new->PlayerList[i].Player_Name = PlayerArray[i];
	    if (new->PlayerList[i].Player_Name == PLAYER_DRACULA) {
	        new->PlayerList[i].Player_Health = GAME_START_BLOOD_POINTS;
	    } else {
	        new->PlayerList[i].Player_Health = GAME_START_HUNTER_LIFE_POINTS;
	    }
	    new->PlayerList[i].Player_Location = NOWHERE;
	}
	
	//Initialising the Vampire
	new->Vampire.Vampire_Location = NOWHERE;
	new->Vampire.Vampire_Round = 0;
	
	
	//Reading in the PastPlays using strtok
	char * cur_play = strtok(pastPlays, " \n");
	
	int pastPlays_counter = 0;
	//counting the round numbers
	while (cur_play != NULL) {
	    
	    
	    //Checking if the string is segmented properly
	    printf("%s\n", cur_play);
	    
	    
	    //New round has started 
	    if ((pastPlays_counter % 6) == 0) {
	        new->Round_no++;
	    }
	    //Figuring out the move of the Player -> locations
	    
	    
	        //if the player is dracula and vampire round
	        //Add vampire into linked list of vampires.
	        
	        
	        
	            //If Dracula is on sea -2
	    
	    
	    
	    //Change the player location to moves
	    
	    
	    
	    //Next Player 
	    
	    
	    //
	    cur_play = strtok(NULL, " \n");
	    pastPlays_counter++;
	}
	
	
	
	
	return new;
}

void GvFree(GameView gv)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	free(gv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round GvGetRound(GameView gv)
{
	return gv->Round_no;
}

Player GvGetPlayer(GameView gv)
{
	return gv->Current_Player;
}

int GvGetScore(GameView gv)
{
	return gv->GameScore;
}

int GvGetHealth(GameView gv, Player player)
{
	for (int i = 0; i < NUM_PLAYERS; i++) {
	    if (gv->PlayerList[i].Player_Name == player) {
	        return gv->PlayerList[i].Player_Health;
	    }
	}
	//maybe return error code if not found?
	return 0;
}

PlaceId GvGetPlayerLocation(GameView gv, Player player)
{
    //Return NOWHERE if the given player has not had a turn yet.
    if (gv->Round_no == 0) {
        return NOWHERE;
    }
    
    //Player Location for Hunters
    if (player != PLAYER_DRACULA) {
	    for (int i = 0; i < NUM_PLAYERS; i++) {
	        if (gv->PlayerList[i].Player_Name == player) {
	            return gv->PlayerList[i].Player_Location;
	        }
	    }
	} else {
	//Player Location for Dracula
	    //Loop that checks if location has been revealed / if Dracula's current
	    //location has been revealed anywhere in the trail
	    
	    //else if it hasn't been revealed
	    
	    //Check for whether the place is on land or sea
	    if (placeIsLand(gv->PlayerList[PLAYER_DRACULA].Player_Location) == LAND) {
	        return CITY_UNKNOWN;
	    } else {
	        return SEA_UNKNOWN;
	    }
	}
	//maybe return error code if not found?
	return 0;
}
//immature vampire;
PlaceId GvGetVampireLocation(GameView gv)
{
	return gv->Vampire.Vampire_Location;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	//For each Trap, go through array of previous traps and see which ones are
	//active. Count the num traps.
	
	//Dynamically allocate size of array based on numTraps
	
	//Copy paste the number of traps
	
	//Return the malloced array
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return NULL;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	*canFree = false;
	return 0;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
