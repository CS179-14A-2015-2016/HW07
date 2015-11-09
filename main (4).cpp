
//

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <random>

using namespace std;

//for font
#include "ResourcePath.hpp"

#define PI 3.14159265
int chance()
{
    // BLACK MAGIC MUMBO JUMBO RANDOM
    random_device random;
    uniform_real_distribution<double> distribution(-3,3);
    double r = distribution(random);
    return r;
}

int main(int, char const**)
{
    
    // Create the main window
    sf::RenderWindow window(sf::VideoMode(1200, 600), "WorchBound");
    
    
    // Create players and land
    sf::RectangleShape player1(sf::Vector2f(80, 20));
    player1.setFillColor(sf::Color(255, 255, 255));
    player1.setPosition(20, 450);
    
    sf::RectangleShape player2(sf::Vector2f(80, 20));
    player2.setFillColor(sf::Color(255, 255, 255));
    player2.setPosition(1100, 450);
    
    sf::RectangleShape land(sf::Vector2f(1200,600));
    land.setPosition(0, 480);
    land.setFillColor(sf::Color(200, 155, 45));
    
    // Create Rounds
    sf::CircleShape round(10);
    round.setPosition(60,450);
    
    sf::CircleShape round2(10);
    round2.setPosition(1140,450);
    
    //Variable declarations
    int angle = 0;
    int power = 0;
    int shotangle = 0;
    int shotpower = 0;
    
    double vx = 0;
    double vy = 0;
    double gravity = -5;
    
    sf::Time delay = sf::seconds(0.1);
    
    bool isShooting = false;
    bool winner = false;
    bool player1turn = true;
    
    string turn= "";
    string win = "";
    double wind = chance();
    
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(resourcePath() + "sansation.ttf")) {
        return EXIT_FAILURE;
    }
    
    window.setFramerateLimit(60);

    
    // Start the game loop
    while (window.isOpen())
    {
        
        string ang = "Angle: " + to_string(angle);
        string pow = "Power: " + to_string(power);
        string air = "Wind: " + to_string(wind);
        if(player1turn)
        {
            turn = "Player 1 turn";
        }
        else
        {
            turn = "Player 2 turn";
        }
        
        sf::Text angtext(ang, font, 25);
        angtext.setColor(sf::Color::White);
        angtext.setPosition(25, 20);
        sf::Text powtext(pow, font, 25);
        powtext.setColor(sf::Color::White);
        powtext.setPosition(175, 20);
        sf::Text turntext(turn, font, 25);
        turntext.setColor(sf::Color::White);
        turntext.setPosition(500, 20);
        sf::Text airtext(air, font, 25);
        airtext.setColor(sf::Color::White);
        airtext.setPosition(700, 20);
        
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            
            // Close window: exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Escape pressed: exit
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        
        
        
        
        //inputs
        if(!isShooting && !winner)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                if(angle <180)
                {
                    angle++;
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (angle > 0){
                    angle--;
                }
            }
            
            
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if(power <100)
                {
                    power++;
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if (power > 0){
                    power--;
                }
            }
            
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                shotangle = angle;
                shotpower = power;
                
                isShooting = true;
                
                vx = cos(shotangle*PI/180) * shotpower;
                vy = sin(shotangle*PI/180) * shotpower;
                
                
            }
        }
        //Processing of inputs
        else
        {
            if(player1turn)
            {
                if(rand >= 0)
                {
                   vx += wind;
                }
                else
                {
                    vx -= wind;
                }
                    round.move(vx,-vy);
                vy += gravity;
                if(round.getGlobalBounds().intersects(player2.getGlobalBounds()))
                {
                    isShooting= false;
                    winner = true;
                    
                }
                else if(round.getGlobalBounds().intersects(land.getGlobalBounds())|| round.getPosition().x < 0 || round.getPosition().x >1200)
                {
                    angle = 0;
                    power = 0;
                    round.setPosition(60,450);
                    wind = chance();
                    player1turn = false;
                    
                    isShooting= false;
                }
            }
            else{
                if(rand >= 0)
                {
                    vx -= wind;
                }
                else
                {
                    vx += wind;
                }
                round2.move(-vx,-vy);
                vy += gravity;
                if(round2.getGlobalBounds().intersects(player1.getGlobalBounds()))
                {
                    isShooting= false;
                    winner = true;
                }
                else if(round2.getGlobalBounds().intersects(land.getGlobalBounds()) || round2.getPosition().x < 0 || round2.getPosition().x > 1200)
                {
                    int wind = chance();
                    angle = 0;
                    power = 0;
                    round2.setPosition(1140,450);
                    isShooting= false;
                    player1turn = true;
                }
                
            }
            
        }
        
        // Clear screen
        window.clear();
        
        
        
        //Draws
        window.draw(angtext);
        window.draw(powtext);
        window.draw(turntext);
        window.draw(airtext);

        window.draw(player1);
        window.draw(player2);
        window.draw(land);
        
        if(winner)
        {
            if(player1turn)
            {
                win = "Player 1 wins!";
            }
            else{
                win = "Player 2 wins!";
            }
            sf::Text wintext(win, font, 50);
            wintext.setColor(sf::Color::White);
            wintext.setPosition(500, 300);
            window.draw(wintext);
        }
      
       
        
        if(isShooting && player1turn)
        {
            window.draw(round);
        }
        else if(isShooting&& !player1turn)
        {
            window.draw(round2);
            
        }
        
        
        sf::sleep(delay);
        
        // Update the window
        window.display();
        
        
      
    }
    
    return EXIT_SUCCESS;
}
