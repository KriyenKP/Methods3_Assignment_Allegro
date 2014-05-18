#include <Graphics_and_Animations.h>
#include <allegro5\allegro.h>
#include <Gameplay.h>

/*
*	Game comprises of 5 general elements:
*		1) Avatar (prev Character). User-controlled.
*		2) Enemy (prev Projectile). Standard opponent.
*		3) Boss. Opponent to be destroyed at end of level.
*		4) Bullet. Thrown by Avatar when attacking Enemy/Boss
*		5) Explosion. Occurs when Bullet and Enemy/Boss collide
*/

class Avatar : public Animation, public Gameplay
{

};