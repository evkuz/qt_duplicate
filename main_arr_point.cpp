#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDataStream>
#include <stdio.h>
#include <iostream>
#include <QByteArray>
#include <QVector>


//#define def_file "/home/evkuz/asm/matrix_mul/rnd_data.bin"
#define def_file "rnd_data.bin"

void File_Open(QString filename);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString fname;
    QFile mfile;
    QByteArray ba;
    quint64 i, j, fsize;
    quint32 fdata, sample, num;
    //quint32  mybuffer[400];
    //QVector<quint32> mybuffer;
    quint32 * mybuffer;
    quint64 rest_of_file;


    bool duplicate;
    duplicate = false;


    fname = def_file;
    qDebug() << "Start checking file consistency ";
    mfile.setFileName(fname);
    fsize = mfile.size();
    mybuffer = new quint32[fsize];
    if (!mfile.open(QIODevice::ReadOnly )) { qDebug() <<  "The file " <<  mfile.fileName() << " does not exist !";  return 0;}
    qDebug() << "Current file name is " << mfile.fileName() << "and size of " << fsize << " bytes";


    j =0; num=0;

    while (!mfile.atEnd()) //#1
    {
     // Когда смещение переходит на последний байт, то он сравнивается сам с собой.
     // В таком случае вообще не нужно ничего делать.
      if (!mfile.seek(j)) {qDebug() <<  "Seek operation ERROR in string, " << "J value is " << j;break;}
      // read sample value to cbuf
        mfile.read(reinterpret_cast<char*>(&sample), 4);
        if (mfile.atEnd()) {break;} // Вот ключевое условие !!!
        //offset сдвинулся, а значение j - не изменилось
        j+=4; // go next dword position, read rest of file
        if (!mfile.seek(j)){qDebug() <<  "Seek operation ERROR in string, " << "J value is " << j;break;}
        //read rest of file
        rest_of_file = fsize - j;
        mfile.read(reinterpret_cast<char*>(mybuffer), rest_of_file); //rest_of_file
        //Вот после этого у нас offset сдвигается в конец файла. И у нас уже случилось mfile.atEnd() == true
        i=0;
         while (i<=rest_of_file/4){ //#2
                fdata = *(mybuffer+i);
            // fdata = mybuffer.at(i);
             // we have quint32 array, so increment index by 1, not by 4
             if ( *(mybuffer+i)==sample) {
                 ba = QByteArray(reinterpret_cast<char*>(&fdata), 4);
                 qDebug() <<  "Found duplicate value   " << ba.toHex().toUpper();// << "next offset value is " << (i+1)*4; duplicate = true; break;
                 qDebug() <<  "1st occurrence offset is " << j-4  << "next occurrence offset is " << j+ i*4; duplicate = true; //break;
                 num++;
             }
             i++;
             if (j >= (fsize - 3)) {break;}

         }//while  (i<=rest_of_file) #2

      // Те же грабли. Мы же когда считываем, у нас offset сдвигается в конец файла.
      // Снова переходим в точку перебора
      if (!mfile.seek(j)) qDebug() <<  "Seek operation ERROR in string, " << "J value is " << j;

    }//while  (!mfile.atEnd()) #1
mfile.close();
if (duplicate) {qDebug() <<  "There were" << num << "duplicate dword values in a file";}
else {qDebug() <<  "There was no ANY duplicate dwords in a file";}
return 0;
//    return a.exec();
}
