#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "ball.h"
#include "rect_area.h"
#include "unistd.h"
#include "vector_ops.h"

std::vector<ball> random_balls(uint32_t num_balls, float w, float h, float max_velcomp, float e) {
    std::vector<ball> balls = {};
    srand(time(NULL));
    for (int i = 0 ; i < num_balls; i++) {
        vec2<float> velocity = {static_cast<float>(rand()), static_cast<float>(rand())};
        vec2<float> pos = {static_cast<float>(rand()), static_cast<float>(rand())};

        velocity = scalar_mul_1d<float>(velocity,max_velcomp/RAND_MAX);
        pos.x *= w/RAND_MAX;
        pos.y *= h/RAND_MAX;

        balls.push_back(ball(velocity, pos, 10, 3, e));
    }

    return balls;
}

int main() {
    sf::RenderWindow window;
    window.create(sf::VideoMode({1400,1000}), "playarea", sf::Style::Close|sf::Style::Titlebar);

    std::vector<ball> balls = {};
    std::vector<vec2<float>> forces = {};
    balls = random_balls(2000, 1400, 1000, 0, 0.99);
    balls.push_back(ball({10,0}, {100,100}, 10, 10, 1));

    //std::vector<ball*> ball_pointers = {};
    for (int i = 0 ; i < balls.size(); i++) {
        //ball_pointers.push_back(&balls[i]);
        forces.push_back({0,0});
    }

    rect_area play_area(1400, 1000, balls);
    for (int i = 0; i < balls.size(); i++) {
        play_area.force_pointers[i] = (&forces[i]);
    }

    bool run_program = true;
    //play_area.do_collisions_ball2ball_fixedgridsize(100,100);
    //play_area.do_collisions_ball2ball_quadtree(2);
    play_area.do_collisions_ball2wall();
    //play_area.cout_balls_data();
    auto start_time = std::chrono::high_resolution_clock::now();
    while (run_program && window.isOpen()) {
        sf::Event event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) return 0;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

            sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
            //std::cout << mouse_pos.x << "\n";
            for (int i = 0 ; i < balls.size(); i++) {
                vec2<float> tpos = {
                    static_cast<float>(mouse_pos.x),
                    static_cast<float>(800 - mouse_pos.y)
                };
                vec2<float> r = two_vector_1d_addorsub<float>(tpos,play_area.balls[i].pos,SUB);
                float r_mag = vect_mag(r);
                forces[i] = two_vector_1d_addorsub(scalar_mul_1d(r, 0.5f*0.01f*vect_mag<float>(r)), {0,-9.81}, ADD);
                //play_area.forces.at(i) = scalar_mul_1d<float>(r, 100);
            }
        }

        srand(0);
        window.clear(sf::Color::Black);
        for (auto& ball : play_area.balls) {
            sf::CircleShape circle = sf::CircleShape(ball.r);
            float mag_vel = vect_mag(ball.vel);

            // if (mag_vel<200) circle.setFillColor(sf::Color::Red);
            // else if (mag_vel < 600) circle.setFillColor(sf::Color::Green);
            // else circle.setFillColor(sf::Color::Blue);
            float r,g,b;
            r = (float)rand()/RAND_MAX*255;
            g = (float)rand()/RAND_MAX*255;
            b = (float)rand()/RAND_MAX*255;
            circle.setFillColor(sf::Color(r,g,b));
            circle.setPosition(ball.pos.x, 1000 - ball.pos.y);
            window.draw(circle);
        }
        window.display();

        float duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start_time).count() / 1e3;
        start_time = std::chrono::high_resolution_clock::now();
        std::cout << 1/duration << "\n";

        for (int i = 0 ; i <2 ;i++) {
            play_area.update_balls( std::min(duration/2, 9.0f));

            play_area.do_collisions_ball2ball_fixedgridsize(150,100, 5);
            //play_area.do_collisions_ball2ball_quadtree(1);

            play_area.do_collisions_ball2wall();
        }


        for (int i = 0 ; i < forces.size(); i++) {
            forces[i].x = 0;
            forces[i].y = 0;
        }

        //play_area.cout_balls_data();
        //usleep(0.01 * 1e6);
    }

    return 0;
}