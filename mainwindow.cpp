#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExpValidator>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    toggleWidgets(true);
    reactToKeyLength();
    ui->keyLine->setValidator(new QRegExpValidator(QRegExp("[A-Za-z0-9\\d]+"), this));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::reactToKeyLength()
{
    if(ui->keyLine->text().length() < 16 )
    {
        ui->rememberKeyButton->setDisabled(true);
    }
    else
        ui->rememberKeyButton->setDisabled(false);
}

void MainWindow::encode()
{

    ByteArray text = splitQString(ui->firstTextField->toPlainText());

    ByteArray keyBA = splitKey(key);

    ByteArray enc;
    AES_Engine::encrypt(keyBA, text, enc);

    QString test;
    for (unsigned long i = 0; i < enc.size(); i+=2)
    {
        if(i+1 == enc.size())
            test.append(QChar(enc[i]<<8|0x00));
        else
            test.append(QChar(enc[i]<<8|enc[i+1]));
    }

    ui->firstTextField->setPlainText(test);
    sendTo(MainWindow::right);
}

void MainWindow::decode()
{
    ByteArray text = splitQString(ui->secondTextField->toPlainText());

    ByteArray keyBA = splitKey(key);

    ByteArray dec;
    AES_Engine::decrypt(keyBA, text, dec);

    QString test;
    for (unsigned long i = 0; i < dec.size(); i+=2)
    {
        if(i+1 == dec.size())
            test.append(QChar(dec[i]<<8|0x00));
        else
            test.append(QChar(dec[i]<<8|dec[i+1]));
    }

    ui->secondTextField->setPlainText(test);
    sendTo(MainWindow::left);
}

void MainWindow::clearAll()
{
    ui->firstTextField->clear();
    ui->secondTextField->clear();
    ui->keyLine->clear();
    toggleWidgets(true);
    key.clear();
    ui->rememberKeyButton->setDisabled(true);
}

void MainWindow::rememberKey()
{
    QRegExp rool("^[a-zA-Z0-9]");
    if(!ui->keyLine->text().contains(rool)
       || ui->keyLine->text().length() <= 0)
    {
        QMessageBox temp(QMessageBox::Warning, "Warning", "This key is not valid.");
        temp.exec();
        return;
    }
    key = ui->keyLine->text();
    toggleWidgets(false);
}

void MainWindow::toggleWidgets(bool toggle)
{
    ui->keyLine->setEnabled(toggle);
    ui->keyLabel->setEnabled(toggle);
    ui->rememberKeyButton->setEnabled(toggle);

    ui->firstTextField->setDisabled(toggle);
    ui->firstTextFieldLabel->setDisabled(toggle);
    ui->secondTextField->setDisabled(toggle);
    ui->secondTextFieldLabel->setDisabled(toggle);

    ui->encodeButton->setDisabled(toggle);
    ui->decodeButton->setDisabled(toggle);

    ui->clearAllButton->setDisabled(toggle);
}

void MainWindow::sendTo(int where)
{
    if(where == MainWindow::left)
    {
        ui->firstTextField->setPlainText(ui->secondTextField->toPlainText());
        ui->secondTextField->clear();
    }
    else if (where == MainWindow::right)
    {
        ui->secondTextField->setPlainText(ui->firstTextField->toPlainText());
        ui->firstTextField->clear();
    }
}

ByteArray MainWindow::splitQString(QString text)
{
    ByteArray result;
    for (int i = 0; i < (2*text.length()); ++i)
        if(i%2==0)
            result.push_back(text[int(i/2)].unicode()>>8);
        else
            if(!((i==2*text.length()-1)&&((text[int(i/2)].unicode()&0xff)==0xf0)))
                result.push_back(text[int(i/2)].unicode()&0xff);

    return result;
}

ByteArray MainWindow::splitKey(QString key)
{
    ByteArray result;
    for (int i = 0; i < (2*key.length()); ++i)
        if(i%2==0)
            result.push_back(key[int(i/2)].unicode()>>8);
        else
            result.push_back(key[int(i/2)].unicode()&0xff);
    return result;
}




























