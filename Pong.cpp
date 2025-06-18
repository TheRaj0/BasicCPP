#include<iostream>
#include<time.h>
#include<conio.h>
#include<Windows.h>

struct Vec2
{
	float x, y;

	Vec2(float x, float y)
		: x(x), y(y) {}

	void normalize()
	{
		float r = sqrt(x * x + y * y);
		if (r != 0)
		{
			x /= r;
			y /= r;
		}
	}

	void operator+=(const Vec2& other)
	{
		x += other.x;
		y += other.y;
	}

	Vec2 operator*(float num) const
	{
		return Vec2(x * num, y * num);
	}
};

class Ball
{
private:
	const Vec2 s_pos;
	Vec2 pos;
	Vec2 velocity;
	float speed = 1.2f;
public:
	Ball(float x, float y)
		: s_pos(x,y), pos(x, y), velocity(0, 0) {}

	void Move()
	{
		pos += velocity * speed;
	}

	void RandomDir()
	{
		srand(time(NULL));
		float rx = rand() % 2 == 1 ? 1 : -1;
		float ry = rand() % 2 == 1 ? 1 : -1;
		velocity = Vec2(rx, ry);
		velocity.normalize();
	}

	inline int getX() const
	{
		return (int)pos.x;
	}

	inline int getY() const
	{
		return (int)pos.y;
	}

	inline void ReverseX()
	{
		velocity.x *= -1;
	}

	inline void ReverseY()
	{
		velocity.y *= -1;
	}
	void Reset()
	{
		pos = s_pos;
		velocity = Vec2(0, 0);
		RandomDir();
	}

};

class Paddle
{
private:
	const Vec2 s_pos;
	Vec2 pos;
	int speed = 2;
public:
	Paddle(float x, float y)
		:s_pos(x, y), pos(x, y) {}

	inline void moveUp()
	{
		pos.y -= speed;
	}

	inline void moveDown()
	{
		pos.y += speed;
	}

	inline int getX() const
	{
		return (int)pos.x;
	}
	inline int getY() const
	{
		return (int)pos.y;
	}

	void Reset()
	{
		pos = s_pos;
	}
};

class GameManager
{
private:
	const int WIDTH = 30;
	const int HEIGHT = 8;
	const int PW = 2;

	int score1, score2;

	Ball* ball;
	Paddle* p1;
	Paddle* p2;
public:
	GameManager()
	{
		ball = new Ball(WIDTH / 2, HEIGHT / 2);
		p1 = new Paddle(2, HEIGHT / 2 - PW/2);
		p2 = new Paddle( WIDTH - 2, HEIGHT / 2 - PW/2);

		score1 = 0;
		score2 = 0;

		ball->RandomDir();
	}

	~GameManager()
	{
		delete ball, p1, p2;
	}

	void Run()
	{
		bool play = true;
		while (play)
		{
			ball->Move();
			draw();
			collideLogic();
			if (_kbhit())
			{
				switch (_getch())
				{
				case 'w':
					if (p1->getY() <= 0) break;

					p1->moveUp();
					break;
				case 's':
					if (p1->getY() + PW >= HEIGHT) break;

					p1->moveDown();
					break;
				case 'i':
					if (p2->getY() <= 0) break;

					p2->moveUp();
					break;
				case 'k':
					if (p2->getY() + PW >= HEIGHT) break;

					p2->moveDown();
					break;
				case 27: //esc
					play = false;
					std::cin.get();
					break;

				}
			}

			Sleep(10);

			//Sleep(10);
		}
	}

private:
	void draw() const
	{
		static char B = 2;
		static char P = 219;
		static char W = 176;

		system("cls");
		for (int i = 0; i <= WIDTH + 2; ++i)
		{
			std::cout << W;
		} std::cout << std::endl;

		for (int y = 0; y <= HEIGHT; y++)
		{
			std::cout << W;
			for (int x = 0; x <= WIDTH; x++)
			{
				char c = ' ';
				if (x == ball->getX() && y == ball->getY())
					c = B;
				else if ((x == p1->getX() && y >= p1->getY() && y <= p1->getY() + PW) ||
					(x == p2->getX() && y >= p2->getY() && y <= p2->getY() + PW)
					) c = P;

				std::cout << c;
			}
			std::cout << W << std::endl;;
		}

		for (int i = 0; i <= WIDTH + 2; ++i)
		{
			std::cout << W;
		} std::cout << std::endl;

		std::cout << "Score 1: " << score1 << std::endl;
		std::cout << "Score 2: " << score2 << std::endl;
	}

	void collideLogic()
	{
		int px1 = p1->getX();
		int py1 = p1->getY();
		int px2 = p2->getX();
		int py2 = p2->getY();
		if  ((ball->getX() == px1+1 && ball->getY() >= py1 && ball->getY() <= py1 + PW) ||
			(ball->getX() == px2-1 && ball->getY() >= py2 && ball->getY() <= py2 + PW))
		{
			ball->ReverseX();
		}
		else if (ball->getX() <= 0)
		{
			score2++;
			ResetPos();
			ball->RandomDir();
		}
		else if (ball->getX() >= WIDTH)
		{
			score1++;
			ResetPos();
			ball->RandomDir();
		}


		if(ball->getY() <= 0 || ball->getY() >= HEIGHT)
		{
			ball->ReverseY();
		}
	}

	void ResetPos()
	{
		ball->Reset();
		p1->Reset();
		p2->Reset();
	}

};

int main()
{
	GameManager* g = new GameManager();
	g->Run();

	/*for (int i = 0; i < 256; i++)
	{
		std::cout << (char)i << "  - " << i << std::endl;
	}*/


	delete g;
	std::cin.get();
}