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
#include <ctime>

using namespace std;

struct Button {
    int x, y, w, h;
    string text;
};

void drawButton(const Button& btn, int color = LIGHTBLUE) {
    setfillstyle(SOLID_FILL, color);
    bar(btn.x, btn.y, btn.x + btn.w, btn.y + btn.h);
    setbkcolor(color);
    setcolor(WHITE);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(btn.x + 20, btn.y + 10, (char*)btn.text.c_str());
}

bool isInside(int mx, int my, const Button& btn) {
    return mx >= btn.x && mx <= btn.x + btn.w && my >= btn.y && my <= btn.y + btn.h;
}

void drawMainMenu(Button buttons[]) {
    setbkcolor(BLACK);
    cleardevice();
    
    setcolor(LIGHTMAGENTA);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
    outtextxy(200, 100, "TYPESTRIKE");

    for (int i = 0; i < 3; ++i)
        drawButton(buttons[i]);
}

void drawRulesPage(Button& homeBtn) {
    cleardevice();
    settextstyle(BOLD_FONT, HORIZ_DIR, 1);
    setcolor(BLACK);
    
    int y = 50;
    outtextxy(50, y, "RULES:");
    y += 30;
    outtextxy(70, y, "1. Type words shown on screen.");
    y += 30;
    outtextxy(70, y, "2. Each correct word deals 1 damage to enemy.");
    y += 30;
    outtextxy(70, y, "3. Select two spells before game starts.");
    y += 30;
    outtextxy(70, y, "4. Spells can only be used once.");
    y += 30;
    outtextxy(70, y, "5. First to lose all HP is defeated.");

    y += 50;
    outtextxy(50, y, "SPELLS:");
    y += 30;
    outtextxy(70, y, "+ INVINCIBLE: Immune to damage for 10s. Half damage output.");
    y += 30;
    outtextxy(70, y, "+ BOOSTER: 120% damage after consistent typing.");
    y += 30;
    outtextxy(70, y, "+ DOUBLE DAMAGE: 2x damage for 10s.");
    y += 30;
    outtextxy(70, y, "+ AEGIS: Shield 15% HP.");
    y += 30;
    outtextxy(70, y, "+ MANIPULATION: Opponent gets harder words.");
    y += 30;
    outtextxy(70, y, "+ HEALING: Restore HP based on typing accuracy.");

    setfillstyle(SOLID_FILL, LIGHTRED);
    bar(homeBtn.x, homeBtn.y, homeBtn.x + homeBtn.w, homeBtn.y + homeBtn.h);
    setcolor(WHITE);
    outtextxy(homeBtn.x + 8, homeBtn.y + 8, "<"); 
}

void transition1()
{   
    char arr[]="TYPESTRIKE";
    string s="";
    for (int i=0; i<11; i++)
    {
        setfillstyle(SOLID_FILL, BLACK);
        setcolor(BLACK);
        bar(0, 0, 800, 600); 
        delay(125);
        setcolor(LIGHTMAGENTA);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
        outtextxy(200, 100, (char*)(s.c_str()));
        delay(150);
        s+=arr[i];
    }
}

void transition2()
{   
    setfillstyle(SOLID_FILL, BLACK);
    setcolor(BLACK);
    bar(0, 0, 800, 600); 
    delay(100);
    setcolor(LIGHTMAGENTA);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 20);
    outtextxy(390, 290, "3");
    delay(900);
    setfillstyle(SOLID_FILL, BLACK);
    setcolor(BLACK);
    bar(0, 0, 800, 600); 
    delay(100);
    setcolor(LIGHTMAGENTA);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 20);
    outtextxy(390, 290, "2");
    delay(900);
    setfillstyle(SOLID_FILL, BLACK);
    setcolor(BLACK);
    bar(0, 0, 800, 600); 
    delay(100);
    setcolor(LIGHTMAGENTA);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 20);
    outtextxy(390, 290, "1");
    delay(900);
}




