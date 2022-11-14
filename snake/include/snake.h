
#ifndef DDZ_SNAKE_H
#define DDZ_SNAKE_H

#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "ctime"
#include "iostream"

class Window;

class State {
private:
    Window *_window;

public :
    virtual ~State() {}

    void setWindow(Window *cw);

    virtual void render(Window &window) = 0;

    virtual void update(Window &window) = 0;


};

class Window {
private:
    State *_state;
    sf::RenderWindow _rend_window;
    sf::Vector2u _windowSize;
    std::string _windowTitle;
    bool _isDone;
    bool _isFullScreen;

    //private functions
    void Destroy();

    void Create();

    void Setup(const std::string &title, const sf::Vector2u &size);

public:
    Window();

    Window(const std::string &title, const sf::Vector2u &size, State *state);

    ~Window();

    sf::RenderWindow *GetRendWindow();

    bool IsFullScreen();

    sf::Vector2u GetWindowSize();

    void ToggleFullScreen();

    void setState(State *st);

    virtual void render(Window &window) { _state->render(window); };

    virtual void update(Window &window) { _state->update(window); };

};


class MainMenu : public State {
    void render(Window &window) override;

    void update(Window &window) override;

};

class Player;

class Game : public State {
private:
    Window *_window;

    Player *_player;

    void initPlayer();

public:
    Game();

    explicit Game(Window *window);

    ~Game();

    void render(Window &window) override;

    void update(Window &window) override;

};

class Options : public State {
    void render(Window &window) override;

    void update(Window &window) override;

};

class Player : public Game {
public:
    Player();

    virtual ~Player();

    void update();

    void render(sf::RenderTarget &target);

    void changeDir(sf::Vector2f dir);

    void move();
    //functions
private:
    sf::Sprite _sprite;
    sf::Texture _texture;
    const float _movementSpeed = 0.01;
    sf::Vector2f _direct;

    void initTexture();

    void initSprite();

};


#endif //DDZ_SNAKE_H
