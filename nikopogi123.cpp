#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


//  ========================= COLORS =========================
    #define RED     "\033[31m"
    #define GREEN   "\033[32m"
    #define YELLOW  "\033[33m"
    #define BLUE    "\033[34m"
    #define RESET   "\033[0m"

// ========================= MAP SIZE =========================
    const int mapSize = 10; 

// ========================= TILE TYPES =========================
    enum Tile {EMPTY, PLAYER, EXIT, MONSTER, LOOT};
// ========================= STRUCT PLAYER =========================
    struct Player{
        int r, c; // R = row C = column
        int hp;
        int atk;
        int armor;
        int lifesteal;
    };
// ========================= MAP =========================
    Tile mapGrid[mapSize][mapSize];

// ========================= CLEAR THE MAP =========================
void clearMap(){
    for (int i = 0; i < mapSize; i++){
        for(int j = 0; j < mapSize; j++){
            mapGrid[i][j] = EMPTY;
        }
    }
}
// ========================= RANDOM TILE =========================
void randomTile(Tile t){
    int r, c;
    do{
        r = rand() % mapSize;
        c = rand() % mapSize;
    } while (mapGrid[r][c] != EMPTY);
    mapGrid[r][c] = t;
}
// ========================= DRAW MAP =========================
void drawMap(Player player, int difficulty){
    cout << endl;
    cout << GREEN << "P = PLAYER " << RESET;
    cout << RED << "M = MONSTER " << RESET;
    cout << YELLOW << "$ = LOOT " << RESET;
    cout << BLUE << "E = EXIT \n\n" << RESET;
    
    for (int i = 0; i < mapSize; i++){
        for(int j=0; j < mapSize; j++){

            if (difficulty == 3){
                int dr = abs(i - player.r);
                int dc = abs(j - player.c);

                if (dr > 1 || dc > 1){
                    cout << "? ";
                    continue;
                }
            }
            switch (mapGrid[i][j]){
                case PLAYER:
                    cout << GREEN << "P " << RESET;
                    break;
                case MONSTER:
                    cout << RED << "M " << RESET;
                    break;
                case LOOT:
                    cout << YELLOW << "$ " << RESET;
                    break;
                case EXIT:
                    cout << BLUE << "E " << RESET;
                    break;
                default:
                    cout << ". ";
                    break;
            }
        }
        cout << '\n';
    }
    
}
// ========================= INT MAIN =========================
int main(){
    srand((unsigned)time(nullptr));
    clearMap();

// ========================= DIFFICULTY =========================
    int difficulty;
    int playerHP;
    int monsterMinDamage;
    int monsterMaxDamage;
    int lootHeal;

    while (true) {
        cout << "===== SELECT MODES =====\n";
        cout << "1. Easy\n";
        cout << "2. Medium\n";
        cout << "3. Hard\n";
        cout << "Choice: ";
        cin >> difficulty;

        if (
            difficulty == 1 ||
            difficulty == 2 ||
            difficulty == 3
        ) {
            break;
        }

        cout << "\nInvalid choice. Try again.\n\n";
    }
switch (difficulty) {
        case 1:
            cout << "\nEasy Mode Selected\n";
            playerHP = 100;
            monsterMinDamage = 5;
            monsterMaxDamage = 15;
            lootHeal = 20;
            break;
        case 2:
            cout << "\nMedium Mode Selected\n";
            playerHP = 100;
            monsterMinDamage = 15;
            monsterMaxDamage = 30;
            lootHeal = 10;
            break;
        case 3:
            cout << "\nHard Mode Selected\n";
            playerHP = 100;
            monsterMinDamage = 30;
            monsterMaxDamage = 50;
            lootHeal = 20;
            break;
    }
// ========================= PLAYER =========================
    Player player;
    player.r = rand() % mapSize;
    player.c = rand() % mapSize;

    player.hp = playerHP;
    player.atk = 10;
    player.armor = 0;
    player.lifesteal = 0;

    mapGrid[player.r][player.c] = PLAYER;

    randomTile(EXIT);

    randomTile(MONSTER);
    randomTile(MONSTER);
    randomTile(MONSTER);

    randomTile(LOOT);
    randomTile(LOOT);
    randomTile(LOOT);

// ========================= GAME VARIABLES =========================
    bool running = true;
    int monstersRemaining = 3;

// ========================= GAME LOOP =========================
    while(running){
        cout << "\n=========================\n";
        cout << GREEN
             << "HP: "
             << player.hp
             << RESET
             << '\n';

        cout << "ATK: "
             << player.atk
             << '\n';
        
        cout << BLUE
             << "Armor: "
             << player.armor
             << RESET
             << '\n';

        cout << YELLOW
             << "Lifesteal: "
             << player.lifesteal
             << RESET
             << '\n';

        cout << RED
             << "Monster Left: "
             << monstersRemaining
             << RESET
             << '\n';

        drawMap(player, difficulty);
        cout << "\nMove (w/s/d/a), q to quit: ";
        char cmd;
        cin >> cmd;

        int nr = player.r;
        int nc = player.c;

// ========================= MOVEMENT =========================
        switch(cmd){
            case 'w':
                nr--;
                break;
            case 's':
                nr++;
                break;
            case 'a':
                nc--;
                break;
            case 'd':
                nc++;
                break;
            case 'q':
                running = false;
                continue;
            default:
            cout << "Invalid Command!\n";
            continue;
        }
// ========================= BOUNDARY CHECK =========================
        if (nr < 0 || nr >= mapSize ||
            nc < 0 || nc >= mapSize) {
            continue;
            }

// ========================= REMOVE OLD POSITION =========================
        mapGrid[player.r][player.c] = EMPTY;

// ========================= REMOVE OLD POSITION =========================
        if(mapGrid[nr][nc] == EXIT){
            if (monstersRemaining){
                cout << RED
                     << "\nThe exit is locked!"
                     << RESET;
                cout << "Defeat all the Monsters!\n";
                cout << "Monsters Remaining: " << monstersRemaining << "\n";
// ========================= PLAYER BACK =========================
                mapGrid[player.r][player.c] = PLAYER;
                continue;
            }
            else {
                cout << GREEN
                     << "\nYou beat all the monsters!\n"
                     << RESET;
                cout << GREEN
                     << "You escaped successfully. You Win!\n"
                     << RESET;
                running = false;
            }
        }
// ========================= MONSTERS =========================
        else if(mapGrid[nr][nc] == MONSTER){
            int monsterDamage = rand() % (monsterMaxDamage - monsterMinDamage + 1) + monsterMinDamage;

// ========================= Armor Damage =========================
            monsterDamage -= player.armor;
            if(monsterDamage < 0){
                monsterDamage = 0;
            }
            cout << RED
                 << "\nA monster attacks!\n"
                 << RESET;

            cout << "You deal "
                 << player.atk
                 << " damage.\n";
// ========================= LIFESTEAL =========================
            if(player.lifesteal > 0){
                player.hp += player.lifesteal;
                cout << YELLOW
                     << "Your lifesteal "
                     << player.lifesteal
                     << " HP.\n"
                     << RESET;
            }
            cout << RED
                 << "You take "
                 << monsterDamage
                 << " damage.\n"
                 << RESET;
            
            player.hp -= monsterDamage;
            mapGrid[nr][nc] = EMPTY;
            monstersRemaining--;

            if(player.hp <= 0){
                cout << RED
                     << "You died!\n"
                     << RESET;
                running = false;
            }
            else{
                player.r = nr;
                player.c = nc;
                mapGrid[player.r][player.c] = PLAYER;
            }
        }
// ========================= LOOT =========================
                else if(mapGrid[nr][nc] == LOOT){
                    int lootEvent = rand() % 5;

// ========================= HEAL =========================
                    if (lootEvent == 0){
                    cout << GREEN
                         << "\nYou found a loot! +"
                         << lootHeal
                         << " HP\n"
                         << RESET;
                    player.hp += lootHeal;
                }
// ========================= TRAP =========================
                else if (lootEvent == 1){
                    cout << RED
                         << "\n It's a trap! -10 HP\n";
                    player.hp -= 10;
                }
// ========================= BOMB ========================= 
                else if (lootEvent == 2){
                    cout << RED
                         << "\nBomb exploded! -20 HP\n"
                         << RESET;
                    player.hp -= 20;
                }
                else if (lootEvent == 3){
                    cout << RED
                         << "\nYou gained armor! +10 armor\n"
                         << RESET;
                    player.armor += 10;
                }
// ========================= LIFESTEAL =========================
                else {
                    cout << YELLOW
                         << "\nYou gained a lifesteal! +5 Lifesteal\n"
                         << RESET;
                    player.lifesteal += 5;
                }
                mapGrid[nr][nc] = EMPTY;
                if (player.hp <= 0){
                    cout << RED
                         << "You died! \n"
                         << RESET;
                    running = false;
                }
            else {
                player.r = nr;
                player.c = nc;
                mapGrid[player.r][player.c] = PLAYER;
            }
        }
    }

        cout << GREEN
             << "\nGame Over. Thanks for playing!\n"
             << RESET;

    return 0;
}