#include "Header.h"
#include <SFML/Graphics.hpp>
#include <optional>
#include <fstream>
#include <string>

Leaderboard leaderboard;

void Leaderboard::AddRecord(string game, string player, int score)
{
    for (int i = 0; i < recordCount; i++)
    {
        if ((records[i].game == game) && (records[i].player == player))
        {
            if (score > records[i].score)   //if score is higher than the one on the board only then will it be overwritten
                records[i].score = score;

            SortRecords();
            SaveToFile();
            return;
        }
    }

    if (recordCount >= MAX_RECORDS) 
        return;

    records[recordCount] = { game, player, score };
    recordCount++;
    SortRecords();
    SaveToFile();
}

void Leaderboard::LoadFromFile()
{
    ifstream file("leaderboard.txt");
    
    if (!file.is_open()) 
        return;

    recordCount = 0;
   
    while (recordCount < MAX_RECORDS && file >> records[recordCount].game >> records[recordCount].player >> records[recordCount].score)
    {
        recordCount++;
    }
    
    file.close();
    SortRecords();
}

void Leaderboard::SaveToFile()
{
    ofstream file("leaderboard.txt");

    for (int i = 0; i < recordCount; i++)
    {
        file << records[i].game << " " << records[i].player << " " << records[i].score << endl;
    }
        
}

void Leaderboard::SortRecords()
{
    Record temp;

    for (int i = 0; i < recordCount - 1; i++)
    {
        for (int j = i + 1; j < recordCount; j++)
        {
            if (records[j].score > records[i].score)
            {
                temp = records[i];
                records[i] = records[j];
                records[j] = temp;
            }
        }
    }
}

void Leaderboard::DisplayLeaderboard(RenderWindow& window)
{
    Font font;
    font.openFromFile("Fonts/arial.ttf");

    const float WIN_W = (float)window.getSize().x;
    const float WIN_H = (float)window.getSize().y;
    
    const float CARD_X = WIN_W / 2.f - 500.f;
    const float CARD_W = 1000.f;
    
    
    const string gameOrder[] = { "Snake", "Tetris", "TicTacToe" };
    const Color GAMES_COLOR[] = {Color(39, 174, 96), Color(41, 128, 185), Color(142, 68, 173)};
    
    const Color GOLD = Color(241, 196, 15);
    const Color SILVER = Color(189, 195, 199);

    bool viewing = true;

    while (viewing && window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            { 
                window.close(); 
                return; 
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Key::Q)) 
            viewing = false;

        window.clear(Color(15, 15, 25));

        for (int y = 0; y < (int)WIN_H; y += 40)
        {
            VertexArray line(PrimitiveType::Lines, 2);
            line[0] = Vertex(Vector2f(0, (float)y), Color(255, 255, 255, 8));
            line[1] = Vertex(Vector2f(WIN_W, (float)y), Color(255, 255, 255, 8));

            window.draw(line);
        }

        Text title(font, "Leaderboard", 72);
        title.setFillColor(Color::White);
        FloatRect tb = title.getLocalBounds();
        title.setOrigin({ tb.size.x / 2.f, 0 });  //centers the text
        title.setPosition({ WIN_W / 2.f, 40.f });
        window.draw(title);

        Text subtitle(font, "Top scores across all games", 28);
        subtitle.setFillColor(Color(160, 160, 180));
        FloatRect sb = subtitle.getLocalBounds();
        subtitle.setOrigin({ sb.size.x / 2.f, 0 });
        subtitle.setPosition({ WIN_W / 2.f, 120.f });
        window.draw(subtitle);

        auto drawGameCard = [&](const string& game, float startY, int colorIdx) -> float
            {
                int indices[MAX_RECORDS];
                int count = 0;

                for (int i = 0; i < recordCount; i++)
                    if (records[i].game == game)
                        indices[count++] = i;

                for (int i = 1; i < count; i++)
                {
                    int key = indices[i];
                    int j = i - 1;
                    
                    while (j >= 0 && records[indices[j]].score < records[key].score) //sorts by score
                    {
                        indices[j + 1] = indices[j];
                        j--;
                    }
                    
                    indices[j + 1] = key;
                }

                const int SHOW = count < 2 ? count : 2;
                const float ROW_H = 48.f;
                const float HEADER_H = 56.f;
                const float CARD_H = HEADER_H + (SHOW > 0 ? SHOW * ROW_H : ROW_H) + 16.f;

                RectangleShape card({ CARD_W, CARD_H });
                card.setPosition({ CARD_X, startY });
                card.setFillColor(Color(22, 22, 38));
                card.setOutlineThickness(1.f);
                card.setOutlineColor(Color(50, 50, 75));
                window.draw(card);

                RectangleShape accent({ 4.f, CARD_H });
                accent.setPosition({ CARD_X, startY });
                accent.setFillColor(GAMES_COLOR[colorIdx]);
                window.draw(accent);

                Text gTitle(font, game, 22);
                gTitle.setFillColor(GAMES_COLOR[colorIdx]);
                gTitle.setPosition({ CARD_X + 20.f, startY + 16.f });
                window.draw(gTitle);

                string countStr = std::to_string(SHOW) + (SHOW == 1 ? " entry" : " entries");
                Text gCount(font, countStr, 15);
                gCount.setFillColor(Color(120, 120, 150));
                gCount.setPosition({ CARD_X + 20.f + gTitle.getLocalBounds().size.x + 12.f, startY + 20.f });
                window.draw(gCount);

                if (SHOW == 0)
                {
                    Text empty(font, "No scores recorded yet", 18);
                    empty.setFillColor(Color(80, 80, 110));
                    FloatRect er = empty.getLocalBounds();
                    empty.setOrigin({ er.size.x / 2.f, 0 });
                    empty.setPosition({ CARD_X + CARD_W / 2.f, startY + HEADER_H + 14.f });
                    window.draw(empty);
                }
                else
                {
                    for (int i = 0; i < SHOW; i++)
                    {
                        float rowY = startY + HEADER_H + 12.f + i * ROW_H;

                        if (i % 2 == 0)
                        {
                            RectangleShape rowBg({ CARD_W - 4.f, ROW_H - 2.f });
                            rowBg.setPosition({ CARD_X + 4.f, rowY });
                            rowBg.setFillColor(Color(255, 255, 255, 6));
                            window.draw(rowBg);
                        }

                        CircleShape medal(14.f);
                        medal.setOrigin({ 14.f, 14.f });
                        medal.setPosition({ CARD_X + 42.f, rowY + ROW_H / 2.f });

                        Color medalFill, medalTextColor;
                        
                        if (i == 0) 
                        { 
                            medalFill = Color(180, 140, 20);  
                            medalTextColor = Color(255, 235, 120); 
                        }
                        else 
                        { 
                            medalFill = Color(120, 130, 140); 
                            medalTextColor = Color(220, 230, 235); 
                        }

                        medal.setFillColor(medalFill);
                        window.draw(medal);

                        Text rankNum(font, to_string(i + 1), 14);
                        rankNum.setFillColor(medalTextColor);
                        FloatRect rr = rankNum.getLocalBounds();
                        rankNum.setOrigin({ rr.size.x / 2.f, rr.size.y / 2.f });
                        rankNum.setPosition({ CARD_X + 42.f, rowY + ROW_H / 2.f - 4.f });
                        window.draw(rankNum);

                        Text playerName(font, records[indices[i]].player, 18);
                        playerName.setFillColor(i == 0 ? Color::White : Color(200, 200, 220));
                        playerName.setPosition({ CARD_X + 70.f, rowY + ROW_H / 2.f - 12.f });
                        window.draw(playerName);

                        Text scoreT(font, to_string(records[indices[i]].score), 18);
                        scoreT.setFillColor(i == 0 ? GOLD : SILVER);
                        FloatRect scr = scoreT.getLocalBounds();
                        scoreT.setOrigin({ scr.size.x, 0 });
                        scoreT.setPosition({ CARD_X + CARD_W - 24.f, rowY + ROW_H / 2.f - 12.f });
                        window.draw(scoreT);
                    }
                }

                return startY + CARD_H + 16.f;
            };

        float cardY = 200.f;
        
        for (int g = 0; g < 3; g++)
            cardY = drawGameCard(gameOrder[g], cardY, g);

        Text hint(font, "Press Q to return to hub", 30);
        hint.setFillColor(Color(80, 80, 110));
        FloatRect hr = hint.getLocalBounds();
        hint.setOrigin({ hr.size.x / 2.f, 0 });
        hint.setPosition({ WIN_W / 2.f, WIN_H - 280.f });
        window.draw(hint);

        window.display();
    }
}