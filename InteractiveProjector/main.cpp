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
#define DEFAULT_MODE CONTROL_MODE //デフォルトモード
#define CONTROL_MODE 0 //操作モード
#define DRAWING_MODE 1 //描画モード
int get_current_mode(void);//現在のモードを取得
int change_mode(void);//モードを切り替える
int current_mode = DEFAULT_MODE;
//

////LeftCam用//
int LNearPoint(CvPoint Lmouse_pt);
void Lon_mouse(int event, int x, int y, int flags, void *param);

CvPoint Lpt[4];
int frame=0;//フレーム数
int key = NULL;
int Lpoint_flag = -1;
Vec3d L = NULL;
///////////////


////RightCam用//
int RNearPoint(CvPoint Rmouse_pt);
void Ron_mouse(int event, int x, int y, int flags, void *param);

CvPoint Rpt[4];
int Rframe=0;//フレーム数
int Rkey = NULL;
int Rpoint_flag = -1;
Vec3d R = NULL;
///////////////


////Corsor用////////////
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
int mouseflag = 0;//マウス切り替え
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


//メイン
int main(int argc, const char* argv[])
{	
	//settingウィンドウを生成
	//set_red.pngをフルスクリーンで表示（windows.hを使用）
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
		Mat LdstImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);//透視変換用
		Mat RdstImage = Mat(Size(WIDTH,HEIGHT),CV_8UC3);//透視変換用
	
		////LeftCam////
			Lcapture >> LframeImage;			

			//HSVに変換
		    cvtColor(LframeImage, LhsvImage, CV_BGR2HSV);
			///////////////
		
			////RightCam////

			Rcapture >> RframeImage;

			//HSVに変換
	        cv::cvtColor(RframeImage, RhsvImage, CV_BGR2HSV);
			////////////////

		//一度画素値を取得したら次に移動する
		bool End0 = false;
		bool End1 = false;
		bool End2 = false;
		bool End3 = false;

		//max_frameの数だけループ
		if(frame<max_frame){
			////LeftCam////
			//左上
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
			//右上
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
			//右下
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
			//左下
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

			//初期化
			End0 = false;
			End1 = false;
			End2 = false;
			End3 = false;

			////RightCam////
			//左上
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
			//右上
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
			//右下
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
			//左下
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

		//頂点の描画
		for(int i=0;i<4;i++){
			circle(LframeImage, Lpt[i], RADIUS, CV_RGB(255, 147, 0), 2, CV_AA, 0);//Left
			circle(RframeImage, Rpt[i], RADIUS, CV_RGB(255, 147, 0), 2, CV_AA, 0);//Right
		}

		//枠の描画
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

		//最終座標
        if(frame<max_frame){ 
			frame++;
		}else { 
			printf("[LeftCamera座標]     [RightCamera座標]\n");
			printf("左上:(%4d,%4d)     左上:(%4d,%4d) \n",Lpt[0].x, Lpt[0].y, Rpt[0].x, Rpt[0].y);
			printf("右上:(%4d,%4d)     右上:(%4d,%4d) \n",Lpt[1].x, Lpt[1].y, Rpt[1].x, Rpt[1].y);
			printf("右下:(%4d,%4d)     右下:(%4d,%4d) \n",Lpt[2].x, Lpt[2].y, Rpt[2].x, Rpt[2].y);
			printf("左下:(%4d,%4d)     左下:(%4d,%4d) \n",Lpt[3].x, Lpt[3].y, Rpt[3].x, Rpt[3].y);

			printf("\n");
			printf("\n");

			//float型に変換
			float Lpt0x=Lpt[0].x, Lpt0y=Lpt[0].y;
			float Lpt1x=Lpt[1].x, Lpt1y=Lpt[1].y;
			float Lpt2x=Lpt[2].x, Lpt2y=Lpt[2].y;
			float Lpt3x=Lpt[3].x, Lpt3y=Lpt[3].y;

			float Rpt0x=Rpt[0].x, Rpt0y=Rpt[0].y;
			float Rpt1x=Rpt[1].x, Rpt1y=Rpt[1].y;
			float Rpt2x=Rpt[2].x, Rpt2y=Rpt[2].y;
			float Rpt3x=Rpt[3].x, Rpt3y=Rpt[3].y;

			Point2f Lpt[] = {Point2f(Lpt0x,Lpt0y),Point2f(Lpt1x,Lpt1y),Point2f(Lpt2x,Lpt2y),Point2f(Lpt3x,Lpt3y)};//変更前の頂点の位置
			Point2f Rpt[] = {Point2f(Rpt0x,Rpt0y),Point2f(Rpt1x,Rpt1y),Point2f(Rpt2x,Rpt2y),Point2f(Rpt3x,Rpt3y)};//変更前の頂点の位置
			
			Point2f pt[] = {Point2f(0,0),Point2f(WIDTH,0),Point2f(WIDTH,HEIGHT),Point2f(0,HEIGHT)};//変換後の座標
			
			Mat Lperspective_matrix = getPerspectiveTransform(Lpt, pt);
			Mat Rperspective_matrix = getPerspectiveTransform(Rpt, pt);

			warpPerspective(LframeImage, LdstImage, Lperspective_matrix, LframeImage.size(), INTER_LINEAR);//変換
			warpPerspective(RframeImage, RdstImage, Rperspective_matrix, RframeImage.size(), INTER_LINEAR);//変換


		}
		
		////LeftCam////
		//RGBに変換
		cvtColor(LhsvImage, LrecImage, CV_HSV2RGB);

		try{
			imshow("Left", LframeImage);
		} catch(...) {
			std::cout << "Leftが認識できません。" << '\n';
		}///////////////

		////RightCam////
		//RGBに変換
		cvtColor(RhsvImage, RrecImage, CV_HSV2RGB);

		try{
			imshow("Right", RframeImage);
		} catch(...) {
			std::cout << "Rightが認識できません。" << '\n';
		}
		////////////////

		/*imshow("LresultImage", LdstImage);
		imshow("RresultImage", RdstImage);*/

		//cキーが押されたら赤の四隅を再検出
		if(key == 'c'){
			frame = 0;
		}

		//qキーが押されたらキャリブレーションを終了
        if (key == 'q') break;

		key = cvWaitKey(10);

    }

	//keyにNULLを代入し初期化
	key = NULL;

	//全ウィンドウの破棄
	destroyAllWindows();

	//モードボタンの描画
	namedWindow("mode_button", CV_WINDOW_AUTOSIZE);
	HWND windowHandle = ::FindWindowA(NULL, "mode_button");
	// ウィンドウスタイル変更（メニューバーなし、最前面）
	SetWindowLongPtr(windowHandle,  GWL_STYLE, WS_POPUP);
	SetWindowPos(windowHandle, HWND_TOPMOST, 50, 80, 50, 50, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	IplImage* mode_img = cvLoadImage("control.png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	cvShowImage("mode_button", mode_img);
	
	//スクリーンショットボタンの描画
	namedWindow("func_screenshot", CV_WINDOW_AUTOSIZE);
	windowHandle = ::FindWindowA(NULL, "func_screenshot");
	SetWindowLongPtr(windowHandle,  GWL_STYLE, WS_POPUP);
	SetWindowPos(windowHandle, HWND_TOPMOST, 50, 150, 50, 50, SWP_DRAWFRAME | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	IplImage* ss_img = cvLoadImage("screenshot.png", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
	cvShowImage("func_screenshot", ss_img);



	//ウィンドウを２つ生成
	//透視変換用
	namedWindow("LresultImage",0);
	cvResizeWindow("LresultImage",SCREENWIDTH, SCREENHEIGHT);
	moveWindow("LresultImage",0,400);
	namedWindow("RresultImage",0);
	cvResizeWindow("RresultImage",SCREENWIDTH, SCREENHEIGHT);
	moveWindow("RresultImage",620,400);

	//変換後の映像をウィンドウに表示
	while(1){
		Lcapture >> LframeImage;
		Rcapture >> RframeImage;

		Mat LdstImage = Mat(Size(SCREENWIDTH,SCREENHEIGHT),CV_8UC3);//透視変換用
		Mat RdstImage = Mat(Size(SCREENWIDTH,SCREENHEIGHT),CV_8UC3);//透視変換用

		//float型に変換
		float Lpt0x=Lpt[0].x, Lpt0y=Lpt[0].y;
		float Lpt1x=Lpt[1].x, Lpt1y=Lpt[1].y;
		float Lpt2x=Lpt[2].x, Lpt2y=Lpt[2].y;
		float Lpt3x=Lpt[3].x, Lpt3y=Lpt[3].y;

		float Rpt0x=Rpt[0].x, Rpt0y=Rpt[0].y;
		float Rpt1x=Rpt[1].x, Rpt1y=Rpt[1].y;
		float Rpt2x=Rpt[2].x, Rpt2y=Rpt[2].y;
		float Rpt3x=Rpt[3].x, Rpt3y=Rpt[3].y;

		Point2f Lpt[] = {Point2f(Lpt0x,Lpt0y),Point2f(Lpt1x,Lpt1y),Point2f(Lpt2x,Lpt2y),Point2f(Lpt3x,Lpt3y)};//変更前の頂点の位置
		Point2f Rpt[] = {Point2f(Rpt0x,Rpt0y),Point2f(Rpt1x,Rpt1y),Point2f(Rpt2x,Rpt2y),Point2f(Rpt3x,Rpt3y)};//変更前の頂点の位置
			
		Point2f pt[] = {Point2f(0,0),Point2f(WIDTH,0),Point2f(WIDTH,HEIGHT),Point2f(0,HEIGHT)};//変換後の座標
			
		Mat Lperspective_matrix = getPerspectiveTransform(Lpt, pt);
		Mat Rperspective_matrix = getPerspectiveTransform(Rpt, pt);

		warpPerspective(LframeImage, LdstImage, Lperspective_matrix, LframeImage.size(), INTER_LINEAR);//変換
		warpPerspective(RframeImage, RdstImage, Rperspective_matrix, RframeImage.size(), INTER_LINEAR);//変換


		///////////Cursor////////////////////////////////
		//setMouseCallback("LresultImage", my_mouse_callback, (void *)&LdstImage);
		//setMouseCallback("RresultImage", my_mouse_callback, (void *)&RdstImage);

		//矩形描画
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


		//L赤色のエリアを探す
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
		//赤色の重心を計算
		Lcx=(int)(Lm.m10/Lm.m00);
		Lcy=(int)(Lm.m01/Lm.m00);

		if(Lcx <= 0) Lcx = NULL;
		if(Lcy <= 0) Lcy = NULL;

		LC.x = Lcx;//LresultImageの座標
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

		//R赤色のエリアを探す
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
		//赤色の重心を計算
		Rcx=(int)(Rm.m10/Rm.m00);
		Rcy=(int)(Rm.m01/Rm.m00);

		if(Rcx <= 0) Rcx = NULL;
		if(Rcy <= 0) Rcy = NULL;

		RC.x = Rcx;//RresultImageの座標
		RC.y = Rcy;

		if(mouseflag==1){//aを押したとき
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

			//GetCursorPos(&LSP);//マウスの座標取得

			SetCursorPos(LCP.x, LCP.y);//マウスの座標に赤の重心の座標を代入

			//GetCursorPos(&RSP);
			//SetCursorPos(RCP.x, RCP.y);
			
			printf("赤色(LresultImage): x:%3d y:%3d\n",LCP.x, LCP.y);
			printf("赤色(RresultImage): x:%3d y:%3d\n",RCP.x, RCP.y);
			if((LCP.x == 0 && RCP.x == 0) || (LCP.y == 0 && RCP.y == 0)){
				printf("触れていません\n");
			} else if(abs(LCP.x - RCP.x) <= 50 &&  abs(LCP.y - RCP.y) <= 50){
				printf("触れています\n");
			} else {
				printf("触れていません\n");
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
				printf("Lresult平均値; %d, %d\n",Lave.x/10, Lave.y/10);
				printf("Rresult平均値; %d, %d\n",Rave.x/10, Rave.y/10);
			}
			*/
		}else{
			GetCursorPos(&LSP);//マウスの座標取得
			printf("マウス座標      : x:%4d y:%4d\n",LSP.x, LSP.y);
			printf("LresultImage座標: x:%3d y:%3d\n",LC.x, LC.y);
			printf("RresultImage座標: x:%3d y:%3d\n",RC.x, RC.y);
			printf("\n");

		}

		///透視変換画像ではなく、それに対応するウィンドウからスクリーンショットを撮る
		if(Corsorflag == 1){//左ボタンを離したとき
			sprintf_s(str,sizeof(str),"img%d.jpg",t);
			Mat cap_img(LdstImage, box);
			imwrite(str, cap_img);
			printf("スクリーンショットを保存しました.\n");
			printf("名前: img%d.jpg\n",t);
			t++;
			Corsorflag = -1;
		}

		circle(LdstImage, LC, RADIUS, CV_RGB(255, 170, 0), 2, CV_AA, 0);
		circle(RdstImage, RC, RADIUS, CV_RGB(255, 170, 0), 2, CV_AA, 0);

		//////////////////////////////////////////////////

		imshow("LresultImage", LdstImage);
		imshow("RresultImage", RdstImage);

		if (key == 'a') mouseflag = 1;//マウス切り替え
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

////LeftCam関数////
//近傍点取得関数
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
//マウスイベント取得関数
void Lon_mouse(int event, int x, int y, int flags, void *param)

{
    CvPoint Lpush_pt = cvPoint(x,y);

    //左クリック時に一番近い点を取得
	if(event == CV_EVENT_LBUTTONDOWN){
        Lpoint_flag = LNearPoint(Lpush_pt);
	}

	//左クリック時に移動で頂点移動
    if((event == CV_EVENT_MOUSEMOVE)&&(flags == CV_EVENT_FLAG_LBUTTON)){
		Lpt[Lpoint_flag] = cvPoint(x, y);
    }
}
///////////////////


////Right関数////
//近傍点取得関数
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
//マウスイベント取得関数
void Ron_mouse(int event, int x, int y, int flags, void *param)

{
    CvPoint Rpush_pt = cvPoint(x,y);

    //左クリック時に一番近い点を取得
	if(event == CV_EVENT_LBUTTONDOWN){
        Rpoint_flag = RNearPoint(Rpush_pt);
	}

	//左クリック時に移動で頂点移動
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

//現在のモードを返す
int get_current_mode(void){
	return current_mode;
}

//モードを切り替える（操作モード・描画モード）
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