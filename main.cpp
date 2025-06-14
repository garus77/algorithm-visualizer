#include <SFML/Graphics.hpp>
#include <iostream>

enum Choices{
    NONE=0,
    BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT, BFSPATH, DFSPATH, DIJKSTRA, BFSGRAPH, DFGRAPH,
    MAX
};

constexpr const char* choiceNames[MAX]={
    "None", "Bubble sort", "Selection sort", "Insertion sort", "Breadth first search - pathfind", "Depth first search - pathfind", "Dijkstra's algorithm", "Breadth first search - graph", "Depth first search - graph"
};

constexpr const char* unknownValueError="Unknown value!\n";

const sf::VideoMode windowBase(800,800);

/* KEYBINDS
Esc - exit
*/
void handleEvents(sf::RenderWindow& window){
    sf::Event event;
    while(window.pollEvent(event)){
        if(event.type==sf::Event::Closed) window.close();
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::Escape) window.close();
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
            switch(i){
                case 1: std::cout<<"  Sorting:\n"; break;
                case 4: std::cout<<"  Pathfinding:\n"; break;
                case 7: std::cout<<"  Graph traversal:\n"; break;
            }
            std::cout<<"    "<<i<<" - "<<choiceNames[i]<<'\n';
        }
        std::cout<<"******************************\nType a value for an algorithm to be visualized:\n";
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

    sf::Clock clock;
    sf::RenderWindow window(windowBase,choiceNames[choice],sf::Style::Close);
    window.setFramerateLimit(60);
    //game loop
    while(window.isOpen()){
        float deltaTime=clock.restart().asSeconds();
        handleEvents(window);
        update(deltaTime);
        render(window);
    }

    return 0;
}
