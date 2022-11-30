
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
    Direction GetPhysicalDirection();

    std::vector<sf::Vector2i> getBodySnake();
    void Lose();

    void Extend();
    void Reset();

    void Move();
    void Tick(std::vector<sf::Vector2i> items);
    void Render(sf::RenderWindow& l_window);
private:
    void CheckCollision(std::vector<sf::Vector2i> items);

    SnakeContainer _snakeBody;
    int _size;
    Direction _dir;
    int _speed;
    int _score;
    bool _lost;
    sf::RectangleShape _bodyRect; // Shape used in rendering
};

class SnakeBot{
public:
    SnakeBot(int l_blockSize);
    SnakeBot() = default;
    ~SnakeBot();

    //methods
    void SetDirection(Direction l_dir);
    void ChangeDirection(sf::Vector2i apple_pos ,std::vector<sf::Vector2i> items );
    int GetSpeed();
    sf::Vector2i GetPosition();
    void SetPosition(sf::Vector2i l_pos);
    bool HasLost();
    void Disappear();
    std::vector<sf::Vector2i> getBodySnake();
    void Lose();

    void Extend();
    void Reset();

    void Move(sf::Vector2i apple_position);
    void Tick(sf::Vector2i apple_position,std::vector<sf::Vector2i>);
    void Render(sf::RenderWindow& l_window);
private:
    void CheckCollision(std::vector<sf::Vector2i> items);

    SnakeContainer _snakeBody;
    int _size;
    Direction _dir;
    int _speed;
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

    sf::Vector2i getApplePosition();

    void Update(Snake& l_player,std::vector<SnakeBot>& bots);
    void Render(sf::RenderWindow& window);

    std::vector<sf::Vector2i> get_world_items();

private:
    sf::Vector2u _windowSize;
    sf::Vector2i _item;
    int _blockSize;
    sf::CircleShape _appleShape;
    std::vector<sf::Vector2i> allItems;
};

using MessageContainer = std::vector<std::string>;
class Textbox{
public:
    Textbox();
    Textbox(int l_visible , int l_charSize, int l_width, sf::Vector2f l_screenPos);
    ~Textbox();

    void Setup(int l_visible, int l_charSize,int l_width,sf::Vector2f l_screenPos);

    void Add(std::string l_message);
    void Clear();

    void Render(sf::RenderWindow& l_wind);
private:
    MessageContainer _messages;
    int _numVisible;

    sf::RectangleShape _backdrop;
    sf::Font _font;
    sf::Text _content;
};

class Game : public State {
private:
    Window *_window;
    World _world;
    Snake _snake;
    sf::Clock _clock;
    sf::Time _elapsed;
    Textbox _text;
    std::vector<SnakeBot> _bots;
    std::vector<sf::Vector2i> get_game_items();

public:
    Game();

    explicit Game(Window *window);
    sf::Time GetElapsed();
    void RestartClock();
    ~Game();
    void update(Window &window) override;
    void render(Window &window) override;
    SnakeBot& getSnakeBot(int i);

};

class Options : public State {
    void render(Window &window) override;

    void update(Window &window) override;

};

class MainMenu : public State {
    void render(Window &window) override;

    void update(Window &window) override;

};

// в функицю collision передавать массив препятствий и позицию которую надо проверить
//змейка будет смотреть +-1 во все стороны и отталкиваться от этого
//можно передать в функцию collision все объекты (кроме яблока) и потом через указатели брать их положение (еще лучше сделать массив с ключами и смотеть по нему)
//голова бота читает все препятсвия кроме головы игрока (свою голову с ней никогда не столкнется)
//world get items пуст пока  . он нужен для стенок
// в items  вхожят все объекты для столкновения т е кроме яблок



#endif //DDZ_SNAKE_H
