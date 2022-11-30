
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

    void Update(Snake& l_player);
    void Render(sf::RenderWindow& window);

private:
    sf::Vector2u _windowSize;
    sf::Vector2i _item;
    int _blockSize;

    sf::CircleShape _appleShape;
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


public:
    Game();

    explicit Game(Window *window);
    sf::Time GetElapsed();
    void RestartClock();
    ~Game();
    void update(Window &window) override;
    void render(Window &window) override;

};

class Button {
public:
    Button(std::string btnText, sf::Vector2f buttonSize, int charSize, sf::Color bgColor, sf::Color textColor) {
        _button.setSize(buttonSize);
        _button.setFillColor(bgColor);

        // Get these for later use:
        _btnWidth = buttonSize.x;
        _btnHeight = buttonSize.y;

        _text.setString(btnText);
        _text.setCharacterSize(charSize);
        _text.setColor(textColor);
    }

    // Pass font by reference:
    void setFont(sf::Font &fonts) {
        _text.setFont(fonts);
    }

    void setBackColor(sf::Color color) {
        _button.setFillColor(color);
    }

    void setTextColor(sf::Color color) {
        _text.setColor(color);
    }

    void setPosition(sf::Vector2f point) {
        _button.setOrigin(_btnWidth/2.0f, _btnHeight/2.0f);
        _button.setPosition(point);

        sf::FloatRect titleRect = _text.getLocalBounds();
        _text.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
        _text.setPosition(point);
    }

    void drawTo(sf::RenderWindow &window) {
        window.draw(_button);
        window.draw(_text);
    }

    // Check if the mouse is within the bounds of the button:
    bool isMouseOver(sf::RenderWindow &window) {
        int mouseX = sf::Mouse::getPosition(window).x;
        int mouseY = sf::Mouse::getPosition(window).y;

        int btnPosX = _button.getPosition().x - _btnWidth/2.0f;
        int btnPosY = _button.getPosition().y - _btnHeight/2.0f;

        int btnxPosWidth = _button.getPosition().x + _btnWidth/2.0f;
        int btnyPosHeight = _button.getPosition().y + _btnHeight/2.0f;


        return mouseX < btnxPosWidth && mouseX > btnPosX && mouseY < btnyPosHeight && mouseY > btnPosY;
    }
private:
    sf::RectangleShape _button;
    sf::Text _text;

    int _btnWidth;
    int _btnHeight;
};

class Options : public State {
public:

    void render(Window &window) override;

    void update(Window &window) override;

};



class MainMenu : public State {
private:
    bool _isStart = false;
    bool _isSettings = false;
    bool _isExit = false;
public:

    MainMenu(){};

    void render(Window &window) override;

    void update(Window &window) override;

};





#endif //DDZ_SNAKE_H
