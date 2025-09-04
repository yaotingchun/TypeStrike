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

class GraphicsRenderer{
private:
    int screenWidth, screenHeight;
    string lastInput;

public:
    GraphicsRenderer(int width = 800, int height = 600);
    ~GraphicsRenderer();
    void initializeStars();
    void drawHealthBar(int x, int y, int width, int height, double percentage, int color, const string& label);
    void drawPixelLaptop(int x, int y, int color);
    void drawRobot(int x, int y);
    void drawBullets(int startX, int startY, int endX, int endY);
    void updateInputArea(const string& currentInput);
    void drawGameInterface(double playerHP, double enemyHP, const vector<string>& words, 
                           const string& currentInput, int playerScore, double accuracy,
                           bool forceRedraw = false);
    void drawGameOver(bool playerWon, int finalScore, double accuracy, double wpm, double cpm, double totalTimeSeconds);
    bool isKeyPressed();
    char getKey();
};

class HealthBar {
private:
    int maxHealth;
    int currentHealth;
    
public:
    HealthBar(int maxHp = 100);
    void takeDamage(int damage);
    void heal(int amount);
    bool isAlive() const;
    int getCurrentHealth() const;
    int getMaxHealth() const;
    double getHealthPercentage() const;
};

class Player {
private:
    string name;
    HealthBar healthBar; // Compostion
    int score;
    int correctWords;
    int totalWords;
    int totalCharactersTyped;
    chrono::steady_clock::time_point gameStartTime;
    chrono::steady_clock::time_point gameEndTime;
    bool gameEnded;
    
public:
    Player(const string& playerName, int maxHp = 100);
    void takeDamage(int damage);
    void heal(int amount);
    void addScore(int points);
    void incrementCorrectWords();
    void incrementTotalWords();
    void addCharactersTyped(int chars);
    void endGame();
    bool isAlive() const;
    string getName() const;
    int getScore() const;
    HealthBar& getHealthBar();
    double getAccuracy() const;
    double getTotalTimeSeconds() const;
    double getWPM() const;
    double getCPM() const;
};

class WordPool {
private:
    vector<string> allWords;
    size_t currentIndex;
    
public:
    WordPool();
    void loadWords(const vector<string>& dictionary);
    vector<string> getCurrentWords(int count = 10);
    bool checkCurrentWord(const string& word);
    bool isEmpty() const;
    int getRemainingWords() const;
    string getCurrentWord() const;
};

class Game {
private:
    Player player1; // Composition
    Player player2; 
    vector<string> dictionary;
    WordPool wordPool; // Composition
    bool gameRunning;
    string currentInput;
    GraphicsRenderer* renderer; //Aggregation
    
    chrono::steady_clock::time_point lastBotAttack;
    int botDamageInterval;
    int botDamageAmount;
    int playerDamagePerWord;
    
    chrono::steady_clock::time_point lastFrameTime;
    const int TARGET_FPS = 30;
    const int FRAME_TIME_MS = 1000 / TARGET_FPS;
    
public:
    Game(const string& p1Name, GraphicsRenderer* existingRenderer);
    ~Game();
    void loadDictionary();
    bool updateBotAI();
    void processWord(const string& word);
    void gameLoop();
    double getWPM() const;
};

// Inheritance implementation (Not yet finished)

class Spell
{
	private:
		string name;					// name of spell
		string desc;					// description of spell
		int duration;					// duration of spell, each spell have distinct duration.
		bool isActive;					// is the spell active
		int remainingTime;				// the remaining time of spell
		
	public:
		Spell(string, string, int);		// Constructor
		void activate(Player);			// The spell is activated, update on player
		void deactivate(Player);		// The spell is deactivated, update on player.
		void updateDuration(int);		// Continuously updates the duration of spell.
		bool checkStatus();				// Check is the spell activate or not.
		double getDuration();			// Constantly the remaining duration of spell.
};

class DoubleDamage: public virtual Spell
{
	private:
		double dmgMutli;				// Multiplier (2.0)
		
	public:
		DoubleDamage(double, int);		// Constructor
		void activate(Player);			
		int calcDoubleDmg(int);			// Calculate the updated damage.
		~DoubleDamage();				// Destructor
};

class Healing: public virtual Spell		// Heal based on how many words the player typed, reduced damage dealt by enemy for x % during period.
{
	private:
		double dmgReduction;			// Enemy damage reduction rate
		int healAmt;					// Calculated heal amount
		
	public:
		Healing(int, double, int);		// Constructor
		void activate(Player);			// 
		int calcHealing(WordPool);		// Calculate total heal based on words typed correctly (Word Class)
		void applyHeal(Player);			// Apply heal to player, update the Healthbar Class
		~Healing();						// Destructor
};

class Booster: public virtual Spell		// Deals x % damage if typed correct words consecutively for n times.
{
	private:
		double dmgMulti;				// Damage multiplier percentage
		int requiredStreak;				// How many consecutive words is needed
		int currentStreak;				// Current correct streak
		
	public:
		Booster(double, int, int);		// Constructor
		void activate(Player);
		void incStreak();				// Increment currentStreak by 1
		bool isActive();				// check is the booster still active by observing the streak
		~Booster();						// Destructor
};

class Invincible: public virtual Spell	// Immune to damage dealt by enemy, reduce player's damage dealt to enemy.
{
	private:
		bool isImmune;					// Immunity status
		double reductionRate;			// Reduced damage by x%
		
	public:
		Invincible(int, int);			// Constructor
		void activate(Player);			// 
		int calcDamage();				// Calculate reduced damage
		bool checkImmunity();			// get status of immunity
};

class Aegis: public virtual Spell		// Gain shield based on health loss percentage, shield is used up before hp.
{
	private:
		double shieldPercentage;		// Shield multiplier
		int shieldAmt;					// Calculated shield amount given
		
	public:
		Aegis(double, int);				// Constructor
		void activate(Player);			// 
		int calcShield(int);			// Calculate shield amount based on current HP / HP loss.
};