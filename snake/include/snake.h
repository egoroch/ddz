//
// Created by egor on 10/30/22.
//

#ifndef DDZ_SNAKE_H
#define DDZ_SNAKE_H

#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Network.hpp"

// created by egor 03/11/22
/*class _Window {
public:
    Window();
    Window(const std::string& title, const sf::Vector2u& size);
    ~Window();

    void BeginDraw();
    void EndDraw();

    void Update();

    bool IsDone();
    bool IsFullScreen();
    sf::Vector2u GetWindowSize();

    void ToggleFullScreen();

    void Draw(sf::Drawable& drawable);
private:
    void Setup(const std::string& title, const sf::Vector2u& size);
    void Destroy();
    void Create();

    sf::RenderWindow _window;
    sf::Vector2u _windowSize;
    std::string _windowTitle;
    bool _isDone;
    bool _isFullScreen;
};*/
//_________________________________________________________________________
class Window;

class State {
private:
    Window *_window;

public :
    virtual ~State() {}

    void setWindow(Window *cw);

    virtual void render(Window &window) = 0;

    virtual void update() = 0;

};

class Window {
private:
    State *_state;
    sf::RenderWindow _rend_window;
    sf::Vector2f _windowSize;
    std::string _windowTitle;
    bool _isDone;
    bool _isFullScreen;

    //private functions
    void Destroy();

    void Create();

    void Setup(const std::string &title, const sf::Vector2f &size);

public:
    Window();

    Window(const std::string &title, const sf::Vector2f &size, State *state);

    ~Window();

    sf::RenderWindow *GetRendWindow();

    bool IsFullScreen();

    sf::Vector2f GetWindowSize();

    void ToggleFullScreen();

    void setState(State *st);

    virtual void render(Window &window) { _state->render(window); };

    virtual void update() { _state->update(); };
};

class Button {
public:
    Button(sf::RectangleShape button, sf::Text value, sf::Color color, sf::Vector2f size,
           sf::Vector2f position) : _button(button), _value(value), _color(color), _size(size), _position(position) {
        _button.setFillColor(_color);
        _button.setSize(_size);
        _button.setOrigin(_size.x / 2.0f, _size.y / 2.0f);
        _button.setPosition(_position);

        sf::FloatRect textRect = _value.getLocalBounds();
        _value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        _value.setPosition(_position);
    };

    void Draw(Window &window) {
        window.GetRendWindow()->draw(_button);
        window.GetRendWindow()->draw(_value);
    }

private:
    sf::RectangleShape _button;
    sf::Text _value;
    sf::Color _color;
    sf::Vector2f _size;
    sf::Vector2f _position;
};

class MainMenu : public State {
    void render(Window &window) override;

    void update() override;
};

class Game : public State {
public:
    void render(Window &window) override;

    void update() override;
};

class Options : public State {
    void render(Window &window) override;

    void update() override;
};

class Enemy : public Game {
private:
    sf::RectangleShape _enemy;
public:
    Enemy();

    sf::RectangleShape GetEnemy() { return _enemy; };
};
//__________________________________________________________


//сделать классы наследующие игры и через них создавать карту, мобов






/*
class BaseWindowState {
public:
    ~BaseWindowState() {};
    virtual void handleInput(Window& window) {}
    virtual void update(Window& window) {}
};*/
/*
class MainMenu{
public:
    MainMenu();
    ~MainMenu() = default;

    void update();
    void Draw();

    bool IsDone() { return _window.IsDone(); };


private:
    Window _window;
    void SetupButtons();

    sf::Text _text;
    sf::Vector2f _buttonSize;
    sf::Vector2f _buttonPosition;
    sf::Text _labels[3];
    sf::RectangleShape _rects[3];
};
*/
/*
class CurrentWindow: public BaseWindowState{
public:
    virtual void handleInput() {
        _state->handleInput(_window);
        BaseWindowState* state = _state;
        if(state != NULL) {
            delete _state;
            _state = state;
        }

    }
    virtual void update() {
        _state->update(_window);
    }

    bool IsDone() { return _window.IsDone(); };
private:
    Window _window;
    BaseWindowState* _state;
};
*/

/*class MainMenu{
public:
    MainMenu();
    ~MainMenu();

    //void HandleInput();
    void Update();
    void Render();
    Window& GetWindow();

private:
    void SetupButtons();
    void SetupGraphics();

    Window _window;
    sf::RectangleShape _buttonStart;
    sf::RectangleShape _buttonSettings;
};*/



#endif //DDZ_SNAKE_H
