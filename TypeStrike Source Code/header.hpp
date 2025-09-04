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
    #include "header.h"
    using namespace std;
    
    GraphicsRenderer::GraphicsRenderer(int width, int height) : screenWidth(width), screenHeight(height), lastInput("") {
        initwindow(width, height, "TypeStrike");
        setbkcolor(BLACK);
        cleardevice();
    }
    
    GraphicsRenderer::~GraphicsRenderer() {
        closegraph();
    }
    
    void GraphicsRenderer::drawHealthBar(int x, int y, int width, int height, double percentage, int color, const string& label) {
        // Draw label
        setcolor(WHITE);
        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
        outtextxy(x - 100, y + 5, (char*)label.c_str());
        
        // Draw health bar border
        setcolor(WHITE);
        rectangle(x, y, x + width, y + height);
        
        // Fill health bar
        int fillWidth = (int)(width * percentage);
        setfillstyle(SOLID_FILL, color);
        bar(x + 1, y + 1, x + fillWidth, y + height - 1);
        
    }
    
    void GraphicsRenderer::drawPixelLaptop(int x, int y, int color) {
        // Laptop Base
        setcolor(color);
        setfillstyle(SOLID_FILL, color);
        bar(x, y + 20, x + 60, y + 35);
        
        // Laptop Screen
        setcolor(LIGHTGRAY);
        setfillstyle(SOLID_FILL, LIGHTGRAY);
        bar(x + 5, y, x + 55, y + 25);
        
        // Inner Laptop Screen 
        setcolor(BLUE);
        setfillstyle(SOLID_FILL, BLUE);
        bar(x + 7, y + 2, x + 53, y + 23);
        
        // Laptop Keyboard
        setcolor(DARKGRAY);
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 8; j++) {
                rectangle(x + 8 + j*5, y + 27 + i*2, x + 11 + j*5, y + 28 + i*2);
            }
        }
        
        // Laptop Screen Details
        setcolor(WHITE);
        line(x + 10, y + 5, x + 50, y + 5);
        line(x + 10, y + 8, x + 45, y + 8);
        line(x + 10, y + 11, x + 50, y + 11);
    }
    
    void GraphicsRenderer::drawRobot(int x, int y) {
        // Robot Head
        setcolor(LIGHTCYAN);
        setfillstyle(SOLID_FILL, LIGHTCYAN);
        bar(x, y, x + 30, y + 25);
        
        // Robot Eyes
        setcolor(RED);
        setfillstyle(SOLID_FILL, RED);
        fillellipse(x + 8, y + 8, 3, 3);
        fillellipse(x + 22, y + 8, 3, 3);
        
        // Robot Body
        setcolor(CYAN);
        setfillstyle(SOLID_FILL, CYAN);
        bar(x - 5, y + 25, x + 35, y + 50);
        
        // Robot Arms
        setcolor(LIGHTCYAN);
        bar(x - 15, y + 30, x, y + 40);
        bar(x + 30, y + 30, x + 45, y + 40);
        
        // Robot Antenna
        setcolor(WHITE);
        line(x + 15, y, x + 15, y - 10);
        fillellipse(x + 15, y - 12, 2, 2);
    }
    
    void GraphicsRenderer::drawBullets(int startX, int startY, int endX, int endY) {
        setcolor(YELLOW);
        // Draw multiple small bullets
        for(int i = 0; i < 3; i++) {
            int bulletX = startX + (endX - startX) * (0.3 + i * 0.2);
            int bulletY = startY + (endY - startY) * (0.3 + i * 0.2);
            fillellipse(bulletX, bulletY, 3, 2);
        }
    }
    
    void GraphicsRenderer::updateInputArea(const string& currentInput) {
        if (currentInput == lastInput) return;
        
        // Clear input area after inputting one word
        setfillstyle(SOLID_FILL, BLACK);
        bar(50, 440, 750, 470);
        
        // Redraw input area
        setcolor(YELLOW);
        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
        string inputDisplay = "Input: " + currentInput + "_";
        outtextxy(50, 450, (char*)inputDisplay.c_str());
        
        lastInput = currentInput;
    }
    
    struct Star {
        int x;
        int y;
    };

    vector<Star> stars;

    void GraphicsRenderer::initializeStars() {
    stars.clear();
    int cols = 20, rows = 15;
    int cellW = screenWidth / cols;
    int cellH = screenHeight / rows;

    for (int i = 0; i < cols; ++i) {
        for (int j = 0; j < rows; ++j) {
            if (rand() % 100 < 60) {
                int offsetX = rand() % (cellW / 2);
                int offsetY = rand() % (cellH / 2);
                Star s;
                s.x = i * cellW + offsetX;
                s.y = j * cellH + offsetY;
                stars.push_back(s);
            }
        }
    }
}

    void GraphicsRenderer::drawGameInterface(double playerHP, double enemyHP, const vector<string>& words, 
                          const string& currentInput, int playerScore, double accuracy, bool forceRedraw) {
        
        // Partial update for input changes
        if (!forceRedraw) {
            updateInputArea(currentInput);
            return;
        }
        
        cleardevice();
        
        // Background gradient effect
        for(int y = 0; y < screenHeight; y++) {
            int colorIntensity = 20 + (y * 15) / screenHeight;
            setcolor(COLOR(0, 0, colorIntensity));
            line(0, y, screenWidth, y);
        }

        // Draw stars
        initializeStars(); 
        for (const Star& s : stars) {
            putpixel(s.x, s.y, WHITE); 
        }

        // Draw health bars
        drawHealthBar(250, 50, 300, 25, playerHP, GREEN, "YOUR HP");
        drawHealthBar(250, 100, 300, 25, enemyHP, RED, "ENEMY HP");
        
        // Draw players
        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
        setcolor(YELLOW);
        outtextxy(100, 200, "PLAYER 1");
        drawPixelLaptop(100, 230, LIGHTBLUE);
        
        setcolor(LIGHTCYAN);
        outtextxy(600, 200, "PLAYER 2");
        drawRobot(620, 230);
        
        // Draw bullets between players (occasional animation)
        if(rand() % 20 == 0) {
            drawBullets(160, 250, 620, 250);
        }
        
        // Draw attack instruction box
        setcolor(WHITE);
        rectangle(200, 350, 600, 390);
        setfillstyle(SOLID_FILL, DARKGRAY);
        bar(201, 351, 599, 389);
        
        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
        setcolor(WHITE);
        outtextxy(280, 365, "TYPE WORD TO ATTACK...");
        
        // Draw available words
        setcolor(LIGHTGREEN);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        string wordDisplay = "Words: ";
        for(size_t i = 0; i < words.size() && i < 8; i++) {
            wordDisplay += words[i];
            if(i < words.size() - 1 && i < 7) wordDisplay += " ";
        }
        outtextxy(50, 420, (char*)wordDisplay.c_str());
        
        // Draw current input
        updateInputArea(currentInput);
        
        // Draw stats
        setcolor(WHITE);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        char scoreStr[50], accuracyStr[50];
        sprintf(scoreStr, "Score: %d", playerScore);
        sprintf(accuracyStr, "Accuracy: %.1f%%", accuracy);
        
        outtextxy(50, 500, scoreStr);
        outtextxy(200, 500, accuracyStr);
        
        // Draw game title
        settextstyle(BOLD_FONT, HORIZ_DIR, 3);
        setcolor(LIGHTRED);
        outtextxy(210, 10, "TYPE FAST, STRIKE HARD!");

    }
    
    
    void GraphicsRenderer::drawGameOver(bool playerWon, int finalScore, double accuracy, double wpm, double cpm, double totalTimeSeconds) {
        cleardevice();
        
        // Draw background
        setbkcolor(BLACK);
        cleardevice();
        
        // Draw title
        settextstyle(BOLD_FONT, HORIZ_DIR, 4);
        if(playerWon) {
            setcolor(GREEN);
            outtextxy(220, 100, "VICTORY!");
        } else {
            setcolor(RED);
            outtextxy(240, 100, "DEFEAT!");
        }
        
        // Draw final stats header
        settextstyle(BOLD_FONT, HORIZ_DIR, 2);
        setcolor(WHITE);
        outtextxy(280, 180, "FINAL STATS");
        
        // Draw divider line
        setcolor(YELLOW);
        line(200, 210, 600, 210);
        
        // Prepare stat strings
        char scoreStr[60], accuracyStr[60], wpmStr[60], cpmStr[60], timeStr[60];
        sprintf(scoreStr, "Final Score: %d", finalScore);
        sprintf(accuracyStr, "Accuracy: %.1f%%", accuracy);
        sprintf(wpmStr, "WPM: %.1f", wpm);
        sprintf(cpmStr, "CPM: %.1f", cpm);
        
        // Calculate time stats
        int minutes = (int)(totalTimeSeconds / 60);
        int seconds = (int)(totalTimeSeconds) % 60;
        int milliseconds = (int)((totalTimeSeconds - (int)totalTimeSeconds) * 100);
        sprintf(timeStr, "Total Time: %d:%02d.%02d", minutes, seconds, milliseconds);
        
        // Draw end game stats
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        int yPos = 240;
        int lineSpacing = 35;
        
        // Score
        setcolor(LIGHTGREEN);
        outtextxy(250, yPos, scoreStr);
        yPos += lineSpacing;
        
        // Accuracy
        setcolor(LIGHTCYAN);
        outtextxy(250, yPos, accuracyStr);
        yPos += lineSpacing;
        
        // WPM
        setcolor(YELLOW);
        outtextxy(250, yPos, wpmStr);
        yPos += lineSpacing;
        
        // CPM
        setcolor(LIGHTMAGENTA);
        outtextxy(250, yPos, cpmStr);
        yPos += lineSpacing;
        
        // Total Time
        setcolor(WHITE);
        outtextxy(250, yPos, timeStr);
        
        // Draw exit instruction
        setcolor(YELLOW);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        outtextxy(300, 450, "Press ESC to exit...");
        
        // Draw border
        setcolor(WHITE);
        rectangle(180, 90, 620, 480);
        rectangle(182, 92, 618, 478);
    }
    
    bool GraphicsRenderer::isKeyPressed() {
        return kbhit();
    }
    
    char GraphicsRenderer::getKey() {
        return getch();
    }

    HealthBar::HealthBar(int maxHp) : maxHealth(maxHp), currentHealth(maxHp) {}
    
    void HealthBar::takeDamage(int damage) {
        currentHealth = max(0, currentHealth - damage);
    }
    
    void HealthBar::heal(int amount) {
        currentHealth = min(maxHealth, currentHealth + amount);
    }
    
    bool HealthBar::isAlive() const {
        return currentHealth > 0;
    }
    
    int HealthBar::getCurrentHealth() const { return currentHealth; }
    int HealthBar::getMaxHealth() const { return maxHealth; }
    
    double HealthBar::getHealthPercentage() const {
        return (double)currentHealth / maxHealth;
    }


    Player::Player(const string& playerName, int maxHp) 
        : name(playerName), healthBar(maxHp), score(0), correctWords(0), totalWords(0), 
          totalCharactersTyped(0), gameEnded(false) {
        gameStartTime = chrono::steady_clock::now();
    }
    
    void Player::takeDamage(int damage) { healthBar.takeDamage(damage); }
    void Player::heal(int amount) { healthBar.heal(amount); }
    void Player::addScore(int points) { score += points; }
    void Player::incrementCorrectWords() { correctWords++; totalWords++; }
    void Player::incrementTotalWords() { totalWords++; }
    void Player::addCharactersTyped(int chars) { totalCharactersTyped += chars; }
    
    void Player::endGame() {
        if (!gameEnded) {
            gameEndTime = chrono::steady_clock::now();
            gameEnded = true;
        }
    }
    
    bool Player::isAlive() const { return healthBar.isAlive(); }
    string Player::getName() const { return name; }
    int Player::getScore() const { return score; }
    HealthBar& Player::getHealthBar() { return healthBar; }
    
    double Player::getAccuracy() const {
        if (totalWords == 0) return 0.0;
        return (double)correctWords / totalWords * 100.0;
    }
    
    double Player::getTotalTimeSeconds() const {
        auto endTime = gameEnded ? gameEndTime : chrono::steady_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(endTime - gameStartTime);
        return duration.count() / 1000.0;
    }
    
    double Player::getWPM() const {
        double timeMinutes = getTotalTimeSeconds() / 60.0;
        if (timeMinutes <= 0) return 0.0;
        return correctWords / timeMinutes;
    }
    
    double Player::getCPM() const {
        double timeMinutes = getTotalTimeSeconds() / 60.0;
        if (timeMinutes <= 0) return 0.0;
        return totalCharactersTyped / timeMinutes;
    }

    WordPool::WordPool() : currentIndex(0) {}
    
    void WordPool::loadWords(const vector<string>& dictionary) {
        allWords = dictionary;
        shuffle(allWords.begin(), allWords.end(), mt19937(random_device{}()));
        currentIndex = 0;
    }
    
    vector<string> WordPool::getCurrentWords(int count) {
        vector<string> current;
        for(int i = 0; i < count && (currentIndex + i) < allWords.size(); i++) {
            current.push_back(allWords[currentIndex + i]);
        }
        return current;
    }
    
    bool WordPool::checkCurrentWord(const string& word) {
        if(currentIndex < allWords.size() && word == allWords[currentIndex]) {
            currentIndex++;
            return true;
        }
        return false;
    }
    
    bool WordPool::isEmpty() const {
        return currentIndex >= allWords.size();
    }
    
    int WordPool::getRemainingWords() const {
        return allWords.size() - currentIndex;
    }
    
    string WordPool::getCurrentWord() const {
        if(currentIndex < allWords.size()) {
            return allWords[currentIndex];
        }
        return "";
    }


    Game::Game(const string& p1Name, GraphicsRenderer* existingRenderer) 
        : player1(p1Name), player2("AI Bot"), gameRunning(true), currentInput(""), renderer(existingRenderer) {
        
        botDamageInterval = 5000; // bot deals dmg every 5s
        botDamageAmount = 4; // bot deals 4 dmg
        playerDamagePerWord = 1; // player deals 1 dmg
        lastBotAttack = chrono::steady_clock::now();
        lastFrameTime = chrono::steady_clock::now();
        
        loadDictionary();
    }
    
    Game::~Game() {}
    
    void Game::loadDictionary() {
        ifstream file("wordgeneratortest1.txt");
        string word;
        while (file >> word) {
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (word.length() >= 3 && word.length() <= 8 && isalpha(word[0])) {
                dictionary.push_back(word);
            }
        }
        
        // if file not found
        if (dictionary.empty()) {
            dictionary = {"file", "not", "found", "file", "not", "found", "file", "not", "found", "hey",
                          "file", "not", "found", "file", "not", "found", "file", "not", "found", "hey",
                          "file", "not", "found", "file", "not", "found", "file", "not", "found", "hey",
                          "file", "not", "found", "file", "not", "found", "file", "not", "found", "hey",
                          "file", "not", "found", "file", "not", "found", "file", "not", "found", "hey",};
        }
        
        // Load words into word pool
        wordPool.loadWords(dictionary);
    }
    
    bool Game::updateBotAI() {
        auto currentTime = chrono::steady_clock::now();
        auto timeSinceLastAttack = chrono::duration_cast<chrono::milliseconds>(currentTime - lastBotAttack);
        
        if (timeSinceLastAttack.count() >= botDamageInterval) {
            player1.takeDamage(botDamageAmount);
            lastBotAttack = currentTime;
            return true;
        }
        return false;
    }
    
    void Game::processWord(const string& word) {
        if (word.empty()) return;
        
        // Track characters typed
        player1.addCharactersTyped(word.length());
        
        // Check if it matches the current word
        if (wordPool.checkCurrentWord(word)) {
            player2.takeDamage(playerDamagePerWord);
            player1.addScore(playerDamagePerWord);
            player1.incrementCorrectWords();
            
            if (!player2.isAlive()) {
                gameRunning = false;
            }
        } else {
            player1.incrementTotalWords(); // Count as wrong word
        }
    }
    
    void Game::gameLoop() {
        bool needsFullRedraw = true;
        
        // Full redraw game interface by setting forceRedraw=true
        renderer->drawGameInterface(player1.getHealthBar().getHealthPercentage(),
                                    player2.getHealthBar().getHealthPercentage(),
                                    wordPool.getCurrentWords(), currentInput,
                                    player1.getScore(), player1.getAccuracy(), true);
        
        while (gameRunning && player1.isAlive() && player2.isAlive() && !wordPool.isEmpty()) {
            auto currentTime = chrono::steady_clock::now();
            
            bool botAttacked = updateBotAI();
            bool significantChange = false;
            
            if (!player1.isAlive()) {
                gameRunning = false;
                break;
            }
            
            // Handle input by user
            if (renderer->isKeyPressed()) {
                char ch = renderer->getKey();
                if (ch == 27) 
                exit(1);

                if (ch == ' ') {
                    if (!currentInput.empty()) {
                        string word = currentInput;
                        transform(word.begin(), word.end(), word.begin(), ::tolower);
                        processWord(word);
                        currentInput = "";
                        significantChange = true; // Force full redraw after an input by spacebar
                    }
                } else if (ch == '\b' || ch == 127) {
                    if (!currentInput.empty()) {
                        currentInput.pop_back();
                        // Partial updating input area graphics by setting forceRedraw=false
                        renderer->drawGameInterface(
                            player1.getHealthBar().getHealthPercentage(),
                            player2.getHealthBar().getHealthPercentage(),
                            wordPool.getCurrentWords(),
                            currentInput,
                            player1.getScore(),
                            player1.getAccuracy(),
                            false 
                        );
                    }
                } else if (isalpha(ch)) {
                    currentInput += tolower(ch);
                    // Track character input for CPM calculation
                    player1.addCharactersTyped(1);
                    renderer->drawGameInterface(
                        player1.getHealthBar().getHealthPercentage(),
                        player2.getHealthBar().getHealthPercentage(),
                        wordPool.getCurrentWords(),
                        currentInput,
                        player1.getScore(),
                        player1.getAccuracy(),
                        false 
                    );
                }
            }
            
            // Only do full redraw for bot attack
            if (significantChange || botAttacked) {
                renderer->drawGameInterface(
                    player1.getHealthBar().getHealthPercentage(),
                    player2.getHealthBar().getHealthPercentage(),
                    wordPool.getCurrentWords(),
                    currentInput,
                    player1.getScore(),
                    player1.getAccuracy(),
                    true 
                );
            }
            
            // Frame rate limiting
            auto frameTime = chrono::duration_cast<chrono::milliseconds>(chrono::steady_clock::now() - currentTime);
            if (frameTime.count() < FRAME_TIME_MS) {
                delay(FRAME_TIME_MS - frameTime.count());
            }
        }
        
        // End the game and calculate final statistics
        player1.endGame();
        
        // Game over screen with final statistics
        renderer->drawGameOver(
            player1.isAlive(), 
            player1.getScore(), 
            player1.getAccuracy(),
            player1.getWPM(),
            player1.getCPM(),
            player1.getTotalTimeSeconds()
        );
        
        // if esc is pressed, end the game
        while (1) {
            if (renderer->getKey()==27)
            {
                delay(100);
                break;
            }
        }
    }

    double Game::getWPM() const{
        return player1.getWPM();
    }