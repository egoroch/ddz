
#ifndef DDZ_SNAKE_H
#define DDZ_SNAKE_H

#include <iostream>
#include <fstream>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "../lib/single_include/nlohmann/json.hpp"
#include "ctime"
#include "iostream"

using json=nlohmann::json;

sf::Text makeText(std::string text, sf::Font& font, sf::Vector2f position, sf::Color color = sf::Color::White, int charSize = 30);

json getConfig(const std::string& fileName);

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
    json _config;
    json _currentConfig;

    //private functions
    void Destroy();

    void Create();

    void Setup(const std::string &title, const sf::Vector2u &size);

public:
    Window();

    Window(const std::string &title, const sf::Vector2u &size, json config, State *state);

    ~Window();

    sf::RenderWindow *GetRendWindow();

    bool IsFullScreen();

    sf::Vector2u GetWindowSize();

    json getConfig() { return _currentConfig; };

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
    Snake(int l_blockSize,bool is_multy);
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
    void Reset(bool is_multy);
    void Disapear();

    void Move();
    void Tick();
    void Render(sf::RenderWindow& l_window);


    void CheckCollision(std::vector<sf::Vector2i> items);
private:
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
    SnakeBot(int l_blockSize,sf::Vector2i headPos);
    SnakeBot() = default;
    ~SnakeBot();


    //methods
    void SetDirection(Direction l_dir);
    void ChangeDirection(sf::Vector2i apple_pos ,std::vector<sf::Vector2i> items,Direction player_dir );
    int GetSpeed();
    sf::Vector2i GetPosition();
    void SetPosition(sf::Vector2i l_pos);
    bool HasLost();
    void Disappear();
    std::vector<sf::Vector2i> getBodySnake();
    void Lose();

    void Extend();
    void Reset(sf::Vector2i headPos);

    void Move(sf::Vector2i apple_position);
    void Tick(sf::Vector2i apple_position,std::vector<sf::Vector2i>,Direction player_dir);
    void Render(sf::RenderWindow& l_window);

    void CheckCollision(std::vector<sf::Vector2i> items);
private:
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

    void Update(Snake& l_player,Snake &secondPlayer,std::vector<SnakeBot>& bots, std::vector<sf::Vector2i> items);
    void Render(sf::RenderWindow& window);

    std::vector<sf::Vector2i> get_world_items();

private:
    sf::Vector2u _windowSize;
    sf::Vector2i _item;
    int _blockSize;
    sf::CircleShape _appleShape;
    std::vector<sf::Vector2i> _stonesPos;
   // sf::RectangleShape _stone;
    std::vector<sf::RectangleShape> _stoneShape;

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
    Snake _player2_snake;
    bool _is_multiplayer;
    sf::Clock _clock;
    sf::Time _elapsed;
    Textbox _text;
    std::vector<SnakeBot> _bots;
    int _rounds;
    int _countOfBots;
    int _firstRounds;
    int _secondRounds;
    std::vector<sf::Vector2i> get_game_items();

public:
    Game();

    void CreateAllBots(Window *window,int blockSIze , std::vector<sf::Vector2i> items,int count);
    explicit Game(Window *window , int count,int rounds ,bool is_multiplayer);
    sf::Time GetElapsed();
    void RestartClock();
    ~Game();
    void update(Window &window) override;
    void render(Window &window) override;
    SnakeBot& getSnakeBot(int i);

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
        _text.setFillColor(textColor);
    }

    // Pass font by reference:
    void setFont(sf::Font &fonts) {
        _text.setFont(fonts);
    }

    sf::Vector2f getSize(){
        sf::Vector2f size = {_btnWidth, _btnHeight};
        return size;
    }

    void setBackColor(sf::Color color) {
        _button.setFillColor(color);
    }

    void setTextColor(sf::Color color) {
        _text.setFillColor(color);
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

    sf::RectangleShape getButton(){
        return _button;
    }
private:
    sf::RectangleShape _button;
    sf::Text _text;

    float _btnWidth;
    float _btnHeight;
};

class TextField{
public:
    TextField(unsigned int maxChars, sf::Vector2f position) :
            _size(maxChars),
            _rect(sf::Vector2f(15 * _size, 40)), // 15 pixels per char, 20 pixels height, you can tweak
            _position(position),
            _hasfocus(false)
    {
        _font.loadFromFile("Textures/arial.ttf");
        _rect.setOutlineThickness(2);
        _rect.setFillColor(sf::Color::White);
        _rect.setOutlineColor(sf::Color(127,127,127));
        sf::FloatRect resultRect = _rect.getLocalBounds();
        _rect.setOrigin(resultRect.left + resultRect.width/2.0f, resultRect.top + resultRect.height/2.0f);
        _rect.setPosition(this->_position);
    }

    void setPosition(float x, float y) {
        _rect.setPosition(x, y);
    }

    std::string getString() {
        return _text;
    }

    bool contains(sf::Vector2f point) const{
        return _rect.getGlobalBounds().contains(point);
    }

    void setFocus(bool focus){
        _hasfocus = focus;
        if (focus){
            _rect.setOutlineColor(sf::Color::Blue);
        }
        else{
            _rect.setOutlineColor(sf::Color(127, 127, 127)); // Gray color
        }
    }

    void handleInput(sf::Event e){
        if (!_hasfocus || e.type != sf::Event::TextEntered)
            return;

        if (e.text.unicode == 8){   // Delete key
            _text = _text.substr(0, _text.size() - 1);
        }
        else if (_text.size() < _size){
            _text += e.text.unicode;
        }
    }

    void draw(sf::RenderWindow& window){
        _inputText = makeText(_text, _font, _position, sf::Color::Black, 18);
        window.draw(_rect);
        window.draw(_inputText);
    }

    void released(sf::RenderWindow& window, sf::Event& event) {
        auto pos = sf::Mouse::getPosition(window);
        this->setFocus(false);
        if (this->contains(sf::Vector2f(pos))) {
            this->setFocus(true);
        }
    }

private:
    unsigned int _size;
    sf::Font _font;
    std::string _text = "";
    sf::Text _inputText;
    sf::RectangleShape _rect;
    sf::Vector2f _position;
    bool _hasfocus;
};

class Options : public State {
public:
    Options(Window* window) {
        _window = window;

        sf::Vector2f size = {250, 80};
        _save = new Button("Save", size, 30, sf::Color::Blue, sf::Color::Yellow);
        _back = new Button("Back", size, 30, sf::Color::Blue, sf::Color::Yellow);

        _name = new TextField(30, {3.0f*_window->GetWindowSize().x/4.0f, _window->GetWindowSize().y/6.0f });

    }

    void render(Window &window) override;

    void update(Window &window) override;
private:
    Window* _window;
    Button* _save;
    Button* _back;

    TextField* _name;
    TextField* _color;
    TextField* _windowSize;

    bool _isSave;
    bool _isBack;
};



class MainMenu : public State {
public:

    MainMenu(Window* window){
        _window = window;
        json config = _window->getConfig();
        sf::Vector2f size = {config["button_width"], config["button_height"]};
        _start = new Button("Start", size, 32, sf::Color::Blue, sf::Color::Yellow);
        _startGame = new Button("Start Game", size, 32, sf::Color::Blue, sf::Color::Yellow);
        _settings = new Button("Settings", size, 32, sf::Color::Blue, sf::Color::Yellow);
        _exit = new Button("Exit", size, 32, sf::Color::Blue, sf::Color::Yellow);
        _textField = new TextField(30, {3.0f*_window->GetWindowSize().x/4.0f, _window->GetWindowSize().y/6.0f });
        _textFieldBots = new TextField(30, {3.0f*_window->GetWindowSize().x/4.0f, 2.0f*_window->GetWindowSize().y/6.0f });
        _textUser = new TextField(30, {3.0f*_window->GetWindowSize().x/4.0f, 3.0f*_window->GetWindowSize().y/6.0f });


        _title.setCharacterSize(44);
        _title.setString("The Best Snake");
        _title.setFillColor(sf::Color::White);

    };

    void render(Window &window) override;

    void update(Window &window) override;

private:
    Window* _window;
    sf::Text _title;
    Button* _start;
    Button* _startGame;
    Button* _settings;
    Button* _exit;
    TextField* _textField;
    TextField* _textFieldBots;
    TextField* _textUser;

    bool _isStart = false;
    bool _isSettings = false;
    bool _isExit = false;
};



#endif //DDZ_SNAKE_H
