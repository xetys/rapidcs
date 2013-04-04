
const unsigned T_ATTACKER = 0;
const unsigned T_DEFENDER = 1;
const unsigned MAX_SLOTS = 100;

struct T_SHIP {
	int id;
	long attack;
	long hull;
	long shield;
	int *rapidfire;
};

 struct T_PLAYER {
	int playerId;
	int techWeapon;
	int techShield;
	int techArmor;
	int *shipList;
};

class T_UNIT {
	public:
	bool exploded;
	int shieldLeft;
	int shieldMax;
	int hullLeft;
	int hullMax;
	T_SHIP *original;
	T_PLAYER *owner;
};

 struct T_ROUND {
	long attackShots;
	long attackDamage;
	long attackAbsorbed;

	long defendShots;
	long defendDamage;
	long defendAbsorbed;

	T_PLAYER *thisRoundData[2];
	int **shiplist[2];
};

class RNG
{
	
	
	public:
	RNG();

	bool chanceDecide(int chance);

	bool chanceDecide(int chance, bool toPerMille);
	
	int getRandom(int a, int b);
};
class CombatSys
{
	private:
		T_SHIP		* _SHIPS;
		T_PLAYER	*_PLAYERS[2];
		T_UNIT		*_UNITS[2];
		int			unitCount;	
		int			unitsSet;
		int			allShipCount[2]; //for allocating memory
		int			allPlayersCount[2];
		RNG			_RNG;
		int			roundCount;
		T_ROUND		_ROUNDS[6];
		int 		_VERSION;
		bool 		_CAN_BATTLE;
		bool 		_CAN_GET_RESULT;
        bool        _SHOOT_AGAINST_EXPLODED;
        bool        _ENABLE_RAPID_FIRE;
	public:
		CombatSys();

		int getVersion();
        
        void setShootAgainstExploded(bool modus);
        
        void setRapidfire(bool modus);

		void setUnitCount(int count);

		bool canSetShip(int id);

		void configShip(int id, long attack, long hull, long shield, char *rapidfire);

		bool allShipsConfigured();
		
		bool canSetAttacker(int id);
		
		void addAttacker(int id, int weapon, int shield, int armor);

		bool canSetDefender(int id);
		
		void addDefender(int id, int weapon, int shield, int armor);

		void addShips(const unsigned fraction, int playerId, int shipId, int amount);

		void UpdateFleets();

		void Shoot(T_UNIT *source, T_UNIT *target, long *attackpower, long *absorbed, bool *nextshot);

		void battle();

		char * getResult();
};
