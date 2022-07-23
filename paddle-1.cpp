#include "breakout_defs.h"
/* --------------------------------------------------------
 *    File: paddle.cpp
 *  Author: Anthony Stephens JR
 *   Class: Your Class (2001 Programming Methodology # Spring 2022 CRN 10410)
 * Purpose: paddle ball game
 * -------------------------------------------------------- */
#include <SFML/Graphics.hpp>
#include <cmath>

// Function declarations (prototypes)

// --------------------------------------------------------
void setup(Ball &ball, MovingBlock &paddle, Brick bricks [BRICK_ROWS][BRICK_COLUMNS], Borders &borders);

Direction processInput();

bool update(Direction &input, Ball &ball, MovingBlock &paddle, Brick bricks [BRICK_ROWS][BRICK_COLUMNS], Borders borders, float delta, bool started);

void render(sf::RenderWindow &window, Ball &ball, MovingBlock &paddle, Brick bricks[BRICK_ROWS][BRICK_COLUMNS], Borders &borders,  float delta);

int collisionPoint(Ball * pBall, Block * pBlock);

bool collisionCheck(Ball * pBall, Block * pBlock);


/**
 * The main application
 * @return OS status message (0=Success)
 */

int main() {
    Ball ball;
    MovingBlock paddle;
    Brick bricks[ BRICK_ROWS] [ BRICK_COLUMNS];
    Borders borders;
    setup(ball, paddle, bricks, borders);

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "HelloSFML");
    window.clear(WINDOW_COLOR);

    // timing variables for the main game loop
    sf::Clock clock;
    sf::Time startTime = clock.getElapsedTime();
    sf::Time stopTime = startTime;
    float delta = 0.0;
    bool gameOver = false;
    bool started = false;
    while (!gameOver){
        // calculate frame time
        stopTime = clock.getElapsedTime();
        delta += (stopTime.asMilliseconds() - startTime.asMilliseconds());
        startTime = stopTime;
        // process events

        sf::Event event;
        while (!gameOver && window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                gameOver = true;
        }
        // Process user input
        // ------------------------------------------------

        Direction userInput = processInput();
        if (userInput == Exit)
            gameOver = true;
        // Process Updates
        // ------------------------------------------------
        if (delta >= FRAME_RATE) {    // if we have made it at least a full frame time
            gameOver = update(userInput, ball, paddle, bricks, borders, delta, started);
            // subtract the frame-rate from the current frame-time
            // for each full frame covered by this update


            while (delta >= FRAME_RATE)
                delta -= FRAME_RATE;
        }
        // Render the window
        // ------------------------------------------------
        render(window, ball, paddle, bricks, borders, delta);
    } // end main game loop
    // game ending, close the graphics window
    window.close();
    return 0;

} // end main


/**
 *
 * @param ball - setup for the ball
 * @param paddle - setup for the paddle
 * @param bricks - setup for the bricks
 * @param borders - setup for the borders
 */

void setup( Ball &ball, MovingBlock &paddle, Brick bricks [BRICK_ROWS][BRICK_COLUMNS], Borders &borders) {

    borders.leftB.blockColor = WALL_COLOR;
    borders.leftB.left = 0.0;
    borders.leftB.top = 0.0;
    borders.leftB.width = WALL_THICKNESS;
    borders.leftB.height = WINDOW_HEIGHT;
    borders.leftB.recShape.setSize(sf::Vector2(borders.leftB.width, borders.leftB.height));
    borders.leftB.recShape.setPosition(borders.leftB.left, borders.leftB.top);
    borders.leftB.recShape.setFillColor(borders.leftB.blockColor);

    borders.topB.blockColor = WALL_COLOR;
    borders.topB.left = 0.0;
    borders.topB.top = 0.0;
    borders.topB.width = WINDOW_WIDTH;
    borders.topB.height = WALL_THICKNESS;
    borders.topB.recShape.setSize(sf::Vector2(borders.topB.width, borders.topB.height));
    borders.topB.recShape.setPosition(borders.topB.left, borders.topB.top);
    borders.topB.recShape.setFillColor(borders.topB.blockColor);

    borders.rightB.blockColor = WALL_COLOR;
    borders.rightB.left = WINDOW_WIDTH - WALL_THICKNESS;
    borders.rightB.top = 0.0;
    borders.rightB.width = WALL_THICKNESS;
    borders.rightB.height = WINDOW_HEIGHT;
    borders.rightB.recShape.setSize(sf::Vector2(borders.rightB.width, borders.rightB.height));
    borders.rightB.recShape.setPosition(borders.rightB.left, borders.rightB.top);
    borders.rightB.recShape.setFillColor(borders.rightB.blockColor);

    borders.bottomB.blockColor = WALL_COLOR;
    borders.bottomB.left = 0.0;
    borders.bottomB.top = WINDOW_HEIGHT - WALL_THICKNESS;
    borders.bottomB.width = WINDOW_WIDTH;
    borders.bottomB.height = WALL_THICKNESS;
    borders.bottomB.recShape.setSize(sf::Vector2(borders.bottomB.width, borders.bottomB.height));
    borders.bottomB.recShape.setPosition(borders.bottomB.left, borders.bottomB.top);
    borders.bottomB.recShape.setFillColor(borders.bottomB.blockColor);

    paddle.generalProps.blockColor = PADDLE_COLOR;
    paddle.generalProps.left = ( WINDOW_WIDTH - PADDLE_WIDTH) / 2.0;
    paddle.generalProps.top = WINDOW_HEIGHT - ( 2.0 * PADDLE_THICKNESS);
    paddle.generalProps.width = PADDLE_WIDTH;
    paddle.generalProps.height = PADDLE_THICKNESS;
    paddle.generalProps.blockColor = PADDLE_COLOR;
    paddle.generalProps.recShape.setSize(sf::Vector2f( paddle.generalProps.width, paddle.generalProps.height));
    paddle.generalProps.recShape.setFillColor(paddle.generalProps.blockColor);
    paddle.blockVeloX = 0.0;
    paddle.blockVeloY = 0.0;


    ball.radius = BALL_RADIUS;
    ball.coordinateX= paddle.generalProps.left + (paddle.generalProps.width/2.0);
    ball.coordinateY = paddle.generalProps.top - ball.radius - 1;
    ball.velocityX = 0.0;
    ball.velocityY = 0.0;
    ball.color = BALL_COLOR;

    float bricksTop = FIRST_BRICK;

    Brick* pNext = &bricks[0][0];
    for (int row = 0; row < BRICK_ROWS; row++){

        float bricksLeft = BRICKS_LEFT;

        for (int column = 0; column < BRICK_COLUMNS; column++){

            pNext->block.left = bricksLeft + 1;
            pNext->block.top = bricksTop + 1;

            pNext->block.width = BRICK_WIDTH - 2;
            pNext->block.height = BRICK_HEIGHT - 2;

            // set row specific properties
            if (row < 2) {
                pNext->block.blockColor = sf::Color::Yellow;
                pNext->points = 1;
                pNext->speedAdjust = 0;
            }
            else if (row < 4) {
                pNext->block.blockColor = sf::Color::Green;
                pNext->points = 3;
                pNext->speedAdjust = 0;
            }
            else if (row < 6) {
                pNext->block.blockColor = sf::Color(255,165,0); //Orange
                pNext->points = 5;
                pNext->speedAdjust = 1;
            }
            else {
                pNext->block.blockColor = sf::Color::Red;
                pNext->points = 7;
                pNext->speedAdjust = 2;
            }

            pNext->block.recShape.setSize(sf::Vector2f(pNext->block.width,pNext->block.height));
            pNext->block.recShape.setFillColor(pNext->block.blockColor);
            pNext->block.recShape.setPosition(pNext->block.left, pNext->block.top);

            // initialize hit flag off so brick is displayed
            pNext->hit = false;

            pNext++;

            bricksLeft += BRICK_WIDTH;

        } //brick columns

        bricksTop -= BRICK_HEIGHT;

    } // brick rows

} //end of setup


/**
 *
 * @param pBall - the point of the ball
 * @param pBlock - the point of the block
 * @return collusion - the collision has occur or has not occur
 */

int collisionPoint(Ball *pBall, Block *pBlock) {
    int collision = 0;

    float checkPtX, checkPtY;

    if (pBall->coordinateX < pBlock->left) {
        checkPtX = pBlock->left;
    }
    else if (pBall->coordinateX > (pBlock->left + pBlock->width)) {
        checkPtX = (pBlock->left + pBlock->width);
    }
    else {
        checkPtX = pBall->coordinateX;
    }

    if (pBall->coordinateY < pBlock->top)
        checkPtY = pBlock->top;
    else if (pBall->coordinateY > (pBlock->top + pBlock->height))
        checkPtY = pBlock->top + pBlock->height;
    else
        checkPtY = pBall->coordinateY;

    float diffX, diffY;

    diffX = checkPtX - pBall->coordinateX;
    diffY = (WINDOW_HEIGHT - checkPtY) - (WINDOW_HEIGHT - pBall->coordinateY);

    double distance = std::sqrt(pow(diffX,2.0) + pow(diffY,2.0));

    if (distance <= pBall->radius) {
        double theta = std::atan2(diffY, diffX);
        double degrees = 90.0 - theta * 180 / M_PI;

        if (degrees <= 0)
            degrees += 360;

        collision = int(degrees);
    }
    return collision;
}


/**
 *
 * @param pBall - point of the ball
 * @param pBlock - point of the block
 * @return collusion - returns a collusion if it occurs
 */

bool collisionCheck(Ball *pBall, Block *pBlock){

    bool collision = false;
    int calling = collisionPoint(pBall, pBlock);

    if (calling) {
        collision = true;

        if (calling > 225 && calling < 315) {
            pBall->velocityX *= -1;
            pBall->coordinateX = pBlock->left + pBlock->width + pBall->radius + 1;
        } else if (calling > 45 && calling < 135) {
            pBall->velocityX *= -1;
            pBall->coordinateX = pBlock->left - pBall->radius - 1;
        }

        if (calling >= 315 || calling <= 45) {
            pBall->velocityY *= -1;
            pBall->coordinateY = pBlock->top + pBlock->height + pBall->radius + 1;
        } else if (calling >= 135 && calling <= 225) {
            pBall->velocityY *= -1;
            pBall->coordinateY = pBlock->top - pBall->radius - 1;
        }
    }
    return collision;

}


/**
 *
 * @param pPaddle - point of the moving paddle
 * @param pBlock - point of the left and right borders
 * @return - returns a collision if it occurs
 */

bool wallCollisionCheck(Block *pPaddle, Block * pBlock){
    bool collision = false;
    if (pPaddle->left < pBlock->left + pBlock->width && pPaddle->left + pPaddle->width > pBlock->left && pPaddle->top < pBlock->top + pBlock->height && pPaddle->top + pPaddle->height > pBlock ->top){
        collision = true;
    }
    return collision;
}


/**
 *
 * @param ball - structure with properties of the ball
 * @param paddle - structure with properties of the paddle
 * @param bricks - structure with properties of the bricks
 * @param borders - structure with properties of the borders
 * @return gameOver - ends the game when ball hits bottom border
 */

bool doCollisionChecks(Ball &ball, MovingBlock &paddle, Brick bricks [BRICK_ROWS][BRICK_COLUMNS], Borders borders){
    bool gameOver = false;

    if (!collisionCheck(&ball, &paddle.generalProps)) {
        if (!collisionCheck(&ball, &borders.topB)) {
            gameOver = (collisionCheck(&ball, &borders.bottomB));
        }
    }
    if (!collisionCheck(&ball, &borders.leftB)) {
        (collisionCheck(&ball, &borders.rightB));

    }
    if ( wallCollisionCheck(&paddle.generalProps, &borders.leftB)){
        paddle.blockVeloX = 0.0;
        paddle.generalProps.left = borders.leftB.left + borders.leftB.width + 1.0;
    }
    else if ( wallCollisionCheck(&paddle.generalProps, &borders.rightB)){
        paddle.blockVeloX = 0.0;
        paddle.generalProps.left = borders.rightB.left - paddle.generalProps.width - 1.0;
    }

    Brick *pBrick = &bricks[0][0];
    for (int row = 0; row < BRICK_ROWS; row++){
        for (int column = 0; column < BRICK_COLUMNS; column++){
            if (!pBrick->hit) {
                pBrick->hit = collisionCheck(&ball, &pBrick->block);
            }
            pBrick++;
        } //columns
    } //rows
    return gameOver;
}


/**
 * convert user keyboard input into recognized integer values
 * for left=1/up=2/right=3/down=4
 * @return Direction - user input (default no-input=None, quit=Exit)
 */

Direction processInput() {
    Direction input = None; // no input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            input = Left;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            input = Up;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            input = Right;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            input = Down;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
            input = Exit;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            input = Start;
        }
        return input;
} // end getUserInput


/**
 * update the state of game objects
 * @param input - user keyboard input
 * @param ball  - update ball position and speed
 * @param delta - current frame time
 * @param started - starts the game
 * @param gameOver - ends the game
 */

    bool update(Direction &input, Ball &ball, MovingBlock &paddle, Brick bricks [BRICK_ROWS][BRICK_COLUMNS], Borders borders,  float delta, bool started) {
    bool gameOver;

    if (input) {
        switch (input) {
            case 1: // Left
                paddle.blockVeloX -= PADDLE_SPEED;
                break;
            case 3: // Right
                paddle.blockVeloX += PADDLE_SPEED;
                break;
            case 5: // Space
                if (!started) {
                    if ((int(delta * 10) & 1) % 2) {
                        ball.velocityX *= -1;
                        started = true;
                    }
                    ball.velocityX = BALL_SPEED_X;
                    ball.velocityY = BALL_SPEED_Y * -1;
                }

                break;
        }
        // clear input
        input = None;
    }
    if (!started) {
        ball.coordinateX += ball.velocityX * delta;
        ball.coordinateY += ball.velocityY * delta;
        }
    else {
        ball.coordinateX = paddle.generalProps.left + (paddle.generalProps.width / 2.0);
        ball.coordinateY = paddle.generalProps.top - ball.radius - 1;
        }

    paddle.generalProps.left += paddle.blockVeloX * delta;
    paddle.generalProps.top += paddle.blockVeloY * delta;

    gameOver = doCollisionChecks(ball, paddle, bricks, borders);

    return gameOver;
    }

    // end update


/**
 * draw the ball on the graphics window
 * @param bricks - structure variable with properties for the bricks
 * @param window - handle to open graphics window
 * @param ball - structure variable with properties for the ball
 * @param delta  - amount of frame time plus lag (in ms)
 * @param borders - structure variable with properties for the borders
 * @param paddle - structure variable with properties for the paddle
 */

    void render(sf::RenderWindow &window, Ball &ball, MovingBlock &paddle, Brick bricks[BRICK_ROWS][BRICK_COLUMNS], Borders &borders, float delta) {
        // Render drawing objects
        // ------------------------------------------------
        // clear the window with the background color

        window.clear(WINDOW_COLOR);


        //draw the borders
        //-------------------------------------------------

        window.draw(borders.topB.recShape);
        window.draw(borders.rightB.recShape);
        window.draw(borders.bottomB.recShape);
        window.draw(borders.leftB.recShape);

        // draw the theBall
        // ------------------------------------------------
        sf::CircleShape circle;
        circle.setFillColor(ball.color);
        circle.setRadius(ball.radius);
        // set screen coordinates relative to the center of the circle
        circle.setOrigin(ball.radius, ball.radius);
        // calculate current drawing location relative to speed and frame-time
        float xPosition = ball.coordinateX + ball.velocityX * delta;
        float yPosition = ball.coordinateY + ball.velocityY * delta;
        circle.setPosition(xPosition, yPosition);
        window.draw(circle);

        // draw the paddle
        // ------------------------------------------------

        float paddleXcord = paddle.generalProps.left + paddle.blockVeloX * delta;
        float paddleYcord = paddle.generalProps.top + paddle.blockVeloY * delta;
        paddle.generalProps.recShape.setPosition(paddleXcord, paddleYcord);
        window.draw(paddle.generalProps.recShape);

        //draw the bricks
        //-------------------------------------------------

        Brick *pBrick = &bricks[0][0];
        for (int row = 0; row < BRICK_ROWS; row++){
            for (int column = 0; column < BRICK_COLUMNS; column++){
                if (!pBrick->hit) {
                    window.draw(pBrick->block.recShape);
                }
                pBrick++;
            } //columns
        } //rows

        // show the new window
        // ------------------------------------------------
        window.display();
    }


// end render


