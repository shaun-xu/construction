#ifndef CALIBRATOR_H
#define CALIBRATOR_H

#include <QObject>

#include <opencv2/opencv.hpp>
#include "CalibrationData.h"

using namespace std;

class Calibrator : public QObject {
    Q_OBJECT
    public:
        Calibrator(unsigned int _screenCols, unsigned int _screenRows) : screenCols(_screenCols), screenRows(_screenRows), N(0){}
        virtual unsigned int getNPatterns(){return N;}
        virtual cv::Mat getCalibrationPattern(unsigned int depth){return patterns[depth];}
        virtual void addFrameSequence(std::vector<cv::Mat> frameSeq){frameSeqs.push_back(frameSeq);}
        virtual void reset(){frameSeqs.clear();}
        virtual CalibrationData calibrate() = 0;
        virtual ~Calibrator(){}
    signals:
        void newSequenceResult(cv::Mat img, unsigned int idx, bool success);
    protected:
        unsigned int screenCols, screenRows;
        unsigned int N;
        vector<cv::Mat> patterns;
        vector< vector<cv::Mat> > frameSeqs;
};

#endif // CALIBRATOR_H
