#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

enum AlgorithmChoices{
    NONE=0,
    BUBBLE_SORT, SELECTION_SORT, INSERTION_SORT, BFS_PATH, DFS_PATH, DIJKSTRA, BFS_GRAPH, DFS_RAPH,
    MAX
};

constexpr const char* choiceAlgorithmNames[MAX]={
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
    void update(/*const float deltaTime*/){
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
        m_pillars[m_j].setFocus(false);
        m_pillars[m_j+1].setFocus(false);
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

class SelectionSorter {
public:
    SelectionSorter(std::vector<SortingPillar>& pillars)
    : m_pillars(pillars)
    { reset(); }

    void reset() {
        m_i = 0;
        m_j = 1;
        m_minIndex = 0;
        m_done = false;
        for (auto& p : m_pillars) p.setFocus(false);
    }

    void step() {
        int n = m_pillars.size();
        if (m_done) {
            for (auto& p : m_pillars) p.setFocus(true);
            return;
        }

        // clear previous highlights
        for (auto& p : m_pillars) p.setFocus(false);

        // scan for new minimum in [m_i+1 .. n)
        if (m_pillars[m_j].m_value < m_pillars[m_minIndex].m_value)
            m_minIndex = m_j;

        // advance j
        ++m_j;
        if (m_j >= n) {
            // end of scan: swap min into position m_i
            if (m_minIndex != m_i) {
                std::swap(m_pillars[m_i].m_value,    m_pillars[m_minIndex].m_value);
                std::swap(m_pillars[m_i].m_position, m_pillars[m_minIndex].m_position);
            }
            ++m_i;
            if (m_i >= n - 1) {
                m_done = true;
                for (auto& p : m_pillars) p.setFocus(true);
                return;
            }
            // reset scan for next pass
            m_minIndex = m_i;
            m_j = m_i + 1;
        }

        // highlight boundary and current minimum
        m_pillars[m_i].setFocus(true);
        m_pillars[m_j].setFocus(true);
        m_pillars[m_minIndex].setFocus(true);
    }

    bool isDone() const { return m_done; }

private:
    std::vector<SortingPillar>& m_pillars;
    int m_i, m_j, m_minIndex;
    bool m_done;
};

class InsertionSorter {
public:
    InsertionSorter(std::vector<SortingPillar>& pillars)
    : m_pillars(pillars)
    { reset(); }

    void reset() {
        m_i    = 1;        // next element to “insert”
        m_j    = 1;        // scan position
        m_done = false;
        for (auto& p : m_pillars) p.setFocus(false);
    }

    void step() {
        int n = m_pillars.size();
        if (m_done) {
            for (auto& p : m_pillars) p.setFocus(true);
            return;
        }

        // clear last highlight
        if (m_j > 0) {
            m_pillars[m_j].setFocus(false);
            m_pillars[m_j - 1].setFocus(false);
        }

        // if out of bounds or in right place, advance i
        if (m_j == 0 ||
            m_pillars[m_j].m_value >= m_pillars[m_j - 1].m_value)
        {
            m_i++;
            m_j = m_i;
            if (m_i >= n) {
                m_done = true;
                for (auto& p : m_pillars) p.setFocus(true);
                return;
            }
        }
        // otherwise, swap down one step
        else {
            std::swap(m_pillars[m_j].m_value,    m_pillars[m_j - 1].m_value);
            std::swap(m_pillars[m_j].m_position, m_pillars[m_j - 1].m_position);
            m_j--;
        }

        // highlight the two being compared/swapped
        if (m_j > 0) {
            m_pillars[m_j].setFocus(true);
            m_pillars[m_j - 1].setFocus(true);
        }
    }

    bool isDone() const { return m_done; }

private:
    std::vector<SortingPillar>& m_pillars;
    int m_i, m_j;
    bool m_done;
};

std::vector<SortingPillar> sortingPillars;
BubbleSorter bubbleSorter(sortingPillars);
SelectionSorter selectionSorter(sortingPillars);
InsertionSorter insertionSorter(sortingPillars);

void initSortingPillars(int nr){
    sortingPillars.clear();
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
        if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::R) {
            shufflePillars(sortingPillars);
            bubbleSorter.reset();
            selectionSorter.reset();
            insertionSorter.reset();
        }
    }
}

void writeAlgorithmMenu(){
    std::cout<<"**************************************************\n0 - EXIT\nAlgorithms:\n";
    for(int i=NONE+1; i<MAX; i++){
        switch(i){
            case 1: std::cout<<"  Sorting:\n"; break;
            case 4: std::cout<<"  Pathfinding:\n"; break;
            case 7: std::cout<<"  Graph traversal:\n"; break;
        }
        std::cout<<"    "<<i<<" - "<<choiceAlgorithmNames[i]<<'\n';
    }
    std::cout<<"**************************************************\n";
}

void getIntChoice(const char* message, int& choice, int min, int max){
    while(true){
        std::cout<<message;
        if(!(std::cin>>choice)){
            std::cin.clear();
            std::cin.ignore(INT_MAX-1,'\n');
            std::cout<<unknownValueError<<std::endl;
            continue;
        }
        if(choice>=min && choice<=max){
            break;
        }
        std::cout<<unknownValueError<<std::endl;
    }
}

int main(){
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    while(true){
        writeAlgorithmMenu();
        int choiceAlgorithm=NONE;
        getIntChoice("Algorithm value: ", choiceAlgorithm, NONE, MAX-1);
        if(choiceAlgorithm)std::cout<<"Picked ["<<choiceAlgorithmNames[choiceAlgorithm]<<"]..."<<std::endl;

        switch(choiceAlgorithm){
            case NONE: return 0;
            case BUBBLE_SORT: bubbleSorter.reset();
            case SELECTION_SORT: selectionSorter.reset();
            case INSERTION_SORT:{
                insertionSorter.reset();
                int nrPillars=2;
                getIntChoice("Number of values to sort (2 - 100): ", nrPillars, 2, 100);
                initSortingPillars(nrPillars); 
                break;
            }
            default:{
                std::cout<<"Unimplemented feature!"<<std::endl;
                continue;
            }
        }

        sf::Clock clock;
        sf::RenderWindow window(windowBase,choiceAlgorithmNames[choiceAlgorithm],sf::Style::Close);
        window.setFramerateLimit(60);
        //game loop
        while(window.isOpen()){
            float deltaTime=clock.restart().asSeconds();
            handleEvents(window);
            //update(deltaTime);
            for(auto& it:sortingPillars) it.update();
            switch(choiceAlgorithm){
                case BUBBLE_SORT:{
                    if(!bubbleSorter.isDone()) bubbleSorter.step();
                    break;
                }
                case SELECTION_SORT:{
                    if(!selectionSorter.isDone()) selectionSorter.step();
                    break;
                }
                case INSERTION_SORT:{
                    if(!insertionSorter.isDone()) insertionSorter.step();
                    break;
                }
            }
            
            //render(window);
            window.clear();
            for(auto& it:sortingPillars) it.draw(window);
            window.display();
        }
        std::cout<<"Exited that one, you should try another!"<<std::endl;
    }
    std::cout<<"\nByebyee!\n";
    return 0;
}
