#include "Environment.hpp"

int main()
{
	world::nature::Environment env;

	env.initApp();
	env.getRoot()->startRendering();
	env.closeApp();

	return 0;
}