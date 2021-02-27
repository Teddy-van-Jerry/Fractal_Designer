#include <QMessageBox>
#include <QLabel>
#include <QDesktopServices>
#include <QSettings>
#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // set the title
    setWindowTitle("Log in");

    QSettings config(QCoreApplication::applicationDirPath() + "/UserConfig.ini", QSettings::IniFormat);
    QString uerName = config.value("USERCONFIG/UserName", "").toString();
    QString passCode = config.value("USERCONFIG/PassCode", "").toString();
    int isRemember = config.value("USERCONFIG/IsRemberPas", "0").toInt();

    if (!uerName.isEmpty() && !passCode.isEmpty())
    {
        ui->Username_line->setText(uerName);
        ui->Passcode_line->setText(passCode);
        m_NameStr = uerName;
        m_PassStr = passCode;
        ui->checkBox_savepasscode->setChecked(true);
    }

    if (isRemember == 1)
    {
        ui->checkBox_savepasscode->setChecked(true);
        m_isRemember = true;
    }
    else
    {
        m_isRemember = false;
    }

    connect(ui->checkBox_savepasscode, SIGNAL(stateChanged(int)), this, SLOT(ChangeRememberStateSlots(int)));

}

Login::~Login()
{
    delete ui;
}

void Login::on_Login_login_clicked()
{
    // test whether the Username and Passcode are correct
    if((ui->Username_line->text() == tr("Teddy van Jerry") && ui->Passcode_line->text() == tr("61520522"))
            ||(ui->Username_line->text() == tr("SEU") && ui->Passcode_line->text() == tr("SEU615205"))
            ||(ui->Username_line->text() == tr("CSDN") && ui->Passcode_line->text() == tr("Southeast61"))
            ||(ui->Username_line->text() == tr("GitHub") && ui->Passcode_line->text() == tr("Southeast61")))
    {
        if((ui->Username_line->text() != m_NameStr || ui->Passcode_line->text() != m_PassStr) && ui->checkBox_savepasscode->isChecked())
        {
            m_NameStr = ui->Username_line->text();
            m_PassStr = ui->Passcode_line->text();

            QSettings setting(QCoreApplication::applicationDirPath() + "/UserConfig.ini", QSettings::IniFormat);
            setting.beginGroup(tr("USERCONFIG"));
            setting.setValue("UserName", m_NameStr);
            setting.setValue("PassCode", m_PassStr);
            setting.endGroup();
        }
        emit user_name(ui->Username_line->text());
        close();
    }
    else
    {
        // give warning information
        QMessageBox::warning(this, tr("Warning"), tr("Wrong Username or Passcode!"), QMessageBox::Yes);

        // clear contents
        ui->Username_line->clear();
        ui->Passcode_line->clear();

        // reset the focus of the cursor
        ui->Username_line->setFocus();
    }
}

void Login::on_Login_cancel_clicked()
{
    close(); // exit
}

bool Login::ChangeRememberStateSlots(int state)
{
    if (state != Qt::Checked)
    {
        m_isRemember = false;
        QSettings config(QCoreApplication::applicationDirPath() + "/UserConfig.ini", QSettings::IniFormat);
        config.setValue("USERCONFIG/IsRemberPas", "0");
        m_NameStr.clear();
        m_PassStr.clear();
        config.clear();
    }
    else
    {
        m_isRemember = true;
        QString str_1 = ui->Username_line->text();
        QString str_2 = ui->Passcode_line->text();
        QSettings setting(QCoreApplication::applicationDirPath() + "/UserConfig.ini", QSettings::IniFormat);
        setting.beginGroup(tr("USERCONFIG"));
        setting.setValue("UserName", str_1);
        setting.setValue("PassCode", str_2);
        setting.setValue("IsRememberPas", true);
        setting.endGroup();
    }
    return true;
}
