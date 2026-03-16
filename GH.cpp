#include "SFML/Graphics.hpp"
#include "Header.h"
using namespace sf;
using namespace std;

int main()
{

    const int STAR_COUNT = 120;
    Star stars[STAR_COUNT];

    for (int i = 0; i < STAR_COUNT; i++)
    {
        float x = rand() % 1920;
        float y = rand() % 1080;

        stars[i].shape.setRadius(rand() % 2 + 1);
        stars[i].shape.setPosition({ x, y });
        stars[i].shape.setFillColor(Color(255, 255, 255, rand() % 180 + 75));
        stars[i].speed = 15 + rand() % 35;
    }

    RenderWindow renderWindow(VideoMode::getDesktopMode(), "Gaming Hub");
    leaderboard.LoadFromFile();
    renderWindow.setVerticalSyncEnabled(true);

    Font font("Fonts/arial.ttf");
    Font Gfont("Fonts/Orbitron-VariableFont_wght.ttf");


    // drawn twice to fake an outline glow
    Text titleGlow(Gfont, "Gaming Hub", 138);
    titleGlow.setFillColor(Color(220, 60, 255, 80));
    titleGlow.setStyle(Text::Bold);
    FloatRect tgb = titleGlow.getLocalBounds();
    titleGlow.setOrigin({ tgb.size.x / 2.f, tgb.size.y / 2.f });
    titleGlow.setPosition({ 965.f, 145.f });

    Text title(Gfont, "Gaming Hub", 138);
    title.setFillColor(Color::White);
    title.setStyle(Text::Bold);
    FloatRect tb = title.getLocalBounds();
    title.setOrigin({ tb.size.x / 2.f, tb.size.y / 2.f });
    title.setPosition({ 960.f, 140.f });


    Texture backgroundTexture;
    backgroundTexture.loadFromFile("img/galaxy.jpg");
    Sprite background(backgroundTexture);
    background.setScale({ 1920.f / backgroundTexture.getSize().x, 1080.f / backgroundTexture.getSize().y });


    Button game1(Gfont, "Tic Tac Toe", { 320,260 }, { 280,340 });
    Button game2(Gfont, "Tetris", { 320,260 }, { 780,340 });
    Button game3(Gfont, "Snake Game", { 320,260 }, { 1280,340 });

    Button leaderboardB(Gfont, "Leaderboard", { 650,80 }, { 620,700 });
    Button exit(font, "Exit", { 300,80 }, { 1580,900 });


    //TictacToe Icon

    //Texture loading
    Texture TicIconTexture;
    TicIconTexture.loadFromFile("img/2r.png");
    TicIconTexture.setSmooth(true);
    Sprite TicIcon(TicIconTexture);

    //Scaling
    Vector2u ticSize = TicIconTexture.getSize();
    TicIcon.setScale({ 150.f / ticSize.x, 150.f / ticSize.y });
    
    //Setting position
    FloatRect Ticbtn = game1.globalBounds();
    FloatRect Ticicon = TicIcon.getGlobalBounds();

    float Ticgap = 20.f;

    TicIcon.setPosition({ Ticbtn.position.x + Ticbtn.size.x / 2.f - Ticicon.size.x / 2.f, Ticbtn.position.y + Ticgap });


    //Tetris Icon

    //Texture loading
    Texture TetrisIconTexture;
    TetrisIconTexture.loadFromFile("img/3r.png");
    TetrisIconTexture.setSmooth(true);
    Sprite TetrisIcon(TetrisIconTexture);

    //Scaling
    Vector2u tetrisSize = TetrisIconTexture.getSize();
    TetrisIcon.setScale({ 150.f / tetrisSize.x, 150.f / tetrisSize.y });

    //Setting position
    FloatRect Tetrisbtn = game2.globalBounds();
    FloatRect Tetrisicon = TetrisIcon.getGlobalBounds();

    float Tetrisgap = 20.f;

    TetrisIcon.setPosition({ Tetrisbtn.position.x + Tetrisbtn.size.x / 2.f - Tetrisicon.size.x / 2.f, Tetrisbtn.position.y + Tetrisgap });


    //Snake Icon

    //Texture loading
    Texture SnakeIconTexture;
    SnakeIconTexture.loadFromFile("img/4r.png");
    SnakeIconTexture.setSmooth(true);
    Sprite SnakeIcon(SnakeIconTexture);

    //Scaling
    Vector2u SnakeSize = SnakeIconTexture.getSize();
    SnakeIcon.setScale({ 150.f / SnakeSize.x, 150.f / SnakeSize.y });

    //Setting position
    FloatRect Snakebtn = game3.globalBounds();
    FloatRect Snakeicon = SnakeIcon.getGlobalBounds();

    float Snakegap = 20.f;

    SnakeIcon.setPosition({ Snakebtn.position.x + Snakebtn.size.x / 2.f - Snakeicon.size.x / 2.f, Snakebtn.position.y + Snakegap });



    Clock clock;

    while (renderWindow.isOpen())
    {
        float dt = clock.restart().asSeconds();

        game1.update(renderWindow);
        game2.update(renderWindow);
        game3.update(renderWindow);

        leaderboardB.update(renderWindow);
        exit.update(renderWindow);

        for (int i = 0; i < STAR_COUNT; i++)
        {
            stars[i].shape.move({ 0, stars[i].speed * dt });

            if (stars[i].shape.getPosition().y > 1080)
            {
                stars[i].shape.setPosition({ (float)(rand() % 1920), 0 });
            }
        }

        while (const optional<Event> event = renderWindow.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                renderWindow.close();
            }
            else if (game1.isClicked(renderWindow, *event))
            {
                runTicTacToe(renderWindow);
                while (renderWindow.pollEvent()) {}
                while (Keyboard::isKeyPressed(Keyboard::Key::Q)) {}
                break;
            }
            else if (game2.isClicked(renderWindow, *event))
            {
                runTetris(renderWindow);
                while (renderWindow.pollEvent()) {}
                while (Keyboard::isKeyPressed(Keyboard::Key::Q)) {}
                break;
            }
            else if (game3.isClicked(renderWindow, *event))
            {
                runSnake(renderWindow);
                while (renderWindow.pollEvent()) {}
                while (Keyboard::isKeyPressed(Keyboard::Key::Q)) {}
                break;
            }
            else if (exit.isClicked(renderWindow, *event))
            {
                renderWindow.close();
            }
            else if (leaderboardB.isClicked(renderWindow, *event))
            {
                leaderboard.DisplayLeaderboard(renderWindow);
                while (renderWindow.pollEvent()) {}
                while (Keyboard::isKeyPressed(Keyboard::Key::Q)) {}
                break;
            }
        }

        renderWindow.clear();

        renderWindow.draw(background);

        for (int i = 0; i < STAR_COUNT; i++)
        {
            renderWindow.draw(stars[i].shape);
        }

        renderWindow.draw(titleGlow);
        renderWindow.draw(title);

        game1.draw(renderWindow);
        renderWindow.draw(TicIcon);
        
        game2.draw(renderWindow);
        renderWindow.draw(TetrisIcon);

        game3.draw(renderWindow);
        renderWindow.draw(SnakeIcon);

        leaderboardB.draw(renderWindow);
        exit.draw(renderWindow);

        renderWindow.display();
    }
}