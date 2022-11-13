#include <iostream>
#include "../include/snake.h"

int main(){
   /* Window menu;
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
    }*/
   sf::RenderWindow window(sf::VideoMode(640,480),"Snake",sf::Style::Titlebar | sf::Style::Close);
   CurrentWindow* current = new CurrentWindow(new MainMenu);
   sf::Event ev;
   while(window.isOpen())
   {
       while(window.pollEvent(ev))
       {
           switch(ev.type)
           {
               case sf::Event::Closed:
                   window.close();
                   break;
               case sf::Event::KeyPressed:
                   if(ev.key.code == sf::Keyboard::Escape)
                       {
                           window.close();
                           break;
                       }
                    if(ev.key.code == sf::Keyboard::Num0)
                       {
                           current->setState(new MainMenu);
                           break;
                       }
                   if(ev.key.code == sf::Keyboard::Num1)
                       {
                           current->setState(new Game);
                           break;
                       }
                   if(ev.key.code == sf::Keyboard::Num2)
                       {
                           current->setState(new Options);
                           break;
                       }
           }
       }
       current->Draw(window);
       window.display();

   }

   return 0;

}