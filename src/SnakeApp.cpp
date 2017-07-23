#include <iostream>
#include <list>
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Snake.h"
#include "TextDisplay.h"

using namespace ci;
using namespace ci::app;

class SnakeApp : public App {
public:
    static void prepareSettings(Settings *settings);

    void keyDown( KeyEvent event ) override;

    void setup() override;
    void update() override;
    void draw() override;

private:
    Snake snake;
    TextDisplay& TextDisplayI = TextDisplay::getInstance();
};

void SnakeApp::prepareSettings(Settings *settings)
{
    settings->setWindowSize(640, 480);
    //settings->disableFrameRate();
    settings->setResizable(false);
    settings->setFullScreen(false);
}

void SnakeApp::keyDown(KeyEvent event)
{
    if(event.getChar() == 'f') {
        // Toggle full screen when the user presses the 'f' key.
        setFullScreen(!isFullScreen());
    }
    else if(event.getCode() == KeyEvent::KEY_LEFT) {
        snake.move(Direction::left);
    }
    else if(event.getCode() == KeyEvent::KEY_RIGHT) {
        snake.move(Direction::right);
    }
    else if(event.getCode() == KeyEvent::KEY_UP) {
        snake.move(Direction::up);
    }
    else if(event.getCode() == KeyEvent::KEY_DOWN) {
        snake.move(Direction::down);
    }
    else if(event.getChar() == ' ') {
        snake.setPaused(!snake.isPaused());
    }
    else if(event.getCode() == KeyEvent::KEY_BACKSPACE) {
        if(snake.isGameOver())
            snake.setup();
    }
    else if(event.getCode() == KeyEvent::KEY_F5) {
        snake.setDebugMode(!snake.isDebugMode());
    }
    else if(event.getCode() == KeyEvent::KEY_KP_PLUS) {
        snake.setSpeed(snake.getSpeed() + 2);
    }
    else if(event.getCode() == KeyEvent::KEY_KP_MINUS) {
        snake.setSpeed(snake.getSpeed() - 2);
    }
    else if(event.getCode() == KeyEvent::KEY_ESCAPE) {
        // Exit full screen, or quit the application, when the user presses the ESC key.
        if(isFullScreen())
            setFullScreen(false);
        else
            quit();
    }
}

void SnakeApp::setup() {

}

void SnakeApp::update() {
    if(!snake.isGameOver())
        snake.update();
}

void SnakeApp::draw() {
    if(!snake.isGameOver())
        snake.draw();

    TextDisplayI.draw();
}

CINDER_APP(SnakeApp, RendererGl(RendererGl::Options().msaa(16)), &SnakeApp::prepareSettings)