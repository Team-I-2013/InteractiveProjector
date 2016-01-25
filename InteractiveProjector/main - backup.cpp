#include <opencv2/core/core.hpp>        // core���W���[���̃w�b�_�[���C���N���[�h
#include <opencv2/highgui/highgui.hpp>  // highgui���W���[���̃w�b�_�[���C���N���[�h
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <errno.h>
#include <stdexcpt.h>
#include <windows.h>

using namespace std;
using namespace cv;

#define WIDTH 640
#define HEIGHT 480
#define RADIUS 4
#define POINT 4
#define max_frame 10

////LeftCam�p//
int LNearPoint(CvPoint Lmouse_pt);
void Lon_mouse(int event, int x, int y, int flags, void *param);

CvPoint Lpt[POINT];
int frame=0;//�t���[����
int key = NULL;
int Lpoint_flag = -1;
cv::Vec3d L = NULL;
///////////////

////RightCam�p//
int RNearPoint(CvPoint Rmouse_pt);
void Ron_mouse(int event, int x, int y, int flags, void *param);

CvPoint Rpt[POINT];
int Rframe=0;//�t���[����
int Rkey = NULL;
int Rpoint_flag = -1;
cv::Vec3d R = NULL;
///////////////

////LeftCam�֐�////
//�ߖT�_�擾�֐�
int LNearPoint(CvPoint Lmouse_pt)
{
    int nx, ny;
    float radius, min_radius = RADIUS;
    int ret = -1;
    
    for(int i=0; i<POINT; i++){
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
    
    for(int i=0; i<POINT; i++){
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


int main(int argc, const char* argv[])
{
	//setting�E�B���h�E�𐶐�
	//set_red.png���t���X�N���[���ŕ\���iwindows.h���g�p�j
	cv::namedWindow("setting", 0);
	cvSetWindowProperty("setting",CV_WND_PROP_FULLSCREEN,CV_WINDOW_FULLSCREEN);
	IplImage* img;
	char imgfile[] = "set_red.png";
	img = cvLoadImage(imgfile, CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	cvShowImage("setting", img);

	//�J�����Q�䕪�̃E�B���h�E�𐶐�
	cv::VideoCapture cap_left(0);
	cv::VideoCapture cap_right(1);
	cv::namedWindow("Left", CV_WINDOW_AUTOSIZE);
	HWND leftCamWindow = ::FindWindowA(NULL, "Left");
	MoveWindow(leftCamWindow, 0, 160, 640, 480, false);
	cv::namedWindow("Right", CV_WINDOW_AUTOSIZE);
	HWND rightCamWindow = ::FindWindowA(NULL, "Right");
	MoveWindow(rightCamWindow, 640, 160, 640, 480, false);
	
	cap_left.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
	cap_left.set(CV_CAP_PROP_FRAME_WIDTH,HEIGHT);

	cap_right.set(CV_CAP_PROP_FRAME_WIDTH,WIDTH);
	cap_right.set(CV_CAP_PROP_FRAME_WIDTH,HEIGHT);

	setMouseCallback("Left", Lon_mouse, NULL);
	setMouseCallback("Right", Ron_mouse, NULL);

	//�����ϊ��pMat
	Mat LframeImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);
	Mat LhsvImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);
	Mat LrecImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);

	Mat RframeImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);
	Mat RhsvImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);
	Mat RrecImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);

	while (1)
	{
		//���E�̃J��������f�����擾���E�B���h�E�ɕ\��
		cv::Mat cam1image;
		cv::Mat cam2image;
		try{
			cap_left >> cam1image; // Left(�����̃J�����̉摜�H)
			cv::imshow("Left",cam1image);
		} catch (...) {
			std::cout << "Left���F���ł��܂���B" << '\n';
		}
		try{
			cap_right >> cam2image; // �J�����Q(�E���̃J�����̉摜�H)
			cv::imshow("Right",cam2image);
		} catch (...) {
			std::cout << "Right���F���ł��܂���B" << '\n';
		}

		//�����ϊ��p
		Mat LdstImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);//�����ϊ��p
		Mat RdstImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);//�����ϊ��p

		cvtColor(LframeImage, LhsvImage, CV_BGR2HSV);
		cvtColor(RframeImage, RhsvImage, CV_BGR2HSV);

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

		imshow("Left", LframeImage);
		///////////////

		////RightCam////
		//RGB�ɕϊ�
		cvtColor(RhsvImage, RrecImage, CV_HSV2RGB);

		imshow("Right", RframeImage);
		////////////////

		

		int key = cv::waitKey(1);
		if(key == 'c') frame = 0;
		if (key == 0x20) break; // �X�y�[�X�L�[�ŏI��
	}


}

