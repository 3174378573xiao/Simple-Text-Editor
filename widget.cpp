#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>
#include <QShortcut>
#include <QWheelEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setLayout(ui->verticalLayout_main);
    ui->widget_down->setLayout(ui->horizontalLayout_down);

    QStringList codec = {
        "UTF-8",
        "UTF-16",
        "UTF-16BE",
        "UTF-16LE",
        "UTF-32",
        "UTF-32BE",
        "UTF-32LE",
        "ANSI",
        "Windows-1250 to 1258",
        "Big5",
        "Big5-HKSCS",
        "CP949",
        "EUC-JP",
        "EUC-KR",
        "GB18030",
        "HP-ROMAN8",
        "IBM 850",
        "IBM 866",
        "IBM 874",
        "ISO 2022-JP",
        "ISO 8859-1 to 10",
        "ISO 8859-13 to 16",
        "KOI8-R",
        "KOI8-U",
        "Macintosh",
        "Shift-JIS",
        "TIS-620",
        "TSCII",
    };
    ui->comboBox->addItems(codec);

    this->setWindowTitle("未打开文件 - 记事本");

    QShortcut *shortcut_save = new QShortcut(QKeySequence(tr("Ctrl+S", "File|Open")),this);
    connect(shortcut_save,&QShortcut::activated,[=](){
        on_pushButton_save_clicked();
    });
    QShortcut *shortcut_open = new QShortcut(QKeySequence(tr("Ctrl+O", "File|Open")),this);
    connect(shortcut_open,&QShortcut::activated,[=](){
        on_pushButton_open_clicked();
    });
    QShortcut *shortcut_close = new QShortcut(QKeySequence(tr("Ctrl+E", "File|Open")),this);
    connect(shortcut_close,&QShortcut::activated,[=](){
        on_pushButton_close_clicked();
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0){
        ui->textEdit->zoomIn();
    }else{
        ui->textEdit->zoomOut();
    }
}

//打开
void Widget::on_pushButton_open_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
          tr("打开文件"), "C:\\Users\\86131\\Desktop\\", tr("Text files (*.txt *.doc)"));


    if(file.isOpen())
        file.close();
    file.setFileName(fileName);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)){
//        qDebug()<<"文件打开失败";
        QMessageBox msgBox;
        msgBox.setText("文件打开失败");
        msgBox.exec();
        return;
    }

    ui->textEdit->clear();

    QTextStream in(&file);
    in.setCodec(codec.toStdString().c_str());
    in.seek(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->textEdit->append(line);
    }
    this->setWindowTitle(fileName+" - 记事本");
}

void Widget::on_pushButton_save_clicked()
{
    if(!file.isOpen()){
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("保存文件"), "C:\\Users\\86131\\Desktop\\untitled.txt", tr("Text files (*.txt *.doc)"));

        if(file.isOpen())
            file.close();
        file.setFileName(fileName);

        if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
//            qDebug()<<"文件保存失败";
            QMessageBox msgBox;
            msgBox.setText("文件保存失败");
            msgBox.exec();
            return;
        }
        this->setWindowTitle(fileName+" - 记事本");
    }

    QTextStream out(&file);
    out.setCodec(codec.toStdString().c_str());

    QString str = ui->textEdit->toPlainText();
    out<<str;


}

void Widget::on_pushButton_close_clicked()
{
    QMessageBox msgBox;
    msgBox.setText(tr("是否要保存更改？"));
    QPushButton *saveButton = msgBox.addButton(tr("保存"), QMessageBox::ActionRole);
    msgBox.addButton(tr("不保存"),QMessageBox::ActionRole);
    QPushButton *noButton = msgBox.addButton(tr("取消"),QMessageBox::ActionRole);

    msgBox.exec();
    if (msgBox.clickedButton() == noButton){
        return;
    } else if (msgBox.clickedButton() == saveButton) {
         on_pushButton_save_clicked();
    }
    ui->textEdit->clear();
    if(file.isOpen()){
        file.close();
        this->setWindowTitle("未打开文件 - 记事本");
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    on_pushButton_close_clicked();
    this->close();
}



void Widget::on_comboBox_activated(const QString &arg1)
{
    codec = arg1;

    if (!file.isOpen() && !file.open(QIODevice::ReadWrite | QIODevice::Text)){
//        qDebug()<<"文件打开失败";
        QMessageBox msgBox;
        msgBox.setText("文件打开失败");
        msgBox.exec();
        return;
    }

    ui->textEdit->clear();

    QTextStream in(&file);
    in.setCodec(codec.toStdString().c_str());
    in.seek(0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        ui->textEdit->append(line);
    }
}

void Widget::on_textEdit_cursorPositionChanged()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    const QString mes = "第" + QString::number(cursor.blockNumber()+1) + "行第" + QString::number(cursor.columnNumber()+1) + "列";
    ui->label->setText(mes);

    //设置高亮
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection extra;

    extra.cursor = cursor;
    QBrush q(Qt::gray);
    extra.format.setBackground(q);
    extra.format.setProperty(QTextFormat::FullWidthSelection,true);

    extraSelections.append(extra);
    ui->textEdit->setExtraSelections(extraSelections);
}
