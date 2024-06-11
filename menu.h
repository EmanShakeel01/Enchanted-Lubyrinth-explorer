#pragma once
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include<fstream>


void game_run();

class Menu
{
public:
    Menu(sf::RenderWindow& window);

    void display_menu();



    sf::RenderWindow& window;
    sf::Event event;

    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    void handle_event();
    void display_help();
    void display_game_page(); // New function declaration
    void cont();
};

Menu::Menu(sf::RenderWindow& window) : window(window)
{
    if (!font.loadFromFile("Nasa21-l23X.ttf"))
    {
        std::cout << "Error loading font file." << std::endl;
    }

    if (!backgroundTexture.loadFromFile("menu.jpeg"))
    {
        std::cout << "Error loading background image." << std::endl;
    }

    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setScale(static_cast<float>(window.getSize().x) / backgroundSprite.getLocalBounds().width,
        static_cast<float>(window.getSize().y) / backgroundSprite.getLocalBounds().height);
}

void Menu::display_menu()
{
    while (window.isOpen())
    {

        while (window.pollEvent(event))
        {
            handle_event();
        }

        window.clear();
        window.draw(backgroundSprite);
        window.display();
    }
}

void Menu::handle_event()
{
    if (event.type == sf::Event::Closed)
    {
        window.close();
    }

    if (event.type == sf::Event::KeyPressed)
    {
        if (event.key.code == sf::Keyboard::Num1)
        {

            std::cout << "Starting game\n";
            display_game_page();
            return;
        }
        else if (event.key.code == sf::Keyboard::Num2)
        {

            display_help();
        }
        else if (event.key.code == sf::Keyboard::Num3)
        {

            std::cout << "exit game\n";
            window.close();
            return;
        }

    }
}

void Menu::display_help()
{
    sf::Texture helpBackgroundTexture;
    if (!helpBackgroundTexture.loadFromFile("settings.jpeg"))
    {
        std::cout << "Error loading help background image." << std::endl;
        return;
    }

    sf::Sprite helpBackgroundSprite(helpBackgroundTexture);
    helpBackgroundSprite.setTexture(helpBackgroundTexture);
    helpBackgroundSprite.setScale(static_cast<float>(window.getSize().x) / helpBackgroundSprite.getLocalBounds().width,
        static_cast<float>(window.getSize().y) / helpBackgroundSprite.getLocalBounds().height);
    window.clear();


    window.draw(helpBackgroundSprite);

    sf::Text helpText("Help:\n\nPress Up key for upward movement\n\nPress down key for downward movement\n\nPress right arrow -> to move right\n\n Press Left arrow key <- to move left\n\n Press Z/K to kill Monsters\n\n Press Space to remove Obstacles", font, 20);
    helpText.setPosition((window.getSize().x - helpText.getGlobalBounds().width) / 3.f, 80.f);
    helpText.setFillColor(sf::Color::White);
    window.draw(helpText);

    window.display();

    sf::Event helpEvent;
    while (window.waitEvent(helpEvent))
    {
        if (helpEvent.type == sf::Event::KeyPressed)
        {
            break;
        }
    }
}

void Menu::display_game_page()
{
    sf::Texture gameBackgroundTexture;
    if (!gameBackgroundTexture.loadFromFile("starto.jpeg")) {
        std::cout << "Error loading game background image." << std::endl;
        return;
    }

    sf::Sprite gameBackgroundSprite(gameBackgroundTexture);
    gameBackgroundSprite.setTexture(gameBackgroundTexture);
    gameBackgroundSprite.setScale(static_cast<float>(window.getSize().x) / gameBackgroundSprite.getLocalBounds().width,
        static_cast<float>(window.getSize().y) / gameBackgroundSprite.getLocalBounds().height);
    window.clear();

    window.draw(gameBackgroundSprite);

    sf::Font boldFont;
    if (!boldFont.loadFromFile("Nasa21-l23X.ttf"))
    {
        std::cout << "Error loading bold font file." << std::endl;
        return;
    }

    sf::Text enterNameText("Enter Your Name:", boldFont, 30);
    enterNameText.setFillColor(sf::Color::White);
    enterNameText.setStyle(sf::Text::Bold);

    enterNameText.setPosition((window.getSize().x - enterNameText.getGlobalBounds().width) / 4.f, (window.getSize().y - enterNameText.getGlobalBounds().height) / 3.f);

    window.draw(enterNameText);

    window.display();


    std::string playerName;
    sf::Event event;

    while (window.waitEvent(event))
    {
        if (event.type == sf::Event::TextEntered)
        {
            if (event.text.unicode == '\b')
            {
                if (!playerName.empty())
                    playerName.pop_back();
            }
            else if (event.text.unicode < 128)
            {
                playerName += static_cast<char>(event.text.unicode);
            }
        }
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
        {
            game_run();
            break;
        }


        window.clear();
        window.draw(gameBackgroundSprite);
        window.draw(enterNameText);


        sf::Text nameText(playerName, boldFont, 30);
        nameText.setFillColor(sf::Color::White);
        nameText.setStyle(sf::Text::Bold);
        nameText.setPosition((window.getSize().x - nameText.getGlobalBounds().width) / 2.f, enterNameText.getPosition().y + 50.f);
        window.draw(nameText);

        window.display();
    }


    std::ofstream file("player_name.txt");
    if (file.is_open()) {
        file << playerName;
        file.close();
    }
    else
    {
        std::cout << "Error saving player name to file." << std::endl;
    }



    sf::Text startText("[Start]", boldFont, 30);
    startText.setFillColor(sf::Color::Red);
    startText.setStyle(sf::Text::Bold);


    startText.setPosition((window.getSize().x - startText.getGlobalBounds().width) / 4.f, window.getSize().y - 80.f);


    sf::Text backText("[Back]", boldFont, 30);
    backText.setFillColor(sf::Color::Black);
    backText.setStyle(sf::Text::Bold);

    backText.setPosition((window.getSize().x - backText.getGlobalBounds().width) / 2.f, window.getSize().y - 80.f);


    window.draw(startText);
    window.draw(backText);

    window.display();

    sf::Event helpEvent;
    while (window.waitEvent(helpEvent))
    {
        if (helpEvent.type == sf::Event::KeyPressed)
        {
            break;
        }
    }
}
