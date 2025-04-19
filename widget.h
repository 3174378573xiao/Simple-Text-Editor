#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QTextCodec>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QFile file;
    QString codec = "UTF-8";


    void wheelEvent(QWheelEvent *event) override;
    void closeEvent(QCloseEvent *event) override;


private slots:
    void on_pushButton_open_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_close_clicked();

    void on_comboBox_activated(const QString &arg1);

    void on_textEdit_cursorPositionChanged();

private:
    Ui::Widget *ui;

};
#endif // WIDGET_H
