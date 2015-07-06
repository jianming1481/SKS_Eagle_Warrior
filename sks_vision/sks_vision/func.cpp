#include "func.h"
#include <QDebug>
#include <string.h>

unsigned char histogram[256]={0};

char switch_autoThreshold=1;
QImage func_Gray(QImage Inimg){
    QImage temp(Inimg.width(),Inimg.height(),QImage::Format_RGB888);
    unsigned char gray;
    for(int h=0;h<Inimg.height();h++){
        for(int w=0;w<Inimg.width();w++){
            gray = (qRed(Inimg.pixel(w,h)) + qGreen(Inimg.pixel(w,h)) + qBlue(Inimg.pixel(w,h)))/3;
            temp.setPixel(w,h,QColor(gray,gray,gray).rgb());
        }
    }
    return temp;
}
void countNum0to255(QImage Inimg){
    unsigned char gray;
    for(int h=0;h<Inimg.height();h++){
        for(int w=0;w<Inimg.width();w++){
            gray = (qRed(Inimg.pixel(w,h)) + qGreen(Inimg.pixel(w,h)) + qBlue(Inimg.pixel(w,h)))/3;
            histogram[gray] = histogram[gray]+1;
        }
    }
}

//int Qtsu(unsigned char histogram[256]){
int Qtsu(QImage Inimg){
    if(switch_autoThreshold){
        std::cout<<"using Otsu's Thresholding"<<std::endl;
        switch_autoThreshold=0;
    }
    memset(histogram,0,sizeof(histogram)/sizeof(histogram[0]));
    countNum0to255(Inimg);

    float size = 640*480;
    // NOTE: Creation of histogram[256] not shown
    float  w = 0;                // first order cumulative
    float  u = 0;                // second order cumulative
    float  uT = 0;               // total mean level

    int    k = 255;              // maximum histogram index
    int    threshold = 0;        // optimal threshold value

    float  histNormalized[256];  // normalized histogram values

    float  work1, work2;		// working variables
    double work3 = 0.0;

    // Create normalised histogram values
    // (size=image width * image height)
    for (int I=1; I<=k; I++)
    histNormalized[I-1] = histogram[I-1]/(float)size;

    // Calculate total mean level
    for (int I=1; I<=k; I++)
    uT+=(I*histNormalized[I-1]);

    // Find optimal threshold value
    for (int I=1; I<k; I++) {
        w+=histNormalized[I-1];
        u+=(I*histNormalized[I-1]);
        work1 = (uT * w - u);
        work2 = (work1 * work1) / ( w * (1.0f-w) );
        if (work2>work3) work3=work2;
    }
    // Convert the final value to an integer
    threshold = (int)sqrt(work3);
    return threshold;
}

int Average_Threshold(QImage InGray){
    if(switch_autoThreshold){
        std::cout<<"using Average's Thresholding"<<std::endl;
        switch_autoThreshold=0;
    }

    int total=0;
    if(qRed(InGray.pixel(10,10))==qGreen(InGray.pixel(10,10)) && qGreen(InGray.pixel(10,10))==qBlue(InGray.pixel(10,10))){
        for(int h=0;h<InGray.height();h++){
            for(int w=0;w<InGray.width();w++){
                total+=qRed(InGray.pixel(w,h));
            }
        }
        return total/(InGray.height()*InGray.width());
    }else{
        std::cout<<"It's not Gray Image"<<std::endl;
        return 0;
    }
}
QImage  cvMatToQImage( const cv::Mat &inMat )
{
      switch ( inMat.type() )
      {
         // 8-bit, 4 channel
         case CV_8UC4:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

            return image;
         }

         // 8-bit, 3 channel
         case CV_8UC3:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         // 8-bit, 1 channel
         case CV_8UC1:
         {
            static QVector<QRgb>  sColorTable;

            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
               for ( int i = 0; i < 256; ++i )
                  sColorTable.push_back( qRgb( i, i, i ) );
            }

            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );

            return image;
         }

         default:
//            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
      }
      return QImage();
}
QImage MatToQImage(const cv::Mat& mat){
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
QImage* IplImageToQImage(const IplImage *pIplImage)
{
    QImage *qImage;

    int w = pIplImage->width;
    int h = pIplImage->height;

    qImage = new QImage(w, h, QImage::Format_RGB32);

    int x, y;
    for(x = 0; x < pIplImage->width; ++x)
    {
        for(y = 0; y < pIplImage->height; ++y)
        {
            CvScalar color = cvGet2D(pIplImage, y, x);

            int r = color.val[2];
            int g = color.val[1];
            int b = color.val[0];

            qImage->setPixel(x, y, qRgb(r,g,b));
        }
    }
    return qImage;
}

void IplImageToQImage(const IplImage *pIplImage,QImage qimg)
{
    QImage *qImage;

    int w = pIplImage->width;
    int h = pIplImage->height;

    qImage = new QImage(w, h, QImage::Format_RGB32);

    int x, y;
    for(x = 0; x < pIplImage->width; ++x)
    {
        for(y = 0; y < pIplImage->height; ++y)
        {
            CvScalar color = cvGet2D(pIplImage, y, x);

            int r = color.val[2];
            int g = color.val[1];
            int b = color.val[0];

            qImage->setPixel(x, y, qRgb(r,g,b));
        }
    }
    qimg = *qImage;
}
