#include "../cppGameEngine/src/engine/GameEngine.h"

int main(int argc, char* argv[]) {
	
	GameEngine g("assets/config.txt");
	g.run();

	return 0;
}
