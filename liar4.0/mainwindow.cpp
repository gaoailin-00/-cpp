#include "mainwindow.h"
#include "card.h"
#include <random>


MainWindow::MainWindow() {
    resize(1280, 760);
    setMinimumSize(1100, 680);
    setWindowTitle("骗子酒馆 liar4");

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    player = PlayerData("旅人");
    playerWonLastGame = false;
    turnState = PlayerTurn;

    battleMessageTimer = new QTimer(this);
    battleMessageTimer->setSingleShot(true);

    // 1. 创建所有场景
    createIntroScene();
    createStartScene();
    createHistoryScene();
    createRuleScene();
    createTavernScene();
    createBattleScene();
    createRoundResultScene();
    createEndScene();

    // 2. 【关键修复】把创建好的场景全部添加到 stack 容器中！
    stack->addWidget(introScene);
    stack->addWidget(startScene);
    stack->addWidget(historyScene);
    stack->addWidget(ruleScene);
    stack->addWidget(tavernScene);
    stack->addWidget(battleScene);
    stack->addWidget(roundResultScene);
    stack->addWidget(endScene);

    // 3. 设置默认显示第一个场景
    stack->setCurrentWidget(introScene);
}
// ==========================
// 牌库系统
// ==========================
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

// ==========================
// NPC 资源与台词
// ==========================
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

// ==========================
// 开始战斗
// ==========================
void MainWindow::startBattle(const QString& npcName) {
    currentNpc = NPCData(npcName);

    initCardPool();
    draw5Cards(player);
    draw5Cards(currentNpc);
    currentOutCards.clear();

    playerWonLastGame = false;
    turnState = PlayerTurn;

    updateBattleUI();
    npcSpeechLabel->setText(npcSpeak(currentNpc.name));
    roundInfoLabel->setText(QString("对局开始！\n目标牌：Q\n现在轮到你出牌。"));
    updateActionButtons();

    stack->setCurrentWidget(battleScene);
}
// ==========================
// 胜负处理（跳转全屏结算）
// ==========================
void MainWindow::handleRoundWin(const QString& message) {
    player.iouCount++;
    playerWonLastGame = true;
    turnState = BattleEnded;
    updateActionButtons();

    roundInfoLabel->setText(message);

    QTimer::singleShot(2000, this, [this]() {
        QString imgPath = getNpcResultImagePath(currentNpc.name);
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            roundResultImageLabel->setPixmap(pix);
            roundResultImageLabel->setText("");
        } else {
            roundResultImageLabel->setText("【图片缺失: " + imgPath + "】");
        }

        if (player.iouCount >= 3) {
            roundResultTextLabel->setText("恭喜你，幸运又狡猾的年轻人，你拿回了全部的欠条！");
            stack->setCurrentWidget(roundResultScene);
            battleMessageTimer->singleShot(3000, this, [this]() {
                goEndScene(true);
            });
        } else {
            roundResultTextLabel->setText(getNpcWinDialogue(currentNpc.name));
            stack->setCurrentWidget(roundResultScene);
            battleMessageTimer->singleShot(3000, this, [this]() {
                restartToTavern();
            });
        }
    });
}

void MainWindow::handleRoundLose(const QString& message) {
    playerWonLastGame = false;
    turnState = BattleEnded;
    updateActionButtons();

    roundInfoLabel->setText(message);

    QTimer::singleShot(2000, this, [this]() {
        QString imgPath = getNpcResultImagePath(currentNpc.name);
        QPixmap pix(imgPath);
        if (!pix.isNull()) {
            roundResultImageLabel->setPixmap(pix);
            roundResultImageLabel->setText("");
        } else {
            roundResultImageLabel->setText("【图片缺失: " + imgPath + "】");
        }

        roundResultTextLabel->setText(getNpcLoseDialogue(currentNpc.name));
        stack->setCurrentWidget(roundResultScene);

        battleMessageTimer->singleShot(3000, this, [this]() {
            restartToTavern();
        });
    });
}

// ==========================
// 玩家出牌
// ==========================
void MainWindow::doPlayerPlayCard() {
    if (turnState != PlayerTurn) return;

    QVector<int> selected;
    for (int i = 0; i < playerCardBtns.size() && i < player.handcards.size(); ++i) {
        if (playerCardBtns[i]->isChecked()) {
            selected.push_back(i);
        }
    }

    if (selected.isEmpty()) {
        QMessageBox::warning(this, "提示", "请先点击选择至少一张手牌。");
        return;
    }

    currentOutCards.clear();
    for (int idx : selected) {
        if (idx >= 0 && idx < player.handcards.size()) {
            currentOutCards.push_back(player.handcards[idx]);
        }
    }

    player.removeCardsByIndexes(selected);
    updateBattleUI();

    turnState = WaitingNpcChallenge;
    updateActionButtons();
    roundInfoLabel->setText(
        QString("你打出了 %1 张牌，并声称它们都是 Q。\n%2 正在观察你的表情……")
            .arg(selected.size()).arg(currentNpc.name)
        );

    bool npcChallenge = (QRandomGenerator::global()->bounded(100) < 35);

    if (npcChallenge) {
        battleMessageTimer->singleShot(1400, this, &MainWindow::resolveNpcChallenge);
    } else {
        battleMessageTimer->singleShot(1400, this, [this]() {
            if (player.cardCount() == 0) {
                handleRoundWin(
                    QString("%1没有质疑，而你已经出完全部手牌。\n你赢下了这局！")
                        .arg(currentNpc.name)
                    );
            } else {
                roundInfoLabel->setText(
                    QString("%1：我不质疑。\n现在轮到我出牌了。").arg(currentNpc.name)
                    );
                turnState = NpcTurn;
                updateActionButtons();
                battleMessageTimer->singleShot(1200, this, &MainWindow::doNpcPlayCard);
            }
        });
    }
}

void MainWindow::resolveNpcChallenge() {
    QString result = QString("%1：我质疑你！翻牌！\n真实牌面：\n").arg(currentNpc.name);
    bool allQ = true;

    for (const Card& c : currentOutCards) {
        result += QString("%1%2 ").arg(c.getSuitSymbol(), c.getNumText());
        if (!c.isTargetQ()) allQ = false;
    }

    if (allQ) {
        handleRoundWin(result + "\n\n【全部都是真Q！】你赢了。");
    } else {
        handleRoundLose(result + "\n\n【你出了假牌，被识破了！】");
    }
}

// ==========================
// NPC 出牌
// ==========================
void MainWindow::doNpcPlayCard() {
    if (turnState != NpcTurn) return;

    if (currentNpc.cardCount() <= 0) {
        handleRoundLose(
            QString("%1已经出完手牌。\n你错过了上一次质疑机会，这局判你失败。")
                .arg(currentNpc.name)
            );
        return;
    }

    int maxPlay = qMin(3, currentNpc.cardCount());
    int playCount = QRandomGenerator::global()->bounded(1, maxPlay + 1);

    QVector<int> selected;
    for (int i = 0; i < playCount; ++i) {
        selected.push_back(i);
    }

    currentOutCards.clear();
    for (int idx : selected) {
        currentOutCards.push_back(currentNpc.handcards[idx]);
    }

    currentNpc.removeCardsByIndexes(selected);
    updateBattleUI();

    roundInfoLabel->setText(
        QString("%1打出了 %2 张牌，并声称它们都是 Q。\n你要质疑吗？")
            .arg(currentNpc.name).arg(playCount)
        );

    turnState = WaitingPlayerDecision;
    updateActionButtons();
}

void MainWindow::resolvePlayerChallenge() {
    if (turnState != WaitingPlayerDecision) return;

    QString result = QString("你选择质疑 %1！\n真实牌面：\n").arg(currentNpc.name);
    bool allQ = true;

    for (const Card& c : currentOutCards) {
        result += QString("%1%2 ").arg(c.getSuitSymbol(), c.getNumText());
        if (!c.isTargetQ()) allQ = false;
    }

    if (allQ) {
        handleRoundLose(result + "\n\n【NPC出的都是真Q！】你质疑失败了。");
    } else {
        handleRoundWin(result + "\n\n【NPC撒谎，被你识破了！】你赢了。");
    }
}

void MainWindow::doPlayerPass() {
    if (turnState != WaitingPlayerDecision) return;

    if (currentNpc.cardCount() == 0) {
        handleRoundLose(
            QString("你没有质疑，而 %1 已经出完了全部手牌。\n这局你输了。")
                .arg(currentNpc.name)
            );
        return;
    }

    roundInfoLabel->setText(QString("你：我不质疑。\n轮到你继续出牌。"));
    turnState = PlayerTurn;
    updateActionButtons();
    updateBattleUI();
}

// ==========================
// 场景切换与 UI 更新
// ==========================
void MainWindow::goEndScene(bool win) {
    playerWonLastGame = win;
    updateEndScene();
    stack->setCurrentWidget(endScene);
}

void MainWindow::restartToTavern() {
    updateTavernInfo();
    stack->setCurrentWidget(tavernScene);
}

void MainWindow::updateTavernInfo() {
    if (!iouLabel) return;
    iouLabel->setText(QString("欠条：%1 / 3").arg(player.iouCount));
}

void MainWindow::updateActionButtons() {
    if (!playCardBtn || !challengeNpcBtn || !passNpcBtn || !backBtn) return;

    playCardBtn->hide();
    challengeNpcBtn->hide();
    passNpcBtn->hide();
    backBtn->hide();

    if (turnState == PlayerTurn) {
        playCardBtn->show();
        backBtn->show();
    } else if (turnState == WaitingPlayerDecision) {
        challengeNpcBtn->show();
        passNpcBtn->show();
    } else if (turnState == BattleEnded) {
        backBtn->show();
    }
}

void MainWindow::updateBattleUI() {
    // 更新NPC手牌显示 (背面)
    for (int i = 0; i < npcCardLabels.size(); ++i) {
        if (i < currentNpc.cardCount()) {
            npcCardLabels[i]->setText("🂠");
            npcCardLabels[i]->show();
        } else {
            npcCardLabels[i]->hide();
        }
    }

    // 更新玩家手牌显示
    for (int i = 0; i < playerCardBtns.size(); ++i) {
        playerCardBtns[i]->setChecked(false);
        if (i < player.cardCount()) {
            Card c = player.handcards[i];
            playerCardBtns[i]->setText(QString("%1\n%2").arg(c.getSuitSymbol(), c.getNumText()));
            if (c.isRed()) {
                playerCardBtns[i]->setStyleSheet("color: red; font-size: 24px; font-weight: bold; background-color: white; border: 1px solid gray; border-radius: 5px;");
            } else {
                playerCardBtns[i]->setStyleSheet("color: black; font-size: 24px; font-weight: bold; background-color: white; border: 1px solid gray; border-radius: 5px;");
            }
            playerCardBtns[i]->show();
        } else {
            playerCardBtns[i]->hide();
        }
    }

    // 更新桌面牌 (使用你头文件里的 battleTitleLabel)
    if (currentOutCards.isEmpty()) {
        battleTitleLabel->setText("[ 桌面上没有牌 ]");
    } else {
        battleTitleLabel->setText(QString("[ 桌面上盖着 %1 张牌 ]").arg(currentOutCards.size()));
    }
}

void MainWindow::updateEndScene() {
    // 使用你头文件里的 endImageLabel
    if (playerWonLastGame) {
        endImageLabel->setText("游戏通关！\n你集齐了所有的欠条！");
        endImageLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: green;");
    } else {
        endImageLabel->setText("游戏结束\n你输光了所有筹码...");
        endImageLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: red;");
    }
}
// ==========================
// UI 场景创建
// ==========================
void MainWindow::createIntroScene() {
    introScene = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(introScene);

    QLabel* title = new QLabel("骗子酒馆 Liar's Bar");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 48px; font-weight: bold; color: darkred;");

    QPushButton* enterBtn = new QPushButton("推门进入");
    enterBtn->setFixedSize(200, 60);
    connect(enterBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScene);
    });

    layout->addStretch();
    layout->addWidget(title);
    layout->addSpacing(50);
    layout->addWidget(enterBtn, 0, Qt::AlignCenter);
    layout->addStretch();
}

void MainWindow::createStartScene() {
    startScene = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(startScene);

    QLabel* title = new QLabel("主菜单");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 36px; font-weight: bold;");

    QPushButton* startBtn = new QPushButton("开始新游戏");
    QPushButton* ruleBtn = new QPushButton("规则说明");
    QPushButton* historyBtn = new QPushButton("历史记录");
    QPushButton* exitBtn = new QPushButton("退出酒馆");

    startBtn->setFixedSize(200, 50);
    ruleBtn->setFixedSize(200, 50);
    historyBtn->setFixedSize(200, 50);
    exitBtn->setFixedSize(200, 50);

    connect(startBtn, &QPushButton::clicked, this, [this]() {
        player = PlayerData("旅人"); // 重置玩家
        updateTavernInfo();
        stack->setCurrentWidget(tavernScene);
    });
    connect(ruleBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(ruleScene);
    });
    connect(historyBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(historyScene);
    });
    connect(exitBtn, &QPushButton::clicked, this, &QWidget::close);

    layout->addStretch();
    layout->addWidget(title);
    layout->addSpacing(30);
    layout->addWidget(startBtn, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(ruleBtn, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(historyBtn, 0, Qt::AlignCenter);
    layout->addSpacing(10);
    layout->addWidget(exitBtn, 0, Qt::AlignCenter);
    layout->addStretch();
}

void MainWindow::createHistoryScene() {
    historyScene = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(historyScene);

    QLabel* title = new QLabel("历史记录");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");

    QTextEdit* textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    textEdit->setText("暂无对局历史记录...");

    QPushButton* backBtnLocal = new QPushButton("返回");
    backBtnLocal->setFixedSize(150, 40);
    connect(backBtnLocal, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScene);
    });

    layout->addWidget(title);
    layout->addWidget(textEdit);
    layout->addWidget(backBtnLocal, 0, Qt::AlignCenter);
}

void MainWindow::createRuleScene() {
    ruleScene = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(ruleScene);

    QLabel* title = new QLabel("规则说明");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 24px; font-weight: bold;");

    QTextEdit* textEdit = new QTextEdit();
    textEdit->setReadOnly(true);
    textEdit->setText(
        "【游戏目标】\n"
        "击败酒馆里的3名NPC，赢回3张欠条。\n\n"
        "【对局规则】\n"
        "1. 每局目标牌固定为【Q】。\n"
        "2. 双方轮流出牌（1-3张），并声明自己出的是Q。\n"
        "3. 你可以出真Q，也可以用其他牌【撒谎】。\n"
        "4. 对方出牌后，你可以选择【质疑】或【不质疑】。\n"
        "   - 质疑成功（对方撒谎）：你赢下本局。\n"
        "   - 质疑失败（对方出真Q）：你输掉本局。\n"
        "5. 如果不质疑，则轮到你出牌。\n"
        "6. 谁先出完所有手牌，且未被质疑，即获得胜利。\n"
        );

    QPushButton* backBtnLocal = new QPushButton("返回");
    backBtnLocal->setFixedSize(150, 40);
    connect(backBtnLocal, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScene);
    });

    layout->addWidget(title);
    layout->addWidget(textEdit);
    layout->addWidget(backBtnLocal, 0, Qt::AlignCenter);
}

void MainWindow::createTavernScene() {
    tavernScene = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(tavernScene);

    QLabel* title = new QLabel("酒馆大厅");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 32px; font-weight: bold;");

    iouLabel = new QLabel("欠条：0 / 3");
    iouLabel->setAlignment(Qt::AlignCenter);
    iouLabel->setStyleSheet("font-size: 20px; color: blue;");

    QHBoxLayout* npcLayout = new QHBoxLayout();
    QStringList npcs = {"牧场主", "老警长", "华工老板"};
    for (const QString& name : npcs) {
        QVBoxLayout* singleNpcLayout = new QVBoxLayout();
        QLabel* imgLabel = new QLabel();
        imgLabel->setFixedSize(200, 300);
        imgLabel->setStyleSheet("border: 1px solid black; background-color: lightgray;");

        QPixmap pix(getNpcImagePath(name));
        if (!pix.isNull()) {
            imgLabel->setPixmap(pix.scaled(200, 300, Qt::KeepAspectRatio));
        } else {
            imgLabel->setText(name + "\n(图片缺失)");
            imgLabel->setAlignment(Qt::AlignCenter);
        }

        QPushButton* btn = new QPushButton("挑战 " + name);
        btn->setFixedSize(200, 40);
        connect(btn, &QPushButton::clicked, this, [this, name]() {
            startBattle(name);
        });

        singleNpcLayout->addWidget(imgLabel, 0, Qt::AlignCenter);
        singleNpcLayout->addWidget(btn, 0, Qt::AlignCenter);
        npcLayout->addLayout(singleNpcLayout);
    }

    QPushButton* backBtnLocal = new QPushButton("返回主菜单");
    backBtnLocal->setFixedSize(200, 50);
    connect(backBtnLocal, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScene);
    });

    layout->addStretch();
    layout->addWidget(title);
    layout->addSpacing(20);
    layout->addWidget(iouLabel);
    layout->addSpacing(40);
    layout->addLayout(npcLayout);
    layout->addSpacing(50);
    layout->addWidget(backBtnLocal, 0, Qt::AlignCenter);
    layout->addStretch();
}

void MainWindow::createBattleScene() {
    battleScene = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(battleScene);

    // NPC 区域
    QHBoxLayout* npcLayout = new QHBoxLayout();
    npcAvatarLabel = new QLabel("NPC头像"); // 实例化头文件中的 npcAvatarLabel
    npcAvatarLabel->setFixedSize(80, 80);
    npcAvatarLabel->setStyleSheet("background-color: lightgray; border: 1px solid black;");

    npcSpeechLabel = new QLabel("NPC台词");
    npcSpeechLabel->setAlignment(Qt::AlignCenter);
    npcSpeechLabel->setStyleSheet("font-size: 18px; color: darkblue; background-color: lightyellow; border: 1px solid gray; padding: 5px;");

    npcLayout->addWidget(npcAvatarLabel);
    npcLayout->addWidget(npcSpeechLabel);

    QHBoxLayout* npcCardsLayout = new QHBoxLayout();
    npcCardLabels.clear();
    for (int i = 0; i < 5; ++i) {
        QLabel* cardLabel = new QLabel("🂠");
        cardLabel->setAlignment(Qt::AlignCenter);
        cardLabel->setFixedSize(60, 90);
        cardLabel->setStyleSheet("font-size: 40px; color: blue; background-color: white; border: 1px solid gray; border-radius: 5px;");
        npcCardsLayout->addWidget(cardLabel);
        npcCardLabels.push_back(cardLabel);
    }
    npcLayout->addLayout(npcCardsLayout);

    // 桌面区域
    battleTitleLabel = new QLabel("[ 桌面上没有牌 ]"); // 实例化头文件中的 battleTitleLabel
    battleTitleLabel->setAlignment(Qt::AlignCenter);
    battleTitleLabel->setStyleSheet("font-size: 20px; font-weight: bold; padding: 20px; background-color: #2e8b57; color: white; border-radius: 10px;");

    // 提示信息区域
    roundInfoLabel = new QLabel("准备开始...");
    roundInfoLabel->setAlignment(Qt::AlignCenter);
    roundInfoLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #333; padding: 10px;");

    // 玩家手牌区域
    QHBoxLayout* playerCardsLayout = new QHBoxLayout();
    playerAvatarLabel = new QLabel("玩家"); // 实例化头文件中的 playerAvatarLabel
    playerAvatarLabel->setFixedSize(80, 80);
    playerAvatarLabel->setStyleSheet("background-color: lightblue; border: 1px solid black;");
    playerCardsLayout->addWidget(playerAvatarLabel);

    playerCardBtns.clear();
    for (int i = 0; i < 5; ++i) {
        QPushButton* btn = new QPushButton("");
        btn->setFixedSize(80, 120);
        btn->setCheckable(true);
        playerCardsLayout->addWidget(btn);
        playerCardBtns.push_back(btn);
    }

    // 操作按钮区域
    QHBoxLayout* actionLayout = new QHBoxLayout();
    playCardBtn = new QPushButton("出牌 (声称是Q)");
    challengeNpcBtn = new QPushButton("质疑NPC");
    passNpcBtn = new QPushButton("不质疑");
    backBtn = new QPushButton("逃跑 (返回酒馆)");

    playCardBtn->setFixedSize(150, 50);
    challengeNpcBtn->setFixedSize(150, 50);
    passNpcBtn->setFixedSize(150, 50);
    backBtn->setFixedSize(150, 50);

    connect(playCardBtn, &QPushButton::clicked, this, &MainWindow::doPlayerPlayCard);
    connect(challengeNpcBtn, &QPushButton::clicked, this, &MainWindow::resolvePlayerChallenge);
    connect(passNpcBtn, &QPushButton::clicked, this, &MainWindow::doPlayerPass);
    connect(backBtn, &QPushButton::clicked, this, [this]() {
        restartToTavern();
    });

    actionLayout->addWidget(playCardBtn);
    actionLayout->addWidget(challengeNpcBtn);
    actionLayout->addWidget(passNpcBtn);
    actionLayout->addStretch();
    actionLayout->addWidget(backBtn);

    layout->addLayout(npcLayout);
    layout->addSpacing(20);
    layout->addWidget(battleTitleLabel);
    layout->addSpacing(20);
    layout->addWidget(roundInfoLabel);
    layout->addSpacing(20);
    layout->addLayout(playerCardsLayout);
    layout->addSpacing(20);
    layout->addLayout(actionLayout);
}

void MainWindow::createRoundResultScene() {
    roundResultScene = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(roundResultScene);

    roundResultTextLabel = new QLabel("");
    roundResultTextLabel->setAlignment(Qt::AlignCenter);
    roundResultTextLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    roundResultImageLabel = new QLabel("");
    roundResultImageLabel->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addWidget(roundResultImageLabel);
    layout->addSpacing(20);
    layout->addWidget(roundResultTextLabel);
    layout->addStretch();
}

void MainWindow::createEndScene() {
    endScene = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(endScene);

    endImageLabel = new QLabel(""); // 实例化头文件中的 endImageLabel
    endImageLabel->setAlignment(Qt::AlignCenter);
    endImageLabel->setStyleSheet("font-size: 36px; font-weight: bold;");

    QPushButton* backToMenuBtn = new QPushButton("返回主菜单");
    backToMenuBtn->setFixedSize(200, 50);
    connect(backToMenuBtn, &QPushButton::clicked, this, [this]() {
        stack->setCurrentWidget(startScene);
    });

    layout->addStretch();
    layout->addWidget(endImageLabel);
    layout->addSpacing(50);
    layout->addWidget(backToMenuBtn, 0, Qt::AlignCenter);
    layout->addStretch();
}