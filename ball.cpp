//
// Created by srivatsan on 13/01/26.
//

#include "ball.h"

#include <complex>


#include <iostream>


ball::ball(vec2<float> vel_, vec2<float> pos_, float m_, float r_) : vel(vel_), pos(pos_) {
    m = m_;
    r = r_;
}

ball::ball(vec2<float> vel_, vec2<float> pos_, float m_, float r_, float e_) : vel(vel_), pos(pos_)  {
    m = m_;
    r = r_;
    e = e_;
}

ball::ball(vec2<float> pos_):  pos(pos_) {}

void ball::update(const float dt, vec2<float> &forces) {
    accel = scalar_mul_1d(forces, 1/m);

    pos = two_vector_1d_addorsub<float>(pos,scalar_mul_1d<float>(accel, dt*dt/2),ADD);
    pos = two_vector_1d_addorsub(scalar_mul_1d<float>(vel, dt), pos, ADD);

    vel = two_vector_1d_addorsub<float>(
        scalar_mul_1d<float>(accel, dt),
        vel,
        ADD
    );
}

void ball::push_balls_apart( ball &other_ball) {
    vec2<float> normal = two_vector_1d_addorsub<float>(other_ball.pos , pos, SUB);
    float norm_mag_before_normalization = vect_mag<float>(normal);
    normal = norm_mag_before_normalization == 0 ?  vec2<float>{0,0} : scalar_mul_1d(normal, 1/norm_mag_before_normalization);

    float distance_to_be_pushed = r + other_ball.r - norm_mag_before_normalization;
    pos = two_vector_1d_addorsub(pos,scalar_mul_1d(normal, distance_to_be_pushed * other_ball.r/(m+other_ball.m)), SUB);
    other_ball.pos = two_vector_1d_addorsub(other_ball.pos,scalar_mul_1d(normal, distance_to_be_pushed * r/(m+other_ball.m)), ADD);
}

void ball::collision_ball_ball( ball &other_ball) {
    vec2<float> normal = two_vector_1d_addorsub<float>(other_ball.pos , pos, SUB);
    float norm_mag_before_normalization = vect_mag(normal);
    normal = norm_mag_before_normalization == 0 ? vec2<float>{0,0} : scalar_mul_1d(normal, 1/norm_mag_before_normalization);

    vec2<float> sub_vec1 = scalar_mul_1d<float>(
        normal,
        norm_mag_before_normalization == 0 ? 0 : dot_product<float>(vel, normal));

    vec2<float> sub_vec2 = scalar_mul_1d<float>(
        normal,
        norm_mag_before_normalization == 0 ? 0: dot_product<float>(other_ball.vel, normal));

    vec2<float> newsub1 = two_vector_1d_addorsub<float>(
        scalar_mul_1d<float>(sub_vec1, (m-(e + other_ball.e)/2*other_ball.m)/(m+other_ball.m)),
        scalar_mul_1d<float>(sub_vec2, ((1 + (e + other_ball.e)/2)*other_ball.m)/(m+other_ball.m)),
        ADD
    );

    vec2<float> newsub2 = two_vector_1d_addorsub<float>(
        scalar_mul_1d<float>(sub_vec1, ((1 + (e + other_ball.e)/2)*m)/(m+other_ball.m)),
        scalar_mul_1d<float>(sub_vec2, (-(e + other_ball.e)/2*m+other_ball.m)/(other_ball.m+m)),
        ADD
    );

    // std::vector<float> newsub1 = two_vector_1d_addorsub<float>(
    //     scalar_mul_1d<float>(sub_vec1, (m-0.1*other_ball.m)/(m+other_ball.m)),
    //     scalar_mul_1d<float>(sub_vec2, (1.1*other_ball.m)/(m+other_ball.m)),
    //     ADD
    // );
    //
    // std::vector<float> newsub2 = two_vector_1d_addorsub<float>(
    //     scalar_mul_1d<float>(sub_vec1, (1.1*m)/(m+other_ball.m)),
    //     scalar_mul_1d<float>(sub_vec2, (-0.1*m+other_ball.m)/(other_ball.m+m)),
    //     ADD
    // );

    vel = two_vector_1d_addorsub<float>(vel, scalar_mul_1d<float>(sub_vec1, -1), ADD);
    vel = two_vector_1d_addorsub<float>(vel, newsub1, ADD);

    other_ball.vel = two_vector_1d_addorsub<float>(other_ball.vel, sub_vec2, SUB);
    other_ball.vel = two_vector_1d_addorsub<float>(other_ball.vel, newsub2, ADD);

}

void ball::collision_wall(WALLS wall) {
    if (wall == TOP_WALL || wall == BOTTOM_WALL) vel.y = -e*vel.y;
    else vel.x = -e*vel.x;
}