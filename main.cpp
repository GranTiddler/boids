
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
                return num - floor(num / mod) * mod;
            }

    public:
        float posX;
        float posY;
        float rotation;
        float speed;

        Boid(){

            
            posX = 100;
            posY = 100;
            speed = 1;
        }

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
                if(floatMod(rotation, M_PI * 2) < M_PI)
                {
                    rotate(-.1 * 10 / posX);
                }
                else
                {
                    rotate(.1 * 10 / posX);
                }
            }
            else if(posX > 900)
            {
                if(floatMod(rotation, M_PI * 2) > M_PI)
                {
                    rotate(-.1 * 10 / (1000 - posX));
                }
                else
                {
                    rotate(.1 * 10 / (1000 - posX));
                }
            }


            if(posY < 100)
            {
                if(floatMod(rotation +  M_PI_2, M_PI * 2) > M_PI)
                {
                    rotate(-.1 * 10 / posY);
                }
                else
                {
                    rotate(.1 * 10 / posY);
                }
            }
            else if(posY > 900)
            {
                if(floatMod(rotation +  M_PI_2, M_PI * 2) < M_PI)
                {
                    rotate(-.1 * 10 / (1000 - posY));
                }
                else
                {
                    rotate(.1 * 10 / (1000 - posY));
                }
            }
        }

};

float floatMod(float num, float mod)
{
    return num - floor(num / mod) * mod;
}

float dist(Boid p1, Boid p2){
    int x1 = p1.posX;
    int x2 = p2.posX;

    int y1 = p1.posY;
    int y2 = p2.posY;
    return sqrt(pow((x1 - x2), 2) + pow(y1 - y2, 2));
}

int main() 
{
    int numChilds = 1000;
    int chunks = 10;

    int range = 10;
    srand(time(0));
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "minesweeper", sf::Style::Close);

    Boid children[numChilds];

    sf::Texture text;
    text.loadFromFile("boi.png");
    
    for(int i = 0; i < numChilds; i++)
    {

        
        children[i] = Boid(200, 200, 0, 2);
        children[i].setTexture(text);
        children[i].setScale(.1, .1);
        children[i].rotate(rand() * M_PI * 2 / RAND_MAX);

        
        
        
        
    }
    
    int chunkList[numChilds];
    int xChunk;
    int yChunk;
    
    
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
        
        
        window.clear(sf::Color(255, 255, 255));
        

        for(int i = 0; i < numChilds; i++){
            children[i].move();
            children[i].rotateRand();
            children[i].avoidWalls();
            window.draw(children[i]);

            xChunk = (children[i].posX - (int)floatMod(children[i].posX, 1000/chunks)) / chunks;
            yChunk = (children[i].posY - (int)floatMod(children[i].posY, 1000/chunks)) / chunks;

            chunkList[i] = (yChunk * chunks) + xChunk;
        }
        
        
        
        

        


        window.display();
        
    }
}