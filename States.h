#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <random>
#include <algorithm>

#include "StateManager.h"
#include "UI.h"
#include "Algorihms.h"

class MainMenuState;

// ============================================================================
// VisualizerState (sorting visualizer screen)
// ============================================================================
class VisualizerState : public IState {
public:
    VisualizerState(StateManager& sm, sf::RenderWindow& window, sf::Font& font)
        : m_sm(sm), m_window(window), m_font(font)
    {
        buildLayout();
        buildUI();
        buildBars();
        seedDemo();
    }


    void handleEvent(const sf::Event& e) override {
        // global back
        if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
            goBack();
            return;
        }

        backBtn.handleEvent(e, m_window);
        addBtn.handleEvent(e, m_window);
        removeBtn.handleEvent(e, m_window);
        startBtn.handleEvent(e, m_window);
        stopBtn.handleEvent(e, m_window);

        methods.handleEvent(e, m_window);
        speed.handleEvent(e, m_window);
    }

    void update(float dt) override {
        if (!m_running) return;

        // speed is a multiplier: bigger = faster steps
        const float mult = speed.getValue();
        const float stepDelay = 0.18f / (mult <= 0.f ? 1.f : mult);
        m_stepAccum += dt;

        while (m_stepAccum >= stepDelay) {
            m_stepAccum -= stepDelay;
            doSortStep();
            if (m_sorted) {
                m_running = false;
                break;
            }
        }
    }

    void render(sf::RenderWindow& w) override {
        w.clear(sf::Color(15, 15, 15));

        // panels
        w.draw(m_leftPanel);
        w.draw(m_mainPanel);
        w.draw(m_bottomPanel);

        // left list
        methods.draw(w);

        // bars
        drawBars(w);

        // bottom widgets
        addBtn.draw(w);
        removeBtn.draw(w);
        startBtn.draw(w);
        stopBtn.draw(w);

        // added numbers box
        w.draw(m_addedTitle);
        w.draw(m_addedText);

        // speed
        speed.draw(w);

        // back
        backBtn.draw(w);
    }

private:
    enum class Algo {
        Bubble,
        Insertion,
        Selection
    };
    std::vector<sf::RectangleShape> m_bars;
    void buildBars()
    {
        auto r = m_mainPanel.getGlobalBounds();
        float padding = 20.f;

        float usableW = r.width - 2 * padding;
        float barW = usableW / (float)m_values.size();
        float baseY = r.top + r.height - padding;

        m_bars.clear();
        m_bars.reserve(m_values.size());

        float maxV = 1.f;
        for (int v : m_values) maxV = std::max(maxV, (float)v);

        for (size_t i = 0; i < m_values.size(); ++i)
        {
            float h = (m_values[i] / maxV) * (r.height - 2 * padding);

            sf::RectangleShape bar;
            bar.setFillColor(sf::Color(0, 180, 0));
            bar.setSize({ barW * 0.85f, -h });               // negative up
            bar.setPosition({ r.left + padding + i * barW, baseY }); // FIXED X
            m_bars.push_back(bar);
        }
    }
    void buildLayout() {
        const auto size = m_window.getSize();
        const float W = (float)size.x;
        const float H = (float)size.y;

        // left methods panel
        m_leftPanel.setPosition({ 0.05f * W, 0.12f * H });
        m_leftPanel.setSize({ 0.22f * W, 0.76f * H });
        m_leftPanel.setFillColor(sf::Color(25, 25, 25));

        // main visualization
        m_mainPanel.setPosition({ m_leftPanel.getPosition().x + m_leftPanel.getSize().x + 0.03f * W, 0.12f * H });
        m_mainPanel.setSize({ 0.62f * W, 0.55f * H });
        m_mainPanel.setFillColor(sf::Color(20, 20, 20));

        // bottom bar
        m_bottomPanel.setPosition({ 0.05f * W, 0.70f * H });
        m_bottomPanel.setSize({ 0.90f * W, 0.18f * H });
        m_bottomPanel.setFillColor(sf::Color(22, 22, 22));

        // bars baseline inside main panel
        m_baseY = m_mainPanel.getGlobalBounds().top + m_mainPanel.getGlobalBounds().height - 18.f;
        m_barAreaLeft = m_mainPanel.getGlobalBounds().left + 18.f;
        m_barAreaWidth = m_mainPanel.getGlobalBounds().width - 36.f;
        m_barMaxHeight = m_mainPanel.getGlobalBounds().height - 36.f;
    }

    void buildUI() {
        ButtonStyle basic;
        basic.fillNormal = sf::Color(40, 40, 40);
        basic.fillHover = sf::Color(55, 55, 55);
        basic.fillPressed = sf::Color(30, 30, 30);
        basic.textColor = sf::Color(140, 255, 140);

        ButtonStyle primary = basic;
        primary.fillNormal = sf::Color(0, 90, 0);
        primary.fillHover = sf::Color(0, 120, 0);
        primary.fillPressed = sf::Color(0, 70, 0);
        primary.textColor = sf::Color(200, 255, 200);

        // back button
        backBtn.create({ 120.f, 40.f }, { 20.f, 20.f }, m_font, "Back", 18, basic);
        backBtn.setCallback([this]() { goBack(); });

        // methods list
        methods.setFont(m_font);
        methods.setArea(m_leftPanel.getGlobalBounds());
        methods.setTitle("Sorting methods");
        methods.setItems({ "Bubble", "Insertion", "Selection" });
        methods.setColor(sf::Color(120, 255, 120), sf::Color(0, 220, 0));

        // bottom controls
        const auto b = m_bottomPanel.getGlobalBounds();
        float x = b.left + 18.f;
        float y = b.top + 18.f;

        addBtn.create({ 150.f, 38.f }, { x, y }, m_font, "add number", 16, basic);
        removeBtn.create({ 170.f, 38.f }, { x + 160.f, y }, m_font, "remove number", 16, basic);
        startBtn.create({ 120.f, 38.f }, { x, y + 46.f }, m_font, "start", 16, primary);
        stopBtn.create({ 120.f, 38.f }, { x + 130.f, y + 46.f }, m_font, "stop", 16, basic);

        addBtn.setCallback([this]() { addRandom(); });
        removeBtn.setCallback([this]() { removeLast(); });
        startBtn.setCallback([this]() { startSorting(); });
        stopBtn.setCallback([this]() { m_running = false; });

        // added numbers
        m_addedTitle.setFont(m_font);
        m_addedTitle.setCharacterSize(16);
        m_addedTitle.setFillColor(sf::Color(120, 255, 120));
        m_addedTitle.setString("Added numbers");
        m_addedTitle.setPosition({ b.left + 380.f, b.top + 18.f });

        m_addedText.setFont(m_font);
        m_addedText.setCharacterSize(14);
        m_addedText.setFillColor(sf::Color(120, 255, 120));
        m_addedText.setPosition({ b.left + 380.f, b.top + 42.f });

        // speed slider
        speed.setFont(m_font);
        speed.setLabel("speed");
        speed.setRange(0.2f, 5.0f);
        speed.setValue(1.0f);
        speed.setPosition({ b.left + b.width - 320.f, b.top + 40.f });
        speed.setWidth(240.f);

        refreshAddedText();
    }

    void seedDemo() {
        m_values = { 60, 120, 80, 140, 90, 110 };
        rebuildBars();
    }

    void rebuildBars() {
        m_bars.clear();
        if (m_values.empty()) return;

        const size_t n = m_values.size();
        const float gap = 8.f;
        float barW = (m_barAreaWidth - gap * (float)(n - 1)) / (float)n;
        if (barW < 6.f) barW = 6.f;

        // scale values to fit max height
        float maxV = 1.f;
        for (int v : m_values) maxV = std::max(maxV, (float)v);
        const float scale = (maxV > 0.f) ? (m_barMaxHeight / maxV) : 1.f;

        m_barSpacing = barW + gap;
        m_baseX = m_barAreaLeft - m_barSpacing; // so that baseX*(i+1) works

        m_bars.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            float h = (float)m_values[i] * scale;
            sf::RectangleShape bar;
            bar.setSize({ barW, h });
            bar.setOrigin({ 0.f, h }); // bottom anchored
            bar.setFillColor(sf::Color(0, 170, 0));
            bar.setPosition({ m_baseX + m_barSpacing * (float)(i + 1), m_baseY });
            m_bars.push_back(bar);
        }
    }

    void refreshAddedText() {
        std::string s;
        const size_t maxShow = 18;
        size_t start = (m_values.size() > maxShow) ? (m_values.size() - maxShow) : 0;
        for (size_t i = start; i < m_values.size(); ++i) {
            s += std::to_string(m_values[i]);
            if (i + 1 < m_values.size()) s += ", ";
        }
        m_addedText.setString(s);
    }

    void addRandom() {
        if (m_running) return;
        if (m_values.size() >= 60) return;
        int v = randInt(20, 160);
        m_values.push_back(v);
        refreshAddedText();
        rebuildBars();
    }

    void removeLast() {
        if (m_running) return;
        if (m_values.empty()) return;
        m_values.pop_back();
        refreshAddedText();
        rebuildBars();
    }

    void startSorting() {
        if (m_bars.size() < 2) return;
        m_running = true;
        m_sorted = false;
        m_stepAccum = 0.f;

        // reset per-algo state
        bubble = {};
        insert = {};
        select = {};
    }

    Algo currentAlgo() const {
        switch (methods.selectedIndex()) {
        case 0: return Algo::Bubble;
        case 1: return Algo::Insertion;
        case 2: return Algo::Selection;
        default: return Algo::Bubble;
        }
    }

    void doSortStep() {
        // We use your step functions (they reposition for us).
        // We mark "sorted" if the step state reaches the end.
        switch (currentAlgo()) {
        case Algo::Bubble:
            Sorting::bubbleSortStep(bubble, m_bars, m_barSpacing, m_baseY);
            if (bubble.i >= (int)m_bars.size() - 1) m_sorted = true;
            break;
        case Algo::Insertion:
            Sorting::sortingStep(insert, m_bars, m_barSpacing, m_baseY);
            if (insert.i >= (int)m_bars.size()) m_sorted = true;
            break;
        case Algo::Selection:
            Sorting::selectionStep(select, m_bars, m_barSpacing, m_baseY);
            if (select.i >= (int)m_bars.size() - 1) m_sorted = true;
            break;
        }
    }

    void drawBars(sf::RenderWindow& w)
    {
        for (auto& b : m_bars) w.draw(b);
    }


    void goBack();

    static int randInt(int a, int b) {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<int> dist(a, b);
        return dist(rng);
    }

private:
    StateManager& m_sm;
    sf::RenderWindow& m_window;
    sf::Font& m_font;

    // panels
    sf::RectangleShape m_leftPanel;
    sf::RectangleShape m_mainPanel;
    sf::RectangleShape m_bottomPanel;

    // UI
    Button backBtn;
    TextList methods;
    Button addBtn, removeBtn, startBtn, stopBtn;
    Slider speed;
    sf::Text m_addedTitle;
    sf::Text m_addedText;

    // bars
    std::vector<int> m_values;

    float m_baseY = 500.f;
    float m_barAreaLeft = 100.f;
    float m_barAreaWidth = 600.f;
    float m_barMaxHeight = 300.f;
    float m_baseX = 0.f;
    float m_barSpacing = 48.f;

    // sorting state
    bool m_running = false;
    bool m_sorted = false;
    float m_stepAccum = 0.f;
    bubbleSortData bubble{};
    sortingData insert{};
    selectionData select{};
};

// ============================================================================
// MainMenuState (start/settings/quit)
// ============================================================================
class MainMenuState : public IState {
public:
    MainMenuState(StateManager& sm, sf::RenderWindow& window, sf::Font& font)
        : m_sm(sm), m_window(window), m_font(font)
    {
        buildLayout();
        buildUI();
    }

    void handleEvent(const sf::Event& e) override {
        startBtn.handleEvent(e, m_window);
        settingsBtn.handleEvent(e, m_window);
        quitBtn.handleEvent(e, m_window);
    }

    void update(float) override {}

    void render(sf::RenderWindow& w) override {
        w.clear(sf::Color(15, 15, 15));
        w.draw(panel);
        w.draw(title);
        w.draw(version);
        startBtn.draw(w);
        settingsBtn.draw(w);
        quitBtn.draw(w);
    }

private:
    void buildLayout() {
        const auto size = m_window.getSize();
        const float W = (float)size.x;
        const float H = (float)size.y;

        panel.setSize({ 0.60f * W, 0.36f * H });
        panel.setFillColor(sf::Color(20, 20, 20));
        panel.setPosition({ (W - panel.getSize().x) * 0.5f, (H - panel.getSize().y) * 0.28f });
    }

    void buildUI() {
        ButtonStyle basic;
        basic.fillNormal = sf::Color(40, 40, 40);
        basic.fillHover = sf::Color(55, 55, 55);
        basic.fillPressed = sf::Color(30, 30, 30);
        basic.textColor = sf::Color(200, 255, 200);

        ButtonStyle primary = basic;
        primary.fillNormal = sf::Color(0, 90, 0);
        primary.fillHover = sf::Color(0, 120, 0);
        primary.fillPressed = sf::Color(0, 70, 0);

        // title
        title.setFont(m_font);
        title.setString("DS Visualizer");
        title.setCharacterSize(40);
        title.setFillColor(sf::Color(200, 255, 200));
        title.setPosition(panel.getPosition().x + 40.f, panel.getPosition().y + 24.f);

        // version
        version.setFont(m_font);
        version.setString("v0.1");
        version.setCharacterSize(16);
        version.setFillColor(sf::Color(120, 255, 120));
        version.setPosition(panel.getPosition().x + panel.getSize().x - 50.f,
            panel.getPosition().y + panel.getSize().y - 34.f);

        const float y = panel.getPosition().y + panel.getSize().y * 0.55f;
        float x = panel.getPosition().x + 60.f;

        startBtn.create({ 160.f, 44.f }, { x, y }, m_font, "start", 18, primary);
        settingsBtn.create({ 160.f, 44.f }, { x + 190.f, y }, m_font, "settings", 18, basic);
        quitBtn.create({ 160.f, 44.f }, { x + 380.f, y }, m_font, "quit", 18, basic);

        startBtn.setCallback([this]() {
            m_sm.set(std::make_unique<VisualizerState>(m_sm, m_window, m_font));
            });

        settingsBtn.setCallback([this]() {
            // placeholder for later settings screen
            });

        quitBtn.setCallback([this]() {
            m_window.close();
            });
    }

private:
    StateManager& m_sm;
    sf::RenderWindow& m_window;
    sf::Font& m_font;

    sf::RectangleShape panel;
    sf::Text title;
    sf::Text version;

    Button startBtn, settingsBtn, quitBtn;
};

inline void VisualizerState::goBack() {
    m_sm.set(std::make_unique<MainMenuState>(m_sm, m_window, m_font));
}
