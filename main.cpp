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
    void setFocus(bool state){
        if(state){
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

class BubbleSorter{
public:
    BubbleSorter(std::vector<SortingPillar>& pillars): 
        m_pillars(pillars) {
        reset();
        }
    void reset(){
        m_i=0;
        m_j=0;
        m_swapped=false;
        m_done=false;
    }
    void step(){
        if(m_done){
            for(auto& it:m_pillars) it.setFocus(true);
            return;
        }
        int n=m_pillars.size();
        if(m_pillars[m_j].m_value>m_pillars[m_j+1].m_value){
            std::swap(m_pillars[m_j].m_value, m_pillars[m_j+1].m_value);
            std::swap(m_pillars[m_j].m_position, m_pillars[m_j+1].m_position);
            m_swapped=true;
        }
        m_j++;
        if(m_j >= n - m_i -1) {
            if(!m_swapped) {
                m_done = true;
                for(auto& it:m_pillars) it.setFocus(true);
                return;
            }
            m_swapped = false;
            m_j = 0;
            m_i++;
            if(m_i >= n-1){
                m_done = true;
                for(auto& it:m_pillars) it.setFocus(true);
            }
        }
        for(auto& it:m_pillars) it.setFocus(false);
        m_pillars[m_j].setFocus(true);
        m_pillars[m_j+1].setFocus(true);
    }
    bool isDone(){
        return m_done;
    }
private:
    std::vector<SortingPillar>& m_pillars;
    int m_i, m_j;
    bool m_swapped;
    bool m_done;
};

std::vector<SortingPillar> sortingPillars;
BubbleSorter bubbleSorter(sortingPillars);

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
    }
}

void shufflePillars(std::vector<SortingPillar>& pillars){
    int n=pillars.size();
    for(int i=0; i<n; i++){
        pillars[i].setFocus(0);
        int random=rand()%n;
        if(i+1+random>=n) random-=n;
        std::swap(pillars[i].m_value,pillars[i+1+random].m_value);
        std::swap(pillars[i].m_position,pillars[i+1+random].m_position);
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
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::D) {
            bubbleSorter.step();
        }
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::R) {
            shufflePillars(sortingPillars);
            bubbleSorter.reset();
        }
    }
}

void update(const float deltaTime){
    for(auto& it:sortingPillars) it.update(deltaTime);
    if(!bubbleSorter.isDone())bubbleSorter.step();
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
    while(true){
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
        std::cout<<"Exited that one, You should try another one!"<<std::endl;
    }
    std::cout<<"\nByebyee!\n";
    return 0;
}
