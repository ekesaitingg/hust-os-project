#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QTime>

#include <stdio.h>
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
    w.setWindowTitle("Process3");
    QLabel textLabel;
    textLabel.resize(200,40);
    textLabel.move(200, 90);
    textLabel.setParent(&w);
    textLabel.show();
    w.show();

    char x[1];
    x[0]='0';
    while(x[0]<='9')
    {
        textLabel.setText(QString::fromStdString(x));
        textLabel.repaint();
        Sleep(500);
        if(x[0]=='9') x[0]='0';
        else x[0]++;
    }
    return app.exec();
}
