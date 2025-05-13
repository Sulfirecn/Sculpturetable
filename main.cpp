#include <QApplication>
#include "coverwindow.h"
#include "gamewindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    coverwindow cover;
    gamewindow game;

    // 连接信号和槽
    QObject::connect(&cover, &coverwindow::difficultySelected, &game, &gamewindow::startGameAfterDifficultySelection);
    QObject::connect(&cover, &coverwindow::difficultySelected, [&cover, &game]() {
        cover.hide();
        game.show();
        game.setFocus(); // 确保游戏窗口获得焦点
    });

    // 新增：游戏结束后显示封面窗口
    QObject::connect(&game, &gamewindow::gameEnded, [&cover, &game]() {
        game.hide();
        cover.show();
        cover.coverBGMPlayer->play(); // 重新播放封面背景音乐
    });

    cover.show();

    return a.exec();
}
