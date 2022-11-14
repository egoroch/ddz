
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
    _isDone = false;
    this->GetRendWindow()->setFramerateLimit(60);
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
    if (_state != nullptr) delete _state;
    _state = st;
    _state->setWindow(this);
}

void MainMenu::render(Window &window) {
    //window.GetRendWindow()->clear(sf::Color::Black);
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
    }
};


Game::Game() { _window = nullptr; }

Game::Game(Window *window) {
    _window = window;
    initPlayer();
}

Game::~Game() {
    delete _player;
}

void Game::render(Window &window) {
    window.GetRendWindow()->clear();
    _player->move();
    _player->render(*(_window->GetRendWindow()));
}

//_________________________________
void Game::update(Window &window) {
    sf::Event event;
    while (window.GetRendWindow()->pollEvent(event)) {
        if (event.Event::type == sf::Event::Closed)
            window.GetRendWindow()->close();
        if (event.Event::KeyPressed && event.Event::key.code == sf::Keyboard::Escape)
            window.GetRendWindow()->close();

        //move player
        if (sf::Keyboard::isKeyPressed((sf::Keyboard::Right)))
            _player->changeDir(sf::Vector2f(1.f, 0));
        if (sf::Keyboard::isKeyPressed((sf::Keyboard::Left)))
            _player->changeDir(sf::Vector2f(-1, 0));
        if (sf::Keyboard::isKeyPressed((sf::Keyboard::Up)))
            _player->changeDir(sf::Vector2f(0, -1));
        if (sf::Keyboard::isKeyPressed((sf::Keyboard::Down)))
            _player->changeDir(sf::Vector2f(0, 1));

    };
};


void Game::initPlayer() {
    _player = new Player();
}


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

Player::Player() {
    this->initTexture();
    this->initSprite();
    _direct = sf::Vector2f(1, 0);
}

Player::~Player() noexcept {}

void Player::update() {}

void Player::render(sf::RenderTarget &target) {
    target.draw(this->_sprite);
}

void Player::initTexture() {
    if (!_texture.loadFromFile("Textures/snake2.png")) {
        std::cout << "error with load snake.png" << '\n';
    }

}

void Player::initSprite() {
    sf::Texture::bind(&_texture);
    _sprite.setTexture(_texture);

    _sprite.scale(0.2, 0.2);
}

void Player::changeDir(sf::Vector2f dir) {
    _direct = dir;
}

void Player::move() {
    _sprite.move(_direct.x * _movementSpeed, _direct.y * _movementSpeed);
}