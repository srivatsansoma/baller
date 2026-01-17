//
// Created by srivatsan on 13/01/26.
//
#pragma once


#ifndef BALLER_RECT_AREA_H
#define BALLER_RECT_AREA_H

#define MAX_BALLS_IN_SUBRREG 10
#define MIN_SIZE 10

#include "ball.h"
#include "vector_ops.h"

// class subcell_quadtree {
// public:
//     std::vector<ball*> ball_pointers = {};
//     float OFFSET_X = 0, OFFSET_Y = 0;
//     float HEIGHT = 0, WIDTH = 0;
//
//     subcell_quadtree(std::vector<ball*> ball_pointers_, float OFFSET_X_, float OFFSET_Y_, float HEIGHT_, float WIDTH_):
//         ball_pointers(ball_pointers_),
//         OFFSET_X(OFFSET_X_),
//         OFFSET_Y(OFFSET_Y_),
//         HEIGHT(HEIGHT_),
//         WIDTH(WIDTH_) {};
//
//
//     void do_collisions_ball2ball_quadtree(int num_pushaparts) {
//     if (ball_pointers.size() > MAX_BALLS_IN_SUBRREG && WIDTH/2 >= MIN_SIZE && HEIGHT/2 >= MIN_SIZE) {
//         std::vector<subcell_quadtree> sub_cells = {};
//         sub_cells.push_back(subcell_quadtree({}, OFFSET_X,           OFFSET_Y,            HEIGHT/2, WIDTH/2));
//         sub_cells.push_back(subcell_quadtree({}, OFFSET_X + WIDTH/2, OFFSET_Y,            HEIGHT/2, WIDTH/2));
//         sub_cells.push_back(subcell_quadtree({}, OFFSET_X + WIDTH/2, OFFSET_Y + HEIGHT/2, HEIGHT/2, WIDTH/2));
//         sub_cells.push_back(subcell_quadtree({}, OFFSET_X,           OFFSET_Y + HEIGHT/2, HEIGHT/2, WIDTH/2));
//
//         for (ball* ball: ball_pointers) {
//             bool left = (ball->pos.x - OFFSET_X < WIDTH/2);
//             bool bottom = (ball->pos.y - OFFSET_Y < HEIGHT/2);
//
//             if (left && bottom) {
//                 sub_cells[0].ball_pointers.push_back(ball);
//             }
//             else if (left && !bottom) {
//                 sub_cells[3].ball_pointers.push_back(ball);
//             }
//             else if (!left && bottom) {
//                 sub_cells[1].ball_pointers.push_back(ball);
//             }
//             else {
//                 sub_cells[2].ball_pointers.push_back(ball);
//             }
//         }
//
//         for (auto& sub_cell : sub_cells) {
//             sub_cell.do_collisions_ball2ball_quadtree(num_pushaparts);
//         }
//     }
//
//     else {
//         for (int i = 0 ; i < ball_pointers.size(); i++) {
//             for (int j = 0 ; j < ball_pointers.size(); j++) {
//                 float dx = ball_pointers[i]->pos.x - ball_pointers[j]->pos.x;
//                 float dy = ball_pointers[i]->pos.y -ball_pointers[j]->pos.y;
//                 bool close = (1.5*(ball_pointers[i]->r + ball_pointers[j]->r)*(ball_pointers[i]->r +ball_pointers[j]->r) >= dx*dx + dy*dy);
//                 if (close) {
//                     if (j > i) ball_pointers[i]->collision_ball_ball(*ball_pointers[j]);
//                     for (int k = 0 ; k < num_pushaparts; k ++ )
//                         ball_pointers[i]->push_balls_apart(*ball_pointers[j]);
//                     //std::cout << "balls " << i << " and " << j << " collided." << "\n";
//                 }
//             }
//         }
//     }
// }
// };

class rect_area {
    public:
    float WIDTH = 100, HEIGHT = 100;
    float ORGIN_OFFSETX = 0, ORGIN_OFFSETY = 0;

    std::vector<ball> balls;
    std::vector<vec2<float>*> force_pointers = {};

    bool Wall_Top = true, Wall_Bottom = true, Wall_Right = true, Wall_Left = true;

    rect_area(float width_, float height_, std::vector<ball>& balls_);
    rect_area(float ORGIN_OFFSETX_, float ORGIN_OFFSETY_, float width_, float height_, std::vector<ball>& balls_);
    rect_area(float ORGIN_OFFSETX_, float ORGIN_OFFSETY_, float width_, float height_, std::vector<ball>& balls_, bool WLeft_, bool WRight_, bool WTop_, bool WBottom_) ;

    void do_collisions_ball2ball_quadtree(int num_pushaparts);
    void do_collisions_ball2ball_fixedgridsize(int h_divide, int w_divide, int max_threads) ;
    void do_collisions_ball2wall() ;
    void cell_collisions_with_forward_neighbours_multiple_rows(std::vector<std::vector<std::vector<int>>>& sub_cells, int i_start, int i_end, int h_divide, int w_divide)  ;
    void join_and_collide(std::vector<std::vector<std::vector<int>>>& sub_cells, int h_divide, int w_divide, int i, int j, int i_neighbour, int j_neighbour);

    bool is_ball_in(ball &b);

    void update_balls_noforces(float dt);
    void update_balls(float dt);
    void cout_balls_data();
};


#endif //BALLER_RECT_AREA_H