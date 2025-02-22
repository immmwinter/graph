#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>     
#include <cstdlib>   
#include <SFML/Graphics.hpp>
#include "test_button.h"
#include "graph.h"

using namespace std;

class Game {
public:
    Game() : window(sf::VideoMode(1080, 720), "Stock Chart"),
        btn1("Next", { 100, 50 }, 30, sf::Color::Green, sf::Color::Black),
        stockGraph(550.0f, 300.0f) {

        srand(time(0));  

        loadAssets();
        setupComponents();
        loadNews();

        btn1.setfont(font);
        btn1.setPosition({ 800, 700 });

        currentNewsIndex = 0;
        applyNewsEffect();
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            render();
        }
    }

private:
    sf::RenderWindow window;
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
    sf::Font font;
    sf::Text titleText;
    sf::Text newsText;
    sf::RectangleShape chartArea;
    Button btn1;
    StockGraph stockGraph;

    vector<string> newsList;
    vector<int> newsEffects;
    int currentNewsIndex;

    void loadAssets() {
        if (!bgTexture.loadFromFile("C:/Users/getbh/source/repos/last/2/element/pl.jpg")) {
            cout << "Failed to load background texture" << endl;
            system("pause");
        }

        if (!font.loadFromFile("C:/Users/getbh/source/repos/last/2/element/Rainbow Memories.otf")) {
            cout << "Failed to load font" << endl;
            system("pause");
        }
    }

    void setupComponents() {
        bgSprite.setTexture(bgTexture);
        bgSprite.setTextureRect(sf::IntRect(0, 0, 1080, 720));
        bgSprite.setPosition(0, 0);

        chartArea.setSize(sf::Vector2f(550.0f, 300.0f));
        chartArea.setPosition(200, 190);
        chartArea.setFillColor(sf::Color::White);
        chartArea.setOutlineThickness(1);
        chartArea.setOutlineColor(sf::Color::Black);

        titleText.setFont(font);
        titleText.setString("Stock Chart");
        titleText.setCharacterSize(50);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition(100, 50);

        
        titleText.setOutlineThickness(3);
        titleText.setOutlineColor(sf::Color::White);

        newsText.setFont(font);
        newsText.setCharacterSize(25);
        newsText.setFillColor(sf::Color::Black);
        newsText.setPosition(150, 600); 
        newsText.setOutlineThickness(1);
        newsText.setOutlineColor(sf::Color::White);
    }

    void loadNews() {
        ifstream file("C:/Users/getbh/source/repos/last/2/news/news.txt");
        if (!file) {
            cout << "Failed to open news file" << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find_last_of(" ");
            if (pos != string::npos) {
                string newsContent = line.substr(0, pos);
                string effectStr = line.substr(pos + 1);

                newsContent.erase(newsContent.find_last_not_of(" ") + 1);
                effectStr.erase(0, effectStr.find_first_not_of(" "));

                try {
                    int effect = stoi(effectStr);
                    newsList.push_back(newsContent);
                    newsEffects.push_back(effect);

                    
                    cout << "Loaded news: " << newsContent << " Effect: " << effect << endl;
                }
                catch (exception& e) {
                    cout << "Error parsing effect: " << effectStr << " in line: " << line << endl;
                }
            }
        }
        file.close();

        if (!newsList.empty()) {
            updateNewsText();
            applyNewsEffect();
        }
    }

    void updateNews() {
        if (!newsList.empty()) {
            currentNewsIndex = rand() % newsList.size();  
            applyNewsEffect();
        }
    }

    void applyNewsEffect() {
        if (!newsEffects.empty()) {
            stockGraph.setNewsEffect(newsEffects[currentNewsIndex]);
            stockGraph.resetGraph();
            updateNewsText();
        }
    }

    void updateNewsText() {
        if (!newsList.empty()) {
            newsText.setString("Market News:\n" + newsList[currentNewsIndex] + " (" + to_string(newsEffects[currentNewsIndex]) + ")");
        }
    }

    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (btn1.isMouseOver(window)) {
                    cout << "Next round" << endl;
                    updateNews();
                }
            }
        }
    }

    void render() {
        window.clear();
        window.draw(bgSprite);
        window.draw(titleText);
        window.draw(chartArea);
        stockGraph.drawGraph(window, chartArea);
        window.draw(newsText);
        btn1.drawTo(window);
        window.display();
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
