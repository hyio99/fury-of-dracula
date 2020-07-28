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
    //copying string into a string literal
    char Past_Plays[TURN_LIMIT_MSECS];
    strcpy(Past_Plays, pastPlays);
    //Simplifying terms
    int *P_Health;
    int *P_Location;
    int *D_Health;
//////////////////////////INITIALISING STATE////////////////////////////////////
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
	
	
	
//////////////////////////READING PLAYS/////////////////////////////////////////

  
	//Reading in the PastPlays using strtok
	char *cur_play = strtok(Past_Plays, " \n");
	
	int pastPlays_counter = 0;

	while (cur_play != NULL) {
	
	    //Simplifying terms
        P_Health = &new->PlayerList[new->Current_Player].Player_Health;
        P_Location = &new->PlayerList[new->Current_Player].Player_Location;
        D_Health = &new->PlayerList[PLAYER_DRACULA].Player_Health;  
	    int Death_Flag = 0;
	    
	    pastPlays_counter++;
	   
	   
//////////////////////////////HUNTER TURN///////////////////////////////////////
        
        	        	        
        //Initialising a string with player location
        char PlayerAbrLoc[2];
        
        //Copying the Location onto the string
        int l = 0;
        for (int j = 1; j < 3; j++) {
            PlayerAbrLoc[l] = cur_play[j];
            l++;
        }

        //Converting Player String to a placeID
        PlaceId PlayerLoc = placeAbbrevToId(PlayerAbrLoc);
        
        if ((pastPlays_counter % NUM_PLAYERS) != 0) {
	           
	        //If the Player Rests
	        if (*P_Location == PlayerLoc) {
	            *P_Health = *P_Health + LIFE_GAIN_REST;
	            if (*P_Health > 9) {
	                *P_Health = 9;
	            }
	             
	        }	        	        
	           	           
	        //Changing the location of the current hunter
	        for (int i = 0; i < NUM_PLAYERS; i++) {
                if (new->PlayerList[i].Player_Name == new->Current_Player) {
                    new->PlayerList[i].Player_Location = PlayerLoc;
                }
            }
            
            //Checking the next part of the string
            
            //First Encounter          
            if (cur_play[4] == 'D' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_DRACULA_ENCOUNTER;               
                *D_Health = *D_Health - LIFE_LOSS_HUNTER_ENCOUNTER ;                               
                if (*P_Health <= 0) {         
                    *P_Location = ST_JOSEPH_AND_ST_MARY;
                    new->GameScore = new->GameScore - SCORE_LOSS_HUNTER_HOSPITAL;
                    *P_Health = GAME_START_HUNTER_LIFE_POINTS;
                    Death_Flag = 1;
                }
                if (*D_Health <= 0) {
                    return new;
                }
            } else if (cur_play[4] == 'V' && Death_Flag != 1) {
                new->Vampire.Vampire_Location = NOWHERE;
                new->Vampire.Vampire_Round = 0;
                new->Vampire.Vampire_State = 0;             
            } else if (cur_play[4] == 'T' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_TRAP_ENCOUNTER ;
                //Get rid of trap ->needs to be implemented
            }
            
            //Second Encounter 
            if (cur_play[5] == 'D' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_DRACULA_ENCOUNTER;               
                *D_Health = *D_Health - LIFE_LOSS_HUNTER_ENCOUNTER ;                               
                if (*P_Health <= 0) {         
                    *P_Location = ST_JOSEPH_AND_ST_MARY;
                    new->GameScore = new->GameScore - SCORE_LOSS_HUNTER_HOSPITAL;
                    *P_Health = GAME_START_HUNTER_LIFE_POINTS;
                    Death_Flag = 1;
                }
                if (*D_Health <= 0) {
                    return new;
                }
            } else if (cur_play[5] == 'V' && Death_Flag != 1) {
                new->Vampire.Vampire_Location = NOWHERE;
                new->Vampire.Vampire_Round = 0;
                new->Vampire.Vampire_State = 0;             
            } else if (cur_play[5] == 'T' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_TRAP_ENCOUNTER ;
                //Get rid of trap ->needs to be implemented
            }
            
                              
            //Third Encounter
            if (cur_play[6] == 'D' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_DRACULA_ENCOUNTER;               
                *D_Health = *D_Health - LIFE_LOSS_HUNTER_ENCOUNTER ;                               
                if (*P_Health <= 0) {         
                    *P_Location = ST_JOSEPH_AND_ST_MARY;
                    new->GameScore = new->GameScore - SCORE_LOSS_HUNTER_HOSPITAL;
                    *P_Health = GAME_START_HUNTER_LIFE_POINTS;
                    Death_Flag = 1;
                }
                if (*D_Health <= 0) {
                    return new;
                }
            } else if (cur_play[6] == 'V' && Death_Flag != 1) {
                new->Vampire.Vampire_Location = NOWHERE;
                new->Vampire.Vampire_Round = 0;
                new->Vampire.Vampire_State = 0;             
            } else if (cur_play[6] == 'T' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_TRAP_ENCOUNTER ;
                //Get rid of trap ->needs to be implemented
            }                   
                        
            //Fourth Encounter
            if (cur_play[7] == 'D' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_DRACULA_ENCOUNTER;               
                *D_Health = *D_Health - LIFE_LOSS_HUNTER_ENCOUNTER ;                               
                if (*P_Health <= 0) {         
                    *P_Location = ST_JOSEPH_AND_ST_MARY;
                    new->GameScore = new->GameScore - SCORE_LOSS_HUNTER_HOSPITAL;
                    *P_Health = GAME_START_HUNTER_LIFE_POINTS;
                    Death_Flag = 1;
                }
                if (*D_Health <= 0) {
                    return new;
                }
            } else if (cur_play[7] == 'V' && Death_Flag != 1) {
                new->Vampire.Vampire_Location = NOWHERE;
                new->Vampire.Vampire_Round = 0;
                new->Vampire.Vampire_State = 0;             
            } else if (cur_play[7] == 'T' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_TRAP_ENCOUNTER ;
                //Get rid of trap ->needs to be implemented
            }
           
            
        } else if ((pastPlays_counter % NUM_PLAYERS) == 0) {
             

	        //Changing the location of dracula
	        for (int i = 0; i < NUM_PLAYERS; i++) {
                if (new->PlayerList[i].Player_Name == new->Current_Player) {
                    new->PlayerList[i].Player_Location = PlayerLoc;
                }
            }	  
	    }
	    	    
	    //If both the hunter and Dracula meet
	        
	        
	        
	        
	        
	        
	            //If Dracula is on sea -2
	    
	    
	    
	    //Change the player location to moves
	    
	    
	    
	     
	    
	    //New round has started when after dracula's turn
	    if (pastPlays_counter > 4 && (pastPlays_counter % NUM_PLAYERS) == 0) {
	        new->Round_no++;
	        //Decreasing the game score
	        new->GameScore--;
	        
	        //If the Vampire State is 1
	        if (new->Vampire.Vampire_State == 1) {
	            new->Vampire.Vampire_Round++;
	        }
	        
	      
	        //if it is the first round or every 13 rounds after that add vampire
	        if ((new->Round_no % 13) == 1) {
	            new->Vampire.Vampire_State = 1;
	            new->Vampire.Vampire_Round = 0;
	            new->Vampire.Vampire_Location = PlayerLoc;
	        }
	        
	        
	        //If the vampire has been alive for 6 rounds
	        if (new->Vampire.Vampire_Round == 6) {
	        new->GameScore = new->GameScore - SCORE_LOSS_VAMPIRE_MATURES;
	        
	        }
	        
	        
	        
	        
	    }
	    
	    //Next Player 
	    //Changing the current Player
	    new->Current_Player = PlayerArray[pastPlays_counter % NUM_PLAYERS];
	    
	    cur_play = strtok(NULL, " \n");
	  
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
    //Player Location for Everyone as Gameview shows all
    
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (gv->PlayerList[i].Player_Name == player) {
            return gv->PlayerList[i].Player_Location;
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
	    }
	}
	//maybe return error code if not found?*/
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
