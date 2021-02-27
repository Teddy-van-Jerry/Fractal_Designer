#ifndef SET_COLOUR_H
#define SET_COLOUR_H

#include <QDialog>
#include <QFile>

namespace Ui {
class Set_Colour;
}

class Set_Colour : public QDialog
{
    Q_OBJECT

public:
    explicit Set_Colour(QWidget *parent = nullptr, QString name = "", int n = 0);
    ~Set_Colour();

    double Colour_SET[2][4][29][2] = {0};

    void showColourFormula();

private slots:
    void on_pushButton_Cancel_clicked();

//    void receiveInfoForColourSet(QString, int);

    void on_pushButton_Save_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_Clear_clicked();

private:
    Ui::Set_Colour *ui;

    void initData(int i);

    void presentData(int i, int j);

    void storeData(int i, int j);

    void saveData(int i);

    void clearData(int i);

    QString Project_Name_t;

    int type_n;

    int Colour_index = 0;
};

#endif // SET_COLOUR_H
