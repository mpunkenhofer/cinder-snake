//
// Created by necator on 7/20/17.
//

#ifndef CINDERSNAKE_SNAKEPOINT_H
#define CINDERSNAKE_SNAKEPOINT_H

#include <iostream>
//#include <stringstream>

#include "cinder/Vector.h"
#include "cinder/Rect.h"
#include "cinder/app/App.h"
#include "cinder/Color.h"
#include "cinder/gl/gl.h"
#include "TextDisplay.h"

class SnakePoint {
public:
    SnakePoint(const ci::Rectf& rect,
               double spawn_time,
               double timeout = 5.0,
               ci::ColorA color = ci::ColorA(0.9, 0.8, 0.2, 1.0),
               bool debug_mode = false) :
            r(rect),
            spawn_time_(spawn_time),
            timeout_(timeout),
            c(color),
            debug_mode_(debug_mode) {};

    SnakePoint(const ci::Rectf& rect,
               double spawn_time,
               double timeout = 5.0,
               bool debug_mode = false) :
            r(rect),
            spawn_time_(spawn_time),
            timeout_(timeout),
            c(ci::ColorA(0.9, 0.8, 0.2, 1.0)),
            debug_mode_(debug_mode) {};

    SnakePoint(const ci::Rectf& rect,
               double timeout = 5.0,
               ci::ColorA color = ci::ColorA(0.9, 0.8, 0.2, 1.0),
               bool debug_mode = false) :
            r(rect),
            spawn_time_(ci::app::getElapsedSeconds()),
            timeout_(timeout),
            c(color),
            debug_mode_(debug_mode) {};

    SnakePoint(const SnakePoint& other) : r(other.r),
                                       c(other.c),
                                       is_collected(other.is_collected),
                                       drawable_(other.drawable_),
                                       debug_mode_(other.debug_mode_),
                                       spawn_time_(other.spawn_time_),
                                       timeout_(other.timeout_)
    {
        debug_label_id = TextDisplay::getInstance().duplicate(other.debug_label_id);
    }

    SnakePoint (SnakePoint&& other) noexcept : r(other.r),
                                               c(other.c),
                                               is_collected(other.is_collected),
                                               drawable_(other.drawable_),
                                               debug_mode_(other.debug_mode_),
                                               spawn_time_(other.spawn_time_),
                                               timeout_(other.timeout_),
                                               debug_label_id(other.debug_label_id) { }


    SnakePoint& operator=(const SnakePoint& other) {
        if(this != &other) {
            SnakePoint tmp(other);
            *this = std::move(tmp);
        }

        return *this;
    }

    SnakePoint& operator=(SnakePoint& other) noexcept {
        if(this != &other) {
            auto id = debug_label_id;
            debug_label_id = -1;
            TextDisplay::getInstance().erase(id);

            debug_label_id = other.debug_label_id;
            other.debug_label_id = -1;

            c = other.c;
            r = other.r;
            is_collected = other.is_collected;
            drawable_ = other.drawable_;
            debug_mode_ = other.debug_mode_;
            spawn_time_ = other.spawn_time_;
            timeout_ = other.timeout_;
        }

        return *this;
    }

    ~SnakePoint() noexcept {
        if(debug_label_id >= 0) {
            auto id = debug_label_id;
            debug_label_id = -1;
            TextDisplay::getInstance().erase(id);

        }
    }

    bool timedOut(double current_time = ci::app::getElapsedSeconds()) {
        if(is_collected || !drawable_)
            return false;
        else
            return current_time > (spawn_time_ + timeout_);
    }

    bool isCollected(bool collected = false) {
        if(collected)
            is_collected = true;

        return is_collected;
    }

    ci::Rectf getRect() const {
        return r;
    }

    bool isDrawable() const {
        return drawable_;
    }

    void setDrawable(bool drawable) {
        drawable_ = drawable;
    }

    bool isDebugMode() const {
        return debug_mode_;
    }

    void setDebugMode(bool d) {
        debug_mode_ = d;

        if(debug_mode_ && debug_label_id >= 0)
            TextDisplay::getInstance().setVisible(debug_label_id, true);
        else if(debug_label_id >= 0)
            TextDisplay::getInstance().setVisible(debug_label_id, false);
    }

    ci::ColorA getColor() {
        auto time_passed_s = ci::app::getElapsedSeconds();
        c.a = 1.0 - (time_passed_s - spawn_time_) / ((spawn_time_ + timeout_) - spawn_time_);
        return c;
    }

    void draw() {
        if(drawable_) {
            if(debug_mode_) {
                if(debug_label_id < 0) {
                    debug_label_id = TextDisplay::getInstance().addText("Point label",
                                                                        r.getCenter() - ci::vec2(8,22),
                                                                        ci::Color::white(),
                                                                        ci::Font(std::string("Arial"), 12), true);
                }
                if(debug_label_id >= 0) {
                    double s_remaining = (spawn_time_ + timeout_) - ci::app::getElapsedSeconds();

                    if(s_remaining >= 0.0) {
                        std::stringstream ss;
                        ss << std::setprecision(2) << s_remaining;

                        TextDisplay::getInstance().setText(debug_label_id, ss.str());
                    } else
                        TextDisplay::getInstance().setVisible(debug_label_id, false);
                }
            }

            ci::gl::color(getColor());
            ci::gl::drawSolidRect(r);
        } else if(debug_mode_) {
            auto red = ci::Color(1, 0, 0);

            ci::gl::color(red);
            ci::gl::drawStrokedRect(r);

            if(debug_label_id < 0) {
                debug_label_id = TextDisplay::getInstance().addText("Point label",
                                                                    r.getCenter() - ci::vec2(8,22),
                                                                    ci::Color::white(),
                                                                    ci::Font(std::string("Arial"), 12), true);
            }

            TextDisplay::getInstance().setText(debug_label_id, "Got it!");
            TextDisplay::getInstance().setPos(debug_label_id, r.getCenter() - ci::vec2(0,22));
            TextDisplay::getInstance().setColor(debug_label_id, red);
        }
    }
private:
    ci::Rectf r;
    ci::ColorA c;

    bool is_collected = false;
    bool drawable_ = true;

    bool debug_mode_ = false;
    int debug_label_id = -1;

    double spawn_time_;
    double timeout_;
};

#endif //CINDERSNAKE_SNAKEPOINT_H
