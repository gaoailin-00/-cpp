#include<iostream>
#include <windows.h>
#include<vector>
#include<string>
#include<cstdlib>
#include<ctime>
#include <algorithm>
#pragma comment(lib, "user32.lib") 
using namespace std;
//卡牌类
class Card {
public://因为想在后续访问，所以设成public
    string cardsuit;
    int cardnum;
public:
    Card() {}
    Card(string a, int b) : cardsuit(a), cardnum(b) {}
    string getcardsuit()const {
        return cardsuit;
    }                 //只读不改花色
    int getcardnum()const {
        return cardnum;
    }                 //只读不改牌面
    void cardshow()const {//注意在class中加const的位置
        cout << cardsuit << "花色的" << cardnum << endl;
    }
    bool rightcard()const {//检验本张牌是否为目标牌Q）
        return(cardnum == 12);//需要规定Q=12 这种return也很惊艳
    }
};

//玩家类
class Player {
private:
    string playername;
    vector<Card> handcards;
    int ioucount;
public:
    Player(string a = "") :playername(a) {}
    string getname()const {//get/set语法来自豆包
        return playername;
    }                 //只读不改人名

    void setname(string a) {
        playername = a;
    }                 //修改人名    ?留么

    Card getcard(int i)const {
        return handcards[i];
    }                 //只读不改卡牌

    void addcard(Card c) {
        handcards.push_back(c);
    }                 //添加卡牌

    int getcardcount()const {
        return handcards.size();
    }                 //查看手中卡牌数量

    void  showhandcards()const {
        cout << "玩家" << playername << " 的手牌如下" << endl;
        for (int i = 0; i < handcards.size(); i++) {
            handcards[i].cardshow();
            cout << " ";
        }
        cout << endl;
    }                 //展示手中的牌
    void givecard(vector<int>chupai) {//参数也要写vector
        sort(chupai.rbegin(), chupai.rend());//vector需要加一个人，
        //必须排序，然后从后往前删
        for (int i = chupai.size() - 1; i >= 0; i--) {
            handcards.erase(handcards.begin() + chupai[i]);
        }
    }//出牌
    void clear() {
        handcards.clear();
    }//清空玩家剩余卡牌，重开
    Player() {
        ioucount = 0;
    }
    int getiou() {
        return ioucount;
    }
    void addiou() {
        ioucount++;
    }
};
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
        else if (name =="接待者") {
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

#include<iostream>
#include<conio.h>//_getch()
#include<cstdlib>//随机数
#include<ctime>//时间——>随机数种子
using namespace std;

//初始化——随机数字
//声明游戏规则
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

void Checkiou(Player& player);

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

int main() {
    system("chcp 65001");
    srand((unsigned int)time(NULL));
    Player player("旅人");
    cout << "骗子酒馆" << endl;
    system("pause");
    chooseNPC(player);

    return 0;
}