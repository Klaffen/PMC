//
// Created by VegarK on 13.11.2017.
//

#ifndef DAT220_PROJECT_MATHHELPER_H
#define DAT220_PROJECT_MATHHELPER_H

#include <cmath>
#include <SFML/Window.hpp>

class mathHelper {
public:
    /**
     * Finds the angle in degrees between two points in regards to the x axis
     * @param p1
     * @param p2
     * @return
     */
    static float getAngle(sf::Vector2f p1, sf::Vector2f p2);

    /**
     * finds the distance between two different points
     * @param p1 The first point
     * @param p2 The second point
     * @return The distance
     */
    static float getDistance(sf::Vector2f p1, sf::Vector2f p2);
};


#endif //DAT220_PROJECT_MATHHELPER_H
