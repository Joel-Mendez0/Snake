#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <array>
#include <random>

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 800
#define GAME_ROWS 20 // MAX 128
#define GAME_COLS 20 // MAX 128

class Snake {
public:
	Snake();
	~Snake();
	void HandleEvents();
	void Update();
	void Render();
	void Game();
	void DisplayGameBoard();
	void Grow(int8_t new_row, int8_t new_col);
	void Move(int8_t new_row, int8_t new_col);
	bool Collision(int8_t new_row, int8_t new_col);
	void Draw();
	void SpawnFood();

	struct Node {
		int8_t row, col;
		Node* next;
	};

	enum class DIRECTION {
		LEFT = 0,
		RIGHT = 1,
		DOWN = 2,
		UP = 3,
		NONE = 4
	};

private:
	sf::RenderWindow* window;
	sf::Event event;
	std::array<std::array<int8_t, GAME_COLS>, GAME_ROWS> game_board = {};
	Node* head;
	Node* tail;
	sf::RectangleShape block;
	DIRECTION direction;
	bool game_over;
	std::pair<int8_t, int8_t> food_position;
	std::vector<std::pair<int8_t, int8_t>> food_queue;
	bool food_ate;	
};