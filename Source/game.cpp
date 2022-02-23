#include "game.h"
#include "Screen.h"


Game::Game(Screen& screen)
	:screen(screen)
{

	target_tick = 60;
	curr_tick = target_tick - 1;
	spawn_time = 10;
	curr_time = spawn_time - 1;
	score = 0;
	is_playing = true;

}

void Game::update() {

	update_input();

	curr_tick++;

	if (curr_tick < target_tick) {
		return;
	}
	curr_tick = 0;
	
	snake.update(screen);

	for (int i = 0; i < snake_parts.size(); i++) {
		if (i == 0) {
			snake_parts[i].part_move(snake.get_next_position());
		}
		else {
			snake_parts[i].part_move(snake_parts[i - 1].get_next_position());
		}
	}


	curr_time++;

	if (curr_time >= spawn_time) {
		spawn_apple();
		curr_time = 0;
	}

	std::erase_if(apples, [](auto& apple) ->bool {return apple.get_state(); });

	handle_collision();

	if (snake.border(screen)) {
		gameover();
	}

}

void Game::draw() {

	if (is_playing)
	{
		snake.draw(screen);

		for (int i = 0; i < snake_parts.size(); i++) {
			snake_parts[i].draw(screen);
		}


		for (int i = 0; i < apples.size(); i++) {
			if (!apples[i].get_state()) {
				apples[i].draw(screen);
			}
		}
		screen.DrawText(300, 20, { 255,255,255 }, "Score: " + std::to_string(get_score()));
	}
	else
	{
		screen.DrawText(400, 500, { 255,0,0 }, "GAME OVER!");
		screen.DrawText(400, 550, { 255,255,255 }, "Score was: " + std::to_string(get_score()));
	}

}

void Game::spawn_apple() {
	Apple temp = Apple();
	temp.spawn(screen);
	apples.push_back(temp);
}

void Game::update_input() {
	if (screen.IsKeyDown(Key::Down)) {
		snake.Set_last_input(Key::Down);
	}
	if (screen.IsKeyDown(Key::Up)) {
		snake.Set_last_input(Key::Up);
	}
	if (screen.IsKeyDown(Key::Left)) {
		snake.Set_last_input(Key::Left);
	}
	if (screen.IsKeyDown(Key::Right)) {
		snake.Set_last_input(Key::Right);
	}
}

void Game::gameover() {
	this->is_playing = false;
}

void Game::handle_collision() {

	for (auto& first : snake_parts) {
		if (check_collision(first, snake)) {
			gameover();
		}
	}

	std::vector<Snake> temp_parts;

	for (auto& apple : apples) {
		if (check_collision(apple, snake)) {
			add_score(100);
			Snake temp = Snake();

			if (snake_parts.size() > 0) {
				temp.grow(snake_parts[snake_parts.size() - 1].get_position());
			} else {
				temp.grow(snake.get_position());
			}

			temp_parts.push_back(temp);

			apple.eaten();
		}
	}

	snake_parts.insert(snake_parts.end(), temp_parts.begin(), temp_parts.end());




}

bool Game::check_collision(gameObject& a, gameObject& b) {
	Position a_pos = a.get_position();
	Position b_pos = b.get_position();

	int a_size = a.get_size().size;
	int b_size = b.get_size().size;

	if (a_pos.x >= b_pos.x && a_pos.x <= b_pos.x + b_size) {
		if (a_pos.y >= b_pos.y && a_pos.y <= b_pos.y + b_size) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

void Game::add_score(int _value) {
	this->score += _value;
}

int Game::get_score() {
	return this->score;
}

int Game::tick_rate() {
	return this->target_tick;
}

bool Game::get_playing() {
	return this->is_playing;
}