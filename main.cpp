#include <SFML/Graphics.hpp>
#include <iostream>

enum Choices{
    NONE=0,
    BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT,
    MAX
};

constexpr const char* choiceNames[MAX]={
    "None", "Bubble sort", "Selection sort", "Insertion sort"
};

constexpr const char* unknownValueError="Unknown value!\n";

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

void getChoice(int& choice){
    while(true){
        std::cout<<"******************************\nAlgorithms:\n";
        for(int i=NONE+1; i<MAX; i++){
            std::cout<<i<<" - "<<choiceNames[i]<<'\n';
        }
        std::cout<<"******************************\nGive a value for an algorithm to be visualized:\n";
        if(!(std::cin>>choice)){
            std::cin.clear();
            std::cin.ignore(INT_MAX-1,'\n');
            std::cout<<unknownValueError<<std::endl;
            continue;
        }
        if(choice>NONE && choice<MAX){
            std::cout<<"Visualizing ["<<choiceNames[choice]<<"]..."<<std::endl;
            break;
        }
        std::cout<<unknownValueError<<std::endl;
    }
}

int main(){
    int choice=NONE;
    getChoice(choice);

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
