#ifndef FUNC_H
#define FUNC_H

#include "func.hpp"
#include <QImage>
#include <QColor>
#include <math.h>
#include <iostream>
#include <opencv/cv.h>

QImage func_Gray(QImage Inimg);
QImage func_DeepGray(QImage Inimg);
int Qtsu(QImage Inimg);
int Average_Threshold(QImage InGray);
int Average_Threshold(cv::Mat InGray);
QImage func_Threshold(QImage Inframe,int gray);
cv::Mat func_Threshold(cv::Mat Inframe,int gray);

QImage  cvMatToQImage( const cv::Mat &inMat );
QImage MatToQImage(const cv::Mat& mat);
QImage Mat2QImage(cv::Mat frame);
QImage* IplImageToQImage(const IplImage *pIplImage);
void IplImageToQImage(const IplImage *pIplImage,QImage qimg);

#endif
