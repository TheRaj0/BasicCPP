#include <iostream>
#include<vector>
#include<conio.h>

class Fruit
{
private:
    int m_x, m_y;

public:
    Fruit(int boundX, int boundY)
    {
        randomize(boundX, boundY);
    }

    void randomize(int boundX, int boundY)
    {
        srand(time(NULL));

        m_x = rand() % (boundX + 1);
        m_y = rand() % (boundY + 1);
    }

    inline int getX() const { return m_x; }
    inline int getY() const { return m_y; }
};

struct Vec2
{
    int x, y;

    Vec2(int x, int y)
        : x(x), y(y) {}
};

class Snake
{
public:
    enum Direction {UP = 0, DOWN, LEFT, RIGHT};
    bool should_increase = false;
private:
    std::vector<Vec2> body;
    Direction dir;

public:

    Snake(int x, int y)
    {
        body.reserve(10);
        body.emplace_back(x, y);

        srand(time(NULL));
        dir = (Direction)(rand() % 4);
    }

    void changeDir(Direction direction)
    {
        dir = direction;
    }

    void move(int boundX, int boundY)
    {
        Vec2& head = body[body.size() - 1];
        switch (dir)
        {
        case UP:
            body.emplace_back(head.x, (boundY + head.y) % (boundY + 1));
            break;
        case DOWN:
            body.emplace_back(head.x, (1 + head.y) % (boundY + 1));
            break;
        case LEFT:
            body.emplace_back((boundX + head.x) % (boundX+1), head.y);
            break;
        case RIGHT:
            body.emplace_back((1 + head.x) % (boundX + 1), head.y);
            break;
        }
    }

    void remove_tail()
    {
        body.erase(body.begin());
    }

    bool collide(const Fruit* f) const
    {
        for(int i = 0; i < body.size(); ++i)
        {
            if (body[i].x == f->getX() && body[i].y == f->getY())
                return true;
        }
        return false;
    }

    bool selfCollide() const
    {
        for (int i = 0; i < body.size() - 1; ++i)
        {
            for (int j = i + 1; j < body.size(); ++j)
            {
                if (body[i].x == body[j].x && body[i].y == body[j].y)
                    return true;
            }
        }
        return false;
    }

    const std::vector<Vec2>& get() const
    {
        return body;
    }

    inline Direction getDir() const { return dir; }
};

class GameManager
{
private:
    int m_width, m_height, score;
    bool game_over = false;

    Snake* m_snake;
    Fruit* m_fruit;

public:
    GameManager(int width, int height)
        : m_width(width), m_height(height)
    {
        m_snake = new Snake(width / 2, height / 2);
        m_fruit = new Fruit(width, height);

        m_snake->move(width, height);
        m_snake->move(width, height);

        score = m_snake->get().size();
    }

    ~GameManager()
    {
        delete m_snake, m_fruit;
    }

    void draw()
    {
        static const char WALL = 176;
        static const char SH = 2;
        static const char SB = 'o';
        static const char F = 219;

        system("cls");

        for (int i = 0; i <= m_width + 2; ++i) { std::cout << WALL; }
        std::cout << std::endl;

        for (int y = 0; y <= m_height; ++y)
        {
            std::cout << WALL;
            for (int x = 0; x <= m_width; ++x)
            {
                char c = ' ';

                for (auto v : m_snake->get())
                {
                    if (x == v.x && y == v.y) c = SB;
                }

                const Vec2& head = m_snake->get()[m_snake->get().size() - 1];
                if (x == head.x && y == head.y) c = SH;
                else if (x == m_fruit->getX() && y == m_fruit->getY()) c = F;

                std::cout << c;
            }
            std::cout << WALL << std::endl;;
        }

        for (int i = 0; i <= m_width + 2; ++i) { std::cout << WALL; }
        std::cout << "Score: " << score << std::endl;
    }

    void handleInput()
    {
        if (_kbhit())
        {
            Snake::Direction curr = m_snake->getDir();
            switch (_getch())
            {
            case 'W':
            case 'w':
                if (curr == Snake::Direction::DOWN) break;
                m_snake->changeDir(Snake::UP);
                break;
            case 'S':
            case 's':
                if (curr == Snake::Direction::UP) break;
                m_snake->changeDir(Snake::DOWN);
                break;
            case 'D':
            case 'd':
                if (curr == Snake::Direction::LEFT) break;
                m_snake->changeDir(Snake::RIGHT);
                break;
            case 'A':
            case 'a':
                if (curr == Snake::Direction::RIGHT) break;
                m_snake->changeDir(Snake::LEFT);
                break;
            }
        }
    }
    void handleCollision()
    {
        if (m_snake->collide(m_fruit))
        {
            score++;
            m_fruit->randomize(m_width, m_height);\
        }
        else if (m_snake->selfCollide())
        {
            game_over = true;
        }
        else {
            m_snake->remove_tail();
        }
    }

    void Run()
    {
        while(!game_over)
        {
            draw();
            handleInput();
            m_snake->move(m_width, m_height);
            handleCollision();
        }
        std::cout << "Game Over!" << std::endl;
        std::cin.get();
    }

    void Reset()
    {
        game_over = false;

        *m_snake = Snake(m_width / 2, m_height / 2);
        m_fruit->randomize(m_width, m_height);
        score = m_snake->get().size();
    }
};



int main()
{
    GameManager* m = new GameManager(23,13);
    
    do
    {
        m->Run();
        m->Reset();
    } while (std::cin.get() == '\n');

    delete m;
}