#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

enum Choices{
    NONE=0,
    BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT, BFS_PATH, DFS_PATH, DIJKSTRA, BFS_GRAPH, DFS_RAPH,
    MAX
};

constexpr const char* choiceNames[MAX]={
    "None", "Bubble sort", "Selection sort", "Insertion sort", "Breadth first search - pathfind", "Depth first search - pathfind", "Dijkstra's algorithm", "Breadth first search - graph", "Depth first search - graph"
};

constexpr const char* unknownValueError="Unknown value!\n";

const sf::VideoMode windowBase(800,800);

class SortingPillar{
public:
    SortingPillar(sf::Vector2f position, sf::Vector2f size, int value):
        m_position(position), m_size(size), m_value(value){
        m_shape.setFillColor(sf::Color::White);
        m_shape.setOutlineColor(sf::Color::Red);
        m_shape.setOutlineThickness(-1);
        m_shape.setOrigin(size);
        m_shape.setPosition(position);
        m_shape.setSize(size);
    }
    void update(const float deltaTime){
        m_shape.setPosition(m_position);
        m_shape.setSize(m_size);
    }
    void draw(sf::RenderTarget& renderTarget){
        renderTarget.draw(m_shape);
    }
    void toggleFocusColor(){
        if(m_shape.getOutlineColor()==sf::Color::Red){
            m_shape.setOutlineColor(sf::Color::Green);
            m_shape.setOutlineThickness(-2);
        }
        else{
            m_shape.setOutlineColor(sf::Color::Red);
            m_shape.setOutlineThickness(-1);
        }
    }
public:
    sf::Vector2f m_position;
    int m_value;
private:
    sf::RectangleShape m_shape;
    sf::Vector2f m_size;
};

std::vector<SortingPillar> sortingPillars;

void initSortingPillars(int nr){
    for(int i=1; i<=nr; i++){
        sortingPillars.emplace_back(
            sf::Vector2f(
                i*windowBase.width/nr,
                windowBase.height
            ),
            sf::Vector2f(
                windowBase.width/nr,
                i*windowBase.height/nr
            ),
            i
        );
        //std::cout<<i<<". pillar Position: "<<i*windowBase.width/nr<<" "<<windowBase.height<<" Size: "<<windowBase.width/nr<<" "<<i*windowBase.height/nr<<std::endl;
    }
}

void shufflePillars(std::vector<SortingPillar>& pillars){
    int n=pillars.size();
    for(int i=0; i<n; i++){
        int random=rand()%n;
        if(i+1+random>=n) random-=n;
        std::swap(pillars[i].m_value,pillars[i+1+random].m_value);
        std::swap(pillars[i].m_position,pillars[i+1+random].m_position);
    }
}

void bubbleSortPillars(std::vector<SortingPillar>& pillars){
    int n=pillars.size();
    bool swapped;
    for(int i=0; i<n-1; i++){
        swapped=false;
        for(int j=0; j<n-i-1; j++){
            if(pillars[j].m_value>pillars[j+1].m_value){
                std::swap(pillars[j].m_value,pillars[j+1].m_value);
                std::swap(pillars[j].m_position,pillars[j+1].m_position);
                swapped=true;
            }
        }
        if(!swapped) break;
    }
}

/* KEYBINDS
Esc - exit
*/
void handleEvents(sf::RenderWindow& window){
    sf::Event event;
    while(window.pollEvent(event)){
        if(event.type==sf::Event::Closed) window.close();
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::Escape) window.close();
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::D) bubbleSortPillars(sortingPillars);
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::R) shufflePillars(sortingPillars);
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::F) sortingPillars[0].toggleFocusColor();
    }
}

void update(const float deltaTime){
    for(auto& it:sortingPillars) it.update(deltaTime);
}

void render(sf::RenderWindow& window){
    window.clear();

    for(auto& it:sortingPillars) it.draw(window);

    window.display();
}

void getChoice(int& choice){
    while(true){
        std::cout<<"**************************************************\nAlgorithms:\n";
        for(int i=NONE+1; i<MAX; i++){
            switch(i){
                case 1: std::cout<<"  Sorting:\n"; break;
                case 4: std::cout<<"  Pathfinding:\n"; break;
                case 7: std::cout<<"  Graph traversal:\n"; break;
            }
            std::cout<<"    "<<i<<" - "<<choiceNames[i]<<'\n';
        }
        std::cout<<"**************************************************\nType a value for an algorithm to be visualized or 0 to exit:\n";
        if(!(std::cin>>choice)){
            std::cin.clear();
            std::cin.ignore(INT_MAX-1,'\n');
            std::cout<<unknownValueError<<std::endl;
            continue;
        }
        if(choice>=NONE && choice<MAX){
            if(choice)std::cout<<"Visualizing ["<<choiceNames[choice]<<"]..."<<std::endl;
            break;
        }
        std::cout<<unknownValueError<<std::endl;
    }
}

int main(){
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int choice=NONE;
    getChoice(choice);


    switch(choice){
        case NONE: return 0;
        case BUBBLE_SORT: initSortingPillars(20); break;
    }

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

    std::cout<<"\nByebyee!\n";
    return 0;
}
