#pragma once
#include<iostream>
#include <windows.h>
#include<vector>
#include<string>
#include<cstdlib>
#include<ctime>
#include <algorithm>
#pragma comment(lib, "user32.lib") 
#include "card.h"
#include "player.h"
#include "npc.h"
#include "gameround.h"
#include "fapai.h"
#include "targetcard.h"
#include "choosenpc.h"
#include "ending.h"
using namespace std;
//随机发牌
Card createRandomCard() {
    string suits[] = { "红桃","黑桃","方片","梅花" };
    int suitindex = rand() % 4;//?这是什么
    int num = rand() % 13 + 1;
    return Card(suits[suitindex], num);
}
//发牌
void give5card(Player& p) {
    for (int i = 0; i < 5; i++) {
        p.addcard(createRandomCard());
    }
}
void give5Cards_NPC(NPC& npc) {
    for (int i = 0; i < 5; i++) {
        npc.addcard(createRandomCard());
    }
}
void fapai(Player& player, NPC& npc) {
    //发牌看牌
    system("cls");
    TargetCard(player, npc);//无&
    give5card(player);
    give5Cards_NPC(npc);
    system("pause");
    system("cls");//为什么不用getoneround.clear?
    cout << "你的手牌为" << endl;
    player.showhandcards();
    cout << "——————————————————————————————————————————————————" << endl;

}
