#include "RecognitionSystem.h"

//using namespace cv;


void RecognitionSystem::searchForMovement( Mat &image, Mat &cameraFeed )
{
	bool objectDetected = false;
	int xpos, ypos;  
	cv::Mat tmp;
	std::vector< std::vector< cv::Point > > contours;
	std::vector<cv::Vec4i> hierarchy;

	image.copyTo(tmp);
	//trouver les contours de l'objet
	findContours(tmp, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	//évaluer le résultat
	if( contours.size() > 0 )objectDetected = true;
	else objectDetected = false;
	if( objectDetected )
	{
		//récup du plus gros contour(se trouvant dans la dernière case du vector)
		std::vector< std::vector< cv::Point > > largestContourVec;
		largestContourVec.push_back(contours.at( contours.size() - 1 ) );

		//trouver la position de l'objet (centre du contour)
		rectanglebound = boundingRect( largestContourVec.at(0) );
		xpos = rectanglebound.x + rectanglebound.width / 2;
		ypos = rectanglebound.y + rectanglebound.height / 2;
		
		//recup les valeurs dans le tableau
		object[0] = xpos;
		object[1] = ypos;
	}

	circle( cameraFeed, Point(xpos,ypos), 20, Scalar( 0,255,0 ), 2 );
	line( cameraFeed, Point( xpos,ypos ), Point( xpos,ypos - 25 ), Scalar( 0,255,0 ), 2 );
	line( cameraFeed, Point( xpos,ypos ), Point( xpos,ypos + 25 ), Scalar( 0,255,0 ), 2 );
	line( cameraFeed, Point( xpos,ypos ), Point( xpos - 25,ypos ), Scalar( 0,255,0 ), 2 );
	line( cameraFeed, Point( xpos,ypos ), Point( xpos + 25,ypos ), Scalar( 0,255,0 ), 2 );

}

/*void RecognitionSystem::catchForCam(  )
{
	VideoCapture vidCap(0);// capture de la webcam
	if( vidCap.isOpened() )
	{
		std::cerr << "erreur cam\n";
		return;
	}
	int iLowH = 0, iHighH = 179, 
		iLowS = 0, iHighS = 255, 
		iLowV = 0, iHighV = 255;

	cvCreateTrackbar("LowH", "Control",&iLowH, 179);
	cvCreateTrackbar("iHighH", "Control",&iHighH, 179);

	cvCreateTrackbar("iLowS", "Control",&iLowS, 255);
	cvCreateTrackbar("iHighS", "Control",&iHighS, 255);

	cvCreateTrackbar("iLowV", "Control",&iLowV, 255);
	cvCreateTrackbar("iHighV", "Control",&iHighV, 255);

	do
	{
		Mat imgOriginal, imgHSV, imgThreesholded;
		bool granted = vidCap.read(imgOriginal);
		if(!granted)
		{
			std::cerr << "erreur lecture\n";
			return;
		}
		cvtColor( imgOriginal ,imgHSV, COLOR_BGR2HSV );
		inRange( imgHSV, Scalar(iLowH,iLowS,iLowV), Scalar(iHighH,iHighS,iHighV),imgThreesholded );//recup de la portion d'image
		
		erode( imgThreesholded, imgThreesholded, getStructuringElement( MORPH_ELLIPSE, Size(5,5) ) );
		dilate( imgThreesholded, imgThreesholded, getStructuringElement( MORPH_ELLIPSE, Size(5,5) ) );

		dilate( imgThreesholded, imgThreesholded, getStructuringElement( MORPH_ELLIPSE, Size(5,5) ) );
		erode( imgThreesholded, imgThreesholded, getStructuringElement( MORPH_ELLIPSE, Size(5,5) ) );
		searchForMovement(imgThreesholded, imgOriginal);

		imshow("image", imgThreesholded);


		if( waitKey(30) == 27 )
		{
			std::cout << "out\n";
			break;
		}
	}while(true);
}*/

void RecognitionSystem::recognizeFromPic( const std::string &pathImg )
{
	const cv::Mat input = cv::imread(pathImg, 0);// load as grayscale
    cv::Mat descriptors;
    Ptr<DescriptorExtractor> extractor = cv::ORB::create();

    /*cv::xfeatures2d::SiftFeatureDetector detector;
    std::vector<cv::KeyPoint> keypoints;
    detector.detect(input, keypoints);
    */

std::vector<KeyPoint> keypoints;
Ptr<FeatureDetector> detector = ORB::create();
detector->detect(input, keypoints);

    extractor->compute(input, keypoints, descriptors);
    std::cout << descriptors.rows << ":" << descriptors.cols << std::endl;
    // too many bits
	std::cout << descriptors << std::endl;
}

/*void RecognitionSystem::analysePicture( std::string &image )
{
	cvNamedWindows( "fenetre", 1 );
	cvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY);
	CascadeClassifier detector;
	detector.load( image );
	if(detector.empty())
	{
		std::cerr<<"erreur chargement image\n";
		return;
	}
	char key;
	do
	{
		Mat frame = cvQueryFrame(capture);
		std::vector<Rect> targets;
		detector.detectMultiScale(frame, targets, 1.1, 30, 0 | CV_HARR_SCALE_IMAGE, Size(200,320));
		for(unsigned int i;targets.size();++i)
		{
			Point pt1( targets[i].x, targets[i].y ), pt2( targets[i].x + targets[i].width, targets[i].y + targets[i].height );
			
			rectangle( frame, pt1, pt2, Scalar(0,0,255), 2 );
			putText(frame, "objet",pt1, FONT_HERSHEY_PLAIN, 1.0, Scalar(0,0,255), 2.0);
		}

		imshow("dd", frame);
		if( waitKey(30) == 27 )
		{
			std::cout << "out\n";
			break;
		}
	}while(true);
}*/


/*bool RecognitionSystem::loadImage( string pathToImage )
{
				image = imread( pathToImage, CV_LOAD_IMAGE_COLOR );
				return image.data;
}

void RecognitionSystem::displayImage()
{
				namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
				imshow( "Display window", image );
				waitKey(0); //wait infinite time for a keypress

				destroyWindow("Display window"); //destroy the window with the name, "Display window"
}

bool RecognitionSystem::captureVideo( string pathToVideo )
{
				captureVideo = make_unique< VideoCapture >( pathToVideo );

				if ( !captureVideo.isOpened() )return false;

}

void RecognitionSystem::recognizeColor()
{

				int iLowH = 0;
				int iHighH = 179;

				int iLowS = 0; 
				int iHighS = 255;

				int iLowV = 0;
				int iHighV = 255;

				//Create trackbars in "Control" window
				cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
				cvCreateTrackbar("HighH", "Control", &iHighH, 179);

				cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
				cvCreateTrackbar("HighS", "Control", &iHighS, 255);

				cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
				cvCreateTrackbar("HighV", "Control", &iHighV, 255);

				while (true)
				{
								Mat imgOriginal;

								bool bSuccess = captureVideo.read(imgOriginal); // read a new frame from video

								if (!bSuccess) //if not success, break loop
								{
												cout << "Cannot read a frame from video stream" << endl;
												break;
								}

								Mat imgHSV;

								cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

								Mat imgThresholded;

								inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

								//morphological opening (remove small objects from the foreground)
								erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
								cv::dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 

								//morphological closing (fill small holes in the foreground)
								dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) ); 
								erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

								imshow("Thresholded Image", imgThresholded); //show the thresholded image
								imshow("Original", imgOriginal); //show the original image

								if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
								{
												cout << "esc key is pressed by user" << endl;
												break; 
								}
				}
}*/
