
float crs_x = scrn_W / 2.0;										//default x location for mouse position detection
float crs_y = scrn_H / 2.0;										//default y location for mouse position detection



int playone = 1;												//restrict sound to play only once
int sign = 1;													//direction for boss movement
bool bosslevel = false;											//only let boss come after certain score
bool win = 0;
bool SecLife = false;
int poweredNum = 0;
int level = 0;

int bossCheck = 0;

int egg = 0;

//animated image var
int curFrame = 0;					//Current frame of animated image
int frameCount = 0;					//frame counter for animated image
int frameDelay = 20;				//rate at which animate image changes
int frameW = 128;					//frame width for animated image
int frameH = 128;					//frame height for animated image
const int maxFrame = 4;				//number of frames in animated image
//End animated image var
