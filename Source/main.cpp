//Mikkel Jonas Sørensen

#include "Screen.h"
#include "game.h"

int screenWidth = 1000;
int screenHeight = 1000;

int main(int argc, char** argv)
{

	srand((unsigned int)time(NULL));

	Screen screen(screenWidth, screenHeight, "Galba");
	Game game(screen);

 	while (screen.IsOpen()) {
		screen.Clear();
		if (game.get_playing()) {
			game.update();
		}
		game.draw();

		screen.Display();
	}
	return 0;
}