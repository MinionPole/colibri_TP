#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , TManager(this)
{
    ui->setupUi(this);
    wantExit = 0;
    //rules
    ui->timerValueEdit->setValidator(new QIntValidator(1, 100, this));
    QRegularExpression xorValueValidatorExp("[0-9A-F]{16}");
    ui->valueXOREdit->setValidator(new QRegularExpressionValidator(xorValueValidatorExp));

    //connections
    connect(ui->selectInitialFolderButton, &QPushButton::clicked,
            this, &MainWindow::selectInitialFolderClick);
    connect(ui->selectResultFolderButton, &QPushButton::clicked,
            this, &MainWindow::selectResultFolderClick);
    connect(ui->startProcessButton, &QPushButton::clicked,
            this, &MainWindow::startProcess);
    connect(ui->exitMainMenuButton, &QPushButton::clicked,
            this, &MainWindow::exitMainMenu);
    connect(&this->TManager, &CalculationThreadManager::workersVectorEmpty, this, &MainWindow::exitChecker);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectResultFolderClick(){

    QString adressString = QFileDialog::getExistingDirectory(
        this,
        tr("Выберите папку"),
        QDir::currentPath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if(!adressString.isEmpty()){
        this->resultPath = adressString;
        ui->ResultAdressLabel->setText(adressString.last(qMin(adressString.length(), 30)));
    }
};

void MainWindow::selectInitialFolderClick(){

    QString adressString = QFileDialog::getExistingDirectory(
        this,
        tr("Выберите папку"),
        QDir::currentPath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

    if(!adressString.isEmpty()){
        this->initialPath = adressString;
        ui->InitialAdressLabel->setText(adressString.last(qMin(adressString.length(), 30)));
    }
};

void MainWindow::exitMainMenu(){
    wantExit = 1;
    this->TManager.exitProcess();
}

void MainWindow::exitChecker(){
    if(wantExit)
        this->close();
}

void MainWindow::startProcess(){
    CalculationData data;
    bool ok = false;
    data.timerWork = ui->timerWorkCB->checkState();
        if(data.timerWork){

        data.timerTime = ui->timerValueEdit->text().toInt(&ok);
        if(!ok){
            QMessageBox::critical(this, "Error", "Нет времени таймера");
            return;
        }
    }

    data.deleteInitial =ui->deleteInitialFlagCB->checkState();
    QRegularExpression re(ui->regularExprEdit->text());
    if (!re.isValid()) {
        QMessageBox::critical(this, "Ошибка",
                             "Некорректное регулярное выражение:\n" + re.errorString());
        return;
    }
    data.fileExpr = re;

    data.initialPath = this->initialPath;
    data.resultPath = this->resultPath;
    data.xorKey = ui->valueXOREdit->text().toULongLong(&ok, 16);
    if(!ok){
        QMessageBox::critical(this, "Error", "неправильное значение для xor");
        return;
    }
    int worker_val = this->TManager.createWorker(data);
    return;
};

void MainWindow::closeEvent(QCloseEvent *event) {
    if(wantExit != 1){
        event->ignore();
        ui->exitMainMenuButton->click();
    }
}
