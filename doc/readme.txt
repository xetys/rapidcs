Contents
__________
1. Introduction
2. Requirements
3. Installing
4. Class documentation
5. Notes




1. Introduction
_______________

Welcome to the quick documentation of RapidCS.

RapidCS was developed as an experiment and for teaching myself the languages C/C++


RapidCS is a combat system like OGame. The battle algorithm was implemented like the official description on OWiki.
The aim of RapidCS is maximum of compability to any games running in PHP. Thats why the system finally runs as en PHP extension,
and can be called inside PHP.

For the demonstration of how the system can be installed in a running codebase, this projects comes with a module
for the OpenSource Projekt 2Moons, where it was tested in many situations.


2. Requirements
_______________
For building the code base you need these packages:

-php5-dev
-g++

Please note, that the this combatsystem was only tested on debian 6 Squeeze and Ubuntu10 LTS.
But I am sure, at will work also in other linux distributions.


3.1 Installing the extension
___________________________
Extract the files from root directory into any directory. 
Then execute this 4 statements:

phpize
./configure --enable-combatsystem
make
sudo make install

The last command will automatically install the extension into your modules directory of PHP5. To proof, that the installation 
was done successfully, test with:

php -d"extension=combatsystem.so" -m

Now see a list of enabled extension, and if you find "combatsystem", the build was successfully.

Now you have to change the "php.ini" files of your server, by adding the following line at the bottom of the file:
extension=combatsystem.so

The installtion takes effect, after a restart of your http-service.

3.2 Testing the combatsystem
____________________________
The file kstest.php includes the default procudure of communication to RapidCS. 
If everything was done right, you will see the inner version of the CombatSystem, an created array and the original
JSON output from the system.


3.3 Installing the system in a running code base
_______________________________________________

RapidCS do only calculate the battleresult, and gives back the rounddata. Any other Information have to be created by
your game. For exmaple you can try RapidCS on 2Moons.

Get the latest version of 2Moons from http://2moons.cc and copy (overwrite) the files from the directory
2Moons_v1.7.

ATTENTION: No warranty for any diseases caused by RapidCS. If you install RapidCS on a running and allready modified 2Moons,
it is strictly recommend, to modify your game step by step.


4. Class documentation
_____________________

CombatSys()			The default constructor does not accept any arguments

setRapidfire(bool)		Enables or disables the rapidfire functionality of the combatsystem. This method cann be called optional.
Default: true


setShootAgainstExploded(bool) 	if this is enabled, the algorithm will shoot randomly against all units during a round, no matter
				whether the units is exploded or not. When this mode is disabled, the algorithm sorts per shoot the 
				unit array by the explosion flag, and targets only non-exploded ships. This mode differs from the 
				original OGame (R) system, but increases the calculation by 400 percent.
				This method can be called optional.
Default:false

setUnitCount(int)		This method must be called at the top, to tell the System, how many types of Ships/Defense the System
				will have. This is necessary for memory allocation.

configShip(	int id,
		int attack,
		int hull
		int shield
		string rf
	)			Exports UnitData to the system. The ids MUST increment beginning by 0. For the combatsystem there is no
				difference between ships and defense. 
				ATTENTION: The rapidfire config has a strict syntax! e.g.: id:amount;id2:amount2;
				Any errors in this configuration may cause crashes. Check this syntax twice, and also the id matching.

setAttacker(	int id,
		int weapon,
		int shield,
		int armor
	)			Exports a attackers slot with its techs to the system. The ids must be incremented beginning by 0.
				More then 2 fleet per player apears like 2 players with the same techs for the system.

setDeffender(	in id,
		int weapon,
		int shield,
		int armor
	)			The analogy to setAttacker. Attackers and defenders have different id-scopes. The first
				defender has to start by 0, and must be also incremented.

setAttackShips(	int playerid,
		int shipid,
		int amount
	)			Sets the ship per player of attackers and per type. Use the above defined ids.

setDefendShips(	int playerid,
		int shipid,
		int amount
	)			Same like setAttackShips for defenders.

battle()			This method is automatically enabled, when all obligatory methods above were callen in the right
				order. Call this method once!

string getResult()		Returns the result in JSON format. See kstest.php for the format specification.

int getVersion()		Returns the inner Version of the system.



5. Notes
_______________

Currently this system calculates a real simulation. That's why it is not possible to use it for large fleet amounts.
We work on a propability theory solution. Until we have no update in this direction, we recommend to use this system
for maximum 5-speed ogame like systems!








David Steiman (c) 2013