#include <iostream>
#include <ctime>
#include <cmath>
#include <irrlicht/irrlicht.h>
#include "TestRouting.h"
#include "MaxCostTarget.h"

using namespace std;
using namespace irr;
u32 segment = 8;
#define IMAGE_PATH "../Xihad/Assets/gfx/skydome.jpg"

extern u32 minLightness = 255;
extern double avgLightness = 0;
void segmentImage(video::IImage* image)
{
	for (u32 x = 0; x < image->getDimension().Width; ++x)
	{
		for (u32 y = 0; y < image->getDimension().Height; ++y)
		{
			u32 lightness = (u32)image->getPixel(x, y).getLightness();
			lightness = (lightness / (255/segment)) * (255/segment);
			if (lightness == 0) lightness++;

			avgLightness += lightness;
			if (lightness < minLightness) 
				minLightness = lightness;

			image->setPixel(x, y, video::SColor(255, lightness, lightness, lightness));
		}
	}
	avgLightness /= image->getDimension().getArea();
}

#define FORWARD
// #define BACKWARD
void testRouting(video::IImage* image)
{
	MyGraph graph(image);
	AStarRouter<Vec2, double> router(&graph);

	Vec2 start(30, 5);
	Vec2 target(500, 300);//image->getDimension().Width/2, image->getDimension().Height/2);

#ifdef FORWARD
	std::list<Vec2> out1;
	{
		out1.clear();
		double cost = router.route(start, MyGoal(target), &out1);
		cout << "Forward cost: " << cost << endl;
	}
#endif

#ifdef BACKWARD
	std::list<Vec2> out2;
	{
		out2.clear();
		clock_t t1 = clock();
		double cost = router.route(target, MyGoal(start), &out2);
		clock_t dt = clock() - t1;
		cout << "Backward Time: " << dt << ", cost: " << cost << endl;
	}
#endif

#ifdef FORWARD
	for (Vec2 node : out1)
		image->setPixel(node.X-1, node.Y-1, video::SColor(255,255,0,0));
#endif

#ifdef BACKWARD
	for (Vec2 node : out2)
		image->setPixel(node.X-1, node.Y-1, video::SColor(255,0,255,0));
#endif
}

void testMark(video::IImage* image)
{
	MyGraph graph(image);
	AStarRouter<Vec2, double> router(&graph);
	MaxCostTarget target(72230);
	router.route(Vec2(30,5), target, (std::list<Vec2>*)0);

	for (Vec2 p : target.all)
	{
		video::SColor color = image->getPixel(p.X, p.Y);
		// color.setAlpha(128);
		image->setPixel(p.X, p.Y, 0xffff0000);//color);
	}

	if (target.all.count(Vec2(500, 300)) && !target.all.count(Vec2(501,301)))
	{
		cout << "500, 300 is boundry" << endl;
	}

	target.all.clear();
}

int main(int argc, char **argv)
{
	IrrlichtDevice* device = createDevice(video::EDT_OPENGL, 
		core::dimension2du(1280, 720));

	video::IVideoDriver* driver = device->getVideoDriver();
	video::IImage* image = driver->createImageFromFile(IMAGE_PATH);
	segmentImage(image);

	clock_t t1 = clock();
	// testRouting(image);
	testMark(image);
	clock_t dt = clock() - t1;
	cout << "Time: " << dt << endl;

	video::ITexture* tex = driver->addTexture("@", image);
	while (device->run())
	{
		driver->beginScene();
		driver->draw2DImage(tex, core::position2di());
		driver->endScene();
	}
	
	system("pause");
	return 0;
}

