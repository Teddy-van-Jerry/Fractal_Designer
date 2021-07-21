#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    QString m_NameStr;
    QString m_PassStr;
    bool m_isRemember;
    bool m_isAutoLog;
    bool m_isLog;

signals:
    void user_name(QString);

private slots:
    void on_Login_login_clicked();

    void on_Login_cancel_clicked();

    bool ChangeRememberStateSlots(int state);

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
