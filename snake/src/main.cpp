#include <iostream>
#include "../include/snake.h"

int main() {

    sf::Vector2u size(300, 300);
    std::string title = "snake";
    Window window(title, size, nullptr);
    window.setState(new Game(&window));
    window.GetRendWindow()->setFramerateLimit(30);
    while (window.GetRendWindow()->isOpen()) {
             window.update(window);

        window.render(window);

    }

    return 0;

}