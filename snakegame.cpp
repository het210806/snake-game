#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

enum class Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };

class Game {
protected:
    int width, height, score;
    bool gameIsOver;
    Direction dir;

public:
    Game() {
        width = 50;
        height = 20;
        score = 0;
        gameIsOver = false;
        dir = Direction::STOP;
    }

    int getWidth() { return width; }
    int getHeight() { return height; }
    int getScore() { return score; }
    void addScore(int points) { score += points; }
    bool isGameOver() { return gameIsOver; }
    void setGameOver(bool state) { gameIsOver = state; }

    Direction getDirection() { return dir; }
    void setDirection(Direction d) { dir = d; }
};

class Snake : public Game {
protected:
    int x, y;
    int tailX[100], tailY[100];
    int tailLength;

public:
    Snake() {
        x = width / 2;
        y = height / 2;
        tailLength = 0;
    }

    void move() {
        int prevX = tailX[0], prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;

        for (int i = 1; i < tailLength; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        switch (getDirection()) {
            case Direction::LEFT:  x--; break;
            case Direction::RIGHT: x++; break;
            case Direction::UP:    y--; break;
            case Direction::DOWN:  y++; break;
            default: break;
        }

        if (x >= width || x < 0 || y >= height || y < 0)
            setGameOver(true);

        for (int i = 0; i < tailLength; i++) {
            if (tailX[i] == x && tailY[i] == y)
                setGameOver(true);
        }
    }

    int getX() { return x; }
    int getY() { return y; }
    int getTailLength() { return tailLength; }
    void increaseTail() { tailLength++; }
};

class Fruit : public Snake {
protected:
    int fruitX, fruitY;

public:
    Fruit() { spawn(); }

    void spawn() {
        fruitX = rand() % (getWidth() - 1);
        fruitY = rand() % (getHeight() - 1);
    }

    int getFruitX() { return fruitX; }
    int getFruitY() { return fruitY; }
};

class Main : public Fruit {
public:
    Main() { system("cls"); }

    void draw() {
        system("cls");
        for (int i = 0; i < getWidth() + 2; i++) cout << "-";
        cout << endl;

        for (int i = 0; i < getHeight(); i++) {
            for (int j = 0; j < getWidth(); j++) {
                if (j == 0) cout << "|";

                if (i == getY() && j == getX()) cout << "O";  // Snake Head
                else if (i == getFruitY() && j == getFruitX()) cout << "@";  // Fruit
                else {
                    bool printTail = false;
                    for (int k = 0; k < getTailLength(); k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o";  // Tail
                            printTail = true;
                        }
                    }
                    if (!printTail) cout << " ";
                }

                if (j == getWidth() - 1) cout << "|";
            }
            cout << endl;
        }

        for (int i = 0; i < getWidth() + 2; i++) cout << "-";
        cout << endl;

        cout << "Score: " << getScore() << endl;
    }

    void updateGame() {
        move();
        if (getX() == getFruitX() && getY() == getFruitY()) {
            addScore(10);
            increaseTail();
            spawn();
        }
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': setDirection(Direction::LEFT); break;
                case 'd': setDirection(Direction::RIGHT); break;
                case 'w': setDirection(Direction::UP); break;
                case 's': setDirection(Direction::DOWN); break;
                case 'x': setGameOver(true); break;
            }
        }
    }
};

int difficulty() {
    int dif, choice;
    cout << "\n SET DIFFICULTY \n 1 : EASY\n 2 : MEDIUM\n 3 : HARD "
         << "\n NOTE : If not chosen, difficulty will be set to MEDIUM."
         << "\n Choose difficulty: ";
    
    cin >> choice;

    switch (choice) {
        case 1: dif = 200; break;
        case 2: dif = 150; break;
        case 3: dif = 100; break;
        default: dif = 150;
    }
    return dif;
}

void game();

void restart(char c){
    if(c=='R' || c=='r')
    {
        game();
    }
    else{exit;}

}

void game() {
    srand(time(0));
    Main game;
    int dif = difficulty();

    while (!game.isGameOver()) {
        game.draw();
        game.input();
        game.updateGame();
        Sleep(dif);
    }

    cout << "Game Over! Final Score: " << game.getScore() << endl;
    cout<<"Press 'R' to play again"<<endl<<"Press any other key to exit"<<endl;
    char c;
    cin>>c;
    restart(c);
}

 int main(){
    game();
    return 0;
 }