// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of David Steiman nor the name of any other
//    contributor may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY David Steiman AND CONTRIBUTORS ``AS IS'' AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL David Steiman OR ANY OTHER
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//#ifndef PHP_COMBATSYSTEM_H
//#define PHP_COMBATSYSTEM_H


#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <math.h>
#include <time.h>
#include <iostream>
#include <linux/types.h>
#include "combat.h"

RNG::RNG()
{
    srand(time(0));
}

bool RNG::chanceDecide(int chance, bool toPerMille)
{
	return toPerMille ? this->getRandom(0,999) < chance : getRandom(0,99) < chance;
}
bool RNG::chanceDecide(int chance)
{
	return this->chanceDecide(chance,false);
}
int RNG::getRandom(int a, int b)
{
	if(a == b) return a;
	int randnum = rand() % 10000;
	float zahl = a + (randnum/10000.0)*(b-a);
	
	int c = (int) zahl;
	float d = zahl - c;
	if(d > 0.5) c++;

	return a == b && a == 0 ? 0 : c;
}

CombatSys::CombatSys()
{
	this->_PLAYERS[T_ATTACKER] = (T_PLAYER*) malloc(MAX_SLOTS * sizeof(T_PLAYER));
	this->_PLAYERS[T_DEFENDER] = (T_PLAYER*) malloc(MAX_SLOTS * sizeof(T_PLAYER));

	this->allShipCount[T_ATTACKER] = 0;
	this->allShipCount[T_DEFENDER] = 0;
	this->allPlayersCount[T_ATTACKER] = 0;
	this->allPlayersCount[T_DEFENDER] = 0;

	this->roundCount = 0;
	this->_VERSION = 95;
	this->_CAN_BATTLE = false;
	this->_CAN_GET_RESULT = false;
	this->unitsSet = 0;
    this->_SHOOT_AGAINST_EXPLODED = false;
    this->_ENABLE_RAPID_FIRE = true;
}

int CombatSys::getVersion()
{
	return this->_VERSION;
}

void CombatSys::setShootAgainstExploded(bool modus)
{
    this->_SHOOT_AGAINST_EXPLODED = modus;   
}

void CombatSys::setRapidfire(bool modus)
{
    this->_ENABLE_RAPID_FIRE = modus;   
}

void CombatSys::setUnitCount(int count)
{
	this->_SHIPS = (T_SHIP *) malloc(count * sizeof(T_SHIP));
	this->unitCount = count;
}
bool CombatSys::canSetShip(int id)
{
	return id > -1 && id < this->unitCount;
}
void CombatSys::configShip(int id, long attack, long hull, long shield, char *rapidfire)
{
	int *list = (int *) malloc(this->unitCount * sizeof(int));
	int rfPercentage;
	//begins with no rf
	memset(list,0,this->unitCount * sizeof(int));
	
	int toid = -1;
		
	//now we get the rapidfire as id1:rf1;id2:rf2;....;idN:rfN;
	char	* nextShip,
			* tmp;
	tmp = (char*) malloc(strlen(rapidfire) * 2 *sizeof(char));
	strcpy(tmp,rapidfire);
	nextShip = (char*) malloc(10*sizeof(char));
	nextShip = strtok(tmp,":;");
	while(nextShip != NULL)
	{
		if(strlen(nextShip) > 0)
		{
			if(toid == -1)
				toid = atoi(nextShip);
			else
			{
				//RF X means a chance of X-1/X for another shoot, that means that if rand(1,1000) < ((x-1) * 1000 / x) is true in x-1 / x * 100 percent of cases
				rfPercentage = atoi(nextShip);
				rfPercentage = ((rfPercentage - 1) * 1000) / rfPercentage;

				list[toid] = rfPercentage;
				toid = -1;
			}
			
		}
		nextShip = strtok(NULL,":;");
	}

	T_SHIP thisShip = {id,attack,hull,shield,list};

	this->_SHIPS[id] = thisShip;
	this->unitsSet++;

}
bool CombatSys::allShipsConfigured()
{
	return this->unitsSet == this->unitCount;
}
bool CombatSys::canSetAttacker(int id)
{
	return id > -1 && id < 	this->allPlayersCount[T_ATTACKER]+1;
}
void CombatSys::addAttacker(int id, int weapon, int shield, int armor)
{
	int * emptyShipList = (int*) malloc(this->unitCount * sizeof(int));
	memset(emptyShipList,0,this->unitCount * sizeof(int));


	T_PLAYER thisPlayer = {id,weapon,shield,armor,emptyShipList};
	this->_PLAYERS[T_ATTACKER][id] = thisPlayer;
	this->allPlayersCount[T_ATTACKER]++;
}

bool CombatSys::canSetDefender(int id)
{
	return id > -1 && id < 	this->allPlayersCount[T_DEFENDER]+1;
}
void CombatSys::addDefender(int id, int weapon, int shield, int armor)
{
	int * emptyShipList = (int*) malloc(this->unitCount * sizeof(int));
	memset(emptyShipList,0,this->unitCount * sizeof(int));



	T_PLAYER thisPlayer = {id,weapon,shield,armor,emptyShipList};
	this->_PLAYERS[T_DEFENDER][id] = thisPlayer;
	this->allPlayersCount[T_DEFENDER]++;
}

void CombatSys::addShips(const unsigned fraction, int playerId, int shipId, int amount)
{
	this->_PLAYERS[fraction][playerId].shipList[shipId] = amount;

	this->allShipCount[fraction] += amount;

	this->_CAN_BATTLE = true;
}


void CombatSys::battle()
{
	if(!this->_CAN_BATTLE) return;
	int			shipCount=0;

	if(this->allShipCount[T_ATTACKER] == 0 || this->allShipCount[T_DEFENDER] == 0)
	{
		this->_CAN_GET_RESULT = true;
		return;
	}
	//all the attackers
	this->_UNITS[T_ATTACKER] = (T_UNIT *) malloc(this->allShipCount[T_ATTACKER] * sizeof(T_UNIT));
	for(int i = 0; i<this->allPlayersCount[T_ATTACKER];i++)
	{
		for(int j=0;j<this->unitCount;j++)
		{
			if(this->_PLAYERS[T_ATTACKER][i].shipList[j] > 0)
			{
				for(int k = 0;k<this->_PLAYERS[T_ATTACKER][i].shipList[j];k++)
				{
					T_UNIT unit = {
										false,
										this->_SHIPS[j].shield * (1 + (float)(this->_PLAYERS[T_ATTACKER][i].techShield / 10)),
										this->_SHIPS[j].shield * (1 + (float)(this->_PLAYERS[T_ATTACKER][i].techShield / 10)),
										this->_SHIPS[j].hull * (1 + (float)(this->_PLAYERS[T_ATTACKER][i].techArmor / 10)),
										this->_SHIPS[j].hull * (1 + (float)(this->_PLAYERS[T_ATTACKER][i].techArmor / 10)),
										&this->_SHIPS[j],
										&this->_PLAYERS[T_ATTACKER][i]
					};
					this->_UNITS[T_ATTACKER][shipCount++] = unit;
				}
			}
		}
	}

	shipCount = 0;
	//same for defenders
	this->_UNITS[T_DEFENDER] = (T_UNIT *) malloc(this->allShipCount[T_DEFENDER] * sizeof(T_UNIT));
	for(int i = 0; i<this->allPlayersCount[T_DEFENDER];i++)
	{
		for(int j=0;j<this->unitCount;j++)
		{
			if(this->_PLAYERS[T_DEFENDER][i].shipList[j] > 0)
			{
				for(int k = 0;k<this->_PLAYERS[T_DEFENDER][i].shipList[j];k++)
				{
					T_UNIT unit = {
										false,
										this->_SHIPS[j].shield * (1 + (float)(this->_PLAYERS[T_DEFENDER][i].techShield / 10)),
										this->_SHIPS[j].shield * (1 + (float)(this->_PLAYERS[T_DEFENDER][i].techShield / 10)),
										this->_SHIPS[j].hull * (1 + (float)(this->_PLAYERS[T_DEFENDER][i].techArmor / 10)),
										this->_SHIPS[j].hull * (1 + (float)(this->_PLAYERS[T_DEFENDER][i].techArmor / 10)),
										&this->_SHIPS[j],
										&this->_PLAYERS[T_DEFENDER][i]
					};
					this->_UNITS[T_DEFENDER][shipCount++] = unit;
				}
			}
		}
	}

	int round;

	//main battle
	for(round = 0;round<6;round++)
	{
		this->roundCount++;
		//define the round data collector
		this->_ROUNDS[round] = T_ROUND();
		T_ROUND* thisRound = &this->_ROUNDS[round];

		thisRound->attackShots = 0;
		thisRound->attackDamage = 0;
		thisRound->attackAbsorbed = 0;
		thisRound->defendShots = 0;
		thisRound->defendDamage = 0;
		thisRound->defendAbsorbed = 0;
		
		bool	shooting;
        int explodedUnits[2];
        explodedUnits[T_ATTACKER] = 0;
        explodedUnits[T_DEFENDER] = 0;

		//attacker vs. defender
		for(int a = 0; a<this->allShipCount[T_ATTACKER];a++)
		{
			if(explodedUnits[T_DEFENDER] == this->allShipCount[T_DEFENDER])
				break;
			do
			{
				thisRound->attackShots++;
				if(explodedUnits[T_DEFENDER] == this->allShipCount[T_DEFENDER])
					break;
				
				int vs = this->_RNG.getRandom(explodedUnits[T_DEFENDER],this->allShipCount[T_DEFENDER] - 1);

				if(!this->_UNITS[T_DEFENDER][vs].exploded || this->_SHOOT_AGAINST_EXPLODED)
				{
					this->Shoot(&this->_UNITS[T_ATTACKER][a], &this->_UNITS[T_DEFENDER][vs], &thisRound->attackDamage,&thisRound->defendAbsorbed,&shooting);
				}
				else
				{
					shooting = false;
				}
				//printf("a(%i): %i(%i) vs %i(%i), exploded:%i\n",round,this->_UNITS[T_ATTACKER][a].original->id,a,this->_UNITS[T_DEFENDER][vs].original->id,vs,this->_UNITS[T_DEFENDER][vs].exploded);

                
                if(this->_UNITS[T_DEFENDER][vs].exploded && !this->_SHOOT_AGAINST_EXPLODED)
                {
					explodedUnits[T_DEFENDER];
					//if(explodedUnits[T_DEFENDER] < this->allShipCount[T_DEFENDER])
					{
						if(explodedUnits[T_DEFENDER] != vs)
							std::swap(this->_UNITS[T_DEFENDER][explodedUnits[T_DEFENDER]],this->_UNITS[T_DEFENDER][vs]);
						explodedUnits[T_DEFENDER]++;
					}
                }
                

			}
			while(shooting);
		}
		//defender vs. attacker
		for(int d = 0; d<this->allShipCount[T_DEFENDER];d++)
		{
			if(explodedUnits[T_ATTACKER] == this->allShipCount[T_ATTACKER])
				break;
			do
			{
				if(explodedUnits[T_ATTACKER] == this->allShipCount[T_ATTACKER])
					break;
				
				thisRound->defendShots++;
				int vs = this->_RNG.getRandom(explodedUnits[T_ATTACKER],this->allShipCount[T_ATTACKER] - 1);

				if(!this->_UNITS[T_ATTACKER][vs].exploded || this->_SHOOT_AGAINST_EXPLODED)
					this->Shoot(&this->_UNITS[T_DEFENDER][d], &this->_UNITS[T_ATTACKER][vs], &thisRound->defendDamage, &thisRound->attackAbsorbed, &shooting);
				else
					shooting = false;
				//printf("d(%i): %i(%i) vs %i(%i), exploded:%i\n",round,this->_UNITS[T_DEFENDER][d].original->id,d,this->_UNITS[T_ATTACKER][vs].original->id,vs,this->_UNITS[T_ATTACKER][vs].exploded);
				
                
                if(this->_UNITS[T_ATTACKER][vs].exploded && !this->_SHOOT_AGAINST_EXPLODED)
                {
					//if(explodedUnits[T_DEFENDER] < this->allShipCount[T_DEFENDER])
					{
						if(explodedUnits[T_ATTACKER] != vs)
							std::swap(this->_UNITS[T_ATTACKER][explodedUnits[T_ATTACKER]],this->_UNITS[T_ATTACKER][vs]);
						explodedUnits[T_ATTACKER]++;
					}
                }
			}
			while(shooting);
		}

		//updating fleet, maybe quit the fight

		//printf("for round %i\n",round);
		this->UpdateFleets();

		
		thisRound->shiplist[T_ATTACKER] = (int**)malloc(this->allPlayersCount[T_ATTACKER] * sizeof(int*));
		for(int player = 0; player < this->allPlayersCount[T_ATTACKER];player++)
		{
			thisRound->shiplist[T_ATTACKER][player] = (int*)malloc(this->unitCount * sizeof(int));
			memset(thisRound->shiplist[T_ATTACKER][player],0,this->unitCount * sizeof(int));

			for(int unit = 0; unit < this->unitCount;unit++)
				
				thisRound->shiplist[T_ATTACKER][player][unit] = this->_PLAYERS[T_ATTACKER][player].shipList[unit];
		}

		thisRound->shiplist[T_DEFENDER] = (int**)malloc(this->allPlayersCount[T_DEFENDER] * sizeof(int*));
		for(int player = 0; player < this->allPlayersCount[T_DEFENDER];player++)
		{
			thisRound->shiplist[T_DEFENDER][player] = (int*)malloc(this->unitCount * sizeof(int));
			memset(thisRound->shiplist[T_DEFENDER][player],0,this->unitCount * sizeof(int));

			for(int unit = 0; unit < this->unitCount;unit++)
				thisRound->shiplist[T_DEFENDER][player][unit] = this->_PLAYERS[T_DEFENDER][player].shipList[unit];
		}
		


		
		//game over
		if(this->allShipCount[T_ATTACKER] == 0 || this->allShipCount[T_DEFENDER] == 0)
		{
			//free(this->_UNITS);
			break;
		}
		//return;
	}

	this->_CAN_GET_RESULT = true;
}

void CombatSys::UpdateFleets()
{
	//Clear Exploded
	T_UNIT *newUnit, *src;
	int cnt;
	int shipCount;
	
	
	bool isexploded;
	
	
	cnt= 0;
	src = this->_UNITS[T_ATTACKER];
	newUnit = (T_UNIT *) malloc(this->allShipCount[T_ATTACKER] * sizeof(T_UNIT));
	shipCount = this->allShipCount[T_ATTACKER];
	
	for(int i=0;i<shipCount;i++)
	{
		
		isexploded = src[i].exploded;
		if(!isexploded)
		{
			this->_UNITS[T_ATTACKER][i].shieldLeft = this->_UNITS[T_ATTACKER][i].shieldMax;
			
			T_UNIT unit = {
				false,
				this->_UNITS[T_ATTACKER][i].shieldMax,
				this->_UNITS[T_ATTACKER][i].shieldMax,
				this->_UNITS[T_ATTACKER][i].hullLeft,
				this->_UNITS[T_ATTACKER][i].hullMax,
				this->_UNITS[T_ATTACKER][i].original,
				this->_UNITS[T_ATTACKER][i].owner,
			};
			
			newUnit[cnt++] = unit;
			
		}
		else
		{
			this->allShipCount[T_ATTACKER]--;
			this->_UNITS[T_ATTACKER][i].owner->shipList[this->_UNITS[T_ATTACKER][i].original->id]--;
		}
	}
	free(this->_UNITS[T_ATTACKER]);
	this->_UNITS[T_ATTACKER] = newUnit;

	cnt= 0;
	newUnit = (T_UNIT *) malloc(this->allShipCount[T_DEFENDER] * sizeof(T_UNIT));
	shipCount = this->allShipCount[T_DEFENDER];
	for(int i=0;i<shipCount;i++)
	{
		if(!this->_UNITS[T_DEFENDER][i].exploded)
		{
			
			
			T_UNIT unit = {
				false,
				this->_UNITS[T_DEFENDER][i].shieldMax,
				this->_UNITS[T_DEFENDER][i].shieldMax,
				this->_UNITS[T_DEFENDER][i].hullLeft,
				this->_UNITS[T_DEFENDER][i].hullMax,
				this->_UNITS[T_DEFENDER][i].original,
				this->_UNITS[T_DEFENDER][i].owner,
			};
			
			newUnit[cnt++] = unit;
		}
		else
		{
			this->allShipCount[T_DEFENDER]--;
			this->_UNITS[T_DEFENDER][i].owner->shipList[this->_UNITS[T_DEFENDER][i].original->id]--;
		}
	}
	free(this->_UNITS[T_DEFENDER]);
	this->_UNITS[T_DEFENDER] = newUnit;
};

void CombatSys::Shoot(T_UNIT *source, T_UNIT *target, long *attackpower, long *absorbed, bool *nextshot)
{
	long		attack,
				shields,
				hull;

	float		dmgPrcnt;

	attack = source->original->attack * (1 + (float)(source->owner->techWeapon / 10));
	shields = target->shieldLeft;
	hull = target->hullLeft;

	//the 1%-rule
	if(target->shieldLeft == 0 || (float)(attack / (float)(target->shieldMax) > 0.01))
	{
		*attackpower += attack;
		//first the shields
		if(attack < shields)
		{
			//we didn't even touch the hull
			target->shieldLeft -= attack;
			*absorbed += attack;
		}
		else
		{
			attack -= target->shieldLeft;
			
			*absorbed += target->shieldLeft;
			target->shieldLeft = 0; //shieldsbroken
			

			target->hullLeft -= attack;

			dmgPrcnt = (float)((1 - target->hullLeft / (float)(target->hullMax))) * 100;
			if(dmgPrcnt > 30)
			{
				//target->exploded = true;
				target->exploded = this->_RNG.chanceDecide((int)(dmgPrcnt));
			}
		}
	}

	//rapidfire
    if(this->_ENABLE_RAPID_FIRE)
    {
    	int rf = source->original->rapidfire[target->original->id];
    	if(rf > 0)
    		*nextshot = this->_RNG.chanceDecide(rf,true);
    	else 
    		*nextshot = false;
    }
    else
        *nextshot = false;
}

char * CombatSys::getResult()
{
	if(!this->_CAN_GET_RESULT)
		return "{\"error\":1}";
	//pointer test
	//free(this->_UNITS);
	//free(this->_PLAYERS);
	//TODO: aproximate txt
	char	*txt,
			*tmp	
		;
	
	txt = (char *)malloc(30000 * sizeof(char));
	tmp = (char *)malloc(30000 * sizeof(char));
	/* Format example
	{"rounds" :
	[
		"ashot"		: 100,
		"adamage"	: 1000,
		"aabsorbed" : 200,
		"aunits" : [
			{"0" : 100, "1" : 20},
			{"11" : 50}
		],
		"dunits" : [{"13" : 10}]
	],...
	}
	*/

	strcpy(txt,"{\"rounds\" : [\n");

	if(this->roundCount == 0)
	{
		strcat(txt, "]}");
		return txt;
	}
	
	for(int round = 0; round<this->roundCount;round++)
	{
		strcat(txt,"	{\n");
		sprintf(tmp,"		\"ashot\":%i,\n		\"adamage\":%i,\n		\"aabsorbed\":%i,\n		\"dshot\":%i,\n		\"ddamage\":%i,\n		\"dabsorbed\":%i,\n		"
			,this->_ROUNDS[round].attackShots
			,this->_ROUNDS[round].attackDamage
			,this->_ROUNDS[round].attackAbsorbed
			,this->_ROUNDS[round].defendShots
			,this->_ROUNDS[round].defendDamage
			,this->_ROUNDS[round].defendAbsorbed
		);
		strcat(txt,tmp);


		//units
		strcat(txt,"\"aunits\":");
		strcat(txt,"\n			[");
		for(int j = 0; j<this->allPlayersCount[T_ATTACKER];j++)
		{

			strcat(txt,"\n				{");
            int cnt = 0;
			for(int k = 0;k<this->unitCount;k++)
			{
				if(this->_ROUNDS[round].shiplist[T_ATTACKER][j][k] > 0)
				{
					if(cnt > 0)
						strcat(txt,",");
					sprintf(tmp,"\n				\"%i\" : %i",k,this->_ROUNDS[round].shiplist[T_ATTACKER][j][k]);
					strcat(txt,tmp);
					cnt++;
				}

			}
			strcat(txt,"\n				}");
			if(j < (this->allPlayersCount[T_ATTACKER] - 1))
				strcat(txt,",");
		}
		strcat(txt,"\n			],\n		");

		strcat(txt,"\"dunits\":");
		strcat(txt,"\n			[");
		for(int j = 0; j<this->allPlayersCount[T_DEFENDER];j++)
		{
			
			strcat(txt,"\n				{");
            int cnt = 0;
			for(int k = 0;k<this->unitCount;k++)
			{
				if(this->_ROUNDS[round].shiplist[T_DEFENDER][j][k] > 0)
				{
					if(cnt > 0)
						strcat(txt,",");
					sprintf(tmp,"\n				\"%i\" : %i",k,this->_ROUNDS[round].shiplist[T_DEFENDER][j][k]);
					strcat(txt,tmp);
					cnt++;
				}

			}
			strcat(txt,"\n				}");
			if(j < (this->allPlayersCount[T_DEFENDER] - 1))
				strcat(txt,",");
		}
		strcat(txt,"\n			]");
		
		


		strcat(txt,"\n	}");
		if(round < (this->roundCount - 1))
			strcat(txt,",");		
		strcat(txt,"\n");
	}
	strcat(txt,"]\n");
	strcat(txt,"}");

	return txt;
}
//#endif