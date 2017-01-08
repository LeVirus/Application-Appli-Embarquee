#ifndef RP
#define RP

#include "opencv2/text.hpp"

using namespace std;
using namespace cv;
using namespace cv::text;

class RecoProcess
{
private:

public:
  //Discard wrongly recognised strings
bool   isRepetitive(const string& s);
//Draw ER's in an image via floodFill
void   er_draw(vector<Mat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, Mat& segmentation);
int process(int argc, char* argv[]);
};

#endif
