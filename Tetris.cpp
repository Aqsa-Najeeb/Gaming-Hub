#include "Header.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <ctime>

using namespace sf;

const int M = 20;
const int N = 10;
const int TILE = 32;

bool scoresAdd = false;

struct Point { int x, y; };

int field1[M][N] = { 0 };
int field2[M][N] = { 0 };

Point a1[4], b1[4];
Point a2[4], b2[4];

int figures[7][4] =
{
    {1,3,5,7},
    {2,4,5,7},
    {3,5,4,6},
    {3,5,4,7},
    {2,3,5,7},
    {3,5,7,6},
    {2,3,4,5}
};

bool check(Point a[4], int field[M][N])
{
    for (int i = 0; i < 4; i++)
    {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
            return false;

        if (a[i].y >= 0 && field[a[i].y][a[i].x])
            return false;
    }

    return true;
}

void spawnPiece(Point a[4])
{
    int n = rand() % 7;

    for (int i = 0; i < 4; i++)
    {
        a[i].x = figures[n][i] % 2 + N / 2 - 1;
        a[i].y = figures[n][i] / 2 - 1;
    }
}

int clearLines(int field[M][N])
{
    int lines = 0;

    for (int i = M - 1; i >= 0; i--)
    {
        bool full = true;

        for (int j = 0; j < N; j++)
            if (!field[i][j]) full = false;

        if (full)
        {
            lines++;

            for (int k = i; k > 0; k--)
                for (int j = 0; j < N; j++)
                    field[k][j] = field[k - 1][j];

            for (int j = 0; j < N; j++)
                field[0][j] = 0;

            i++;
        }
    }

    return lines;
}

bool gameOver(int field[M][N])
{
    for (int j = 0; j < N; j++)
        if (field[0][j]) return true;

    return false;
}

void resetGame(int& score1, int& score2, bool& p1Over, bool& p2Over)
{
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
        {
            field1[i][j] = 0;
            field2[i][j] = 0;
        }

    score1 = 0;
    score2 = 0;

    p1Over = false;
    p2Over = false;
}

void runTetris(RenderWindow& window)
{
    scoresAdd = false;
    srand(time(0));

    Font font;
    font.openFromFile("Fonts/arial.ttf");

    const float WIN_W = (float)window.getSize().x;
    const float WIN_H = (float)window.getSize().y;

    Text scoreText(font);
    scoreText.setCharacterSize(28);

    int score1 = 0;
    int score2 = 0;

    bool p1Over = false;
    bool p2Over = false;

    spawnPiece(a1);
    spawnPiece(a2);

    Clock clock;
    float timer = 0;
    float delay = 0.3f;

    float moveTimer1 = 0.f;
    float moveTimer2 = 0.f;
    const float MOVE_DELAY = 0.15f;

    RectangleShape block(Vector2f(TILE - 2, TILE - 2));
    RectangleShape grid(Vector2f(TILE - 1, TILE - 1));
    grid.setFillColor(Color(40, 40, 40));

    int offset1 = 500;   // shifted right
    int offset2 = 1100;  // shifted right

    while (window.isOpen())
    {
        float time = clock.restart().asSeconds();
        timer += time;

        while (const optional<Event> event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Q)) return;

        if (!p1Over && !p2Over)
        {
            moveTimer1 += time;

            if (moveTimer1 >= MOVE_DELAY)
            {
                if (Keyboard::isKeyPressed(Keyboard::Key::A))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        b1[i] = a1[i];
                        a1[i].x--;
                    }

                    if (!check(a1, field1)) for (int i = 0; i < 4; i++)
                        a1[i] = b1[i];

                    moveTimer1 = 0.f;
                }
                if (Keyboard::isKeyPressed(Keyboard::Key::D))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        b1[i] = a1[i];
                        a1[i].x++;
                    }

                    if (!check(a1, field1)) for (int i = 0; i < 4; i++) a1[i] = b1[i];

                    moveTimer1 = 0.f;
                }
            }

            moveTimer2 += time;

            if (moveTimer2 >= MOVE_DELAY)
            {
                if (Keyboard::isKeyPressed(Keyboard::Key::Left))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        b2[i] = a2[i];
                        a2[i].x--;
                    }

                    if (!check(a2, field2))
                        for (int i = 0; i < 4; i++)
                            a2[i] = b2[i];

                    moveTimer2 = 0.f;
                }
                if (Keyboard::isKeyPressed(Keyboard::Key::Right))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        b2[i] = a2[i];
                        a2[i].x++;
                    }

                    if (!check(a2, field2))
                        for (int i = 0; i < 4; i++)
                            a2[i] = b2[i];

                    moveTimer2 = 0.f;
                }
            }

            //speed up falling
            float currentDelay = delay;

            if (Keyboard::isKeyPressed(Keyboard::Key::S))    currentDelay = 0.05f;
            if (Keyboard::isKeyPressed(Keyboard::Key::Down)) currentDelay = 0.05f;

            if (timer > currentDelay)
            {
                timer = 0;

                for (int i = 0; i < 4; i++) { b1[i] = a1[i]; a1[i].y++; }
                if (!check(a1, field1))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (b1[i].y >= 0)
                        {
                            field1[b1[i].y][b1[i].x] = 1;
                        }

                    }

                    score1 += clearLines(field1);
                    spawnPiece(a1);

                    if (!check(a1, field1))
                        p1Over = true;
                }

                for (int i = 0; i < 4; i++) { b2[i] = a2[i]; a2[i].y++; }
                if (!check(a2, field2))
                {
                    for (int i = 0; i < 4; i++)
                    {
                        if (b2[i].y >= 0)
                        {
                            field2[b2[i].y][b2[i].x] = 1;
                        }
                    }

                    score2 += clearLines(field2);
                    spawnPiece(a2);

                    if (!check(a2, field2))
                        p2Over = true;
                }
            }
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
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                grid.setPosition(Vector2f(j * TILE + offset1, i * TILE + 150));
                window.draw(grid);
                grid.setPosition(Vector2f(j * TILE + offset2, i * TILE + 150));
                window.draw(grid);
            }

        // borders
        RectangleShape border(Vector2f(N * TILE, M * TILE));
        border.setFillColor(Color::Transparent);
        border.setOutlineThickness(2.f);
        border.setOutlineColor(Color(80, 200, 255));

        border.setPosition(Vector2f(offset1, 150));
        window.draw(border);
        border.setPosition(Vector2f(offset2, 150));
        window.draw(border);

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (field1[i][j])
                {
                    block.setFillColor(Color(200, 60, 100));
                    block.setPosition(Vector2f(j * TILE + offset1, i * TILE + 150));
                    window.draw(block);
                }
                if (field2[i][j])
                {
                    block.setFillColor(Color(50, 170, 140));
                    block.setPosition(Vector2f(j * TILE + offset2, i * TILE + 150));
                    window.draw(block);
                }
            }

        for (int i = 0; i < 4; i++)
        {
            block.setFillColor(Color(255, 80, 120)); 
            block.setPosition(Vector2f(a1[i].x * TILE + offset1, a1[i].y * TILE + 150));
            window.draw(block);

            block.setFillColor(Color(80, 220, 180)); 
            block.setPosition(Vector2f(a2[i].x * TILE + offset2, a2[i].y * TILE + 150));
            window.draw(block);
        }

        // Title
        Text title(font, "TETRIS", 34);
        title.setLetterSpacing(2.f);
        title.setFillColor(Color(160, 220, 255));
        FloatRect titleB = title.getLocalBounds();
        title.setOrigin({ titleB.size.x / 2.f, titleB.size.y / 2.f }); //centering
        title.setPosition({ window.getSize().x / 2.f, 95.f });
        window.draw(title);

        // Scores
        scoreText.setFillColor(Color(255, 80, 120));
        scoreText.setString("Player 1: " + to_string(score1));
        scoreText.setPosition(Vector2f(offset1, 100));
        window.draw(scoreText);

        scoreText.setFillColor(Color(80, 220, 180));
        scoreText.setString("Player 2: " + to_string(score2));
        scoreText.setPosition(Vector2f(offset2, 100));
        window.draw(scoreText);

        if ((p1Over || p2Over) && !scoresAdd)
        {
            if (p1Over && p2Over)
            {
                leaderboard.AddRecord("Tetris", "Player1", 0);
                leaderboard.AddRecord("Tetris", "Player2", 0);
            }
            else if (p1Over)
            {
                leaderboard.AddRecord("Tetris", "Player1", 0);
                leaderboard.AddRecord("Tetris", "Player2", 5);
            }
            else
            {
                leaderboard.AddRecord("Tetris", "Player1", 5);
                leaderboard.AddRecord("Tetris", "Player2", 0);
            }
            scoresAdd = true;
        }

        if (p1Over || p2Over)
        {
            string resultStr;
            Color resultColor;

            if (p1Over && p2Over)
            {
                resultStr = "  DRAW!  ";
                resultColor = Color(220, 200, 60);
            }
            else if (p1Over)
            {
                resultStr = "Player 2 Wins!";
                resultColor = Color(80, 220, 180);
            }
            else
            {
                resultStr = "Player 1 Wins!";
                resultColor = Color(255, 80, 120);
            }

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

            if (Keyboard::isKeyPressed(Keyboard::Key::R))
            {
                resetGame(score1, score2, p1Over, p2Over);
                spawnPiece(a1);
                spawnPiece(a2);
                scoresAdd = false;
            }
        }

        window.display();
    }
}