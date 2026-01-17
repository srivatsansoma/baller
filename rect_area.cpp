//
// Created by srivatsan on 13/01/26.
//

#include "rect_area.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>

#include "ball.h"
#include "vector_ops.h"

rect_area::rect_area(float width_, float height_, std::vector<ball>& balls_) {
    WIDTH = width_;
    HEIGHT = height_;
    balls = balls_;
    force_pointers = std::vector<vec2<float>*>(balls.size(), nullptr);
}

rect_area::rect_area(float ORGIN_OFFSETX_, float ORGIN_OFFSETY_, float width_, float height_, std::vector<ball>& balls_) {
    WIDTH = width_;
    HEIGHT = height_;
    balls = balls_;
    force_pointers = std::vector<vec2<float>*>(balls.size(), nullptr);
    ORGIN_OFFSETX = ORGIN_OFFSETX_;
    ORGIN_OFFSETY = ORGIN_OFFSETY_;
}
rect_area::rect_area(float ORGIN_OFFSETX_, float ORGIN_OFFSETY_, float width_, float height_, std::vector<ball>& balls_, bool WLeft_, bool WRight_, bool WTop_, bool WBottom_) {
    WIDTH = width_;
    HEIGHT = height_;
    ORGIN_OFFSETX = ORGIN_OFFSETX_;
    ORGIN_OFFSETY = ORGIN_OFFSETY_;
    balls = balls_;
    force_pointers = std::vector<vec2<float>*>(balls.size(), nullptr);
    Wall_Left = WLeft_;
    Wall_Right = WRight_;
    Wall_Top = WTop_;
    Wall_Bottom = WBottom_;

}

void rect_area::join_and_collide(std::vector<std::vector<std::vector<int>>>& sub_cells, int h_divide, int w_divide, int i, int j, int i_neighbour, int j_neighbour) {
    if (i_neighbour<0 || i_neighbour>=h_divide || j_neighbour<0 || j_neighbour>=w_divide) return;

    std::vector<int> sub_cell = sub_cells[i][j];
    if (i == i_neighbour && j == j_neighbour) NULL;
    else sub_cell.insert(sub_cell.end(), sub_cells[i_neighbour][j_neighbour].begin(), sub_cells[i_neighbour][j_neighbour].end());

    // for (int i = 0; i < sub_cell.size(); i++) {
    //     for (int j = i+1; j < sub_cell.size(); j++) {
    //         sub_cell[i]->collision_ball_ball(*sub_cell[j]);
    //         sub_cell[i]->push_balls_apart(*sub_cell[j]);
    //     }
    // }

    //removed sqrt to optimize
    for (int i = 0 ; i < sub_cell.size(); i++) {
        for (int j = i+1 ; j < sub_cell.size(); j++) {
            float dx = balls[sub_cell[i]].pos.x - balls[sub_cell[j]].pos.x;
            float dy = balls[sub_cell[i]].pos.y - balls[sub_cell[j]].pos.y;
            bool close = ((balls[sub_cell[i]].r + balls[sub_cell[j]].r)*(balls[sub_cell[i]].r + balls[sub_cell[j]].r) >= dx*dx + dy*dy);
            if (close) {
                balls[sub_cell[i]].collision_ball_ball(balls[sub_cell[j]]);
                balls[sub_cell[i]].push_balls_apart(balls[sub_cell[j]]);
                //std::cout << "balls " << i << " and " << j << " collided." << "\n";
            }
        }
    }
}

void rect_area::cell_collisions_with_forward_neighbours_multiple_rows(std::vector<std::vector<std::vector<int>>>& sub_cells, int i_start, int i_end, int h_divide, int w_divide) {
    for (int i = i_start; i < i_end+1; i++) {
        // std::thread t(&rect_area::cell_collisions_with_forward_neighbours_single_row,this , sub_cells, i, h_divide, w_divide);
        // t.join();
        for (int j = 0 ; j < w_divide; j++) {
            join_and_collide(sub_cells, h_divide,  w_divide, i, j, i, j+1);
            join_and_collide(sub_cells, h_divide,  w_divide, i, j, i+1, j-1);
            join_and_collide(sub_cells,  h_divide, w_divide, i, j, i+1, j);
            join_and_collide(sub_cells, h_divide,  w_divide,i, j, i+1, j+1);
        }
    }
}

void rect_area::do_collisions_ball2ball_fixedgridsize(int h_divide, int w_divide, int max_threads) {
    std::vector<std::vector<std::vector<int>>> sub_cells;
    float w_subcell = WIDTH/w_divide;
    float h_subcell = HEIGHT/h_divide;
    for (int i = 0 ; i < h_divide; i++) {
        sub_cells.push_back({});
        for (int j = 0 ; j < w_divide; j++) {
            sub_cells[i].push_back({});
        }
    }
    for (int i = 0 ; i < balls.size(); i++) {
        int subcell_x = (int)((balls[i].pos.x - ORGIN_OFFSETX) / w_subcell);
        int subcell_y = (int)((balls[i].pos.y - ORGIN_OFFSETY) / h_subcell);

        subcell_x = std::clamp(subcell_x, 0, w_divide - 1);
        subcell_y = std::clamp(subcell_y, 0, h_divide - 1);


        sub_cells[subcell_y][subcell_x].push_back(i);
    }
    std::thread cell_threads = {};

    // int rows_done = 0;
    // std::thread t1(&rect_area::cell_collisions_with_forward_neighbours_multiple_rows, this, std::ref(sub_cells), 0, (int)400, h_divide, w_divide);
    // std::thread t2(&rect_area::cell_collisions_with_forward_neighbours_multiple_rows, this, std::ref(sub_cells), 401, 800, h_divide, w_divide);
    // t1.join();
    // t2.join();
    for (int i = 0; i < sub_cells.size(); i++) {
        for (int j = 0 ; j < sub_cells[0].size(); j++) {
            join_and_collide(sub_cells, h_divide,  w_divide, i, j, i, j+1);
            join_and_collide(sub_cells, h_divide,  w_divide, i, j, i+1, j-1);
            join_and_collide(sub_cells,  h_divide, w_divide, i, j, i+1, j);
            join_and_collide(sub_cells, h_divide,  w_divide,i, j, i+1, j+1);
        }
    }
}

// void rect_area::do_collisions_ball2ball_quadtree(int num_pushaparts) {
//     if (ball_pointers.size() > MAX_BALLS_IN_SUBRREG && WIDTH/2 >= MIN_SIZE && HEIGHT/2 >= MIN_SIZE) {
//
//         std::vector<subcell_quadtree> sub_cells = {};
//         sub_cells.push_back(subcell_quadtree({}, ORGIN_OFFSETX,           ORGIN_OFFSETY,            HEIGHT/2, WIDTH/2));
//         sub_cells.push_back(subcell_quadtree({}, ORGIN_OFFSETX + WIDTH/2, ORGIN_OFFSETY,            HEIGHT/2, WIDTH/2));
//         sub_cells.push_back(subcell_quadtree({}, ORGIN_OFFSETX + WIDTH/2, ORGIN_OFFSETY + HEIGHT/2, HEIGHT/2, WIDTH/2));
//         sub_cells.push_back(subcell_quadtree({}, ORGIN_OFFSETX,           ORGIN_OFFSETY + HEIGHT/2, HEIGHT/2, WIDTH/2));
//
//         for (ball* ball: ball_pointers) {
//             bool left = (ball->pos.x - ORGIN_OFFSETX < WIDTH/2);
//             bool bottom = (ball->pos.y - ORGIN_OFFSETY < HEIGHT/2);
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
//                 bool close = ((ball_pointers[i]->r + ball_pointers[j]->r)*(ball_pointers[i]->r +ball_pointers[j]->r) >= dx*dx + dy*dy);
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

void rect_area::do_collisions_ball2wall()
{
    for (ball& ball : balls)
    {
        if (Wall_Top && ball.pos.y >= HEIGHT - ball.r) {
            ball.collision_wall(TOP_WALL);
            ball.pos.y = HEIGHT - ball.r;
        }
        if (Wall_Bottom && ball.pos.y <= ball.r) {
            ball.collision_wall(BOTTOM_WALL);
            ball.pos.y = ball.r;
        }

        if (Wall_Right && ball.pos.x >= WIDTH - ball.r) {
            ball.collision_wall(RIGHT_WALL);
            ball.pos.x = WIDTH - ball.r;
        }

        if (Wall_Left && (ball.pos.x <= ball.r)) {
            ball.collision_wall(LEFT_WALL);
            ball.pos.x = ball.r;
        }
    }
}

bool rect_area::is_ball_in(ball &b) {
    if (!(b.pos.x > ORGIN_OFFSETX && b.pos.x < ORGIN_OFFSETX + WIDTH)) return false;
    if (!(b.pos.y > ORGIN_OFFSETY && b.pos.y < ORGIN_OFFSETY + HEIGHT)) return false;

    return true;
}

void rect_area::update_balls_noforces(float dt) {
    vec2<float> t_force = {0,0};
    for (int i = 0; i < balls.size(); i++) {
        balls[i].update(dt, t_force);
    }
}

void rect_area::update_balls(float dt) {
    for (int i = 0; i < balls.size(); i++) {
        balls[i].update(dt, *force_pointers[i]);
    }
}


void rect_area::cout_balls_data() {
    for (int i = 0 ; i < balls.size(); i++) {
        std::cout << "ball no. " << i << "\n";
        std::cout << "x_pos: " << balls[i].pos.x << ", y_pos: " << balls[i].pos.y << "\n";
        std::cout << "x_vel: " << balls[i].vel.x << ", y_vel: " << balls[i].vel.y << "\n";
        std::cout << "\n";
    }
}

