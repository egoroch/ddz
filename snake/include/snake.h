
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

    virtual void update(Window &window , sf::Event ev) = 0;


};

class Window {
private:
    State *_state;
    sf::RenderWindow _rend_window;
    sf::Vector2u _windowSize;
    std::string _windowTitle;
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

    virtual void update(Window &window, sf::Event ev) { _state->update(window,ev); };

};




struct SnakeSegment{
    SnakeSegment(int x,int y) : position(x,y){}
    sf::Vector2i position;
};

using SnakeContainer = std::vector<SnakeSegment>;

enum class Direction {None,Up,Down,Left,Right};

class Snake{
public:
    Snake(int l_blockSize);
    Snake() = default;
    ~Snake();

    //methods
    void SetDirection(Direction l_dir);
    Direction GetDirection();
    int GetSpeed();
    sf::Vector2i GetPosition();
    void SetPosition(sf::Vector2i l_pos);
    int GetScore();
    void IncreaseScore();
    bool HasLost();

    void Lose();

    void Extend();
    void Reset();

    void Move();
    void Tick();
    void Render(sf::RenderWindow& l_window);
private:
    void CheckCollision();

    SnakeContainer _snakeBody;
    int _size;
    Direction _dir;
    int _speed;
    int _score;
    bool _lost;
    sf::RectangleShape _bodyRect; // Shape used in rendering
};

class World{
public:
    World(const sf::Vector2u &l_windSize);
    World() = default;
    ~World();

    int GetBlockSize();

    void RespawnApple();

    void Update(Snake& l_player, sf::Event ev);
    void Render(sf::RenderWindow& window);

private:
    sf::Vector2u _windowSize;
    sf::Vector2i _item;
    int _blockSize;

    sf::CircleShape _appleShape;
};


class Game : public State {
private:
    Window *_window;
    World _world;
    Snake _snake;
    sf::Clock _clock;
    sf::Time _elapsed;

public:
    Game();

    explicit Game(Window *window);
    sf::Time GetElapsed();
    void RestartClock();
    ~Game();
    void update(Window &window,sf::Event ev) override;
    void render(Window &window) override;

};

class Options : public State {
    void render(Window &window) override;

    void update(Window &window, sf::Event ev) override;

};

class MainMenu : public State {
    void render(Window &window) override;

    void update(Window &window, sf::Event ev) override;

};




#endif //DDZ_SNAKE_H
