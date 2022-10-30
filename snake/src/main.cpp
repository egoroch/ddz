#include <iostream>
#include <thread>
#include <SFML/Graphics.hpp>

void NewThreadFunc(sf::RenderWindow& window) {
    window.setActive();
    sf::RenderWindow window2(sf::VideoMode(600,600), "2021-1-10-sem-17.10.2022");
    while(window2.isOpen()){
        sf::Event event;
        while(window2.pollEvent(event)){
            if(event.type == sf::Event::Closed) {
                window2.close();
                window.setActive(false);
            }
        }
        window2.clear();
        window2.display();
    }


}

int main()
{
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");
    sf::Font font;

    if(!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf")){
        return 0;
    }
    sf::Text text;
    std::string str = "";
    text.setFont(font);
    text.setCharacterSize(24);
    text.setString(str);
    text.setColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);
    int code = 0;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::TextEntered) {
                code = event.text.unicode;

                if(code == 8 || code ==127)
                    str.pop_back();
                else
                    str += static_cast<char>(code);
                text.setString(str);
                if(str == "window") {
                    window.clear();
                    window.draw(text);
                    window.display();
                    window.setActive(false);
                    sf::Thread newThread(NewThreadFunc, std::ref(window));
                    newThread.launch();
                    newThread.wait();
                    window.setActive();
                }
            }
        }
        window.clear();
        window.draw(text);
        window.display();
    }

    return 0;
}
