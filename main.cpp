#include <SFML/Graphics.hpp>

const sf::VideoMode windowBase(800,800);

/* KEYBINDS
Esc - exit
*/
void handleEvents(bool& running, sf::RenderWindow& window){
    sf::Event event;
    while(window.pollEvent(event)){
        if(event.type==sf::Event::Closed) running=false;
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::Escape) running=false;
    }
}

void update(float deltaTime){
}

void render(sf::RenderWindow& window){
    window.clear();

    

    window.display();
}

int main(){
    bool running=true;
    sf::Clock clock;
    sf::RenderWindow window(windowBase,"TITLE",sf::Style::Close);
    window.setFramerateLimit(60);

    //game loop
    while(running){
        float deltaTime=clock.restart().asSeconds();
        handleEvents(running,window);
        update(deltaTime);
        render(window);
    }

    return 0;
}
