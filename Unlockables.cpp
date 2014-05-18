#include <allegro5\allegro.h>
#include <string>

using namespace std;

class Unlocks
{
public:
	Unlocks();
	~Unlocks();

	void getUnlocksVenue(ALLEGRO_CONFIG *config, const char *section, const char *key) //reads config value from file
	{
		isunlocked = al_get_config_value(config, section, key);
	}

	void setUnlocksVenue(const char *vennum,int setval) //sets venue unlocks
	{
		if (setval == 1)
		{
			al_set_config_value(savegame, "venueunlocks", vennum, "1"); //sets values on the the venues only
		}
		else{
			al_set_config_value(savegame, "venueunlocks", vennum, "0");
		}


	}

private:
	string isunlocked = "0";
	string hiscore = "0";
	ALLEGRO_CONFIG *savegame;
	ALLEGRO_CONFIG *section;
	string venue = "unlocked";
};

Unlocks::Unlocks()	//default constructor sets everything to lock
{
	ALLEGRO_CONFIG *savegame = al_load_config_file("config.ini");	//name of config file for this game
	setUnlocksVenue("unlocked1");
}


Unlocks::~Unlocks()
{
	al_destroy_bitmap()
}


