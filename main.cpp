
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <math.h> 

class Boid : public sf::Sprite
{
    private:
        float max_turn = 0.03;

        float floatMod(float num, float mod)
            {
                std::cout<<num - floor(num / mod) * mod << std::endl;
                return num - floor(num / mod) * mod;
            }

    public:
        float posX;
        float posY;
        float rotation;
        float speed;

        Boid(int x, int y, float angle, float s)
        {
            posX = x;
            posY = y;
            rotate(angle);
            speed = s;
            
        }

        void move()
        {
            posX += cos(rotation) * speed;
            posY += sin(rotation) * speed;

            setPosition(posX, posY);
        }
        
        void rotate(float angle)
        {
            sf::Sprite::rotate(angle * (360/ (M_PI * 2)));
            rotation += angle;
        }

        void rotateRand(){
            rotate(((float)rand() * max_turn * 2/RAND_MAX) - max_turn);
            
        }

        void avoidWalls()
        {
            if(posX < 100)
            {
                if(floatMod(rotation, M_PI * 2) > M_PI)
                {
                    rotate(.1);
                }
                else
                {
                    rotate(-.1);
                }
            }
            else if(posX > 900)
            {
                if(floatMod(rotation, M_PI * 2) > M_PI)
                {
                    rotate(-.1);
                }
                else
                {
                    rotate(.1);
                }
            }


            if(posY < 100)
            {
                rotate(max_turn);
            }
            else if(posY > 900)
            {
                rotate(max_turn);
            }
        }

};





int main() 
{
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "minesweeper", sf::Style::Close);

    Boid boy(800, 200, M_PI/4, 3);
    sf::Texture text;
    text.loadFromFile("boi.png");
    boy.setTexture(text);

    boy.setScale(0.5, 0.5);

    
    window.setFramerateLimit(60);
    while (window.isOpen())
    {      
        //if close window button clicked close window
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear(sf::Color::White);

        boy.move();
        boy.rotateRand();
        boy.avoidWalls();
        window.draw(boy);
        








        window.display();
        
    }
}