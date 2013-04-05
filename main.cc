/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * main.cc
 * Copyright (C) David Steiman 2013 <>
 * 
 * CombatSystem is free software copyrighted by David Steiman.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name ``David Steiman'' nor the name of any other
 *    contributor may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 
 * CombatSystem IS PROVIDED BY David Steiman ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL David Steiman OR ANY OTHER CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include "combat.cc"

int main()
{
	
	CombatSys lcsInstance;

	lcsInstance.setUnitCount(22);
	
	lcsInstance.configShip(0,			5,			400,			10,			 "8:5;11:5;"									);	//Small Cargo
	lcsInstance.configShip(1,			5,			1200,			25,			 "8:5;11:5;"									);	//Large Cargo
	lcsInstance.configShip(2,			50,			400,			10,			 "8:5;11:5;"									);	//Light Fighter
	lcsInstance.configShip(3,			150,		1000,			25,			 "2:4;8:5;11:5;"								);	//Heavy Fighter
	lcsInstance.configShip(4,			400,		2700,			50,			 "2:6;18:5;11:5;14:10;"							);	//Cruiser
	lcsInstance.configShip(5,			1000,		6000,			200,		 "8:5;11:5;"									);	//BattleShip
	lcsInstance.configShip(6,			50,			3000,			100,		 "8:5;11:5;"									);	//Colonizer
	lcsInstance.configShip(7,			1,			1600,			10,			 "8:5;11:5;"									);	//Recycler
	lcsInstance.configShip(8,			0,			100,			1,			 ""												);	//Spy Probe
	lcsInstance.configShip(9,			1000,		7500,			500,		 "8:5;11:5;14:20;15:20;16:10;17:10;"			);	//Bomber
	lcsInstance.configShip(10,			5,			400,			10,			 ""												);	//Solarsatellit
	lcsInstance.configShip(11,			2000,		11000,			500,		 "9:5;12:5;14:2;17:10;"							);	//Destroyer
	lcsInstance.configShip(12,			200000,		900000,			50000,		 "0:250;1:250;2:200;3:100;4:33;5:30;6:250;7:250;8:1250;9:25;10:1250;11:5;13:15;14:200;15:100;16:50;17:100;");		//Death Star
	lcsInstance.configShip(13,			700,		7000,			400,		 "0:3;1:3;2:4;4:4;5:7;8:5;11:5;"				);	//Battle Cruiser
	lcsInstance.configShip(14,			80,			200,			20,			 ""			);	//Rocket Launcher
	lcsInstance.configShip(15,			100,		200,			25,			 ""			);	//Light Laser
	lcsInstance.configShip(16,			250,		800,			100,		 ""			);	//Heavy Laser
	lcsInstance.configShip(17,			1100,		3500,			200,		 ""			);	//Gauscanon
	lcsInstance.configShip(18,			150,		800,			500,		 ""			);	//Ionturret
	lcsInstance.configShip(19,			3000,		10000,			300,		 ""			);	//Plasmaturret
	lcsInstance.configShip(20,			1,			2000,			2000,		 ""			);	//Small Shield
	lcsInstance.configShip(21,			1,			10000,			10000,		 ""			);	//Big shield
	
	bool hardcoreTest = true;

	if(hardcoreTest)
	{
		lcsInstance.addAttacker(0,16,16,16);
		lcsInstance.addAttacker(1,16,16,16);
		lcsInstance.addAttacker(2,15,15,15);
		lcsInstance.addAttacker(3,16,16,16);


		lcsInstance.addDefender(0,16,15,16);

		//Slot 1
		lcsInstance.addShips(T_ATTACKER,0,11,10000);

		//Slot 2
		lcsInstance.addShips(T_ATTACKER,1,4,48600);
		lcsInstance.addShips(T_ATTACKER,1,5,9500);
		lcsInstance.addShips(T_ATTACKER,1,13,30500);

		//Slot 3
		lcsInstance.addShips(T_ATTACKER,2,4,10000);
		lcsInstance.addShips(T_ATTACKER,2,13,10000);

		//Slot 4
		lcsInstance.addShips(T_ATTACKER,3,2,252000);

		//Defender;
		//8.191    1.500	117.221	426	13.416
		//4.505    2	12.950	482	1.214	476	6.264	43	7.185	2.480	1.193	19	80	31	1	1
		lcsInstance.addShips(T_DEFENDER,0,0,8191);
		lcsInstance.addShips(T_DEFENDER,0,1,1500);
		lcsInstance.addShips(T_DEFENDER,0,2,117221);
		lcsInstance.addShips(T_DEFENDER,0,3,464);
		lcsInstance.addShips(T_DEFENDER,0,4,13416);
		lcsInstance.addShips(T_DEFENDER,0,5,4505);
		lcsInstance.addShips(T_DEFENDER,0,6,2);
		lcsInstance.addShips(T_DEFENDER,0,7,12950);
		lcsInstance.addShips(T_DEFENDER,0,8,482);
		lcsInstance.addShips(T_DEFENDER,0,9,1214);
		lcsInstance.addShips(T_DEFENDER,0,10,476);
		lcsInstance.addShips(T_DEFENDER,0,11,6264);
		lcsInstance.addShips(T_DEFENDER,0,12,43);
		lcsInstance.addShips(T_DEFENDER,0,13,7185);
		lcsInstance.addShips(T_DEFENDER,0,14,2480);
		lcsInstance.addShips(T_DEFENDER,0,15,1193);
		lcsInstance.addShips(T_DEFENDER,0,17,19);
		lcsInstance.addShips(T_DEFENDER,0,18,80);
		lcsInstance.addShips(T_DEFENDER,0,19,31);
		lcsInstance.addShips(T_DEFENDER,0,20,1);
		lcsInstance.addShips(T_DEFENDER,0,21,1);
	}
	else
	{
		lcsInstance.addAttacker(0,16,16,16);
		lcsInstance.addDefender(0,16,16,16);

		lcsInstance.addShips(T_ATTACKER,0,13,10);
		lcsInstance.addShips(T_DEFENDER,0,2,20);
	}
	

	lcsInstance.battle();

	printf(lcsInstance.getResult());
	return 0;
}
