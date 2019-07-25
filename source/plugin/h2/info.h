#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include "../plugin/xpage.h"

namespace Ui {
class H2Info;
}

namespace mrx_h2 {

class Info : public XPage
{
    Q_OBJECT

public:
    explicit Info(QString strDevInfo = "", QWidget *parent = 0);
    ~Info();

    void change_online_status(bool bl);

    int readDeviceConfig(); //从设备上读取配置数据
    int writeDeviceConfig();//将软件界面的数据配置设置到设备
    int loadConfig();       //读取本地配置文件 启动软件或者添加新设备时调用
    int saveConfig();       //将软件界面的数据配置写入到本地配置文件
    void updateShow();      //更新界面

    void translateUI();

signals:
    void signal_online_clicked(QString);

private slots:
    void on_pushButton_status_clicked();

private:
    Ui::H2Info *ui;

    QString m_IP;
    QString m_Version;
    QString m_Type;
    QString m_SN;
};

}

#endif // H2PRODUCT_H
