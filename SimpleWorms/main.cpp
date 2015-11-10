#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <complex>
#include <cmath>
#include <random>
#include <map>

using namespace std;

int window_width = 1200;
int window_height = 800;
int player_width = 40;
int player_height = 30;
double round_radius = 5;

bool player_1_turn = 1;
bool round_fired = 0;

double fps = 60;
sf::Time mspf = sf::milliseconds(1000/fps);

class My_Vector
{
    double x, y;

public:
    My_Vector(double, double);
    double Get_X()
    {
        return x;
    }
    double Get_Y()
    {
        return y;
    }
    void Set_X(double a)
    {
        x = a;
    }
    void Set_Y(double b)
    {
        y = b;
    }
};

My_Vector::My_Vector(double a, double b)
{
    x = a;
    y = b;
}

//double player_1_position_x = window_width/8;
//double player_1_position_y = window_height*3/4 - player_height/2;
//double player_2_position_x = window_width*7/8;
//double player_2_position_y = window_height*3/4 - player_height/2;

//double acceleration_gravity_per_frame = 9.8/fps;
//double acceleration_wind_per_frame = 0;
//string acceleration_wind_per_frame_string;
double acceleration_wind = 0;
string acceleration_wind_string;
double round_velocity;
string round_velocity_string;
//double round_velocity_x;
//double round_velocity_y;
//double round_position_x;
//double round_position_y;
int player_1_score = 0;
string player_1_score_string;
int player_2_score = 0;
string player_2_score_string;
double round_angle = 0;
string round_angle_string;
//double distance_round_x_prime;
//double distance_round_y_prime;
double distance_round_position_to_prime;

//double distance_round_player_x;
//double distance_round_player_y;
//double round_player_collision_x;
//double round_player_collision_y;

const double pi = acos(-1);

double computation_debug = 0;

string make_string(double x)
{
    ostringstream os;
    os << x;
    return os.str();
}

int main()
{
    //Vector Declarations
    My_Vector player_1_position(window_width/8, window_height*3/4 - player_height/2);
    My_Vector player_2_position(window_width*7/8, window_height*3/4 - player_height/2);
    My_Vector acceleration_gravity_per_frame_vector(0,9.8/fps);
    My_Vector acceleration_wind_per_frame_vector(0,0);
    My_Vector round_position_vector(0,0);
    My_Vector round_velocity_vector(0,0);
    My_Vector distance_round_prime_vector(0,0);


    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Simple Worms", sf::Style::Close);

    sf::RectangleShape land(sf::Vector2f(window_width, window_height/4));
    land.setFillColor(sf::Color(200,150,100));
    land.setOrigin(window_width/2, window_height/8);
    land.setPosition(window_width/2, window_height*7/8);

    //sf::CircleShape hill(window_width/6);
    //hill.setFillColor(sf::Color(200,150,100));

    sf::RectangleShape player_1(sf::Vector2f(player_width, player_height));
    player_1.setFillColor(sf::Color(100,149,237));
    player_1.setOrigin(player_width/2, player_height/2);
    //player_1.setPosition(player_1_position_x, player_1_position_y);
    player_1.setPosition(player_1_position.Get_X(), player_1_position.Get_Y());

    sf::RectangleShape player_2(sf::Vector2f(player_width, player_height));
    player_2.setFillColor(sf::Color(255,99,71));
    player_2.setOrigin(player_width/2, player_height/2);
    //player_2.setPosition(player_2_position_x, player_2_position_y);
    player_2.setPosition(player_2_position.Get_X(), player_2_position.Get_Y());

    sf::CircleShape player_1_round(round_radius);
    player_1_round.setFillColor(sf::Color(100,109,237));
    player_1_round.setOrigin(round_radius/2, round_radius/2);

    sf::CircleShape player_2_round(round_radius);
    player_2_round.setFillColor(sf::Color(255,59,71));
    player_2_round.setOrigin(round_radius/2, round_radius/2);

    sf::Font font;
    if(!font.loadFromFile("AlegreyaSans-Regular.ttf"))
    {
        return 0;
    }

    sf::Text instructions_display;
    instructions_display.setFont(font);
    //instructions_display.setCharacterSize(100);
    instructions_display.setColor(sf::Color(138,43,226));
    instructions_display.setString("A&D control angle.\nW&S control Power.\nSPACE fires the round.");
    instructions_display.setPosition(10, window_height*13/16);

    sf::Text player_turn_display;
    player_turn_display.setFont(font);
    player_turn_display.setCharacterSize(60);

    sf::Text score_player_1_display;
    score_player_1_display.setFont(font);
    score_player_1_display.setColor(sf::Color(100,149,237));
    //score_player_1_display.setPosition(player_1_position_x - player_width/2, 80);
    score_player_1_display.setPosition(player_1_position.Get_X() - player_width/2, 80);

    sf::Text score_player_2_display;
    score_player_2_display.setFont(font);
    score_player_2_display.setColor(sf::Color(255,99,71));
    //score_player_2_display.setPosition(player_2_position_x - player_width/2, 80);
    score_player_2_display.setPosition(player_2_position.Get_X() - player_width/2, 80);

    sf::Text shot_angle_display;
    shot_angle_display.setFont(font);
    shot_angle_display.setColor(sf::Color(138,43,226));
    shot_angle_display.setPosition(window_width*3/8, window_height*12/16);

    sf::Text shot_power_display;
    shot_power_display.setFont(font);
    shot_power_display.setColor(sf::Color(138,43,226));
    shot_power_display.setPosition(window_width*3/8, window_height*13/16);

    sf::Text wind_display;
    wind_display.setFont(font);
    wind_display.setColor(sf::Color(138,43,226));
    wind_display.setPosition(window_width*3/8, window_height*14/16);

    //random_device rd;
    //std::random_device rd;
    //std::uniform_int_distribution<int> uniform_dist(1, 6);

    /**
    random_device seed;
    mt19937 generator(seed());
    uniform_int_distribution<> random_range(2, 15);

    player_1_score = random_range(generator);
    **/

    mt19937 gen(time(0));
    uniform_real_distribution<> dist(-3,3);
    //acceleration_wind_per_frame = dist(gen);
    acceleration_wind = dist(gen);
    acceleration_wind_per_frame_vector.Set_X(acceleration_wind/fps);

    while (window.isOpen())
    {
        //start the clock
        sf::Clock frame_timer;

        //initiate randoms
        //mt19937 gen(time(0));
        //uniform_real_distribution<> dist(-1.0,1.0);
        //acceleration_wind_per_frame = dist(gen);


        //take inputs
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            if (round_fired == 0)
            {
                //acceleration_wind_per_frame = dist(gen);

                round_angle -= 0.5;

                if (round_angle < 0)
                {
                    round_angle = 0;
                }
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            if (round_fired == 0)
            {
                round_angle += 0.5;

                if (round_angle >180)
                {
                    round_angle = 180;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (round_fired == 0)
            {
                round_velocity -= 6;

                if (round_velocity < -1080)
                {
                    round_velocity = -1080;
                }
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            if (round_fired == 0)
            {
                round_velocity += 6;

                if (round_velocity > 0)
                {
                    round_velocity = 0;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (round_fired == 0)
            {
                round_fired = 1;

                //round_velocity_y = round_velocity/fps * cos((-90 + round_angle)*pi/180);
                //round_velocity_x = round_velocity/fps * sin((-90 + round_angle)*pi/180);

                round_velocity_vector.Set_Y(round_velocity/fps * cos((-90 + round_angle)*pi/180));
                round_velocity_vector.Set_X(round_velocity/fps * sin((-90 + round_angle)*pi/180));

                if (player_1_turn == 1)
                {
                    //round_position_x = player_1_position_x;
                    //round_position_x = player_1_position.Get_X();
                    round_position_vector.Set_X(player_1_position.Get_X());

                    //round_position_y = player_1_position_y;
                    //round_position_y = player_1_position.Get_Y();
                    round_position_vector.Set_Y(player_1_position.Get_Y());


                    //round_velocity_x = round_velocity * sin((-90 + round_angle)*pi/180);
                }
                else
                {
                    //round_position_x = player_2_position_x;
                    //round_position_x = player_2_position.Get_X();
                    round_position_vector.Set_X(player_2_position.Get_X());

                    //round_position_y = player_2_position_y;
                    //round_position_y = player_2_position.Get_Y();
                    round_position_vector.Set_Y(player_2_position.Get_Y());

                    //round_velocity_x = -1 * round_velocity * sin((-90 + round_angle)*pi/180);
                }
            }
        }

        //simulate

        //displays
        player_1_score_string = "P1: ";
        player_1_score_string.append(make_string(player_1_score));
        score_player_1_display.setString(player_1_score_string);

        player_2_score_string = "P2: ";
        player_2_score_string.append(make_string(player_2_score));
        score_player_2_display.setString(player_2_score_string);

        round_angle_string = "Angle: ";
        round_angle_string.append(make_string(round_angle));
        round_angle_string.append(" degrees");
        shot_angle_display.setString(round_angle_string);

        round_velocity_string = "Power: ";
        round_velocity_string.append(make_string(round_velocity*-1));
        //round_velocity_string.append(make_string(round_velocity*-1*60));
        round_velocity_string.append(" units / second");
        //round_velocity_string.append(" units / second");
        shot_power_display.setString(round_velocity_string);

        //acceleration_wind_per_frame_string = "Wind: ";
        acceleration_wind_string = "Wind: ";
        //acceleration_wind_per_frame_string.append(make_string(acceleration_wind_per_frame*60));
        acceleration_wind_string.append(make_string(acceleration_wind));
        //acceleration_wind_per_frame_string.append(" units / second");
        acceleration_wind_string.append(" units / second");
        //wind_display.setString(acceleration_wind_per_frame_string);
        wind_display.setString(acceleration_wind_string);

        //core
        if (player_1_turn == 1)
        {
            player_turn_display.setString("Player 1's Turn");
            player_turn_display.setColor(sf::Color(100,149,237));

            //player_1_round.setPosition(round_position_x, round_position_y);
            player_1_round.setPosition(round_position_vector.Get_X(), round_position_vector.Get_Y());

            //distance_round_x_prime = max(player_2_position_x-player_width/2, min(player_2_position_x+player_width/2, round_position_x));
            //distance_round_x_prime = max(player_2_position.Get_X()-player_width/2, min(player_2_position.Get_X()+player_width/2, round_position_x));
            //distance_round_prime_vector.Set_X(max(player_2_position.Get_X()-player_width/2, min(player_2_position.Get_X()+player_width/2, round_position_x)));
            distance_round_prime_vector.Set_X(max(player_2_position.Get_X()-player_width/2, min(player_2_position.Get_X()+player_width/2, round_position_vector.Get_X())));

            //distance_round_y_prime = max(player_2_position_y-player_height/2, min(player_2_position_y+player_height/2, round_position_y));
            //distance_round_y_prime = max(player_2_position.Get_Y()-player_height/2, min(player_2_position.Get_Y()+player_height/2, round_position_y));
            //distance_round_prime_vector.Set_Y(max(player_2_position.Get_Y()-player_height/2, min(player_2_position.Get_Y()+player_height/2, round_position_y)));
            distance_round_prime_vector.Set_Y(max(player_2_position.Get_Y()-player_height/2, min(player_2_position.Get_Y()+player_height/2, round_position_vector.Get_Y())));

            //distance_round_position_to_prime = pow(round_position_x-distance_round_x_prime, 2) + pow(round_position_y-distance_round_y_prime, 2);
            //distance_round_position_to_prime = pow(round_position_x-distance_round_prime_vector.Get_X(), 2) + pow(round_position_y-distance_round_prime_vector.Get_Y(), 2);
            distance_round_position_to_prime = pow(round_position_vector.Get_X()-distance_round_prime_vector.Get_X(), 2) + pow(round_position_vector.Get_Y()-distance_round_prime_vector.Get_Y(), 2);

            //player_1_score = distance_round_position_to_prime;
            //player_2_score = distance_round_y_prime;

            if (distance_round_position_to_prime < pow(round_radius,2))
            {
                round_fired = 0;
                //player_1_round.setPosition(round_position_x, round_position_y);
                player_1_round.setPosition(round_position_vector.Get_X(), round_position_vector.Get_Y());
                player_1_turn = !player_1_turn;
                player_1_score++;
                //acceleration_wind_per_frame = dist(gen);
                acceleration_wind = dist(gen);
                acceleration_wind_per_frame_vector.Set_X(acceleration_wind/fps);
            }
        }
        else
        {
            player_turn_display.setString("Player 2's Turn");
            player_turn_display.setColor(sf::Color(255,59,71));

            //player_2_round.setPosition(round_position_x, round_position_y);
            player_2_round.setPosition(round_position_vector.Get_X(), round_position_vector.Get_Y());

            //distance_round_x_prime = max(player_1_position_x-player_width/2, min(player_1_position_x+player_width/2, round_position_x));
            //distance_round_x_prime = max(player_1_position.Get_X()-player_width/2, min(player_1_position.Get_X()+player_width/2, round_position_x));
            //distance_round_prime_vector.Set_X(max(player_1_position.Get_X()-player_width/2, min(player_1_position.Get_X()+player_width/2, round_position_x)));
            distance_round_prime_vector.Set_X(max(player_1_position.Get_X()-player_width/2, min(player_1_position.Get_X()+player_width/2, round_position_vector.Get_X())));


            //distance_round_y_prime = max(player_1_position_y-player_height/2, min(player_1_position_y+player_height/2, round_position_y));
            //distance_round_y_prime = max(player_1_position.Get_Y()-player_height/2, min(player_1_position.Get_Y()+player_height/2, round_position_y));
            //distance_round_prime_vector.Set_Y(max(player_1_position.Get_Y()-player_height/2, min(player_1_position.Get_Y()+player_height/2, round_position_y)));
            distance_round_prime_vector.Set_Y(max(player_1_position.Get_Y()-player_height/2, min(player_1_position.Get_Y()+player_height/2, round_position_vector.Get_Y())));

            //distance_round_position_to_prime = pow(round_position_x-distance_round_x_prime, 2) + pow(round_position_y-distance_round_y_prime, 2);
            //distance_round_position_to_prime = pow(round_position_x-distance_round_prime_vector.Get_X(), 2) + pow(round_position_y-distance_round_prime_vector.Get_Y(), 2);
            distance_round_position_to_prime = pow(round_position_vector.Get_X()-distance_round_prime_vector.Get_X(), 2) + pow(round_position_vector.Get_Y()-distance_round_prime_vector.Get_Y(), 2);

            //player_1_score = distance_round_position_to_prime;
            //player_2_score = distance_round_y_prime;

            if (distance_round_position_to_prime < pow(round_radius,2))
            {
                round_fired = 0;
                //player_2_round.setPosition(round_position_x, round_position_y);
                player_2_round.setPosition(round_position_vector.Get_X(), round_position_vector.Get_Y());
                player_1_turn = !player_1_turn;
                player_2_score++;
                //acceleration_wind_per_frame = dist(gen);
                acceleration_wind = dist(gen);
                acceleration_wind_per_frame_vector.Set_X(acceleration_wind/fps);
            }
        }

        if (round_fired == 1)
        {
            //round_velocity_x += acceleration_wind_per_frame;
            //round_velocity_x += acceleration_wind_per_frame_vector.Get_X();
            round_velocity_vector.Set_X(round_velocity_vector.Get_X() + acceleration_wind_per_frame_vector.Get_X());

            //round_velocity_y += acceleration_gravity_per_frame;
            //round_velocity_y += acceleration_gravity_per_frame_vector.Get_Y();
            round_velocity_vector.Set_Y(round_velocity_vector.Get_Y() + acceleration_gravity_per_frame_vector.Get_Y());

            //player_1_round.setPosition(round_position_x, round_position_y);

            //round_position_x += round_velocity_x;
            //round_position_x += round_velocity_vector.Get_X();
            round_position_vector.Set_X(round_position_vector.Get_X() + round_velocity_vector.Get_X());

            //round_position_y += round_velocity_y;
            //round_position_y += round_velocity_vector.Get_Y();
            round_position_vector.Set_Y(round_position_vector.Get_Y() + round_velocity_vector.Get_Y());

        }


        /**
        if (player_1_turn == 1)
        {
            player_turn_display.setString("Player 1's Turn");
            player_turn_display.setColor(sf::Color(100,149,237));

            player_1_round.setPosition(round_position_x, round_position_y);
        }
        else
        {
            player_turn_display.setString("Player 2's Turn");
            player_turn_display.setColor(sf::Color(255,59,71));

            player_2_round.setPosition(round_position_x, round_position_y);
        }
        **/

        //boundary collisions
        if ((round_position_vector.Get_X() < 0) || (round_position_vector.Get_X() >= window_width) || (round_position_vector.Get_Y() >= window_height*3/4))
        {
            player_1_turn = !player_1_turn;
            round_fired = 0;

            if (player_1_turn == 1)
            {
                //round_position_x = player_1_position.Get_X();
                //round_position_y = player_1_position.Get_Y();

                round_position_vector.Set_X(player_1_position.Get_X());
                round_position_vector.Set_Y(player_1_position.Get_Y());
            }
            else
            {
                //round_position_x = player_2_position_x;
                //round_position_x = player_2_position.Get_X();
                //round_position_y = player_2_position_y;
                //round_position_y = player_2_position.Get_Y();

                round_position_vector.Set_X(player_2_position.Get_X());
                round_position_vector.Set_Y(player_2_position.Get_Y());
            }
            //round_velocity_x = 0;
            round_velocity_vector.Set_X(0);

            //round_velocity_y = 0;
            round_velocity_vector.Set_Y(0);

            //acceleration_wind_per_frame = dist(gen);
            acceleration_wind = dist(gen);
            acceleration_wind_per_frame_vector.Set_X(acceleration_wind/fps);
        }


        //Draw
        window.clear();

        window.draw(land);
        //window.draw(hill);
        window.draw(instructions_display);
        window.draw(player_turn_display);
        window.draw(score_player_1_display);
        window.draw(score_player_2_display);
        window.draw(shot_angle_display);
        window.draw(shot_power_display);
        window.draw(wind_display);

        window.draw(player_1);
        window.draw(player_2);

        if (round_fired == 1)
        {
            if (player_1_turn == 1)
            {
                window.draw(player_1_round);
            }
            else
            {
                window.draw(player_2_round);
            }
        }

        //final display
        window.display();

        //Synchronize Time Step
        sf::Time loop_duration = frame_timer.getElapsedTime();

        if (loop_duration < mspf)
        {
            sf::sleep(mspf - loop_duration);
        }
        frame_timer.restart();
    }

    return 0;
}
