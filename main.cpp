#include "SFML/Graphics.hpp"
#include <array>
#include <cstdlib>
//#include "global.hpp"

const int PARTICLE = 3;
const int STROKE = 5;
const int SCREEN_HEIGHT = 800;
const int SCREEN_WIDTH = 1200;
const float TIME_STEP = 0.9f; 

void render(const std::array<std::array<bool, SCREEN_HEIGHT/PARTICLE>, SCREEN_WIDTH/PARTICLE>& state, sf::RenderWindow& window) {
    for (int i = 0; i < SCREEN_WIDTH/PARTICLE; ++i) {
        for (int j = 0; j < SCREEN_HEIGHT/PARTICLE; ++j) {
            if (state[i][j]) {
                sf::RectangleShape on(sf::Vector2f(PARTICLE, PARTICLE));
                on.setFillColor(sf::Color::Blue);
                on.setPosition(i*PARTICLE, j * PARTICLE);
                window.draw(on);
            }
        }
    }
}

void handleInput(std::array<std::array<bool, SCREEN_HEIGHT/PARTICLE>, SCREEN_WIDTH/PARTICLE>& state, sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            sf::Event mouseEvent;
            while (window.pollEvent(mouseEvent)) {
                if (mouseEvent.type == sf::Event::MouseButtonReleased && mouseEvent.mouseButton.button == sf::Mouse::Left) {
                    break;
                }
            }

            int i = sf::Mouse::getPosition(window).x / PARTICLE;
            int j = sf::Mouse::getPosition(window).y / PARTICLE;
            //state[i][j] = 1;

            for (int a = 0; a < STROKE; ++a) {
                for (int b = 0; b < STROKE; ++b) {
                    state[i+a][j+b] = 1;
                }
            }
        }
    }
}

void update(std::array<std::array<bool, SCREEN_HEIGHT/PARTICLE>, SCREEN_WIDTH/PARTICLE>& state) {
    static float elapsedTime = 0.0f; 
    elapsedTime += TIME_STEP; 
    
    if (elapsedTime >= 1.0f) {
        for (int i = 0; i < SCREEN_WIDTH / PARTICLE; ++i) {
            for (int j = SCREEN_HEIGHT / PARTICLE - 2; j >= 0; --j) { 
                if (state[i][j]) {
                    if (!state[i][j + 1]) { 
                        state[i][j] = false;
                        state[i][j + 1] = true;
                    } else if (!state[i-1][j+1] || !state[i+1][j+1]) {
                        if (!state[i-1][j+1] && !state[i+1][j+1]) {
                            if (rand() % 2 == 0) {
                                state[i-1][j + 1] = true;
                            } else {
                                state[i+1][j + 1] = true;
                            }
                        } else if (!state[i-1][j+1]) {
                            state[i-1][j + 1] = true;
                        } else {
                            state[i+1][j + 1] = true;
                        }
                        state[i][j] = false;
                    }
                }
            }
        }
    }
}


int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Sand!");

    std::array<std::array<bool, SCREEN_HEIGHT/PARTICLE>, SCREEN_WIDTH/PARTICLE> state;
     
     for (int i = 0; i < SCREEN_WIDTH/PARTICLE; ++i) {
        for (int j = 0; j < SCREEN_HEIGHT/PARTICLE; ++j) {
            state[i][j] = 0;
        }
     }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        if (event.type == sf::Event::MouseButtonPressed) {
            handleInput(state, event, window);
        }
        window.clear();
        update(state);
        render(state, window);
        window.display();
       
    }
}