#include "Header.h"

//Button Member Functions

Button::Button(Font& font, string label, Vector2f size, Vector2f position) : text(font, label, 40)
{
    if (label == "Exit")
        isExit = true;

    shape.setSize(size);
    shape.setPosition(position);

    if (isExit)
        shape.setFillColor(exitIdleColor);
    else
        shape.setFillColor(idleColor);
    
    shape.setOutlineThickness(2);
    shape.setOutlineColor(Color(80, 140, 255));

    text.setFillColor(Color::White);

    FloatRect textRect = text.getLocalBounds();

    if (size.y == 260)
    {
        offset = 80;
    }

    text.setOrigin({textRect.size.x / 2.f, textRect.size.y / 2.f - offset});
    text.setPosition({position.x + size.x / 2.f, position.y + size.y / 2.f - 10.f});
}

void Button::update(RenderWindow& window)
{
    Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

    if (shape.getGlobalBounds().contains(mouse))
    {
        if (isExit)
            shape.setFillColor(exitHoverColor);
        else
            shape.setFillColor(hoverColor);
    }
    else
    {
        if (isExit)
            shape.setFillColor(exitIdleColor);
        else
            shape.setFillColor(idleColor);
    }
}

FloatRect Button::globalBounds()
{
    return shape.getGlobalBounds();
}

bool Button::isClicked(RenderWindow& window, const Event& event)
{
    if (event.is<Event::MouseButtonPressed>())
    {
        Vector2f mouse = window.mapPixelToCoords(Mouse::getPosition(window));

        if (shape.getGlobalBounds().contains(mouse))
            return true;
    }

    return false;
}

void Button::draw(RenderWindow& window)
{
    window.draw(shape);
    window.draw(text);
}