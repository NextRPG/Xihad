#include <iostream>
#include <irrlicht/irrlicht.h>
#include <boost/scoped_ptr.hpp>
#include "CXMLFontWriter.h"
#include "CFreeTypeBitmapFactory.h"
#include "CBinaryFontWriter.h"

using namespace std;
using namespace boost;
using namespace irr;
using namespace xihad::font;

// Command line parameters
static const char* path = 0;

static u32 charWidth  = 20;
static u32 charHeight = 0;
static u32 imageWidth = 2048;
static u32 imageHeight= 1024;
static const char* out= "out";
static const char* fmt= "bmp";
static bool binary	  = true;
static bool useAlpha  = false;


class IrrlichtImageFactory : public IImageFactory
{
public:
	IrrlichtImageFactory(video::IVideoDriver* driver) : driver(driver) {}

	virtual video::IImage* createImage( ColorFormat format, core::dimension2du dimension, void* data ) 
	{
		return driver->createImageFromData(format, dimension, data);
	}


private:
	video::IVideoDriver* driver;
};

static IFontBitmapFactory* createBitmapFactory(const char* fontpath, bool alpha)
{
	CFreeTypeBitmapFactory* bitmapFactory = new CFreeTypeBitmapFactory(
		video::ECF_A8R8G8B8, alpha);
	bitmapFactory->setFontPath(fontpath);
	bitmapFactory->setCharSize(core::dimension2du(charWidth, charHeight));
	bitmapFactory->setImageSize(core::dimension2du(imageWidth, imageHeight));
	return bitmapFactory;
}

static void error(const char* msg)
{
	cerr << msg << endl;
	cout << "Usage: FontTool fontFilePath [arguments]\n";
	cout << "	-w		character width in pixel\n";
	cout << "	-h		character height in pixel\n";
	cout << "	-W		output image width in pixel\n";
	cout << "	-H		output image height in pixel\n";
	cout << "	-o		output file name\n";
	cout << "	-f		output image format\n";
	cout << "	-b(B)		output binary font file\n";
	cout << "	-a(A)		use alpha channel\n";
	exit(1);
}

static void readVaraibles(int argc, const char** argv)
{
	if (argc < 2)
		error("font path is required");

	path = argv[1];
	for (int i = 2; i < argc; ++i)
	{
		const char* arg = argv[i];
		if (strlen(arg) < 2)
			error("invalid argument");
		if (arg[0] != '-')
			error("invalid argument");

		const char* arg_ = &arg[2];
		switch (arg[1])
		{
		case 'w':
			charWidth = atoi(arg_);
			break;
		case 'h':
			charHeight = atoi(arg_);
			break;
		case 'W':
			imageWidth = atoi(arg_);
			break;
		case 'H':
			imageHeight = atoi(arg_);
			break;
		case 'o':
			out = arg_;
			break;
		case 'f':
			fmt = arg_;
			break;
		case 'b':
		case 'B':
			binary = (arg[1] == 'b');
			break;
		case 'a':
		case 'A':
			useAlpha = (arg[1] == 'a');
			break;
		default:
			error("Unrecognized argument");
		}
	}
}

IFontWriter* createFontWriter(IrrlichtDevice* device) 
{
	if (!binary)
		return new CXMLFontWriter(device, out, fmt, useAlpha);
	
	return new CBinaryFontWriter(device, out, fmt, useAlpha);
}

int main(int argc, const char** argv)
{
	readVaraibles(argc, argv);

	IrrlichtDevice* device =createDevice(video::EDT_NULL);
	SFontInfo info;
	try {
		scoped_ptr<IFontBitmapFactory> bitmapFactory(createBitmapFactory(path, useAlpha));
		scoped_ptr<IFontWriter> writer(createFontWriter(device));
		scoped_ptr<IImageFactory> imageFactory(new IrrlichtImageFactory(device->getVideoDriver()));

		bitmapFactory->createFontInfo(info, imageFactory.get());
		if (writer->write(info))
			cout << "Font file saved" << endl;
	} catch (std::exception& e) {
		cerr << e.what() << endl;
	}
	
	for (u32 i = 0; i < info.images.size(); ++i)
		info.images[i]->drop();

	device->drop();

	IRR_MLD_RELEASE_INSTANCE;
	system("pause");
	return 0;
}

