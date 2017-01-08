#ifndef POCR
#define POCR

#include "opencv2/text.hpp"

using namespace std;
using namespace cv;
using namespace cv::text;

//OCR recognition is done in parallel for different detections
template <class T>
class Parallel_OCR: public cv::ParallelLoopBody
{
private:
    vector<Mat> &detections;
    vector<string> &outputs;
    vector< vector<Rect> > &boxes;
    vector< vector<string> > &words;
    vector< vector<float> > &confidences;
    vector< Ptr<T> > &ocrs;

public:
    Parallel_OCR(vector<Mat> &_detections, vector<string> &_outputs, vector< vector<Rect> > &_boxes,
                 vector< vector<string> > &_words, vector< vector<float> > &_confidences,
                 vector< Ptr<T> > &_ocrs);
        /*: detections(_detections), outputs(_outputs), boxes(_boxes), words(_words),
          confidences(_confidences), ocrs(_ocrs)
    {}*/

    virtual void operator()( const cv::Range &r ) const;
    /*{
        for (int c=r.start; c < r.end; c++)
        {
            ocrs[c%ocrs.size()]->run(detections[c], outputs[c], &boxes[c], &words[c], &confidences[c], OCR_LEVEL_WORD);
        }
    }*/
    Parallel_OCR & operator=(const Parallel_OCR &a);
};

#endif
