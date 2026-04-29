#include <windows.h>
#include<vector>
#include<string>
#include <algorithm>
#pragma comment(lib, "user32.lib") 
#include<iostream>
#include<conio.h>//_getch()
#include<cstdlib>//随机数
#include<ctime>//时间——>随机数种子

#include "card.h"
#include "player.h"
#include "npc.h"
#include "gameround.h"
#include "fapai.h"
#include "targetcard.h"
#include "choosenpc.h"
#include "ending.h"

using namespace std;

//初始化——随机数字
//声明游戏规则

int main() {
    system("chcp 65001");
    srand((unsigned int)time(NULL));
    Player player("旅人");
    cout << "骗子酒馆" << endl;
    system("pause");
    chooseNPC(player);

    return 0;
}