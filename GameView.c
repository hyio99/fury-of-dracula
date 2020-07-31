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
typedef struct Trail {
    PlaceId Trail_Location;
    int Trap_State;
} Trail;


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
	//Array for Trail
	Trail Trail[TRAIL_SIZE];
	//The Moves turn
	char Past_Plays[TURN_LIMIT_MSECS];
	
	
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
    int *D_Location;
    
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
	
	//Initialising the Trail_Location & Trap_State
	for (int i = 0; i < TRAIL_SIZE; i++) {
	    new->Trail[i].Trail_Location = NOWHERE;
	    new->Trail[i].Trap_State = 0;
	}
	
	strcpy(new->Past_Plays, Past_Plays);
	
//////////////////////////READING PLAYS/////////////////////////////////////////

  
	//Reading in the PastPlays using strtok
	char *cur_play = strtok(Past_Plays, " \n");
	int Death_Flag = 0;
	int pastPlays_counter = 0;
	int Trail_Flag = 0;
	int TrapCount;

	while (cur_play != NULL) {
	
	    //Simplifying terms
        P_Health = &new->PlayerList[new->Current_Player].Player_Health;
        P_Location = &new->PlayerList[new->Current_Player].Player_Location;
        D_Health = &new->PlayerList[PLAYER_DRACULA].Player_Health;  
	    D_Location = &new->PlayerList[PLAYER_DRACULA].Player_Location;  
	    
	    pastPlays_counter++;
	   
	   
//////////////////////////////HUNTER TURN///////////////////////////////////////
        
        	        	        
        //Initialising a string with player location
        char PlayerAbrLoc[2];
        if (Death_Flag == 1) {          
            new->PlayerList[new->Current_Player-1].Player_Health = 9;
            Death_Flag = 0;
        }
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
            if (cur_play[3] == 'D' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_DRACULA_ENCOUNTER;               
                *D_Health = *D_Health - LIFE_LOSS_HUNTER_ENCOUNTER ;                               
                if (*P_Health <= 0) {         
                    *P_Health = 0;
                    *P_Location = ST_JOSEPH_AND_ST_MARY;
                    new->GameScore = new->GameScore - SCORE_LOSS_HUNTER_HOSPITAL;                   
                    Death_Flag = 1;
                }
                if (*D_Health <= 0) {
                    return new;
                }
            } else if (cur_play[3] == 'V' && Death_Flag != 1) {
                new->Vampire.Vampire_Location = NOWHERE;
                new->Vampire.Vampire_Round = 0;
                new->Vampire.Vampire_State = 0;             
            } else if (cur_play[3] == 'T' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_TRAP_ENCOUNTER ;
                //Removing Trap from Dracula's Trail
                for (int i = TRAIL_SIZE - 1; i > 0; i--) {
                     if (new->Trail[i].Trail_Location == *P_Location) {
                         new->Trail[i].Trap_State = 0;
                     }                
                }
            }
            
            //Second Encounter 
            if (cur_play[4] == 'D' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_DRACULA_ENCOUNTER;               
                *D_Health = *D_Health - LIFE_LOSS_HUNTER_ENCOUNTER ;                               
                if (*P_Health <= 0) {
                    *P_Health = 0;       
                    *P_Location = ST_JOSEPH_AND_ST_MARY;
                    new->GameScore = new->GameScore - SCORE_LOSS_HUNTER_HOSPITAL;                   
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
                //Removing 2nd Trap from Dracula's Trail
                TrapCount = 0;
                for (int i = TRAIL_SIZE - 1; i > 0; i--) {
                     if (new->Trail[i].Trail_Location == *P_Location) {
                         TrapCount++;
                         if (TrapCount == 1) {
                            new->Trail[i].Trap_State = 0;
                         }
                     }                
                }
            }
            
                              
            //Third Encounter
            if (cur_play[5] == 'D' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_DRACULA_ENCOUNTER;               
                *D_Health = *D_Health - LIFE_LOSS_HUNTER_ENCOUNTER ;                               
                if (*P_Health <= 0) {
                    *P_Health = 0;         
                    *P_Location = ST_JOSEPH_AND_ST_MARY;
                    new->GameScore = new->GameScore - SCORE_LOSS_HUNTER_HOSPITAL;
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
                //Removing 3rd Trap from Dracula's Trail
                TrapCount = 0;
                for (int i = TRAIL_SIZE - 1; i > 0; i--) {
                     if (new->Trail[i].Trail_Location == *P_Location) {
                         TrapCount++;
                         if (TrapCount == 2) {
                            new->Trail[i].Trap_State = 0;
                         }
                     }                
                }
            }                   
                        
            //Fourth Encounter
            if (cur_play[6] == 'D' && Death_Flag != 1) {
                *P_Health = *P_Health - LIFE_LOSS_DRACULA_ENCOUNTER;               
                *D_Health = *D_Health - LIFE_LOSS_HUNTER_ENCOUNTER ;                               
                if (*P_Health <= 0) {
                    *P_Health = 0;         
                    *P_Location = ST_JOSEPH_AND_ST_MARY;
                    new->GameScore = new->GameScore - SCORE_LOSS_HUNTER_HOSPITAL;
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
                //Technically impossible but just for edge
            }           
            
            
//////////////////////////////DRACULA TURN//////////////////////////////////////            
        } else if ((pastPlays_counter % NUM_PLAYERS) == 0) {
            
            
            //If Dracula Hides
            //printf("%d\n", Trail_Flag);
            if (PlayerLoc == HIDE) {
                PlayerLoc = *P_Location;
            }
            
            //Doubleback 1
            if (PlayerLoc == DOUBLE_BACK_1) {
                PlayerLoc = new->Trail[0].Trail_Location;
            } 
            
            //Doubleback 2
            if (PlayerLoc == DOUBLE_BACK_2) {
                PlayerLoc = new->Trail[1].Trail_Location;
            } 
            
            //Doubleback 3
            if (PlayerLoc == DOUBLE_BACK_3) {
                PlayerLoc = new->Trail[2].Trail_Location;
            } 
            
            
            //Doubleback 4
            if (PlayerLoc == DOUBLE_BACK_4) {
                PlayerLoc = new->Trail[3].Trail_Location;
            }           
            
            //Doubleback 5
            if (PlayerLoc == DOUBLE_BACK_5) {
                PlayerLoc = new->Trail[4].Trail_Location;
            } 
            
	        //Changing the location of dracula
	        for (int i = 0; i < NUM_PLAYERS; i++) {
                if (new->PlayerList[i].Player_Name == new->Current_Player) {
                    new->PlayerList[i].Player_Location = PlayerLoc;
                }
            }	
            
            //Adding Location to the front of Queue (Pushing every other one)           
            //Loop that pushes down the trail location
            for (int i = TRAIL_SIZE - 1; i > 0; i--) {
                 //PlaceId Temp = new->Trail[i].Trail_Location;
                 new->Trail[i].Trail_Location =  new->Trail[i-1].Trail_Location;
                 new->Trail[i].Trap_State = new->Trail[i-1].Trap_State;
            }
            new->Trail[0].Trail_Location = *D_Location;
            
            
            //Adding Trap to the Trail if round is not a Vampire Round & enc < 3
            //Counting the no. of encounters in the city
            if (cur_play[3] == 'T') {
                new->Trail[0].Trap_State = 1;
            }
            
            
            
            ////If Dracula stays in Castle Dracula
            if (*D_Location == CASTLE_DRACULA) {
                *D_Health = *D_Health + LIFE_GAIN_CASTLE_DRACULA;
            
            }
            
            
            //If Dracula stays in the same location and it is still a sea
	        if (placeIdToType(PlayerLoc) == SEA) {	      
	            *D_Health = *D_Health - LIFE_LOSS_SEA ;
	            if (*D_Health <= 0) {
                    return new;
                }               
	        }
	    }    
	    	    


	    
	    
	    //Change the player location to moves
	    	     
      	    
	    //New round has started when after dracula's turn
	    if (pastPlays_counter > 4 && (pastPlays_counter % NUM_PLAYERS) == 0) {
	        new->Round_no++;
	        //Decreasing the game score
	        new->GameScore--;
	        //Increasing trailflag
	        Trail_Flag++;
	        
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
	        new->Vampire.Vampire_State = 0;
	        new->Vampire.Vampire_Round = 0;
	        new->Vampire.Vampire_Location = NOWHERE;
	        
	        }
	        
	        	        
	        
	    }
	    //printf("%d%d%d%d%d%d\n", new->Trail[0].Trail_Location,new->Trail[1].
	    //Trail_Location,new->Trail[2].Trail_Location,new->Trail[3].Trail_Location
	    //,new->Trail[4].Trail_Location,new->Trail[5].Trail_Location);
	    //Next Player 
	    //Changing the current Player
	    new->Current_Player = PlayerArray[pastPlays_counter % NUM_PLAYERS];
	    
	    cur_play = strtok(NULL, " \n");
	  
	}

	
	
	return new;
}

void GvFree(GameView gv)
{
	
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
    int TrapCount = 0;
    int Trail_Counter = 0;
	//For each Trap, go through array of previous traps and see which ones are
	//active. Count the num traps.
	for (int i = 0; i < TRAIL_SIZE; i++) {
	    if (gv->Trail[i].Trap_State == 1) {
	        TrapCount++;
	    }	
	}	
	if(numTraps == 0) {
        return NULL;
    }
    //printf("%d\n", TrapCount);
    *numTraps = TrapCount;
    
    //Dynamically allocate size of array based on numTraps	
	PlaceId *Traps = malloc(sizeof(PlaceId)*TrapCount);
	
	//Go through Trail add add all traps to array;
	
	for (int i = 0; i < TRAIL_SIZE; i++) {
	    if (gv->Trail[i].Trap_State == 1) {
	        Traps[Trail_Counter] = gv->Trail[i].Trail_Location;
	        Trail_Counter++;
	    }	
	}	
	
	//Return the malloced array
	return Traps;
}

////////////////////////////////////////////////////////////////////////
// Game History

PlaceId *GvGetMoveHistory(GameView gv, Player player,
                          int *numReturnedMoves, bool *canFree)
{
    //counting how many moves the player had
    char location[3];
    int move_count = 0;
    int action_count = 0;
    int array_counter = 0;
    //Initialising 2 strtok strings
    char play1[TURN_LIMIT_MSECS];
    char plays[TURN_LIMIT_MSECS];
    strcpy(play1, gv->Past_Plays);
    strcpy(plays, gv->Past_Plays);
	char *cur_play = strtok(play1, " \n");
	while (cur_play != NULL) {
	    //printf("%s\n", cur_play);
	    if ((action_count % NUM_PLAYERS) == player) {
	        move_count++;
	    }
	    cur_play = strtok(NULL, " \n");
	    action_count++;
	}
	//printf("%d\n", move_count);
	*numReturnedMoves = move_count;
	
	//if move_count == 0
	if (move_count == 0) {
	    return NULL;
	}
	//Dynamically allocate an array based on number of moves
	PlaceId *moves = malloc(sizeof(PlaceId) *move_count);
	
	
	//going through the array and setting the locations into placeID
	char *history_play = strtok(plays, " \n");
	//printf("%s\n", history_play);
	
    action_count = 0;
	while (history_play != NULL) {
	    //printf("%s\n", history_play);
	    if ((action_count % NUM_PLAYERS) == player) {
            //for next one while less than numMoves
            int l = 0;
	        for (int i = 1; i < 3; i++) {
	            location[l] = history_play[i];
	            l++;
	        } 
	        location[2] = '\0';
	        //printf("%s\n", location);
	        
	        //Converting the location string into ID
	        PlaceId temp = placeAbbrevToId(location);
	        
	      
	        moves[array_counter] = temp;
	        array_counter++;
	        
	 	        
	    }    
	    action_count++;
	    history_play = strtok(NULL, " \n");
	}
	
	
	*canFree = true;
	return moves;
}

PlaceId *GvGetLastMoves(GameView gv, Player player, int numMoves,
                        int *numReturnedMoves, bool *canFree)
{
	//counting how many moves the player had
    char location[3];
    int move_count = 0;
    int action_count = 0;
    int array_counter = 0;
    
    char plays[TURN_LIMIT_MSECS];
    strcpy(plays, gv->Past_Plays);
    
	char *cur_play = strtok(gv->Past_Plays, " \n");
	while (cur_play != NULL) {
	    //printf("%s\n", cur_play);
	    if ((action_count % NUM_PLAYERS) == player) {
	        move_count++;
	    }
	    cur_play = strtok(NULL, " \n");
	    action_count++;
	}
	//printf("%d\n", move_count);
	if (numMoves > move_count) {
	    *numReturnedMoves = move_count;
	} else {
	     move_count = numMoves;
	     *numReturnedMoves = move_count;
	}
	
	//if move_count == 0
	if (move_count == 0) {
	    return NULL;
	}
	//Dynamically allocate an array based on number of moves
	PlaceId *moves = malloc(sizeof(PlaceId) *move_count);
	
	
	//going through the array and setting the locations into placeID
	char *history_play = strtok(plays, " \n");
	//printf("%s\n", history_play);
	
    action_count = 0;
	while (history_play != NULL && array_counter < move_count-1) {
	   
	    if ((action_count % NUM_PLAYERS) == player) {
            //for next one while less than numMoves
            int l = 0;
	        for (int i = 1; i < 3; i++) {
	            location[l] = history_play[i];
	            l++;
	        } 
	        location[2] = '\0';
	        
	        
	        //Converting the location string into ID
	        PlaceId temp = placeAbbrevToId(location);
	        
	       
	        moves[array_counter] = temp;
	        array_counter++;
	        
	 	        
	    }    
	    action_count++;
	    history_play = strtok(NULL, " \n");
	}
	
	
	*canFree = true;
	return moves;
}

PlaceId *GvGetLocationHistory(GameView gv, Player player,
                              int *numReturnedLocs, bool *canFree)
{
	//counting how many moves the player had
    char location[3];
    int move_count = 0;
    int action_count = 0;
    int array_counter = 0;
    //Initialising 2 strtok plays
    char play1[TURN_LIMIT_MSECS];
    char plays[TURN_LIMIT_MSECS];
    strcpy(play1, gv->Past_Plays);
    strcpy(plays, gv->Past_Plays);
   
	char *cur_play = strtok(play1, " \n");
	while (cur_play != NULL) {
	    //printf("%s\n", cur_play);
	    if ((action_count % NUM_PLAYERS) == player) {
	        move_count++;
	    }
	    cur_play = strtok(NULL, " \n");
	    action_count++;
	}
	//printf("%d\n", move_count);
	*numReturnedLocs = move_count;
	
	//if move_count == 0
	if (move_count == 0) {
	    return NULL;
	}
	//Dynamically allocate an array based on number of moves
	PlaceId *moves = malloc(sizeof(PlaceId) *move_count);
	
	
	if (player == PLAYER_DRACULA) {
	    for (int i = 0; i < move_count; i++) {
	        moves[move_count-1-i] = gv->Trail[i].Trail_Location;
	    
	    }
	
	} else {
	    //going through the array and setting the locations into placeID
	    char *history_play = strtok(plays, " \n");
	    //printf("%s\n", history_play);
	    
        action_count = 0;
	    while (history_play != NULL) {
	      
	        if ((action_count % NUM_PLAYERS) == player) {
                //for next one while less than numMoves
                int l = 0;
	            for (int i = 1; i < 3; i++) {
	                location[l] = history_play[i];
	                l++;
	            } 
	            location[2] = '\0';
	       
	            
	            //Converting the location string into ID
	            PlaceId temp = placeAbbrevToId(location);
	            
	          
	            moves[array_counter] = temp;
	            array_counter++;
	            
	     	        
	        }    
	        action_count++;
	        history_play = strtok(NULL, " \n");
	    }
	}
	
	
	*canFree = true;
	return moves;
}

PlaceId *GvGetLastLocations(GameView gv, Player player, int numLocs,
                            int *numReturnedLocs, bool *canFree)
{
	//counting how many moves the player had
    char location[3];
    int move_count = 0;
    int action_count = 0;
    int array_counter = 0;
    
    //initialising 2 strtok arrays
    char play1[TURN_LIMIT_MSECS];
    char plays[TURN_LIMIT_MSECS];
    strcpy(play1, gv->Past_Plays);
    strcpy(plays, gv->Past_Plays);

	char *cur_play = strtok(play1, " \n");
	while (cur_play != NULL) {
	 
	    if ((action_count % NUM_PLAYERS) == player) {
	        move_count++;
	    }
	    cur_play = strtok(NULL, " \n");
	    action_count++;
	}
	//printf("%d\n", move_count);
	if (numLocs > move_count) {
	    *numReturnedLocs = move_count;
	} else {
	    move_count = numLocs;
	    *numReturnedLocs = move_count;
	}
	
	//if move_count == 0
	if (move_count == 0) {
	    return NULL;
	}

	//Dynamically allocate an array based on number of moves
	PlaceId *moves = malloc(sizeof(PlaceId) *move_count);

	
	
	if (player == PLAYER_DRACULA) {
	    for (int i = 0; i < move_count; i++) {
	        moves[move_count-1-i] = gv->Trail[i].Trail_Location;
	    
	    }
	
	} else {
	    //going through the array and setting the locations into placeID
	    char *history_play = strtok(plays, " \n");
	    
	    
        action_count = 0;
	    while (history_play != NULL) {
	       
	        if ((action_count % NUM_PLAYERS) == player && array_counter <= move_count) {
                //for next one while less than numMoves
                int l = 0;
	            for (int i = 1; i < 3; i++) {
	                location[l] = history_play[i];
	                l++;
	            } 
	            location[2] = '\0';
	            
	            
	            //Converting the location string into ID
	            PlaceId temp = placeAbbrevToId(location);
	            
	            
	            moves[array_counter] = temp;
	            array_counter++;
	            
	     	        
	        }    
	        action_count++;
	        history_play = strtok(NULL, " \n");
	    }
	}
	
	
	*canFree = true;
	return moves;
}

////////////////////////////////////////////////////////////////////////
// Making a Move

PlaceId *GvGetReachable(GameView gv, Player player, Round round,
                        PlaceId from, int *numReturnedLocs)
{
    int reachable_counter = 1;
	int connection_counter = 1;
	Map current = MapNew();
	//Counting the no. of connections in list
	ConnList placeFrom = MapGetConnections(current, from);
	    ConnList curr = placeFrom->next;
	while (curr != NULL) {
	    connection_counter++;
	    printf("%d\n", curr->p);
	    curr = curr->next;
	}
	
	*numReturnedLocs = connection_counter;
	//Creating an array that has all the connections
	
	PlaceId *reachable_type = malloc(sizeof(PlaceId) * connection_counter);
	reachable_type[0] = from;
	//putting all places into the array
	ConnList curr1 = placeFrom->next;
	while (curr1 != NULL) {    
	    reachable_type[reachable_counter] = curr1->p;
	    curr1 = curr1->next;
	   
	}
	
	    
	return reachable_type;
	
}

PlaceId *GvGetReachableByType(GameView gv, Player player, Round round,
                              PlaceId from, bool road, bool rail,
                              bool boat, int *numReturnedLocs)
{
    int reachable_counter = 1;
	int connection_counter = 1;
	
	Map current = MapNew();
	//Counting the no. of connections in list
	ConnList placeFrom = MapGetConnections(current, from);
	    ConnList curr = placeFrom->next;
	while (curr != NULL) {
	    connection_counter++;
	    printf("%d\n", curr->p);
	    curr = curr->next;
	}
	
	*numReturnedLocs = connection_counter;
	//Creating an array that has all the connections
	PlaceId *reachable_type = malloc(sizeof(PlaceId) * connection_counter);
	//adding curr location if it 
	
	
	//Adding the locations to list if they satisy the conditions
	
	ConnList curr1 = placeFrom->next;
	while (curr1 != NULL) {
	
	    if (road == true) {
	        if (rail == true) {
	            if (boat == true) {
	                //road == true, rail = true, boat == true
	                if (curr1->type == ROAD || curr1->type == BOAT) {
	                    reachable_type[reachable_counter] = curr1->p;
	                    reachable_counter++;
	                }
	            } else {
	                //road = true, rail == true, boat == false
	                if (curr1->type == ROAD || curr1->type == RAIL) {
	                    reachable_type[reachable_counter] = curr1->p;
	                    reachable_counter++;
	                }
	            }        
	        } else {
	            if (boat == true) {
	                //road == true, rail = false, boat == true 
	            } else {
	                //road = true, rail == false, boat == false
	            }  
	        }
	    } else {
	        if (rail == true) {
	            if (boat == true) {
	                //road == false, rail = true, boat == true 
	            } else {
	                //road = false, rail == true, boat == false
	            }        
	        } else {
	            if (boat == true) {
	                //road == false, rail = false, boat == true 
	            } else {
	                //road = false, rail == false, boat == false
	            }  
	        }
	    
	    }
	
	
	    if (curr1->type == road && curr1->type == rail && curr1->type == boat) {
	        reachable_type[reachable_counter] = curr1->p;
	        
	    }
	    curr1 = curr1->next;
	   
	}
	
	    
	return reachable_type;
}

////////////////////////////////////////////////////////////////////////
// Your own interface functions

// TODO
