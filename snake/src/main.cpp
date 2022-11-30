#include <iostream>
#include "../include/snake.h"

int main() {

    sf::Vector2u size(1280, 640);
    std::string title = "snake";
    Window window(title, size, nullptr);
    window.setState(new MainMenu);
    window.GetRendWindow()->setFramerateLimit(30);
    while (window.GetRendWindow()->isOpen()) {
        window.render(window);
        window.update(window);
    }

    return 0;

}