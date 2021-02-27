#include "set_colour.h"
#include "ui_set_colour.h"
#include "mainwindow.h"

Set_Colour::Set_Colour(QWidget *parent, QString name, int n) :
    QDialog(parent),
    ui(new Ui::Set_Colour),
    Project_Name_t(name),
    type_n(n)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    ui->comboBox->setCurrentIndex(0);

    // Colour_SET[0][3][28][1] = Colour_SET[1][3][28][1] = 255;

    if(type_n == 0)
    {
        ui->label_tpye->setText("Convergent");
        ui->Change_1->setText("|Zn| / MIN");
        ui->Change_2->setText("|Z1| / MIN");
        ui->Change_3->setText("e^(|Zn| / MIN)");
        ui->Change_4->setText("e^(|Z1| / MIN)");
    }
    else // n == 1
    {
        ui->label_tpye->setText("Unconvergent");
        ui->Change_1->setText("|Zn| / MAX");
        ui->Change_2->setText("|Z1| / MAX");
        ui->Change_3->setText("e^(|Zn| / MAX)");
        ui->Change_4->setText("e^(|Z1| / MAX)");
    }
    initData(type_n);
    presentData(type_n, 0);
    showColourFormula();
}

Set_Colour::~Set_Colour()
{
    delete ui;
}

void Set_Colour::initData(int i)
{
    MainWindow* p = (MainWindow*)parentWidget();
    if(p->Version_Higher_Than_4)
    {
        qDebug() << "High Version Colour ini";
        For_All_Colour(i_, j_)
            if(i == 0)
            {
                Colour_SET[0][i_][j_][0] = p->pre_info[p->current_info_v].Colour_Data_1[i_][j_][0];
                Colour_SET[0][i_][j_][1] = p->pre_info[p->current_info_v].Colour_Data_1[i_][j_][1];
            }
            else
            {
                Colour_SET[1][i_][j_][0] = p->pre_info[p->current_info_v].Colour_Data_2[i_][j_][0];
                Colour_SET[1][i_][j_][1] = p->pre_info[p->current_info_v].Colour_Data_2[i_][j_][1];
            }
        End_All_Colour
    }
    else // low version
    {
        QString pre_file_name_temp = Project_Name_t + ((type_n == 0)? "/Colour_Set_temp1.txt" : "/Colour_Set_temp2.txt");
        QString pre_file_name_saved = Project_Name_t + ((type_n == 0)? "/Colour_Set_1.txt" : "/Colour_Set_2.txt");
        QFile Colour_Set_saved_pre(pre_file_name_saved);
        QFile Colour_Set_temp_pre(pre_file_name_temp);
        if(Colour_Set_saved_pre.exists())
        {
            QFile::copy(pre_file_name_saved, pre_file_name_temp);

            Colour_Set_temp_pre.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&Colour_Set_temp_pre);

            for(int j = 0; j != 4; j++)
            {
                in >> Colour_SET[i][j][0][0]  >> Colour_SET[i][j][0][1];
                in >> Colour_SET[i][j][1][0]  >> Colour_SET[i][j][1][1];
                in >> Colour_SET[i][j][2][0]  >> Colour_SET[i][j][2][1];
                in >> Colour_SET[i][j][3][0]  >> Colour_SET[i][j][3][1];
                in >> Colour_SET[i][j][4][0]  >> Colour_SET[i][j][4][1];
                in >> Colour_SET[i][j][5][0]  >> Colour_SET[i][j][5][1];
                in >> Colour_SET[i][j][6][0]  >> Colour_SET[i][j][6][1];
                in >> Colour_SET[i][j][7][0]  >> Colour_SET[i][j][7][1];
                in >> Colour_SET[i][j][8][0]  >> Colour_SET[i][j][8][1];
                in >> Colour_SET[i][j][9][0]  >> Colour_SET[i][j][9][1];
                in >> Colour_SET[i][j][10][0] >> Colour_SET[i][j][10][1];
                in >> Colour_SET[i][j][11][0] >> Colour_SET[i][j][11][1];
                in >> Colour_SET[i][j][12][0] >> Colour_SET[i][j][12][1];
                in >> Colour_SET[i][j][13][0] >> Colour_SET[i][j][13][1];
                in >> Colour_SET[i][j][14][0] >> Colour_SET[i][j][14][1];
                in >> Colour_SET[i][j][15][0] >> Colour_SET[i][j][15][1];
                in >> Colour_SET[i][j][16][0] >> Colour_SET[i][j][16][1];
                in >> Colour_SET[i][j][17][0] >> Colour_SET[i][j][17][1];
                in >> Colour_SET[i][j][18][0] >> Colour_SET[i][j][18][1];
                in >> Colour_SET[i][j][19][0] >> Colour_SET[i][j][19][1];
                in >> Colour_SET[i][j][20][0] >> Colour_SET[i][j][20][1];
                in >> Colour_SET[i][j][21][0] >> Colour_SET[i][j][21][1];
                in >> Colour_SET[i][j][22][0] >> Colour_SET[i][j][22][1];
                in >> Colour_SET[i][j][23][0] >> Colour_SET[i][j][23][1];
                in >> Colour_SET[i][j][24][0] >> Colour_SET[i][j][24][1];
                in >> Colour_SET[i][j][25][0] >> Colour_SET[i][j][25][1];
                in >> Colour_SET[i][j][26][0] >> Colour_SET[i][j][26][1];
                in >> Colour_SET[i][j][27][0] >> Colour_SET[i][j][27][1];
                in >> Colour_SET[i][j][28][0] >> Colour_SET[i][j][28][1];
            }
            Colour_Set_temp_pre.close();
        }
        else
        {
            for(int k = 0; k != 4; k++)
            {
                clearData(k);
            }
            // Colour_SET[i][3][28][1] = 255;
        }
    }
    qDebug() << Colour_SET[0][0][0][1];
}

void Set_Colour::on_pushButton_Cancel_clicked()
{
    close();
}

void Set_Colour::presentData(int i, int j)
{
    ui->SC_1->setValue(Colour_SET[i][j][0][0]);
    ui->SC_2->setValue(Colour_SET[i][j][1][0]);
    ui->SC_3->setValue(Colour_SET[i][j][2][0]);
    ui->SC_4->setValue(Colour_SET[i][j][3][0]);
    ui->SC_5->setValue(Colour_SET[i][j][4][0]);
    ui->SC_6->setValue(Colour_SET[i][j][5][0]);
    ui->SC_7->setValue(Colour_SET[i][j][6][0]);
    ui->SC_8->setValue(Colour_SET[i][j][7][0]);
    ui->SC_9->setValue(Colour_SET[i][j][8][0]);
    ui->SC_10->setValue(Colour_SET[i][j][9][0]);
    ui->SC_11->setValue(Colour_SET[i][j][10][0]);
    ui->SC_12->setValue(Colour_SET[i][j][11][0]);
    ui->SC_13->setValue(Colour_SET[i][j][12][0]);
    ui->SC_14->setValue(Colour_SET[i][j][13][0]);
    ui->SC_15->setValue(Colour_SET[i][j][14][0]);
    ui->SC_16->setValue(Colour_SET[i][j][15][0]);
    ui->SC_17->setValue(Colour_SET[i][j][16][0]);
    ui->SC_18->setValue(Colour_SET[i][j][17][0]);
    ui->SC_19->setValue(Colour_SET[i][j][18][0]);
    ui->SC_20->setValue(Colour_SET[i][j][19][0]);
    ui->SC_21->setValue(Colour_SET[i][j][20][0]);
    ui->SC_22->setValue(Colour_SET[i][j][21][0]);
    ui->SC_23->setValue(Colour_SET[i][j][22][0]);
    ui->SC_24->setValue(Colour_SET[i][j][23][0]);
    ui->SC_25->setValue(Colour_SET[i][j][24][0]);
    ui->SC_26->setValue(Colour_SET[i][j][25][0]);
    ui->SC_27->setValue(Colour_SET[i][j][26][0]);
    ui->SC_28->setValue(Colour_SET[i][j][27][0]);
    ui->SC_29->setValue(Colour_SET[i][j][28][0]);

    ui->SC_1_->setValue(Colour_SET[i][j][0][1]);
    ui->SC_2_->setValue(Colour_SET[i][j][1][1]);
    ui->SC_3_->setValue(Colour_SET[i][j][2][1]);
    ui->SC_4_->setValue(Colour_SET[i][j][3][1]);
    ui->SC_5_->setValue(Colour_SET[i][j][4][1]);
    ui->SC_6_->setValue(Colour_SET[i][j][5][1]);
    ui->SC_7_->setValue(Colour_SET[i][j][6][1]);
    ui->SC_8_->setValue(Colour_SET[i][j][7][1]);
    ui->SC_9_->setValue(Colour_SET[i][j][8][1]);
    ui->SC_10_->setValue(Colour_SET[i][j][9][1]);
    ui->SC_11_->setValue(Colour_SET[i][j][10][1]);
    ui->SC_12_->setValue(Colour_SET[i][j][11][1]);
    ui->SC_13_->setValue(Colour_SET[i][j][12][1]);
    ui->SC_14_->setValue(Colour_SET[i][j][13][1]);
    ui->SC_15_->setValue(Colour_SET[i][j][14][1]);
    ui->SC_16_->setValue(Colour_SET[i][j][15][1]);
    ui->SC_17_->setValue(Colour_SET[i][j][16][1]);
    ui->SC_18_->setValue(Colour_SET[i][j][17][1]);
    ui->SC_19_->setValue(Colour_SET[i][j][18][1]);
    ui->SC_20_->setValue(Colour_SET[i][j][19][1]);
    ui->SC_21_->setValue(Colour_SET[i][j][20][1]);
    ui->SC_22_->setValue(Colour_SET[i][j][21][1]);
    ui->SC_23_->setValue(Colour_SET[i][j][22][1]);
    ui->SC_24_->setValue(Colour_SET[i][j][23][1]);
    ui->SC_25_->setValue(Colour_SET[i][j][24][1]);
    ui->SC_26_->setValue(Colour_SET[i][j][25][1]);
    ui->SC_27_->setValue(Colour_SET[i][j][26][1]);
    ui->SC_28_->setValue(Colour_SET[i][j][27][1]);
    ui->SC_29_->setValue(Colour_SET[i][j][28][1]);
}

void Set_Colour::storeData(int i, int j)
{
    Colour_SET[i][j][0][0]  = ui->SC_1->value();
    Colour_SET[i][j][1][0]  = ui->SC_2->value();
    Colour_SET[i][j][2][0]  = ui->SC_3->value();
    Colour_SET[i][j][3][0]  = ui->SC_4->value();
    Colour_SET[i][j][4][0]  = ui->SC_5->value();
    Colour_SET[i][j][5][0]  = ui->SC_6->value();
    Colour_SET[i][j][6][0]  = ui->SC_7->value();
    Colour_SET[i][j][7][0]  = ui->SC_8->value();
    Colour_SET[i][j][8][0]  = ui->SC_9->value();
    Colour_SET[i][j][9][0]  = ui->SC_10->value();
    Colour_SET[i][j][10][0] = ui->SC_11->value();
    Colour_SET[i][j][11][0] = ui->SC_12->value();
    Colour_SET[i][j][12][0] = ui->SC_13->value();
    Colour_SET[i][j][13][0] = ui->SC_14->value();
    Colour_SET[i][j][14][0] = ui->SC_15->value();
    Colour_SET[i][j][15][0] = ui->SC_16->value();
    Colour_SET[i][j][16][0] = ui->SC_17->value();
    Colour_SET[i][j][17][0] = ui->SC_18->value();
    Colour_SET[i][j][18][0] = ui->SC_19->value();
    Colour_SET[i][j][19][0] = ui->SC_20->value();
    Colour_SET[i][j][20][0] = ui->SC_21->value();
    Colour_SET[i][j][21][0] = ui->SC_22->value();
    Colour_SET[i][j][22][0] = ui->SC_23->value();
    Colour_SET[i][j][23][0] = ui->SC_24->value();
    Colour_SET[i][j][24][0] = ui->SC_25->value();
    Colour_SET[i][j][25][0] = ui->SC_26->value();
    Colour_SET[i][j][26][0] = ui->SC_27->value();
    Colour_SET[i][j][27][0] = ui->SC_28->value();
    Colour_SET[i][j][28][0] = ui->SC_29->value();

    Colour_SET[i][j][0][1]  = ui->SC_1_->value();
    Colour_SET[i][j][1][1]  = ui->SC_2_->value();
    Colour_SET[i][j][2][1]  = ui->SC_3_->value();
    Colour_SET[i][j][3][1]  = ui->SC_4_->value();
    Colour_SET[i][j][4][1]  = ui->SC_5_->value();
    Colour_SET[i][j][5][1]  = ui->SC_6_->value();
    Colour_SET[i][j][6][1]  = ui->SC_7_->value();
    Colour_SET[i][j][7][1]  = ui->SC_8_->value();
    Colour_SET[i][j][8][1]  = ui->SC_9_->value();
    Colour_SET[i][j][9][1]  = ui->SC_10_->value();
    Colour_SET[i][j][10][1] = ui->SC_11_->value();
    Colour_SET[i][j][11][1] = ui->SC_12_->value();
    Colour_SET[i][j][12][1] = ui->SC_13_->value();
    Colour_SET[i][j][13][1] = ui->SC_14_->value();
    Colour_SET[i][j][14][1] = ui->SC_15_->value();
    Colour_SET[i][j][15][1] = ui->SC_16_->value();
    Colour_SET[i][j][16][1] = ui->SC_17_->value();
    Colour_SET[i][j][17][1] = ui->SC_18_->value();
    Colour_SET[i][j][18][1] = ui->SC_19_->value();
    Colour_SET[i][j][19][1] = ui->SC_20_->value();
    Colour_SET[i][j][20][1] = ui->SC_21_->value();
    Colour_SET[i][j][21][1] = ui->SC_22_->value();
    Colour_SET[i][j][22][1] = ui->SC_23_->value();
    Colour_SET[i][j][23][1] = ui->SC_24_->value();
    Colour_SET[i][j][24][1] = ui->SC_25_->value();
    Colour_SET[i][j][25][1] = ui->SC_26_->value();
    Colour_SET[i][j][26][1] = ui->SC_27_->value();
    Colour_SET[i][j][27][1] = ui->SC_28_->value();
    Colour_SET[i][j][28][1] = ui->SC_29_->value();
}

void Set_Colour::saveData(int i)
{
    MainWindow* p = (MainWindow*)parentWidget();
    if(p->Version_Higher_Than_4)
    {
        For_All_Colour(i_, j_)
            if(i == 0)
            {
                p->buff_info.Colour_Data_1[i_][j_][0] = Colour_SET[0][i_][j_][0];
                p->buff_info.Colour_Data_1[i_][j_][1] = Colour_SET[0][i_][j_][1];
            }
            else
            {
                p->buff_info.Colour_Data_2[i_][j_][0] = Colour_SET[1][i_][j_][0];
                p->buff_info.Colour_Data_2[i_][j_][1] = Colour_SET[1][i_][j_][1];
            }
        End_All_Colour
        p->edit(1); // EDIT_ALREADY
        if(p->isColourAutoSaved())
        {
            p->saveElsewhere();
        }
    }
    else // low version
    {
        QString file_name = Project_Name_t + ((i == 0)? "/Colour_Set_temp1.txt" : "/Colour_Set_temp2.txt");
        QFile Colour_Set_File(file_name);
        Colour_Set_File.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&Colour_Set_File);

        for(int j = 0; j != 4; j++)
        {
            out << Colour_SET[i][j][0][0]  << " " << Colour_SET[i][j][0][1]  << Qt::endl;
            out << Colour_SET[i][j][1][0]  << " " << Colour_SET[i][j][1][1]  << Qt::endl;
            out << Colour_SET[i][j][2][0]  << " " << Colour_SET[i][j][2][1]  << Qt::endl;
            out << Colour_SET[i][j][3][0]  << " " << Colour_SET[i][j][3][1]  << Qt::endl;
            out << Colour_SET[i][j][4][0]  << " " << Colour_SET[i][j][4][1]  << Qt::endl;
            out << Colour_SET[i][j][5][0]  << " " << Colour_SET[i][j][5][1]  << Qt::endl;
            out << Colour_SET[i][j][6][0]  << " " << Colour_SET[i][j][6][1]  << Qt::endl;
            out << Colour_SET[i][j][7][0]  << " " << Colour_SET[i][j][7][1]  << Qt::endl;
            out << Colour_SET[i][j][8][0]  << " " << Colour_SET[i][j][8][1]  << Qt::endl;
            out << Colour_SET[i][j][9][0]  << " " << Colour_SET[i][j][9][1]  << Qt::endl;
            out << Colour_SET[i][j][10][0] << " " << Colour_SET[i][j][10][1] << Qt::endl;
            out << Colour_SET[i][j][11][0] << " " << Colour_SET[i][j][11][1] << Qt::endl;
            out << Colour_SET[i][j][12][0] << " " << Colour_SET[i][j][12][1] << Qt::endl;
            out << Colour_SET[i][j][13][0] << " " << Colour_SET[i][j][13][1] << Qt::endl;
            out << Colour_SET[i][j][14][0] << " " << Colour_SET[i][j][14][1] << Qt::endl;
            out << Colour_SET[i][j][15][0] << " " << Colour_SET[i][j][15][1] << Qt::endl;
            out << Colour_SET[i][j][16][0] << " " << Colour_SET[i][j][16][1] << Qt::endl;
            out << Colour_SET[i][j][17][0] << " " << Colour_SET[i][j][17][1] << Qt::endl;
            out << Colour_SET[i][j][18][0] << " " << Colour_SET[i][j][18][1] << Qt::endl;
            out << Colour_SET[i][j][19][0] << " " << Colour_SET[i][j][19][1] << Qt::endl;
            out << Colour_SET[i][j][20][0] << " " << Colour_SET[i][j][20][1] << Qt::endl;
            out << Colour_SET[i][j][21][0] << " " << Colour_SET[i][j][21][1] << Qt::endl;
            out << Colour_SET[i][j][22][0] << " " << Colour_SET[i][j][22][1] << Qt::endl;
            out << Colour_SET[i][j][23][0] << " " << Colour_SET[i][j][23][1] << Qt::endl;
            out << Colour_SET[i][j][24][0] << " " << Colour_SET[i][j][24][1] << Qt::endl;
            out << Colour_SET[i][j][25][0] << " " << Colour_SET[i][j][25][1] << Qt::endl;
            out << Colour_SET[i][j][26][0] << " " << Colour_SET[i][j][26][1] << Qt::endl;
            out << Colour_SET[i][j][27][0] << " " << Colour_SET[i][j][27][1] << Qt::endl;
            out << Colour_SET[i][j][28][0] << " " << Colour_SET[i][j][28][1] << Qt::endl;
        }
        Colour_Set_File.close();
    }
}

void Set_Colour::clearData(int i)
{
    for(int k = 0; k!= 29; k++)
    {
        Colour_SET[type_n][i][k][0] = 0;
        Colour_SET[type_n][i][k][1] = 0;
    }
    presentData(type_n, i);
}

void Set_Colour::on_pushButton_Save_clicked()
{
    storeData(type_n, Colour_index);
    saveData(type_n);
    showColourFormula();
}

void Set_Colour::on_comboBox_currentIndexChanged(int index)
{
    storeData(type_n, Colour_index);
    Colour_index = index;
    presentData(type_n, index);
}

void Set_Colour::on_pushButton_Clear_clicked()
{
    clearData(Colour_index);
}

void Set_Colour::showColourFormula()
{
    MainWindow* p = (MainWindow*)parentWidget();
    p->showColourFormula();
}
