#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <opencv2/features2d.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <opencv2/core/core.hpp>

using namespace cv;

class RecognitionSystem
{
private:
	int object[2] = {0,0};
	Mat image;
	Mat cam;
	Rect rectanglebound = Rect(0,0,0,0);
	std::unique_ptr< VideoCapture > captureVideo; 


public:
	RecognitionSystem() = default;
	void searchForMovement( Mat &image, Mat &cameraFeed  );
	void catchForCam();
	void analysePicture( std::string &image );
	void recognizeFromPic( const std::string &pathImg );
	/*bool loadImage( std::string pathToImage );
	void displayImage();
	bool captureVideo( std::string pathToVideo );
	void recognizeColor();*/
};


