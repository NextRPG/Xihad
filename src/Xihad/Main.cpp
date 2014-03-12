#include "Engine/MemoryLeakDetector.h"
#include <irrlicht/CMemoryLeakDetector.h>
#include <cstdlib>

int launchScript(int argc, const char** argv);
int irr_test(int argc, const char** argv);
int cegui_test(int argc, const char** argv);
int loadIrrScene(int argc, const char** argv);
int rt_test(int argc, const char** argv);

int main(int argc, const char** argv)
{
	// return loadIrrScene(argc, argv);
	
	// return irr_test(argc, argv);
	
	launchScript(argc, argv);
	
	// rt_test(argc, argv);
	
	XIHAD_MLD_RELEASE_INSTANCE;
	IRR_MLD_RELEASE_INSTANCE;
	system("pause");
	return 0;
}

