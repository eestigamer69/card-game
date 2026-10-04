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

    int mm = 2;
    int manaCount = 0;
    int handCount = 0;
    int deckCount = 25;
    struct {
        int mana;
        int creature;
        int sum;
    } playCount = {0, 0, 0};
    int cardW = 63 * mm, cardH = 88 * mm;
    char manaCountStr[10];

    time_t randomiser = time(NULL);

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
    Cards buffer[60] = {};
    Cards played[60] = {};

    Cards hand[60] = {};
    
    Vector2 mouseV = GetMousePosition();
    Rectangle plusAllManaRec = {40, 40, 40, 40};
    Rectangle deckRec = {winW - cardW - 40, winH - cardH - 40, cardW, cardH};
    Rectangle discardRec = {winW - cardW - 40, winH - 30, cardW, 20};
    Rectangle suffleRec = {winW - 80, 40, 40, 40};
    Rectangle startGame = {winW - 140, 40, 40, 40};
    Rectangle chosenRec = {0, 0, 0, 0};
    Rectangle bufferRec = {0, 0, cardW, cardH};
    Rectangle handRec = {40, winH - cardH - 40, 924, cardH};

    Rectangle playRec = {40, winH / 4 - 40, winW - 120 - cardW, 2 * cardH + 40};
    Rectangle manaRec = {40, winH / 4 + cardH, winW - 120 - cardW, cardH};
    Rectangle creatureRec = {40, winH / 4 - 40, winW - 120 - cardW, cardH};


    int chosen = -1;
    int manaChosen = -1;

    int discarding = 0;
    int holding = 0;
    int place = -1;
    char hpC[4];
    char dmgC[4];

    int extra = 0;

    while (!WindowShouldClose()) {
        mouseV = GetMousePosition();

        if (chosen != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {                                                             //card holding
            holding = 1;
        } else {
            holding = 0;
        }

        if (holding == 1 && hand[chosen].type == mana && CheckCollisionPointRec(mouseV, playRec)) {                             //card playing
            place = chosen;
        } else if (holding == 1) {
            place = -1;
        }

        if (CheckCollisionPointRec(mouseV, manaRec) && holding != 1) {                                                          //played card choose
            for (int i = playCount.mana - 1; i > -1; i--) {
                bufferRec.x = 40 + ((winW - 120 - cardW) / playCount.mana * i);
                bufferRec.y = winH / 4 + cardH;
                bufferRec.width = cardW;
                bufferRec.height = cardH;
                if (CheckCollisionPointRec(mouseV, bufferRec)) {
                    manaChosen = i;
                    break;
                }
            }
        } else if (holding != 1) {                                                                                              //played card unchoose
            manaChosen = -1;
        }
       
        if (CheckCollisionPointRec(mouseV, handRec) && holding != 1) {                                                          //card choice
                    if (handCount == 1) {
                        for (int i = 0; i < handCount; i++) {
                            bufferRec.x = 40 + (964 / 2);
                            bufferRec.y = winH - cardH - 40;
                            bufferRec.width = cardW;
                            bufferRec.height = cardH;
                            if (CheckCollisionPointRec(mouseV, bufferRec)) {
                                chosen = i;
                            }
                        }
                    } else {
                        for (int i = handCount - 1; i > -1; i--) {
                            bufferRec.x = 40 + (964 / handCount * i);
                            bufferRec.y = winH - cardH - 40;
                            bufferRec.width = cardW;
                            bufferRec.height = cardH;
                            if (CheckCollisionPointRec(mouseV, bufferRec)) {
                                chosen = i;
                                break;
                            }
                        }
                    }
        } else if (holding != 1) {                                                                                              //card unchoose
            chosen = -1;
        }  
        
        if(CheckCollisionPointRec(mouseV, bufferRec) && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {                                                           //place card
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

        if (CheckCollisionPointRec(mouseV, plusAllManaRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {                        //plus mana
            manaCount += 11111;
        }
        if (CheckCollisionPointRec(mouseV, deckRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {                               //draw card
            hand[handCount] = deck[0];
            handCount++;
            deckCount--;
            for (int i = 0; i < deckCount; i++) {
                deck[i] = deck[i + 1];
            }
        }
        if (CheckCollisionPointRec(mouseV, discardRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {                            //activate discard
            discarding = (discarding == 0) ? 1 : 0;
        }
        if (CheckCollisionPointRec(mouseV, bufferRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {                             //hand discard
            if (discarding == 1) {
                for (int i = chosen; i < deckCount; i++) {
                    hand[i] = hand[i + 1];
                }
                chosen = -1;
                handCount--;
                discarding = 0;
            } 
        }  
        if (CheckCollisionPointRec(mouseV, suffleRec) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {                             //shuffle deck
            for (int i = deckCount - 1; i > 0; i--) {
                int j = rand() * time(NULL) % (i + 1);
                Cards temp = deck[j];
                deck[j] = deck[i];
                deck[i] = temp;
            }
        }   //Fisher–Yates shuffle
        if (CheckCollisionPointRec(mouseV, startGame) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {                             //start game
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

        sprintf(manaCountStr, "%d", manaCount);

        BeginDrawing();
            ClearBackground(LIGHTGRAY);
            if (holding == 1) {
                DrawRectangleLinesEx(playRec, 4, BLACK);
            }
            DrawRectangleRec(plusAllManaRec, RED);
            DrawRectangleRec(deckRec, BLACK);
            DrawRectangleRec(discardRec, RED);
            if (discarding == 1) {
                DrawText("Cancel", discardRec.x + (discardRec.width / 2) - MeasureText("Cancel", 12) / 2, discardRec.y + (discardRec.height / 2 - 6), 12, BLACK);
            }
            DrawRectangleRec(suffleRec, GREEN);
            DrawRectangleRec(startGame, BLUE);
            
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

            for (int i = 0; i < handCount; i++) {                                                                               //draw hand
                sprintf(dmgC, "%d", hand[i].dmg);
                sprintf(hpC, "%d", hand[i].hp);
                if (handCount == 1) {
                    if (chosen == i && holding != 1) {
                        DrawRectangle(40 + (964 / 2), winH - cardH - 40 - 40, cardW, cardH, hand[i].color);
                        DrawText(hand[i].name, 40 + (964 / 2) + 4, winH - cardH - 40 + 4 - 40, 8, BLACK);
                        if (hand[i].type == monster) {
                            DrawText(dmgC, 40 + (964 / 2) + 4, winH - 40 - 12 - 40, 8, BLACK);
                            DrawText(hpC, 40 + (964 / 2) + cardW - 4 - MeasureText(hpC, 8), winH - 40 - 40 - 12, 8, BLACK);
                        }
                    } else if (holding != 1) {
                        DrawRectangle(40 + (964 / 2), winH - cardH - 40, cardW, cardH, hand[i].color);
                        DrawText(hand[i].name, 40 + (964 / 2) + 4, winH - cardH - 40 + 4, 8, BLACK);
                        if (hand[i].type == monster) {
                            DrawText(dmgC, 40 + (964 / 2) + 4, winH - 40 - 12, 8, BLACK);
                            DrawText(hpC, 40 + (964 / 2) + cardW - 4 - MeasureText(hpC, 8), winH - 40 - 12, 8, BLACK);
                        }
                    }
                    if (holding == 1) {
                        sprintf(dmgC, "%d", hand[chosen].dmg);
                        sprintf(hpC, "%d", hand[chosen].hp);
                        DrawRectangle(mouseV.x - cardW / 2, mouseV.y - cardH / 2, cardW, cardH, hand[chosen].color);
                        DrawText(hand[chosen].name, mouseV.x - cardW / 2 + 4, mouseV.y - cardH / 2 + 4, 8, BLACK);
                        if (hand[chosen].type == monster) {
                            DrawText(dmgC, mouseV.x - cardW / 2 + 4, mouseV.y + cardH / 2 - 12, 8, BLACK);
                            DrawText(hpC, mouseV.x + cardW / 2 - 4 - MeasureText(hpC, 8), mouseV.y + cardH / 2 - 12, 8, BLACK);
                        }
                    }
                } else {
                    if (chosen == i) {
                        continue;
                    } else {
                        if (holding == 1) {
                            DrawRectangle(40 + (964 / handCount * i), winH - cardH / 2, cardW, cardH, hand[i].color); //from 40 until deckRec - 40
                            DrawText(hand[i].name, 40 + (964 / handCount * i) + 4, winH - cardH / 2 + 4, 8, BLACK);
                            if (hand[i].type == monster) {
                                DrawText(dmgC, 40 + (964 / handCount * i) + 4, winH - 12, 8, BLACK);
                                DrawText(hpC, 40 + (964 / handCount * i) + cardW - 4 - MeasureText(hpC, 8), winH - 12, 8, BLACK);
                            }
                        } else {
                            DrawRectangle(40 + (964 / handCount * i), winH - cardH - 40, cardW, cardH, hand[i].color); //from 40 until deckRec - 40
                            DrawText(hand[i].name, 40 + (964 / handCount * i) + 4, winH - cardH - 40 + 4, 8, BLACK);
                            if (hand[i].type == monster) {
                                DrawText(dmgC, 40 + (964 / handCount * i) + 4, winH - 40 - 12, 8, BLACK);
                                DrawText(hpC, 40 + (964 / handCount * i) + cardW - 4 - MeasureText(hpC, 8), winH - 40 - 12, 8, BLACK);
                            }
                        }
                    }
                    if (chosen != -1) {
                        if (holding == 1) {
                            sprintf(dmgC, "%d", hand[chosen].dmg);
                            sprintf(hpC, "%d", hand[chosen].hp);
                            DrawRectangle(mouseV.x - cardW / 2, mouseV.y - cardH / 2, cardW, cardH, hand[chosen].color);
                            DrawText(hand[chosen].name, mouseV.x - cardW / 2 + 4, mouseV.y - cardH / 2 + 4, 8, BLACK);
                            if (hand[chosen].type == monster) {
                                DrawText(dmgC, mouseV.x - cardW / 2 + 4, mouseV.y + cardH / 2 - 12, 8, BLACK);
                                DrawText(hpC, mouseV.x + cardW / 2 - 4 - MeasureText(hpC, 8), mouseV.y + cardH / 2 - 12, 8, BLACK);
                            }
                        } else {
                            sprintf(dmgC, "%d", hand[chosen].dmg);
                            sprintf(hpC, "%d", hand[chosen].hp);
                            DrawRectangle(40 + (964 / handCount * chosen), winH - cardH - 40 - 40, cardW, cardH, hand[chosen].color);
                            DrawText(hand[chosen].name, 40 + (964 / handCount * chosen) + 4, winH - cardH - 40 + 4 - 40, 8, BLACK);
                            if (hand[chosen].type == monster) {
                                DrawText(dmgC, 40 + (964 / handCount * chosen) + 4, winH - 40 - 12 - 40, 8, BLACK);
                                DrawText(hpC, 40 + (964 / handCount * chosen) + cardW - 4 - MeasureText(hpC, 8), winH - 40 - 40 - 12, 8, BLACK);
                            }
                        }
                    }
                }
                //winH - cardH - 40
            }
            
            
            DrawText(manaCountStr, 1000, 40, 18, BLACK);
            for (int i = 0; i < deckCount; i++) {   
                DrawText(deck[i].name, 20, 40 + 1 + (i * 8), 8, BLACK);
            }
            DrawText("test", winW - 120, 40 + 1, 8, BLACK);
            for (int i = 0; i < playCount.sum; i++) {   
                DrawText(played[i].name, winW - 120, 40 + 1 + (i * 8), 8, BLACK);
            }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}