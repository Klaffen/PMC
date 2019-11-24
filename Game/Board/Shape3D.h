//
// Created by Roman Aleksandrov on 18/10/2017.
//

#ifndef DAT220_PROJECT_SHAPE3D_H
#define DAT220_PROJECT_SHAPE3D_H

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Shape3D {

public:

    /**
     * Default obstacle constructor
     * @param size Size of the obstacle to be created
     * @param position Position of the obstacle to be created
     * @param height Apparent height of the obstacle to be created. 1 for half and 2 for full cover.
     */
    Shape3D(sf::Vector2f size, sf::Vector2f position, float height);

    /**
     *
     * @return Returns the size of the obstacle
     */
    sf::Vector2f getSize();

    /**
     *
     * @return Returns the base rectangle of the obstacle
     */
    sf::RectangleShape &getBaseRect();

    /**
     * Draws obstacle
     * @param window Reference to the game window
     */
    void draw(sf::RenderWindow &window);

protected:

    sf::RectangleShape base;
    sf::RectangleShape top;
    sf::RectangleShape transparentRect;
    sf::VertexArray rear;
    sf::VertexArray right;
    sf::VertexArray front;
    sf::VertexArray left;

    float height;
    sf::Vector2f size;
    sf::Vector2f position;
    float outlineThickness;
    sf::Color topColor;
    sf::Color wallTopColor;
    sf::Color wallBaseColor;
    sf::Color outlineColor;
};

#endif //DAT220_PROJECT_SHAPE3D_H
