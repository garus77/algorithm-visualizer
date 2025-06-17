#include "main.h"

enum AlgorithmChoices
{
    NONE = 0,
    BUBBLE_SORT,
    SELECTION_SORT,
    INSERTION_SORT,
    BFS_PATH,
    DFS_PATH,
    DIJKSTRA,
    BFS_GRAPH,
    DFS_RAPH,
    MAX
};

constexpr const char *choiceAlgorithmNames[MAX] = {"None", "Bubble sort", "Selection sort", "Insertion sort", "Breadth first search - pathfind", "Depth first search - pathfind", "Dijkstra's algorithm", "Breadth first search - graph", "Depth first search - graph"};

constexpr const char *unknownValueError = "Unknown value!\n";

const sf::VideoMode windowBase(800, 800);

std::vector<SortingPillar> sortingPillars;
BubbleSorter bubbleSorter(sortingPillars);
SelectionSorter selectionSorter(sortingPillars);
InsertionSorter insertionSorter(sortingPillars);

void initSortingPillars(int nr)
{
    sortingPillars.clear();
    for (int i = 1; i <= nr; i++)
    {
        sortingPillars.emplace_back(sf::Vector2f(i * windowBase.width / nr, windowBase.height), sf::Vector2f(windowBase.width / nr, i * windowBase.height / nr), i);
    }
}

void shufflePillars(std::vector<SortingPillar> &pillars)
{
    int n = pillars.size();
    for (int i = 0; i < n; i++)
    {
        pillars[i].setFocus(0);
        int random = rand() % n;
        if (i + 1 + random >= n) random -= n;
        std::swap(pillars[i].m_value, pillars[i + 1 + random].m_value);
        std::swap(pillars[i].m_position, pillars[i + 1 + random].m_position);
    }
}

/* KEYBINDS
Esc - exit
*/
void handleEvents(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
        {
            shufflePillars(sortingPillars);
            bubbleSorter.reset();
            selectionSorter.reset();
            insertionSorter.reset();
        }
    }
}

void writeAlgorithmMenu()
{
    std::cout << "**************************************************\n0 - EXIT\nAlgorithms:\n";
    for (int i = NONE + 1; i < MAX; i++)
    {
        switch (i)
        {
        case 1:
            std::cout << "  Sorting:\n";
            break;
        case 4:
            std::cout << "  Pathfinding:\n";
            break;
        case 7:
            std::cout << "  Graph traversal:\n";
            break;
        }
        std::cout << "    " << i << " - " << choiceAlgorithmNames[i] << '\n';
    }
    std::cout << "**************************************************\n";
}

void getIntChoice(const char *message, int &choice, int min, int max)
{
    while (true)
    {
        std::cout << message;
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(INT_MAX - 1, '\n');
            std::cout << unknownValueError << std::endl;
            continue;
        }
        if (choice >= min && choice <= max)
        {
            break;
        }
        std::cout << unknownValueError << std::endl;
    }
}

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    while (true)
    {
        writeAlgorithmMenu();
        int choiceAlgorithm = NONE;
        getIntChoice("Algorithm value: ", choiceAlgorithm, NONE, MAX - 1);
        if (choiceAlgorithm) std::cout << "Picked [" << choiceAlgorithmNames[choiceAlgorithm] << "]..." << std::endl;

        switch (choiceAlgorithm)
        {
        case NONE:
            return 0;
        case BUBBLE_SORT:
            bubbleSorter.reset();
        case SELECTION_SORT:
            selectionSorter.reset();
        case INSERTION_SORT:
        {
            insertionSorter.reset();
            int nrPillars = 2;
            getIntChoice("Number of values to sort (2 - 100): ", nrPillars, 2, 100);
            initSortingPillars(nrPillars);
            break;
        }
        default:
        {
            std::cout << "Unimplemented feature!" << std::endl;
            continue;
        }
        }

        int frameRate;
        getIntChoice("Sorting framerate cap (0 for uncapped - 1000): ", frameRate, 0, 1000);
        sf::RenderWindow window(windowBase, choiceAlgorithmNames[choiceAlgorithm], sf::Style::Close);
        if (frameRate) window.setFramerateLimit(frameRate);
        // game loop
        sf::Clock clock;
        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();
            handleEvents(window);
            // update(deltaTime);
            for (auto &it : sortingPillars)
                it.update();
            switch (choiceAlgorithm)
            {
            case BUBBLE_SORT:
            {
                if (!bubbleSorter.isDone()) bubbleSorter.step();
                break;
            }
            case SELECTION_SORT:
            {
                if (!selectionSorter.isDone()) selectionSorter.step();
                break;
            }
            case INSERTION_SORT:
            {
                if (!insertionSorter.isDone()) insertionSorter.step();
                break;
            }
            }

            // render(window);
            window.clear();
            for (auto &it : sortingPillars)
                it.draw(window);
            window.display();
        }
        std::cout << "Exited that one, you should try another!" << std::endl;
    }
    std::cout << "\nByebyee!\n";
    return 0;
}
