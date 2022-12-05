#include <iostream>
#include "../include/snake.h"

int main() {
    json config = getConfig("src/config.json");
    sf::Vector2u size(config["default"]["window_width"], config["default"]["window_height"]);
    std::string title = "The Best Snake";
    Window window(title, size, config, nullptr);
    window.setState(new MainMenu(&window));
    window.GetRendWindow()->setFramerateLimit(30);
    while (window.GetRendWindow()->isOpen()) {
        window.render(window);
        window.update(window);

    }

    return 0;

}