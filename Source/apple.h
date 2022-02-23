#pragma once
#include "gameObject.h"

class Apple : public gameObject {

private:
	bool is_eaten;
public:
	Apple();
	void spawn(Screen& screen);
	void eaten();
	bool get_state();
};