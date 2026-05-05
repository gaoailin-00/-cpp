#include "mainwindow.h"

//==========================
// 选牌对话框实现
//==========================
CardSelectDialog::CardSelectDialog(const QVector<Card>& cards, QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("选择要出的牌");
    resize(350, 300);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QLabel* tip = new QLabel("请选择要出的牌（至少1张）", this);
    tip->setStyleSheet("font-size:16px; font-weight:bold;");
    layout->addWidget(tip);

    for (int i = 0; i < cards.size(); ++i) {
        QString text = QString("第%1张：%2").arg(i + 1).arg(cards[i].toString());
        QCheckBox* cb = new QCheckBox(text, this);
        cb->setStyleSheet("font-size:14px; padding:5px;");
        checkBoxes.append(cb);
        layout->addWidget(cb);
    }

    QDialogButtonBox* btnBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    btnBox->setStyleSheet("font-size:14px;");
    connect(btnBox, &QDialogButtonBox::accepted, this, &CardSelectDialog::onAccept);
    connect(btnBox, &QDialogButtonBox::rejected, this, &CardSelectDialog::reject);
    layout->addWidget(btnBox);
}

QVector<int> CardSelectDialog::selectedIndexes() const {
    return m_selectedIndexes;
}

void CardSelectDialog::onAccept() {
    m_selectedIndexes.clear();
    for (int i = 0; i < checkBoxes.size(); ++i) {
        if (checkBoxes[i]->isChecked()) {
            m_selectedIndexes.push_back(i);
        }
    }

    if (m_selectedIndexes.isEmpty()) {
        QMessageBox::warning(this, "提示", "至少选择1张牌。");
        return;
    }

    accept();
}

//==========================
// 主窗口实现
//==========================
MainWindow::MainWindow() {
    resize(1280, 720);
    setWindowTitle("骗子酒馆");

    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    player = PlayerData("旅人");
    playerWonLastGame = false;

    createStartScene();
    createTavernScene();
    createBattleScene();
    createEndScene();

    stack->setCurrentWidget(startScene);
}

Card MainWindow::createRandomCard() {
    QStringList suits = {"红桃", "黑桃", "方片", "梅花"};
    int suitIndex = QRandomGenerator::global()->bounded(4);
    int num = QRandomGenerator::global()->bounded(1, 14);
    return Card(suits[suitIndex], num);
}

void MainWindow::give5Cards(PlayerData& p) {
    p.clearCards();
    for (int i = 0; i < 5; ++i) {
        p.addCard(createRandomCard());
    }
}

void MainWindow::give5Cards(NPCData& npc) {
    npc.clearCards();
    for (int i = 0; i < 5; ++i) {
        npc.addCard(createRandomCard());
    }
}

QString MainWindow::npcSpeak(const QString& name) {
    if (name == "牧场主") {
        return "哼，想从我这里拿走欠条？先赢了我再说吧！";
    } else if (name == "老警长") {
        return "年轻人，卡牌比枪更能解决问题，出手吧！";
    } else if (name == "华工老板") {
        return "年轻人，我们华工讲诚信也讲义气，赢了我，欠条少不了你！";
    }
    return "来吧，开始这场较量。";
}

void MainWindow::startBattle(const QString& npcName) {
    currentNpc = NPCData(npcName);
    give5Cards(player);
    give5Cards(currentNpc);
    currentOutCards.clear();

    updateBattleUI();

    QMessageBox::information(this, "对局开始",
                             QString("对局双方：你 VS %1\n目标牌：【Q】\n\n规则：\n"
                                     "1. 双方初始五张牌。\n"
                                     "2. 你出牌后宣称“我出的是Q”。\n"
                                     "3. 对方可能质疑，翻牌验证。\n"
                                     "4. 若都是真Q，则质疑者输；若有假牌，则出牌者输。\n\n"
                                     "%2：%3")
                                 .arg(currentNpc.name,
                                      currentNpc.name,
                                      npcSpeak(currentNpc.name)));

    stack->setCurrentWidget(battleScene);
}

void MainWindow::doPlayerPlayCard() {
    if (player.cardCount() <= 0) {
        QMessageBox::information(this, "提示", "你已经没有手牌了。");
        return;
    }

    CardSelectDialog dlg(player.handcards, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }

    QVector<int> selected = dlg.selectedIndexes();
    currentOutCards.clear();

    for (int idx : selected) {
        if (idx >= 0 && idx < player.handcards.size()) {
            currentOutCards.push_back(player.handcards[idx]);
        }
    }

    player.removeCardsByIndexes(selected);
    updateBattleUI();

    QMessageBox::information(this, "出牌", "你：我出的都是Q");

    bool challenge = (QRandomGenerator::global()->bounded(2) == 0);

    if (challenge) {
        resolveNpcChallenge();
    } else {
        QMessageBox::information(this, "结果",
                                 QString("%1选择不质疑。").arg(currentNpc.name));

        if (player.cardCount() == 0) {
            player.iouCount++;
            playerWonLastGame = true;
            QMessageBox::information(this, "胜利",
                                     "你已经出完了所有手牌，赢得本局！\n获得1张欠条！");
            goEndScene(true);
        } else {
            roundInfoLabel->setText("本轮继续：NPC未质疑。你可以继续出牌。");
        }
    }
}

void MainWindow::resolveNpcChallenge() {
    QString result = "你被质疑了！\n你的真实牌面是：\n";
    bool allQ = true;

    for (const Card& c : currentOutCards) {
        result += c.toString() + "\n";
        if (!c.isTargetQ()) {
            allQ = false;
        }
    }

    if (allQ) {
        result += "\n全部都是真Q！你赢了！\n获得1张欠条。";
        player.iouCount++;
        playerWonLastGame = true;
        QMessageBox::information(this, "质疑结果", result);
        goEndScene(true);
    } else {
        result += "\n你出了假牌，被识破了！";
        playerWonLastGame = false;
        QMessageBox::warning(this, "质疑结果", result);
        goEndScene(false);
    }
}

void MainWindow::playerChallengeNpc() {
    QMessageBox::information(this, "质疑",
                             "当前版本先保留“玩家质疑NPC”按钮。\n"
                             "后续建议加入：\n"
                             "1. NPC随机出1~2张牌\n"
                             "2. 桌面中央显示‘NPC已出牌（背面）’\n"
                             "3. 玩家选择是否质疑");
}
void MainWindow::showPlayerHand() {
    QString text = "你的当前手牌：\n\n";
    for (int i = 0; i < player.handcards.size(); ++i) {
        text += QString("第%1张：%2\n").arg(i + 1).arg(player.handcards[i].toString());
    }

    if (player.handcards.isEmpty()) {
        text += "（没有手牌）";
    }

    QMessageBox::information(this, "我的手牌", text);
}

void MainWindow::goEndScene(bool win) {
    playerWonLastGame = win;
    updateTavernInfo();
    updateEndScene();
    stack->setCurrentWidget(endScene);
}

void MainWindow::restartToTavern() {
    updateTavernInfo();
    stack->setCurrentWidget(tavernScene);
}

void MainWindow::createStartScene() {
    startScene = new QWidget();
    startScene->setStyleSheet("background-color: #2c1810;");

    QVBoxLayout* layout = new QVBoxLayout(startScene);
    layout->setContentsMargins(60, 60, 60, 60);

    layout->addStretch();

    QLabel* title = new QLabel("骗子酒馆", startScene);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet(
        "QLabel{"
        "color:#ffcc66;"
        "font-size:48px;"
        "font-weight:bold;"
        "background:rgba(0,0,0,120);"
        "border:2px solid #ff9900;"
        "border-radius:15px;"
        "padding:20px;"
        "}"
        );
    layout->addWidget(title, 0, Qt::AlignHCenter);

    QLabel* sub = new QLabel("欢迎来到骗子酒馆，出牌小心点，这里都是骗子！", startScene);
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet(
        "QLabel{color:white;font-size:20px;background:rgba(0,0,0,100);padding:10px;border-radius:10px;}"
        );
    layout->addWidget(sub, 0, Qt::AlignHCenter);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* startBtn = new QPushButton("开始游戏", startScene);
    QPushButton* quitBtn = new QPushButton("退出", startScene);

    startBtn->setFixedSize(180, 60);
    quitBtn->setFixedSize(180, 60);

    startBtn->setStyleSheet("QPushButton{font-size:22px;background:#d88c3a;color:white;border-radius:10px;}"
                            "QPushButton:hover{background:#e39c4f;}");
    quitBtn->setStyleSheet("QPushButton{font-size:22px;background:#7a3d2b;color:white;border-radius:10px;}"
                           "QPushButton:hover{background:#91503d;}");

    btnLayout->addStretch();
    btnLayout->addWidget(startBtn);
    btnLayout->addSpacing(30);
    btnLayout->addWidget(quitBtn);
    btnLayout->addStretch();

    layout->addSpacing(30);
    layout->addLayout(btnLayout);
    layout->addStretch();

    connect(startBtn, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(tavernScene);
    });
    connect(quitBtn, &QPushButton::clicked, this, [=]() {
        close();
    });

    stack->addWidget(startScene);
}

void MainWindow::createTavernScene() {
    tavernScene = new QWidget();
    tavernScene->setStyleSheet("background-color: #3a2010;");

    QVBoxLayout* mainLayout = new QVBoxLayout(tavernScene);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QLabel* title = new QLabel("酒馆大厅 - 选择你的对手", tavernScene);
    title->setStyleSheet("QLabel{color:white;font-size:28px;font-weight:bold;background:rgba(0,0,0,120);padding:10px;border-radius:10px;}");

    iouLabel = new QLabel("欠条：0 / 3", tavernScene);
    iouLabel->setStyleSheet("QLabel{color:#ffd36b;font-size:22px;font-weight:bold;background:rgba(0,0,0,120);padding:10px;border-radius:10px;}");

    topLayout->addWidget(title);
    topLayout->addStretch();
    topLayout->addWidget(iouLabel);

    mainLayout->addLayout(topLayout);
    mainLayout->addStretch();

    QHBoxLayout* npcBtnLayout = new QHBoxLayout();

    QPushButton* rancherBtn = new QPushButton("挑战牧场主", tavernScene);
    QPushButton* sheriffBtn = new QPushButton("挑战老警长", tavernScene);
    QPushButton* bossBtn = new QPushButton("挑战华工老板", tavernScene);

    QList<QPushButton*> btns = {rancherBtn, sheriffBtn, bossBtn};
    for (auto b : btns) {
        b->setFixedSize(200, 70);
        b->setStyleSheet("QPushButton{font-size:22px;background:rgba(70,40,20,200);color:white;border:2px solid #d9a55a;border-radius:12px;}"
                         "QPushButton:hover{background:rgba(110,60,30,220);}");
        npcBtnLayout->addWidget(b);
        npcBtnLayout->addSpacing(20);
    }

    mainLayout->addLayout(npcBtnLayout);
    mainLayout->addStretch();

    QPushButton* tavernBackBtn = new QPushButton("返回主菜单", tavernScene);
    tavernBackBtn->setFixedSize(160, 50);
    tavernBackBtn->setStyleSheet("QPushButton{font-size:18px;background:#555;color:white;border-radius:8px;}"
                                 "QPushButton:hover{background:#777;}");
    mainLayout->addWidget(tavernBackBtn, 0, Qt::AlignHCenter);

    stack->addWidget(tavernScene);

    connect(rancherBtn, &QPushButton::clicked, this, [=]() {
        startBattle("牧场主");
    });
    connect(sheriffBtn, &QPushButton::clicked, this, [=]() {
        startBattle("老警长");
    });
    connect(bossBtn, &QPushButton::clicked, this, [=]() {
        startBattle("华工老板");
    });
    connect(tavernBackBtn, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(startScene);
    });

    updateTavernInfo();
}

void MainWindow::updateTavernInfo() {
    if (!iouLabel) return;
    iouLabel->setText(QString("欠条：%1 / 3").arg(player.iouCount));

    if (player.iouCount >= 3 && stack->currentWidget() != endScene) {
        QMessageBox::information(this, "胜利", "你已集齐3张欠条！游戏胜利！");
        goEndScene(true);
    }
}

void MainWindow::createBattleScene() {
    battleScene = new QWidget();
    battleScene->setStyleSheet("background:#3a2518;");

    QVBoxLayout* mainLayout = new QVBoxLayout(battleScene);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    battleTitleLabel = new QLabel("对局中 - 骗子酒馆", battleScene);
    battleTitleLabel->setStyleSheet("color:#ffcc66; font-size:30px; font-weight:bold;");
    battleTitleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(battleTitleLabel);

    QHBoxLayout* nameLayout = new QHBoxLayout();
    npcNameLabel = new QLabel("NPC", battleScene);
    playerNameLabel = new QLabel("你", battleScene);

    npcNameLabel->setStyleSheet("color:white; font-size:22px;");
    playerNameLabel->setStyleSheet("color:white; font-size:22px;");
    npcNameLabel->setAlignment(Qt::AlignCenter);
    playerNameLabel->setAlignment(Qt::AlignCenter);

    nameLayout->addWidget(npcNameLabel);
    nameLayout->addStretch();
    nameLayout->addWidget(playerNameLabel);
    mainLayout->addLayout(nameLayout);

    QGridLayout* tableLayout = new QGridLayout();

    npcCardLabels.clear();
    for (int i = 0; i < 5; ++i) {
        QLabel* card = new QLabel("🂠", battleScene);
        card->setFixedSize(80, 120);
        card->setStyleSheet("background:#8B4513; border:2px solid #ffcc66; border-radius:6px;"
                            "font-size:40px; qproperty-alignment:AlignCenter; color:white;");
        npcCardLabels.append(card);
        tableLayout->addWidget(card, 0, i);
    }

    playerCardLabels.clear();
    for (int i = 0; i < 5; ++i) {
        QLabel* card = new QLabel("🂠", battleScene);
        card->setFixedSize(80, 120);
        card->setStyleSheet("background:#8B4513; border:2px solid #ffcc66; border-radius:6px;"
                            "font-size:40px; qproperty-alignment:AlignCenter; color:white;");
        playerCardLabels.append(card);
        tableLayout->addWidget(card, 1, i);
    }

    mainLayout->addLayout(tableLayout);
    mainLayout->addSpacing(20);

    roundInfoLabel = new QLabel("请选择操作", battleScene);
    roundInfoLabel->setStyleSheet("color:#ffd36b; font-size:18px;");
    roundInfoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(roundInfoLabel);
    mainLayout->addSpacing(15);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    playCardBtn = new QPushButton("出牌", battleScene);
    viewHandBtn = new QPushButton("查看手牌", battleScene);
    challengeBtn = new QPushButton("质疑NPC", battleScene);
    backBtn = new QPushButton("放弃对局", battleScene);

    QList<QPushButton*> btns = {playCardBtn, viewHandBtn, challengeBtn, backBtn};
    for (auto b : btns) {
        b->setFixedSize(150, 50);
        b->setStyleSheet("QPushButton{font-size:18px; background:#cc7a33; color:white; border-radius:8px;}"
                         "QPushButton:hover{background:#e38c44;}");
        btnLayout->addWidget(b);
    }

    mainLayout->addLayout(btnLayout);
    stack->addWidget(battleScene);

    connect(playCardBtn, &QPushButton::clicked, this, &MainWindow::doPlayerPlayCard);
    connect(viewHandBtn, &QPushButton::clicked, this, &MainWindow::showPlayerHand);
    connect(challengeBtn, &QPushButton::clicked, this, &MainWindow::playerChallengeNpc);
    connect(backBtn, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(tavernScene);
    });
}

void MainWindow::updateBattleUI() {
    npcNameLabel->setText(currentNpc.name);
    playerNameLabel->setText(player.name);
    battleTitleLabel->setText(QString("对局中 - %1 VS 你").arg(currentNpc.name));

    for (int i = 0; i < 5; ++i) {
        if (i < currentNpc.cardCount())
            npcCardLabels[i]->show();
        else
            npcCardLabels[i]->hide();
    }

    for (int i = 0; i < 5; ++i) {
        if (i < player.cardCount())
            playerCardLabels[i]->show();
        else
            playerCardLabels[i]->hide();
    }
}

void MainWindow::createEndScene() {
    endScene = new QWidget();
    endScene->setStyleSheet("background:#1a1008;");

    QVBoxLayout* layout = new QVBoxLayout(endScene);
    layout->setContentsMargins(50, 50, 50, 50);
    layout->addStretch();

    QLabel* resultLabel = new QLabel("游戏结束", endScene);
    resultLabel->setAlignment(Qt::AlignCenter);
    resultLabel->setStyleSheet("color:#ffcc66; font-size:46px; font-weight:bold;"
                               "background:rgba(0,0,0,150); padding:20px; border-radius:15px;");
    layout->addWidget(resultLabel, 0, Qt::AlignHCenter);

    QLabel* tipLabel = new QLabel("", endScene);
    tipLabel->setAlignment(Qt::AlignCenter);
    tipLabel->setStyleSheet("color:white; font-size:22px; background:rgba(0,0,0,120); padding:15px; border-radius:10px;");
    layout->addWidget(tipLabel, 0, Qt::AlignHCenter);
    layout->addSpacing(30);

    QPushButton* againBtn = new QPushButton("返回酒馆", endScene);
    QPushButton* menuBtn = new QPushButton("返回主菜单", endScene);
    againBtn->setFixedSize(180, 60);
    menuBtn->setFixedSize(180, 60);
    againBtn->setStyleSheet("QPushButton{font-size:20px;background:#d88c3a;color:white;border-radius:10px;}"
                            "QPushButton:hover{background:#e39c4f;}");
    menuBtn->setStyleSheet("QPushButton{font-size:20px;background:#777;color:white;border-radius:10px;}"
                           "QPushButton:hover{background:#999;}");

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(againBtn);
    btnLayout->addSpacing(30);
    btnLayout->addWidget(menuBtn);
    btnLayout->addStretch();

    layout->addLayout(btnLayout);

    layout->addStretch();
    stack->addWidget(endScene);

    connect(againBtn, &QPushButton::clicked, this, &MainWindow::restartToTavern);
    connect(menuBtn, &QPushButton::clicked, this, [=]() {
        stack->setCurrentWidget(startScene);
    });

    endResultLabel = resultLabel;
    endTipLabel = tipLabel;
}

void MainWindow::updateEndScene() {
    if (playerWonLastGame) {
        endResultLabel->setText("胜利");
        endTipLabel->setText("你赢得了对局，获得欠条！继续挑战吧！");
    } else {
        endResultLabel->setText("惜败");
        endTipLabel->setText("你被识破了，下次要更谨慎地出牌……");
    }

    if (player.iouCount >= 3) {
        endResultLabel->setText("圆满结局");
        endTipLabel->setText("你集齐了所有欠条，守护了牛仔的荣耀！");
    }
}