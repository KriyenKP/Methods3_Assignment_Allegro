/*	Unlockables.cpp - Lecturer Defence Game - Computer Methods 3 - ENEL3CC - UKZN 2014
Author: M. De Neef - 212503024
Date: 19/5/2014
Description:
Deals with the file system I/O for the file containing what additional features of the game have been unlocked
abstracts the processing from main. 

*/
#include <allegro5/allegro.h>					//Allegro libraries
#include <allegro5/allegro_native_dialog.h>		//Error message dialog library
#include <string>								//String manipulation 
#include <sstream>								//for int-> string 


using namespace std;

class Unlocks
{
public:
	ALLEGRO_DISPLAY			*display = NULL;	//message display feature object
	ALLEGRO_CONFIG *savegame = al_load_config_file("config.ini");	//name of config file for this game

	Unlocks()	//default constructor inits config file
	{

		initcheck(savegame);			//makes sure config file exists

		if (getHiscore() == 0)		//only resets values if user hasn't played before
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

	const char *getUnlocksVenue(int vennum) //queries the venue state, only need to send integer on game side
	{
		switch (vennum){
		case 1:	return al_get_config_value(savegame, "venueunlocks", "unlocked1"); //gets values of locked venues
		case 2:	return al_get_config_value(savegame, "venueunlocks", "unlocked2");
		case 3:	return al_get_config_value(savegame, "venueunlocks", "unlocked3");
		case 4:	return al_get_config_value(savegame, "venueunlocks", "unlocked4");
		case 5:	return al_get_config_value(savegame, "venueunlocks", "unlocked5");
		case 6:	return al_get_config_value(savegame, "venueunlocks", "unlocked6");
		}
	}

	const char *getUnlocksWeapons(int vennum) //queries the venue state, integer easy on game side
	{
		switch (vennum){
		case 2:	return al_get_config_value(savegame, "weaponunlocks", "unlocked2"); //checks weapons if unlocked
		case 3:	return al_get_config_value(savegame, "weaponunlocks", "unlocked3");

		}
	}

	const void setUnlocksVenue(const char *vennum,int setval) //game passes CHAR "unlockedX" and INT 1 or 0 , its converted from int to const char and stored in the file
	{
		if (setval == 1)		//if the value is a SET, write "1" to the venue key
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

	int getHiscore() //returns the high score in char form for display
	{
		hiscore = al_get_config_value(savegame, "highscore","playsc");
		stringstream str;

		str << hiscore;

		double intscore;		//convert int value sent to a char
		str >> intscore;
		return intscore;		//returns a char fr highscore
	}

	int setHiscore(int ps) //set the high score from char form , returns int for conditionals
	{
		char score[10];		//converts from int to char & stores
		sprintf_s(score, "%i", ps);
		al_set_config_value(savegame, "highscore", "playsc",score);
		return ps;
	}

	void Unlocks::initcheck(ALLEGRO_CONFIG *savefile) //throws a friendly error if the config file is AWOL
	{
		if (savefile == NULL)
		{
			al_show_native_message_box(display, "Error", "Config Load/Save failed!", "Check if config.ini exists in the game dir", NULL, ALLEGRO_MESSAGEBOX_WARN);
			al_destroy_display(display);

		}
	}; 

	void Unlocks::saveconfig(ALLEGRO_CONFIG *savefile)//run this to save the config file
	{
		al_save_config_file("config.ini", savegame);
	}

private:
	const char mutable *isunlocked ="0";	//flag for unlocks
	const char mutable *hiscore = "0";
	ALLEGRO_CONFIG *section;
};

