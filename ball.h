//
// Created by srivatsan on 13/01/26.
//
#pragma once

#ifndef BALLER_BALL_H
#define BALLER_BALL_H

#include <vector>
#include "vector_ops.h"

enum WALLS {
    TOP_WALL,
    BOTTOM_WALL,
    LEFT_WALL,
    RIGHT_WALL
};

class ball {
    public:

    vec2<float> vel = {0,0};
    vec2<float> accel = {0,0};
    vec2<float> pos = {0,0};
    float m = 1, r = 1;
    float e = 1;

    ball(vec2<float> vel_, vec2<float> pos_, float m_, float r_);
    ball(vec2<float> vel_, vec2<float> pos_, float m_, float r_, float e_) ;
    ball(vec2<float> pos_) ;

    void update(const float dt, vec2<float> &forces) ;

    void collision_ball_ball( ball &other_ball) ;
    void push_balls_apart( ball &other_ball) ;
    void collision_wall(WALLS wall) ;

    vec3<float> rgb_color = {2500,0,0};//default is red

};


#endif //BALLER_BALL_H