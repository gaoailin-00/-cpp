#pragma once
#include<iostream>
#include <windows.h>
using namespace std;
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


//npc选择
void chooseNPC(Player& player) {//参数形式
    int a;
    system("cls");
    cout << "选择对手" << endl;
    cout << "1、牧场主" << endl;
    cout << "2、老警长" << endl;
    cout << "3、华工老板" << endl;
    //cout<<"0、退出"<<endl;改进的时候加回来，包括cls也可以改进再加
    cin >> a;
    if (a == 1) {
        NPC npc("牧场主");
        fapai(player, npc);
        GameRound(player, npc);
    }
    else if (a == 2) {
        NPC npc("老警长");
        fapai(player, npc);
        GameRound(player, npc);
    }
    else if (a == 3) {
        NPC npc("华工老板");
        fapai(player, npc);
        GameRound(player, npc);
    }
    else if (a == 0) {
        cout << "犹豫的旅人回到大厅，决定再看看风景……" << endl;
        return;//break,return,goto用法和区别
    }
    else {
        cout << "输入错误，请重新输入" << endl;
        system("pause");
        chooseNPC(player);   // 重新调用自己 = 重新显示菜单
        return;
    }
}