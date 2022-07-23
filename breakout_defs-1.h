#include <SFML/Graphics.hpp>
/* --------------------------------------------------------
 *    File: breakout_def.h
 *  Author: Anthony Stephens JR
 *   Class: Your Class (2001 Programming Methodology # Spring 2022 CRN 10410)
 * Purpose: paddle ball game definitions
 * -------------------------------------------------------- */

#ifndef PADDLE_BREAKOUT_DEFS_H
#define PADDLE_BREAKOUT_DEFS_H

// Global Constants
// --------------------------------------------------------
// window properties
const float WALL_THICKNESS = 2.0;
const sf::Color WALL_COLOR = sf::Color::Blue;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const sf::Color WINDOW_COLOR = sf::Color::Black;
// drawing properties


const float FRAME_RATE = (1.0/30.0) * 1000.0;       // FPS in ms
const sf::Color BALL_COLOR = sf::Color::Red;
// These are just for fun
// speed that the can accelerate at to span window in
// n-seconds (in ms) broken up for each frame

const float BALL_RADIUS = 10.0;
const float SPEED_TIME = (3.0 * 1000.0) * 30.0;     //
const float BALL_SPEED_X = BALL_RADIUS * 10.0/1000.0;    // speed horizontally
const float BALL_SPEED_Y = BALL_RADIUS * 8.5/1000.0;   // span  vertically

const float PADDLE_WIDTH = 80.0;
const float PADDLE_THICKNESS = 10.0;
sf::Color PADDLE_COLOR = sf::Color::White;
const float PADDLE_SPEED = PADDLE_WIDTH/10.0/1000.0;

// ---- brick constants ----
const int BRICK_ROWS = 8;
const int BRICK_COLUMNS = 14;
const float BRICK_WIDTH = WINDOW_WIDTH/BRICK_COLUMNS;
const float BRICK_HEIGHT = PADDLE_THICKNESS + 2;
const float BRICKS_HEIGHT = BRICK_HEIGHT * BRICK_ROWS;
const float BRICKS_TOP = WINDOW_HEIGHT/2.0 - BRICKS_HEIGHT + 0.75;
const float BRICKS_LEFT = WALL_THICKNESS;
const float FIRST_BRICK =  BRICKS_TOP + (BRICK_ROWS - 1) * BRICK_HEIGHT;
// Type definitions
// --------------------------------------------------------
enum Direction {
    Exit = -1,
    None = 0,
    Left = 1,
    Up = 2,
    Right = 3,
    Down = 4,
    Start = 5,
};
//wall properties
struct Block{
    float left;
    float top;
    float width;
    float height;
    sf::Color blockColor;
    sf::RectangleShape recShape;
};

struct MovingBlock{
    Block generalProps;
    float blockVeloX;
    float blockVeloY;

};

struct Borders{
    Block leftB;
    Block topB;
    Block rightB;
    Block bottomB;
};


// theBall properties
struct Ball{
    float radius;
    float coordinateX;
    float coordinateY;
    float velocityX;
    float velocityY;
    sf::Color color;

};

// Brick properties
struct Brick{
    Block block;
    bool hit;
    int points;
    float speedAdjust;

};

#endif //PADDLE_BREAKOUT_DEFS_H