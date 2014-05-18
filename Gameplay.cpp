#include <Graphics_and_Animations.h>

class Gameplay{
public:
	static void detectCollision(DynamicImg*, DynamicImg*, void(*)());
};

void Gameplay::detectCollision(DynamicImg images1[], DynamicImg images2[], void(*doOnCollision)())
{
	const int images1_max = sizeof(images1) / sizeof(DynamicImg);
	const int images2_max = sizeof(images2) / sizeof(DynamicImg);
	for (int i = 0; i < images1_max; ++i)
	{
		if (images1[i].checkActive())
		{
			for (int j = 0; j < images2_max; ++j)
			{
				int x1 = images1[i].getX(),
					x2 = images2[j].getX(),
					x2_offset = images2[j].getBoundX(),
					y1 = images1[i].getY(),
					y2 = images2[j].getY(),
					y2_offset = images2[j].getBoundY();
				/*
				if (x1 > (x2 - x2_offset) && x1 < (x2 + x2_offset)
				&& y1 > (y2 - y2_offset) && y1 < (y2 + y2_offset))
				*/

				if (abs(x1 - x2) < x2_offset && abs(y1 - y2) < y2_offset)// double check my maths please
				{
					doOnCollision();
				}
			}
		}
	}
}