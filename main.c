#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

typedef enum {
    none            =   0,
    doubleStrike    =   1 << 0,
    flight          =   1 << 1,
    reach           =   1 << 2,
    win             =   1 << 3
}Special;

typedef enum {
    anyM    =   1,
    windM   =   10,
    fireM   =   100,
    earthM  =   1000,
    waterM  =   10000,
    anyC    =   -1,
    windC   =   -10,
    fireC   =   -100,
    earthC  =   -1000,
    waterC  =   -10000
}Mana;

typedef enum {
    mana,
    monster,
    spell
}Type;

typedef struct cards
{
    char name[128];
    Type type;
    Mana cost;
    int dmg;
    int hp;
    Special abilities;
}Cards;

int main() {
    int winW = 1280, winH = 720;
    InitWindow(winW, winH, "card game");
    SetTargetFPS(60);

    int manaCount = 0;
    char manaCountStr[10];

    Cards deck[60] = {
    {"Goblin", monster, (fireC), 2, 2, none},
    {"Ent", monster, (3 * earthC + anyC), 3, 4, reach},
    {"Eagle", monster, (2 * windC), 1, 1, flight},
    {"Island", mana, waterM, 0, 0, none},
    {"Island", mana, waterM, 0, 0, none},
    {"Island", mana, waterM, 0, 0, none},
    {"Island", mana, waterM, 0, 0, none},
    {"Island", mana, waterM, 0, 0, none},
    {"Forest", mana, earthM, 0, 0, none},
    {"Forest", mana, earthM, 0, 0, none},
    {"Forest", mana, earthM, 0, 0, none},
    {"Forest", mana, earthM, 0, 0, none},
    {"Forest", mana, earthM, 0, 0, none},
    {"Desert", mana, fireM, 0, 0, none},
    {"Desert", mana, fireM, 0, 0, none},
    {"Desert", mana, fireM, 0, 0, none},
    {"Desert", mana, fireM, 0, 0, none},
    {"Desert", mana, fireM, 0, 0, none},
    {"Mountain", mana, windM, 0, 0, none},
    {"Mountain", mana, windM, 0, 0, none},
    {"Mountain", mana, windM, 0, 0, none},
    {"Mountain", mana, windM, 0, 0, none},
    {"Mountain", mana, windM, 0, 0, none},
    {"Wasteland", mana, anyM, 0, 0, none},
    {"I win", spell, (5 * (anyC + windC + fireC + earthC + waterC)), 0, 0, win}
    };

    Cards hand[60] = {};
    Vector2 mouseV = GetMousePosition();
    Rectangle plusAllMana = {40, 40, 40, 40};

    while (!WindowShouldClose()) {
        mouseV = GetMousePosition();
        if (CheckCollisionPointRec(mouseV, plusAllMana) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            manaCount += 11111;
        }
        
        sprintf(manaCountStr, "%d", manaCount);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangleRec(plusAllMana, RED);
            DrawText(manaCountStr, 1000, 40, 18, BLACK); 
        EndDrawing();
    }
    CloseWindow();
    return 0;
}