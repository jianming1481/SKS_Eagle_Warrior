#ifndef FUNC_H
#define FUNC_H

#include "func.h"
#include <QImage>
#include <QColor>
#include <math.h>
#include <iostream>
#include <opencv/cv.h>

QImage func_Gray(QImage Inimg);
int Qtsu(unsigned char histogram[256]);
int Average_Threshold(QImage InGray);
QImage  cvMatToQImage( const cv::Mat &inMat );
QImage MatToQImage(const cv::Mat& mat);
QImage* IplImageToQImage(const IplImage *pIplImage);
void IplImageToQImage(const IplImage *pIplImage,QImage qimg);

#endif
