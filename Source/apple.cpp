#include "Apple.h"


Apple::Apple() : gameObject()
{
	is_eaten = false;
	position = { 0, 0 };
	color = { 255,0,0,255 };
	size = { 10 };
}

void Apple::spawn(Screen& screen) {

	int x_offset = screen.GetWindowWidth() / 50;
	int y_offset = screen.GetWindowHeight() / 50;

	int x_index = rand() % x_offset;
	int y_index = rand() % y_offset;

	position.x = x_index * 50;
	position.y = y_index * 50;

}

void Apple::eaten() {
	this->is_eaten = true;
}

bool Apple::get_state() {
	return this->is_eaten;
}