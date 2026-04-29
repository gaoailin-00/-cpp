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
using namespace std; #pragma once

void Checkiou(Player& player);

void happyEnding(Player& player) {
    system("cls");
    cout << "——————————————————————————————————————————————————" << endl;
    cout << "  你终于集齐了3张欠条！" << endl;
    cout << "  所有债务全部追回，你完成了壮举，却默默离开了西部，这是一个无人知晓的传奇。" << endl;
    cout << "  牛仔的荣耀，由你守护！" << endl;
    cout << "——————————————————————————————————————————————————" << endl;
    system("pause");
    exit(0); // 直接结束游戏
}

void badEnding(Player& playeer) {

}

//需要在所有等待用户输入、或者有循环的地方，插入一行监听代码。
//这样玩家在任何时候按 ESC 都能立刻退出。
bool checkEscAndExit(Player& player) {
    if (_kbhit()) { // 检测是否有按键按下，是windows控制台特有的函数
        int key = _getch(); // 获取按下的键值
        if (key == 27) { // 27 是 ESC 键的ASCII码
            system("cls");
            cout << "——————————————————————————————————————————————————" << endl;
            if (player.getiou() < 3) {
                cout << "  你选择了放弃，未能集齐所有欠条。" << endl;
                cout << "  西部的风，也吹散了你的勇气……" << endl;
            }
            else {
                happyEnding(player);
            }
            cout << "——————————————————————————————————————————————————" << endl;
            system("pause");
            exit(0); // 直接结束程序
        }
    }
    return false;
}

void Checkiou(Player& player) {
    if (player.getiou() >= 3) {
        happyEnding(player);
        return;
    }
}
