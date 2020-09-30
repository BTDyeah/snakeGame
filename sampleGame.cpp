#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

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
        sf::RectangleShape fruit;
        enum class Dir {UP, DOWN, LEFT, RIGHT};
        Dir direction;
        sf::Texture snakeTexture, tailTexture;
        sf::Sprite snakeSprite, tailSprite;
        std::random_device rd;
        std::mt19937 gen;
        std::uniform_int_distribution<> cod;
};

Game::Game() : myWindow(sf::VideoMode(550, 550), "snakeGame!", sf::Style::Close | sf::Style::Titlebar), gen(rd()), cod(1, 520) {
        direction = Dir::UP;
        /*
        0,0,40,10 right
        0,10,40,10 left
        0,20,10,40 up
        10, 20, 10, 40 down
        */
        
        if(!snakeTexture.loadFromFile("wholeSnake.png", sf::IntRect(0, 20, 10 , 40)))
                return;
        
        if(!tailTexture.loadFromFile("wholeSnake.png", sf::IntRect(0, 0, 10 , 10)))
                return;

        /*
        if(!texture.loadFromFile("wholeSnake.png", sf::IntRect(0, 20, 10 ,40)) && !(direction == Dir::UP))
                return;
        else 
        if(!texture.loadFromFile("wholeSnake.png", sf::IntRect(10, 20, 10 ,40)) && !(direction == Dir::DOWN))
                return;
        else
        if(!texture.loadFromFile("wholeSnake.png", sf::IntRect(0, 10, 40 ,10)) && !(direction == Dir::LEFT))
                return;
        else
        if(!texture.loadFromFile("wholeSnake.png", sf::IntRect(0, 0, 40 ,10)) && !(direction == Dir::RIGHT))
                return;
        */        

        snakeSprite.setTexture(snakeTexture);
        tailSprite.setTexture(tailTexture);

        //A QUICK TOUGHT THAT I MAY NOT FUNDAMENTALLY BE WRONG ABOUT THINGS LIKE THIS FOR ANIMATING BUT THAT I NEED ANOTHER SPRITE.SETTEXTURE()?
        

        fruit.setSize(sf::Vector2f(10, 10));
        fruit.setFillColor(sf::Color::Red);
        fruit.setPosition(cod(gen), cod(gen));

        snakeSprite.setPosition(cod(gen), cod(gen));

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
        if (direction == Dir::UP)    movement.y -= 0.01f;
        if (direction == Dir::DOWN)  movement.y += 0.01f;
        if (direction == Dir::LEFT)  movement.x -= 0.01f;
        if (direction == Dir::RIGHT) movement.x += 0.01f;

        isColliding(movement);
}

void Game::render(){
        myWindow.clear();
        myWindow.draw(snakeSprite);   
        myWindow.draw(fruit);
        myWindow.display();
}

/*
1. Make a seperate head(harder on spritesheet but easier on coding it) 
2. Make it work so only head counts the other way(intersects()???)
*/

void Game::isColliding(sf::Vector2f mov){
        if (snakeSprite.getPosition().x < 0 ) { mov.x = myWindow.getSize().x - snakeSprite.getLocalBounds().width;  }
        if (snakeSprite.getPosition().y < 0 ) { mov.y = myWindow.getSize().y - snakeSprite.getLocalBounds().height; }
        if (snakeSprite.getPosition().x + snakeSprite.getLocalBounds().width > myWindow.getSize().x )  { mov.x = snakeSprite.getLocalBounds().width -  myWindow.getSize().x; }
        if (snakeSprite.getPosition().y + snakeSprite.getLocalBounds().height > myWindow.getSize().y ) { mov.y = snakeSprite.getLocalBounds().height - myWindow.getSize().y;}
        snakeSprite.move(mov);
        
       

        /*if (snakeSprite.getPosition().x < fruit.getPosition().x && snakeSprite.getPosition().y < fruit.getPosition().y
            && snakeSprite.getPosition().x + snakeSprite.getLocalBounds().width > fruit.getGlobalBounds().left
            && snakeSprite.getPosition().y + snakeSprite.getLocalBounds().height > fruit.getGlobalBounds().top    
           ) 
           { 
             fruit.setPosition(cod(gen), cod(gen)); 
             //tailSprite.setPosition(snakeSprite.getPosition().x + tailSprite.getGlobalBounds().width, snakeSprite.getPosition().y + tailSprite.getGlobalBounds().height);         
           }*/
        
        if(snakeSprite.getGlobalBounds().intersects(fruit.getGlobalBounds()))
                fruit.setPosition(cod(gen), cod(gen)); //easier than whole mess up but still eats with tail which is wrong

}

int main(){
        
    Game game;
    game.run();

    return 0;
}