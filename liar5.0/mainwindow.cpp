#include "mainwindow.h"
#include <algorithm>
#include <random>

MainWindow::MainWindow() {
    resize(1280, 760);
    setMinimumSize(1100, 680);
    setWindowTitle("骗子酒馆 liarliar");

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    player = PlayerData("旅人");
    playerWonLastGame = false;
    turnState = PlayerTurn;

    battleMessageTimer = new QTimer(this);
    battleMessageTimer->setSingleShot(true);

    createIntroScene();
    createStartScene();
    createHistoryScene();
    createRuleScene();
    createTavernScene();
    createBattleScene();
    createRoundResultScene();
    createEndScene();

    stack->setCurrentWidget(introScene);
}

void MainWindow::initCardPool() {
    cardPool.clear();
    QStringList suits = {"红桃", "黑桃", "方片", "梅花"};
    QList<int> nums = {11, 12, 13};

    for (int deck = 0; deck < 2; ++deck) {
        for (const QString& suit : suits) {
            for (int num : nums) {
                cardPool.push_back(Card(suit, num));
            }
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cardPool.begin(), cardPool.end(), g);
}

void MainWindow::draw5Cards(PlayerData& p) {
    p.clearCards();
    for (int i = 0; i < 5; ++i) {
        if (!cardPool.isEmpty()) {
            p.addCard(cardPool.takeLast());
        }
    }
}

void MainWindow::draw5Cards(NPCData& npc) {
    npc.clearCards();
    for (int i = 0; i < 5; ++i) {
        if (!cardPool.isEmpty()) {
            npc.addCard(cardPool.takeLast());
        }
    }
}

QString MainWindow::npcSpeak(const QString& name) {
    if (name == "牧场主") return "哼，想从我这里拿走欠条？先赢了我再说吧！";
    if (name == "老警长") return "年轻人，卡牌比枪更能解决问题，出手吧！";
    if (name == "华工老板") return "我们讲诚信，也讲义气。想拿欠条，就凭本事。";
    return "来吧，开始这场较量。";
}

QString MainWindow::getNpcImagePath(const QString& name) {
    QString basePath = QCoreApplication::applicationDirPath() + "/";
    if (name == "牧场主")   return basePath + "image-muchangzhu.png";
    if (name == "老警长")   return basePath + "image-laojingzhang.png";
    if (name == "华工老板") return basePath + "image-huagong.png";
    return "";
}

QString MainWindow::getNpcResultImagePath(const QString& name) {
    QString basePath = QCoreApplication::applicationDirPath() + "/";
    if (name == "牧场主")   return basePath + "image-mcz.png";
    if (name == "老警长")   return basePath + "image-ljz.png";
    if (name == "华工老板") return basePath + "image-hglb.png";
    return "";
}

QString MainWindow::getNpcWinDialogue(const QString& name) {
    if (name == "牧场主")
        return "算你狠！拿去，这是你的欠条，别让我在我的牛圈旁再看到你！";
    if (name == "老警长")
        return "（老警长沉默片刻，递给你一张皱巴巴的欠条）\n你的眼神比你的牌更稳，年轻人，拿走吧。";
    if (name == "华工老板")
        return "我们华工从不食言，年轻人，欠条给你！江湖路远，好自为之。";
    return "你赢了，欠条给你。";
}

QString MainWindow::getNpcLoseDialogue(const QString& name) {
    if (name == "牧场主")
        return "连牛群都骗不过的手法，也敢在我桌上耍花样？滚回老家去吧！";
    if (name == "老警长")
        return "小子，你的表情比牌更先露馅。在这片地界，犹豫的人总是先输。";
    if (name == "华工老板")
        return "做买卖讲眼力，打牌讲城府。今天这局，你火候还不够，回去练练吧。";
    return "看来你还得多练练。";
}