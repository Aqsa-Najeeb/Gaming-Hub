#include "Header.h"
#include <SFML/Graphics.hpp>
#include <optional>

using namespace sf;
using namespace std;

const int CELL = 180;
const int BOARD_SIZE = 3 * CELL;

int board[3][3];
bool scoreAdded = false;

int checkWinner()
{
    for (int i = 0; i < 3; i++)
    {
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];
    return 0;
}

bool checkTie()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (board[i][j] == 0) return false;
    return true;
}

void resetBoard()
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = 0;
}

void drawLine(RenderWindow& window, Vector2f a, Vector2f b, Color color)
{
    float len = sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
    Vector2f unit = { (b.x - a.x) / len, (b.y - a.y) / len };
    float half = 3.f;
    Vector2f perp = { -unit.y * half, unit.x * half };

    ConvexShape seg(4);
    seg.setPoint(0, a + perp);
    seg.setPoint(1, a - perp);
    seg.setPoint(2, b - perp);
    seg.setPoint(3, b + perp);
    seg.setFillColor(color);
    window.draw(seg);
}

void drawCircle(RenderWindow& window, Vector2f center, float radius, Color color)
{
    CircleShape c(radius);
    c.setOrigin({ radius, radius });
    c.setPosition(center);
    c.setFillColor(Color::Transparent);
    c.setOutlineThickness(6.f);
    c.setOutlineColor(color);
    window.draw(c);
}

void drawGrid(RenderWindow& window, float ox, float oy)
{
    Color gridColor(80, 200, 255);

    for (int i = 1; i < 3; i++)
    {
        drawLine(window, { ox + i * CELL, oy }, { ox + i * CELL, oy + BOARD_SIZE }, gridColor);
        drawLine(window, { ox, oy + i * CELL }, { ox + BOARD_SIZE, oy + i * CELL }, gridColor);
    }
}

void drawSymbols(RenderWindow& window, float ox, float oy)
{
    float pad = 35.f;
    Color xColor(255, 80, 120);
    Color oColor(80, 220, 180);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (board[i][j] == 1)
            {
                Vector2f tl = { ox + j * CELL + pad, oy + i * CELL + pad };
                Vector2f br = { ox + j * CELL + CELL - pad, oy + i * CELL + CELL - pad };
                Vector2f tr = { ox + j * CELL + CELL - pad, oy + i * CELL + pad };
                Vector2f bl = { ox + j * CELL + pad, oy + i * CELL + CELL - pad };

                drawLine(window, tl, br, xColor);
                drawLine(window, tr, bl, xColor);
            }
            else if (board[i][j] == 2)
            {
                float cx = ox + j * CELL + CELL / 2.f;
                float cy = oy + i * CELL + CELL / 2.f;
                drawCircle(window, { cx, cy }, CELL / 2.f - pad, oColor);
            }
        }
    }
}

void runTicTacToe(RenderWindow& window)
{
    scoreAdded = false;
    int  currentPlayer = 1;
    bool gameOver = false;
    bool tieGame = false;
    int  winner = 0;

    Font font;
    font.openFromFile("Fonts/arial.ttf");

    const float WIN_W = (float)window.getSize().x;
    const float WIN_H = (float)window.getSize().y;

    float boardOffsetX = (window.getSize().x - BOARD_SIZE) / 2.f;
    float boardOffsetY = (window.getSize().y - BOARD_SIZE) / 2.f + 20.f;

    RectangleShape cellBg(Vector2f(CELL - 2, CELL - 2));
    RectangleShape hoverBg(Vector2f(CELL - 2, CELL - 2));
    hoverBg.setFillColor(Color(40, 60, 120));

    resetBoard();

    while (window.isOpen())
    {
        Vector2f mousePos = window.mapPixelToCoords(Mouse::getPosition(window));

        while (const optional<Event> event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
                return;
            }

            if (const auto* mouse = event->getIf<Event::MouseButtonPressed>())
            {
                if (mouse->button == Mouse::Button::Left && !gameOver)
                {
                    int x = (mousePos.x - boardOffsetX) / CELL;
                    int y = (mousePos.y - boardOffsetY) / CELL;

                    if (x >= 0 && x < 3 && y >= 0 && y < 3 && board[y][x] == 0)
                    {
                        board[y][x] = currentPlayer;
                        winner = checkWinner();

                        if (winner != 0)
                            gameOver = true;
                        else if (checkTie())
                        {
                            gameOver = true;
                            tieGame = true;
                        }
                        else
                            currentPlayer = (currentPlayer == 1) ? 2 : 1;

                        if (gameOver && !scoreAdded)
                        {
                            if (winner == 1)
                            {
                                leaderboard.AddRecord("TicTacToe", "PlayerX", 1);
                                leaderboard.AddRecord("TicTacToe", "PlayerO", 0);
                            }
                            else if (winner == 2)
                            {
                                leaderboard.AddRecord("TicTacToe", "PlayerX", 0);
                                leaderboard.AddRecord("TicTacToe", "PlayerO", 1);
                            }
                            else
                            {
                                leaderboard.AddRecord("TicTacToe", "PlayerX", 0);
                                leaderboard.AddRecord("TicTacToe", "PlayerO", 0);
                            }
                            scoreAdded = true;
                        }
                    }
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Q))
            return;

        if (gameOver && Keyboard::isKeyPressed(Keyboard::Key::R))
        {
            resetBoard();
            currentPlayer = 1;
            gameOver = false;
            tieGame = false;
            winner = 0;
            scoreAdded = false;
        }

        window.clear(Color(15, 15, 25));

        for (int y = 0; y < (int)WIN_H; y += 40)
        {
            VertexArray line(PrimitiveType::Lines, 2);
            line[0] = Vertex(Vector2f(0, (float)y), Color(255, 255, 255, 8));
            line[1] = Vertex(Vector2f(WIN_W, (float)y), Color(255, 255, 255, 8));

            window.draw(line);
        }

        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                float cx = boardOffsetX + j * CELL + 1.f;
                float cy = boardOffsetY + i * CELL + 1.f;

                bool hovered = false;
                if (!gameOver && board[i][j] == 0)
                {
                    if (mousePos.x >= boardOffsetX + j * CELL &&
                        mousePos.x < boardOffsetX + (j + 1) * CELL &&
                        mousePos.y >= boardOffsetY + i * CELL &&
                        mousePos.y < boardOffsetY + (i + 1) * CELL)
                        hovered = true;
                }

                if (hovered)
                {
                    hoverBg.setPosition({ cx, cy });
                    window.draw(hoverBg);
                }
                else
                {
                    cellBg.setFillColor(Color(12, 18, 55));
                    cellBg.setPosition({ cx, cy });
                    window.draw(cellBg);
                }
            }
        }

        //grid lines
        drawGrid(window, boardOffsetX, boardOffsetY);

        //border
        RectangleShape border(Vector2f(BOARD_SIZE, BOARD_SIZE));
        border.setPosition({ boardOffsetX, boardOffsetY });
        border.setFillColor(Color::Transparent);
        border.setOutlineThickness(2.f);
        border.setOutlineColor(Color(80, 200, 255));
        window.draw(border);

        drawSymbols(window, boardOffsetX, boardOffsetY);

        // Title
        Text title(font, "TIC  TAC  TOE", 34);
        title.setLetterSpacing(2.f);
        title.setFillColor(Color(160, 220, 255));
        FloatRect tb = title.getLocalBounds();
        title.setOrigin({ tb.size.x / 2.f, tb.size.y / 2.f });
        title.setPosition({ window.getSize().x / 2.f, boardOffsetY - 52.f });
        window.draw(title);

        if (!gameOver)
        {
            Color turnColor = (currentPlayer == 1) ? Color(255, 80, 120) : Color(80, 220, 180);
            string turnStr = (currentPlayer == 1) ? "Player X's Turn" : "Player O's Turn";

            Text turnTxt(font, turnStr, 24);
            turnTxt.setFillColor(turnColor);
            FloatRect tt = turnTxt.getLocalBounds();
            turnTxt.setOrigin({ tt.size.x / 2.f, tt.size.y / 2.f });
            turnTxt.setPosition({ window.getSize().x / 2.f, boardOffsetY + BOARD_SIZE + 44.f });
            window.draw(turnTxt);
        }

        if (gameOver)
        {
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

            string resultStr;
            Color  resultColor;
            if (tieGame)
            {
                resultStr = "  DRAW!  ";
                resultColor = Color(220, 200, 60);
            }
            else if (winner == 1)
            {
                resultStr = "Player X Wins!";
                resultColor = Color(255, 80, 120);
            }
            else
            {
                resultStr = "Player O Wins!";
                resultColor = Color(80, 220, 180);
            }

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

        window.display();
    }
}