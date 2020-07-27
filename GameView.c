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


//Adjacentry matrix of all the locations?

typedef struct playerView {
    Player Player_Name;
    int Player_Health;
    PlaceId Player_Location;
    
} playerView;


/*typedef struct Dracula {
    int Dracula_Health;
    PlaceId Dracula_Location;

} Dracula;*/

struct gameView {
	//Integer for GameScore
	int GameScore;
	//Integer for round no.
	Round Round_no;
	//Player who has the current turn
	Player Current_Player;
	//Linked-list of players/array
	playerView PlayerList[NUM_PLAYERS]; 

	
	
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
	
	/*/Initialising the Dracula Structs
	new->Dracula.Dracula_Health = GAME_START_BLOOD_POINTS;
	new->Dracula.Dracula_Location = NOWHERE;
	*/
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
    
	for (int i = 0; i < NUM_PLAYERS; i++) {
	    if (gv->PlayerList[i].Player_Name == player) {
	        return gv->PlayerList[i].Player_Location;
	    }
	}
	//maybe return error code if not found?
	return 0;
}

PlaceId GvGetVampireLocation(GameView gv)
{
	return NOWHERE;
}

PlaceId *GvGetTrapLocations(GameView gv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
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
