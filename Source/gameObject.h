#pragma once
#include "Screen.h"
#include <iostream>

struct Position {

	int x, y;

};

struct Size {
	int size;
};

struct object_color {
	unsigned char r, b, g, a = 255;
};

class gameObject {
protected:
	Position position;
	Position prev_position;
	Size size;
	object_color color;
	int id;
	static int last_id;
	float dx, dy;
public:
	gameObject();
	void draw(Screen& screen);
	bool border(Screen& screen);
	Position get_position();
	Position get_next_position();
	Size get_size();

};