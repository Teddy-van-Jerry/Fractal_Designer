#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <QDialog>
#include <QScrollArea>
#include <QPushButton>
#include <QFile>
#include <QLabel>

struct Possible_Result
{
    int index = -1;
    QString Text;
    QString Tip;
    QStringList Key_Words;
    int value = 0;
};

namespace Ui {
class Search_Result;
}

class Search_Result : public QDialog
{
    Q_OBJECT

public:
    explicit Search_Result(QWidget *parent = nullptr);
    ~Search_Result();

    QScrollArea* Scroll_Area_Search;

signals:
    void search_result_clicked_signal(QString);

public slots:
    void getSearch(QString);

private:
    Ui::Search_Result *ui;

    QVector<QPushButton*> btn;

    QLabel* label;

    QPushButton *Help_E, *Help_C;

    bool already_open = false;
};

#endif // SEARCH_RESULT_H
