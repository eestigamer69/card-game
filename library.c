cards deck[60] = {
    {"Goblin", monster, (fireC), 2, 2, none},
    {"Ent", monster, (3 * earthC + anyC), 3, 4, reach},
    {"Eagle", monster, (2 * windC), 1, 1, flight},
    {"Island", mana, waterM, 0, 0, mana},
    {"Island", mana, waterM, 0, 0, mana},
    {"Island", mana, waterM, 0, 0, mana},
    {"Island", mana, waterM, 0, 0, mana},
    {"Island", mana, waterM, 0, 0, mana},
    {"Forest", mana, earthM, 0, 0, mana},
    {"Forest", mana, earthM, 0, 0, mana},
    {"Forest", mana, earthM, 0, 0, mana},
    {"Forest", mana, earthM, 0, 0, mana},
    {"Forest", mana, earthM, 0, 0, mana},
    {"Desert", mana, fireM, 0, 0, mana},
    {"Desert", mana, fireM, 0, 0, mana},
    {"Desert", mana, fireM, 0, 0, mana},
    {"Desert", mana, fireM, 0, 0, mana},
    {"Desert", mana, fireM, 0, 0, mana},
    {"Mountain", mana, windM, 0, 0, mana},
    {"Mountain", mana, windM, 0, 0, mana},
    {"Mountain", mana, windM, 0, 0, mana},
    {"Mountain", mana, windM, 0, 0, mana},
    {"Mountain", mana, windM, 0, 0, mana},
    {"Wasteland", mana, anyM, 0, 0, mana},
    {"I win", spell, (5 * (anyC + windC + fireC + earthC + waterC))}
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