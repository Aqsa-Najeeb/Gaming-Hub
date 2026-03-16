#include "Header.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <ctime>

using namespace sf;
using namespace std;

const int N = 20;
const int M = 20;

int cellSize;

struct Snake { int x, y; };  //Snake coordinates
struct Fruit { int x, y; };

Snake s1[100], s2[100]; //size of snake
Fruit f1, f2;

//0 = Down
//1 = Left
//2 = Right
//3 = Up

int num1 = 4, num2 = 4;    //snake size is 4 cells
int dir1 = 2, dir2 = 2;    //snake direction is right

bool scoresAdded = false;

void spawnFruit(Fruit& f)
{
    f.x = rand() % N;
    f.y = rand() % M;
}

void Tick(Snake s[], int& num, int dir, Fruit& fruit)
{
    for (int i = num; i > 0; i--)
    {
        s[i] = s[i - 1];
    }

    if (dir == 0)
        s[0].y++;

    if (dir == 1)
        s[0].x--;

    if (dir == 2)
        s[0].x++;

    if (dir == 3)
        s[0].y--;

    if (s[0].x == fruit.x && s[0].y == fruit.y)
    {
        num++;
        spawnFruit(fruit);
    }
}

bool hitWall(Snake s[])
{
    if (s[0].x < 0 || s[0].x >= N || s[0].y < 0 || s[0].y >= M)
        return true;

    return false;
}

void resetGame()
{
    num1 = 4;
    num2 = 4;

    dir1 = 2;
    dir2 = 2;

    for (int i = 0; i < num1; i++)
    {
        s1[i] = { 5 - i, 5 };
        s2[i] = { 5 - i, 5 };
    }

    spawnFruit(f1);
    spawnFruit(f2);
}

void runSnake(RenderWindow& window)
{
    bool gameOver = false;
    int winner = 0;
    scoresAdded = false;
    srand(time(0));

    cellSize = window.getSize().y / (M + 6);

    const float WIN_W = (float)window.getSize().x;
    const float WIN_H = (float)window.getSize().y;

    int boardW = N * cellSize;
    int boardH = M * cellSize;

    int gap = 80;

    int totalW = boardW * 2 + gap;

    int offsetX = (window.getSize().x - totalW) / 2;
    int offsetY = (window.getSize().y - boardH) / 2;

    int board1X = offsetX;
    int board2X = offsetX + boardW + gap;

    RectangleShape cell(Vector2f(cellSize - 1, cellSize - 1));

    // player 1 snake
    RectangleShape body1(Vector2f(cellSize - 2, cellSize - 2));
    RectangleShape head1(Vector2f(cellSize - 2, cellSize - 2));
    body1.setFillColor(Color(200, 60, 100));
    head1.setFillColor(Color(255, 80, 120));

    // player 2 snake
    RectangleShape body2(Vector2f(cellSize - 2, cellSize - 2));
    RectangleShape head2(Vector2f(cellSize - 2, cellSize - 2));
    body2.setFillColor(Color(50, 170, 140));
    head2.setFillColor(Color(80, 220, 180));

    CircleShape fruit(cellSize / 2 - 2);
    fruit.setFillColor(Color(220, 200, 60));

    Font font("Fonts/arial.ttf");

    Clock clock;
    float timer = 0;
    float delay = 0.12;

    resetGame();

    while (window.isOpen())
    {
        float time = clock.restart().asSeconds();
        timer += time;

        while (const optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return;
            }
        }

        if (!gameOver)
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::A)) dir1 = 1;
            if (Keyboard::isKeyPressed(Keyboard::Key::D)) dir1 = 2;
            if (Keyboard::isKeyPressed(Keyboard::Key::W)) dir1 = 3;
            if (Keyboard::isKeyPressed(Keyboard::Key::S)) dir1 = 0;

            if (Keyboard::isKeyPressed(Keyboard::Key::Left)) dir2 = 1;
            if (Keyboard::isKeyPressed(Keyboard::Key::Right)) dir2 = 2;
            if (Keyboard::isKeyPressed(Keyboard::Key::Up)) dir2 = 3;
            if (Keyboard::isKeyPressed(Keyboard::Key::Down)) dir2 = 0;

            if (timer > delay)
            {
                timer = 0;

                Tick(s1, num1, dir1, f1);
                Tick(s2, num2, dir2, f2);

                if (hitWall(s1))
                {
                    gameOver = true;
                    winner = 2;
                }

                if (hitWall(s2))
                {
                    gameOver = true;
                    winner = 1;
                }
            }
        }
        else
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::R))
            {
                resetGame();
                gameOver = false;
                winner = 0;
                scoresAdded = false;
            }

            if (Keyboard::isKeyPressed(Keyboard::Key::Q))
                return;
        }

        window.clear(Color(15, 15, 25));

        for (int y = 0; y < (int)WIN_H; y += 40)
        {
            VertexArray line(PrimitiveType::Lines, 2);
            line[0] = Vertex(Vector2f(0, (float)y), Color(255, 255, 255, 8));
            line[1] = Vertex(Vector2f(WIN_W, (float)y), Color(255, 255, 255, 8));

            window.draw(line);
        }

        // board cells
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                cell.setFillColor((i + j) % 2 ? Color(18, 26, 70) : Color(12, 18, 55));

                cell.setPosition({ float(board1X + i * cellSize), float(offsetY + j * cellSize) });
                window.draw(cell);

                cell.setPosition({ float(board2X + i * cellSize), float(offsetY + j * cellSize) });
                window.draw(cell);
            }
        }

        // borders
        RectangleShape border(Vector2f(boardW, boardH));
        border.setFillColor(Color::Transparent);
        border.setOutlineThickness(2.f);
        border.setOutlineColor(Color(80, 200, 255));

        border.setPosition({ float(board1X), float(offsetY) });
        window.draw(border);

        border.setPosition({ float(board2X), float(offsetY) });
        window.draw(border);

        for (int i = 0; i < num1; i++)
        {
            RectangleShape& part = (i == 0 ? head1 : body1);
            part.setPosition({ float(board1X + s1[i].x * cellSize), float(offsetY + s1[i].y * cellSize) });
            window.draw(part);
        }

        for (int i = 0; i < num2; i++)
        {
            RectangleShape& part = (i == 0 ? head2 : body2);
            part.setPosition({ float(board2X + s2[i].x * cellSize), float(offsetY + s2[i].y * cellSize) });
            window.draw(part);
        }

        fruit.setPosition({ float(board1X + f1.x * cellSize), float(offsetY + f1.y * cellSize) });
        window.draw(fruit);

        fruit.setPosition({ float(board2X + f2.x * cellSize), float(offsetY + f2.y * cellSize) });
        window.draw(fruit);

        // Scores
        Text score1Txt(font, "Player 1: " + to_string(num1 - 4), 24);
        score1Txt.setFillColor(Color(255, 80, 120));
        FloatRect s1b = score1Txt.getLocalBounds();
        score1Txt.setOrigin({ s1b.size.x / 2.f, s1b.size.y / 2.f });
        score1Txt.setPosition({ float(board1X + boardW / 2), float(offsetY - 36) });
        window.draw(score1Txt);

        Text score2Txt(font, "Player 2: " + to_string(num2 - 4), 24);
        score2Txt.setFillColor(Color(80, 220, 180));
        FloatRect s2b = score2Txt.getLocalBounds();
        score2Txt.setOrigin({ s2b.size.x / 2.f, s2b.size.y / 2.f });
        score2Txt.setPosition({ float(board2X + boardW / 2), float(offsetY - 36) });
        window.draw(score2Txt);

        // Title
        Text title(font, "SNAKE", 34);
        title.setLetterSpacing(2.f);
        title.setFillColor(Color(160, 220, 255));
        FloatRect tb = title.getLocalBounds();
        title.setOrigin({ tb.size.x / 2.f, tb.size.y / 2.f });
        title.setPosition({ window.getSize().x / 2.f, float(offsetY - 36) });
        window.draw(title);

        if (gameOver)
        {
            string resultStr = (winner == 1) ? "Player 1 Wins!" : "Player 2 Wins!";
            
            Color resultColor = (winner == 1) ? Color(255, 80, 120) : Color(80, 220, 180);

            RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
            overlay.setFillColor(Color(0, 0, 0, 160));
            window.draw(overlay);

            float pw = 420.f, ph = 170.f;
            RectangleShape panel(Vector2f(pw, ph));
            panel.setOrigin({ pw / 2.f, ph / 2.f });
            panel.setPosition({ window.getSize().x / 2.f, window.getSize().y / 2.f });
            panel.setFillColor(Color(8, 14, 50));
            panel.setOutlineThickness(2.f);
            panel.setOutlineColor(Color(80, 200, 255));
            window.draw(panel);

            Text msg(font, resultStr, 46);
            msg.setFillColor(resultColor);
            FloatRect mr = msg.getLocalBounds();
            msg.setOrigin({ mr.size.x / 2.f, mr.size.y / 2.f });
            msg.setPosition({ window.getSize().x / 2.f, window.getSize().y / 2.f - 25.f });
            window.draw(msg);

            Text sub(font, "[R]  Replay          [Q]  Quit", 22);
            sub.setFillColor(Color(140, 150, 210));
            FloatRect sr = sub.getLocalBounds();
            sub.setOrigin({ sr.size.x / 2.f, sr.size.y / 2.f });
            sub.setPosition({ window.getSize().x / 2.f, window.getSize().y / 2.f + 48.f });
            window.draw(sub);
        }

        if (gameOver && !scoresAdded)
        {
            int score1 = 5 * (winner == 1 ? 1 : 0) + (num1 - 4);
            int score2 = 5 * (winner == 2 ? 1 : 0) + (num2 - 4);

            leaderboard.AddRecord("Snake", "Player1", score1);
            leaderboard.AddRecord("Snake", "Player2", score2);

            scoresAdded = true;
        }

        window.display();
    }
}