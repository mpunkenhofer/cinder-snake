//
// Created by necator on 7/21/17.
//

#include "TextDisplay.h"
#include "cinder/app/App.h"
#include "cinder/Vector.h"

using namespace ci;
using namespace ci::app;

const Font TextDisplay::DefaultFont = ci::Font(std::string("Arial"), 24);

void TextDisplay::draw() {
    if(texts.empty())
        return;

    gl::color(Color::white());

    for(auto& t : texts) {
        if(t.second.getVisible())
            gl::draw(t.second.getTexture(), t.second.getPos());
    }
}

int TextDisplay::addText(const std::string& text,
            const vec2& pos,
            const Color& color,
            const Font& font,
            bool visible) {
    auto id = id_count;
    id_count++;

    auto ti = TextInfo(text, pos, color, font, visible);

    texts.insert(std::make_pair(id, ti));

    std::cout << "New Label: " << text << ", id count: " << id_count << std::endl;
    return id;
}

void TextDisplay::erase(int id) {
    auto it = texts.find(id);

    if(it != texts.end()) {
        texts.erase(it);
        std::cout << "Removed Label with id: " << id << std::endl;
    }
}

int TextDisplay::duplicate(int id) {
    auto duplicate_id = id_count;
    id_count++;

    auto text = texts.at(id);

    texts.insert(std::make_pair(duplicate_id, text));

    return duplicate_id;
}

bool TextDisplay::containsKey(int i) const {
    return (texts.find(i)) != texts.end();
}

void TextDisplay::setText(int id, const std::string& text) {
    texts.at(id).setText(text);
}

void TextDisplay::setPos(int id, const ci::vec2& pos) {
    texts.at(id).setPos(pos);
}

void TextDisplay::setColor(int id, const ci::Color& color) {
    texts.at(id).setColor(color);
}

void TextDisplay::setFont(int id, const ci::Font& font) {
    texts.at(id).setFont(font);
}

void TextDisplay::setVisible(int id, bool v) {
    texts.at(id).setVisible(v);
}

std::string TextDisplay::getText(int id) const {
    texts.at(id).getText();
}

ci::vec2 TextDisplay::getPos(int id) const {
    texts.at(id).getPos();
}

ci::Color TextDisplay::getColor(int id) const {
    texts.at(id).getColor();
}

ci::Font TextDisplay::getFont(int id) const {
    texts.at(id).getFont();
}

bool TextDisplay::getVisible(int id) const {
    texts.at(id).getVisible();
}

void TextDisplay::TextInfo::setText(const std::string &text) {
    cached = false;
    text_ = text;
}

void TextDisplay::TextInfo::setPos(const ci::vec2 &pos) {
    cached = false;
    pos_ = pos;
}

void TextDisplay::TextInfo::setColor(const ci::Color &color) {
    cached = false;
    color_ = color;
}

void TextDisplay::TextInfo::setFont(const ci::Font &font) {
    cached = false;
    font_ = font;
}

void TextDisplay::TextInfo::setVisible(bool v) {
    visible_ = v;
}

std::string TextDisplay::TextInfo::getText() const {
    return text_;
}

ci::vec2 TextDisplay::TextInfo::getPos() const {
    return pos_;
}

ci::Color TextDisplay::TextInfo::getColor() const {
    return color_;
}

ci::Font TextDisplay::TextInfo::getFont() const {
    return font_;
}

bool TextDisplay::TextInfo::getVisible() const {
    return visible_;
}

gl::Texture2dRef TextDisplay::TextInfo::getTexture() {
    if(cached)
        return texture;
    else {
        TextLayout tl;
        tl.setFont(font_);
        tl.setColor(color_);
        tl.addLine(text_);
        texture = gl::Texture2d::create(tl.render(true, true));

        cached = true;

        return texture;
    }
}