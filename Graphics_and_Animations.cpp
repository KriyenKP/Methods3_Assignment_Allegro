#include <allegro5\allegro.h>
#include <Graphics_and_Animations.h>

//DynamicImg
DynamicImg::DynamicImg(int bX = 0, int bY = 0)
: boundX(bX),
  boundY(bY)
{

}
void DynamicImg::setX(int xVal)
{
	x = xVal;
}
void DynamicImg::setY(int yVal)
{
	y = yVal;
}
void DynamicImg::setActive(bool isActive)
{
	active = isActive;
}
bool DynamicImg::checkActive() const
{
	return active;
}
int DynamicImg::getX() const
{
	return x;
}
int DynamicImg::getY() const
{
	return y;
}
void DynamicImg::draw()
{}
int DynamicImg::getBoundX() const
{
	return boundX;
}
int DynamicImg::getBoundY() const
{
	return boundY;
}

//SimpleGraphic Methods

void SimpleGraphic::toggleActive()
{
	active = !active;
}
void SimpleGraphic::draw(ALLEGRO_BITMAP *bitmap)
{
	if (active)
		al_draw_bitmap(bitmap, x, y, 0);
}
void SimpleGraphic::draw(ALLEGRO_BITMAP *bitmap, int cur, int fW, int fH)
{
	if (active)
		al_draw_bitmap_region(bitmap, cur * fW, 0, fW, fH, x, y, 0);
}