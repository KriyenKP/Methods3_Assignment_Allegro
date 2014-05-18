#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <string>


using namespace std;

class Unlocks
{
public:
	ALLEGRO_DISPLAY			*display = NULL;	//message display feature
	ALLEGRO_CONFIG *savegame = al_load_config_file("config.ini");	//name of config file for this game

	Unlocks()
	{

		initcheck(savegame);			//makes sure config file exists

		if (strcmp(getHiscore(), "0") == 0)		//only resets values if user hasn't played before
		{

			setUnlocksVenue("unlocked1", 1);		//write config file to default values
			setUnlocksVenue("unlocked2", 0);	//uses int for condition--Allegro likes const char*
			setUnlocksVenue("unlocked3", 0);
			setUnlocksVenue("unlocked4", 0);
			setUnlocksVenue("unlocked5", 0);
			setUnlocksVenue("unlocked6", 0);

			setUnlocksWeapon("unlocked2", 0);
			setUnlocksWeapon("unlocked3", 0);
			setUnlocksWeapon("unlocked1", 1);	//do the same for the weapons--leave one unlocked
		}

		saveconfig(savegame); //save what you've just done...
	};
	
	//~Unlocks();

	const void setUnlocksVenue(const char *vennum,int setval) //game passes CHAR "unlockedX" and INT 1 or 0 , its converted from int to const char and stored in the file
	{
		if (setval == 1)
		{
			isunlocked = "1";
			al_set_config_value(savegame, "venueunlocks", vennum, isunlocked); //sets values on the the venues only
		}
		else{
			isunlocked = "0";
			al_set_config_value(savegame, "venueunlocks", vennum, isunlocked);//keeps it locked
		}
	}

	const void setUnlocksWeapon(const char *atknum, int setval) //same as venues
	{
		if (setval == 1)
		{
			isunlocked = "1";
			al_set_config_value(savegame, "weaponunlocks", atknum, isunlocked); 
		}
		else if (setval == 0){
			isunlocked = "0";
			al_set_config_value(savegame, "weaponunlocks", atknum, isunlocked);//keeps it locked
		}
		
	}

	const char* getHiscore() //returns the high score in char form for display
	{
		hiscore = al_get_config_value(savegame, "highscore","playsc");
		return hiscore; 
	}

	void Unlocks::initcheck(ALLEGRO_CONFIG *savefile) //throws a friendly error if the config file is AWOL
	{
		if (savefile == NULL)
		{
			al_show_native_message_box(display, "Error", "Config Load/Save failed!", "Check if config.ini exists in the game dir", NULL, ALLEGRO_MESSAGEBOX_WARN);
			al_destroy_display(display);

		}
	}; 

	void Unlocks::saveconfig(ALLEGRO_CONFIG *savefile)
	{
		al_save_config_file("config.ini", savefile);
	}//run this to save the config file

private:
	const char mutable *isunlocked ="0";
	const char mutable *hiscore = "0";
	;
	ALLEGRO_CONFIG *section;
};


////check for unlocks
//if (ps > 10)
//{
//	al_set_config_value(savegame, "venueunlock 2", "unlocked", "1"); //unlock TB Davis
//	if (al_get_config_value(savegame, "highscore", "playsc")<score) //only shows if not unlocked before
//	{
//		al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 380, 0, "NEW UNLOCKS: TB Davis!!");
//	}
//
//}
//if (ps > 20)
//{
//	al_set_config_value(savegame, "venueunlock 3", "unlocked", "1");//unlock Park
//	if (al_get_config_value(savegame, "highscore", "playsc")<score) //only shows if not unlocked before
//	{
//		al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 400, 0, "NEW UNLOCKS: Park!");
//	}
//}
//if (ps > 30)
//{
//	al_set_config_value(savegame, "venueunlock 4", "unlocked", "1");//unlock Science
//	al_set_config_value(savegame, "weaponunlocks", "unlocked2", "1");//unlock Pencil
//	if (al_get_config_value(savegame, "highscore", "playsc")<score) //only shows if not unlocked before
//	{
//		al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 420, 0, "NEW UNLOCKS: Science & Pencil!");
//	}
//}
//if (ps > 40)
//{
//	al_set_config_value(savegame, "venueunlock 5", "unlocked", "1");//Unlock Cafe
//
//	if (al_get_config_value(savegame, "highscore", "playsc")<score) //only shows if not unlocked before
//	{
//		al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 440, 0, "NEW UNLOCKS: Cafe!");
//	}
//}
//
//if (ps > 50)
//{
//	al_set_config_value(savegame, "venueunlock 6", "unlocked", "1");//unlock Amphitheatre
//	al_set_config_value(savegame, "weaponunlocks", "unlocked3", "1");//unlock C++
//
//	if (al_get_config_value(savegame, "highscore", "playsc") < score) //only shows if not unlocked before
//	{
//		al_draw_textf(fonts[0], green, scrn_W / 2 - 100, 460, 0, "NEW UNLOCKS: Amphitheatre & C++");
//	}
//}
//al_save_config_file("config.ini", savegame);