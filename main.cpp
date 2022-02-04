
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
	int wallAvoid = 250;

	float floatMod(float num, float mod)
	{
		return num - floor(num / mod) * mod;
	}

public:
	float posX;
	float posY;
	float rotation;
	float speed;

	Boid()
	{

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
		sf::Sprite::rotate(angle * (360 / (M_PI * 2)));
		rotation += angle;
	}

	void rotateRand()
	{
		rotate(((float)rand() * max_turn * 2 / RAND_MAX) - max_turn);
	}

	void avoidWalls()
	{
		if (posX < 100)
		{
			if (floatMod(rotation, M_PI * 2) < M_PI)
			{
				rotate(-.1 * wallAvoid / posX);
			}
			else
			{
				rotate(.1 * wallAvoid / posX);
			}
		}
		else if (posX > 900)
		{
			if (floatMod(rotation, M_PI * 2) > M_PI)
			{
				rotate(-.1 * wallAvoid / (1000 - posX));
			}
			else
			{
				rotate(.1 * wallAvoid / (1000 - posX));
			}
		}

		if (posY < 100)
		{
			if (floatMod(rotation + M_PI_2, M_PI * 2) > M_PI)
			{
				rotate(-.1 * wallAvoid / posY);
			}
			else
			{
				rotate(.1 * wallAvoid / posY);
			}
		}
		else if (posY > 900)
		{
			if (floatMod(rotation + M_PI_2, M_PI * 2) < M_PI)
			{
				rotate(-.1 * wallAvoid / (1000 - posY));
			}
			else
			{
				rotate(.1 * wallAvoid / (1000 - posY));
			}
		}
	}
};

float floatMod(float num, float mod)
{
	return num - floor(num / mod) * mod;
}

float dist(Boid p1, Boid p2)
{
	int x1 = p1.posX;
	int x2 = p2.posX;

	int y1 = p1.posY;
	int y2 = p2.posY;
	return sqrt(pow((x1 - x2), 2) + pow(y1 - y2, 2));
}

int main()
{
	int rad = 30;
	float alignment = .05;

	int numChilds = 1000;
	int chunks = 1000 / rad * 2;

	int range = 10;
	srand(time(0));
	sf::RenderWindow window(sf::VideoMode(1000, 1000), "BOID", sf::Style::Close);

	Boid children[numChilds];

	sf::Texture text;
	text.loadFromFile("boi.png");

	for (int i = 0; i < numChilds; i++)
	{
		children[i] = Boid(100 + (((float)rand() / RAND_MAX) * 800), 100 + (((float)rand() / RAND_MAX) * 800), 0, 2);
		children[i].setTexture(text);
		children[i].setScale(.25, .25);
		children[i].rotate(rand() * M_PI * 2 / RAND_MAX);
	}

	std::vector<int> chunkList[chunks][chunks];
	int xChunk;
	int yChunk;

	int nearby;
	float avgRotation;

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

		for (int i = 0; i < numChilds; i++)
		{
			children[i].move();
			children[i].rotateRand();
			children[i].avoidWalls();
			window.draw(children[i]);

			xChunk = (children[i].posX - (int)floatMod(children[i].posX, 1000 / chunks)) / (1000 / chunks);
			yChunk = (children[i].posY - (int)floatMod(children[i].posY, 1000 / chunks)) / (1000 / chunks);

			if (xChunk < 0)
			{
				std::cout << xChunk << std::endl;
			}

			chunkList[xChunk][yChunk].push_back(i);
		}

		for (int i = 0; i < numChilds; i++)
		{
			nearby = 0;
			avgRotation = 0;
			xChunk = (children[i].posX - (int)floatMod(children[i].posX, 1000 / chunks)) / (1000 / chunks);
			yChunk = (children[i].posY - (int)floatMod(children[i].posY, 1000 / chunks)) / (1000 / chunks);
			for (int x = 0; x <= 9; x++)
			{

				int xx = (x % 3) - 1;
				int y = ((x - xx) / 3) - 1;
				if (!(xChunk + xx < 0 || xChunk + xx + 1 > chunks || yChunk + y < 0 || yChunk + y + 1 > chunks))
				{
					for (int j = 0; j < chunkList[xChunk + xx][yChunk + y].size(); j++)
					{

						if (dist(children[i], children[chunkList[xChunk + xx][yChunk + y][j]]) < rad)
						{
							nearby++;
							avgRotation += floatMod(children[chunkList[xChunk + xx][yChunk + y][j]].rotation, M_PI * 2);
							//std::cout<<floatMod(children[chunkList[xChunk + xx][yChunk + y][j]].rotation, M_PI * 2)<<std::endl;
						}
					}
				}
			}
			avgRotation /= nearby;
			if ((avgRotation - children[i].rotation) < M_PI)
			{
				children[i].rotate((avgRotation - children[i].rotation) * alignment);
			}
			else
			{
				children[i].rotate(((avgRotation - children[i].rotation) - (2 * M_PI)) * alignment);
			}
		}

		for (int i = 0; i < chunks; i++)
		{
			for (int j = 0; j < chunks; j++)
			{
				chunkList[i][j].clear();
			}
		}
		window.display();
	}
}