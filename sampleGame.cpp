#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <random>
#include <iterator>

class Game {
    public:
        Game();
        void run();
    private:
        void ProcessEvents();
        void update();
        void render();
        void handlePlayerInput(sf::Keyboard::Key key);
        void isColliding(sf::Vector2f mov);
        void snakeFood();
        void foodCollision();
    private:
        sf::RenderWindow myWindow;
        std::deque<sf::RectangleShape> snake;
        sf::RectangleShape fruit;
        enum class Dir {UP, DOWN, LEFT, RIGHT};
        Dir direction;
        sf::Texture texture;
        sf::Sprite sprite;
};

Game::Game() : myWindow(sf::VideoMode(640, 480), "snakeGame!", sf::Style::Close | sf::Style::Titlebar), snake(3) {
        direction = Dir::UP;
        /*
        0,0,40,10 right
        0,10,40,10 left
        0,20,10,40 up
        10, 20, 10, 40 down
        */
        
        texture.loadFromFile("wholeSnake.png", sf::IntRect(0, 20, 10 ,40));
        sprite.setPosition(124, 120);

        
        
        /*THE ANIMATION PART*/
        /*switch(direction){
            case Dir::RIGHT: x = 0; y = 0; w = 40; h = 10;   xPos += 1;   sprite.setPosition(xPos + 1, yPos);   break;
            case Dir::LEFT:  x = 0; y = 10; w = 40; h = 10;  xPos -= 1;   sprite.setPosition(xPos - 1, yPos);   break;
            case Dir::UP:    x = 0; y = 20; w = 10; h = 40;  yPos -= 1;   sprite.setPosition(xPos, yPos - 1);   break;
            case Dir::DOWN:  x = 10; y = 20; w = 10; h = 40; yPos += 1;   sprite.setPosition(xPos, yPos + 1);   break;
        }            
        */

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> cod(1, 640);

        fruit.setSize(sf::Vector2f(10, 10));
        fruit.setFillColor(sf::Color::Red);
        fruit.setPosition(cod(gen), cod(gen));
}

void Game::run(){
        while (myWindow.isOpen()){
            ProcessEvents();
            update();
            render();
        }
}

void Game::ProcessEvents(){
        sf::Event event;
        while (myWindow.pollEvent(event)){
                switch(event.type){
                        case sf::Event::KeyPressed : handlePlayerInput(event.key.code); break;
                        case sf::Event::Closed : myWindow.close(); break;
                }
        }
}

void Game::handlePlayerInput(sf::Keyboard::Key key){
        if (key == sf::Keyboard::W || key == sf::Keyboard::Up) direction = Dir::UP;
        else if (key == sf::Keyboard::S || key == sf::Keyboard::Down) direction = Dir::DOWN;
        else if (key == sf::Keyboard::A || key == sf::Keyboard::Left) direction = Dir::LEFT;
        else if (key == sf::Keyboard::D || key == sf::Keyboard::Right) direction = Dir::RIGHT;
}

void Game::update(){
        sf::Vector2f movement(0.0f, 0.0f);
        if (direction == Dir::UP)    movement.y -= 0.03f;
        if (direction == Dir::DOWN)  movement.y += 0.03f;
        if (direction == Dir::LEFT)  movement.x -= 0.03f;
        if (direction == Dir::RIGHT) movement.x += 0.03f;

        isColliding(movement);
}

void Game::render(){
        myWindow.clear();
        myWindow.draw(sprite);   
        myWindow.draw(fruit);
        myWindow.display();
}

void Game::isColliding(sf::Vector2f mov){
        if (sprite.getPosition().x < 0 ) { mov.x = myWindow.getSize().x - sprite.getLocalBounds().width;  }
        if (sprite.getPosition().y < 0 ) { mov.y = myWindow.getSize().y - sprite.getLocalBounds().height; }
        if (sprite.getPosition().x + sprite.getLocalBounds().width > myWindow.getSize().x )  { mov.x = sprite.getLocalBounds().width -  myWindow.getSize().x; }
        if (sprite.getPosition().y + sprite.getLocalBounds().height > myWindow.getSize().y ) { mov.y = sprite.getLocalBounds().height - myWindow.getSize().y;}
        sprite.move(mov);
}

int main(){
        
    Game game;
    game.run();

    return 0;
}