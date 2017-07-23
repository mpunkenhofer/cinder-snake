//
// Created by necator on 7/16/17.
//

#ifndef CINDERSNAKE_SNAKE_H
#define CINDERSNAKE_SNAKE_H

#include <list>
#include <iostream>
#include <tuple>
#include "cinder/Vector.h"
#include "SnakePoint.h"
#include "TextDisplay.h"

enum class Direction {
    up, down, left, right, unchanged
};

std::ostream& operator<<(std::ostream& os, const Direction& dir);

class Snake {
public:
    Snake();

    void setup();
    void update();
    void draw();
    void move(const Direction& direction);

    void setPaused(bool p);
    bool isPaused() const;

    bool isGameOver() const;

    bool isDebugMode() const;
    void setDebugMode(bool d);

    int getSpeed() const;
    void setSpeed(int speed);
private:
    const int segment_size = 15;
    const int start_segments = 15;

    const double spawn_rate = 2.5;
    const double point_lifetime = 7.0;

    const int base_speed = 1000;
    const int speed_increase = 10;

    int speed = 92;

    bool is_paused = false;
    bool is_game_over = false;
    bool is_debug_mode = false;

    struct DebugModeTextIDs {
        int DebugMode;
        int Fps;
        int Misc;
    };

    DebugModeTextIDs debug_text_ids;

    TextDisplay& TextDisplayI = TextDisplay::getInstance();

    Direction dir = Direction::right;

    std::list<ci::Rectf> snake_segments;
    std::list<SnakePoint> snake_points;

    void drawSegments();
    void drawPoints();
    void drawHead();

    bool collision(const ci::Rectf& new_head);
    bool collectPoint(const ci::Rectf &segment, bool remove = false);
    void hidePoint(const ci::Rectf& r);

    void spawnPoint();
    ci::vec2 randomPoint();

    void removeTimedOutPoints();

    void gameOver();
};
#endif //CINDERSNAKE_SNAKE_H
