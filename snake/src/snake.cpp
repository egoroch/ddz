
#include "../include/snake.h"

// constructors and destructor
Window::Window() {
    Setup("Snake", sf::Vector2u(1280, 720));
}

Window::Window(const std::string &title, const sf::Vector2u &size, State *state) {
    Setup(title, size);
    _state = state;
    Create();
}

Window::~Window() {
    Destroy();
}

// functions
sf::Vector2u Window::GetWindowSize() {
    return _windowSize;
};

void Window::Destroy() {
    _rend_window.close();
    delete _state;
}

void Window::ToggleFullScreen() {
    _isFullScreen = !_isFullScreen;
    Destroy();
    Create();
}

void Window::Setup(const std::string &title, const sf::Vector2u &size) {
    _windowTitle = title;
    _windowSize = size;
    _isFullScreen = false;
    Create();
}

sf::RenderWindow *Window::GetRendWindow() {
    return &_rend_window;
}

void Window::Create() {
    auto style = (_isFullScreen ? sf::Style::Fullscreen : sf::Style::Default);
    _rend_window.create({_windowSize.x, _windowSize.y, 32}, _windowTitle, style);
}

void State::setWindow(Window *cw) {
    _window = cw;
}

bool Window::IsFullScreen() { return _isFullScreen; }


void Window::setState(State *st) {
   // if (_state != nullptr) delete _state;
    _state = st;
    _state->setWindow(this);
}

void MainMenu::render(Window& window) {
    window.GetRendWindow()->clear(sf::Color::Black);
    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf")) {
        return;
    }
    _start->setFont(font);
    _settings->setFont(font);
    _exit->setFont(font);
    _title.setFont(font);

    _start->setBackColor(sf::Color::Blue);
    _start->setTextColor(sf::Color::Yellow);
    _settings->setBackColor(sf::Color::Blue);
    _settings->setTextColor(sf::Color::Yellow);
    _exit->setBackColor(sf::Color::Blue);
    _exit->setTextColor(sf::Color::Yellow);

    _start->setPosition({_window->GetWindowSize().x / 2.0f, _window->GetWindowSize().y / 2.0f});
    _settings->setPosition({_window->GetWindowSize().x / 2.0f, _window->GetWindowSize().y / 2.0f + 110});
    _exit->setPosition({_window->GetWindowSize().x / 2.0f, _window->GetWindowSize().y / 2.0f + 220});
    sf::FloatRect titleRect = _title.getLocalBounds();
    _title.setOrigin({titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f});
    _title.setPosition(_window->GetWindowSize().x / 2, _window->GetWindowSize().y / 6);

    this->_isStart = _start->isMouseOver(*window.GetRendWindow());
    this->_isExit = _exit->isMouseOver(*this->_window->GetRendWindow());
    this->_isSettings = _settings->isMouseOver(*this->_window->GetRendWindow());
    if(_isStart) {
        _start->setBackColor(sf::Color::Red);
        _start->setTextColor(sf::Color::Green);
    }
    if(_isExit) {
        _exit->setBackColor(sf::Color::Red);
        _exit->setTextColor(sf::Color::Green);
    }
    if(_isSettings){
        _settings->setBackColor(sf::Color::Red);
        _settings->setTextColor(sf::Color::Green);
    }

    window.GetRendWindow()->draw(_title);
    _start->drawTo(*window.GetRendWindow());
    _settings->drawTo(*window.GetRendWindow());
    _exit->drawTo(*window.GetRendWindow());
    _window->GetRendWindow()->display();
}

void MainMenu::update(Window &window) {
    window.GetRendWindow()->clear(sf::Color::Magenta);
    //render stuff
    sf::Event e;
    while (window.GetRendWindow()->pollEvent(e)) {
        if (e.Event::type == sf::Event::Closed)
            window.GetRendWindow()->close();
        if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
            window.GetRendWindow()->close();
        if(this->_isExit) {
            if(e.type == sf::Event::MouseButtonPressed)
                window.GetRendWindow()->close();
        }
        if(this->_isStart) {
            if(e.type == sf::Event::MouseButtonPressed )
                window.setState(new Game(&window));
        }
    }
};

void Options::render(Window &window) {
}

void Options::update(Window &window) {
    window.GetRendWindow()->clear(sf::Color::Cyan);
    //render stuff
    sf::Event e;
    while (window.GetRendWindow()->pollEvent(e)) {
        if (e.Event::type == sf::Event::Closed)
            window.GetRendWindow()->close();
        if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
            window.GetRendWindow()->close();
    };
}

Game::Game() {
    _window = nullptr;
    _snake = Snake(0);
    sf::Vector2u size(0, 0);
    _world = size;
    _text.Setup(1,30,350,sf::Vector2f(225,0));
    _text.Add("Seeded random number generator with: " + std::to_string(time(NULL)));
}

Game::Game(Window *window) {
    _window = window;
    _world = World(_window->GetWindowSize());
    _snake = Snake(_world.GetBlockSize());
    _text.Setup(2,14,300,sf::Vector2f(225,0));
}

Game::~Game() {
    //delete _player;
}

void Game::render(Window &window) {
    window.GetRendWindow()->clear();
    _world.Render(*_window->GetRendWindow());
    _snake.Render(*_window->GetRendWindow());
    _text.Add(std::to_string(_snake.GetScore()));
    _text.Render(*_window->GetRendWindow());
    _window->GetRendWindow()->display();
    this->RestartClock();
    // _player->move();
    //_player->render(*(_window->GetRendWindow()));
}

void Game::update(Window &window) {
    sf::Event event;
    while (window.GetRendWindow()->pollEvent(event)) {
        if (event.Event::type == sf::Event::Closed)
            _window->GetRendWindow()->close();
        if (event.Event::KeyPressed && event.Event::key.code == sf::Keyboard::M)
            _window->setState(new MainMenu(_window));
        if (event.Event::KeyPressed && event.Event::key.code == sf::Keyboard::Escape)
            _window->GetRendWindow()->close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
            && _snake.GetPhysicalDirection() != Direction::Down) {
            _snake.SetDirection(Direction::Up);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                   && _snake.GetPhysicalDirection() != Direction::Up) {
            _snake.SetDirection(Direction::Down);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                   && _snake.GetPhysicalDirection() != Direction::Right) {
            _snake.SetDirection(Direction::Left);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
                   && _snake.GetPhysicalDirection() != Direction::Left) {
            _snake.SetDirection(Direction::Right);
        }
    };
    float timestep = 1.0f / _snake.GetSpeed();

    if (_elapsed.asSeconds() >= timestep) {
        _snake.Tick();
        _world.Update(_snake);
        _elapsed -= sf::seconds(timestep);
        if (_snake.HasLost()) {
            _snake.Reset();
        }
  }
    this->RestartClock();
};


Snake::Snake(int l_blockSize) {
    _size = l_blockSize;
    _bodyRect.setSize(sf::Vector2f(_size - 1, _size - 1));
    Reset();
}

Snake::~Snake() {}

void Snake::Reset() {
    _snakeBody.clear();

    _snakeBody.push_back(SnakeSegment(5, 7));
    _snakeBody.push_back(SnakeSegment(5, 6));
    _snakeBody.push_back(SnakeSegment(5, 5));

    SetDirection(Direction::None);
    _speed = 15;
    _score = 0;
    _lost = false;
}

void Snake::SetDirection(Direction l_dir) { _dir = l_dir; }

Direction Snake::GetDirection() { return _dir; }

int Snake::GetSpeed() { return _speed; }

sf::Vector2i Snake::GetPosition() {
    return (!_snakeBody.empty() ? _snakeBody.front().position : sf::Vector2i(1, 1));
}

void Snake::SetPosition(sf::Vector2i l_pos) {
    _snakeBody.front().position.x = l_pos.x;
    _snakeBody.front().position.y = l_pos.y;
}

int Snake::GetScore() { return _score; }

void Snake::IncreaseScore() { _score += 10; }

bool Snake::HasLost() { return _lost; }

void Snake::Lose() { _lost = true; }

void Snake::Extend() {
    if (_snakeBody.empty()) { return; }
    SnakeSegment &tail_head =
            _snakeBody[_snakeBody.size() - 1];
    if (_snakeBody.size() > 1) {
        SnakeSegment &tail_bone =
                _snakeBody[_snakeBody.size() - 2];
        if (tail_head.position.x == tail_bone.position.x) {
            if (tail_head.position.y > tail_bone.position.y) {
                _snakeBody.push_back(SnakeSegment(
                        tail_head.position.x, tail_head.position.y + 1));
            } else {
                _snakeBody.push_back(SnakeSegment(
                        tail_head.position.x, tail_head.position.y - 1));
            }
        } else if (tail_head.position.y == tail_bone.position.y) {
            if (tail_head.position.x > tail_bone.position.x) {
                _snakeBody.push_back(SnakeSegment(
                        tail_head.position.x + 1, tail_head.position.y));
            } else {
                _snakeBody.push_back(SnakeSegment(
                        tail_head.position.x - 1, tail_head.position.y));
            }
        }
    } else {
        if (_dir == Direction::Up) {
            _snakeBody.push_back(SnakeSegment(
                    tail_head.position.x, tail_head.position.y + 1));
        } else if (_dir == Direction::Down) {
            _snakeBody.push_back(SnakeSegment(
                    tail_head.position.x, tail_head.position.y - 1));
        } else if (_dir == Direction::Left) {
            _snakeBody.push_back(SnakeSegment(
                    tail_head.position.x + 1, tail_head.position.y));
        } else if (_dir == Direction::Right) {
            _snakeBody.push_back(SnakeSegment(
                    tail_head.position.x - 1, tail_head.position.y));
        }
    }
}

void Snake::Tick() {
    if (_snakeBody.empty()) { return; }
    if (_dir == Direction::None) { return; }
    Move();
    CheckCollision();
}

Direction Snake::GetPhysicalDirection()
{
    SnakeSegment& head = _snakeBody[0];
    SnakeSegment& shoulder = _snakeBody[1];

    if(head.position.x == shoulder.position.x)
        return (head.position.y > shoulder.position.y ? Direction::Down : Direction::Up );
    if(head.position.y == shoulder.position.y)
        return (head.position.x > shoulder.position.x ? Direction::Right : Direction::Left );

}

void Snake::Move() {
    for (int i = _snakeBody.size() - 1; i > 0; --i) {
        _snakeBody[i].position = _snakeBody[i - 1].position;
    }

    if (_dir == Direction::Left) {
        --_snakeBody[0].position.x;
    } else if (_dir == Direction::Right) {
        ++_snakeBody[0].position.x;
    } else if (_dir == Direction::Up) {
        --_snakeBody[0].position.y;
    } else if (_dir == Direction::Down) {
        ++_snakeBody[0].position.y;
    }

}

void Snake::CheckCollision() {
    if (_snakeBody.size() < 5) { return; }
    SnakeSegment &head = _snakeBody.front();
    for (auto itr = _snakeBody.begin() + 1; itr != _snakeBody.end(); ++itr) {
        if (itr->position == head.position) {
            Lose();
            break;
        }
    }
}

void Snake::Render(sf::RenderWindow &l_window) {
    if (_snakeBody.empty()) { return; }

    auto head = _snakeBody.begin();
    _bodyRect.setFillColor(sf::Color::Yellow);
    _bodyRect.setPosition(head->position.x * _size, head->position.y * _size);
    l_window.draw(_bodyRect);

    _bodyRect.setFillColor(sf::Color::Green);
    for (auto itr = _snakeBody.begin() + 1; itr != _snakeBody.end(); ++itr) {
        _bodyRect.setPosition(itr->position.x * _size, itr->position.y * _size);
        l_window.draw(_bodyRect);
    }
}

World::World(const sf::Vector2u &l_windSize) {
    _blockSize = 16;

    _windowSize = l_windSize;
    RespawnApple();
    _appleShape.setFillColor(sf::Color::Red);
    _appleShape.setRadius(_blockSize / 2);
}

World::~World() {}

void World::RespawnApple() {
    int maxX = (_windowSize.x / _blockSize);
    int maxY = (_windowSize.y / _blockSize);
    _item = sf::Vector2i(
            rand() % maxX, rand() % maxY);
    _appleShape.setPosition(
            _item.x * _blockSize,
            _item.y * _blockSize);
}

void World::Update(Snake &l_player) {
    if (l_player.GetPosition() == _item) {
        l_player.Extend();
        l_player.IncreaseScore();
        RespawnApple();
    }
    int gridSize_x = _windowSize.x / _blockSize;
    int gridSize_y = _windowSize.y / _blockSize;

    if (l_player.GetPosition().x == -1) {
        l_player.SetPosition(sf::Vector2i( gridSize_x-1, l_player.GetPosition().y));
    }
    if (l_player.GetPosition().y == -1) {
        l_player.SetPosition(sf::Vector2i(l_player.GetPosition().x, gridSize_y-1));
    }
    if (l_player.GetPosition().y == gridSize_y) {
        l_player.SetPosition(sf::Vector2i(l_player.GetPosition().x, 0));
    }
    if (l_player.GetPosition().x == gridSize_x) {
        l_player.SetPosition(sf::Vector2i(0, l_player.GetPosition().y));
    }
}

void World::Render(sf::RenderWindow &window) {
    window.draw(_appleShape);
}

int World::GetBlockSize() { return _blockSize; }

sf::Time Game::GetElapsed() { return _elapsed; }

void Game::RestartClock() { _elapsed += _clock.restart(); }

Textbox::Textbox() {Setup(5,9,200,sf::Vector2f(0,0));}
Textbox::Textbox(int l_visible, int l_charSize, int l_width, sf::Vector2f l_screenPos) {
    Setup(l_visible,l_charSize,l_width,l_screenPos);
}
Textbox::~Textbox(){Clear();}

void Textbox::Setup(int l_visible, int l_charSize, int l_width, sf::Vector2f l_screenPos) {
    _numVisible = l_visible;

    sf::Vector2f offset(2.0f,2.0f);

    _font.loadFromFile("Textures/arial.ttf");
    _content.setFont(_font);
    _content.setString("");
    _content.setCharacterSize(l_charSize);
    _content.setColor(sf::Color::White);
    _content.setPosition(l_screenPos+offset);

    _backdrop.setSize(sf::Vector2f(l_width,(l_visible*(l_charSize*1.2f))));
    _backdrop.setFillColor(sf::Color(90,90,90,90));
    _backdrop.setPosition(l_screenPos);
}

void Textbox::Add(std::string l_message) {
    _messages.push_back(l_message);
    if(_messages.size()<2){return; }
    _messages.erase(_messages.begin());
}

void Textbox::Clear() {
    _messages.clear();
}

void Textbox::Render(sf::RenderWindow& l_wind){
    std::string l_content;
    for(auto &itr : _messages){
        l_content.append(itr+"\n");
    }
    if(l_content != "") {
        _content.setString(l_content);
        l_wind.draw(_backdrop);
        l_wind.draw(_content);
    }
}

//рахобраться с временем