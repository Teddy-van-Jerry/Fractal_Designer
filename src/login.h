#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <FRD_TitleBar.h>

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

    void initui();
    inline FRD_TitleBar& FRD_titleBar() const { return *this->m_titleBar; }
    bool init;

    QWidget *m_titleBarW;
    QWidget *m_menuWidget;
    QMenuBar *m_menuBar;
    class::FRD_TitleBar *m_titleBar;

    QToolBar *m_leftBorder;
    QToolBar *m_rightBorder;
    QToolBar *m_bottomBorder;

    QLabel FRD_icon;
    Qt::Edges m_lock;
    QPoint m_posCursor;

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
