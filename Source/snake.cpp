#include "Snake.h"


Snake::Snake() : gameObject()
{
	direction = 1;
	position = { 500, 500 };
	prev_position = { -100, -100 };
	size = { 48 };
	color = { 0,255,0,255 };
	last_input = Key::Up;
}

void Snake::update(Screen& screen) {

	prev_position = { position.x, position.y };

	int new_dir = direction;

	if (get_last_input() == Key::Up && direction != 2) {
		new_dir = 1;
	}
	if (get_last_input() == Key::Down && direction != 1) {
		new_dir = 2;
	}
	if (get_last_input() == Key::Left && direction != 4) {
		new_dir = 3;
	}
	if (get_last_input() == Key::Right&& direction != 3) {
		new_dir = 4;
	}

	direction = new_dir;

	switch (direction)
	{
	case 1:
		position.y -= 50;
		break;
	case 2:
		position.y += 50;
		break;
	case 3:
		position.x -= 50;
		break;
	case 4:
		position.x += 50;
		break;
	default:
		position.x += 50;
		break;
	}

}

void Snake::part_move(Position next_pos) {
	prev_position = { position.x, position.y };

	position.x = next_pos.x;
	position.y = next_pos.y;
}

void Snake::grow(Position pos) {
	position = pos;
}

void Snake::Set_last_input(Key new_input) {
	this->last_input = new_input;
}

Key Snake::get_last_input() {
	return this->last_input;
}