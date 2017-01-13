#ifndef RP
#define RP

#include "opencv2/text.hpp"

using namespace std;
using namespace cv;
using namespace cv::text;
//code récupérer d'un exemple d'utilisation
class RecoProcess
{
private:

public:
bool   isRepetitive(const string& s);
void   er_draw(vector<Mat> &channels, vector<vector<ERStat> > &regions, vector<Vec2i> group, Mat& segmentation);
int process(int argc, char* argv[]);
};

#endif
