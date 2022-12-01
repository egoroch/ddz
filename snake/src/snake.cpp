
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
    if (!font.loadFromFile("Textures/arial.ttf")) {
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
    window.GetRendWindow()->clear();
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
        if(this->_isSettings) {
            if(e.type == sf::Event::MouseButtonPressed)
                window.setState(new Options(&window));
        }
    }
};

json MainMenu::getConfig(const std::string &fileName) {
    std::ifstream inpFile(fileName);
    if(!inpFile){
        //возвращает default значения
        return 1;
    }

    json config = json::parse(inpFile);
}

void Options::render(Window &window) {
    window.GetRendWindow()->clear();
    sf::Font font;
    if (!font.loadFromFile("Textures/arial.ttf")) {
        return;
    }
    _save->setFont(font);
    _back->setFont(font);

    _save->setBackColor(sf::Color::Blue);
    _save->setTextColor(sf::Color::Yellow);
    _back->setBackColor(sf::Color::Blue);
    _back->setTextColor(sf::Color::Yellow);

    _save->setPosition({3.0f*window.GetWindowSize().x/4.0f, 7.0f*window.GetWindowSize().y/8.0f});
    _back->setPosition({window.GetWindowSize().x/4.0f, 7.0f*window.GetWindowSize().y/8.0f});
    this->_isSave = this->_save->isMouseOver(*_window->GetRendWindow());
    this->_isBack = this->_back->isMouseOver(*_window->GetRendWindow());
    if(_isSave){
        _save->setBackColor(sf::Color::Red);
        _save->setTextColor(sf::Color::Green);
    }
    if(_isBack){
        _back->setBackColor(sf::Color::Red);
        _back->setTextColor(sf::Color::Green);
    }

    _save->drawTo(*_window->GetRendWindow());
    _back->drawTo(*_window->GetRendWindow());
    _window->GetRendWindow()->display();

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
        if(this->_isSave) {
            if(e.type == sf::Event::MouseButtonPressed)
                //отправить данные в конфиг
                window.setState(new MainMenu(&window));
        }
        if(this->_isBack) {
            if(e.type == sf::Event::MouseButtonPressed)
                window.setState(new MainMenu(&window));
        }
    };
}

Game::Game() {
    _window = nullptr;
    _snake = Snake(0);
    SnakeBot bot1 = SnakeBot(0);
    _bots.push_back(bot1);
    sf::Vector2u size(0, 0);
    _world = size;
    _text.Setup(1,30,350,sf::Vector2f(225,0));
}

SnakeBot& Game::getSnakeBot(int i){
    return _bots[i];
}

Game::Game(Window *window) {
    _window = window;
    _world = World(_window->GetWindowSize());
    _snake = Snake(_world.GetBlockSize());
    _text.Setup(1,30,_window->GetWindowSize().x,sf::Vector2f(0,_window->GetWindowSize().y-50));
    SnakeBot bot1 = SnakeBot(_world.GetBlockSize());
    _bots.push_back(bot1);
}

std::vector<sf::Vector2i> Game::get_game_items(){
    std::vector<sf::Vector2i> res;
    std::vector<sf::Vector2i>  fromWorld = _world.get_world_items();
    std::vector<sf::Vector2i>  fromSnake = _snake.getBodySnake();
    //   sizeof(arr)/sizeof(arr[0]);
    for(auto itr = fromWorld.begin();itr<fromWorld.end();++itr){
        res.push_back(*itr);
    }
    for(auto itr = fromSnake.begin();itr<fromSnake.end();++itr){
        res.push_back(*itr);
    }
    for (auto itr = _bots.begin();itr != _bots.end();++itr)
    {
        std::vector<sf::Vector2i>  fromBot = itr->getBodySnake();
        for(auto pointer = fromBot.begin();pointer != fromBot.end();++pointer){
            res.push_back(*pointer);
        }
    }
    return res;

}

Game::~Game() {
    _bots.clear();
    //delete _player;
}

void Game::render(Window &window) {
    window.GetRendWindow()->clear();
    _world.Render(*_window->GetRendWindow());
    _snake.Render(*_window->GetRendWindow());
    _bots[0].Render(*_window->GetRendWindow());
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
    float timestepMainSnake = 1.0f / _snake.GetSpeed();

    if (_elapsed.asSeconds() >= timestepMainSnake) {

        std::vector<sf::Vector2i> allItems = this->get_game_items();
        _snake.Tick(allItems);
        _bots[0].Tick(_world.getApplePosition(),allItems);

        //_bots[0].ChangeDirection(this->_world.getApplePosition());

        _world.Update(_snake,_bots);
        _elapsed -= sf::seconds(timestepMainSnake);
        if (_snake.HasLost()) {
            _snake.Reset();
        }
        if (_bots[0].HasLost()) {
             _bots[0].Disappear();
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
    _speed = 10;
    _score = 0;
    _lost = false;
}

std::vector<sf::Vector2i> Snake::getBodySnake(){
    std::vector<sf::Vector2i> res;
    for (auto itr = _snakeBody.begin() ; itr != _snakeBody.end(); ++itr)
    {
        res.push_back(sf::Vector2i(itr->position.x,itr->position.y));
    }
    return res;
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

void Snake::Tick(std::vector<sf::Vector2i> items) {
    if (_snakeBody.empty()) { return; }
    if (_dir == Direction::None) { return; }
    Move();
    CheckCollision(items);
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

void Snake::CheckCollision(std::vector<sf::Vector2i> items) {
    //if (_snakeBody.size() < 5) { return; }
     auto head = _snakeBody.begin();
    for (auto itr = items.begin() ; itr != items.end(); ++itr) {
        if (*itr == head->position) {
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

SnakeBot::SnakeBot(int l_blockSize){
    _size = l_blockSize;
    _bodyRect.setSize(sf::Vector2f(_size - 1, _size - 1));
    Reset();
}

SnakeBot::~SnakeBot() {}

void SnakeBot::Reset() {
    _snakeBody.clear();

    _snakeBody.push_back(SnakeSegment(14, 5));
    _snakeBody.push_back(SnakeSegment(14, 6));
    _snakeBody.push_back(SnakeSegment(14, 7));

    SetDirection(Direction::Down);
    _speed = 15;
    _lost = false;
}

void SnakeBot::SetDirection(Direction l_dir) { _dir = l_dir; }

std::vector<sf::Vector2i> SnakeBot::getBodySnake(){
    std::vector<sf::Vector2i> res;
    for (auto itr = _snakeBody.begin() ; itr != _snakeBody.end(); ++itr)
    {
        res.push_back(sf::Vector2i(itr->position.x,itr->position.y));
    }
    return res;
}

void SnakeBot::ChangeDirection(sf::Vector2i apple_position, std::vector<sf::Vector2i> items) {
    bool DangerLeft = false;
    bool DangerRight = false;
    bool DangerUp = false;
    bool DangerFalse = false;
   // for(auto itr = items.begin(); itr!=items.end();++itr )
    //{
      //  if(items == )
    //}
    int posX = this->GetPosition().x;
    int posY = this->GetPosition().y;
    int posAppleX = apple_position.x;
    int posAppleY = apple_position.y;
    double distance =0;
    double caseUp = abs(posX*posX-posAppleX*posAppleX) + abs((posY-1)*(posY-1)-posAppleY*posAppleY);
    double caseDown = abs(posX*posX-posAppleX*posAppleX) + abs((posY+1)*(posY+1)-posAppleY*posAppleY);
    double caseRight = abs((posX+1)*(posX+1)-posAppleX*posAppleX) + abs((posY)*(posY)-posAppleY*posAppleY);
    double caseLeft = abs((posX-1)*(posX-1)-posAppleX*posAppleX) + abs((posY)*(posY)-posAppleY*posAppleY);
    if (_dir == Direction::Left) {
        distance = caseLeft;
        if (caseLeft <= caseUp && caseLeft<=caseDown) {
        }
        if (caseUp <= caseLeft && caseUp<=caseDown) {
            this->SetDirection(Direction::Up);
        }
        if (caseDown <= caseLeft && caseDown<=caseUp) {
            this->SetDirection(Direction::Down);
        }

    } else if (_dir == Direction::Right) {
        if (caseRight<= caseUp && caseRight<=caseDown) {
        }
        if (caseUp <= caseRight && caseUp<=caseDown) {
            this->SetDirection(Direction::Up);
        }
        if (caseDown <= caseRight && caseDown<=caseUp) {
            this->SetDirection(Direction::Down);
        }
    } else if (_dir == Direction::Up) {
        if (caseUp<= caseRight && caseUp<=caseLeft) {
        }
        if (caseRight <= caseUp && caseRight<=caseLeft) {
            this->SetDirection(Direction::Right);
        }
        if (caseLeft <= caseRight && caseLeft<=caseUp) {
            this->SetDirection(Direction::Left);
        }

    } else if (_dir == Direction::Down) {
        if (caseDown<= caseRight && caseDown<=caseLeft) {
        }
        if (caseRight <= caseDown && caseRight<=caseLeft) {
            this->SetDirection(Direction::Right);
        }
        if (caseLeft <= caseRight && caseLeft<=caseDown) {
            this->SetDirection(Direction::Left);
        }
    }
}

int SnakeBot::GetSpeed() { return _speed; }

sf::Vector2i SnakeBot::GetPosition() {
    return (!_snakeBody.empty() ? _snakeBody.front().position : sf::Vector2i(1, 1));
}

void SnakeBot::SetPosition(sf::Vector2i l_pos) {
    _snakeBody.front().position.x = l_pos.x;
    _snakeBody.front().position.y = l_pos.y;
}

bool SnakeBot::HasLost() { return _lost; }

void SnakeBot::Lose() { _lost = true; }

void SnakeBot::Extend() {
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

void SnakeBot::Tick(sf::Vector2i apple_position, std::vector<sf::Vector2i> items) {
    if (_snakeBody.empty()) { return; }
    if (_dir == Direction::None) { return; }
    this->ChangeDirection(apple_position, items);
    Move(apple_position);
    CheckCollision(items);
}

void SnakeBot::Move(sf::Vector2i apple_position) {
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
void SnakeBot::Disappear(){
    _snakeBody.clear();
}

void SnakeBot::CheckCollision(std::vector<sf::Vector2i> items) {
    auto head = _snakeBody.begin();
    for (auto itr = items.begin() ; itr != items.end(); ++itr) {
        if (*itr == head->position) {
            Lose();
            break;
        }
    }
}

void SnakeBot::Render(sf::RenderWindow &l_window) {
    if (_snakeBody.empty()) { return; }

    auto head = _snakeBody.begin();
    _bodyRect.setFillColor(sf::Color::Yellow);
    _bodyRect.setPosition(head->position.x * _size, head->position.y * _size);
    l_window.draw(_bodyRect);

    _bodyRect.setFillColor(sf::Color::Yellow);
    for (auto itr = _snakeBody.begin() + 1; itr != _snakeBody.end(); ++itr) {
        _bodyRect.setPosition(itr->position.x * _size, itr->position.y * _size);
        l_window.draw(_bodyRect);
    }
}

World::World(const sf::Vector2u &l_windSize) {
    _blockSize = 16;
    _windowSize.y = l_windSize.y - 50;
    _windowSize.x = l_windSize.x ;
    RespawnApple();
    _appleShape.setFillColor(sf::Color::Red);
    _appleShape.setRadius(_blockSize / 2);
}

World::~World() {}


std::vector<sf::Vector2i> World::get_world_items(){
    std::vector<sf::Vector2i> result;
    result.push_back(sf::Vector2i(-1,-1));
    return result;
}

void World::RespawnApple() {
    int maxX = (_windowSize.x / _blockSize);
    int maxY = (_windowSize.y / _blockSize);
    _item = sf::Vector2i(
            rand() % maxX, rand() % maxY);
    _appleShape.setPosition(
            _item.x * _blockSize,
            _item.y * _blockSize);
}

sf::Vector2i World::getApplePosition(){
    return _item;
}

void World::Update(Snake &l_player,std::vector<SnakeBot> &bots) {
    if (l_player.GetPosition() == _item) {
        l_player.Extend();
        l_player.IncreaseScore();
        RespawnApple();
    }

    if (bots[0].GetPosition() == _item) {
        bots[0].Extend();
        RespawnApple();

    }
    //менять положение на нечетных клетках (ослабит бота)
    //bots[0].ChangeDirection(_item);
    int gridSize_x = _windowSize.x / _blockSize;
    int gridSize_y = _windowSize.y / _blockSize;

    if (bots[0].GetPosition().x == -1) {
        bots[0].SetPosition(sf::Vector2i( gridSize_x-1, bots[0].GetPosition().y));
    }
    if (bots[0].GetPosition().y == -1) {
        bots[0].SetPosition(sf::Vector2i(bots[0].GetPosition().x, gridSize_y-1));
    }
    if (bots[0].GetPosition().y == gridSize_y) {
        bots[0].SetPosition(sf::Vector2i(bots[0].GetPosition().x, 0));
    }
    if (bots[0].GetPosition().x == gridSize_x) {
        bots[0].SetPosition(sf::Vector2i(0, bots[0].GetPosition().y));
    }

    //_________________

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
    _content.setString("score::");
    _content.setCharacterSize(l_charSize);
    _content.setColor(sf::Color::White);
    _content.setPosition(l_screenPos+offset);

    _backdrop.setSize(sf::Vector2f(l_width,(l_visible*(l_charSize*1.2f))));
    _backdrop.setFillColor(sf::Color(90,90,90,90));
    _backdrop.setPosition(l_screenPos);
}

void Textbox::Add(std::string l_message) {
    l_message = "score: "+l_message;
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
        l_content.append(itr+" ");
    }
   // if(l_content != "") {
        _content.setString(l_content);
        l_wind.draw(_backdrop);
        l_wind.draw(_content);
    //}
}

//рахобраться с временем