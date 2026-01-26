#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <string>

#include "StateManager.h"
#include "UI.h"
#include "Algorihms.h"

// Forward declarations (break circular includes)
class MainMenuState;

// =========================
// VisualizerState
// =========================
class VisualizerState : public IState
{
public:
    VisualizerState(StateManager& sm, sf::RenderWindow& window, sf::Font& font)
        : m_sm(sm), m_window(window), m_font(font)
    {
        buildBars();

        backBtn.create({ 120.f, 40.f }, { 30.f, 30.f }, m_font, "Back", 18);
        backBtn.setCallback([this]() {
            m_sm.set(std::make_unique<MainMenuState>(m_sm, m_window, m_font));
            });
    }

    void handleEvent(const sf::Event& e) override
    {
        backBtn.handleEvent(e, m_window);
    }

    void update(float) override
    {
        // later: step sorting here
    }

    void render(sf::RenderWindow& w) override
    {
        for (auto& b : m_bars)
            w.draw(b);

        backBtn.draw(w);
    }

private:
    void buildBars()
    {
        std::vector<float> heights = { 30.f, 80.f, 50.f, 120.f, 60.f, 90.f };

        const float barWidth = 40.f;
        const float gap = 10.f;
        const float startX = 100.f;
        const float baseY = 600.f;

        m_bars.clear();
        m_bars.reserve(heights.size());

        for (size_t i = 0; i < heights.size(); ++i)
        {
            sf::RectangleShape bar;
            bar.setSize({ barWidth, -heights[i] });
            bar.setPosition(startX + i * (barWidth + gap), baseY);
            bar.setFillColor(sf::Color::White);
            m_bars.push_back(bar);
        }
    }

private:
    StateManager& m_sm;
    sf::RenderWindow& m_window;
    sf::Font& m_font;

    std::vector<sf::RectangleShape> m_bars;
    Button backBtn;
};

// =========================
// MainMenuState
// =========================
class MainMenuState : public IState
{
public:
    MainMenuState(StateManager& sm, sf::RenderWindow& window, sf::Font& font)
        : m_sm(sm), m_window(window), m_font(font)
    {
        m_panel.setSize({ 900.f, 180.f });
        m_panel.setPosition({ 190.f, 60.f });
        m_panel.setFillColor(sf::Color(30, 30, 30));
        m_panel.setOutlineThickness(2.f);
        m_panel.setOutlineColor(sf::Color(80, 80, 80));

        m_title.setFont(m_font);
        m_title.setString("DS VIS.");
        m_title.setCharacterSize(56);
        m_title.setFillColor(sf::Color::White);
        centerTextInRect(m_title, m_panel.getGlobalBounds(), 0.f, -35.f);

        m_version.setFont(m_font);
        m_version.setString("v0.1");
        m_version.setCharacterSize(18);
        m_version.setFillColor(sf::Color(170, 170, 170));
        {
            auto pb = m_panel.getGlobalBounds();
            auto vb = m_version.getLocalBounds();
            m_version.setOrigin(vb.left + vb.width, vb.top + vb.height);
            m_version.setPosition(pb.left + pb.width - 12.f, pb.top + pb.height - 12.f);
        }

        ButtonStyle primary;
        primary.fillNormal = sf::Color(90, 90, 90);
        primary.fillHover = sf::Color(120, 120, 120);
        primary.fillPressed = sf::Color(70, 70, 70);

        ButtonStyle secondary;
        secondary.fillNormal = sf::Color(60, 60, 60);
        secondary.fillHover = sf::Color(80, 80, 80);
        secondary.fillPressed = sf::Color(40, 40, 40);

        ButtonStyle danger = secondary;
        danger.fillHover = sf::Color(120, 70, 70);

        const sf::Vector2f btnSize{ 180.f, 52.f };
        const float rowY = m_panel.getPosition().y + 105.f;
        startBtn.create(btnSize, { m_panel.getPosition().x + 140.f, rowY }, m_font, "Start", 24, primary);
        settingsBtn.create(btnSize, { m_panel.getPosition().x + 360.f, rowY }, m_font, "Settings", 24, secondary);
        quitBtn.create(btnSize, { m_panel.getPosition().x + 580.f, rowY }, m_font, "Quit", 24, danger);

        startBtn.setCallback([this]() {
            m_sm.set(std::make_unique<VisualizerState>(m_sm, m_window, m_font));
            });

        settingsBtn.setCallback([this]() {
            // TODO: add real settings state/overlay later
            });

        quitBtn.setCallback([this]() {
            m_window.close();
            });
    }

    void handleEvent(const sf::Event& e) override
    {
        startBtn.handleEvent(e, m_window);
        settingsBtn.handleEvent(e, m_window);
        quitBtn.handleEvent(e, m_window);
    }

    void update(float) override {}

    void render(sf::RenderWindow& w) override
    {
        w.draw(m_panel);
        w.draw(m_title);
        w.draw(m_version);

        startBtn.draw(w);
        settingsBtn.draw(w);
        quitBtn.draw(w);
    }

private:
    static void centerTextInRect(sf::Text& t, const sf::FloatRect& rect, float dx = 0.f, float dy = 0.f)
    {
        auto b = t.getLocalBounds();
        t.setOrigin(b.left + b.width * 0.5f, b.top + b.height * 0.5f);
        t.setPosition(rect.left + rect.width * 0.5f + dx, rect.top + rect.height * 0.5f + dy);
    }

private:
    StateManager& m_sm;
    sf::RenderWindow& m_window;
    sf::Font& m_font;

    sf::RectangleShape m_panel;
    sf::Text m_title;
    sf::Text m_version;

    Button startBtn, settingsBtn, quitBtn;
};
