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

//The game is in fixed time-stamp
//set in 30 frames per second


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
bool newRound = true;
int score1st = 0;
int score2nd = 0;


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
    //"WIND" text object
    sf::Text txt;
    txt.setFont(customFont);
    txt.setCharacterSize(200);
    //txt.setPosition(gameWidth/2.f -200, gameHeight/2.f);
    txt.setColor(sf::Color::White);
    txt.setPosition((gameWidth/2.f) - 100,-150);
    //txt.setString("123456789101112131415161718192021222324252627282930AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");

    //Wind value text object
    sf::Text windVal;
    windVal.setFont(customFont);
    windVal.setCharacterSize(70);
    windVal.setColor(sf::Color::White);
    windVal.setPosition((gameWidth/2.f) - 100,0);

    //Score for Tank 1
    sf::Text score1;
    score1.setFont(customFont);
    score1.setCharacterSize(70);
    score1.setColor(sf::Color::White);
    score1.setPosition((gameWidth/10.f) + 35,0);

    //Score for Tank 2
    sf::Text score2;
    score2.setFont(customFont);
    score2.setCharacterSize(70);
    score2.setColor(sf::Color::White);
    score2.setPosition(8 * (gameWidth/10.f) + 35,0);


    sf::Clock clock;

    //Set gravity as y component for acceleration vector
    acc.y = gravity;
    while(window.isOpen())
    {
        txt.setString("WIND:");
        //Generate Wind
        if(newRound == true)
        {
            Vector wind = wp.generateWind();
            acc.x = wind.x;
            newRound = false;
        }
        //Set wind indication as an arrow
        if(acc.x < 0)
        {
            float intensity = -1 * std::round(acc.x/50);
            string arrow = "<";
            for(int i = 0; i < intensity * 2; i++) arrow = arrow + "-";
            windVal.setString(arrow);
        }
        else if(acc.x > 0)
        {
            float intensity = std::round(acc.x/50);
            string arrow = "";
            for(int i = 0; i < intensity * 2; i++) arrow = arrow + "-";
            arrow = arrow + ">";
            windVal.setString(arrow);
        }
        else if(acc.x == 0) windVal.setString("No wind is occuring");

        //Set the Scoreboard
        score1.setString(std::to_string(score1st));
        score2.setString(std::to_string(score2nd));

        //Set tank position
        tank1.setPosition(gameWidth/10.f, gameHeight - 50);
        tank2.setPosition(8 * gameWidth/10.f, gameHeight - 50);

        sf::Event event;
        while(window.pollEvent(event))
        {
            //When escape key is pressed or window is closed, game ends
            if((event.type == sf::Event::Closed) ||
            ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }
        }

        //Aiming movement
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
                if(turn2)x = -1 * (std::sqrt(std::pow(100, 2.f) - (std::pow((clampy - tankCenter.y), 2.f))) - tankCenter.x);
                if(turn1)clampx = wp.clamp(x, tankCenter.x, tankCenter.x + radiusAim);
                if(turn2)clampx = wp.clamp(x, tankCenter.x - radiusAim, tankCenter.x);
                //std::cout << x <<endl;
                cannon.setPosition(clampx, clampy);
            }
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            //Charging power
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
            //Charging and aiming is finished
           {
                fillPowerbar.setFillColor(sf::Color::Black);
                fillPowerbar.setPosition(0, 0);
                float deltaY = tankCenter.y - clampy;
                float deltaX = clampx - tankCenter.x;
                if(turn2) deltaX *= -1;
                //std::cout << deltaX << " " << deltaY << " " <<endl;
                angle = (std::atan(deltaY/deltaX) * (180/PI));
                //std::cout << angle <<endl;
                initVelo = wp.initialVelocity((float) power *40.0, angle);
                if(turn2) initVelo.x *= -1;
                //if(turn2) initVelo.x = -1 * initVelo.x;
                //initVelo = Vector(((float) power *10000) * std::cos(angle), -1 * ((float) power *10000) * std::sin(angle));
                //std::cout << initVelo.y << " " << endl;
                isFiring = true;
                //std::cout<< "A" << endl;
                first = true;
           }

        //State where the ball is fired
        if(isFiring)
        {
                std::cout << first;
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
                            score1st++;
                        }
                        else if(turn2)
                        {
                            turn2 = false;
                            turn1 = true;
                            score2nd++;
                        }
                        setAngle = false;
                        power = 2;
                        newRound = true;
                        cannon.setPosition(0,0);
                        //first = true;
                        //clock.restart();
                    }
                if(a.x > gameWidth || a.x < 0)
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
                        newRound = true;
                        cannon.setPosition(0,0);
                        //first = true;
                        //clock.restart();
                    }
                if(a.y > gameHeight || a.y < 0)
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
                        newRound = true;
                        cannon.setPosition(0,0);
                        //first = true;
                        //clock.restart();
                    }
                first = false;
                //std::cout<< "X: " << a.x <<endl;
                //std::cout<< "Y: " << a.y <<endl;
        }


        //Render codes
        //Clears The Screen
        window.clear(sf::Color::Black);

        //Draw objects
        window.draw(tank1);
        window.draw(tank2);
        window.draw(cannon);
        window.draw(fillPowerbar);
        window.draw(txt);
        window.draw(windVal);
        window.draw(score1);
        window.draw(score2);
        //Display on Screen
        window.display();

        //Sleep
        //time translated to seconds
        float post = clock.getElapsedTime().asSeconds();
        if(post < dt)
        {
            sf::Time sle = sf::seconds(dt - post);
            sf::sleep(sle);
        }

        //Restart clock
        clock.restart();
    }
    return EXIT_SUCCESS;
}
