#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace sf;

const int Width = 20, Height = 20;
const float CellSize = 30, Delay =200;

struct Posicion
{
    int X;
    int Y;
};

enum Direcion
{
    Up,
    Down,
    Left,
    Right
};

std::vector<Posicion> snake;

bool Move(Direcion Dir, Posicion &FoodPos);
void SpawnFood(Posicion &FoodPos);

int main()
{
    Direcion Dir = Direcion::Up;
    Clock timer;
    
    snake.push_back(Posicion{Width/2, Height/2});

    RenderWindow window(VideoMode(Width * CellSize, Height * CellSize), "SNAKE"); 

    RectangleShape Cell({CellSize ,CellSize});
    Cell.setFillColor(Color( 34, 153, 84 ));
    Cell.setOutlineThickness(-2);
    Cell.setOutlineColor(Color( 20, 90, 50 ));

    Posicion FoodPos{rand()%Width, rand()%Height};
    RectangleShape Food({CellSize ,CellSize});
    Food.setFillColor(Color( 192, 57, 43 ));
    Food.setOutlineThickness(-2);
    Food.setOutlineColor(Color( 169, 50, 38 ));
    
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case Event::Closed:
                {
                    window.close();
                }
                break;
                case Event::KeyPressed://klawiatura
                {
                    switch(event.key.code)
                    {
                    case Keyboard::Escape:
                        {
                            window.close();
                            break;
                        }
                    case Keyboard::W:
                        {
                            if(Dir != Direcion::Down)
                                Dir = Direcion::Up;
                            break;
                        }
                    case Keyboard::S:
                        {
                            if(Dir != Direcion::Up)
                                Dir = Direcion::Down;
                            break;
                        }
                    case Keyboard::A:
                        {
                            if(Dir != Direcion::Right)
                                Dir = Direcion::Left;
                            break;
                        }
                    case Keyboard::D:
                        {
                            if(Dir != Direcion::Left)
                                Dir = Direcion::Right;
                            break;
                        }
                    }
                }
                break;
            }
        }
        if(timer.getElapsedTime().asMilliseconds() >= (Delay-snake.size()*2))
        {
            if(!Move(Dir, FoodPos))
            {
            window.close();
            break;
            }
            timer.restart();
        }

        window.clear(Color( 23, 32, 42 ));
        Food.setPosition(FoodPos.X*CellSize, FoodPos.Y*CellSize);
        window.draw(Food);
        for (int i = 0; i < snake.size(); i++)
        {
            Cell.setPosition(snake[i].X*CellSize, snake[i].Y*CellSize);
            window.draw(Cell);
        }
        window.display();
    }

    return 0;
}

bool Move(Direcion Dir, Posicion &FoodPos)
{
    Posicion NextStep = snake[snake.size()-1];
    switch(Dir)
    {
        case Up:
        {
            NextStep.Y--;
            break;
        }
        case Down:
        {
            NextStep.Y++;
            break;
        }
        case Left:
        {
            NextStep.X--;
            break;
        }
        case Right:
        {
            NextStep.X++;
            break;
        }
    }

    //is valid 
    if (NextStep.X < 0 || NextStep.X >= Width || NextStep.Y < 0 || NextStep.Y >=Height)
    {
        return false;
    }
    for (int i = 0; i < snake.size()-1; i++)
    {
        if(snake[i].X == NextStep.X && snake[i].Y == NextStep.Y)
        {
            return false;
        }
    }

    snake.push_back(NextStep);

    //Zdobycie Jedzenia
    if(!(NextStep.X == FoodPos.X && NextStep.Y == FoodPos.Y))
    {
        snake.erase(snake.begin());
    }
    else
    {
        SpawnFood(FoodPos);
    }
    
    return true;
}

void SpawnFood(Posicion &FoodPos)
{
    int X;
    int Y;

    bool isValid;

    do
    {
        isValid = false;
        X = rand()%Width;
        Y = rand()%Height;

        for (int i = 0; i < snake.size()-1; i++)
        {
            if(snake[i].X == X && snake[i].Y == Y)
            {
                isValid = true;
            }
        }
    } while(isValid);

    FoodPos = Posicion{X,Y};
}