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
   sf::Vector2u size(1280,700);
   std::string title = "snake";
   Window window(title,size, new MainMenu);
   sf::Event ev;
   while(window.GetRendWindow()->isOpen())
   {
       while(window.GetRendWindow()->pollEvent(ev))
       {
           switch(ev.type)
           {
               case sf::Event::Closed:
                   window.GetRendWindow()->close();
                   break;
               case sf::Event::KeyPressed:
                   if(ev.key.code == sf::Keyboard::Escape)
                       {
                           window.GetRendWindow()->close();
                           break;
                       }
                    if(ev.key.code == sf::Keyboard::Num0)
                       {
                           window.setState(new MainMenu);
                           break;
                       }
                   if(ev.key.code == sf::Keyboard::Num1)
                       {
                           window.setState(new Game);
                           break;
                       }
                   if(ev.key.code == sf::Keyboard::Num2)
                       {
                           window.setState(new Options);
                           break;
                       }
           }
       }
       window.render(window);
       window.GetRendWindow()->display();

   }

   return 0;

}