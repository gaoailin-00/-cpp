#pragma once//防止重复包含
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
class NPC {
private:
    string npcname;
    vector<Card> handcards;
public:
    NPC(string a = "") :npcname(a) {}
    string getname()const {
        return npcname;
    }//只读不改NPC名字
    void setname(string a) {
        npcname = a;
    }//修改NPC名字
    Card getcard(int i)const {
        return handcards[i];
    }//只读不改卡牌
    void addcard(Card c) {
        handcards.push_back(c);
    }//添加卡牌
    int getcardcount()const {
        return handcards.size();
    }//只读NPC手中卡牌数量 
    void speak(string name) {
        if (name == "牧场主") {
            cout << "【" << name << "】：哼，想从我这里拿走欠条？先赢了我再说吧！" << endl;
        }
        else if (name == "老警长") {
            cout << "【" << name << "】：年轻人，卡牌比枪更能解决问题，出手吧！" << endl;
        }
        else if (name == "华工老板") {
            cout << "【" << name << "】：年轻人，我们华工讲诚信也讲义气，赢了我，欠条少不了你！" << endl;
        }
        else if (name == "接待者") {
            cout << "欢迎来到骗子酒馆，出牌小心点，这里都是骗子！" << endl;
        }
    }
    void givecard(int i) {
        handcards.erase(handcards.begin() + i);
    }//出牌
    void clear() {
        handcards.clear();
    }//一轮结束，清空
    Card getonecard(int i)const {
        return handcards[i];
    }//玩家技能：获取某张牌信息
};
