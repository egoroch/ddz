#include <iostream>
#include "../include/snake.h"

int main() {

    sf::Vector2u size(1280, 640);
    std::string title = "snake";
    Window window(title, size, nullptr);
    window.setState(new Game(&window));
    sf::Event event;
    window.GetRendWindow()->setFramerateLimit(30);
    while (window.GetRendWindow()->isOpen()) {
             window.update(window,event);

        window.render(window);

    }

    return 0;

}