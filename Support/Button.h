#ifndef DAT220_PROJECT_BUTTON_H
#define DAT220_PROJECT_BUTTON_H

#include <memory>
#include <iostream>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class Button {
public:
    /**
     * Constructor for the button with initialization of a basic button
     */
    Button();

    /**
     * Draws the button to the window
     * @param window Window to draw the button in
     */
    void draw(sf::RenderWindow &window);
    /**
     * Get position of the button
     * @return Returns the position of the button
     */
    const sf::Vector2f &getPosition() const;
    /**
     * Set position of the button
     * @param position The bosition of the button
     */
    void setPosition(const sf::Vector2f &position);
    /**
     * Set button size
     * @param buttonSize The size of the button
     */
    void setButtonSize(sf::Vector2f buttonSize);
    /**
     * Get button size
     * @return Returns the size of the button
     */
    const sf::Vector2f &getButtonSize() const;
    /**
     * Set the color of the button
     * @param color the color of the button
     */
    void setButtonColor(const sf::Color &color);
    /**
     * Set Highlighting of the button
     * @param buttonHighlight Set true if you want the button to be highlighted
     */
    void setButtonHighlight(bool buttonHighlight);
    /**
     * Set the button highlight color
     * @param color The color of the highlight
     */
    void setButtonHighlightColor(const sf::Color &color);
    /**
     * Set the outline color of the button
     * @param outlineColor The outline color
     */
    void setButtonOutlineColor(const sf::Color &outlineColor);
    /**
     * Set the thickness of the button outline
     * @param size Size of the outline
     */
    void setButtonOutlineSize(const float &size);

    /**
     * Set button text font
     * @param fontLocation Path to the font
     */
    void setFont(const std::string &fontLocation);
    /**
     * Set button text
     * @param text The text to set in the button
     */
    void setText(const std::string &text);
    /**
     * Set the size of button text
     * @param textSize size of thext
     */
    void setTextSize(uint8_t textSize);
    /**
     * Set the color of the button text
     * @param textColor The color of the button text
     */
    void setTextColor(const sf::Color &textColor);
    /**
     * Set highlighting of text
     * @param textHighlight Set true if you want to highlight text
     */
    void setTextHighlight(bool textHighlight);
    /**
     * Set the highlight color of the text
     * @param textHighlightColor The color to be highlighted
     */
    void setTextHighlightColor(const sf::Color &textHighlightColor);
    /**
     * Set text outline color
     * @param textOutlineColor The color of the outline
     */
    void setTextOutlineColor(const sf::Color &textOutlineColor);
    /**
     * Set the thickness of text ouline
     * @param textOutlineSize Size of outline
     */
    void setTextOutlineSize(float textOutlineSize);
    /**
     * Set if you want only to show text and no background
     * @param onlyText Set true if you want to only show text
     */
    void setOnlyText(bool onlyText);

    /**
     * Get the local bounds of the text
     * @return Returns the local bounds of the text
     */
    sf::FloatRect getTextLocalBounds();
    /**
     * Get the global bounds of the text
     * @return Returns the global bounds of the text
     */
    sf::FloatRect getTextGlobalBounds();
    /**
     * Get button global bounds
     * @return Returns the global bounds of the button
     */
    sf::FloatRect getButtonGlobalBounds() const;
    /**
     * Get the local bounds of the button
     * @return Returns the local bounds of the button
     */
    sf::FloatRect getButtonLocalBounds();

private:
    sf::Vector2f position;

    sf::RectangleShape shape;
    sf::Vector2f buttonSize;
    float buttonOutlineSize;
    sf::Color buttonColor;
    sf::Color buttonHighlightColor;
    sf::Color buttonOutlineColor;
    bool buttonHighlight;

    sf::Font font;
    sf::Text text;
    std::string textString;
    uint8_t textSize;
    sf::Color textColor;
    sf::Color textHighlightColor;
    sf::Color textOutlineColor;
    float textOutlineSize;
    bool onlyText;
    bool textHighlight;

    sf::FloatRect textRect;
};

typedef std::shared_ptr<Button> ButtonPtr;

#endif //DAT220_PROJECT_BUTTON_H
