// #include "recordwallwindow.h"
// #include <QFile>
// #include <QTextStream>
// #include <QPixmap>
// #include <QVBoxLayout> // 添加头文件

// RecordWallWindow::RecordWallWindow(QWidget *parent) : QWidget(parent)
// {
//     setWindowTitle("Record Wall");
//     setFixedSize(400, 300);

//     // 添加背景图片
//     backgroundLabel = new QLabel(this);
//     QPixmap backgroundPixmap("C:/Users/lenovo/Desktop/bighomework/v2-b247fdf4c050ac45db753c0b93db4166_r.jpg");
//     backgroundLabel->setPixmap(backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding));
//     backgroundLabel->lower();

//     recordTextEdit = new QTextEdit(this);
//     recordTextEdit->setReadOnly(true);

//     QVBoxLayout *layout = new QVBoxLayout(this); // 确保正确使用QVBoxLayout
//     layout->addWidget(recordTextEdit);

//     // 读取记录文件
//     QFile file("record_wall.txt");
//     if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         QTextStream in(&file);
//         QString recordText = in.readAll();
//         recordTextEdit->setPlainText(recordText);
//         file.close();
//     }
// }

// RecordWallWindow::~RecordWallWindow()
// {
//     delete recordTextEdit;
//     delete backgroundLabel;
// }
