#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTime>

#include <stdio.h>
#include <time.h>
#include <string.h>
using namespace std;

void Sleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget w;
    w.resize(400, 200);
    w.setWindowTitle("Process2");
    QLabel textLabel;
    textLabel.resize(200,40);
    textLabel.move(120, 90);
    textLabel.setParent(&w);
    textLabel.show();
    w.show();
    time_t t;
    while(1){
        time(&t);
        textLabel.setText(QString::fromStdString(ctime(&t)));
        textLabel.repaint();
        Sleep(1000);
    }
    return app.exec();
}
