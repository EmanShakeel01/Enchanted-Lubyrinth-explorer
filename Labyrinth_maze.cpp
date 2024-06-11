#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include <random>

#include "menu.h"
#include "avl.h"

using namespace std;
#define SIZE 30
#define CELL_WIDTH 20

class Cell
{
public:
    int x, y;
    int pos;
    float size = 30.f;
    float thickness = 2.f;
    bool walls[4] = { true,true,true,true };
    bool visited = false;
    bool isActive = false;
    bool hasObstacle = false;
    bool hasmonster = false;
    bool isCollected = false;

    Cell();
    Cell(int, int);
    void draw(sf::RenderWindow* window, sf::Texture& obstacleTexture);
    void drawTreasure(sf::RenderWindow* window, sf::Texture& treasureTexture);
    void drawMonster(sf::RenderWindow* window, sf::Texture& monsterTexture);
    void drawGem(sf::RenderWindow* window, sf::Texture& gemTexture);

};

Cell::Cell() 
{
    x = 0;
    y = 0;
    pos = 0;
    size = 30.f;
    thickness = 2.f;
    visited = false;
    isActive = false;
    hasObstacle = false;
    hasmonster = false;
    isCollected = false;
}

Cell::Cell(int _x, int _y) 
{
    x = _x;
    y = _y;
    pos = 0;
    size = 30.f;
    thickness = 2.f;
    visited = false;
    isActive = false;
    hasObstacle = false;
    hasmonster = false;
    isCollected = false;
}

class Treasure 
{
public:
    int x, y;
    bool isCollected = false;
    Treasure();
    Treasure(int, int);

    bool operator<( Treasure other) const
    {

        if (x < other.x) 
        {
            return true;
        }
        else if (x == other.x) 
        {
            return y < other.y;
        }
        else
        {
            return false;
        }
    }

    bool operator>( Treasure other) const
    {

        if (x > other.x) 
        {
            return true;
        }
        else if (x == other.x)
        {
            return y > other.y;
        }
        else
        {
            return false;
        }
    }
};

Treasure::Treasure() {}
Treasure::Treasure(int _x, int _y)
{
    x = _x;
    y = _y;
}

class Gem 
{
public:
    int x, y;
    bool isCollected = false;
    int points = 10;

    Gem();
    Gem(int, int);

    bool operator<( Gem other) const 
    {
        if (x < other.x) 
        {
            return true;
        }
        else if (x == other.x)
        {
            return y < other.y;
        }
        else 
        {
            return false;
        }
    }

    bool operator>( Gem other) const
    {
        if (x > other.x)
        {
            return true;
        }
        else if (x == other.x) 
        {
            return y > other.y;
        }
        else 
        {
            return false;
        }
    }
};

Gem::Gem() {}
Gem::Gem(int _x, int _y) 
{
    x = _x;
    y = _y;
}



void Cell::draw(sf::RenderWindow* window, sf::Texture& obstacleTexture) 
{
    sf::RectangleShape rect;

    rect.setFillColor(sf::Color::Cyan);

    if (hasObstacle)
    {
        sf::Sprite obstacleSprite(obstacleTexture);
        obstacleSprite.setScale(size / obstacleTexture.getSize().x, size / obstacleTexture.getSize().y);
        obstacleSprite.setPosition(x, y);
        window->draw(obstacleSprite);
    }

    if (walls[0]) 
    {
        rect.setSize(sf::Vector2f(size, thickness));
        rect.setPosition(x, y);
        window->draw(rect);
    }
    if (walls[1]) 
    {
        rect.setSize(sf::Vector2f(thickness, size));
        rect.setPosition(x + size, y);
        window->draw(rect);
    }
    if (walls[2])
    {
        rect.setSize(sf::Vector2f(size + thickness, thickness));
        rect.setPosition(x, y + size);
        window->draw(rect);
    }
    if (walls[3]) 
    {
        rect.setSize(sf::Vector2f(thickness, size));
        rect.setPosition(x, y);
        window->draw(rect);
    }
}

void Cell::drawTreasure(sf::RenderWindow* window, sf::Texture& treasureTexture)
{
    if (!isCollected) 
    {
        sf::Sprite treasureSprite(treasureTexture);
        treasureSprite.setScale(size / treasureTexture.getSize().x, size / treasureTexture.getSize().y);
        treasureSprite.setPosition(x, y);
        window->draw(treasureSprite);
    }
}

void Cell::drawMonster(sf::RenderWindow* window, sf::Texture& monsterTexture)
{
    if (hasmonster)
    {
        sf::Sprite monsterSprite(monsterTexture);
        monsterSprite.setScale(size / monsterTexture.getSize().x, size / monsterTexture.getSize().y);
        monsterSprite.setPosition(x, y);
        window->draw(monsterSprite);
    }
}

void Cell::drawGem(sf::RenderWindow* window, sf::Texture& gemTexture)
{
    if (!isCollected) 
    {
        sf::Sprite gemSprite(gemTexture);
        gemSprite.setScale(size / gemTexture.getSize().x, size / gemTexture.getSize().y);
        gemSprite.setPosition(x, y);
        window->draw(gemSprite);
    }
}

void resetMaze(Cell* maze, int size) 
{
    for (int i = 0; i < size * size; i++) 
    {
        for (int j = 0; j < 4; j++)
        {
            maze[i].walls[j] = true;
            maze[i].visited = false;
            maze[i].isActive = false;

        }
    }
}

void removeWallsBetween(Cell* current, Cell* chosen, int size) 
{
    int direction = chosen->pos - current->pos;
    if (direction == 1)
    {
        current->walls[1] = false;
        chosen->walls[3] = false;
    }
    else if (direction == -1) 
    {
        current->walls[3] = false;
        chosen->walls[1] = false;
    }
    else if (direction == size)
    {
        current->walls[2] = false;
        chosen->walls[0] = false;
    }
    else if (direction == -size)
    {
        current->walls[0] = false;
        chosen->walls[2] = false;
    }
    else 
    {
        srand(time(NULL));
        int numWallsToRemove = rand() % 4 + 1;
        for (int i = 0; i < numWallsToRemove; ++i) 
        {
            int randomWall = rand() % 4;
            switch (randomWall)
            {
            case 0:
                current->walls[0] = false;
                chosen->walls[2] = false;
                break;
            case 1:
                current->walls[1] = false;
                chosen->walls[3] = false;
                break;
            case 2:
                current->walls[2] = false;
                chosen->walls[0] = false;
                break;
            case 3:
                current->walls[3] = false;
                chosen->walls[1] = false;
                break;
            }
        }
    }
}

void makeMaze(Cell* maze, int size)
{
    resetMaze(maze, size);
    Cell* stack = new Cell[size * size];
    int top = -1;
    maze[0].visited = true;
    stack[++top] = maze[0];

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<int> obstacleDist(0, 90);
    std::uniform_int_distribution<int> monsterDist(0, 110);

    for (int i = 0; i < size * size; i++)
    {
        if (obstacleDist(rng) == 0)
        {
            maze[i].hasObstacle = true;
        }
    }

    for (int i = 0; i < size * size; i++)
    {
        if (obstacleDist(rng) == 0) 
        {
            maze[i].hasObstacle = true;
        }
        if (monsterDist(rng) == 0) 
        { // checking if athe monster should be placed at this cell
            maze[i].hasmonster = true;
        }
    }

    while (top >= 0)
    {
        Cell current = stack[top];
        top--;
        int pos = current.pos;
        std::vector<int> neighbours;

        if ((pos) % (size) != 0 && pos > 0)
        {
            Cell left = maze[pos - 1];
            if (!left.visited) 
            {
                neighbours.push_back(pos - 1);
            }
        }
        if ((pos + 1) % (size) != 0 && pos < size * size)
        {
            Cell right = maze[pos + 1];
            if (!right.visited) 
            {
                neighbours.push_back(pos + 1);
            }
        }
        if ((pos + size) < size * size) 
        {
            Cell bottom = maze[pos + size];
            if (!bottom.visited) 
            {
                neighbours.push_back(pos + size);
            }
        }
        if ((pos - size) > 0)
        {
            Cell top = maze[pos - size];
            if (!top.visited) 
            {
                neighbours.push_back(pos - size);
            }
        }

        if (neighbours.size() > 0)
        {
            std::uniform_int_distribution<std::mt19937::result_type> dist6(0, neighbours.size() - 1);
            int randneighbourpos = dist6(rng);

            Cell* chosen = &maze[neighbours[randneighbourpos]];

            stack[++top] = current;

            removeWallsBetween(&maze[current.pos], chosen, size);

            chosen->visited = true;
            stack[++top] = *chosen;
        }
    }

    delete[] stack;
}

void removeObstacle(Cell* maze, int index)
{
    maze[index].hasObstacle = false;
}
void removemonster(Cell* maze, int index) 
{
    maze[index].hasmonster = false;
}

int collectedTreasures = 0;
int cellsVisited = 0;
int totalScore = 0;
int collectedGems = 0;

void handleMove(sf::Event event, Cell* maze, int* currentPos, int size, std::map<int, Treasure*>& treasureMap, std::map<int, Gem*>& gemMap) 
{

    if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::H)
    {
        if (!maze[*currentPos].walls[3] && !maze[*currentPos - 1].walls[1] && !maze[*currentPos - 1].hasObstacle) 
        {
            *currentPos = *currentPos - 1;
            maze[*currentPos].isActive = true;
        }
    }
    else if (event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::L )
    {
        if (!maze[*currentPos].walls[1] && !maze[*currentPos + 1].walls[3] && !maze[*currentPos + 1].hasObstacle)
        {
            *currentPos = *currentPos + 1;
            maze[*currentPos].isActive = true;
        }
    }
    else if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::K)
       {
        if ((*currentPos - size) < 0) 
        {
            return;
        }
        if (!maze[*currentPos].walls[0] && !maze[*currentPos - size].walls[2] && !maze[*currentPos - size].hasObstacle)
        {
            *currentPos = *currentPos - size;
            maze[*currentPos].isActive = true;
        }
    }
    else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::J )
    {
        if ((*currentPos + size) > size * size)
        {
            return;
        }
        if (!maze[*currentPos].walls[2] && !maze[*currentPos + size].walls[0] && !maze[*currentPos + size].hasObstacle) 
        {
            *currentPos = *currentPos + size;
            maze[*currentPos].isActive = true;
        }
    }
    
    if (event.key.code == sf::Keyboard::Space) 
    {
        int x = maze[*currentPos].x / CELL_WIDTH;
        int y = maze[*currentPos].y / CELL_WIDTH;

        if (x >= 0 && maze[*currentPos - 1].hasObstacle)
        {
            removeObstacle(maze, *currentPos - 1); //  left
        }
        else if (x <= SIZE - 1 && maze[*currentPos + 1].hasObstacle)
        {
            removeObstacle(maze, *currentPos + 1); //  right
        }
        else if (y >= 0 && maze[*currentPos - SIZE].hasObstacle)
        {
            removeObstacle(maze, *currentPos - SIZE); //  up
        }
        else if (y <= SIZE - 1 && maze[*currentPos + SIZE].hasObstacle)
        {
            removeObstacle(maze, *currentPos + SIZE); //  down
        }
        else if (x == 0 && maze[*currentPos + 1].hasObstacle) 
        {
            removeObstacle(maze, *currentPos + 1); // left edge
        }
        else if (x == SIZE - 1 && maze[*currentPos].x % CELL_WIDTH == CELL_WIDTH - 1 && maze[*currentPos - 1].hasObstacle && (maze[*currentPos - SIZE].hasObstacle || maze[*currentPos + SIZE].hasObstacle)) 
        {
            //  right edge 
            removeObstacle(maze, *currentPos - 1);
        }
        else if (x == SIZE - 1 && maze[*currentPos].x % CELL_WIDTH == 0 && maze[*currentPos + 1].hasObstacle && (maze[*currentPos - SIZE].hasObstacle || maze[*currentPos + SIZE].hasObstacle)) 
        {
            //  right edge with obstacle on right side
            removeObstacle(maze, *currentPos + 1);
        }
        else if (y == 0 && maze[*currentPos + SIZE].hasObstacle) 
        {
            removeObstacle(maze, *currentPos + SIZE); //  top edge
        }
        else if (y == SIZE - 1 && maze[*currentPos + SIZE].hasObstacle)
        {
            removeObstacle(maze, *currentPos - SIZE); // bottom edge
        }
    }

    if (event.key.code == sf::Keyboard::Z)
    {
        // Check adjacent cells for monst
        if (maze[*currentPos].x >= 0 && maze[*currentPos].x < (CELL_WIDTH * SIZE) &&
            maze[*currentPos].y >= 0 && maze[*currentPos].y < (CELL_WIDTH * SIZE))
        {
            if (maze[*currentPos - 1].hasmonster && (*currentPos % size) != 0)
            {
                removemonster(maze, *currentPos - 1);
            }
            else if (maze[*currentPos + 1].hasmonster && ((*currentPos + 1) % size) != 0) 
            {
                removemonster(maze, *currentPos + 1);
            }
            else if (maze[*currentPos - size].hasmonster && (*currentPos - size) >= 0)
            {
                removemonster(maze, *currentPos - size);
            }
            else if (maze[*currentPos + size].hasmonster && (*currentPos + size) < (SIZE * SIZE)) 
            {
                removemonster(maze, *currentPos + size);
            }
        }

    }

    maze[*currentPos].isActive = true;
    cellsVisited++;
    auto it = treasureMap.find(*currentPos);
    if (it != treasureMap.end())
    {

        Treasure* treasure = it->second;
        treasure->isCollected = true;

    }

    auto gemIt = gemMap.find(*currentPos);
    if (gemIt != gemMap.end())
    {
        Gem* gem = gemIt->second;
        if (!gem->isCollected)
        {
            
            totalScore += gem->points;
            gem->isCollected = true;
        }
    }

}

sf::Texture gameOverTexture;
sf::Sprite gameOverSprite;

void loadGameOverSprite(sf::RenderWindow& window)
{
    if (!gameOverTexture.loadFromFile("gameover.jpeg"))
    {
        
        std::cerr << "Failed to load game over texture!" << std::endl;
    }
    else 
    {
       
        gameOverSprite.setTexture(gameOverTexture);
        sf::Vector2u windowSize = window.getSize();
        gameOverSprite.setScale(static_cast<float>(windowSize.x) / gameOverTexture.getSize().x, static_cast<float>(windowSize.y) / gameOverTexture.getSize().y);
    }
}

void drawScore(sf::RenderWindow& window, sf::Font& font)
{
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setFillColor(sf::Color::Cyan);

   
    float textWidth = scoreText.getLocalBounds().width;
    float textHeight = scoreText.getLocalBounds().height;
    float windowWidth = window.getSize().x;
    scoreText.setPosition(windowWidth - textWidth - 100, 10); 

   
    if (totalScore < 0) 
    {
        
        window.draw(gameOverSprite);
    }
    else 
    {
       
        scoreText.setString("Score: " + std::to_string(totalScore));
        window.draw(scoreText);
    }
}

void generateTreasures(AVLTree<Treasure>& tree, Cell* maze, int numTreasures, int size, std::map<int, Treasure*>& treasureMap)
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist(0, size * size - 1);

    for (int i = 0; i < numTreasures; i++)
    {
        int pos = dist(rng);
        Treasure* treasure = new Treasure(maze[pos].x, maze[pos].y);
        tree.insert(*treasure);
        treasureMap[pos] = treasure;
    }

}

void generateGems(AVLTree<Gem>& tree, Cell* maze, int numGems, int size, std::map<int, Gem*>& gemMap)
{
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist(0, size * size - 1);

    for (int i = 0; i < numGems; i++) {
        int pos = dist(rng);
        Gem* gem = new Gem(maze[pos].x, maze[pos].y);
        tree.insert(*gem);
        gemMap[pos] = gem;
    }
}


std::map<int, Gem*> gemMap;
std::map<int, Treasure*> treasureMap;

const int INF = 1e9; // infinity vlaue
class Edge
{
public:

    int to;
    int weight;
    Edge* next;

    Edge(int t, int w)
    {
        to = t;
        weight = w;
        next = nullptr;
    }
};

class Node
{
public:

    int value;
    int priority;
    Node* next;

    Node(int val, int p)
    {
        value = val;
        priority = p;
        next = nullptr;
    }
};

class PriorityQueue
{
public:

    Node* head;

    PriorityQueue()
    {
        head = nullptr;
    }

    void push(int value, int priority)
    {
        Node* newNode = new Node(value, priority);
        if (!head || priority < head->priority)
        {
            newNode->next = head;
            head = newNode;
        }
        else
        {
            Node* current = head;
            while (current->next && current->next->priority <= priority)
            {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    int top()
    {
        return head->value;
    }

    void pop()
    {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    bool empty()
    {
        return head == nullptr;
    }
};

void Dijkstra_Shortest_Path(Edge** graph, int n, int source, int* dist)
{
    bool* visited = new bool[n]();
    for (int i = 0; i < n; ++i)
    {
        dist[i] = INF; // assign every distance to infiity
    }
    dist[source] = 0; // assign distnce to source as zero

    PriorityQueue obj;

    obj.push(source, 0);

    while (!obj.empty())
    {
        int u = obj.top();
        obj.pop();

        // igar the current distance is greater than the stored distance, ignore
        if (visited[u]) continue;
        visited[u] = true;

        // explore all neighbors of u
        for (Edge* e = graph[u]; e; e = e->next)
        {
            int v = e->to;
            int w = e->weight;


            if (dist[u] + w < dist[v])
            {
                dist[v] = dist[u] + w;
                obj.push(v, dist[v]);
            }
        }
    }

    delete[] visited;
}


Cell* maze; 

Edge** createGraph(Cell* maze, int size)
{
    Edge** graph = new Edge * [size * size];
    for (int i = 0; i < size * size; ++i) 
    {
        graph[i] = nullptr;
        
        int row = i / size;
        int col = i % size;
        if (col > 0 && !maze[i].walls[3])
        { 
            int to = i - 1;
            graph[i] = new Edge(to, 1);
        }
        if (col < size - 1 && !maze[i].walls[1]) 
        { 
            int to = i + 1;
            Edge* newEdge = new Edge(to, 1);
            if (!graph[i]) 
            {
                graph[i] = newEdge;
            }
            else 
            {
                newEdge->next = graph[i]->next;
                graph[i]->next = newEdge;
            }
        }
        if (row > 0 && !maze[i].walls[0]) 
        { 
            int to = i - size;
            Edge* newEdge = new Edge(to, 1);
            if (!graph[i]) 
            {
                graph[i] = newEdge;
            }
            else
            {
                newEdge->next = graph[i]->next;
                graph[i]->next = newEdge;
            }
        }
        if (row < size - 1 && !maze[i].walls[2]) 
        { 
            int to = i + size;
            Edge* newEdge = new Edge(to, 1);
            if (!graph[i])
            {
                graph[i] = newEdge;
            }
            else
            {
                newEdge->next = graph[i]->next;
                graph[i]->next = newEdge;
            }
        }
    }
    return graph;
}

void destroyGraph(Edge** graph, int size) 
{
    for (int i = 0; i < size; ++i)
    {
        Edge* current = graph[i];
        while (current) 
        {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] graph;
}

vector<int> findShortestPath(int playerPos, int finishPos, Edge** graph, int size)
{
    int* dist = new int[size * size];
    for (int i = 0; i < size * size; ++i)
    {
        dist[i] = INF;
    }
    dist[playerPos] = 0;

    PriorityQueue obj;

    obj.push(playerPos, 0);

    while (!obj.empty()) {
        int u = obj.top();
        obj.pop();

        if (u == finishPos)
        {
            break; 
        }

        for (Edge* e = graph[u]; e; e = e->next)
        {
            int v = e->to;
            int w = e->weight;

            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                obj.push(v, dist[v]);
            }
        }
    }
    vector<int> shortestPath;
    int current = finishPos;
    while (current != playerPos)
    {
        shortestPath.push_back(current);
        for (Edge* e = graph[current]; e; e = e->next)
        {
            if (dist[e->to] + e->weight == dist[current]) 
            {
                current = e->to;
                break;
            }
        }
    }
    shortestPath.push_back(playerPos);
    reverse(shortestPath.begin(), shortestPath.end());

    delete[] dist;
    return shortestPath;
}

void drawShortestPath(sf::RenderWindow& window, const vector<int>& shortestPath)
{
    sf::RectangleShape pathRect;
    pathRect.setFillColor(sf::Color(240, 240, 240));
    pathRect.setSize(sf::Vector2f(CELL_WIDTH, CELL_WIDTH));

    for (int cell : shortestPath)
    {
        int x = maze[cell].x;
        int y = maze[cell].y;
        pathRect.setPosition(x, y);
        window.draw(pathRect);
    }
}

void handleGameWon(sf::RenderWindow& window, sf::Music& music, sf::Sprite& gameWonSprite, sf::Font& font)
{
 
    music.stop();

   
    sf::Music victoryMusic;
    if (!victoryMusic.openFromFile("victory.ogg")) 
    {
        std::cerr << "Failed to load victory music!" << std::endl;
        return;
    }
    victoryMusic.play();

    window.clear();
    window.draw(gameWonSprite);
    window.display();

    totalScore = 0;
    
    sf::Event event;
    while (window.waitEvent(event))
    {
        if (event.type == sf::Event::Closed) 
        {
            window.close();
            return;
        }
        else if (event.type == sf::Event::KeyPressed)
        {
            
            victoryMusic.stop();
            music.play();
            Menu menu(window);

            while (true) 
            {
                menu.display_menu();
            }
        }
    }
}

void game_run()
{

    int progress = 0;

    int currentPos = 0;

    int movesCount = 0;

    sf::RenderWindow window(sf::VideoMode(CELL_WIDTH * SIZE + 90, CELL_WIDTH * SIZE + 90), "maze");
    Cell* maze = new Cell[SIZE * SIZE * SIZE];
    window.setFramerateLimit(30);
    sf::Music music;
    music.openFromFile("mazze.ogg");

    sf::Font font;
    if (!font.loadFromFile("Nasa21-l23X.ttf"))
    { 
        std::cerr << "Failed to load font!" << std::endl;
        return;
    }

    loadGameOverSprite(window);

  
    sf::Text treasureText;
    treasureText.setFont(font);
    treasureText.setCharacterSize(18);
    treasureText.setFillColor(sf::Color::Cyan);
    treasureText.setPosition(10, 10); 

    sf::Text progressText;
    progressText.setFont(font);
    progressText.setCharacterSize(18);
    progressText.setFillColor(sf::Color::Cyan);
    progressText.setPosition(10, 40); 

    sf::Text gemText;
    gemText.setFont(font);
    gemText.setCharacterSize(18);
    gemText.setFillColor(sf::Color::Cyan);
    gemText.setPosition(10, 40); 
    treasureText.setOutlineThickness(20);


    music.stop();
    music.play();

    sf::Texture wallTexture;
    if (!wallTexture.loadFromFile("wallh.jpg")) 
    {
        // Handle error if texture loading fails
        std::cerr << "Failed to load background texture!" << std::endl;
    }

    sf::Texture bgTexture;
    if (!bgTexture.loadFromFile("backwall.jpg"))
    {
        std::cerr << "Failed to load background texture!" << std::endl;

    }

    sf::Sprite background(bgTexture);
    background.setScale(window.getSize().x / bgTexture.getSize().x, window.getSize().y / bgTexture.getSize().y);

    sf::Texture obstacleTexture;
    if (!obstacleTexture.loadFromFile("cone.png"))
    {
        std::cerr << "Failed to load obstacle texture!" << std::endl;

    }
    sf::Texture monsterTexture;
    if (!monsterTexture.loadFromFile("most.png")) 
    {
        std::cerr << "Failed to load monster texture!" << std::endl;

    }
    sf::Texture treasureTexture;
    if (!treasureTexture.loadFromFile("sword.png"))
    {
        std::cerr << "Failed to load treasure texture!" << std::endl;
        return;
    }
    sf::Texture gemTexture;
    if (!gemTexture.loadFromFile("GEEM3.png"))
    {
        cerr << "Failed to load gem texture!" << endl;
        return;
    }
    sf::Sprite gameWonSprite;
    sf::Texture gameWonTexture;
    if (!gameWonTexture.loadFromFile("victory.jpeg"))
    {
        std::cerr << "Failed to load game won sprite!" << std::endl;
        
    }
    else
    {
        gameWonSprite.setTexture(gameWonTexture);
        float scaleX = static_cast<float>(window.getSize().x) / gameWonTexture.getSize().x;
        float scaleY = static_cast<float>(window.getSize().y) / gameWonTexture.getSize().y;

        gameWonSprite.setScale(scaleX, scaleY);
    }

    int size = SIZE;
    std::random_device dev;
    std::mt19937 rng(dev());

    int numNewObjects = 10;

    std::map<int, Treasure*> newObjectMap;

    std::uniform_int_distribution<int> newObjectDist(0, size * size - 1);
    for (int i = 0; i < numNewObjects; i++)
    {
        int pos = newObjectDist(rng);
        Treasure* newObject = new Treasure(maze[pos].x, maze[pos].y);
        newObjectMap[pos] = newObject; 
    }

    sf::Texture currentTexture;
    currentTexture.loadFromFile("player.png");

    sf::Texture finishTexture;
    finishTexture.loadFromFile("endtreasure.png");
    sf::Sprite currentSprite;
    currentSprite.setTexture(currentTexture);
    currentSprite.setScale(static_cast<float>(CELL_WIDTH) / currentTexture.getSize().x,
        static_cast<float>(CELL_WIDTH) / currentTexture.getSize().y);

    
    sf::Sprite finishSprite;
    finishSprite.setTexture(finishTexture);
    finishSprite.setScale(static_cast<float>(CELL_WIDTH) / finishTexture.getSize().x,
        static_cast<float>(CELL_WIDTH) / finishTexture.getSize().y);



    for (int i = 70, k = 0; i < CELL_WIDTH * SIZE + 70; i += CELL_WIDTH)
    {
        for (int j = 40; j < CELL_WIDTH * SIZE + 40; j += CELL_WIDTH, k++)
        {
            maze[k].y = i;
            maze[k].x = j;
            maze[k].size = CELL_WIDTH;
            maze[k].pos = k;
        }
    }

    makeMaze(maze, SIZE);

    AVLTree<Treasure> treasureTree;
    generateTreasures(treasureTree, maze, 10, SIZE, treasureMap);  
    maze[currentPos].isActive = true;

    AVLTree<Gem> gemTree;
    generateGems(gemTree, maze, 20, SIZE, gemMap);


    background.setScale((float)window.getSize().x / bgTexture.getSize().x, (float)window.getSize().y / bgTexture.getSize().y);
   
    Edge** graph = createGraph(maze, SIZE);


    while (window.isOpen())
    {
        sf::Event event;

        bool highlightPath = false; 

        vector<int> shortestPath = findShortestPath(currentPos, SIZE * SIZE - 1, graph, SIZE);
        bool moved = false;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Num3)
                {
                    window.close();
                }
                else if (event.key.code == sf::Keyboard::H)
                {
                    highlightPath = !highlightPath; // toggle highlight path
                }
                else 
                {
                    
                    int previousPos = currentPos;

                    handleMove(event, maze, &currentPos, SIZE, treasureMap, gemMap);

                    if (currentPos != previousPos)
                    {
                        moved = true;
                        movesCount++;
                    }
                }
                break;
            default:
                break;
            }
        }
        if (moved && movesCount % 10 == 0)
        {
            progress = progress + 2; 
        }

        if (moved && highlightPath) 
        {
        
            moved = false; 
        }
        
        if (highlightPath)
        {
            for (int cell : shortestPath) 
            {
                
                if (cell == currentPos || cell == SIZE * SIZE - 1) 
                {
                    continue;
                }

                int x = maze[cell].x;
                int y = maze[cell].y;
                sf::RectangleShape pathRect;
                pathRect.setFillColor(sf::Color(240, 240, 240));
                pathRect.setSize(sf::Vector2f(CELL_WIDTH, CELL_WIDTH));
                pathRect.setPosition(x, y);
                window.draw(pathRect);
            }
        }

       
        if (moved)
        {
            // progress++;
            moved = false;
        }
        int collectedTreasures = 0;

        int collectedGems = 0;
        if (currentPos == (SIZE * SIZE - 1)) 
        {
            
            makeMaze(maze, SIZE);
            currentPos = 0;
            maze[currentPos].isActive = true;

            handleGameWon(window, music, gameWonSprite, font);
        }


        if (maze[currentPos].hasmonster)
        {
            totalScore -= 5; 
            maze[currentPos].hasmonster = false; 
        }

        if (currentPos == (SIZE * SIZE - 1)) 
        {
            handleGameWon(window, music, gameWonSprite, font);
           
            makeMaze(maze, SIZE);
            currentPos = 0;
            maze[currentPos].isActive = true;
        }

        window.clear(sf::Color(13, 2, 33));
        window.draw(background);
        for (int i = 0; i < SIZE * SIZE; i++) 
        {
            maze[i].draw(&window, obstacleTexture);
        }
        for (int i = 0; i < SIZE * SIZE; i++)
        {
            maze[i].drawMonster(&window, monsterTexture); 
        }
        for (auto& pair : treasureMap)
        {
            Treasure* treasure = pair.second;
            if (!treasure->isCollected)
            {
                sf::Sprite treasureSprite(treasureTexture);
                treasureSprite.setScale(0.10f, 0.10f);
                treasureSprite.setPosition(treasure->x, treasure->y);
                window.draw(treasureSprite);
            }
        }
        for (auto& pair : gemMap) 
        {
            Gem* gem = pair.second;
            if (!gem->isCollected)
            {
                maze[pair.first].drawGem(&window, gemTexture);
            }
        }

        for (const auto& pair : treasureMap) 
        {
            if (pair.second->isCollected)
            {
                collectedTreasures++;

            }

        }
        for (const auto& pair : gemMap) 
        {
            if (pair.second->isCollected)
            {
                collectedGems++;
            }
        }

        treasureText.setString("Sword: " + std::to_string(collectedTreasures));
        gemText.setString("Gems: " + std::to_string(collectedGems));

        window.draw(treasureText);
        window.draw(gemText);


        currentSprite.setPosition(maze[currentPos].x, maze[currentPos].y);
        window.draw(currentSprite);

      
        finishSprite.setPosition(maze[SIZE * SIZE - 1].x, maze[SIZE * SIZE - 1].y);
        window.draw(finishSprite);

        if (highlightPath)
        {
            for (int cell : shortestPath) 
            {
               
                if (cell == currentPos || cell == SIZE * SIZE - 1) 
                {
                    continue;
                }

                int x = maze[cell].x;
                int y = maze[cell].y;
                sf::RectangleShape pathRect;
                pathRect.setFillColor(sf::Color(240, 240, 240));
                pathRect.setSize(sf::Vector2f(CELL_WIDTH, CELL_WIDTH));
                pathRect.setPosition(x, y);
                window.draw(pathRect);
            }
        }



        sf::Text progressText;
        progressText.setFont(font);
        progressText.setCharacterSize(18);
        progressText.setFillColor(sf::Color::Cyan);

      
        float textWidth = progressText.getLocalBounds().width;
        float textHeight = progressText.getLocalBounds().height;
        float windowWidth = window.getSize().x;
        progressText.setPosition(windowWidth - textWidth - 110, 40); 

        progressText.setString("Progress: % " + std::to_string(progress));
        window.draw(progressText);

        drawScore(window, font);


        window.display();
    }


    delete[] maze;
    destroyGraph(graph, SIZE);
    //return;
}

int main(int argc, char* argv[])
{
    sf::RenderWindow window(sf::VideoMode(630, 630), "Menu");
    Menu menu(window);

    while (true) 
    {
        menu.display_menu();

    }

    return 0;
}