#pragma once
#include "Screen.h"
#include "Snake.h"
#include "apple.h"
#include <vector>
#include <iostream>

class Game {
protected:
	int target_tick, curr_tick;
	float spawn_time, curr_time;
	int score;
	bool is_playing;
	Screen& screen;
	Snake snake;
	std::vector<Snake> snake_parts;
	std::vector<Apple> apples;
	bool check_collision(gameObject& a, gameObject& b);
	void handle_collision();
	void gameover();
	void spawn_apple();
	void add_score(int _value);
	int get_score();
public:
	Game(Screen& screen);
	void update();
	void draw();
	void update_input();
	int tick_rate();
	bool get_playing();
};