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
class Window {
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
};
//_________________________________________________________________________
class CurrentWindow;

class State {
protected:
    CurrentWindow* _currentWindow;
public :
    virtual ~State(){}
    void setWindow(CurrentWindow* cw);
   // virtual void Update() = 0;
    virtual void Draw(sf::RenderWindow& window) =0;

};

class CurrentWindow{
private:
    State* state;
public:
    CurrentWindow(State* st) :state(nullptr){setState(st);}
    void setState(State* st);
    virtual void Draw(sf::RenderWindow& window) {state->Draw(window);};
};


class MainMenu :public State
{
   // void Update() override;
    void Draw(sf::RenderWindow& window) override;
};

class Game :public State
{
    //void Update() override;
    void Draw(sf::RenderWindow& window) override;
};

class Options :public State
{
    //void Update() override;
    void Draw(sf::RenderWindow& window) override;
};
//__________________________________________________________






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
