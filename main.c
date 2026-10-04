#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

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
    Color color;
    Special abilities;
}Cards;

int main() {
    int winW = 1280, winH = 720;
    InitWindow(winW, winH, "card game");
    SetTargetFPS(60);

    //stat counters
    int manaCount = 0;
    int handCount = 0;
    int deckCount = 25;
    struct {
        int mana;
        int creature;
        int sum;
    } playCount = {0, 0, 0};

    //card size
    int mm = 2;
    int cardW = 63 * mm, cardH = 88 * mm;

    //true/false parameters
    int chosen = -1;
    int manaChosen = -1;
    int place = -1;
    int discarding = 0;
    int holding = 0;
    
    //card drawing extras
    int extra = 0;
    int cardX = 0;
    int cardY = 0;

    //stat strings
    char hpC[4];
    char dmgC[4];
    char manaCountStr[10];

    //randomness generator
    time_t randomiser = time(NULL);


    //card arrays for different states (deck, hand, played)
    Cards deck[60] = {
    {"Goblin", monster, (fireC), 2, 2, RED, none},
    {"Ent", monster, (3 * earthC + anyC), 3, 4, GREEN, reach},
    {"Eagle", monster, (2 * windC), 1, 1, WHITE, flight},
    {"Island", mana, waterM, 0, 0, BLUE, none},
    {"Island", mana, waterM, 0, 0, BLUE, none},
    {"Island", mana, waterM, 0, 0, BLUE, none},
    {"Island", mana, waterM, 0, 0, BLUE, none},
    {"Island", mana, waterM, 0, 0, BLUE, none},
    {"Forest", mana, earthM, 0, 0, GREEN, none},
    {"Forest", mana, earthM, 0, 0, GREEN, none},
    {"Forest", mana, earthM, 0, 0, GREEN, none},
    {"Forest", mana, earthM, 0, 0, GREEN, none},
    {"Forest", mana, earthM, 0, 0, GREEN, none},
    {"Desert", mana, fireM, 0, 0, RED, none},
    {"Desert", mana, fireM, 0, 0, RED, none},
    {"Desert", mana, fireM, 0, 0, RED, none},
    {"Desert", mana, fireM, 0, 0, RED, none},
    {"Desert", mana, fireM, 0, 0, RED, none},
    {"Mountain", mana, windM, 0, 0, WHITE, none},
    {"Mountain", mana, windM, 0, 0, WHITE, none},
    {"Mountain", mana, windM, 0, 0, WHITE, none},
    {"Mountain", mana, windM, 0, 0, WHITE, none},
    {"Mountain", mana, windM, 0, 0, WHITE, none},
    {"Wasteland", mana, anyM, 0, 0, GRAY, none},
    {"I win", spell, (5 * (anyC + windC + fireC + earthC + waterC)), 0, 0, MAGENTA, win}
    };
    Cards hand[60] = {};
    Cards played[60] = {};

    //mouse position
    Vector2 mouseV = GetMousePosition();

    //debug rectangles
    Rectangle plusAllManaRec = {40, 40, 40, 40};
    Rectangle suffleRec = {winW - 80, 40, 40, 40};
    Rectangle startGame = {winW - 140, 40, 40, 40};

    //drawing and discarding rectangles
    Rectangle deckRec = {winW - cardW - 40, winH - cardH - 40, cardW, cardH};
    Rectangle discardRec = {winW - cardW - 40, winH - 30, cardW, 20};
    
    //cards in hand rectangles
    Rectangle chosenRec = {0, 0, 0, 0};
    Rectangle bufferRec = {0, 0, cardW, cardH};
    Rectangle handRec = {40, winH - cardH - 40, (winW - 80 - 2 * cardW), cardH};

    //played cards rectangles
    Rectangle playRec = {40, winH / 4 - 40, winW - 120 - cardW, 2 * cardH + 40};
    Rectangle manaRec = {40, winH / 4 + cardH, winW - 120 - cardW, cardH};
    Rectangle creatureRec = {40, winH / 4 - 40, winW - 120 - cardW, cardH};

    while (!WindowShouldClose()) {
        mouseV = GetMousePosition();
    
        //card holding
        if (chosen != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            holding = 1;
        } else {
            holding = 0;
        }

        //card choice
        if (CheckCollisionPointRec(mouseV, handRec) && holding != 1) {
            for (int i = handCount - 1; i > -1; i--) {
                bufferRec.x = 40 + ((winW - 80 - 2 * cardW) / ((handCount > 1) ? handCount : 1) * i);
                bufferRec.y = winH - cardH - 40;
                bufferRec.width = cardW;
                bufferRec.height = cardH;
                if (CheckCollisionPointRec(mouseV, bufferRec)) {
                    chosen = i;
                    break;
                }
            }
        }
        //card unchoose
        else if (holding != 1) {
            chosen = -1;
        } 

        //card playing
        if (holding == 1 && hand[chosen].type == mana && CheckCollisionPointRec(mouseV, playRec)) {
            place = chosen;
        /*} else if (holding == 1 && hand[chosen].type == monster && CheckCollisionPointRec(mouseV, playRec)) {
            place = chosen;*/
        }else if (holding == 1) {
            place = -1;
        }

        //place card
        if(CheckCollisionPointRec(mouseV, playRec) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && place > -1) {
            played[playCount.sum] = hand[place];
            playCount.sum++;
            if (played[playCount.sum].type == mana) {
                playCount.mana++;
            } else if (played[playCount.sum].type == monster){
                playCount.creature++;
            }
            for (int i = place; i < handCount; i++) {
                hand[i] = hand[i + 1];
            }
            handCount--;
            place = -1;
        }
        //draw card
        if (CheckCollisionPointRec(mouseV, deckRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            hand[handCount] = deck[0];
            handCount++;
            deckCount--;
            for (int i = 0; i < deckCount; i++) {
                deck[i] = deck[i + 1];
            }
        }
        //activate discard
        if (CheckCollisionPointRec(mouseV, discardRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            discarding = (discarding == 0) ? 1 : 0;
        }
        //hand discard
        if (CheckCollisionPointRec(mouseV, bufferRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            if (discarding == 1 && chosen != -1) {
                for (int i = chosen; i < handCount - 1; i++) {
                    hand[i] = hand[i + 1];
                }
                chosen = -1;
                handCount--;
                discarding = 0;
            } 
        }  
        
        //deck shuffle using the Fisher–Yates shuffle
        if (CheckCollisionPointRec(mouseV, suffleRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = deckCount - 1; i > 0; i--) {
                int j = rand() * randomiser % (i + 1);
                Cards temp = deck[j];
                deck[j] = deck[i];
                deck[i] = temp;
            }
        }
        
        //game starting function, shuffles the deck and draws 7 cards
        if (CheckCollisionPointRec(mouseV, startGame) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = deckCount - 1; i > 0; i--) {
                int j = rand() * time(NULL) % (i + 1);
                Cards temp = deck[j];
                deck[j] = deck[i];
                deck[i] = temp;
            }
            for (int i = 0; i < 7; i++) {
                hand[handCount] = deck[0];
                handCount++;
                deckCount--;
                for (int i = 0; i < deckCount; i++) {
                    deck[i] = deck[i + 1];
                }
            }
        }

        //give the player all types of mana (DEBUGGING)
        if (CheckCollisionPointRec(mouseV, plusAllManaRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            manaCount += 11111;
        }

        //debug strings
        sprintf(manaCountStr, "%d", manaCount);

        BeginDrawing();
            //CLEAR BACKGROUND
            ClearBackground(LIGHTGRAY);

            //DRAWING OF GAME BUTTONS
            {if (holding == 1) {
                DrawRectangleLinesEx(playRec, 4, BLACK);
            }
            DrawRectangleRec(deckRec, BLACK);
            DrawRectangleRec(discardRec, RED);
            if (discarding == 1) {
                DrawText("Cancel", discardRec.x + (discardRec.width / 2) - MeasureText("Cancel", 12) / 2, discardRec.y + (discardRec.height / 2 - 6), 12, BLACK);
            }}

            //DRAWING OF CARDS ON PLAYFIELD
            for (int i = 0; i < playCount.sum; i++) {
                if (played[i].type == mana) {
                    if (manaChosen == i && holding != 1) {
                        extra = -20;
                    } else {
                        extra = 0;
                    }
                    DrawRectangle(40 + ((winW - 120 - cardW) / playCount.mana * i), winH / 3 + cardH + extra, cardW, cardH, played[i].color);
                    DrawText(played[i].name, 40 + ((winW - 120 - cardW) / playCount.mana * i) + 4, winH / 3 + cardH + 4 + extra, 8, BLACK);
                } else {
                    DrawRectangle(40 + ((winW - 120 - cardW) / playCount.creature * i), winH / 3 + cardH, cardW, cardH, played[i].color);
                    DrawText(played[i].name, 40 + ((winW - 120 - cardW) / playCount.creature * i) + 4, winH / 3 + cardH + 4, 8, BLACK);
                }
            }
            
            //DRAWING OF CARDS IN HAND
            for (int i = 0; i < handCount; i++) {
                sprintf(dmgC, "%d", hand[i].dmg);
                sprintf(hpC, "%d", hand[i].hp);
                if (chosen == i) { //skip the chosen card, it will be drawn on the most top layer
                    continue;
                } else { //draw all the other cards that arent chosen
                    if (holding == 1) { //if the chosen card is held then draw the cards lower
                        extra = -(cardH / 2);
                    } else { //if the chosen card isnt held then draw all the cards normally
                        extra = -cardH - 40;
                    }
                    DrawRectangle(40 + ((winW - 80 - 2 * cardW) / handCount * i), winH + extra, cardW, cardH, hand[i].color);
                    DrawText(hand[i].name, 40 + ((winW - 80 - 2 * cardW) / handCount * i) + 4, winH + 4 + extra, 8, BLACK);
                    if (hand[i].type == monster) {
                        DrawText(dmgC, 40 + ((winW - 80 - 2 * cardW) / handCount * i) + 4, winH - 12 + extra + cardH, 8, BLACK);
                        DrawText(hpC, 40 + ((winW - 80 - 2 * cardW) / handCount * i) + cardW - 4 - MeasureText(hpC, 8), winH - 12 + extra + cardH, 8, BLACK);
                    }
                }
            }
            if (chosen != -1) { //if there is an actual card chosen then
                if (holding == 1) { //if you are holding the chosen card then draw it where the mouse is
                    cardX = mouseV.x - cardW / 2;
                    cardY = mouseV.y - cardH / 2;
                } else { //if youre not holding the chosen card then just draw the chosen card higher than the other cards in hand
                    cardX = 40 + ((winW - 80 - 2 * cardW) / ((handCount > 1) ? handCount : 1) * chosen);
                    cardY = winH - cardH - 40 - 40;
                }
                sprintf(dmgC, "%d", hand[chosen].dmg);
                sprintf(hpC, "%d", hand[chosen].hp);
                DrawRectangle(cardX, cardY, cardW, cardH, hand[chosen].color);
                DrawText(hand[chosen].name, cardX + 4, cardY + 4, 8, BLACK);
                if (hand[chosen].type == monster) { //if the card is a monster type card then also draw its damage and health ammount
                    DrawText(dmgC, cardX + 4, cardY + cardH - 12, 8, BLACK);
                    DrawText(hpC, cardX + cardW - 4 - MeasureText(hpC, 8), cardY + cardH - 12, 8, BLACK);
                }
            }

            //DRAWING OF DEBUG BUTTONS
            DrawRectangleRec(plusAllManaRec, RED);
            DrawRectangleRec(suffleRec, GREEN);
            DrawRectangleRec(startGame, BLUE);

            //DRAWING OF DEBUG INFO
             DrawText(manaCountStr, 1000, 40, 18, BLACK);
            for (int i = 0; i < deckCount; i++) {   
                DrawText(deck[i].name, 20, 40 + 1 + (i * 8), 8, BLACK);
            }
            for (int i = 0; i < playCount.sum; i++) {   
                DrawText(played[i].name, winW - 120, 40 + 1 + (i * 8), 8, BLACK);
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}