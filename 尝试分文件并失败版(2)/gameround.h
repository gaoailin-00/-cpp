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

void GameRound(Player& player, NPC& npc) {

    bool roundover = false;

    //出牌
    int choicenum;
    cout << "您想出几张牌（1～5)" << endl;
    cin >> choicenum;
    while (choicenum < 1 || choicenum>5 || choicenum > player.getcardcount()) {
        cout << "输入错误，请重新输入" << endl;
        cin >> choicenum;//可不可以用goto？
    }
    vector<int>choosecard;
    cout << "请输入您想要出的" << choicenum << "张牌的牌号（请用空格隔开）" << endl;
    for (int i = 0; i < choicenum; i++) {
        int n;
        cin >> n;
        choosecard.push_back(n - 1);//!!
    }
    vector<Card>outCard;
    for (int i = 0; i < choosecard.size(); i++) {
        outCard.push_back(player.getcard(choosecard[i]));
    }
    player.givecard(choosecard);

    cout << "你：我出的都是Q" << endl;
    system("pause");

    //质疑（狠狠地简化了）
    //后面这里可以改多样一点
    //先判断玩家是否已经出完5张

    bool playerFinishAll = (player.getcardcount() == 0);

    bool challenge = rand() % 2 == 0;
    if (challenge) {
        cout << "你被质疑了！" << endl;
        cout << "你的真实牌面是：" << endl;
        for (int i = 0; i < outCard.size(); i++) {
            outCard[i].cardshow();//??where is it?
        }
        bool allright = true;
        for (int i = 0; i < outCard.size(); i++) {
            if (!outCard[i].rightcard()) {
                allright = false;
                break;
            }
        }
        if (allright) {
            cout << "好吧……你赢了，欠条还给你……" << endl;
            player.addiou();
            Checkiou(player);
        }
        else {
            cout << "假牌！你果然撒谎了！" << endl;
        }
        roundover = true;
    }
    else {
        cout << npc.getname() << "选择不质疑。" << endl;

        if (playerFinishAll && !challenge) {
            cout << "你已经出完了手中的五张牌,赢得本剧！" << endl;
            roundover = true;
        }
        else {
            cout << "本轮继续！" << endl;
            roundover = false;
        }
    }
    if (roundover) {
        player.clear();
        npc.clear();
        cout << "本场对决结束" << endl;
        cout << "——————————————————————————————————————————————————" << endl;
        system("pause");
    }
    else if (!roundover) {
        GameRound(player, npc);
    }
}