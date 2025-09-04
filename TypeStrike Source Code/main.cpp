#include "login.hpp"
#include "header.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <string>
#include <iomanip>
#include <graphics.h>
#include <cmath>
using namespace std;

void login(GraphicsRenderer& renderer);
void leaderboard();

int main() {
    string name;
    double score;
    ofstream outfile("leaderboard.txt", ios::app);
    cout << "Welcome to TypeStrike ^_^\n Please Enter Your Name: ";
    cin >> name;
    GraphicsRenderer renderer(800, 600);
    transition1();
    login(renderer);
    transition2();
    Game game(" ", &renderer);
    game.gameLoop();
    score=game.getWPM();
    outfile << name << " " << fixed << setprecision(1) << score << "\n";
    outfile.close();
    leaderboard();
    return 0;
}



// Intro Interface 
void login(GraphicsRenderer& renderer) {
    setbkcolor(BLACK);
    cleardevice();
    Button mainButtons[3] = {
        {300, 250, 200, 50, "START"},
        {300, 320, 200, 50, "HOW TO PLAY"},
        {300, 390, 200, 50, "EXIT"}
    };
    Button homeButton = {730, 20, 40, 40, "<"};

    bool onMainMenu = true;
    drawMainMenu(mainButtons);
    renderer.drawPixelLaptop(100, 400, LIGHTBLUE);
    renderer.drawRobot(700, 400);

    while (true) {
        if (kbhit())
        {
            if (getch()==27)
            exit(1);
        }
        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);

            if (onMainMenu) {
                if (isInside(x, y, mainButtons[0])) {
                    // Start Game
                    setbkcolor(BLACK);
                    cleardevice();  
                    return;
                } else if (isInside(x, y, mainButtons[1])) {
                    // How to Play
                    drawRulesPage(homeButton);
                    onMainMenu = false;
                } else if (isInside(x, y, mainButtons[2])) {
                    // Exit
                    closegraph();
                    exit(1);
                }
            } else {
                if (isInside(x, y, homeButton)) {
                    drawMainMenu(mainButtons);
                    onMainMenu = true;
                }
            }
        }
    }
}

// Sort Leaderboard
void leaderboard() {
    ifstream infile("leaderboard.txt");
    vector<string> names;
    vector<double> scores;

    string name;
    double score;

    // Read the file
    while (infile >> name >> score) {
        names.push_back(name);
        scores.push_back(score);
    }
    infile.close();

    // Create a vector of indices
    vector<int> indices(names.size());
    for (int i = 0; i < indices.size(); ++i) {
        indices[i] = i;
    }

    // Sort indices based on scores
    sort(indices.begin(), indices.end(), [&](int a, int b) {
        return scores[a] > scores[b]; // Descending order
    });

    // Write sorted data back to file
    ofstream outfile("leaderboard.txt");
    for (int idx : indices) {
        outfile << names[idx] << " " << fixed << setprecision(1) << scores[idx] << endl;
    }
    outfile.close();
}
