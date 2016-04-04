#include <opencv2/core/core.hpp>        
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "windows.h"

using namespace std;
using namespace cv;

#define WIDTH 640
#define HEIGHT 480
#define SCREENWIDTH 1280
#define SCREENHEIGHT 800
#define RADIUS 4
#define max_frame 20

//
#define DEFAULT_MODE CONTROL_MODE //�f�t�H���g���[�h
#define CONTROL_MODE 0 //���샂�[�h
#define DRAWING_MODE 1 //�`�惂�[�h
int get_current_mode(void);//���݂̃��[�h���擾
int change_mode(void);//���[�h��؂�ւ���
int current_mode = DEFAULT_MODE;
//

////LeftCam�p//
int LNearPoint(CvPoint Lmouse_pt);
void Lon_mouse(int event, int x, int y, int flags, void *param);

CvPoint Lpt[4];
int frame=0;//�t���[����
int key = NULL;
int Lpoint_flag = -1;
Vec3d L = NULL;
///////////////


////RightCam�p//
int RNearPoint(CvPoint Rmouse_pt);
void Ron_mouse(int event, int x, int y, int flags, void *param);

CvPoint Rpt[4];
int Rframe=0;//�t���[����
int Rkey = NULL;
int Rpoint_flag = -1;
Vec3d R = NULL;
///////////////


////Corsor�p////////////
void my_mouse_callback(int event, int x, int y, int flags, void* param);
void draw_box(Mat* img, Rect rect);
BOOL SetCursorPos(CvPoint X, CvPoint Y);

Rect box;
RECT Lrect;
RECT Rrect;
RECT Drect;
bool drawing_box = false;
char str[32];
int t=0;
int mouseflag = 0;//�}�E�X�؂�ւ�
int Corsorflag = -1;
int Eventmouseflag = 0;
POINT LSP;
POINT LCP;
POINT P;

POINT RSP;
POINT RCP;
POINT RP;

int maxk = 0;

int Lcx = 0;
int Lcy = 0;
CvPoint LC;

int Rcx = 0;
int Rcy = 0;
CvPoint RC;

int f = 0;
CvPoint Lave;
CvPoint Rave;

//////////////////////////


//���C��
int main(int argc, const char* argv[])
{	
	//setting�E�B���h�E�𐶐�
	//set_red.png���t���X�N���[���ŕ\���iwindows.h���g�p�j
	cv::namedWindow("setting", 0);
	cvSetWindowProperty("setting",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);
	IplImage* img;
	char imgfile[] = "set_blue.png";
	img = cvLoadImage(imgfile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	cvShowImage("setting", img);
	cvMoveWindow("setting",1280,0);

	////LeftCam////
	VideoCapture Lcapture(0);
	//if(!Lcapture.isOpened()){
	//	return -1;
	//}

	Lcapture.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
	Lcapture.set(CV_CAP_PROP_FRAME_WIDTH,HEIGHT);

	namedWindow("Left", CV_WINDOW_AUTOSIZE);
	moveWindow("Left",0, 100);

	Mat LframeImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);
	Mat LhsvImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);
	Mat LrecImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);

	setMouseCallback("Left", Lon_mouse, NULL);
	///////////////

	////RightCam////
	VideoCapture Rcapture(1);
	//if(!Rcapture.isOpened()){
	//  return -1;
	//}

	Rcapture.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
	Rcapture.set(CV_CAP_PROP_FRAME_WIDTH,HEIGHT);
	
	namedWindow("Right", CV_WINDOW_AUTOSIZE);
	moveWindow("Right",620, 100);

	Mat RframeImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);
	Mat RhsvImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);
	Mat RrecImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);

	setMouseCallback("Right", Ron_mouse, NULL);
	////////////////

	/////Cursor////////////
	double Ltmparea;
	double Lmaxarea;

	double Rtmparea;
	double Rmaxarea;

	box = Rect(-1, -1, 0, 0);
	//////////////////////

	while (1) {
		Mat LdstImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);//�����ϊ��p
		Mat RdstImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);//�����ϊ��p
	
		////LeftCam////
			Lcapture >> LframeImage;			

			//HSV�ɕϊ�
		    cvtColor(LframeImage, LhsvImage, CV_BGR2HSV);
			///////////////
		
			////RightCam////

			Rcapture >> RframeImage;

			//HSV�ɕϊ�
	        cv::cvtColor(RframeImage, RhsvImage, CV_BGR2HSV);
			////////////////

		//��x��f�l���擾�����玟�Ɉړ�����
		bool End0 = false;
		bool End1 = false;
		bool End2 = false;
		bool End3 = false;

		//max_frame�̐��������[�v
		if(frame<max_frame){
			////LeftCam////
			//����
			for (Lpt[0].y=0; Lpt[0].y<HEIGHT/2; Lpt[0].y++) {
				for (Lpt[0].x=0; Lpt[0].x<WIDTH/2; Lpt[0].x++) {
					L = LhsvImage.at<Vec3b>(Lpt[0].y, Lpt[0].x);
					if ((L.val[0]>95 && L.val[0]<130) && L.val[1]>120 && L.val[2]>70) {
						L = LhsvImage.at<Vec3b>(Lpt[0].y, Lpt[0].x);
						End1 = true;
					}
					if (End1) break;
				}
				if(End1) break;
			}
			//�E��
			for (Lpt[1].y=0; Lpt[1].y<HEIGHT/2; Lpt[1].y++) {
				for (Lpt[1].x=WIDTH/2; Lpt[1].x<WIDTH; Lpt[1].x++) {
					L = LhsvImage.at<Vec3b>(Lpt[1].y, Lpt[1].x);
					if ((L.val[0]>95 && L.val[0]<130) && L.val[1]>120 && L.val[2]>70) {
						L = LhsvImage.at<Vec3b>(Lpt[1].y, Lpt[1].x);
						End1 = true;
					}
					if (End1) break;
				}
				if(End1) break;
			}
			//�E��
			for (Lpt[2].y=HEIGHT/2; Lpt[2].y<HEIGHT; Lpt[2].y++) {
				for (Lpt[2].x=WIDTH/2; Lpt[2].x<WIDTH; Lpt[2].x++) {
					L = LhsvImage.at<Vec3b>(Lpt[2].y, Lpt[2].x);
					if ((L.val[0]>95 && L.val[0]<130) && L.val[1]>120 && L.val[2]>70) {
						L = LhsvImage.at<Vec3b>(Lpt[2].y, Lpt[2].x);
						End2 = true;
					}
					if (End2) break;
				}
				if(End2) break;
			}
			//����
			for (Lpt[3].y=HEIGHT/2; Lpt[3].y<HEIGHT; Lpt[3].y++) {
				for (Lpt[3].x=0; Lpt[3].x<WIDTH/2; Lpt[3].x++) {
					L = LhsvImage.at<Vec3b>(Lpt[3].y, Lpt[3].x);
					if ((L.val[0]>95 && L.val[0]<130) && L.val[1]>120 && L.val[2]>70) {
						L = LhsvImage.at<Vec3b>(Lpt[3].y, Lpt[3].x);
						End3 = true;
					}
					if (End3) break;
				}
				if(End3) break;
			}
			///////////////

			//������
			End0 = false;
			End1 = false;
			End2 = false;
			End3 = false;

			////RightCam////
			//����
			for (Rpt[0].y=0; Rpt[0].y<HEIGHT/2; Rpt[0].y++) {
				for (Rpt[0].x=0; Rpt[0].x<WIDTH/2; Rpt[0].x++) {
					R = RhsvImage.at<Vec3b>(Rpt[0].y, Rpt[0].x);
					if ((R.val[0]>95 && R.val[0]<130) && R.val[1]>120 && R.val[2]>70) {
						R = RhsvImage.at<Vec3b>(Rpt[0].y, Rpt[0].x);
						End1 = true;
					}
					if (End1) break;
				}
				if(End1) break;
			}
			//�E��
			for (Rpt[1].y=0; Rpt[1].y<HEIGHT/2; Rpt[1].y++) {
				for (Rpt[1].x=WIDTH/2; Rpt[1].x<WIDTH; Rpt[1].x++) {
					R = RhsvImage.at<Vec3b>(Rpt[1].y, Rpt[1].x);
					if ((R.val[0]>95 && R.val[0]<130) && R.val[1]>120 && R.val[2]>70) {
						R = RhsvImage.at<Vec3b>(Rpt[1].y, Rpt[1].x);
						End1 = true;
					}
					if (End1) break;
				}
				if(End1) break;
			}
			//�E��
			for (Rpt[2].y=HEIGHT/2; Rpt[2].y<HEIGHT; Rpt[2].y++) {
				for (Rpt[2].x=WIDTH/2; Rpt[2].x<WIDTH; Rpt[2].x++) {
					R = RhsvImage.at<Vec3b>(Rpt[2].y, Rpt[2].x);
					if ((R.val[0]>95 && R.val[0]<130) && R.val[1]>120 && R.val[2]>70) {
						R = RhsvImage.at<Vec3b>(Rpt[2].y, Rpt[2].x);
						End2 = true;
					}
					if (End2) break;
				}
				if(End2) break;
			}
			//����
			for (Rpt[3].y=HEIGHT/2; Rpt[3].y<HEIGHT; Rpt[3].y++) {
				for (Rpt[3].x=0; Rpt[3].x<WIDTH/2; Rpt[3].x++) {
					R = RhsvImage.at<Vec3b>(Rpt[3].y, Rpt[3].x);
					if ((R.val[0]>95 && R.val[0]<130) && R.val[1]>120 && R.val[2]>70) {
						R = RhsvImage.at<Vec3b>(Rpt[3].y, Rpt[3].x);
						End3 = true;
					}
					if (End3) break;
				}
				if(End3) break;
			}
			////////////////
		}

		//���_�̕`��
		for(int i=0;i<4;i++){
			circle(LframeImage, Lpt[i], RADIUS, CV_RGB(255, 147, 0), 2, CV_AA, 0);//Left
			circle(RframeImage, Rpt[i], RADIUS, CV_RGB(255, 147, 0), 2, CV_AA, 0);//Right
		}

		//�g�̕`��
		////LeftCam////
		line(LframeImage, Lpt[0], Lpt[1], Scalar(0,0,150), 1, CV_AA);
		line(LframeImage, Lpt[1], Lpt[2], Scalar(0,0,150), 1, CV_AA);
		line(LframeImage, Lpt[2], Lpt[3], Scalar(0,0,150), 1, CV_AA);
		line(LframeImage, Lpt[3], Lpt[0], Scalar(0,0,150), 1, CV_AA);
		///////////////

		////RightCam////
		line(RframeImage, Rpt[0], Rpt[1], Scalar(0,0,150), 1, CV_AA);
		line(RframeImage, Rpt[1], Rpt[2], Scalar(0,0,150), 1, CV_AA);
		line(RframeImage, Rpt[2], Rpt[3], Scalar(0,0,150), 1, CV_AA);
		line(RframeImage, Rpt[3], Rpt[0], Scalar(0,0,150), 1, CV_AA);
		////////////////

		//�ŏI���W
        if(frame<max_frame){ 
			frame++;
		}else { 
			printf("[LeftCamera���W]     [RightCamera���W]\n");
			printf("����:(%4d,%4d)     ����:(%4d,%4d) \n",Lpt[0].x, Lpt[0].y, Rpt[0].x, Rpt[0].y);
			printf("�E��:(%4d,%4d)     �E��:(%4d,%4d) \n",Lpt[1].x, Lpt[1].y, Rpt[1].x, Rpt[1].y);
			printf("�E��:(%4d,%4d)     �E��:(%4d,%4d) \n",Lpt[2].x, Lpt[2].y, Rpt[2].x, Rpt[2].y);
			printf("����:(%4d,%4d)     ����:(%4d,%4d) \n",Lpt[3].x, Lpt[3].y, Rpt[3].x, Rpt[3].y);

			printf("\n");
			printf("\n");

			//float�^�ɕϊ�
			float Lpt0x=Lpt[0].x, Lpt0y=Lpt[0].y;
			float Lpt1x=Lpt[1].x, Lpt1y=Lpt[1].y;
			float Lpt2x=Lpt[2].x, Lpt2y=Lpt[2].y;
			float Lpt3x=Lpt[3].x, Lpt3y=Lpt[3].y;

			float Rpt0x=Rpt[0].x, Rpt0y=Rpt[0].y;
			float Rpt1x=Rpt[1].x, Rpt1y=Rpt[1].y;
			float Rpt2x=Rpt[2].x, Rpt2y=Rpt[2].y;
			float Rpt3x=Rpt[3].x, Rpt3y=Rpt[3].y;

			Point2f Lpt[] = {Point2f(Lpt0x,Lpt0y),Point2f(Lpt1x,Lpt1y),Point2f(Lpt2x,Lpt2y),Point2f(Lpt3x,Lpt3y)};//�ύX�O�̒��_�̈ʒu
			Point2f Rpt[] = {Point2f(Rpt0x,Rpt0y),Point2f(Rpt1x,Rpt1y),Point2f(Rpt2x,Rpt2y),Point2f(Rpt3x,Rpt3y)};//�ύX�O�̒��_�̈ʒu
			
			Point2f pt[] = {Point2f(0,0),Point2f(WIDTH,0),Point2f(WIDTH,HEIGHT),Point2f(0,HEIGHT)};//�ϊ���̍��W
			
			Mat Lperspective_matrix = getPerspectiveTransform(Lpt, pt);
			Mat Rperspective_matrix = getPerspectiveTransform(Rpt, pt);

			warpPerspective(LframeImage, LdstImage, Lperspective_matrix, LframeImage.size(), INTER_LINEAR);//�ϊ�
			warpPerspective(RframeImage, RdstImage, Rperspective_matrix, RframeImage.size(), INTER_LINEAR);//�ϊ�


		}
		
		////LeftCam////
		//RGB�ɕϊ�
		cvtColor(LhsvImage, LrecImage, CV_HSV2RGB);

		try{
			imshow("Left", LframeImage);
		} catch(...) {
			std::cout << "Left���F���ł��܂���B" << '\n';
		}///////////////

		////RightCam////
		//RGB�ɕϊ�
		cvtColor(RhsvImage, RrecImage, CV_HSV2RGB);

		try{
			imshow("Right", RframeImage);
		} catch(...) {
			std::cout << "Right���F���ł��܂���B" << '\n';
		}
		////////////////

		/*imshow("LresultImage", LdstImage);
		imshow("RresultImage", RdstImage);*/

		//c�L�[�������ꂽ��Ԃ̎l�����Č��o
		if(key == 'c'){
			frame = 0;
		}

		//q�L�[�������ꂽ��L�����u���[�V�������I��
        if (key == 'q') break;

		key = cvWaitKey(10);

    }

	//key��NULL������������
	key = NULL;

	//�S�E�B���h�E�̔j��
	destroyAllWindows();

	//���[�h�{�^���̕`��
	namedWindow("mode_button", CV_WINDOW_AUTOSIZE);
	HWND windowHandle = ::FindWindowA(NULL, "mode_button");
	// �E�B���h�E�X�^�C���ύX�i���j���[�o�[�Ȃ��A�őO�ʁj
	SetWindowLongPtr(windowHandle,  GWL_STYLE, WS_POPUP);
	SetWindowPos(windowHandle, HWND_TOPMOST, 50, 80, 50, 50, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	IplImage* mode_img = cvLoadImage("control.png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	cvShowImage("mode_button", mode_img);
	
	//�X�N���[���V���b�g�{�^���̕`��
	namedWindow("func_screenshot", CV_WINDOW_AUTOSIZE);
	windowHandle = ::FindWindowA(NULL, "func_screenshot");
	SetWindowLongPtr(windowHandle,  GWL_STYLE, WS_POPUP);
	SetWindowPos(windowHandle, HWND_TOPMOST, 50, 150, 50, 50, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	IplImage* ss_img = cvLoadImage("screenshot.png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	cvShowImage("func_screenshot", ss_img);



	//�E�B���h�E���Q����
	//�����ϊ��p
	namedWindow("LresultImage",0);
	cvResizeWindow("LresultImage",SCREENWIDTH, SCREENHEIGHT);
	moveWindow("LresultImage",0,400);
	namedWindow("RresultImage",0);
	cvResizeWindow("RresultImage",SCREENWIDTH, SCREENHEIGHT);
	moveWindow("RresultImage",620,400);

	//�ϊ���̉f�����E�B���h�E�ɕ\��
	while(1){
		Lcapture >> LframeImage;
		Rcapture >> RframeImage;

		Mat LdstImage = Mat(Size(SCREENWIDTH,SCREENHEIGHT),CV_8UC3);//�����ϊ��p
		Mat RdstImage = Mat(Size(SCREENWIDTH,SCREENHEIGHT),CV_8UC3);//�����ϊ��p

		//float�^�ɕϊ�
		float Lpt0x=Lpt[0].x, Lpt0y=Lpt[0].y;
		float Lpt1x=Lpt[1].x, Lpt1y=Lpt[1].y;
		float Lpt2x=Lpt[2].x, Lpt2y=Lpt[2].y;
		float Lpt3x=Lpt[3].x, Lpt3y=Lpt[3].y;

		float Rpt0x=Rpt[0].x, Rpt0y=Rpt[0].y;
		float Rpt1x=Rpt[1].x, Rpt1y=Rpt[1].y;
		float Rpt2x=Rpt[2].x, Rpt2y=Rpt[2].y;
		float Rpt3x=Rpt[3].x, Rpt3y=Rpt[3].y;

		Point2f Lpt[] = {Point2f(Lpt0x,Lpt0y),Point2f(Lpt1x,Lpt1y),Point2f(Lpt2x,Lpt2y),Point2f(Lpt3x,Lpt3y)};//�ύX�O�̒��_�̈ʒu
		Point2f Rpt[] = {Point2f(Rpt0x,Rpt0y),Point2f(Rpt1x,Rpt1y),Point2f(Rpt2x,Rpt2y),Point2f(Rpt3x,Rpt3y)};//�ύX�O�̒��_�̈ʒu
			
		Point2f pt[] = {Point2f(0,0),Point2f(WIDTH,0),Point2f(WIDTH,HEIGHT),Point2f(0,HEIGHT)};//�ϊ���̍��W
			
		Mat Lperspective_matrix = getPerspectiveTransform(Lpt, pt);
		Mat Rperspective_matrix = getPerspectiveTransform(Rpt, pt);

		warpPerspective(LframeImage, LdstImage, Lperspective_matrix, LframeImage.size(), INTER_LINEAR);//�ϊ�
		warpPerspective(RframeImage, RdstImage, Rperspective_matrix, RframeImage.size(), INTER_LINEAR);//�ϊ�


		///////////Cursor////////////////////////////////
		//setMouseCallback("LresultImage", my_mouse_callback, (void *)&LdstImage);
		//setMouseCallback("RresultImage", my_mouse_callback, (void *)&RdstImage);

		//��`�`��
		if (drawing_box) {
			draw_box(&LdstImage, box);
			draw_box(&RdstImage, box);
        }

		Mat Llab_img, Limg, Limg_Result, Lcopy_Result, Lr_img, Lg_img, Lb_img;
		cvtColor(LdstImage, Llab_img, CV_BGR2Lab);
		vector<Mat> Lplanes;
		split(LdstImage, Lplanes);
		threshold(Lplanes[2], Lr_img, 100, 255, THRESH_BINARY);
		Limg = 10 * Lplanes[2]/Lplanes[1];
		threshold(Limg, Lg_img,15, 255, THRESH_BINARY);
		Limg = 10 * Lplanes[2]/Lplanes[0];
		threshold(Limg, Lb_img, 15, 255, THRESH_BINARY);
		Limg = Lg_img & Lb_img; 
		Limg_Result = Limg & Lr_img;
		Lcopy_Result = Limg_Result.clone();
		vector <vector<Point>> Lcontours;
  		vector <Vec4i> Lhierarchy;


		//L�ԐF�̃G���A��T��
		if(!Lcontours.empty()){
			Lmaxarea = contourArea(Mat(Lcontours[0]));

			findContours(Lcopy_Result, Lcontours, Lhierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

			for ( size_t k = 1; k < Lcontours.size(); k++) {
				if ((Ltmparea = contourArea(Mat(Lcontours[k]))) > Lmaxarea) {
					maxk = k; 
					Lmaxarea = Ltmparea;
				}
			}
		}

		Moments Lm = moments(Limg_Result,true);
		//�ԐF�̏d�S���v�Z
		Lcx=(int)(Lm.m10/Lm.m00);
		Lcy=(int)(Lm.m01/Lm.m00);

		if(Lcx <= 0) Lcx = NULL;
		if(Lcy <= 0) Lcy = NULL;

		LC.x = Lcx;//LresultImage�̍��W
		LC.y = Lcy;

		Mat Rlab_img, Rimg, Rimg_Result, Rcopy_Result, Rr_img, Rg_img, Rb_img;
		cvtColor(RdstImage, Rlab_img, CV_BGR2Lab);
		vector<Mat> Rplanes;
		split(RdstImage, Rplanes);
		threshold(Rplanes[2], Rr_img, 100, 255, THRESH_BINARY);
		Rimg = 10 * Rplanes[2]/Rplanes[1];
		threshold(Rimg, Rg_img,15, 255, THRESH_BINARY);
		Rimg = 10 * Rplanes[2]/Rplanes[0];
		threshold(Rimg, Rb_img, 15, 255, THRESH_BINARY);
		Rimg = Rg_img & Rb_img; 
		Rimg_Result = Rimg & Rr_img;
		Rcopy_Result = Rimg_Result.clone();
		vector <vector<Point>> Rcontours;
  		vector <Vec4i> Rhierarchy;

		//R�ԐF�̃G���A��T��
		if(!Rcontours.empty()){
			Rmaxarea = contourArea(Mat(Rcontours[0]));

			findContours(Rcopy_Result, Rcontours, Rhierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

			for ( size_t k = 1; k < Rcontours.size(); k++) {
				if ((Rtmparea = contourArea(Mat(Rcontours[k]))) > Rmaxarea) {
					maxk = k; 
					Rmaxarea = Rtmparea;
				}
			}
		}

		Moments Rm = moments(Rimg_Result,true);
		//�ԐF�̏d�S���v�Z
		Rcx=(int)(Rm.m10/Rm.m00);
		Rcy=(int)(Rm.m01/Rm.m00);

		if(Rcx <= 0) Rcx = NULL;
		if(Rcy <= 0) Rcy = NULL;

		RC.x = Rcx;//RresultImage�̍��W
		RC.y = Rcy;

		if(mouseflag==1){//a���������Ƃ�
			setMouseCallback("LresultImage", my_mouse_callback, (void *)&LdstImage);
			setMouseCallback("RresultImage", my_mouse_callback, (void *)&RdstImage);

			HWND Lhwnd = (HWND)cvGetWindowHandle("LresultImage");
			HWND hDesk = GetDesktopWindow();
			HWND Rhwnd = (HWND)cvGetWindowHandle("RresultImage");

			GetWindowRect(Lhwnd, &Lrect);
			GetWindowRect(hDesk, &Drect);
			GetWindowRect(Rhwnd, &Rrect);

			P.x = Drect.left + Lrect.left;
			P.y = Drect.top + Lrect.top;

			RP.x = Drect.left + Rrect.left;
			RP.y = Drect.top + Rrect.top;

			ScreenToClient(hDesk, &LSP);

			LCP.x = LC.x;
			LCP.y = LC.y;

			RCP.x = RC.x;
			RCP.y = RC.y;
			
			//

			RCP.x *= 2;
			LCP.x *= 2;
			RCP.y *= (800/480);
			LCP.y *= (800/480);

			//GetCursorPos(&LSP);//�}�E�X�̍��W�擾

			SetCursorPos(LCP.x, LCP.y);//�}�E�X�̍��W�ɐԂ̏d�S�̍��W����

			//GetCursorPos(&RSP);
			//SetCursorPos(RCP.x, RCP.y);
			
			printf("�ԐF(LresultImage): x:%3d y:%3d\n",LCP.x, LCP.y);
			printf("�ԐF(RresultImage): x:%3d y:%3d\n",RCP.x, RCP.y);
			if((LCP.x == 0 && RCP.x == 0) || (LCP.y == 0 && RCP.y == 0)){
				printf("�G��Ă��܂���\n");
			} else if(abs(LCP.x - RCP.x) <= 50 &&  abs(LCP.y - RCP.y) <= 50){
				printf("�G��Ă��܂�\n");
			} else {
				printf("�G��Ă��܂���\n");
			}
			printf("\n");
		   /*
			if(f < 10){
				Lave.x += LCP.x;
				Lave.y += LCP.y;

				Rave.x += RCP.x;
				Rave.y += RCP.x;
		
				f++;
			} else {
				printf("Lresult���ϒl; %d, %d\n",Lave.x/10, Lave.y/10);
				printf("Rresult���ϒl; %d, %d\n",Rave.x/10, Rave.y/10);
			}
			*/
		}else{
			GetCursorPos(&LSP);//�}�E�X�̍��W�擾
			printf("�}�E�X���W      : x:%4d y:%4d\n",LSP.x, LSP.y);
			printf("LresultImage���W: x:%3d y:%3d\n",LC.x, LC.y);
			printf("RresultImage���W: x:%3d y:%3d\n",RC.x, RC.y);
			printf("\n");

		}

		///�����ϊ��摜�ł͂Ȃ��A����ɑΉ�����E�B���h�E����X�N���[���V���b�g���B��
		if(Corsorflag == 1){//���{�^���𗣂����Ƃ�
			sprintf_s(str,sizeof(str),"img%d.jpg",t);
			Mat cap_img(LdstImage, box);
			imwrite(str, cap_img);
			printf("�X�N���[���V���b�g��ۑ����܂���.\n");
			printf("���O: img%d.jpg\n",t);
			t++;
			Corsorflag = -1;
		}

		circle(LdstImage, LC, RADIUS, CV_RGB(255, 170, 0), 2, CV_AA, 0);
		circle(RdstImage, RC, RADIUS, CV_RGB(255, 170, 0), 2, CV_AA, 0);

		//////////////////////////////////////////////////

		imshow("LresultImage", LdstImage);
		imshow("RresultImage", RdstImage);

		if (key == 'a') mouseflag = 1;//�}�E�X�؂�ւ�
		if (key == 'b') mouseflag = 0;
		if(key == 'w'){
			change_mode();
			if(get_current_mode() == CONTROL_MODE){
				mode_img = cvLoadImage("control.png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
			} else if (get_current_mode() == DRAWING_MODE){
				mode_img = cvLoadImage("drawing.png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
			}
			cvShowImage("mode_button", mode_img);	
		}
		if (key == 'q') break;
		key = cvWaitKey(10);

		
	}
	return 0;
}

////LeftCam�֐�////
//�ߖT�_�擾�֐�
int LNearPoint(CvPoint Lmouse_pt)
{
    int nx, ny;
    float radius, min_radius = RADIUS;
    int ret = -1;
    
    for(int i=0; i<4; i++){
        nx = Lmouse_pt.x - Lpt[i].x;
        ny = Lmouse_pt.y - Lpt[i].y;
        radius = sqrt((float)(nx*nx + ny*ny));
        if(radius < min_radius){
            min_radius = radius;
            ret = i;
        }
    }
    return ret;
}
//�}�E�X�C�x���g�擾�֐�
void Lon_mouse(int event, int x, int y, int flags, void *param)

{
    CvPoint Lpush_pt = cvPoint(x,y);

    //���N���b�N���Ɉ�ԋ߂��_���擾
	if(event == CV_EVENT_LBUTTONDOWN){
        Lpoint_flag = LNearPoint(Lpush_pt);
	}

	//���N���b�N���Ɉړ��Œ��_�ړ�
    if((event == CV_EVENT_MOUSEMOVE)&&(flags == CV_EVENT_FLAG_LBUTTON)){
		Lpt[Lpoint_flag] = cvPoint(x, y);
    }
}
///////////////////


////Right�֐�////
//�ߖT�_�擾�֐�
int RNearPoint(CvPoint Rmouse_pt)
{
    int nx, ny;
    float radius, min_radius = RADIUS;
    int ret = -1;
    
    for(int i=0; i<4; i++){
        nx = Rmouse_pt.x - Rpt[i].x;
        ny = Rmouse_pt.y - Rpt[i].y;
        radius = sqrt((float)(nx*nx + ny*ny));
        if(radius < min_radius){
            min_radius = radius;
            ret = i;
        }
    }
    return ret;
}
//�}�E�X�C�x���g�擾�֐�
void Ron_mouse(int event, int x, int y, int flags, void *param)

{
    CvPoint Rpush_pt = cvPoint(x,y);

    //���N���b�N���Ɉ�ԋ߂��_���擾
	if(event == CV_EVENT_LBUTTONDOWN){
        Rpoint_flag = RNearPoint(Rpush_pt);
	}

	//���N���b�N���Ɉړ��Œ��_�ړ�
    if((event == CV_EVENT_MOUSEMOVE)&&(flags == CV_EVENT_FLAG_LBUTTON)){
		Rpt[Rpoint_flag] = cvPoint(x, y);
    }
}
////////////////

////Corsor////////////////
void my_mouse_callback(int event, int x, int y, int flags, void* param){
    Mat* image = static_cast<Mat*>(param);

    switch (event){
    case CV_EVENT_MOUSEMOVE:
        if (drawing_box){
            box.width = x - box.x;
            box.height = y - box.y;
        }
        break;
	case CV_EVENT_LBUTTONDOWN:
				drawing_box = true;
				box = Rect(x, y, 0, 0);
				Eventmouseflag = 1;
				break;
    case CV_EVENT_LBUTTONUP:
        drawing_box = false;
        if (box.width < 0){
            box.x += box.width;
            box.width *= -1;
        }
        if (box.height < 0){
            box.y += box.height;
            box.height *= -1;
        }
		Corsorflag = 1;
        draw_box(image, box);
        break;
	
	}
	
}

void draw_box(Mat* img, Rect rect){
    rectangle(*img, Point2d(box.x, box.y), Point2d(box.x + box.width, box.y + box.height),
        Scalar(0xff, 0x00, 0x00));
}

//���݂̃��[�h��Ԃ�
int get_current_mode(void){
	return current_mode;
}

//���[�h��؂�ւ���i���샂�[�h�E�`�惂�[�h�j
int change_mode(void){
	IplImage* mode_img;
	if(get_current_mode() == CONTROL_MODE){
		current_mode = DRAWING_MODE;
	} else if(get_current_mode() == DRAWING_MODE){
		current_mode = CONTROL_MODE;
	}
	return current_mode;
}

///////////////////////////