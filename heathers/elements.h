#pragma once
#include <SFML/Graphics.hpp>

class SortingPillar{
public:
    SortingPillar(sf::Vector2f position, sf::Vector2f size, int value):
        m_position(position), m_size(size), m_value(value){
        m_shape.setFillColor(sf::Color::White);
        m_shape.setOutlineColor(sf::Color::Red);
        m_shape.setOutlineThickness(-1);
        m_shape.setOrigin(size);
        m_shape.setPosition(position);
        m_shape.setSize(size);
    }
    void update(/*const float deltaTime*/){
        m_shape.setPosition(m_position);
        m_shape.setSize(m_size);
    }
    void draw(sf::RenderTarget& renderTarget){
        renderTarget.draw(m_shape);
    }
    void setFocus(bool state){
        if(state){
            m_shape.setOutlineColor(sf::Color::Green);
            m_shape.setOutlineThickness(-2);
        }
        else{
            m_shape.setOutlineColor(sf::Color::Red);
            m_shape.setOutlineThickness(-1);
        }
    }
public:
    sf::Vector2f m_position;
    int m_value;
private:
    sf::RectangleShape m_shape;
    sf::Vector2f m_size;
};