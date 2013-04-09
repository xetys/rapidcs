Class documentation


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


David Steiman (c) 2013