#ifndef SNAKE_H
#define SNAKE_H

#include <vector>

class Snake {
public:
	std::vector<int> x, y;
	int direction;

	Snake();
	Snake(std::vector<int> x_init, std::vector<int> y_init);
	void move(int x_food, int y_food);
	void change_direction(int new_direction);
};

Snake::Snake() {
	this -> x = std::vector<int>();
	this -> y = std::vector<int>();
	this -> direction = 0;
}

Snake::Snake(std::vector<int> x_init, std::vector<int>y_init) {
	this -> x = x_init;
	this -> y = y_init;
	this -> direction = 0;
}

void Snake::change_direction(int new_direction) {
	if (abs(new_direction - this -> direction) != 2)
		this -> direction = new_direction;
}

void Snake::move(int x_food, int y_food) {
	int x_head = this -> x[x.size() - 1];
	int y_head = this -> y[y.size() - 1];
	int new_x_head, new_y_head;


	if (direction == 0) {
		new_x_head = x_head + 1;
		new_y_head = y_head;	
	} else if (direction == 1) {
		new_x_head = x_head;
		new_y_head = y_head + 1;
	} else if (direction == 2) {
		new_x_head = x_head - 1;
		new_y_head = y_head;
	} else {
		new_x_head = x_head;
		new_y_head = y_head - 1;
	}

	this -> x.push_back(new_x_head);
	this -> y.push_back(new_y_head);

	if (new_x_head != x_food || new_y_head != y_food) {
		this -> x.erase(this -> x.begin());
		this -> y.erase(this -> y.begin());
	}
	
}

#endif
