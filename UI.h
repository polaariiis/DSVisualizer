#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>
#include <vector>
#include <cstdio>

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


            if (wasPressed && inside && m_onClick) m_onClick();
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

// -----------------------------
// Simple vertical text list (used for "Sorting methods")
// -----------------------------
class TextList {
public:
    void setFont(sf::Font& font) { m_font = &font; }

    void setArea(const sf::FloatRect& r) {
        m_area = r;
        layout();
    }

    void setTitle(const std::string& t) {
        m_titleStr = t;
        layout();
    }

    void setItems(const std::vector<std::string>& items) {
        m_itemsStr = items;
        if (m_selected >= (int)m_itemsStr.size()) m_selected = (int)m_itemsStr.size() - 1;
        if (m_selected < 0) m_selected = 0;
        layout();
    }

    int selectedIndex() const { return m_selected; }
    const std::string& selectedItem() const { return m_itemsStr.at((size_t)m_selected); }

    void handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
        if (!m_font) return;
        if (e.type != sf::Event::MouseButtonReleased || e.mouseButton.button != sf::Mouse::Left)
            return;

        const auto mouse = sf::Vector2f(sf::Mouse::getPosition(window));

        for (size_t i = 0; i < m_itemTexts.size(); ++i) {
            if (m_itemTexts[i].getGlobalBounds().contains(mouse)) {
                m_selected = (int)i;
                updateColors();
                break;
            }
        }
    }

    void draw(sf::RenderTarget& target) const {
        if (!m_font) return;
        target.draw(m_title);
        for (auto& t : m_itemTexts) target.draw(t);
    }

    void setColor(sf::Color normal, sf::Color selected) {
        m_colorNormal = normal;
        m_colorSelected = selected;
        updateColors();
    }

private:
    void layout() {
        if (!m_font) return;

        m_title.setFont(*m_font);
        m_title.setString(m_titleStr);
        m_title.setCharacterSize(m_titleSize);
        m_title.setFillColor(m_colorNormal);
        m_title.setPosition(m_area.left + m_padX, m_area.top + m_padY);

        m_itemTexts.clear();
        m_itemTexts.reserve(m_itemsStr.size());

        float y = m_area.top + m_padY + (float)m_titleSize + 16.f;
        for (size_t i = 0; i < m_itemsStr.size(); ++i) {
            sf::Text t;
            t.setFont(*m_font);
            t.setString(m_itemsStr[i]);
            t.setCharacterSize(m_itemSize);
            t.setPosition(m_area.left + m_padX, y);
            y += (float)m_itemSize + 10.f;
            m_itemTexts.push_back(t);
        }
        updateColors();
    }

    void updateColors() {
        m_title.setFillColor(m_colorNormal);
        for (size_t i = 0; i < m_itemTexts.size(); ++i) {
            m_itemTexts[i].setFillColor(((int)i == m_selected) ? m_colorSelected : m_colorNormal);
        }
    }

private:
    sf::Font* m_font = nullptr;
    sf::FloatRect m_area{};
    std::string m_titleStr = "Sorting methods";
    std::vector<std::string> m_itemsStr;
    std::vector<sf::Text> m_itemTexts;
    sf::Text m_title;

    int m_selected = 0;
    unsigned int m_titleSize = 18;
    unsigned int m_itemSize = 18;
    float m_padX = 14.f;
    float m_padY = 14.f;
    sf::Color m_colorNormal = sf::Color(120, 255, 120);
    sf::Color m_colorSelected = sf::Color(0, 200, 0);
};

// -----------------------------
// Simple horizontal slider (used for speed)
// -----------------------------
class Slider {
public:
    void setFont(sf::Font& font) { m_font = &font; layoutLabel(); }

    void setPosition(sf::Vector2f p) {
        m_pos = p;
        layout();
    }

    void setWidth(float w) {
        m_width = w;
        layout();
    }

    void setRange(float minV, float maxV) {
        m_min = minV;
        m_max = maxV;
        setValue(m_value);
    }

    void setValue(float v) {
        if (m_max <= m_min) { m_value = m_min; }
        else {
            if (v < m_min) v = m_min;
            if (v > m_max) v = m_max;
            m_value = v;
        }
        updateKnobFromValue();
        updateLabelValue();
    }

    float getValue() const { return m_value; }

    void setLabel(const std::string& s) { m_labelStr = s; layoutLabel(); updateLabelValue(); }

    void handleEvent(const sf::Event& e, const sf::RenderWindow& window) {
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
            const auto mouse = sf::Vector2f(sf::Mouse::getPosition(window));
            if (m_knob.getGlobalBounds().contains(mouse) || m_track.getGlobalBounds().contains(mouse)) {
                m_dragging = true;
                setFromMouse(mouse.x);
            }
        }
        else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left) {
            m_dragging = false;
        }
        else if (e.type == sf::Event::MouseMoved) {
            if (m_dragging) {
                const auto mouse = sf::Vector2f(sf::Mouse::getPosition(window));
                setFromMouse(mouse.x);
            }
        }
    }

    void draw(sf::RenderTarget& target) const {
        target.draw(m_label);
        target.draw(m_valueText);
        target.draw(m_track);
        target.draw(m_knob);
    }

private:
    void layout() {
        // track
        m_track.setPosition({ m_pos.x, m_pos.y + 24.f });
        m_track.setSize({ m_width, 4.f });
        m_track.setFillColor(sf::Color(0, 120, 0));
        // knob
        m_knob.setSize({ 12.f, 20.f });
        m_knob.setOrigin({ 6.f, 10.f });
        updateKnobFromValue();
        layoutLabel();
        updateLabelValue();
    }

    void layoutLabel() {
        if (!m_font) return;
        m_label.setFont(*m_font);
        m_label.setString(m_labelStr);
        m_label.setCharacterSize(16);
        m_label.setFillColor(sf::Color(120, 255, 120));
        m_label.setPosition(m_pos);

        m_valueText.setFont(*m_font);
        m_valueText.setCharacterSize(16);
        m_valueText.setFillColor(sf::Color(120, 255, 120));
        m_valueText.setPosition({ m_pos.x + m_width + 12.f, m_pos.y });
    }

    void updateLabelValue() {
        if (!m_font) return;
        // show like 1.0x
        char buf[32];
        std::snprintf(buf, sizeof(buf), "%.2fx", m_value);
        m_valueText.setString(buf);
    }

    void updateKnobFromValue() {
        float t = 0.f;
        if (m_max > m_min) t = (m_value - m_min) / (m_max - m_min);
        float x = m_track.getPosition().x + t * m_width;
        float y = m_track.getPosition().y + 2.f;
        m_knob.setPosition({ x, y });
        m_knob.setFillColor(sf::Color(0, 200, 0));
    }

    void setFromMouse(float mouseX) {
        float left = m_track.getPosition().x;
        float right = left + m_width;
        if (mouseX < left) mouseX = left;
        if (mouseX > right) mouseX = right;
        float t = (mouseX - left) / m_width;
        float v = m_min + t * (m_max - m_min);
        setValue(v);
    }

private:
    sf::Font* m_font = nullptr;
    sf::Vector2f m_pos{ 0.f, 0.f };
    float m_width = 240.f;
    float m_min = 0.2f;
    float m_max = 5.0f;
    float m_value = 1.0f;
    bool m_dragging = false;

    std::string m_labelStr = "Speed";
    sf::Text m_label;
    sf::Text m_valueText;
    sf::RectangleShape m_track;
    sf::RectangleShape m_knob;
};
