#include "gameObject.h"
#include "Screen.h"

int gameObject::last_id = 0;

gameObject::gameObject()
	:id(last_id++)
{
	dx = 0;
	dy = 0;
	position = { 0,0 };
	prev_position = { position };
	size = { 50 };

}

void gameObject::draw(Screen& screen) {

	screen.DrawRectangle(position.x + (50 - get_size().size) / 2, position.y + (50 - get_size().size) / 2, get_size().size, get_size().size, {color.r, color.b, color.g, color.a});
}

bool gameObject::border(Screen& screen) {

	if (position.x < 0) {
		return true;
	}
	if (position.x > screen.GetWindowWidth()) {
		return true;
	}
	if (position.y < 0) {
		return true;
	}
	if (position.y > screen.GetWindowHeight()) {
		return true;
	}
	else
	{
		return false;
	}
}

Position gameObject::get_position() {
	return this->position;
}
Position gameObject::get_next_position() {
	return this->prev_position;
}
Size gameObject::get_size() {
	return this->size;
}


