#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "snake.h"

class Board {
private:
	bool allow_direction_change;
	int x_food, y_food;

	void _initialize_pixel();
	void _initialize_snake();
	void _initialize_wall();
	void _clear_board();
	void _sync_board();
	bool _check_collision();
	bool _check_food_collision();
	void _generate_food();

public:
	std::vector<std::vector<int>> pixel;
	std::vector<int>x_wall, y_wall;
	int x_size, y_size, score;
	bool alive;	
	Snake snake;

	Board(int x_size_, int y_size_);
	void move_snake();
	void change_snake_direction(int new_direction);
};

Board::Board(int x_size_, int y_size_) {
	srand(time(NULL));

	this -> x_size = x_size_;
	this -> y_size = y_size_;

	this -> _initialize_pixel();
	this -> _initialize_snake();
	this -> _initialize_wall();

	this -> alive = true;
	this -> allow_direction_change = true;
	this -> score = 0;

	this -> _generate_food();

	this -> _sync_board();
}

void Board::_generate_food() {
	int x = rand() % (this -> x_size - 2) + 1;
	int y = rand() % (this -> y_size - 2) + 1;

	while (this -> pixel[x][y] == 1 || this -> pixel[x][y] == 2) {
		x = rand() % (this -> x_size - 2) + 1;
		y = rand() % (this -> y_size - 2) + 1;
	}

	this -> x_food = x;
	this -> y_food = y;

}

void Board::_initialize_wall() {
	int x_limit[2] = {0, this -> x_size - 1};
	int y_limit[2] = {0, this -> y_size - 1};

	for (int i = 0; i < 2; i++) {
		for (int j = y_limit[0]; j < y_limit[1]; j++) {
			this -> pixel[x_limit[i]][j] = 1;

			this -> x_wall.push_back(x_limit[i]);
			this -> y_wall.push_back(j);
		}
	}

	for (int i = 0; i < 2; i++) {
		for (int j = x_limit[0]; j < x_limit[1]; j++) {
			this -> pixel[j][y_limit[i]] = 1;

			this -> x_wall.push_back(j);
			this -> y_wall.push_back(y_limit[i]);
		}
	}
}

void Board::change_snake_direction(int new_direction) {
	if (this -> allow_direction_change) {
		this -> snake.change_direction(new_direction);
	}
	this -> allow_direction_change = false;
}

bool Board::_check_collision() {
	int x_head = this -> snake.x[this -> snake.x.size() - 1];
	int y_head = this -> snake.y[this -> snake.y.size() - 1];

	if (
		this -> pixel[x_head][y_head] == 1 || 
		this -> pixel[x_head][y_head] == 2
	) {
		return true;	
	} else {
		return false;
	}
}

bool Board::_check_food_collision() {
	int x_head = this -> snake.x[this -> snake.x.size() - 1];
	int y_head = this -> snake.y[this -> snake.y.size() - 1];

	if (this -> pixel[x_head][y_head] == 3) {
		return true;	
	} else {
		return false;
	}
}

void Board::move_snake() {
	bool collision = false, food_collision = false;

	if (this -> alive) {
		this -> snake.move(this -> x_food, this -> y_food);
		collision = this -> _check_collision();

		if (collision) {
			this -> alive = false;
		} else {
			food_collision = this -> _check_food_collision();

			if (food_collision) {
				this -> score++;
				this -> _generate_food();
			}

			this -> _sync_board();
			this -> allow_direction_change = true;
		}

	}

}

void Board::_initialize_snake() {
	int x_tail, y_tail, x_head, y_head;
	int snake_init_length = 3;
	std::vector<int> x_init, y_init;

	x_tail = int(this -> x_size / 2);
	y_tail = int(this -> y_size / 2);
	
	for (int i = 0; i < snake_init_length; i++) {
		x_init.push_back(x_tail + i);
		y_init.push_back(y_tail);
	}

	this -> snake = Snake(x_init, y_init);
	//this -> _sync_board();
}

void Board::_initialize_pixel() {
	for (int i = 0; i < this -> x_size; i++) {
		this -> pixel.push_back(std::vector<int>());
		for (int j = 0; j < this -> y_size; j++) {
			this -> pixel[i].push_back(0);
		}
	}
}

void Board::_clear_board() {
	for (int i = 0; i < this -> x_size; i++) {
		for (int j = 0; j < this -> y_size; j++) {
			this -> pixel[i][j] = 0;

			this -> x_wall.resize(0);
			this -> y_wall.resize(0);
		}
	}
}

void Board::_sync_board() {
	this -> _clear_board();
	this -> _initialize_wall();

	for (int i = 0; i < this -> snake.x.size(); i++) {
		this -> pixel[this -> snake.x[i]][this -> snake.y[i]] = 2;

		this -> x_wall.push_back(this -> snake.x[i]);
		this -> y_wall.push_back(this -> snake.y[i]);
	}

	this -> pixel[this -> x_food][this -> y_food] = 3;
	this -> x_wall.push_back(this -> x_food);
	this -> y_wall.push_back(this -> y_food);

}

#endif
