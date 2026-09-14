#include "calculationworker.h"

CalculationWorker::CalculationWorker(const CalculationData &data, QObject *parent)
    : QObject{parent}
{
    this->data = data;
    fileNameCnt = 0;
}

void CalculationWorker::startWorker(){
    if (onWork != 0) return;
    onWork = 1;

    QDir dir(data.initialPath);
    QFileInfoList allFiles = dir.entryInfoList(
        QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
    totalFiles = 0; // оценка для прогресс бара
    for (const QFileInfo &fi : allFiles) {
        if (data.fileExpr.match(fi.fileName()).hasMatch())
            totalFiles++;
    }
    processedFiles = 0;

    dirIterator = new QDirIterator(data.initialPath,
                                   QDir::Files | QDir::NoDotAndDotDot);
    QMetaObject::invokeMethod(this, "doWorkStep", Qt::QueuedConnection);
};


void CalculationWorker::pauseWorker(){
    sync.lock();
    paused = true;
    sync.unlock();
    pauseCond.wakeAll();
};

void CalculationWorker::resumeWorker(){
    sync.lock();
    paused = false;
    sync.unlock();
    pauseCond.wakeAll();
};

void CalculationWorker::stopWorker(){
    sync.lock();
    onWork = 0;
    sync.unlock();
    pauseCond.wakeAll();
};

void CalculationWorker::exitWorker(){
    sync.lock(); 
    onWork = -1;
    sync.unlock();
    pauseCond.wakeAll();
    if(inStep == false)
        emit finished();
};

void CalculationWorker::doWorkStep(){
    sync.lock();
    inStep = true;
    while(paused && onWork == 1)
        pauseCond.wait(&sync);
    if(onWork != 1){
        if(currentFile != nullptr){
            currentFile->close();
        }
        if(resultFile != nullptr){
            resultFile->close();
        }
        delete currentFile;
        delete resultFile;
        currentFile = nullptr;
        resultFile = nullptr;
        delete dirIterator;
        inStep = false;
        if(onWork == -1)
            emit finished();
        sync.unlock();
        return;
    }

    if(currentFile == nullptr){
        if(!dirIterator->hasNext()){
            onWork = 0;
        }
        else
            while(dirIterator->hasNext()){
                dirIterator->next();
                if(data.fileExpr.match(dirIterator->fileName()).hasMatch()){
                    this->currentFile = new QFile(dirIterator->filePath());
                    if(!this->currentFile->open(QIODevice::ReadOnly)){
                        delete this->currentFile;
                        this->currentFile = nullptr;
                        continue;
                    }
                    QFileInfo info(dirIterator->filePath());
                    QString newName = info.completeBaseName()+ QString::number(fileNameCnt++)+ "." + info.suffix();
                    resultFile = new QFile(data.resultPath + '/' + newName);
                    if (!resultFile->open(QIODevice::WriteOnly)) {
                        qWarning() << "не открывается файл дебаг" << resultFile->errorString();
                        currentFile->close();
                        this->currentFile = nullptr;
                        this->resultFile = nullptr;
                        continue;
                    }
                    break;
                }
            }
    }
    else{
      // место для проверки на то что файл все ещё есть
    }

    if (this->currentFile != nullptr && !this->currentFile->atEnd()) {
        bufferFromFile = currentFile->read(4096);

        for (int i = 0; i < bufferFromFile.size(); i += 8) {
            int len = qMin(bufferFromFile.size() - i, 8);
            std::uint64_t chunk = 0;
            for (int j = 0; j < len; ++j) {
                chunk |= static_cast<std::uint64_t>(static_cast<unsigned char>(bufferFromFile[i + j])) << (8 * (7-j));
            }
            chunk ^= data.xorKey;
            for (int j = 0; j < len; ++j) {
                bufferFromFile[i + j] = static_cast<char>((chunk >> (8 * (7-j))) & 0xFF);
            }
        }

        resultFile->write(bufferFromFile);

    }
    else {
        if(currentFile != nullptr){
            currentFile->close();
        }
        if(resultFile != nullptr){
            resultFile->close();
        }

        delete currentFile;
        delete resultFile;
        currentFile = nullptr;
        resultFile = nullptr;
    }
    sync.unlock();
    QMetaObject::invokeMethod(this, "doWorkStep", Qt::QueuedConnection);
};