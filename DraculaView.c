////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// DraculaView.c: the DraculaView ADT implementation
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v2.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v3.0	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "DraculaView.h"
#include "Game.h"
#include "GameView.h"
#include "Map.h"
// add your own #includes here


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

struct draculaView {
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
	//Linked List for Trail
    
};

////////////////////////////////////////////////////////////////////////
// Constructor/Destructor

DraculaView DvNew(char *pastPlays, Message messages[])
{
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
}

void DvFree(DraculaView dv)
{
    
	free(dv);
}

////////////////////////////////////////////////////////////////////////
// Game State Information

Round DvGetRound(DraculaView dv)
{
	return dv->Round_no;
}

int DvGetScore(DraculaView dv)
{
	return dv->GameScore;
}

int DvGetHealth(DraculaView dv, Player player)
{
	for (int i = 0; i < NUM_PLAYERS; i++) {
	    if (dv->PlayerList[i].Player_Name == player) {
<<<<<<< HEAD
	        return dv->PlayerList[i].Player_Health;
=======
	        return dv->hp;
>>>>>>> 648c6ff96a24ddaadce88e07173ae70c1067c749
	    }
	}
	//maybe return error code if not found?
	return 0;
}

PlaceId DvGetPlayerLocation(DraculaView dv, Player player)
{
<<<<<<< HEAD
	//Player Location for Everyone as Gameview shows all
    
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (dv->PlayerList[i].Player_Name == player) {
            return dv->PlayerList[i].Player_Location;
        }
    }

	/*else {
	//Player Location for Dracula
	    //Loop that checks if location has been revealed / if Dracula's current
	    //location has been revealed anywhere in the trail
	    
	    //else if it hasn't been revealed
	    
	    //Check for whether the place is on land or sea
	    if (placeIsLand(gv->PlayerList[PLAYER_DRACULA].Player_Location) == LAND) {
	        return CITY_UNKNOWN;
	    } else {
	        return SEA_UNKNOWN;
=======
	for (int i = 0; i < NUM_PLAYERS; i++) {
	    if (dv->PlayerList[i].Player_Name == player) {
	        return dv->draculaLocation;
>>>>>>> 648c6ff96a24ddaadce88e07173ae70c1067c749
	    }
	}
	//maybe return error code if not found?*/
	return 0;
}

PlaceId DvGetVampireLocation(DraculaView dv)
{
	return dv->Vampire.Vampire_Location;
}

PlaceId *DvGetTrapLocations(DraculaView dv, int *numTraps)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numTraps = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *DvGetValidMoves(DraculaView dv, int *numReturnedMoves)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedMoves = 0;
	return NULL;
}

PlaceId *DvWhereCanIGo(DraculaView dv, int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanIGoByType(DraculaView dv, bool road, bool boat,
                             int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanTheyGo(DraculaView dv, Player player,
                          int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

PlaceId *DvWhereCanTheyGoByType(DraculaView dv, Player player,
                                bool road, bool rail, bool boat,
                                int *numReturnedLocs)
{
	// TODO: REPLACE THIS WITH YOUR OWN IMPLEMENTATION
	*numReturnedLocs = 0;
	return NULL;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO