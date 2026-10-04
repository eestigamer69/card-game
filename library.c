cards deck[60] = {
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

//card
/*
char name[127];
Type type;
int cost;
int dmg;
int hp;
int abilities[];
*/

//costs
/*
1 = any
10 = wind
100 = fire
1000 = earth
10000 = water
*/