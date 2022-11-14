#include <iostream>
#include "../include/snake.h"

int main() {

    sf::Vector2u size(1280, 700);
    std::string title = "snake";
    Window window(title, size, new Game(&window));
    sf::Event ev;
    while (window.GetRendWindow()->isOpen()) {
        window.update(window);
        window.render(window);
        window.GetRendWindow()->display();

    }

    return 0;

}