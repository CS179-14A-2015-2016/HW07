////////////////////
///////Headers//////
////////////////////
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <unistd.h>
#include <thread>
#include <string>
#include "Vector.hpp"
#include "WormProcess.hpp"

///////////////////
//////Defines//////
///////////////////
#define PI 3.14159265


Vector windCurr;
Vector tankSize(70, 50);
WormProcess wp;
Vector proc;
Vector acc(0, 0);
Vector currPos;
Vector prevPos;
float gravity = 134.f;
Vector tankCenter;
float radiusCannon = 10.f;
float radiusAim = 100.f;
Vector arenaSize(1280, 380);
Vector powerSize(200, 25);
Vector initVelo(0, 0);
float angle;
float clampx;
float clampy;
float dt = 0.03333;
int gameHeight = 600;
int gameWidth = 800;
int power = 0;
bool fullPowerbar = false;
bool emptyPowerbar = true;
bool drawn = false;
bool turn1 = true;
bool turn2 = false;
bool setAngle = false;
bool isFiring = false;
bool first = true;
const sf::Time TimePerFrame = sf::seconds(1.0/30.f);


int main()
{
    //Create the window
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32),
        "WorchBound", sf::Style::Titlebar|sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    //Create left tank
    sf::RectangleShape tank1;
    sf::Vector2f a(tankSize.x, tankSize.y);
    tank1.setSize(a);
    tank1.setOutlineThickness(1);
    tank1.setOutlineColor(sf::Color(100, 100, 100));
    tank1.setFillColor(sf::Color(29, 33, 13)); //Army Green Color

    //Create right tank
    sf::RectangleShape tank2;
    tank2.setSize(a);
    tank2.setOutlineThickness(1);
    tank2.setOutlineColor(sf::Color(100, 100, 100));
    tank2.setFillColor(sf::Color(29, 33, 13)); //Army Green Color

    //Create cannon
    sf::CircleShape cannon;
    cannon.setRadius(radiusCannon);
    cannon.setOutlineThickness(2);
    cannon.setOutlineColor(sf::Color::Black);
    cannon.setFillColor(sf::Color(224, 223, 219)); //Stainless Steel Color

        //Create colored part of the power bar
    sf::RectangleShape fillPowerbar;
    sf::Vector2f t(powerSize.x,powerSize.y);
    fillPowerbar.setSize(t);
    fillPowerbar.setFillColor(sf::Color::Black);

    //Get and load custom font
    sf::Font customFont;
    if(!customFont.loadFromFile("disparador_stencil.ttf"))
        return EXIT_FAILURE;

    //Make a Text object using the loaded font
    sf::Text txt;
    txt.setFont(customFont);
    txt.setCharacterSize(50);
    txt.setPosition(gameWidth/2.f -200, gameHeight/2.f);
    txt.setColor(sf::Color::White);
    txt.setString("WELCOME TO WORCHBOUND");


    sf::Clock clock;
    Vector wind = wp.generateWind();
    acc.x = wind.x;
    acc.y = gravity;
    while(window.isOpen())
    {

        tank1.setPosition(gameWidth/10.f, gameHeight - 50);
        tank2.setPosition(8 * gameWidth/10.f, gameHeight - 50);



        sf::Event event;
        while(window.pollEvent(event))
        {
            if((event.type == sf::Event::Closed) ||
            ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
        }

        if(event.type == sf::Event::MouseMoved)
        {
            if(!setAngle)
            {
                sf::Vector2i pos = sf::Mouse::getPosition(window);
                float mousey = (float) pos.y;
                if(turn1) tankCenter.x = (gameWidth/10.f) + 35;
                if(turn2) tankCenter.x = (8 * (gameWidth/10.f) + 35);
                tankCenter.y = gameHeight - 25;
                clampy = wp.clamp(mousey, tankCenter.y - radiusAim, tankCenter.y);
                float x;
                if(turn1)x = std::sqrt(std::pow(100, 2.f) - (std::pow((clampy - tankCenter.y), 2.f))) + tankCenter.x;
                if(turn2)x = std::sqrt(std::pow(100, 2.f) - (std::pow((clampy - tankCenter.y), 2.f))) + tankCenter.x;
                if(turn1)clampx = wp.clamp(x, tankCenter.x, tankCenter.x + radiusAim);
                if(turn2)clampx = wp.clamp(x, tankCenter.x, tankCenter.x - radiusAim);
                cannon.setPosition(clampx, clampy);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if(turn1) fillPowerbar.setPosition(tank1.getPosition().x, tank1.getPosition().y - 30);
            if(turn2) fillPowerbar.setPosition(tank2.getPosition().x - 50, tank1.getPosition().y - 30);
            fillPowerbar.setFillColor(sf::Color::Red);
            if(emptyPowerbar)
            {
                power++;
                if(power == 11)
                {
                    fullPowerbar = true;
                    emptyPowerbar = false;
                }
            }
            if(fullPowerbar)
            {
                power--;
                if(power == 1)
                {
                    fullPowerbar = false;
                    emptyPowerbar = true;
                }
            }
            fillPowerbar.setSize(sf::Vector2f(power * (powerSize.x/10.f), powerSize.y));
            setAngle = true;
        }
        if((event.type == sf::Event::KeyReleased))
           {
                fillPowerbar.setFillColor(sf::Color::Black);
                fillPowerbar.setPosition(0, 0);
                float deltaY = tankCenter.y - clampy;
                float deltaX = clampx - tankCenter.x;
                //std::cout << deltaX << " " << deltaY << " " <<endl;
                angle = (std::atan(deltaY/deltaX) * (180/PI));
                initVelo = wp.initialVelocity((float) power *40.0, angle);
                if(turn2) initVelo.x = -1 * initVelo.x;
                //initVelo = Vector(((float) power *10000) * std::cos(angle), -1 * ((float) power *10000) * std::sin(angle));
                //std::cout << initVelo.y << " " << endl;
                isFiring = true;
           }

        //State where the ball is fired
        if(isFiring)
        {
                sf::Vector2f cur = cannon.getPosition();
                Vector c(cur.x, cur.y);
                Vector a;
                a = wp.verletPhysics(c, prevPos, initVelo, acc, dt , first);
                prevPos.x = cur.x;
                prevPos.y = cur.y;
                cannon.setPosition(a.x, a.y);
                //std::cout << a.x << " " << a.y << " " <<endl;
                Vector cen(cur.x + radiusCannon, cur.y + radiusCannon);
                if(turn2) tankCenter.x = (gameWidth/10.f) + 35;
                if(turn1) tankCenter.x = (8 * (gameWidth/10.f) + 35);
                tankCenter.y = gameHeight - 25;
                if(wp.collissionAABBCircle(cen, radiusCannon, tankCenter, tankSize.x, tankSize.y))
                    {
                        if(turn1)
                        {
                            turn1 = false;
                            turn2 = true;
                        }
                        else if(turn2)
                        {
                            turn2 = false;
                            turn1 = true;
                        }
                        setAngle = false;
                        power = 2;
                        isFiring = false;
                        initVelo.x = 0;
                        initVelo.y = 0;
                        //clock.restart();
                    }
                if(cen.x + radiusCannon > gameWidth || cen.x - radiusCannon < 0)
                    {
                        if(turn1)
                        {
                            turn1 = false;
                            turn2 = true;
                        }
                        else if(turn2)
                        {
                            turn2 = false;
                            turn1 = true;
                        }
                        setAngle = false;
                        power = 2;
                        isFiring = false;
                        //clock.restart();
                        initVelo.x = 0;
                        initVelo.y = 0;
                    }
                if(cen.y + radiusCannon > gameHeight || cen.y - radiusCannon < 0)
                    {
                        if(turn1)
                        {
                            turn1 = false;
                            turn2 = true;
                            setAngle = false;
                            power = 1;
                        }
                        else if(turn2)
                        {
                            turn2 = false;
                            turn1 = true;
                            setAngle = false;
                            power = 1;
                        }
                        setAngle = false;
                        power = 2;
                        isFiring = false;
                        //clock.restart();
                        initVelo.x = 0;
                        initVelo.y = 0;
                    }
                first = false;
        }


        //Render codes
        //Clears The Screen
        window.clear(sf::Color::Black);

        //Draw objects
        window.draw(tank1);
        window.draw(tank2);
        window.draw(cannon);
        window.draw(fillPowerbar);

        //Display on Screen
        window.display();

        //Sleep
        float post = clock.getElapsedTime().asSeconds();
        if(post < dt)
        {
            sf::Time sle = sf::seconds(dt - post);
            sf::sleep(sle);
        }
        clock.restart();
    }


    return EXIT_SUCCESS;
}
