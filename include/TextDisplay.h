//
// Created by necator on 7/21/17.
//

#ifndef CINDERSNAKE_TEXTHELPER_H
#define CINDERSNAKE_TEXTHELPER_H

#include <string>
#include <map>

#include "cinder/Text.h"
#include "cinder/gl/gl.h"

class TextDisplay {
public:
    static TextDisplay& getInstance() {
        static TextDisplay instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    TextDisplay(TextDisplay const&) = delete;
    void operator=(TextDisplay const&) = delete;

    //void setup();
    void draw();

    int addText(const std::string& text,
                const ci::vec2& pos,
                const ci::Color& color = ci::Color(0, 0, 0),
                const ci::Font& font = ci::Font(std::string("Arial"), 24),
                bool visible = true);

    void setText(int id, const std::string& text);
    void setPos(int id, const ci::vec2& pos);
    void setColor(int id, const ci::Color& color);
    void setFont(int id, const ci::Font& font);
    void setVisible(int id, bool v);

    std::string getText(int id) const;
    ci::vec2 getPos(int id) const;
    ci::Color getColor(int id) const;
    ci::Font getFont(int id) const;
    bool getVisible(int id) const;

    void erase(int id);
    int duplicate(int id);

    const static ci::Font DefaultFont;
private:
    TextDisplay() {}

    class TextInfo {
    public:
        TextInfo(const std::string& t,
                 const ci::vec2& p,
                 const ci::Color& c,
                 const ci::Font& f,
                 bool v) : text_(t), pos_(p), color_(c), font_(f), visible_(v) {}

        ci::gl::Texture2dRef getTexture();

        void setText(const std::string& text);
        void setPos(const ci::vec2& pos);
        void setColor(const ci::Color& color);
        void setFont(const ci::Font& font);
        void setVisible(bool v);

        std::string getText() const;
        ci::vec2 getPos() const;
        ci::Color getColor() const;
        ci::Font getFont() const;
        bool getVisible() const;
    private:
        std::string text_;
        ci::vec2 pos_;
        ci::Color color_;
        ci::Font font_;
        bool visible_;

        ci::gl::Texture2dRef texture;
        bool cached = false;
    };

    int id_count = 0;
    std::map<int, TextInfo> texts;

    bool containsKey(int i) const;
};
#endif //CINDERSNAKE_TEXTHELPER_H
