#pragma once
#include "gameObject.h"



class Snake : public gameObject {

private:
	int direction;
	Key last_input;
public:
	Snake();
	void update(Screen& screen);
	void part_move(Position next_pos);
	void grow(Position pos);
	void Set_last_input(Key new_input);
	Key get_last_input();
};
