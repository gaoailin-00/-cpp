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

void TargetCard(Player& player, NPC& npc) {
    const string target_card = "Q";
    srand((unsigned int)time(NULL));//

    cout << "西部的勇士，开始跟我对决吧！你要做骗子还是诚实人？" << endl;
    cout << "——————————————————————————————————————————————————" << endl;
    cout << "对局双方：你vs" << npc.getname() << endl;
    cout << "本局目标牌：【" << target_card << "】（始终不变）" << endl;
    cout << "对局规则  1. 每人初始五张牌，出牌后必须喊“我出的是Q”；" << endl;
    cout << "          2. 可选择撒谎，若对方或你选择质疑，翻牌验证真假；" << endl;
    cout << "          3. 验证为真则质疑者输；验证为假则出牌者输；" << endl;
    cout << "          4. 赢得对局即可获得一张欠条，集齐三张牛仔们便可留在这片西部土地上！" << endl;
    npc.speak(npc.getname());//不能调用private量，那就用类似getname这种函数
    cout << "按任意键开始发牌" << endl;
}