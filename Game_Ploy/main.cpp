#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <SFML\Graphics.hpp>
#include "Animation.h"
#include "Player.h"
#include "Platform.h"
#include <vector>
#include<time.h>
#include<stdio.h>
#include <string.h>
#include <utility>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;
static const float VIEW_HEIGHT = 1080.0f;
static const float VIEW_WIDTH = 1920.0f;

void ResizeView(const sf::RenderWindow& window, sf::View& view)
{
    float aspectRatio = float(window.getSize().x) / float(window.getSize().y);
    view.setSize(VIEW_HEIGHT * aspectRatio, VIEW_WIDTH);
}

void Showtexet(int x, int y, int num, int size, sf::RenderWindow& window, sf::Font* font)
{
    sf::Text text;
    text.setFont(*font);
    text.setPosition(x, y);
    string str;
    stringstream ss;
    ss << num;
    ss >> str;
    text.setString(str);
    text.setFillColor(sf::Color::Magenta);
    text.setCharacterSize(size);
    window.draw(text);
}

void ShowScoretexet(int x, int y, string word, int size,sf::Color color, sf::RenderWindow& window, sf::Font* font)
{
    sf::Text text;
    text.setFont(*font);
    text.setPosition(x, y);
    text.setString(word);
    text.setCharacterSize(size);
    text.setFillColor(color);
    window.draw(text);
}

void updatascore(string a, int b)
{
    FILE* fp;
    char temp[255];
    int score[6];
    string name[6];
    vector <pair<int, string>> userScore;
    fp = fopen("score.txt", "r");
    for (int i = 0; i < 5; i++)
    {
        fscanf(fp, "%s", &temp);
        name[i] = temp;
        fscanf(fp, "%d", &score[i]);
        userScore.push_back(make_pair(score[i], name[i]));
    }
    name[5] = a;
    score[5] = b;
    userScore.push_back(make_pair(score[5], name[5]));
    sort(userScore.begin(), userScore.end());
    fclose(fp);
    fopen("score.txt", "w");
    for (int i = 5; i >= 1; i--)
    {
        strcpy(temp, userScore[i].second.c_str());
        fprintf(fp, "%s %d\n", temp, userScore[i].first);
    }
    fclose(fp);
}


void Showscoreboard(sf::RenderWindow& window, float x)
{
    sf::Font font;
    font.loadFromFile("Font/Dog.ttf");
    FILE* fp;
    char temp[255];
    char score[255];
    fp = fopen("score.txt", "r");
    for (int i = 0; i < 5; i++)
    {
        char no[3] = { '1' + i };
        no[1] = '.';
        no[2] = '\0';
        ShowScoretexet(x+20.0f, 350 + 100 * i, no, 40, sf::Color::White, window, &font);
        fscanf(fp, "%s", temp);
        ShowScoretexet(x + 70.0f, 350 + 100 * i, temp, 40, sf::Color::White, window, &font);
        fscanf(fp, "%s", score);
        ShowScoretexet(x + 420, 350 + 100 * i, score, 40,sf::Color::White, window, &font);
    }
    fclose(fp);

}
int main()
{
    int Score = 0, TimeCandy = 0, TimeEnemy = 0, TimeCookie = 0, TimeErase = 0, GameMode = 1;
    float SumTime = 0.0f,MainTime = 100.0f;
    int R1 = 0,R2 = 0;
    string name;
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "Game's Ploy", sf::Style::Titlebar | sf::Style::Close | sf::Style::Fullscreen);
    //sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(1020, 1080));
    sf::Texture playerTexture;
        playerTexture.loadFromFile("Pic/main.png");
    sf::Texture Candy_pic;
        Candy_pic.loadFromFile("Pic/Candy.png");
    sf::Texture Choco;
        Choco.loadFromFile("Pic/Chocolate.png");
    sf::Texture Pumpkin_pic;
        Pumpkin_pic.loadFromFile("Pic/Pumpkin1.png");
    sf::Texture Cookie_pic;
        Cookie_pic.loadFromFile("Pic/Cookie.png");
    sf::Texture Bg;
        Bg.loadFromFile("Pic/B2.png");
    sf::Texture Bg1_pic;
        Bg1_pic.loadFromFile("Pic/B1.png");
    sf::Texture st;
        st.loadFromFile("Pic/state.jpg");
    sf::Texture s1_pic;
        s1_pic.loadFromFile("Pic/Stick.png");
    sf::Texture s2_pic;
        s2_pic.loadFromFile("Pic/Stick.png");
    sf::Texture b_pic;
        b_pic.loadFromFile("Pic/c1.png");
    sf::Texture c_pic;
        c_pic.loadFromFile("Pic/Stick.png");
    sf::Texture Spider_pic;
        Spider_pic.loadFromFile("Pic/Spider.png");
    sf::Font Font;
        Font.loadFromFile("Font/Dog.ttf");
    sf::Texture B_Score;
        B_Score.loadFromFile("Pic/Score.png");
    sf::Texture Sym1_Score_p;
        Sym1_Score_p.loadFromFile("Pic/Clock.png");
    sf::Texture Sym2_Score_p;
        Sym2_Score_p.loadFromFile("Pic/2.png");
    sf::Texture Scoreboard_p;
        Scoreboard_p.loadFromFile("Pic/Scoreboard.png");
    sf::Texture BgN_p;
        BgN_p.loadFromFile("Pic/BgN.png");
    sf::Texture NEXT_pic;
        NEXT_pic.loadFromFile("Pic/NEXT.png");
    sf::Texture PLAY_pic;
        PLAY_pic.loadFromFile("Pic/PLAY.png");
    sf::Texture SCORE_pic;
        SCORE_pic.loadFromFile("Pic/SCORE(1).png");
    sf::Texture EXIT_pic;
        EXIT_pic.loadFromFile("Pic/EXIT.png");
    sf::Texture BACK_pic;
        BACK_pic.loadFromFile("Pic/BACK.png");

    Player player(&playerTexture, sf::Vector2u(20, 3), 0.075f, 400.0f,250.0f);
    
    std::vector<Platform> platforms,Item,Enemy,Candy,Pumpkin,Cookie;
    Platform Background(&Bg, sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT) / 2.0f);
    Platform Bg1(&Bg1_pic, sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT) / 2.0f);
    Platform state(&st, sf::Vector2f(VIEW_WIDTH, 400), sf::Vector2f(VIEW_WIDTH/2.0f, VIEW_HEIGHT+100));
    Platform Scoreboard(&Scoreboard_p, sf::Vector2f(2000, 1100), sf::Vector2f(900, 550));
    Platform s1(&s1_pic, sf::Vector2f(300, 50), sf::Vector2f(400, 700));
    Platform s2(&s1_pic, sf::Vector2f(300, 50), sf::Vector2f(1500, 700));
    Platform b(&b_pic, sf::Vector2f(130, 130), sf::Vector2f(800, 915));
    Platform c(&c_pic, sf::Vector2f(400, 50), sf::Vector2f(950, 480));
    Platform Bg_Score(&B_Score, sf::Vector2f(250, 150), sf::Vector2f(280, 170));
    Platform Bg_Score2(&B_Score, sf::Vector2f(250, 150), sf::Vector2f(1580, 170));
    Platform BgN(&BgN_p, sf::Vector2f(900, 400), sf::Vector2f(970, 490));
    Platform Sym1_Score(&Sym1_Score_p, sf::Vector2f(80, 80), sf::Vector2f(1450, 170));
    Platform Sym2_Score(&Sym2_Score_p, sf::Vector2f(100, 100), sf::Vector2f(430, 170));
    Platform NEXT(&NEXT_pic, sf::Vector2f(200.0f, 200.0f), sf::Vector2f(1450.0f, 900.0f));
    Platform BACK1(&BACK_pic, sf::Vector2f(190.0f, 190.0f), sf::Vector2f(500.0f, 900.0f));
    Platform SCORE(&SCORE_pic, sf::Vector2f(200.0f, 200.0f), sf::Vector2f(570.0f, 800.0f));
    Platform PLAY(&PLAY_pic, sf::Vector2f(200.0f, 200.0f), sf::Vector2f(970.0f, 800.0f));
    Platform EXIT(&EXIT_pic, sf::Vector2f(200.0f, 200.0f), sf::Vector2f(1370.0f, 800.0f));
    Platform EXIT2(&EXIT_pic, sf::Vector2f(150.0f, 150.0f), sf::Vector2f(1200.0f, 950.0f));
    Platform BACK(&BACK_pic, sf::Vector2f(140.0f, 140.0f), sf::Vector2f(650.0f, 950.0f));
    srand(time(NULL));
    float deltaTime = 0.0f;
    sf::Clock clock;
  
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        if (deltaTime > 1.0f / 20.0f)
            deltaTime = 1.0f / 20.0f;
        
        sf::Event ev;
        while (window.pollEvent(ev))
        {
            if (ev.type == sf::Event::Closed)
                window.close();
     
            if (GameMode == 2)
            {
                if (ev.type == sf::Event::TextEntered)
                    name.push_back(ev.text.unicode);
                if (!name.empty() && name.back() == 8)
                {
                    name.pop_back();
                    if (!name.empty())
                        name.pop_back();

                }

                if (!name.empty() && !(name.back() >= 'a' && name.back() <= 'z' || name.back() >= 'A' && name.back() <= 'Z' || name.back() >= '0' && name.back() <= '9'))
                {
                    name.pop_back();
                }
            }
        }

        int x = sf::Mouse::getPosition().x;
        int y = sf::Mouse::getPosition().y;

        //PLAY
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x >= 870.0f && x <= 1070.0f && y >= 700.0f && y <= 900.0f && GameMode == 1)
        {
            GameMode = 2;
        }

        //SCORE
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x >= 470.0f && x <= 670.0f && y >= 700.0f && y <= 900.0f && GameMode == 1)
        {
            GameMode = 4;
        }

        //EXIT
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x >= 1270.0f && x <= 1470.0f && y >= 700.0f && y <= 900.0f && GameMode == 1)
        {
            window.close();
        }

        //NEXT
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x >= 1350.0f && x <= 1550.0f && y >= 800.0f && y <= 1000.0f && GameMode==2)
        {
            GameMode = 3;
        } 

        //BACK1
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x >= 400.0f && x <= 600.0f && y >= 800.0f && y <= 1000.0f && GameMode == 2)
        {
            GameMode = 1;
        }

        //EXIT2
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x >= 1125.0f && x <= 1275.0f && y >= 875.0f && y <= 1025.0f && GameMode == 4)
        {
            window.close();
        }

        //BACK
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && x >= 580.0f && x <= 1670.0f && y >= 870.0f && y <= 1020.0f && GameMode == 4)
        {
            GameMode = 1;
        }

        //BACK2
        if (GameMode == 3 && sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            GameMode = 1;
        }

        if (GameMode == 3)
        {
            SumTime += deltaTime;
            if(SumTime >= 0.7f)
            { 
                R1 = rand() % 850 + 100;
                R2 = rand() % 800 + 100;
                MainTime-=0.5f;
                if (MainTime < 0) MainTime = 0;
                TimeErase++;
                TimeCandy++;
                TimeEnemy++;
                TimeCookie++;
                int x = rand() % 5;
                if(x==0)
                    Item.push_back(Platform(&Choco, sf::Vector2f(80.0f, 80.0f), sf::Vector2f(R1, -100.0f)));
                else if (x == 1 && TimeEnemy >= 2)
                {
                    Enemy.push_back(Platform(&Spider_pic, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(R1, -100.0f)));
                    TimeEnemy -= 2;
                }
                
                else if (x == 2 && TimeEnemy >= 2)
                {
                    Pumpkin.push_back(Platform(&Pumpkin_pic, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(R1, -100.0f)));
                    TimeEnemy -= 2;
                }
              
                else if (x == 3 && TimeCandy >= 15)
                {
                    TimeCandy -= 15;
                    Candy.push_back(Platform(&Candy_pic, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(R1, R2)));
                }

                else if (x == 4 && TimeCookie >= 20)
                {
                    TimeCookie -= 20;
                    Cookie.push_back(Platform(&Cookie_pic, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(R1, -100.0f)));
                }

                else Item.push_back(Platform(&Choco, sf::Vector2f(80.0f, 80.0f), sf::Vector2f(R1, -100.0f)));


                R1 = rand() % 850 + 1000;
                R2 = rand() % 800 + 100;
                TimeErase++;
                TimeCandy++;
                TimeEnemy++;
                TimeCookie++;
                x = rand() % 5;
                if (x == 0)
                    Item.push_back(Platform(&Choco, sf::Vector2f(80.0f, 80.0f), sf::Vector2f(R1, -100.0f)));
                else if (x == 1 && TimeEnemy >= 2)
                {
                    Enemy.push_back(Platform(&Spider_pic, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(R1, -100.0f)));
                    TimeEnemy -= 2;
                }

                else if (x == 2 && TimeEnemy >= 2)
                {
                    Pumpkin.push_back(Platform(&Pumpkin_pic, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(R1, -100.0f)));
                    TimeEnemy -= 2;
                }

                else if (x == 3 && TimeCandy >= 15)
                {
                    TimeCandy -= 15;
                    Candy.push_back(Platform(&Candy_pic, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(R1, R2)));
                }

                else if (x == 4 && TimeCookie >= 20)
                {
                    TimeCookie -= 20;
                    Cookie.push_back(Platform(&Cookie_pic, sf::Vector2f(100.0f, 100.0f), sf::Vector2f(R1, -100.0f)));
                }

                else Item.push_back(Platform(&Choco, sf::Vector2f(80.0f, 80.0f), sf::Vector2f(R1, -100.0f)));
                SumTime -= 0.7f;
            }
        }
        
        
        //Update
        if (GameMode == 3)
        {
            player.Update(deltaTime);

             for (Platform& S : Item)
                S.update(deltaTime);

             for (Platform& S : Enemy)
                 S.update(deltaTime);

             for (Platform& S : Candy)
                 S.sumTime(deltaTime);

             for (Platform& S : Pumpkin)
                 S.update(deltaTime);

             for (Platform& S : Cookie)
                 S.update(deltaTime);

            
        }
        

        //ชน && +- Score
        if (GameMode == 3)
        {
            Collider playerCollision = player.GetCollider();
            sf::Vector2f direction;

            for (Platform& S : Item)
                if (S.GetCollider().CheckCollider(playerCollision))
                {
                    S.body.setPosition(S.body.getPosition().x,1800.0f );
                    Score+=2; 
                }
        
            for (Platform& S : Candy)
                if (S.GetCollider().CheckCollider(playerCollision))
                {
                    S.body.setPosition(S.body.getPosition().x, 1800.0f);
                    MainTime +=2.0f;
                    
              
                }

            for (Platform& S : Enemy)
                if (S.GetCollider().CheckCollider(playerCollision))
                {
                    S.body.setPosition(S.body.getPosition().x, 1800.0f);
                
                    Score -= 5;
                    if (Score < 0) Score = 0;
                    MainTime -= 2.5f;
                }

            for (Platform& S : Pumpkin)
                if (S.GetCollider().CheckCollider(playerCollision))
                {
                    S.body.setPosition(S.body.getPosition().x, 1800.0f);
                    if (Score < 0) Score = 0;
                    Score -= 10;
                    if (Score < 0) Score = 0;
                    MainTime -= 2.5f;
                    
                }

            for (Platform& S : Cookie)
                if (S.GetCollider().CheckCollider(playerCollision))
                {
                    S.body.setPosition(S.body.getPosition().x, 1800.0f);
                    Score += 20;
                }

            if (MainTime <= 0)
            {
                GameMode = 4;
                char temp[100];
                strcpy(temp, name.c_str());
                if (strlen(temp) == 0)
                    strcpy(temp, "anonymous");
                updatascore(temp, Score);
            }
            
 
            if (state.GetCollider().CheckCollider(playerCollision, direction, 1.0f))
                player.OnCollosion(direction);

            if (s1.GetCollider().CheckCollider(playerCollision, direction, 1.0f))
                player.OnCollosion(direction);

            if (s2.GetCollider().CheckCollider(playerCollision, direction, 1.0f))
                player.OnCollosion(direction);

            if (c.GetCollider().CheckCollider(playerCollision, direction, 1.0f))
                player.OnCollosion(direction);

            if (b.GetCollider().CheckCollider(playerCollision, direction, 0.5f)) {
                player.OnCollosion(direction);
                
            }
                
        }
        
        
        //delete
        while (!Item.empty() && (Item.front().body.getPosition().y > 1080.0f))
            Item.erase(Item.begin());

        while (!Enemy.empty() && (Enemy.front().body.getPosition().y > 1080.0f))
            Enemy.erase(Enemy.begin());

        while (!Pumpkin.empty() && (Pumpkin.front().body.getPosition().y > 1080.0f))
            Pumpkin.erase(Pumpkin.begin());

        while (!Candy.empty() && Candy[0].sumTimeErase >= 5) {
            Candy.erase(Candy.begin());
        }
            

        while (!Cookie.empty() && (Cookie.front().body.getPosition().y > 1080.0f))
            Cookie.erase(Cookie.begin());

        //Draw
        
        window.clear(sf::Color(150,150,150));
        if (GameMode == 1)
        {
            Bg1.Draw(window);
            SCORE.Draw(window);
            PLAY.Draw(window);
            EXIT.Draw(window);
        }
        if (GameMode == 2)
        {
            Background.Draw(window);
            BgN.Draw(window);
            if(!name.empty())
            ShowScoretexet(680, 400, name, 100,sf::Color::Magenta, window, &Font);
            else ShowScoretexet(680, 400, "Enter Name", 100, sf::Color::Magenta, window, &Font);
            NEXT.Draw(window);
        }
        if (GameMode == 3)
        {
            Background.Draw(window);
            b.Draw(window);
            s1.Draw(window);
            s2.Draw(window);
            c.Draw(window);
            for (Platform& S : Item)
                S.Draw(window);
            for (Platform& S : Enemy)
                S.Draw(window);
            for (Platform& S : Candy)
                S.Draw(window);
            for (Platform& S : Pumpkin)
                S.Draw(window);
            for (Platform& S : Cookie)
                S.Draw(window);
            state.Draw(window);
            Bg_Score.Draw(window);
            Bg_Score2.Draw(window);
            Sym1_Score.Draw(window);
            Sym2_Score.Draw(window);
            BACK2.Draw(window);
            
            //Text
            Showtexet(190, 110, Score, 90, window, &Font);
            Showtexet(1510, 110, MainTime, 90, window, &Font);

            player.Draw(window);

        }

        if (GameMode == 4)
        {
            Background.Draw(window);
            BACK.Draw(window);
            EXIT2.Draw(window);
            Scoreboard.Draw(window);
            Showscoreboard(window, 700.0f);
        }
        
        window.display();
    }
    return 0;
}