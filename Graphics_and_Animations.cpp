/*
*	Abstract Base Class DynamicImg used as a template for images in the game which move across the screen.
*
*	Derived Classes from DynamicImg are: 
*	1) SimpleGraphic 
*	   simple graphics are single images that move across the screen
*	2) Animation
*	   animations cycle through a series of images with each refresh of the game screen, and move across the screen.
*
*	Old Version Structure	  |		New Version Class
*							  |
*	Bullet					  |		SimpleGraphic
*	Projectile				  |		SimpleGraphic
*	Boss					  |		SimpleGraphic
*	Explosion				  |		Animation
*	Character				  |		Animation
*
*	$	Big picture plan: Write a class for Bullet, Projectile, etc that inherits from 2 sources.
*			source 1 - a class derived from DynamicImg that handles the graphics-related aspects.
*			source 2 - a class derived from Gameplay that handles the aspects related to an object's role in the game.
*				       {Gameplay class structure not designed yet, but this way we're being good OOP programmers
*				        Keep graphics management and gameplay management separated and then write a class that incorporates both}
*
*	^ a $ indicates Sulaiman-style explanation talk, to be modified prior to submission
*/

class DynamicImg{
public: int x;			//x-coord relative to fixed top-left axis
		int y;			//y-coord relative to fixed top-left axis
		int live;		//indicates whether image is destroyed or not, to prevent memory leaks
						//$ may be able to make this a protected variable
		DynamicImg() {};
};

class Animation : public DynamicImg{
public: Animation(){};
		int maxFrame;
		int curFrame;
		int frameCount;
		int frameDelay;
		int frameWidth;
		int frameHeight;
		int animationColumns;
		int animationDirection;
		ALLEGRO_BITMAP *image;
};