
#include "../include/snake.h"

sf::Text makeText(std::string text, sf::Font &font, sf::Vector2f position, sf::Color color, int charSize) {
    sf::Text result(text, font, charSize);
    sf::FloatRect resultRect = result.getLocalBounds();
    result.setOrigin(resultRect.left + resultRect.width / 2.0f, resultRect.top + resultRect.height / 2.0f);
    result.setPosition(position);
    result.setFillColor(color);

    return result;
}

json getConfig(const std::string &fileName) {
    std::ifstream file(fileName);
    json config;
    if (!file) {
        config["default"] = {
                {"window_width",   1280},
                {"window_height",  720},
                {"button_width",   300},
                {"button_height",  100},
                {"character_size", 32}
        };
        return config;
    }

    file >> config;

    file.close();
    return config;
}

// constructors and destructor
Window::Window() {
    Setup("Snake", sf::Vector2u(1280, 720));
}

Window::Window(const std::string &title, const sf::Vector2u &size, json config, State *state) {
    Setup(title, size);
    _config = config;
    _currentConfig = _config["default"];
    _state = state;
    Create();
}

Window::~Window() {
    Destroy();
}

// functions
sf::Vector2u Window::GetWindowSize() {
    return _windowSize;
}

void Window::Destroy() {
    _rend_window.close();
    //delete _state;
}

void Window::ToggleFullScreen() {
    _isFullScreen = !_isFullScreen;
    if (_isFullScreen) {
        _currentConfig = _config["fullScreen"];
    } else {
        _currentConfig = _config["default"];
    }
    Destroy();
    this->_windowSize = {_currentConfig["window_width"], _currentConfig["window_height"]};
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
    _rend_window.create({_windowSize.x, _windowSize.y, 32}, _windowTitle, sf::Style::Default);
    this->GetRendWindow()->setFramerateLimit(30);
}

void State::setWindow(Window *cw) {
    _window = cw;
}

bool Window::IsFullScreen() { return _isFullScreen; }


void Window::setState(State *st) {
    if (_state != nullptr) _state = nullptr;
    _state = st;
    _state->setWindow(this);
}

void MainMenu::render(Window &window) {
    window.GetRendWindow()->clear();
    sf::Font font;
    if (!font.loadFromFile("./Textures/retro-land-mayhem.ttf")) {
        return;
    }
    sf::Texture texture;
    texture.loadFromFile("./picture/64/button.png");

    if (this->_isStart) {
        sf::Text rounds = makeText("Count of rounds", font,
                                   {_window->GetWindowSize().x / 4.0f, _window->GetWindowSize().y / 6.0f});
        sf::Text bots = makeText("Count of bots", font,
                                 {_window->GetWindowSize().x / 4.0f, 2.0f * _window->GetWindowSize().y / 6.0f});
        sf::Text user = makeText("Count of players", font,
                                 {_window->GetWindowSize().x / 4.0f, 3.0f * _window->GetWindowSize().y / 6.0f});
        _back->setFont(font);
        _back->setBackColor(sf::Color::Transparent);
        _back->setTextColor(sf::Color::Yellow);
        _back->setPosition({window.GetWindowSize().x / 4.0f, 7.0f * window.GetWindowSize().y / 8.0f});
        _back->setTexture(texture);
        _startGame->setFont(font);
        _startGame->setTexture(texture);
        _startGame->setBackColor(sf::Color::Transparent);
        _startGame->setTextColor(sf::Color::Yellow);
        _startGame->setPosition({3*window.GetWindowSize().x / 4.0f, 7.0f * window.GetWindowSize().y / 8.0f});

        if (_startGame->isMouseOver(*_window->GetRendWindow())) {
            _startGame->setBackColor(sf::Color::Red);
        }
        if (_back->isMouseOver(*_window->GetRendWindow())) {
            _back->setBackColor(sf::Color::Red);
        }

        _window->GetRendWindow()->draw(rounds);
        _window->GetRendWindow()->draw(bots);
        _window->GetRendWindow()->draw(user);
        _startGame->drawTo(*_window->GetRendWindow());
        _back->drawTo(*_window->GetRendWindow());
        _textField->draw(*window.GetRendWindow());
        _textFieldBots->draw(*window.GetRendWindow());
        _textUser->draw(*window.GetRendWindow());
        _window->GetRendWindow()->display();
    } else {
        _start->setFont(font);
        _settings->setFont(font);
        _exit->setFont(font);
        _title.setFont(font);

        _start->setTexture(texture);
        _settings->setTexture(texture);
        _exit->setTexture(texture);

        _start->setBackColor(sf::Color::Transparent);
        _start->setTextColor(sf::Color::Yellow);
        _settings->setBackColor(sf::Color::Transparent);
        _settings->setTextColor(sf::Color::Yellow);
        _exit->setBackColor(sf::Color::Transparent);
        _exit->setTextColor(sf::Color::Yellow);

        _start->setPosition({_window->GetWindowSize().x / 2.0f, _window->GetWindowSize().y / 2.0f});
        _settings->setPosition(
                {_window->GetWindowSize().x / 2.0f, _window->GetWindowSize().y / 2.0f + _start->getSize().y + 10});
        _exit->setPosition({_window->GetWindowSize().x / 2.0f,
                            _window->GetWindowSize().y / 2.0f + 2 * _settings->getSize().y + 20});
        sf::FloatRect titleRect = _title.getLocalBounds();
        _title.setOrigin({titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f});
        _title.setPosition(_window->GetWindowSize().x / 2, _window->GetWindowSize().y / 6);
        this->_isExit = _exit->isMouseOver(*this->_window->GetRendWindow());
        this->_isSettings = _settings->isMouseOver(*this->_window->GetRendWindow());
        if (_start->isMouseOver(*this->_window->GetRendWindow())) {
            _start->setBackColor(sf::Color::Red);
        }
        if (_isExit) {
            _exit->setBackColor(sf::Color::Red);
        }
        if (_isSettings) {
            _settings->setBackColor(sf::Color::Red);
        }
        //window.GetRendWindow()->draw(sprite);
        window.GetRendWindow()->draw(_title);
        _start->drawTo(*window.GetRendWindow());
        _settings->drawTo(*window.GetRendWindow());
        _exit->drawTo(*window.GetRendWindow());
        _window->GetRendWindow()->display();
    }
}

void MainMenu::update(Window &window) {
    window.GetRendWindow()->clear();
    int round = -1;
    int bot = -1;
    int user = -1;
    sf::Event e;
    while (window.GetRendWindow()->pollEvent(e)) {
        if (e.Event::type == sf::Event::Closed)
            window.GetRendWindow()->close();
        if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape) {
                window.setState(new Pause(&window, this, false));
        }
        if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::F5) {
            window.ToggleFullScreen();
            window.setState(new MainMenu(&window));
        }

        if (this->_isExit) {
            if (e.type == sf::Event::MouseButtonPressed)
                window.setState(new Pause(&window, this, false));
        }

        if (this->_start->isMouseOver(*window.GetRendWindow()) && e.type == sf::Event::MouseButtonPressed) {
            this->_isStart = true;
        }
        if (this->_startGame->isMouseOver(*window.GetRendWindow())) {
            if (e.type == sf::Event::MouseButtonPressed) {
                std::cout << _textField->getString() << " " << _textFieldBots->getString();
                if (_textField->getString().size() == 0 || _textFieldBots->getString().size() == 0 ||
                    _textUser->getString().size() == 0) {
                    continue;
                } else {
                    round = std::stoi(_textField->getString());
                    bot = std::stoi(_textFieldBots->getString());
                    user = std::stoi(_textUser->getString());
                    bool multyfruct = user > 1;
                    window.setState(new Game(&window, bot, round, multyfruct));
                }
            }
        }
        if (e.type == sf::Event::MouseButtonReleased) {
            _textField->released(*window.GetRendWindow(), e);
            _textFieldBots->released(*window.GetRendWindow(), e);
            _textUser->released(*window.GetRendWindow(), e);
        } else {
            _textField->handleInput(e);
            _textFieldBots->handleInput(e);
            _textUser->handleInput(e);
        }
        if (this->_isSettings) {
            if (e.type == sf::Event::MouseButtonPressed)
                window.setState(new Options(&window));
        }
        if(this->_back->isMouseOver(*_window->GetRendWindow()) && e.type == sf::Event::MouseButtonPressed)
            window.setState(new MainMenu(&window));
    }
}

void Options::render(Window &window) {
    window.GetRendWindow()->clear();
    sf::Font font;
    if (!font.loadFromFile("Textures/retro-land-mayhem.ttf")) {
        return;
    }
    sf::Texture texture;
    texture.loadFromFile("./picture/64/button.png");

    _save->setFont(font);
    _save->setTexture(texture);
    _back->setFont(font);
    _back->setTexture(texture);
    sf::Text name = makeText("Input name", font, {window.GetWindowSize().x / 4.0f, window.GetWindowSize().y / 6.0f});
    sf::Text color = makeText("Input color", font, {window.GetWindowSize().x / 4.0f, 2*window.GetWindowSize().y / 6.0f});

    _save->setBackColor(sf::Color::Transparent);
    _save->setTextColor(sf::Color::Yellow);
    _back->setBackColor(sf::Color::Transparent);
    _back->setTextColor(sf::Color::Yellow);

    for(size_t i = 0; i < 5; ++i){
        _color[i].setPosition({(7 + i)*window.GetWindowSize().x/12.0f, 2*window.GetWindowSize().y/6.0f});
    }

    _save->setPosition({3.0f * window.GetWindowSize().x / 4.0f, 7.0f * window.GetWindowSize().y / 8.0f});
    _back->setPosition({window.GetWindowSize().x / 4.0f, 7.0f * window.GetWindowSize().y / 8.0f});
    this->_isSave = this->_save->isMouseOver(*_window->GetRendWindow());
    this->_isBack = this->_back->isMouseOver(*_window->GetRendWindow());
    if (_isSave) {
        _save->setBackColor(sf::Color::Red);
    }
    if (_isBack) {
        _back->setBackColor(sf::Color::Red);
    }

    _save->drawTo(*_window->GetRendWindow());
    _back->drawTo(*_window->GetRendWindow());
    _name->draw(*_window->GetRendWindow());
    for(size_t i = 0; i < 5; ++i){
        _window->GetRendWindow()->draw(_color[i]);
    }
    _window->GetRendWindow()->draw(name);
    _window->GetRendWindow()->draw(color);
    _window->GetRendWindow()->display();

}

void Options::update(Window &window) {
    window.GetRendWindow()->clear();
    sf::Color currentColor;
    //render stuff
    sf::Event e;
    while (window.GetRendWindow()->pollEvent(e)) {
        if (e.Event::type == sf::Event::Closed)
            window.GetRendWindow()->close();
        if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
            window.setState(new Pause(&window, this, false));
        if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::F5)
            window.ToggleFullScreen();
        if (e.type == sf::Event::MouseButtonReleased) {
            _name->released(*window.GetRendWindow(), e);
        } else {
            _name->handleInput(e);
        }
        if (this->_isBack) {
            if (e.type == sf::Event::MouseButtonPressed)
                window.setState(new MainMenu(&window));
        }
        for(size_t i = 0 ; i < 5; ++i) {
            if(this->isMouseOver(*_window->GetRendWindow(), _color[i]))
             {
                _hasFocus = false;
                 if(e.type == sf::Event::MouseButtonReleased) {
                    _hasFocus = true;
                    currentColor = _color[i].getFillColor();
                }
                _color[i].setOutlineColor(sf::Color::Cyan);
                if(!_hasFocus) {
                    _color[i].setOutlineColor(sf::Color(127, 127, 127));
                }
            }
        }
        if (this->_isSave) {
            if (e.type == sf::Event::MouseButtonPressed) {
                window.setState(new MainMenu(&window));
                if(empty(this->_name->getString())){
                    continue;
                } else {
                    std::cout << this->_name->getString();
                    std::cout << currentColor.toInteger();
                }
            }
        }
    }
}

Game::Game() {
    _window = nullptr;
    _snake = Snake(0, false);
    SnakeBot bot1 = SnakeBot(0, sf::Vector2i(-1, -1));
    _bots.push_back(bot1);
    sf::Vector2u size(0, 0);
    _countOfBots =0;
    _world = size;
    _text.Setup(1, 30, 350, sf::Vector2f(225, 0));
}

void Pause::render(Window &window) {
    window.GetRendWindow()->clear(sf::Color::Transparent);
    sf::Font font;
    if (!font.loadFromFile("Textures/retro-land-mayhem.ttf")) {
        return;
    }

    _title.setFont(font);
    sf::FloatRect titleRect = _title.getLocalBounds();
    _title.setOrigin({titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f});
    _title.setPosition(_window->GetWindowSize().x/2.0f, _window->GetWindowSize().y/4.0f);

    json config = _window->getConfig();
    sf::Texture texture;
    texture.loadFromFile("./picture/64/button.png");

    if(_isPause){
        _title.setString("Pause");
        _return->setFont(font);
        _return->setTexture(texture);
        _restart->setFont(font);
        _restart->setTexture(texture);
        _continue->setFont(font);
        _continue->setTexture(texture);

        _return->setBackColor(sf::Color::Transparent);
        _return->setTextColor(sf::Color::Yellow);
        _restart->setBackColor(sf::Color::Transparent);
        _restart->setTextColor(sf::Color::Yellow);
        _continue->setBackColor(sf::Color::Transparent);
        _continue->setTextColor(sf::Color::Yellow);
        _return->setPosition({_window->GetWindowSize().x/2.0f, 2.5f*_window->GetWindowSize().y/6.0f + 2*_restart->getSize().y });
        _restart->setPosition({_window->GetWindowSize().x/2.0f, 2.5f*_window->GetWindowSize().y/6.0f + _continue->getSize().y });
        _continue->setPosition({_window->GetWindowSize().x/2.0f, 2.5f*_window->GetWindowSize().y/6.0f});

        if(this->_continue->isMouseOver(*_window->GetRendWindow())){
            _continue->setBackColor(sf::Color::Red);
        }

        if(this->_return->isMouseOver(*_window->GetRendWindow())){
            _return->setBackColor(sf::Color::Red);
        }
        if(this->_restart->isMouseOver(*_window->GetRendWindow())) {
            _restart->setBackColor(sf::Color::Red);
        }
        _return->drawTo(*_window->GetRendWindow());
        _restart->drawTo(*_window->GetRendWindow());
        _continue->drawTo(*_window->GetRendWindow());
    } else {
        _title.setString("Do you want to quit the game?");
        _exit->setFont(font);
        _exit->setTexture(texture);
        _back->setFont(font);
        _back->setTexture(texture);
        _back->setBackColor(sf::Color::Transparent);
        _back->setTextColor(sf::Color::Yellow);
        _back->setPosition({_window->GetWindowSize().x/2.0f, 3*_window->GetWindowSize().y/6.0f});
        _exit->setBackColor(sf::Color::Transparent);
        _exit->setTextColor(sf::Color::Yellow);
        _exit->setPosition({_window->GetWindowSize().x/2.0f, 3*_window->GetWindowSize().y/6.0f + _back->getSize().y});
        if(this->_exit->isMouseOver(*_window->GetRendWindow())){
            _exit->setBackColor(sf::Color::Red);
        }
        if(this->_back->isMouseOver(*_window->GetRendWindow())){
            _back->setBackColor(sf::Color::Red);
        }
        _exit->drawTo(*_window->GetRendWindow());
        _back->drawTo(*_window->GetRendWindow());

    }

    window.GetRendWindow()->draw(_title);
    window.GetRendWindow()->display();
}

void Pause::update(Window& window){
    window.GetRendWindow()->clear();
    sf::Event event;
    while (window.GetRendWindow()->pollEvent(event)) {
        if (event.Event::type == sf::Event::Closed)
            window.GetRendWindow()->close();
        if (this->_return->isMouseOver(*_window->GetRendWindow()) && event.type == sf::Event::MouseButtonPressed)
            _window->setState(new MainMenu(_window));
        if ((this->_continue->isMouseOver(*_window->GetRendWindow()) && event.type == sf::Event::MouseButtonPressed) ||
                (this->_back->isMouseOver(*_window->GetRendWindow()) && event.type == sf::Event::MouseButtonPressed))
            _window->setState(_previous);
        if (this->_restart->isMouseOver(*_window->GetRendWindow()) && event.type == sf::Event::MouseButtonPressed)
            _window->setState(_previous);
        if (this->_exit->isMouseOver(*_window->GetRendWindow()) && event.type == sf::Event::MouseButtonPressed)
            _window->GetRendWindow()->close();
    };
}

SnakeBot &Game::getSnakeBot(int i) {
    return _bots[i];
}

Game::Game(Window *window, int countOfBots, int rounds, bool is_multiplay) {
    _rounds = rounds;
    _window = window;
    _world = World(_window->GetWindowSize());
    _snake = Snake(_world.GetBlockSize(),false);
    _firstRounds =0;
    _secondRounds =0;
    _countOfBots =countOfBots;

    std::vector<sf::Vector2i> res;
    std::vector<sf::Vector2i> fromWorld = _world.get_world_items();
    std::vector<sf::Vector2i> fromSnake = _snake.getBodySnake();
    for (auto itr = fromWorld.begin(); itr < fromWorld.end(); ++itr) {
        res.push_back(*itr);
    }
    for (auto itr = fromSnake.begin(); itr < fromSnake.end(); ++itr) {
        res.push_back(*itr);
    }
    _is_multiplayer = is_multiplay;
    if (_is_multiplayer) {
        _player2_snake = Snake(_world.GetBlockSize(), is_multiplay);
        std::vector<sf::Vector2i> fromSecondSnake = _player2_snake.getBodySnake();
        for (auto itr = fromSecondSnake.begin(); itr < fromSecondSnake.end(); ++itr) {
            res.push_back(*itr);
        }
    }else _player2_snake =Snake(0,true);

    _text.Setup(1, 30, _window->GetWindowSize().x, sf::Vector2f(0, _window->GetWindowSize().y - 50));
    this->CreateAllBots(window,_world.GetBlockSize(),res,countOfBots);

}


std::vector<sf::Vector2i> Game::get_game_items() {
    std::vector<sf::Vector2i> res;
    std::vector<sf::Vector2i> fromWorld = _world.get_world_items();
    std::vector<sf::Vector2i> fromSnake = _snake.getBodySnake();

    //   sizeof(arr)/sizeof(arr[0]);
    for (auto itr = fromWorld.begin(); itr < fromWorld.end(); ++itr) {
        res.push_back(*itr);
    }
    for (auto itr = fromSnake.begin(); itr < fromSnake.end(); ++itr) {
        res.push_back(*itr);
    }
    if (_is_multiplayer) {
        std::vector<sf::Vector2i> fromSecondSnake = _player2_snake.getBodySnake();
        for (auto itr = fromSecondSnake.begin(); itr < fromSecondSnake.end(); ++itr) {
            res.push_back(*itr);
        }
    }

    for (auto itr = _bots.begin(); itr != _bots.end(); ++itr) {
        std::vector<sf::Vector2i> fromBot = itr->getBodySnake();
        for (auto pointer = fromBot.begin(); pointer != fromBot.end(); ++pointer) {
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
    if (_is_multiplayer)
        _player2_snake.Render(*_window->GetRendWindow());
    for (auto itr = _bots.begin(); itr != _bots.end(); ++itr)
        itr->Render(*_window->GetRendWindow());
    _text.Add(std::to_string(_firstRounds));
    _text.Render(*_window->GetRendWindow());
    _window->GetRendWindow()->display();
    this->RestartClock();
    // _player->move();t
    //_player->render(*(_window->GetRendWindow()));
}

void Game::update(Window &window) {
    sf::Event event;
    while (window.GetRendWindow()->pollEvent(event)) {
        if (event.Event::type == sf::Event::Closed)
            _window->GetRendWindow()->close();
        if (event.Event::KeyPressed && event.Event::key.code == sf::Keyboard::Escape)
            window.setState(new Pause(&window, this, true));
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
        if (_is_multiplayer) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)
                && _player2_snake.GetPhysicalDirection() != Direction::Down) {
                _player2_snake.SetDirection(Direction::Up);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)
                       && _player2_snake.GetPhysicalDirection() != Direction::Up) {
                _player2_snake.SetDirection(Direction::Down);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)
                       && _player2_snake.GetPhysicalDirection() != Direction::Right) {
                _player2_snake.SetDirection(Direction::Left);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)
                       && _player2_snake.GetPhysicalDirection() != Direction::Left) {
                _player2_snake.SetDirection(Direction::Right);
            }
        }
    }
    float timestepMainSnake = 1.0f / _snake.GetSpeed();

    if (_elapsed.asSeconds() >= timestepMainSnake) {


        _snake.Tick();
        if (_is_multiplayer){
            _player2_snake.Tick();}
        std::vector<sf::Vector2i> allItems = this->get_game_items();
        for (auto itr = _bots.begin() + 1; itr != _bots.end(); ++itr)
            itr->Tick(_world.getApplePosition(), allItems, _snake.GetDirection());
        _bots.begin()->Tick(sf::Vector2i(_snake.GetPosition().x + 2, _snake.GetPosition().y), allItems,
                            _snake.GetDirection());
        allItems.clear();
        allItems = this->get_game_items();

        _snake.CheckCollision(allItems);
        if (_is_multiplayer) {
            _player2_snake.CheckCollision(allItems);
        }
            for (auto itr = _bots.begin(); itr != _bots.end(); ++itr){
                itr->CheckCollision(allItems);}


        //_bots[0].ChangeDirection(this->_world.getApplePostition());

        _world.Update(_snake, _player2_snake, _bots, allItems);
        _elapsed -= sf::seconds(timestepMainSnake);

        if (_is_multiplayer) {
            /**/

            for (auto itr = _bots.begin(); itr != _bots.end(); ++itr)
                if (itr->HasLost()) {
                    itr->Disappear();
                }

                if (_snake.HasLost() || _player2_snake.HasLost()) {
                    if (_snake.HasLost()) _secondRounds++;
                    if (_player2_snake.HasLost()) _firstRounds++;

                    _snake.Reset(false);
                    _player2_snake.Reset(true);

                    allItems.clear();
                    allItems = this->get_game_items();
                    std::cout << "прошло цикл с мультиплеем"<<'\n';
                    CreateAllBots(_window, _world.GetBlockSize(), allItems, _countOfBots);
                    std::cout << "не лагает"<<'\n';
                    //_world.Update(_snake, _player2_snake, _bots, allItems);


                }
            } else {
                if (_snake.HasLost()) {
                    _snake.Reset(false);
                }

            for (auto itr = _bots.begin(); itr != _bots.end(); ++itr)
                if (itr->HasLost()) {
                    itr->Disappear();
                }
        }
    }
    this->RestartClock();
};

void Game::CreateAllBots(Window *window, int blockSIze, std::vector<sf::Vector2i> items, int count) {
    _bots.clear();
    std::vector<SnakeBot> res;
    int maxX = (window->GetWindowSize().x / blockSIze);
    int maxY = (window->GetWindowSize().y / blockSIze);
    sf::Vector2i head;
    if(count == 0){
        head = sf::Vector2i (-2,-2);
        res.push_back(SnakeBot(blockSIze, head));
        return ;
    }
    //head = sf::Vector2i(rand() % maxX, rand() % (maxY-3));

    for (int i = 0; i < count; ++i) {
        int Check = -1;
        while (Check != 0) {
            Check = 0; //check++
            head = sf::Vector2i(rand() % maxX, (rand() % (maxY - 3)) + 3);

            for (auto itr = items.begin(); itr != items.end(); ++itr) {
                if ((itr->x == head.x) && ((itr->y == head.y) || (itr->y == (head.y - 1)) || (itr->y == head.y - 2))) {
                    Check++;
                }

            }
        }
        items.push_back(sf::Vector2i(head.x, head.y));
        items.push_back(sf::Vector2i(head.x, head.y - 1));
        items.push_back(sf::Vector2i(head.x, head.y - 2));

        res.push_back(SnakeBot(blockSIze, head));
    }
    _bots = res;
    return;
}


Snake::Snake(int l_blockSize, bool is_multy) {
    if (l_blockSize == 0) {
        _snakeBody.clear();
        return;
    }
    _size = l_blockSize;
    //_bodyRect.setSize(sf::Vector2f(_size - 1, _size - 1));
    Reset(is_multy);
}

Snake::~Snake() {}

void Snake::Reset(bool is_multy) {
    _snakeBody.clear();
    if (!is_multy) {
        _snakeBody.push_back(SnakeSegment(5, 7));
        _snakeBody.push_back(SnakeSegment(5, 6));
        _snakeBody.push_back(SnakeSegment(5, 5));
    } else {
        _snakeBody.push_back(SnakeSegment(15, 7));
        _snakeBody.push_back(SnakeSegment(15, 6));
        _snakeBody.push_back(SnakeSegment(15, 5));
    }

    SetDirection(Direction::None);
    _speed = 10;
    _score = 0;
    _lost = false;
}

void Snake::Disapear() {
    _snakeBody.clear();
};

std::vector<sf::Vector2i> Snake::getBodySnake() {
    std::vector<sf::Vector2i> res;
    for (auto itr = _snakeBody.begin(); itr != _snakeBody.end(); ++itr) {
        res.push_back(sf::Vector2i(itr->position.x, itr->position.y));
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

void Snake::Tick() {
    if (_snakeBody.empty()) { return; }
    if (_dir == Direction::None) { return; }
    Move();
    // CheckCollision(items);
}

Direction Snake::GetPhysicalDirection() {
    SnakeSegment &head = _snakeBody[0];
    SnakeSegment &shoulder = _snakeBody[1];

    if (head.position.x == shoulder.position.x)
        return (head.position.y > shoulder.position.y ? Direction::Down : Direction::Up);
    if (head.position.y == shoulder.position.y)
        return (head.position.x > shoulder.position.x ? Direction::Right : Direction::Left);
    return Direction::None;
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
    int CountMatches = 0;
    for (auto itr = items.begin(); itr != items.end(); ++itr) {
        if (*itr == head->position) {
            CountMatches++;
        }
        if (2 == CountMatches) {
            Lose();
            std::cout<<"snake die"<<'\n';
            break;
        }
    }
}

void Snake::Render(sf::RenderWindow &l_window) {
    if (_snakeBody.empty()) { return; }

    sf::Texture body;
    body.loadFromFile("./picture/body.png");

    sf::Texture header;
    header.loadFromFile("./picture/head.png");

    auto head = _snakeBody.begin();
    _bodyRect.setTexture(header);
    _bodyRect.setPosition(head->position.x * _size, head->position.y * _size);
    l_window.draw(_bodyRect);


    _bodyRect.setTexture(body);
    for (auto itr = _snakeBody.begin() + 1; itr != _snakeBody.end(); ++itr) {
        _bodyRect.setPosition(itr->position.x * _size, itr->position.y * _size);
        l_window.draw(_bodyRect);
    }
}

SnakeBot::SnakeBot(int l_blockSize, sf::Vector2i headPos) {
    if(l_blockSize==0) {
        _snakeBody.clear();
       return;
    }
    _size = l_blockSize;
    _bodyRect.setSize(sf::Vector2f(_size - 1, _size - 1));
    Reset(headPos);
}

SnakeBot::~SnakeBot() {}

void SnakeBot::Reset(sf::Vector2i headPos) {
    _snakeBody.clear();

    _snakeBody.push_back(SnakeSegment(headPos.x, headPos.y - 2));
    _snakeBody.push_back(SnakeSegment(headPos.x, headPos.y - 1));
    _snakeBody.push_back(SnakeSegment(headPos.x, headPos.y));

    SetDirection(Direction::Down);
    _speed = 15;
    _lost = false;
}

void SnakeBot::SetDirection(Direction l_dir) { _dir = l_dir; }

std::vector<sf::Vector2i> SnakeBot::getBodySnake() {
    std::vector<sf::Vector2i> res;
    for (auto itr = _snakeBody.begin(); itr != _snakeBody.end(); ++itr) {
        res.push_back(sf::Vector2i(itr->position.x, itr->position.y));
    }
    return res;
}

void SnakeBot::ChangeDirection(sf::Vector2i apple_position, std::vector<sf::Vector2i> items, Direction player_dir) {
    bool DangerLeft = false;
    bool DangerRight = false;
    bool DangerUp = false;
    bool DangerDown = false;
    for (auto itr = items.begin(); itr != items.end(); ++itr) {
        if (*itr == sf::Vector2i(GetPosition().x + 1, GetPosition().y)) {
            DangerRight = true;
        }
        if (*itr == sf::Vector2i(GetPosition().x - 1, GetPosition().y)) {
            DangerLeft = true;
        }
        if (*itr == sf::Vector2i(GetPosition().x, GetPosition().y + 1)) {
            DangerDown = true;
        }
        if (*itr == sf::Vector2i(GetPosition().x, GetPosition().y - 1)) {
            DangerUp = true;
        }
    }
    int posX = this->GetPosition().x;
    int posY = this->GetPosition().y;
    int posAppleX = apple_position.x;
    int posAppleY = apple_position.y;
    double distance = 0;
    double caseUp = abs(posX * posX - posAppleX * posAppleX) + abs((posY - 1) * (posY - 1) - posAppleY * posAppleY);
    double caseDown = abs(posX * posX - posAppleX * posAppleX) + abs((posY + 1) * (posY + 1) - posAppleY * posAppleY);
    double caseRight =
            abs((posX + 1) * (posX + 1) - posAppleX * posAppleX) + abs((posY) * (posY) - posAppleY * posAppleY);
    double caseLeft =
            abs((posX - 1) * (posX - 1) - posAppleX * posAppleX) + abs((posY) * (posY) - posAppleY * posAppleY);
    if (_dir == Direction::Left) {
        if (caseLeft <= caseUp && caseLeft <= caseDown) {
            this->SetDirection(Direction::Left);
            if (DangerLeft) {
                if (caseUp <= caseDown) {
                    this->SetDirection(Direction::Up);
                    if (DangerUp) { this->SetDirection(Direction::Down); }
                }
                if (caseDown <= caseUp) {
                    this->SetDirection(Direction::Down);
                    if (DangerDown) { this->SetDirection(Direction::Up); }
                }
            }
        }

        if (caseUp <= caseLeft && caseUp <= caseDown) {
            this->SetDirection(Direction::Up);
            if (DangerUp) {
                if (caseLeft <= caseDown) {
                    this->SetDirection(Direction::Left);
                    if (DangerLeft) { this->SetDirection(Direction::Down); }
                }
                if (caseDown <= caseLeft) {
                    this->SetDirection(Direction::Down);
                    if (DangerDown) { this->SetDirection(Direction::Left); }
                }
            }

        }

        if (caseDown <= caseLeft && caseDown <= caseUp) {
            this->SetDirection(Direction::Down);
            if (DangerDown) {
                if (caseLeft <= caseUp) {
                    this->SetDirection(Direction::Left);
                    if (DangerLeft) { this->SetDirection(Direction::Up); }
                }
                if (caseUp <= caseLeft) {
                    this->SetDirection(Direction::Up);
                    if (DangerUp) { this->SetDirection(Direction::Left); }
                }
            }
        }

    } else if (_dir == Direction::Right) {
        if (caseRight <= caseUp && caseRight <= caseDown) {
            this->SetDirection(Direction::Right);
            if (DangerRight) {
                if (caseUp <= caseDown) {
                    this->SetDirection(Direction::Up);
                    if (DangerUp) { this->SetDirection(Direction::Down); }
                }
                if (caseDown <= caseUp) {
                    this->SetDirection(Direction::Down);
                    if (DangerDown) { this->SetDirection(Direction::Up); }
                }
            }
        }

        if (caseUp <= caseRight && caseUp <= caseDown) {
            this->SetDirection(Direction::Up);
            if (DangerUp) {
                if (caseRight <= caseDown) {
                    this->SetDirection(Direction::Right);
                    if (DangerRight) { this->SetDirection(Direction::Down); }
                }
                if (caseDown <= caseRight) {
                    this->SetDirection(Direction::Down);
                    if (DangerDown) { this->SetDirection(Direction::Right); }
                }
            }

        }

        if (caseDown <= caseRight && caseDown <= caseUp) {
            this->SetDirection(Direction::Down);
            if (DangerDown) {
                if (caseRight <= caseUp) {
                    this->SetDirection(Direction::Right);
                    if (DangerRight) { this->SetDirection(Direction::Up); }
                }
                if (caseUp <= caseRight) {
                    this->SetDirection(Direction::Up);
                    if (DangerUp) { this->SetDirection(Direction::Right); }
                }
            }
        }
    } else if (_dir == Direction::Up) {
        if (caseRight <= caseUp && caseRight <= caseLeft) {
            this->SetDirection(Direction::Right);
            if (DangerRight) {
                if (caseUp <= caseLeft) {
                    this->SetDirection(Direction::Up);
                    if (DangerUp) { this->SetDirection(Direction::Left); }
                }
                if (caseLeft <= caseUp) {
                    this->SetDirection(Direction::Left);
                    if (DangerLeft) { this->SetDirection(Direction::Up); }
                }
            }
        }

        if (caseUp <= caseRight && caseUp <= caseLeft) {
            this->SetDirection(Direction::Up);
            if (DangerUp) {
                if (caseRight <= caseLeft) {
                    this->SetDirection(Direction::Right);
                    if (DangerRight) { this->SetDirection(Direction::Left); }
                }
                if (caseLeft <= caseRight) {
                    this->SetDirection(Direction::Left);
                    if (DangerLeft) { this->SetDirection(Direction::Right); }
                }
            }

        }

        if (caseLeft <= caseRight && caseLeft <= caseUp) {
            this->SetDirection(Direction::Left);
            if (DangerLeft) {
                if (caseRight <= caseUp) {
                    this->SetDirection(Direction::Right);
                    if (DangerRight) { this->SetDirection(Direction::Up); }
                }
                if (caseUp <= caseRight) {
                    this->SetDirection(Direction::Up);
                    if (DangerUp) { this->SetDirection(Direction::Right); }
                }
            }
        }
    } else if (_dir == Direction::Down) {
        if (caseRight <= caseDown && caseRight <= caseLeft) {
            this->SetDirection(Direction::Right);
            if (DangerRight) {
                if (caseDown <= caseLeft) {
                    this->SetDirection(Direction::Down);
                    if (DangerDown) { this->SetDirection(Direction::Left); }
                }
                if (caseLeft <= caseDown) {
                    this->SetDirection(Direction::Left);
                    if (DangerLeft) { this->SetDirection(Direction::Down); }
                }
            }
        }

        if (caseDown <= caseRight && caseDown <= caseLeft) {
            this->SetDirection(Direction::Down);
            if (DangerDown) {
                if (caseRight <= caseLeft) {
                    this->SetDirection(Direction::Right);
                    if (DangerRight) { this->SetDirection(Direction::Left); }
                }
                if (caseLeft <= caseRight) {
                    this->SetDirection(Direction::Left);
                    if (DangerLeft) { this->SetDirection(Direction::Right); }
                }
            }

        }

        if (caseLeft <= caseRight && caseLeft <= caseDown) {
            this->SetDirection(Direction::Left);
            if (DangerLeft) {
                if (caseRight <= caseDown) {
                    this->SetDirection(Direction::Right);
                    if (DangerRight) { this->SetDirection(Direction::Down); }
                }
                if (caseDown <= caseRight) {
                    this->SetDirection(Direction::Down);
                    if (DangerDown) { this->SetDirection(Direction::Right); }
                }
            }
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

void SnakeBot::Tick(sf::Vector2i apple_position, std::vector<sf::Vector2i> items, Direction player_dir) {
    if (_snakeBody.empty()) { return; }
    if (player_dir == Direction::None) {
        return;
    }
    this->ChangeDirection(apple_position, items, player_dir);
    Move(apple_position);
    //CheckCollision(items);//CheckCollision(items);
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

void SnakeBot::Disappear() {
    _snakeBody.clear();
}

void SnakeBot::CheckCollision(std::vector<sf::Vector2i> items) {

    auto head = _snakeBody.begin();
    int CountMatches = 0;
    for (auto itr = items.begin(); itr != items.end(); ++itr) {
        if (*itr == head->position) {
            CountMatches++;
        }
        if (CountMatches == 2) {
            Lose();
            std::cout<<"bot die"<<'\n';
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
    _blockSize = 32;
    _windowSize.y = l_windSize.y - 50;
    _windowSize.x = l_windSize.x;
    RespawnApple();
    //_______________________
    std::vector<sf::Vector2i> stonesPos;
    std::vector<sf::RectangleShape> stones;
    srand(unsigned(time(0)));
    int countOfStones = 5 + (rand() % 7);
    int maxX = (_windowSize.x / _blockSize);
    int maxY = (_windowSize.y / _blockSize);
    for (int i = 0; i < countOfStones; ++i) {
        stonesPos.push_back(sf::Vector2i(rand() % maxX, rand() % maxY));
        stones.push_back(sf::RectangleShape());
        stones[i].setPosition(stonesPos[i].x * _blockSize, stonesPos[i].y * _blockSize);
        stones[i].setFillColor(sf::Color::Blue);
        stones[i].setSize(sf::Vector2f(_blockSize, _blockSize));
    }
    _stonesPos = stonesPos;
    _stoneShape = stones;
    //_________________
    //  std::vector<sf::Vector2i> _allStones;
    //    sf::RectangleShape _stone;
    //обновить добавление ов все айтемы для мтолкнований
}

World::~World() {}


std::vector<sf::Vector2i> World::get_world_items() {
    std::vector<sf::Vector2i> result;
    for (auto itr = _stonesPos.begin(); itr != _stonesPos.end(); ++itr)
        result.push_back(*itr);
    return result;
}

void World::RespawnApple() {
    int maxX = (_windowSize.x / _blockSize);
    int maxY = (_windowSize.y / _blockSize);
    srand(unsigned(time(0)));
    _item = sf::Vector2i(
            rand() % maxX, rand() % maxY);
    _appleShape.setPosition(
            _item.x * _blockSize,
            _item.y * _blockSize);
}

sf::Vector2i World::getApplePosition() {
    return _item;
}

void
World::Update(Snake &l_player, Snake &l_second_player, std::vector<SnakeBot> &bots, std::vector<sf::Vector2i> items) {

    if (l_player.GetPosition() == _item) {
        l_player.Extend();
        l_player.IncreaseScore();
        RespawnApple();
    }

    if (l_second_player.GetPosition() == _item) {
        l_second_player.Extend();
        l_second_player.IncreaseScore();
        RespawnApple();
    }

    for (auto itr = bots.begin(); itr != bots.end(); ++itr)
        if (itr->GetPosition() == _item) {
            itr->Extend();
            RespawnApple();

        }
    //менять положение на нечетных клетках (ослабит бота)
    //bots[0].ChangeDirection(_item);
    int gridSize_x = _windowSize.x / _blockSize;
    int gridSize_y = _windowSize.y / _blockSize;
    for (auto itr = bots.begin(); itr != bots.end(); ++itr) {
        if (itr->GetPosition().x == -1) {
            itr->SetPosition(sf::Vector2i(gridSize_x - 1, itr->GetPosition().y));
        }
        if (itr->GetPosition().y == -1) {
            itr->SetPosition(sf::Vector2i(itr->GetPosition().x, gridSize_y - 1));
        }
        if (itr->GetPosition().y == gridSize_y) {
            itr->SetPosition(sf::Vector2i(itr->GetPosition().x, 0));
        }
        if (itr->GetPosition().x == gridSize_x) {
            itr->SetPosition(sf::Vector2i(0, itr->GetPosition().y));
        }
    }
    //___

    if (l_player.GetPosition().x == -1) {
        l_player.SetPosition(sf::Vector2i(gridSize_x - 1, l_player.GetPosition().y));
    }
    if (l_player.GetPosition().y == -1) {
        l_player.SetPosition(sf::Vector2i(l_player.GetPosition().x, gridSize_y - 1));
    }
    if (l_player.GetPosition().y == gridSize_y) {
        l_player.SetPosition(sf::Vector2i(l_player.GetPosition().x, 0));
    }
    if (l_player.GetPosition().x == gridSize_x) {
        l_player.SetPosition(sf::Vector2i(0, l_player.GetPosition().y));
    }


    if (l_second_player.GetPosition().x == -1) {
        l_second_player.SetPosition(sf::Vector2i(gridSize_x - 1, l_second_player.GetPosition().y));
    }
    if (l_second_player.GetPosition().y == -1) {
        l_second_player.SetPosition(sf::Vector2i(l_second_player.GetPosition().x, gridSize_y - 1));
    }
    if (l_second_player.GetPosition().y == gridSize_y) {
        l_second_player.SetPosition(sf::Vector2i(l_second_player.GetPosition().x, 0));
    }
    if (l_second_player.GetPosition().x == gridSize_x) {
        l_second_player.SetPosition(sf::Vector2i(0, l_second_player.GetPosition().y));
    }
}

void World::Render(sf::RenderWindow &window) {
    sf::Texture texture;
    texture.loadFromFile("./picture/64/food.png");
    _appleShape.setTexture(texture);
    _appleShape.setScale(0.5,0.5);
    window.draw(_appleShape);
    for (auto i = _stoneShape.begin(); i != _stoneShape.end(); ++i) {
        window.draw(*i);
    }
}

int World::GetBlockSize() { return _blockSize; }

sf::Time Game::GetElapsed() { return _elapsed; }

void Game::RestartClock() { _elapsed += _clock.restart(); }

Textbox::Textbox() { Setup(5, 14, 350, sf::Vector2f(225, 0)); }

Textbox::Textbox(int l_visible, int l_charSize, int l_width, sf::Vector2f l_screenPos) {
    Setup(l_visible, l_charSize, l_width, l_screenPos);
}

Textbox::~Textbox() { Clear(); }

void Textbox::Setup(int l_visible, int l_charSize, int l_width, sf::Vector2f l_screenPos) {
    _numVisible = l_visible;

    sf::Vector2f offset(2.0f, 2.0f);
    _font.loadFromFile("Textures/retro-land-mayhem.ttf");
    _content.setFont(_font);
    _content.setString("score::");
    _content.setCharacterSize(l_charSize);

    _content.setFillColor(sf::Color::White);
    _content.setPosition(l_screenPos + offset);


    _backdrop.setSize(sf::Vector2f(l_width, (l_visible * (l_charSize * 1.2f))));
    _backdrop.setFillColor(sf::Color(90, 90, 90, 90));
    _backdrop.setPosition(l_screenPos);
}

void Textbox::Add(std::string l_message) {
    l_message = "score: " + l_message;
    _messages.push_back(l_message);
    if (_messages.size() < 2) { return; }
    _messages.erase(_messages.begin());
}

void Textbox::Clear() {
    _messages.clear();
}

void Textbox::Render(sf::RenderWindow &l_wind) {
    std::string l_content;
    for (auto &itr: _messages) {
        l_content.append(itr + " ");
    }
    // if(l_content != "") {
    _content.setString(l_content);
    l_wind.draw(_backdrop);
    l_wind.draw(_content);
    // }
}


//рахобраться с временем