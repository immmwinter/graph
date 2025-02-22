#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <ctime>
using namespace std;

class StockGraph {
private:
    float width;
    float height;
    const int numPoints = 50;
    int newsEffect;
    vector<float> openPrices, closePrices;
    random_device rd;
    mt19937 gen;
    normal_distribution<float> distribution;
    vector<sf::RectangleShape> candles;

public:
    StockGraph(float w, float h) :
        width(w),
        height(h),
        newsEffect(0),
        gen(rd()),
        distribution(0.0f, 1.0f) {
        resetGraph();
    }

    void setNewsEffect(int effect) {
        newsEffect = effect;
        float mean = float(newsEffect) * 0.3f;
        float stddev = 2.0f;
        distribution = normal_distribution<float>(mean, stddev);
    }

    void resetGraph() {
        openPrices.clear();
        closePrices.clear();
        candles.clear();

        float basePrice = height / 2;
        float currentPrice = basePrice;
        float candleWidth = (width / numPoints) * 0.8f;

        for (int i = 0; i < numPoints; i++) {
            float priceChange = distribution(gen) * (abs(newsEffect) + 5.0f);
            float open = currentPrice;
            float close = open + priceChange;

            // จำกัดราคาไม่ให้เกินขอบเขต
            close = min(max(close, height * 0.1f), height * 0.9f);

            openPrices.push_back(open);
            closePrices.push_back(close);

            currentPrice = close;

            // สร้างแท่งเทียน
            float candleHeight = abs(close - open);
            if (candleHeight < 1.0f) candleHeight = 1.0f; // ความสูงขั้นต่ำ 1 pixel

            sf::RectangleShape candle(sf::Vector2f(candleWidth, candleHeight));

            // กำหนดตำแหน่งและสี
            if (close > open) { // ราคาขึ้น
                candle.setPosition(i * (width / numPoints), height - open);
                candle.setFillColor(sf::Color::Green);
            }
            else { // ราคาลง
                candle.setPosition(i * (width / numPoints), height - close);
                candle.setFillColor(sf::Color::Red);
            }

            candles.push_back(candle);
        }
    }

    void drawGraph(sf::RenderWindow& window, const sf::RectangleShape& chartArea) {
        sf::View currentView = window.getView();

        sf::View graphView;
        sf::Vector2f chartPos = chartArea.getPosition();
        sf::Vector2f chartSize = chartArea.getSize();

        graphView.reset(sf::FloatRect(0, 0, width, height));
        graphView.setViewport(sf::FloatRect(
            chartPos.x / window.getSize().x,
            chartPos.y / window.getSize().y,
            chartSize.x / window.getSize().x,
            chartSize.y / window.getSize().y
        ));

        window.setView(graphView);

        // วาดเฉพาะแท่งเทียน
        for (const auto& candle : candles) {
            window.draw(candle);
        }

        window.setView(currentView);
    }
};
