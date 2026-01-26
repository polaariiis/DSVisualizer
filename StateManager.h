#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

struct IState {
    virtual ~IState() = default;
    virtual void handleEvent(const sf::Event& e) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& w) = 0;
};

class StateManager {
    std::unique_ptr<IState> m_current;
    std::unique_ptr<IState> m_pending;

public:
    template<class T>
    void set(std::unique_ptr<T> s) {
        static_assert(std::is_base_of_v<IState, T>);
        m_pending = std::move(s);   // DO NOT swap immediately
    }

    void applyPending() {
        if (m_pending) m_current = std::move(m_pending);
    }

    void handleEvent(const sf::Event& e) { if (m_current) m_current->handleEvent(e); }
    void update(float dt) { if (m_current) m_current->update(dt); }
    void render(sf::RenderWindow& w) { if (m_current) m_current->render(w); }
};
