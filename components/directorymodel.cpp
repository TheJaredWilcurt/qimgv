#include "directorymodel.h"

DirectoryModel::DirectoryModel(QObject *parent) : QObject(parent) {
    thumbnailer = new Thumbnailer(&dirManager); // remove pointers
    scaler = new Scaler(&cache);

    connect(&dirManager, SIGNAL(fileRemoved(QString, int)), this, SIGNAL(fileRemoved(QString, int)));
    connect(&dirManager, SIGNAL(fileAdded(QString)), this, SIGNAL(fileAdded(QString)));
    connect(&dirManager, SIGNAL(fileModified(QString)), this, SIGNAL(fileModified(QString)));
    connect(&dirManager, SIGNAL(fileRenamed(QString, QString)), this, SIGNAL(fileRenamed(QString, QString)));
    // ditch this and make dirModel single directory only (passed in constructor?)
    connect(&dirManager, SIGNAL(directoryChanged(QString)), this, SIGNAL(directoryChanged(QString)));
}

DirectoryModel::~DirectoryModel() {
    delete scaler;
    delete thumbnailer;
}

int DirectoryModel::itemCount() const {
    return dirManager.fileCount();
}

int DirectoryModel::indexOf(QString fileName) {
    return dirManager.indexOf(fileName);
}

QString DirectoryModel::fileNameAt(int index) {
    return dirManager.fileNameAt(index);
}

QString DirectoryModel::fullFilePath(QString fileName) {
    return dirManager.fullFilePath(fileName);
}

bool DirectoryModel::contains(QString fileName) {
    return dirManager.contains(fileName);
}

bool DirectoryModel::removeFile(QString fileName, bool trash) {
    return dirManager.removeFile(fileName, trash);
}

bool DirectoryModel::isEmpty() {
    return dirManager.isEmpty();
}

QString DirectoryModel::nextOf(QString fileName) {
    return dirManager.nextOf(fileName);
}

QString DirectoryModel::prevOf(QString fileName) {
    return dirManager.prevOf(fileName);
}

QString DirectoryModel::first() {
    return dirManager.first();
}

QString DirectoryModel::last() {
    return dirManager.last();
}

QString DirectoryModel::absolutePath() {
    return dirManager.absolutePath();
}

QDateTime DirectoryModel::lastModified(QString fileName) {
    return dirManager.lastModified(fileName);
}

bool DirectoryModel::copyTo(QString destDirectory, QString fileName) {
    return dirManager.copyTo(destDirectory, fileName);
}

bool DirectoryModel::moveTo(QString destDirectory, QString fileName) {
    if(!dirManager.contains(fileName)) {
        qDebug() << "[DirectoryModel] Error: source file does not exist.";
        return false;
    }
    QFileInfo file(destDirectory + "/" + fileName);
    if(file.exists()) {
        qDebug() << "[DirectoryModel] Error: destination file already exists.";
        //mw->showMessage("Error: destination file already exists.");
        return false;
    }
    if(dirManager.copyTo(destDirectory, fileName)) {
        // remove original file
        if(!dirManager.removeFile(fileName, false)) {
            // revert on failure
            dirManager.removeFile(destDirectory + "/" + fileName, false);
            return false;
        }
        //mw->showMessage("File moved to: " + destDirectory);
        return true;
    } else {
        //mw->showMessage("Error: could not write a file to " + destDirectory);
        qDebug() << "Error: could not write a file to " << destDirectory;
        return false;
    }
}

void DirectoryModel::setDirectory(QString path) {
    dirManager.setDirectory(path);
}