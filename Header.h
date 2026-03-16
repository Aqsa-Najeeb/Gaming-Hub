#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <fstream>

using namespace sf;
using namespace std;

//GH

struct Star
{
    CircleShape shape;
    float speed;
};

class Button
{
    RectangleShape shape;
    Text text;

    Color idleColor = Color(20, 40, 100);
    Color hoverColor = Color(50, 90, 200);
    Color exitIdleColor = Color(0, 174, 57);
    Color exitHoverColor = Color(0, 138, 57);


    bool isExit = false;
    int offset = 0;

    public:

    Button(Font& font, string label, Vector2f size, Vector2f position);

    void update(RenderWindow& window);

    FloatRect globalBounds();

    bool isClicked(RenderWindow& window, const Event& event);

    void draw(RenderWindow& window);
};

//Snake

void runSnake(RenderWindow& window);

//Tetris

void runTetris(RenderWindow& window);

//TicTacToe

void runTicTacToe(RenderWindow& window);


// Leaderboard Classes

struct Record
{
    string game;
    string player;
    int score;
};

class Leaderboard
{
    static const int MAX_RECORDS = 100;

    Record records[MAX_RECORDS];

    int recordCount = 0;

    public:

    void AddRecord(string game, string player, int score);

    void LoadFromFile();

    void SaveToFile();

    void SortRecords();

    void DisplayLeaderboard(RenderWindow& window);
};

extern Leaderboard leaderboard;