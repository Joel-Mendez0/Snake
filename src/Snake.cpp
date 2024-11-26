#include "Snake.h"

Snake::Snake()
    : window(nullptr), game_over(false), food_ate(false){

    srand(time(NULL));
    this->window = new sf::RenderWindow(sf::VideoMode(WINDOW_HEIGHT, WINDOW_WIDTH), "Snake");
    this->window->setFramerateLimit(15);

    for (int8_t row = 0; row < GAME_ROWS; row++) {
        for (int8_t col = 0; col < GAME_COLS; col++) {
            this->food_queue.emplace_back(row, col);
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(this->food_queue.begin(), this->food_queue.end(), g);

    this->SpawnFood();

    this->direction = DIRECTION::NONE;
    this->head = new Node{ GAME_ROWS / 2,GAME_COLS / 2,nullptr };
    this->tail = head;
    this->Game();
}

Snake::~Snake() {
    delete this->window;
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}

void Snake::Game() {

    while (this->window->isOpen() && !game_over) {

        this->HandleEvents();
        this->Update();
        this->Render();
        this->DisplayGameBoard();
    }
}

void Snake::HandleEvents() {
   while (this->window->pollEvent(this->event)) {
    switch (this->event.type) {
    case sf::Event::Closed:
        this->window->close();
        break;
    case sf::Event::KeyPressed:
        if (this->event.key.code == sf::Keyboard::Escape) {
            this->window->close();
        }
        if (this->event.key.code == sf::Keyboard::W || this->event.key.code == sf::Keyboard::Up) {
            this->direction = DIRECTION::UP;
        }
        else if (this->event.key.code == sf::Keyboard::A || this->event.key.code == sf::Keyboard::Left) {
            this->direction = DIRECTION::LEFT;
        }
        else if (this->event.key.code == sf::Keyboard::S || this->event.key.code == sf::Keyboard::Down) {
            this->direction = DIRECTION::DOWN;
        }
        else if (this->event.key.code == sf::Keyboard::D || this->event.key.code == sf::Keyboard::Right) {
            this->direction = DIRECTION::RIGHT;
        }
        break;        
    }
   }
    return;
}

void Snake::Update() {
    if (this->direction == DIRECTION::RIGHT) {
        this->head->col++;
    }
    else if (this->direction == DIRECTION::LEFT) {
        this->head->col--;
    }
    else if (this->direction == DIRECTION::DOWN) {
        this->head->row++;
    }
    else if (this->direction == DIRECTION::UP) {
        this->head->row--;
    }
    if (!Collision(this->head->row, this->head->col) && direction != DIRECTION::NONE) {
        if (this->food_ate == true) {
            this->Grow(this->head->row, this->head->col);
            this->food_ate = false;
            this->SpawnFood();
        }
        else {
            this->Move(this->head->row, this->head->col);
        }
    }
    return;
}

void Snake::Render() {
    this->window->clear();

    this->Draw();

    this->window->display();
    return;
}

void Snake::DisplayGameBoard() {
    for (int8_t row = 0; row < GAME_ROWS; row++) {
        for (int8_t col = 0; col < GAME_COLS; col++) {
            std::cout << static_cast<int>(game_board[row][col]) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------------" << std::endl;
}

void Snake::Grow(int8_t new_row, int8_t new_col) {

    Node* new_head = new Node{ new_row,new_col,head };
    head = new_head;
    this->game_board[new_row][new_col] = 1;
}
void Snake::Move(int8_t new_row, int8_t new_col) {

    int8_t prev_row = this->head->row;
    int8_t prev_col = this->head->col;

    this->head->row = new_row;
    this->head->col = new_col;

    Node* curr = this->head->next;
    while (curr) {
        int8_t temp_row = curr->row;
        int8_t temp_col = curr->col;

        curr->row = prev_row;
        curr->col = prev_col;

        prev_row = temp_row;
        prev_col = temp_col;

        curr = curr->next;
    }
    // Optimize Later
    for (int8_t row = 0; row < GAME_ROWS; row++) {
        for (int8_t col = 0; col < GAME_COLS; col++) {
            this->game_board[row][col] = 0;
        }
    }
    curr = head;
    while (curr) {
        this->game_board[curr->row][curr->col] = 1;
        curr = curr->next;
    }
}

void Snake::Draw() {

    sf::RectangleShape headblock;
    Node* curr = head;

    while (curr) {

        this->block.setPosition((WINDOW_WIDTH / GAME_COLS) * curr->col, (WINDOW_HEIGHT / GAME_ROWS) * curr->row);
        headblock.setPosition((WINDOW_WIDTH / GAME_COLS) * head->col, (WINDOW_HEIGHT / GAME_ROWS) * head->row);

        this->block.setSize(sf::Vector2f((WINDOW_WIDTH / GAME_COLS), (WINDOW_HEIGHT / GAME_ROWS)));
        headblock.setSize(sf::Vector2f((WINDOW_WIDTH / GAME_COLS), (WINDOW_HEIGHT / GAME_ROWS)));

        this->block.setFillColor(sf::Color::White);
        headblock.setFillColor(sf::Color::Green);

        this->window->draw(this->block);
        this->window->draw(headblock);

        curr = curr->next;
    }

    this->block.setPosition((WINDOW_WIDTH / GAME_COLS) * this->food_position.second, (WINDOW_HEIGHT / GAME_ROWS) * this->food_position.first);
    this->block.setSize(sf::Vector2f((WINDOW_WIDTH / GAME_COLS), (WINDOW_HEIGHT / GAME_ROWS)));
    this->block.setFillColor(sf::Color::Red);
    this->window->draw(this->block);
}

bool Snake::Collision(int8_t new_row, int8_t new_col) {

    if (new_row == -1 || new_row == GAME_ROWS || new_col == -1 || new_col == GAME_COLS) {
        this->direction = DIRECTION::NONE;
        this->game_over = true;
        return true;
    }

    if (this->food_position.first == new_row && this->food_position.second == new_col) {
        this->food_ate = true;
        return false;
    }
    Node* curr = head;
    curr = curr->next;
    while (curr) {
        if (new_row == curr->row && new_col == curr->col) {
            this->game_over = true;
            return true;
        }
        curr = curr->next;
    }

    return false;
}

void Snake::SpawnFood() {

    this->food_position.first = this->food_queue.back().first;
    this->food_position.second = this->food_queue.back().second;
    this->food_queue.pop_back();

}
