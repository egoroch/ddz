
#include "../include/snake.h"

//_________________________________________\
// constructors and destructor
Window::Window() {
    Setup("Snake", sf::Vector2u(1280, 720));
}

Window::Window(const std::string& title, const sf::Vector2u& size,State* state)
{
    Setup(title, size);
    _state = state;
    Create();
}

Window::~Window() {
    Destroy();
}

// functions
sf::Vector2u Window::GetWindowSize(){
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
    Create();
}
sf::RenderWindow* Window::GetRendWindow(){
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


void Window::setState(State* st){
    if(_state != nullptr) delete _state;
    _state =st;
    _state->setWindow(this);
}

void MainMenu::render(Window& window) {
    window.GetRendWindow()->clear(sf::Color::Black);
}
void MainMenu::update() {};
void MainMenu::pollEvent() {};


void Game::render(Window& window) {
    window.GetRendWindow()->clear(sf::Color::Red);
}
void Game::update() {};
void Game::pollEvent() {};


void Options::render(Window& window) {
    window.GetRendWindow()->clear(sf::Color::Blue);
}
void Options::update() {};
void Options::pollEvent() {};





//_________________________________________
/*
MainMenu::MainMenu()  {
    MainMenu::SetupButtons();
}

void MainMenu::SetupButtons() {
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf");
    _text.setFont(font);
    _text.setCharacterSize(18);
    _text.setString("MENU:");

    sf::FloatRect textRect = _text.getGlobalBounds();
    _text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    _text.setPosition(400,100);

    _buttonSize = sf::Vector2f(300.0f, 32.0f);
    _buttonPosition = sf::Vector2f(400, 200);

    std::string str[3];
    str[0] = "PLAY";
    str[1] = "CREDITS";
    str[2] = "EXIT";

    for(int i = 0; i < 3; ++i) {
        sf::Vector2f buttonPosition(_buttonPosition.x,_buttonPosition.y +
                                                      (i * (_buttonSize.y)));
        _rects[i].setSize(_buttonSize);
        _rects[i].setFillColor(sf::Color::Red);
        _rects[i].setOrigin(_buttonSize.x / 2.0f,
                            _buttonSize.y / 2.0f);
        _rects[i].setPosition(buttonPosition);
        _labels[i].setFont(font);
        _labels[i].setString(sf::String(str[i]));
        _labels[i].setCharacterSize(12);
        sf::FloatRect rect = _labels[i].getLocalBounds();
        _labels[i].setOrigin(rect.left + rect.width / 2.0f,
                             rect.top + rect.height / 2.0f);
        _labels[i].setPosition(buttonPosition);
    }
}

void MainMenu::update() {
    _window.Update();
}

void MainMenu::Draw(){
    _window.BeginDraw();
    _window.Draw(_text);
    _window.Draw(_rects[0]);
    _window.Draw(_labels[0]);
    _window.Draw(_rects[1]);
    _window.Draw(_labels[1]);
    _window.Draw(_rects[2]);
    _window.Draw(_labels[2]);
    _window.EndDraw();
}
MainMenu::MainMenu() : _window() {
    MainMenu::SetupButtons();
    MainMenu::SetupGraphics();
}

MainMenu::~MainMenu() {
}

Window& MainMenu::GetWindow(){
    return _window;
}

void MainMenu::SetupGraphics() {
    std::cout << "graphics";
}

void MainMenu::SetupButtons() {
    sf::Font font;

    if(!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf")){
        return;
    }

    sf::Text start("New game", font);
    sf::Text settings("Settings", font);

    _buttonStart.setSize({240, 100});
    _buttonSettings.setSize({240, 100});

    _buttonStart.setOrigin({120,50});
    _buttonSettings.setOrigin({120,50});

    start.setOrigin({120,50});
    settings.setOrigin({120,50});

    //start.setPosition({})

    _buttonStart.setPosition({static_cast<float>(_window.GetWindowSize().x/2), static_cast<float>(_window.GetWindowSize().y/2) + 70});
    _buttonSettings.setPosition({static_cast<float>(_window.GetWindowSize().x/2), static_cast<float>(_window.GetWindowSize().y/2) - 70});

    _buttonStart.setFillColor(sf::Color::White);
    _buttonSettings.setFillColor(sf::Color::White);


}

void MainMenu::Update() {
    _window.Update();
}

void MainMenu::Render() {
    _window.BeginDraw();
    _window.Draw(_buttonStart);
    _window.Draw(_buttonSettings);
    _window.EndDraw();
}*/