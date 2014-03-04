int launchScript(int argc, const char** argv);
int irr_test(int argc, const char** argv);
int cegui_test(int argc, const char** argv);
int loadIrrScene(int argc, const char** argv);
int main(int argc, const char** argv)
{
	// return loadIrrScene(argc, argv);
	// return irr_test(argc, argv);
	return launchScript(argc, argv);
}

