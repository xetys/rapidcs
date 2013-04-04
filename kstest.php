<?php
set_time_limit(0);

if(!class_exists("CombatSys"))
	die("extension 'combatsystem' not found");

$lcsInstance = new CombatSys();

echo "v:".$lcsInstance->getVersion()."<br>";

$lcsInstance->setShootAgainstExploded(false);
$lcsInstance->setRapidfire(true);

$lcsInstance->setUnitCount(22);

$lcsInstance->configShip(0,			5,			400,			10,			 "8:5;11:5;"									);	//Small Cargo
$lcsInstance->configShip(1,			5,			1200,			25,			 "8:5;11:5;"									);	//Large Cargo
$lcsInstance->configShip(2,			50,			400,			10,			 "8:5;11:5;"									);	//Light Fighter
$lcsInstance->configShip(3,			150,		1000,			25,			 "0:3;8:5;11:5;"								);	//Heavy Fighter
$lcsInstance->configShip(4,			400,		2700,			50,			 "2:6;18:5;11:5;14:10;"							);	//Cruiser
$lcsInstance->configShip(5,			1000,		6000,			200,		 "8:5;11:5;"									);	//BattleShip
$lcsInstance->configShip(6,			50,			3000,			100,		 "8:5;11:5;"									);	//Colonizer
$lcsInstance->configShip(7,			1,			1600,			10,			 "8:5;11:5;"									);	//Recycler
$lcsInstance->configShip(8,			0,			100,			1,			 ""												);	//Spy Probe
$lcsInstance->configShip(9,			1000,		7500,			500,		 "8:5;11:5;14:20;15:20;16:10;17:10;"			);	//Bomber
$lcsInstance->configShip(10,			5,			400,			10,			 ""												);	//Solarsatellit
$lcsInstance->configShip(11,			2000,		11000,			500,		 "9:5;12:5;14:2;17:10;"							);	//Destroyer
$lcsInstance->configShip(12,			200000,		900000,			50000,		 "0:250;1:250;2:200;3:100;4:33;5:30;6:250;7:250;8:1250;9:25;10:1250;11:5;13:15;14:200;15:100;16:50;17:100;");		//Death Star
$lcsInstance->configShip(13,			700,		7000,			400,		 "0:3;1:3;2:4;4:4;5:7;8:5;11:5;"				);	//Battle Cruiser
$lcsInstance->configShip(14,			80,			200,			20,			 ""			);	//Rocket Launcher
$lcsInstance->configShip(15,			100,		200,			25,			 ""			);	//Light Laser
$lcsInstance->configShip(16,			250,		800,			100,		 ""			);	//Heavy Laser
$lcsInstance->configShip(17,			1100,		3500,			200,		 ""			);	//Gauscanon
$lcsInstance->configShip(18,			150,		800,			500,		 ""			);	//Ionturret
$lcsInstance->configShip(19,			3000,		10000,			300,		 ""			);	//Plasmaturret
$lcsInstance->configShip(20,			1,			2000,			2000,		 ""			);	//Small Shield
$lcsInstance->configShip(21,			1,			10000,			10000,		 ""			);	//Big shield


$lcsInstance->addAttacker(0,16,16,16);
$lcsInstance->addAttacker(1,16,16,16);
$lcsInstance->addAttacker(2,15,15,15);
$lcsInstance->addAttacker(3,16,16,16);


$lcsInstance->addDefender(0,16,15,16);


//Slot 1
$lcsInstance->addAttackShips(0,11,10000);

//Slot 2
$lcsInstance->addAttackShips(1,4,48600);
$lcsInstance->addAttackShips(1,5,9500);
$lcsInstance->addAttackShips(1,13,30500);

//Slot 3
$lcsInstance->addAttackShips(2,4,10000);
$lcsInstance->addAttackShips(2,13,10000);

//Slot 4
$lcsInstance->addAttackShips(3,2,252000);

//Defender;
//8.191    1.500	117.221	426	13.416
//4.505    2	12.950	482	1.214	476	6.264	43	7.185	2.480	1.193	19	80	31	1	1
$lcsInstance->addDefendShips(0,0,8191);
$lcsInstance->addDefendShips(0,1,1500);
$lcsInstance->addDefendShips(0,2,117221);
$lcsInstance->addDefendShips(0,3,464);
$lcsInstance->addDefendShips(0,4,13416);
$lcsInstance->addDefendShips(0,5,4505);
$lcsInstance->addDefendShips(0,6,2);
$lcsInstance->addDefendShips(0,7,12950);
$lcsInstance->addDefendShips(0,8,482);
$lcsInstance->addDefendShips(0,9,1214);
$lcsInstance->addDefendShips(0,10,476);
$lcsInstance->addDefendShips(0,11,6264);
$lcsInstance->addDefendShips(0,12,43);
$lcsInstance->addDefendShips(0,13,7185);
$lcsInstance->addDefendShips(0,14,2480);
$lcsInstance->addDefendShips(0,15,1193);
$lcsInstance->addDefendShips(0,17,19);
$lcsInstance->addDefendShips(0,18,80);
$lcsInstance->addDefendShips(0,19,31);
$lcsInstance->addDefendShips(0,20,1);
$lcsInstance->addDefendShips(0,21,1);


$start = microtime(true);
$lcsInstance->battle();
$total = microtime(true) - $start;

$lsResult = $lcsInstance->getResult();

$laResults = (array)json_decode($lsResult,true);

echo "Berechnet in: $total Sek,<br>";
echo "<pre>".var_export($laResults,true)."</pre>";
echo "<pre>".$lsResult."</pre>";
