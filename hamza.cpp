#include <sstream>
#include <string>
#include <iostream>
//#include <opencv\highgui.h>
#include <opencv2/opencv.hpp>
//#include <opencv\cv.h>

using namespace cv;
using namespace std;

//initialiser le min et le max du filtre de HSV.
//pour faire du test on peut les éliminer.
int H_MIN = 65, H_MAX = 115;
int S_MIN = 79, S_MAX = 142;
int V_MIN = 89, V_MAX = 130;

//changer la résolution par default 
const int FRAME_WIDTH = 360;
const int FRAME_HEIGHT = 280;

//le nombre maximum des objets a detecter
const int MAX_NUM_OBJECTS=1;

//la taille minimum et maximum de l'object 
//la taille maximum de l'objet ne doit pas depasser 3/2 la resolution totale de la capture(frame)
const int MIN_OBJECT_TAILL = 10*10;
const int MAX_OBJECT_TAILL = FRAME_HEIGHT*FRAME_WIDTH/1.5;

//le trackbar est utiliser initialement pour modifier la couleur de l'objet a suivre
const string trackbarWindowName = "Trackbars";
void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed


}

string intToString(int number)
{


	std::stringstream ss;
	ss << number;
	return ss.str();
}

void createTrackbars()
{
	//create window for trackbars


    namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	/*
	sprintf( TrackbarName, "H_MIN", H_MIN);
	sprintf( TrackbarName, "H_MAX", H_MAX);
	sprintf( TrackbarName, "S_MIN", S_MIN);
	sprintf( TrackbarName, "S_MAX", S_MAX);
	sprintf( TrackbarName, "V_MIN", V_MIN);
	sprintf( TrackbarName, "V_MAX", V_MAX);
	*/
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
    createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
    createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
    createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
    createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
    createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
    createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );


}

void drawObject(int x, int y,Mat &frame)
{

	//use some of the openCV drawing functions to draw crosshairs
	//on your tracked image!

    //UPDATE:JUNE 18TH, 2013
    //added 'if' and 'else' statements to prevent
    //memory errors from writing off the screen (ie. (-25,-25) is not within the window!)

	circle(frame,Point(x,y),20,Scalar(0,255,0),2);
    if(y-25>0)
    line(frame,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,0),Scalar(0,255,0),2);
    if(y+25<FRAME_HEIGHT)
    line(frame,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(x,FRAME_HEIGHT),Scalar(0,255,0),2);
    if(x-25>0)
    line(frame,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(0,y),Scalar(0,255,0),2);
    if(x+25<FRAME_WIDTH)
    line(frame,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);
    else line(frame,Point(x,y),Point(FRAME_WIDTH,y),Scalar(0,255,0),2);

	putText(frame,intToString(x)+","+intToString(y),Point(x,y+30),1,1,Scalar(0,255,0),2);

}

void morphOps(Mat &thresh)
{

	//"dilatee" et "eroder" l'image.
	//the element chosen here is a 3px by 3px rectangle

	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(2,2));
    //dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(7,7));

	//erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);


	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
	


}

void trackFilteredObject(int &x, int &y, Mat threshold, Mat &frame)
{

	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
        //if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
        if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
                if(area>MIN_OBJECT_TAILL && area<MAX_OBJECT_TAILL && area>refArea){
					x = moment.m10/area;
					y = moment.m01/area;
					objectFound = true;
					refArea = area;
				}else objectFound = false;


			}
			//let user know you found an object
			if(objectFound ==true){
				putText(frame,"Tracking Object",Point(0,50),2,1,Scalar(0,255,0),2);
				//draw object location on screen
				drawObject(x,y,frame);}

		}else putText(frame,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}

int main(int argc, char* argv[])
{
	//On a declarer des variables booliens pour donner une flexibilitie. 
	//au programme pour faire des choses differents.
    bool trackObjects = false;
    bool useMorphOps = true;
	
	//La matrice qui tienne la valeur du frame capture (l'image originale).
	Mat frame;
	
	//La matrice de l'image HSV.
	Mat HSV;
	
	//La matrice de l'image binaire.
	Mat threshold;
	
	//x et y sont les valeurs du position de l'objet.
	int x=0, y=0;
	
	//creier le trackbar pour changer les parametre u filtrage HSV.
	createTrackbars();
	
	//declarer la variable de capture.
	VideoCapture capture;
	
	//on peut trouver les webcams disponible dans ("/dev/video*").
	capture.open(0);
	
	//donner la resolution du capture.
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	
	//la boucle du programme.
	while(1){
		//lire l'image dans la matrice frame.
		capture.read(frame);
		
		//convertir frame depuis BGR jusqu'a HSV.
		cvtColor(frame,HSV,COLOR_BGR2HSV);
		
		//les pixeles entre le vecteur minimale(H_MIN,S_MIN,V_MIN)
		//et le vecteurmaximale(H_MAX,S_MAX,V_MAX)
		//sont stocker par des 1 dans une nouvelle matrice de zeros appeller threshold.
		inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
		
		//utiliser les operations morphologique dans l'image threshold pour eliminer le bruit.
		//et donner une forme intereptable pour l'objet.
		if(useMorphOps)morphOps(threshold);

		//La fonction "trackFilteredObject" va retourner la position de
		//l'objet a suivre par ces position dans les variable x et y.
		if(trackObjects)trackFilteredObject(x,y,threshold,frame);

		//afficher les images
		imshow("Image Thresholded",threshold);
		imshow("Image Original",frame);
		imshow("Image HSV",HSV);
		

		//un retard de 30ms pour que l'affichage soit possible.
		//l'appuie sur n'import quelle touche va terminer le programme.
		if(waitKey(30)>0)break;
	}

	return 0;
}
