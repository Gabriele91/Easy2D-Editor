#ifndef PROJECTUTILITY_H
#define PROJECTUTILITY_H

#include <QFile>
#include <QString>

class ProjectUtility
{
    public:

    static bool createProject(const QString &path,
                              const QString &name,
                              QString &outFileProject)
    {
        //save dir
        QString projectPath=path+"/"+name;
        //copy template
        if(copyRecursively("assets/template/",projectPath))
        {
            //save path
            outFileProject= projectPath+"/resources.rs.e2d";
            //done
            return true;
        }
        else
        {
            return false;
        }
    }

    static bool copyRecursively(const QString &srcFilePath,
                                const QString &tgtFilePath)
    {
        QFileInfo srcFileInfo(srcFilePath);
        if (srcFileInfo.isDir()) {
            QDir targetDir(tgtFilePath);
            targetDir.cdUp();
            /*
            if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            { // can't create directory
                return false;
            }
            */
            targetDir.mkdir(QFileInfo(tgtFilePath).fileName());
            targetDir=QDir(tgtFilePath); //update

            if(!targetDir.exists() ||
                targetDir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count())
            {
                return false;
            }
            QDir sourceDir(srcFilePath);
            QStringList fileNames = sourceDir.entryList(QDir::Files |
                                                        QDir::Dirs |
                                                        QDir::NoDotAndDotDot |
                                                        QDir::Hidden |
                                                        QDir::System);
            for (const QString &fileName : fileNames)
            {
                const QString newSrcFilePath
                = srcFilePath + QLatin1Char('/') + fileName;
                const QString newTgtFilePath
                = tgtFilePath + QLatin1Char('/') + fileName;
                if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                    return false;
            }
        }
        else
        {
            if (!QFile::copy(srcFilePath, tgtFilePath))
                return false;
        }
        return true;
    }

};

#endif
