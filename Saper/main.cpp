//
// Created by Krzysztof Wolarz on 2019-04-23.
//
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <unistd.h>
using namespace sf;
using namespace std;

class Wynik{
public:
    bool win; //// sprawdzanie czy wygrana
    int gameTime; //// czas gry gracza
};

class Board{
public:
    int plansza[10][10];

    bool generujPlansze() { //// wypełniam plansze zakrytymi obrazkami
        for(int x = 0; x<10; x++)
            for(int y = 0; y<10; y++)
                plansza[x][y] = 0;

        return true;
    }
};

class Bomb{
private:
    time_t t;
    int numberOfBombs = 10;
    int posX, posY;
public:
    Board p;
    Bomb(){}   
    bool ustawMine(int pos_X, int pos_Y){ //// funkcja do ustawiania miny
        if(p.plansza[pos_X][pos_Y] != 9){
            p.plansza[pos_X][pos_Y] = 9;

            for(int k = -1; k<2; k++)
                for(int l = -1; l<2; l++){
                    if((pos_X + l) < 0 or (pos_Y + k) < 0) continue;
                    if((pos_X + l) > 9 or (pos_Y + k) > 9) continue;

                    if(p.plansza[pos_X + l][pos_Y + k] == 9) continue;
                    p.plansza[pos_X + l][pos_Y + k] += 1;
                }
        }
        return true;
    }

    void losujPozycje(){ //// funkcja do losowania pozucji miny
        srand((unsigned)time(&t));

        for(numberOfBombs; numberOfBombs>0; ){
            posX = rand()%10;
            posY = rand()%10;
            if(p.plansza[posX][posY] != 9){
                ustawMine(posX, posY);
                numberOfBombs--;
            }
        }
    }
};

class Game{
private:
    Font font;
    Bomb b;
    Wynik r;
    int w = 32; ////szerokosc obrazka
    int grid[10][10]; //// plansza komputera
    int sgrid[10][10]; //// plansza graca
    Clock stoper;
    float czas;
    int flags = 10;
    bool koniec = false;
    bool wczytane = false;
    string zapisane; ////nazwa zapisu gry
    string wyjatek = "Brak tego pliku w katalogu cmake-build-debug";
public:

    Game(){}

    Game(string nazwaPliku){
            zapisane = nazwaPliku;
            wczytane = true;
        }

    bool checkWin(){
        int bombs = 0;
        for(int i = 0; i<10; i++)
            for(int j = 0; j<10; j++)
                if(sgrid[i][j] == 11 and sgrid[i][j] != 10)
                    bombs++;

        if(bombs == 10) return true;
        return false;
    }

    void show(int x, int y){
        if (x < 0 or x > 9 ) return;
        if (y < 0 or y > 9) return;
        if (sgrid[x][y] == grid[x][y]) return;

        if(sgrid[x][y] != 9 and sgrid[x][y] != grid[x][y] and sgrid[x][y] != 11)
            sgrid[x][y] = grid[x][y];

        if (sgrid[x][y] != 0) return;

        show(x-1,y-1);
        show(x-1,y);
        show(x-1,y+1);
        show(x+1,y-1);
        show(x+1,y);
        show(x+1,y+1);
        show(x,y-1);
        show(x,y);
        show(x,y+1);
    }

    void showResult(){
        if(r.win)
            cout<<"Gratulacje! Udało Ci się wygrać! Czas gry to: "<< r.gameTime<<"s";
        if(!r.win)
            cout<<"Niestety przegrałeś. Czas gry to: "<< r.gameTime<<"s";
    }

    void play(){

        srand(time(0));
        b.p.generujPlansze();
        b.losujPozycje();

        try {
            if(!font.loadFromFile("arial.ttf"))
                throw wyjatek;
        }
        catch(string w){
            cout<<"Nie udało się wczytać pliku."<<endl;
            cout<<"Powód: "<<w<<endl;
        }

        RenderWindow window(VideoMode(320, 400), "SAPER");
        Vector2f offset(0, 80);

        RectangleShape minyTlo;
        minyTlo.setPosition(32.5,20);
        minyTlo.setSize(Vector2f(70,50));
        minyTlo.setFillColor(Color::Black);

        Text miny;
        miny.setFont(font);
        miny.setCharacterSize(42);
        miny.setColor(Color::Red);
        miny.setPosition(50, 20);
        miny.setString(to_string(flags));

        RectangleShape timerTlo;
        timerTlo.setPosition(185+32.5,20);
        timerTlo.setSize(Vector2f(70,50));
        timerTlo.setFillColor(Color::Black);

        Text timer;
        timer.setFont(font);
        timer.setCharacterSize(42);
        timer.setColor(Color::Red);
        timer.setPosition(185+70, 20);
        timer.setString(("0"));

        Texture t, t1, t2, t3;
        t.loadFromFile("images/tiles.jpg");
        t1.loadFromFile("images/gra.png");
        t2.loadFromFile("images/przegrana.png");
        t3.loadFromFile("images/wygrana.png");
        Sprite s(t);
        Sprite s1(t1);

        if(!wczytane){
            for (int i=0 ;i<10;i++)
                for (int j=0;j<10;j++) {
                    sgrid[i][j] = 10;
                    grid[i][j] = b.p.plansza[i][j];
                }
        }

        if(wczytane){
            int a[10][10];
            ifstream odczyt(zapisane);

            for(int g = 0; g<10; g++) {
                for (int h = 0; h < 10; h++) {
                    odczyt >> a[g][h];
                    sgrid[g][h] = a[g][h];
                    grid[g][h] = b.p.plansza[g][h];
                }
            }
            odczyt.close();
        }

        while (window.isOpen())
        {
            Vector2i pos = Mouse::getPosition(window) - Vector2i(offset);
            int x = pos.x/w;
            int y = pos.y/w;

            czas = stoper.getElapsedTime().asSeconds();
            timer.setString(to_string(int(czas)));
            r.gameTime = czas;
            if(int(czas) == 10)
                timer.setPosition(185+50, 20);
            if(int(czas) == 100)
                timer.setPosition(185+30, 20);

            Event e;
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                    window.close();

                if(e.type == Event::KeyPressed)
                    if(e.key.code == Keyboard::S) {
                        ofstream zapis("zapisGry.txt");
                        for(int e = 0; e<10; e++){
                            zapis<<endl;
                            for(int f = 0; f<10; f++) {
                                int u = sgrid[e][f];
                                zapis << u <<" ";
                            }
                        }
                        zapis.close();
                        minyTlo.setFillColor(Color::Blue);
                        timerTlo.setFillColor(Color::Blue);
                    }

                if (e.type == Event::MouseButtonPressed)
                    if (e.key.code == Mouse::Left){
                        minyTlo.setFillColor(Color::Black);
                        timerTlo.setFillColor(Color::Black);
                        show(x, y);
                        sgrid[x][y] = grid[x][y];
                        if(sgrid[x][y] == 9)
                            koniec = true;
                        
                        if(checkWin()){
                            r.win = true;
                            s1.setTexture(t3);
                            showResult();
                        }
                        
                        if(koniec){
                            s1.setTexture(t2);
                            r.win = false;
                            showResult();
                        }
                    }

                    else if (e.key.code == Mouse::Right) {
                        if(flags) {

                            if(sgrid[x][y] == 11) {
                                sgrid[x][y] = 10;
                                flags++;
                            }
                            else if(sgrid[x][y] == 10){
                                sgrid[x][y] = 11;
                                flags--;
                            }
                            miny.setString(to_string(flags));
                            
                            if(flags<10)
                                miny.setPosition(70, 20);
                            else
                                miny.setPosition(50, 20);

                        }
                    }
            }

            window.clear(Color::White);
            s1.setPosition(135, 20);
            for (int i=0;i<10;i++)
                for (int j=0;j<10;j++)
                {
                    if (koniec) {
                        sgrid[i][j] = grid[i][j];
                        timer.setString("0");
                    }
                    if(r.win)
                        timer.setString("0");

                    s.setTextureRect(IntRect(sgrid[i][j]*w,0,w,w));
                    s.setPosition(i*w, j*w);
                    s.move(offset);
                    window.draw(s);
                }

            window.draw(s1);
            window.draw(minyTlo);
            window.draw(timerTlo);
            window.draw(miny);
            window.draw(timer);
            window.display();
        }
    }
};

class MainMenu {
public:
    MainMenu(float width, float height){
        font.loadFromFile("arial.ttf");

        menu[0].setFont(font);
        menu[0].setColor(Color::Red);
        menu[0].setString("Graj");
        menu[0].setCharacterSize(50);
        menu[0].setPosition(Vector2f(width/2 - menu[0].getGlobalBounds().width/2, height/(3 + 1) * 1));

        menu[1].setFont(font);
        menu[1].setColor(Color::White);
        menu[1].setString("Wczytaj");
        menu[1].setCharacterSize(50);
        menu[1].setPosition(Vector2f(width/2 - menu[1].getGlobalBounds().width/2, height/(3 + 1) * 2));

        menu[2].setFont(font);
        menu[2].setColor(Color::White);
        menu[2].setString("Wyjscie");
        menu[2].setCharacterSize(50);
        menu[2].setPosition(Vector2f(width/2 - menu[2].getGlobalBounds().width/2, height/(3 + 1) * 3));

        selectedItemIndex = 0;
    }

    void draw(RenderWindow &window){
        for (int i = 0; i < 3; i++)
            window.draw(menu[i]);
    }

    void MoveUp(){
        if (selectedItemIndex - 1 >= 0){
            menu[selectedItemIndex].setColor(Color::White);
            selectedItemIndex--;
            menu[selectedItemIndex].setColor(Color::Red);
        }
    }

    void MoveDown(){
        if (selectedItemIndex + 1 < 3){
            menu[selectedItemIndex].setColor(Color::White);
            selectedItemIndex++;
            menu[selectedItemIndex].setColor(Color::Red);
        }
    }

    int GetPressedItem() { return selectedItemIndex; }

private:
    int selectedItemIndex;
    Font font;
    Text menu[3];

public:
    void showMenu() {

        RenderWindow window2(VideoMode(600, 600), "SAPER - MENU");
        MainMenu menu(window2.getSize().x, window2.getSize().y);
        while (window2.isOpen()) {
            Event event;

            while (window2.pollEvent(event)) {
                switch (event.type) {
                    case Event::KeyReleased:
                        switch (event.key.code) {
                            case Keyboard::Up:
                                menu.MoveUp();
                                break;

                            case Keyboard::Down:
                                menu.MoveDown();
                                break;

                            case Keyboard::Return:
                                switch (menu.GetPressedItem()) {
                                    case 0: {
                                        window2.close();
                                        Game saper;
                                        saper.play();
                                        saper.showResult();
                                    } break;
                                    
                                    case 1: {
                                        window2.close();
                                        Game saper("zapisGry.txt");
                                        saper.play();
                                    } break;
                                    
                                    case 2: window2.close(); break;
                                } break;
                        } break;
                    
                    case Event::Closed: window2.close(); break;
                }
            }
            window2.clear();
            menu.draw(window2);
            window2.display();
        }
    }
};

int main(){
    MainMenu menu(600, 600);
    menu.showMenu();
}