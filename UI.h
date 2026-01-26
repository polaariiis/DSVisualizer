#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

struct ButtonStyle
{
    sf::Color fillNormal = sf::Color(60, 60, 60);
    sf::Color fillHover = sf::Color(80, 80, 80);
    sf::Color fillPressed = sf::Color(40, 40, 40);

    sf::Color textColor = sf::Color::White;

    float outlineThickness = 0.f;
    sf::Color outlineColor = sf::Color::Transparent;
};

class Button
{
public:
    using Callback = std::function<void()>;

    Button() = default;

    Button(sf::Vector2f size,
        sf::Vector2f pos,
        sf::Font& font,
        const std::string& label,
        unsigned int charSize,
        const ButtonStyle& style = {})
    {
        create(size, pos, font, label, charSize, style);
    }

    void create(sf::Vector2f size,
        sf::Vector2f pos,
        sf::Font& font,
        const std::string& label,
        unsigned int charSize,
        const ButtonStyle& style = {})
    {
        m_style = style;

        m_shape.setSize(size);
        m_shape.setPosition(pos);
        m_shape.setFillColor(m_style.fillNormal);
        m_shape.setOutlineThickness(m_style.outlineThickness);
        m_shape.setOutlineColor(m_style.outlineColor);

        m_text.setFont(font);
        m_text.setString(label);
        m_text.setCharacterSize(charSize);
        m_text.setFillColor(m_style.textColor);

        centerText();
    }

    void setPosition(sf::Vector2f pos)
    {
        m_shape.setPosition(pos);
        centerText();
    }

    void setCallback(Callback cb) { m_onClick = std::move(cb); }

    bool isInside(sf::Vector2f mousePos) const
    {
        return m_shape.getGlobalBounds().contains(mousePos);
    }

    // Event-based click handling (press inside + release inside)
    void handleEvent(const sf::Event& e, const sf::RenderWindow& window)
    {
        const auto mouse = sf::Vector2f(sf::Mouse::getPosition(window));
        const bool inside = m_shape.getGlobalBounds().contains(mouse);

        if (e.type == sf::Event::MouseMoved)
        {
            m_hovered = inside;
        }
        else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        {
            if (inside) m_pressed = true;
        }
        else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left)
        {
            const bool wasPressed = m_pressed;
            m_pressed = false;

            if (wasPressed && inside && m_onClick)
                m_onClick();
        }

        updateVisual();
    }

    void draw(sf::RenderTarget& target) const
    {
        target.draw(m_shape);
        target.draw(m_text);
    }

private:
    void centerText()
    {
        auto bounds = m_text.getLocalBounds();
        m_text.setOrigin(bounds.left + bounds.width * 0.5f,
            bounds.top + bounds.height * 0.5f);

        auto pos = m_shape.getPosition();
        auto size = m_shape.getSize();
        m_text.setPosition(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f);
    }

    void updateVisual()
    {
        if (m_pressed)      m_shape.setFillColor(m_style.fillPressed);
        else if (m_hovered) m_shape.setFillColor(m_style.fillHover);
        else                m_shape.setFillColor(m_style.fillNormal);
    }

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    ButtonStyle m_style;

    bool m_hovered = false;
    bool m_pressed = false;
    Callback m_onClick;
};
