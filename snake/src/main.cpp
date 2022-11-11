#include <iostream>
#include "../include/snake.h"

int main(){
    Window menu;
    sf::Text text;
    std::string str = "its done";
    text.setString(str);
    sf::Font font;

    if(!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/Ubuntu-B.ttf")){
        return 1;
    }
    text.setFont(font);

    while(!menu.IsDone()) {
        menu.Update();
        menu.BeginDraw();
        menu.Draw(text);
        menu.EndDraw();
        //menu.handleInput();
    }
}