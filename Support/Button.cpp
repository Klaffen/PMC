#include "Button.h"

#include <stdexcept>

Button::Button() {
    buttonSize           = {250, 45};
    buttonOutlineSize    = 2;
    buttonColor          = sf::Color::Red;
    buttonHighlightColor = sf::Color::Green;
    buttonOutlineColor   = sf::Color::Cyan;
    buttonHighlight      = false;

    shape = sf::RectangleShape(buttonSize);
    shape.setFillColor(buttonColor);
    shape.setOutlineColor(buttonOutlineColor);

    if (!font.openFromFile("Data/Fonts/Inconsolata-Regular.ttf")) {
        throw std::runtime_error("Could not load font: Data/Fonts/Inconsolata-Regular.ttf");
    }
    text.emplace(font);
    textSize           = 35;
    textOutlineSize    = 1;
    textColor          = sf::Color::Magenta;
    textHighlightColor = sf::Color::White;
    textOutlineColor   = sf::Color(33, 33, 33);
    onlyText           = false;
    textHighlight      = false;
}

void Button::draw(sf::RenderWindow& window) {
    if (buttonHighlight) {
        shape.setFillColor(buttonHighlightColor);
    } else {
        shape.setFillColor(buttonColor);
    }
    shape.setOutlineThickness(buttonOutlineSize);
    shape.setOutlineColor(buttonOutlineColor);
    shape.setPosition(position);

    text->setString(textString);
    text->setCharacterSize(textSize);
    text->setOutlineThickness(textOutlineSize);
    text->setOutlineColor(textOutlineColor);

    if (onlyText) {
        shape.setSize({(shape.getSize().x * (text->getLocalBounds().size.x / shape.getSize().x)),
            (shape.getSize().y * (text->getLocalBounds().size.y / shape.getSize().y))});
    }

    if (textHighlight) {
        text->setFillColor(textHighlightColor);
    } else {
        text->setFillColor(textColor);
    }

    textRect = text->getLocalBounds();
    text->setOrigin({textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f});
    text->setPosition(
        {(shape.getPosition().x + (shape.getSize().x / 2)), (shape.getPosition().y + (shape.getSize().y / 2))});

    if (!onlyText) {
        window.draw(shape);
    }
    window.draw(*text);
}

const sf::Vector2f& Button::getPosition() const {
    return position;
}
void Button::setPosition(const sf::Vector2f& position) {
    Button::position = position;
}


void Button::setButtonSize(sf::Vector2f buttonSize) {
    this->buttonSize = buttonSize;
}
const sf::Vector2f& Button::getButtonSize() const {
    return buttonSize;
}
void Button::setButtonColor(const sf::Color& color) {
    this->buttonColor = color;
}
void Button::setButtonHighlight(bool buttonHighlight) {
    this->buttonHighlight = buttonHighlight;
}
void Button::setButtonHighlightColor(const sf::Color& color) {
    this->buttonHighlightColor = color;
}
void Button::setButtonOutlineColor(const sf::Color& outlineColor) {
    this->buttonOutlineColor = outlineColor;
}
void Button::setButtonOutlineSize(const float& size) {
    this->buttonOutlineSize = size;
}

void Button::setFont(const std::string& fontLocation) {
    if (!font.openFromFile(fontLocation)) {
        throw std::runtime_error("Could not load font: " + fontLocation);
    }
    text.emplace(font);
}
void Button::setText(const std::string& text) {
    this->textString = text;
}
void Button::setTextSize(uint8_t textSize) {
    this->textSize = textSize;
}
void Button::setTextColor(const sf::Color& textColor) {
    this->textColor = textColor;
}
void Button::setTextHighlight(bool textHighlight) {
    this->textHighlight = textHighlight;
}
void Button::setTextHighlightColor(const sf::Color& textHighlightColor) {
    this->textHighlightColor = textHighlightColor;
}
void Button::setTextOutlineColor(const sf::Color& textOutlineColor) {
    this->textOutlineColor = textOutlineColor;
}
void Button::setTextOutlineSize(float textOutlineSize) {
    this->textOutlineSize = textOutlineSize;
}
void Button::setOnlyText(bool onlyText) {
    this->onlyText = onlyText;
}

sf::FloatRect Button::getButtonGlobalBounds() const {
    return shape.getGlobalBounds();
}

sf::FloatRect Button::getButtonLocalBounds() {
    return shape.getLocalBounds();
}

sf::FloatRect Button::getTextLocalBounds() {
    return text->getLocalBounds();
}

sf::FloatRect Button::getTextGlobalBounds() {
    return text->getGlobalBounds();
}
