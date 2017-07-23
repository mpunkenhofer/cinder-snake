//
// Created by necator on 7/16/17.
//

#include <random>
#include <algorithm>
#include <cmath>
#include <iomanip>

#include "Snake.h"
#include "cinder/gl/gl.h"
#include "cinder/app/App.h"


using namespace ci;
using namespace ci::app;

void updateSegment(Rectf& r, const vec2& v);
bool rectContainsSegment(const Rectf& r, const Rectf& s);

std::ostream& operator<<(std::ostream& os, const Direction& dir) {
    if(os) {
        switch(dir) {
            case Direction::right: os << "right"; break;
            case Direction::left: os << "left"; break;
            case Direction::up: os << "up"; break;
            case Direction::down: os << "down"; break;
            case Direction::unchanged: os << "unchanged"; break;
        }
    }

    return os;
}

Snake::Snake() {
    debug_text_ids.DebugMode = TextDisplayI.addText("DEBUG MODE",
                                                    vec2(10),
                                                    Color(0.1, 0.7, 0.2),
                                                    Font(std::string("Arial"), 16),
                                                    false);
    debug_text_ids.Fps = TextDisplayI.addText("FPS",
                                              vec2(getWindowWidth() - 30, 10),
                                              Color(0.1, 0.7, 0.2),
                                              Font(std::string("Arial"), 14),
                                              false);

    debug_text_ids.Misc = TextDisplayI.addText("MISC",
                                              vec2(10, getWindowHeight() - 20),
                                              Color(0.1, 0.7, 0.2),
                                              Font(std::string("Arial"), 12),
                                              false);

    setup();
}

void Snake::move(const Direction& direction) {
    //std::cout << direction << '\n';

    if(dir == Direction::left && direction == Direction::right)
        return;
    if(dir == Direction::right && direction == Direction::left)
        return;
    if(dir == Direction::up && direction == Direction::down)
        return;
    if(dir == Direction::down && direction == Direction::up)
        return;

    if(!is_game_over && !is_paused)
        dir = direction;
};

void Snake::setPaused(bool p) {
    is_paused = p;
}

bool Snake::isPaused() const {
    return is_paused;
}

bool Snake::isGameOver() const {
    return is_game_over;
}

bool Snake::isDebugMode() const {
    return is_debug_mode;
}

void Snake::setDebugMode(bool d) {
    if(d) {
        for(auto& p : snake_points)
            p.setDebugMode(true);

    } else {
        for(auto& p : snake_points)
            p.setDebugMode(false);
    }

    is_debug_mode = d;
}

int Snake::getSpeed() const {
    return speed;
}

void Snake::setSpeed(int s) {
    if(s >= 0 && s <= 100)
        speed = s;
}

void Snake::gameOver() {
    std::cout << "Game over!\n";
    is_game_over = true;

    drawHead();

    gl::color(0.1, 0.1, 0.1, 0.7);
    gl::drawSolidRect(Rectf(vec2(0), getWindowSize()));
}

void Snake::setup() {
    std::cout << "New game!\n";

    snake_points.clear();
    snake_segments.clear();

    dir = Direction::right;

    //auto segment_size = 15;
    auto nr_segments = start_segments;

    auto screen_center = getWindowCenter();
    auto snake_start_x = screen_center.x;

    for(auto s = 0; s < nr_segments; ++s) {
        auto snake_x = snake_start_x - (s * segment_size);

        auto top_left = vec2(snake_x - segment_size / 2, screen_center.y + segment_size / 2);
        auto bottom_right = vec2(top_left.x + segment_size, top_left.y - segment_size);

        snake_segments.push_back(Rectf(top_left, bottom_right));
    }

    is_game_over = false;
}

void Snake::update() {
    static double last_update = 0.0;
    static double last_spawn = -(spawn_rate - 1); //spawn first point one sec after game start

    if(!is_paused && !is_game_over) {
        auto s_passed = getElapsedSeconds();
        auto ms_since_last_update = (s_passed - last_update) * 1000;
        auto update_condition = base_speed - (speed * speed_increase);

        if (ms_since_last_update > update_condition) {
            removeTimedOutPoints();

            auto new_head = *snake_segments.begin();

            if (dir == Direction::right)
                updateSegment(new_head, vec2(segment_size, 0));
            else if (dir == Direction::left)
                updateSegment(new_head, vec2(-segment_size, 0));
            else if (dir == Direction::up)
                updateSegment(new_head, vec2(0, -segment_size));
            else if (dir == Direction::down)
                updateSegment(new_head, vec2(0, segment_size));
            else {
                std::cout << "Invalid direction!\n";
                quick_exit(1);
            }

            if (collision(new_head)) {
                gameOver();
            } else {
                hidePoint(new_head);

                snake_segments.push_front(new_head);

                auto tail = snake_segments.back();

                if (!collectPoint(tail, true)) {
                    snake_segments.pop_back();
                }

                last_update = s_passed;
            }
        }

        if (last_spawn + spawn_rate < s_passed) {
            spawnPoint();
            last_spawn = s_passed;
        }
    }
}

void Snake::draw() {
    gl::clear(Color(0.1, 0.1, 0.1));

    drawSegments();
    drawPoints();

    if (is_paused) {
        gl::color(0.1, 0.1, 0.1, 0.7);
        gl::drawSolidRect(Rectf(vec2(0), getWindowSize()));
    }

    if (is_debug_mode) {
        TextDisplayI.setVisible(debug_text_ids.DebugMode, true);
        TextDisplayI.setVisible(debug_text_ids.Fps, true);
        TextDisplayI.setVisible(debug_text_ids.Misc, true);

        auto fps = getFrameRate();

        std::stringstream ss1;

        ss1 << fps;
        TextDisplayI.setText(debug_text_ids.Fps, ss1.str());

        std::stringstream ss2;

        ss2 << "Base Speed: " << std::setw(5) << base_speed << std::string(5, ' ')
           << "Speed increase: " << std::setw(5) << speed_increase << std::string(5, ' ')
           << "Current speed: " << std::setw(3) << speed << "%" << std::string(5, ' ')
           << "Start Segments: " << std::setw(3) << start_segments << std::string(5, ' ')
           << "Current segments: " << std::setw(3) << snake_segments.size();

        TextDisplayI.setText(debug_text_ids.Misc, ss2.str());
    } else {
        TextDisplayI.setVisible(debug_text_ids.DebugMode, false);
        TextDisplayI.setVisible(debug_text_ids.Fps, false);
        TextDisplayI.setVisible(debug_text_ids.Misc, false);
    }
}

void Snake::drawSegments() {
    gl::color(Color(0, 0.9, 0.4));

    for(auto& s : snake_segments)
        gl::drawSolidRect(s);
}

void Snake::drawPoints() {
    for(auto& p : snake_points)
        p.draw();
}

void Snake::drawHead() {
    gl::color(Color(1, 0, 0));
    auto head = snake_segments.front();
    auto center = head.getCenter();
    auto half_segment = segment_size / 2.0;

    if (dir == Direction::right)
        gl::drawLine(center + vec2(half_segment, -half_segment), center + vec2(half_segment, half_segment));
    else if (dir == Direction::left)
        gl::drawLine(center + vec2(-half_segment, -half_segment), center + vec2(-half_segment, half_segment));
    else if (dir == Direction::up)
        gl::drawLine(center + vec2(-half_segment, -half_segment), center + vec2(half_segment, -half_segment));
    else if (dir == Direction::down)
        gl::drawLine(center + vec2(-half_segment, half_segment), center + vec2(half_segment, half_segment));
    else {
        std::cout << "Invalid direction!\n";
        quick_exit(1);
    }
}

bool Snake::collision(const Rectf& new_head) {
    Rectf window_box(vec2(0), vec2(getWindowWidth(), getWindowHeight()));

    if(!rectContainsSegment(window_box, new_head))
        return true;

    for(const auto& s : snake_segments) {
        if(new_head.getUpperLeft() == s.getUpperLeft() && new_head.getLowerRight() == s.getLowerRight())
            return true;
    }

    return false;
}

void Snake::spawnPoint() {
    auto point = randomPoint();

    auto diff_to_center = point - getWindowCenter();
    auto segments_to_center = vec2(std::round(diff_to_center.x / static_cast<double>(segment_size)),
                                   std::round(diff_to_center.y / static_cast<double>(segment_size)));
    auto point_center = getWindowCenter() + segments_to_center * vec2(segment_size);

    auto top_left = vec2(point_center.x - segment_size / 2, point_center.y + segment_size / 2);
    auto bottom_right = vec2(top_left.x + segment_size, top_left.y - segment_size);
    auto rect = Rectf(top_left, bottom_right);

    auto sp_it = std::find_if(snake_points.begin(), snake_points.end(), [&rect](auto& r) {
        return (r.getRect().getUpperLeft() == rect.getUpperLeft() &&
                r.getRect().getLowerRight() == rect.getLowerRight());
    });
    auto ss_it = std::find_if(snake_segments.begin(), snake_segments.end(), [&rect](auto& r) {
        return (r.getUpperLeft() == rect.getUpperLeft() &&
                r.getLowerRight() == rect.getLowerRight());
    });

    //retry
    if(sp_it != snake_points.end() || ss_it != snake_segments.end()) {
        //std::cout << "need to try again..." << std::endl;
        spawnPoint();
    } else {
        snake_points.emplace_front(SnakePoint(rect, getElapsedSeconds(), point_lifetime, is_debug_mode));
    }
}

bool Snake::collectPoint(const ci::Rectf &segment, bool remove) {
    for(auto it = snake_points.begin(); it != snake_points.end(); ++it) {
        auto& p = *it;
        if(!p.timedOut() &&
                p.getRect().getUpperLeft() == segment.getUpperLeft() &&
                p.getRect().getLowerRight() == segment.getLowerRight()) {

            if(remove) {
                snake_points.erase(it);
            } else {
                it->isCollected(true);
            }

            return true;
        }
    }

    return false;
}

void Snake::hidePoint(const Rectf &rec) {
    auto it = std::find_if(snake_points.begin(), snake_points.end(), [&rec](auto& r) {
        return (r.getRect().getUpperLeft() == rec.getUpperLeft() &&
               r.getRect().getLowerRight() == rec.getLowerRight());
    });

    if(it != snake_points.end())
        it->setDrawable(false);
}

void Snake::removeTimedOutPoints() {
    //auto it = std::remove_if(snake_points.begin(), snake_points.end(), [](auto& p) { return p.timedOut();});
    for(auto it = snake_points.begin(); it != snake_points.end(); it++) {
        if(it->timedOut())
            it = snake_points.erase(it);

    }

}

vec2 Snake::randomPoint() {
    static std::random_device rd;     // only used once to initialise (seed) engine
    static std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)

    const static int border_separation = segment_size * 3;
    std::uniform_int_distribution<int> uni_w(border_separation, getWindowWidth() - border_separation);
    std::uniform_int_distribution<int> uni_h(border_separation, getWindowHeight() - border_separation);

    return vec2(uni_w(rng), uni_h(rng));
}

void updateSegment(Rectf& r, const vec2& v) {
    r.set(r.getX1() + v.x, r.getY1() + v.y, r.getX2() + v.x, r.getY2() + v.y);
}

bool rectContainsSegment(const Rectf& r, const Rectf& s) {
    return r.contains(s.getUpperLeft()) && r.contains(s.getLowerRight());
}
