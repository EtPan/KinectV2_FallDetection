#include <iostream>
#include "pKinect.h"

using  namespace std;
using  namespace   cv;
using  namespace Sample;
using  namespace DirectX;

static DWORD  framenumber = 0; 
static DWORD  colornumber = 0;
static DWORD  depthnumber = 0;			//数据帧编号
static DWORD  tempnumber  = 0;

int vDetection,HeightDetection;
float  SpineHeightin, SpineHeightout, SpineV;

// Constructor
CBodyBasics::CBodyBasics() :
	mySensor(NULL),
	myColorReader(NULL),
	myDepthReader(NULL),
	myBodyReader(NULL),
	myMapper(NULL)
{}

// Destructor
CBodyBasics::~CBodyBasics()
{
	// done with frame reader
	SafeRelease(myColorReader);
	SafeRelease(myDepthReader);
	SafeRelease(myBodyReader);
	SafeRelease(myInfraredReader);

	// done with coordinate mapper
	SafeRelease(myMapper);

	// close the Kinect Sensor
	if (mySensor)
	{
		mySensor->Close();
	}
	SafeRelease(mySensor);
}

//初始化
HRESULT CBodyBasics::InitKinect()
{
	HRESULT hr;
	hr = GetDefaultKinectSensor(&mySensor);
	if (mySensor) 
	{
		mySensor->Open();
		IFrameDescription	* myDescription = nullptr;

		IColorFrameSource	* myColorSource = nullptr;
		int COLORHEIGHT = 0, COLORWIDTH = 0;
		mySensor->get_ColorFrameSource(&myColorSource);
		myColorSource->get_FrameDescription(&myDescription);
		myDescription->get_Height(&COLORHEIGHT);
		myDescription->get_Width(&COLORWIDTH);
		myColorSource->OpenReader(&myColorReader);			//以上为Color帧的准备，直接开好Reader

		IDepthFrameSource	* myDepthSource = nullptr;
		int DEPTHHEIGHT = 0, DEPTHWIDTH = 0;
		mySensor->get_DepthFrameSource(&myDepthSource);
		myDepthSource->get_FrameDescription(&myDescription);
		myDescription->get_Height(&DEPTHHEIGHT);
		myDescription->get_Width(&DEPTHWIDTH);
		myDepthSource->OpenReader(&myDepthReader);			//以上为Depth帧的准备，直接开好Reader

		IBodyFrameSource	* myBodySource = nullptr;
		mySensor->get_BodyFrameSource(&myBodySource);
		myBodySource->OpenReader(&myBodyReader);			//以上为Body帧的准备，直接开好Reader
		
		IInfraredFrameSource* myInfraredSource = nullptr;
		mySensor->get_InfraredFrameSource(&myInfraredSource);
		myInfraredSource->OpenReader(&myInfraredReader);

		SafeRelease(myColorSource);
		SafeRelease(myDepthSource);
		SafeRelease(myBodySource);
		SafeRelease(myInfraredSource);

		mySensor->get_CoordinateMapper(&myMapper);
	}
	if (!mySensor || FAILED(hr))
	{
		cout << "Kinect initialization failed!" << endl;
		return E_FAIL;
	}
	return hr;
}

//保存骨骼图
void CBodyBasics::SaveBodyImg()
{
	stringstream stream0, stream1;
	string str, filepath, str1;
	filepath = "E:\\temp\\";			//设置固定保存路径

	if (filepath.empty())
		filepath = "E:\\temp\\";			//设置默认固定保存路径
	else
		filepath += "/";

	//从选择文件夹里面获取当前选择的路径
	stream1 << framenumber;        //从long型数据输入
	stream1 >> str;					//转换为 string
	str1 = "成功保存第 " + str + " 帧骨骼图\r\n";

	if (BodyImg.data){
		imwrite(filepath + str + "body.bmp", BodyImg);
		cout << str1;
		cout << str + "depth.bmp" << endl;}
	else{
		cout << "没有数据，保存失败" << endl;}
}

//保存深度图
void CBodyBasics::SaveDepthImg()
{
	stringstream stream0, stream1;
	string str, filepath, str1;
	filepath = "E:\\temp\\";			//设置固定保存路径

	if (filepath.empty())
		filepath = "E:\\temp\\";			//设置默认固定保存路径
	else
		filepath += "/";

	stream1 << depthnumber;        //从long型数据输入
	stream1 >> str;					//转换为 string
	str1 = "成功保存第 " + str + " 帧深度图\r\n";

	if (DepthImg.data)
	{
		imwrite(filepath + str + "depth.bmp", DepthImg);
		cout << str1;
		cout << str + "depth.bmp" << endl;
	}
	else
	{
		cout << "没有数据，保存失败" << endl;
	}
	/*if (InfraredImg.data)
	{
		imwrite(filepath + str + "Infrared.bmp", InfraredImg);
		cout << str1;
		cout << str + "Infrared.bmp" << endl;
	}
	else
	{
		cout << "没有数据，保存失败" << endl;
	}*/
}

//保存彩色图
void CBodyBasics::SaveColorImg()
{
	stringstream stream0, stream1;
	string str, filepath, str1;
	//CMFC_DEMO01Dlg *pDlg0 = CMFC_DEMO01Dlg::s_pDlg;
	filepath = "E:\\temp\\";			//设置固定保存路径
									//从选择文件夹里面获取当前选择的路径
									//filepath = (pDlg0->m_edit).GetBuffer(0);
	if (filepath.empty())
		filepath = "E:\\temp\\";			//设置默认固定保存路径
	else
		filepath += "/";

	//从选择文件夹里面获取当前选择的路径
	stream1 << colornumber;        //从long型数据输入
	stream1 >> str;					//转换为 string
	str1 = "成功保存第 " + str + " 帧彩色图\r\n";
	
	if (ColorImg.data){
		imwrite(filepath + str + "color.bmp", ColorImg);
		cout << str1;
		cout << str + "color.bmp" << endl;}
	else{
		cout << "没有数据，保存失败" << endl;}
}

//语音询问
void CBodyBasics::SpeechDetection()
{

}

//发送邮件报警
void CBodyBasics::SendEmail()
{
	CSmtp smtp(
		25,								/*smtp端口*/
		"smtp.163.com",					/*smtp服务器地址*/
		"*******@163.com",	/*你的邮箱地址*/
		"**********",					/*邮箱密码*/
		"******@***.com",	/*目的邮箱地址*/
		"WARNING",							/*主题*/
		"家中有人跌倒！！！速来救助"		/*邮件正文*/
		);

	//添加附件时注意,\一定要写成\\，因为转义字符的缘故
	//string filePath("E:\\pic\\318body.bmp");
	//smtp.AddAttachment(filePath);

	//string filePath("E:\\pic\\318color.bmp");
	//smtp.AddAttachment(filePath);

	int err;
	if ((err = smtp.SendEmail_Ex()) != 0)
	{
		if (err == 1)
			cout << "错误1: 由于网络不畅通，发送失败!" << endl;
		if (err == 2)
			cout << "错误2: 用户名错误,请核对!" << endl;
		if (err == 3)
			cout << "错误3: 用户密码错误，请核对!" << endl;
		if (err == 4)
			cout << "错误4: 请检查附件目录是否正确，以及文件是否存在!" << endl;
	}
	//system("pause");
}

//获取系统时间
string CBodyBasics::getTime()
{
	time_t timep;
	time (&timep);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
	return tmp;
}

void CBodyBasics::curl_test()
{
	curl_global_init(CURL_GLOBAL_ALL); // 首先全局初始化CURL
	CURL* curl = curl_easy_init(); // 初始化CURL句柄
	if (NULL == curl) {
		printf("(curl is NULL!\r\n");}
	// 设置目标URL
	curl_easy_setopt(curl, CURLOPT_URL, "https://sc.ftqq.com/************=SomeoneFellDownAtHome!!");
	// 执行一次URL请求
	CURLcode res = curl_easy_perform(curl);
	// 清理干净
	curl_easy_cleanup(curl);	
}

//画身体
void CBodyBasics::DrawBody(const Joint * joints, const DepthSpacePoint*depthSpacePosition)
{
	// Draw the bones
	//---------------------------body-------------------------------
	DrawBone(joints, depthSpacePosition, JointType_Head, JointType_Neck);
	DrawBone(joints, depthSpacePosition, JointType_Neck, JointType_SpineShoulder);
	DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_SpineMid);
	DrawBone(joints, depthSpacePosition, JointType_SpineMid, JointType_SpineBase);
	DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_ShoulderRight);
	DrawBone(joints, depthSpacePosition, JointType_SpineShoulder, JointType_ShoulderLeft);
	DrawBone(joints, depthSpacePosition, JointType_SpineBase, JointType_HipRight);
	DrawBone(joints, depthSpacePosition, JointType_SpineBase, JointType_HipLeft);

	// -----------------------Right Arm ------------------------------------ 
	DrawBone(joints, depthSpacePosition, JointType_ShoulderRight, JointType_ElbowRight);
	DrawBone(joints, depthSpacePosition, JointType_ElbowRight, JointType_WristRight);
	DrawBone(joints, depthSpacePosition, JointType_WristRight, JointType_HandRight);
	DrawBone(joints, depthSpacePosition, JointType_HandRight, JointType_HandTipRight);
	DrawBone(joints, depthSpacePosition, JointType_WristRight, JointType_ThumbRight);

	//----------------------------------- Left Arm--------------------------
	DrawBone(joints, depthSpacePosition, JointType_ShoulderLeft, JointType_ElbowLeft);
	DrawBone(joints, depthSpacePosition, JointType_ElbowLeft, JointType_WristLeft);
	DrawBone(joints, depthSpacePosition, JointType_WristLeft, JointType_HandLeft);
	DrawBone(joints, depthSpacePosition, JointType_HandLeft, JointType_HandTipLeft);
	DrawBone(joints, depthSpacePosition, JointType_WristLeft, JointType_ThumbLeft);

	// ----------------------------------Right Leg--------------------------------
	DrawBone(joints, depthSpacePosition, JointType_HipRight, JointType_KneeRight);
	DrawBone(joints, depthSpacePosition, JointType_KneeRight, JointType_AnkleRight);
	DrawBone(joints, depthSpacePosition, JointType_AnkleRight, JointType_FootRight);

	// -----------------------------------Left Leg---------------------------------
	DrawBone(joints, depthSpacePosition, JointType_HipLeft, JointType_KneeLeft);
	DrawBone(joints, depthSpacePosition, JointType_KneeLeft, JointType_AnkleLeft);
	DrawBone(joints, depthSpacePosition, JointType_AnkleLeft, JointType_FootLeft);

	// Draw the joints
	for (int j = 0; j < JointType_Count; ++j)
	{
		if (joints[j].TrackingState == TrackingState_Inferred)//不确定的关节点 用青色
		{
			circle(BodyImg, CvPoint(depthSpacePosition[j].X, depthSpacePosition[j].Y), 3, cvScalar(0, 255, 255), -1, 8, 0);
		}
		else if (joints[j].TrackingState == TrackingState_Tracked) //确定的关节点 用紫色
		{
			circle(BodyImg, CvPoint(depthSpacePosition[j].X, depthSpacePosition[j].Y), 3, cvScalar(255, 0, 255), -1, 8, 0);
		}
	}
}

//画手势状态
void CBodyBasics::DrawHandState(HandState handState, const DepthSpacePoint handPosition)
{
	//给不同的手势分配不同颜色
	CvScalar color;
	switch (handState)
	{
	case HandState_Open:
		color = cvScalar(255, 0, 0);	//手是张开的状态，用蓝色表示
		break;
	case HandState_Closed:
		color = cvScalar(0, 255, 0);	//手是闭合的状态，用绿色表示
		break;
	case HandState_Lasso:
		color = cvScalar(0, 0, 255);	//手是介于张开和闭合的状态，用红色表示
		break;
	default:							//如果没有确定的手势，就不要画
		return;
	}

	circle(BodyImg,cvPoint(handPosition.X, handPosition.Y),20, color, -1);
}

//画骨架
void CBodyBasics::DrawBone(const Joint * pJoints, const DepthSpacePoint * depthSpacePosition, JointType joint0, JointType joint1)
{
	TrackingState joint0State = pJoints[joint0].TrackingState;
	TrackingState joint1State = pJoints[joint1].TrackingState;

	// If we can't find either of these joints, exit
	if ((joint0State == TrackingState_NotTracked) || (joint1State == TrackingState_NotTracked))
	{
		return;
	}

	// Don't draw if both points are inferred
	if ((joint0State == TrackingState_Inferred) && (joint1State == TrackingState_Inferred))
	{
		return;
	}

	CvPoint p1 = cvPoint(depthSpacePosition[joint0].X, depthSpacePosition[joint0].Y),
		p2 = cvPoint(depthSpacePosition[joint1].X, depthSpacePosition[joint1].Y);

	// We assume all drawn bones are inferred unless BOTH joints are tracked
	if ((joint0State == TrackingState_Tracked) && (joint1State == TrackingState_Tracked))
	{
		//非常确定的骨架，用白色直线
		line(BodyImg, p1, p2, cvScalar(255, 255, 255), 6);
	}
	else
	{
		//不确定的骨架，用红色直线
		line(BodyImg, p1, p2, cvScalar(0, 0, 255), 1);
	}
}

//角度测量
DOUBLE CBodyBasics::Angle(const DirectX::XMVECTOR* vec, JointType jointA, JointType jointB, JointType jointC)
{
	double angle = 0.0;

	XMVECTOR vBA = XMVectorSubtract(vec[jointB], vec[jointA]);
	XMVECTOR vBC = XMVectorSubtract(vec[jointB], vec[jointC]);

	XMVECTOR vAngle = XMVector3AngleBetweenVectors(vBA, vBC);

	angle = XMVectorGetX(vAngle) * 180.0 * XM_1DIVPI;    // XM_1DIVPI: An optimal representation of 1 / π

	return angle;
}

//距离测量
double CBodyBasics::Distance(Joint p1, Joint p2)
{
	double dist = 0;
	dist = sqrt(pow(p2.Position.X - p1.Position.X, 2) +
		pow(p2.Position.Y - p1.Position.Y, 2) + pow(p2.Position.Z - p1.Position.Z, 2));
	return dist;
}

//数据更新
void CBodyBasics::Update()
{
	//显示系统时间
	string time = getTime();
	//*********************Color
	ColorImg.setTo(0);
	ColorImg.create(cColorHeight, cColorWidth, CV_8UC4);
	IColorFrame* myColorFrame = nullptr;
	while (myColorReader->AcquireLatestFrame(&myColorFrame) != S_OK);
	myColorFrame->CopyConvertedFrameDataToArray(cColorHeight * cColorWidth * 4, ColorImg.data, ColorImageFormat_Bgra);

	putText(ColorImg, time, cvPoint(0, 1000), CV_FONT_HERSHEY_COMPLEX, 2, cvScalar(0, 0, 255), 2);

	//彩色数据处理及显示程序待写
	//OpenCV显示
	namedWindow("ColorImg", 0);
	resizeWindow("ColorImg", 640, 480);
	imshow("ColorImg", ColorImg);
	waitKey(5);
	SafeRelease(myColorFrame);

	//*********************Depth
	DepthImg.setTo(0);
	DepthImg.create(cDepthHeight, cDepthWidth, CV_16UC1);
	Mat iDepthImg(cDepthHeight, cDepthWidth, CV_8UC1);
	IDepthFrame* myDepthFrame = nullptr;
	while (myDepthReader->AcquireLatestFrame(&myDepthFrame) != S_OK);
	myDepthFrame->CopyFrameDataToArray(cDepthHeight * cDepthWidth, reinterpret_cast<UINT16*>(DepthImg.data));
	DepthImg.convertTo(iDepthImg, CV_8U, 255.0f / 4500);//converto()第一个参数是输出矩阵，第二个是转换类型，第三个是缩放因子，其中4500是深度数据的最大距离
	putText(DepthImg, time, cvPoint(0, 400), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
	//深度数据处理及显示程序待写  
	//OpenCV显示
	namedWindow("DepthImg", 0);
	resizeWindow("DepthImg", 640, 480);
	imshow("DepthImg",iDepthImg);
	waitKey(5);
	SafeRelease(myDepthFrame);

	//**********************Infrared
	/*int        iWidth = 0;
	int        iHeight = 0;
	IFrameDescription* pFrameDescription = nullptr;
	pFrameDescription->get_Width(&iWidth);
	pFrameDescription->get_Height(&iHeight);
	pFrameDescription->Release();
	pFrameDescription = nullptr;
	InfraredImg.setTo(0);
	InfraredImg.create(iHeight,iWidth,CV_16UC1);
	IInfraredFrame* myInfraredFrame = nullptr;
	while(myInfraredReader->AcquireLatestFrame(&myInfraredFrame) == S_OK);
	myInfraredFrame->CopyFrameDataToArray(iWidth * iHeight,reinterpret_cast<UINT16*>(InfraredImg.data));//强制转换数据类型
	putText(InfraredImg, time, cvPoint(0, 400), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
	namedWindow("InfraredImage");
	imshow("InfraredImage", InfraredImg);*/

	//*********************Body
	BodyImg.setTo(0);
	BodyImg.create(cDepthHeight, cDepthWidth, CV_8UC3);
	IBodyFrame* myBodyFrame = NULL;
	while (myBodyReader->AcquireLatestFrame(&myBodyFrame) != S_OK);
	IBody* myBodies[BODY_COUNT] = { 0 };
	while (myBodyFrame->GetAndRefreshBodyData(_countof(myBodies), myBodies) != S_OK);
	
	putText(BodyImg, time, cvPoint(0, 400), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
	
	ProcessBody(BODY_COUNT, myBodies);

	//Opencv显示
	namedWindow("BodyImg", 0);
	resizeWindow("BodyImg", 640, 480);
	imshow("BodyImg", BodyImg);
	waitKey(5);
	for (int i = 0; i < _countof(myBodies); ++i)
	{
		SafeRelease(myBodies[i]);
	}
	SafeRelease(myBodyFrame);
}

//身体数据处理
void CBodyBasics::ProcessBody(int nBodyCount, IBody ** aBodyData)
{
	HRESULT hr;
	FilterDoubleExponential filterKinect;	//霍尔特双指数滤波器

	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* myBody = aBodyData[i];
		if (myBody)
		{
			BOOLEAN bTracked = false;
			hr = myBody->get_IsTracked(&bTracked);  //确认追踪状态
			filterKinect.Update(myBody);			//平滑和稳定身体骨架

			if (SUCCEEDED(hr) && bTracked)
			{
				//存储关节点类
				Joint joints[JointType::JointType_Count];
				HandState leftHandState = HandState_Unknown;//左手状态
				HandState rightHandState = HandState_Unknown;//右手状态

				//获取左右手状态
				myBody->get_HandLeftState(&leftHandState);
				myBody->get_HandRightState(&rightHandState);
				//存储深度坐标系中关节点位置
				DepthSpacePoint *depthSpacePosition = new DepthSpacePoint[_countof(joints)];
				
				//获取关节点类
				if (myBody->GetJoints(JointType::JointType_Count, joints) == S_OK)
				{
					filterKinect.Update(joints);		//平滑身体每个骨骼点
					
					const DirectX::XMVECTOR* vec = filterKinect.GetFilteredJoints();    // Retrive Filtered Joints

					double angle = Angle(vec, JointType_WristRight, JointType_ElbowRight, JointType_ShoulderRight); // Get ElbowRight joint angle
					char s[20];
					sprintf_s(s, "Angle:%.0f", angle);
					string strAngleInfo = s;
					putText(BodyImg, strAngleInfo, cvPoint(0, 50), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255)); // 屏幕上显示角度信息
					
					for (int j = 0; j < _countof(joints); ++j)
					{
							float x = 0.0f, y = 0.0f, z = 0.0f;
							// Retrieve the x/y/z component of an XMVECTOR Data and storing that component's value in an instance of float referred to by a pointer
							XMVectorGetXPtr(&x, vec[j]);
							XMVectorGetYPtr(&y, vec[j]);
							XMVectorGetZPtr(&z, vec[j]); 

							CameraSpacePoint cameraSpacePoint = { x, y, z };
							myMapper->MapCameraPointToDepthSpace(cameraSpacePoint, &depthSpacePosition[j]);
							
							//将关节点坐标从摄像机坐标系（-1~1）转到深度坐标系（424*512）
							//myMapper->MapCameraPointToDepthSpace(joints[j].Position, &depthSpacePosition[j]);
					}	

					DrawBody(joints, depthSpacePosition);

					DrawHandState(leftHandState, depthSpacePosition[JointType_HandLeft]);
					DrawHandState(rightHandState, depthSpacePosition[JointType_HandRight]);

					// 获取关节方向
					JointOrientation aOrientations[JointType::JointType_Count];
					if (myBody->GetJointOrientations(JointType::JointType_Count, aOrientations) != S_OK)
					{
						cerr << "Get joints fail" << endl;
					}

					//savephoto
					if (tempnumber > 0) {
						if (framenumber == (tempnumber + 2)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
						if (framenumber == (tempnumber + 5)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
						if (framenumber == (tempnumber + 10)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
						if (framenumber == (tempnumber + 15)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
						if (framenumber == (tempnumber + 20)) {
							SaveColorImg();
							SaveDepthImg();
							SaveBodyImg();}
					}

					//falldetection
					if (joints[JointType_SpineBase].Position.Z > 0.5&&joints[JointType_SpineBase].Position.Z<3.5)
						Detection(joints);
					else
						cout << "为了检测的准确度，请尽量站在离Kinect 0.5--3.5 米之间，谢谢配合！" << endl;
					
				}
				delete[] depthSpacePosition;
			}
		}
	}
	framenumber++;
	depthnumber++;
	colornumber++;
}

//检测程序
void CBodyBasics::Detection(Joint joints[])
{
	
	//检测速度特征
	static double tin, tout;
	//double tframe;
	//大约30帧为1s,计算每相邻30帧的高度差，从而计算速度，1，31，62....
	if (framenumber % 11 == 1)		//framenumber是帧序列号，自己定义的
	{
		tin = static_cast<double>(GetTickCount());
		//cout << "tin是" << tin << endl;
		SpineHeightin = joints[JointType_SpineMid].Position.Y;
		//-cout << "当前帧号为：" << framenumber << endl;
		//cout << "当前SpineHeightin的高度为" << SpineHeightin << "  m"<<endl;
	}
	if (!(framenumber % 11))
	{
		tout = static_cast<double>(GetTickCount());
		cout << framenumber << endl;
		//cout <<"tout是"<< tout << endl;
		cout << "每10帧计算一次下降的速度" << endl;
		SpineHeightout = joints[JointType_SpineMid].Position.Y;
		cout << "当前帧号为：" << framenumber << endl;
		 cout << "***********************************" << endl;
		// cout << "当前SpineHeightin的高度为" << SpineHeightin << "  m" << endl;
		//tframe = (tout - tin) / getTickFrequency();
		// cout <<tframe << endl;
		// cout << getTickFrequency()<<endl;
		//cout << "当前SpineHeightout的高度为" << SpineHeightout << "  m" << endl;
		//SpineV = (SpineHeightin - SpineHeightout) / tframe;
		SpineV = SpineHeightin - SpineHeightout;
		cout << "SpineV是多少？？" << SpineV << endl;

		// 屏幕上显示速度信息
		char v[20];
		sprintf_s(v, "downSpeed:%.4f", SpineV);
		string strSpeedInfo = v;
		putText(BodyImg, strSpeedInfo, cvPoint(0, 100), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255)); 

		if ((SpineV) > 0.35)	//文献中给定的数据是1.35m/s，这个可能要根据实际情况略有调整	
		{
			vDetection = true;
			tempnumber = framenumber;
			stringstream stream0;
			string str, str1;
			stream0 << SpineV;
			stream0 >> str;
			str1 = "身体中心向下的速度是： " + str + " m/s\r\n";
			cout << "身体中心向下的速度是：   " << (SpineV) << "    m/s" << endl;
		}
		else vDetection = false;
	}

	//检测高度特征，对地面的检测，离地面的距离。这里转化为spine和foot之间的高度。
	if ((joints[JointType_SpineBase].Position.Y - joints[JointType_FootRight].Position.Y) <= 0.5)//0.22
	{
		waitKey(15);//延时检测，防止误判             waitkey()函数的作用是不断刷新图像，频率时间是delay，单位是ms
		if ((joints[JointType_SpineBase].Position.Y - joints[JointType_FootRight].Position.Y) <= 0.5)
		{
			if (vDetection)
			{
				HeightDetection = TRUE;
				vDetection = false;

				stringstream stream0, stream1;
				string str, str1, str2;
				stream0 << joints[JointType_SpineBase].Position.Y;
				stream0 >> str;
				str1 = "JointType_SpineBase的高度是： " + str + " m\r\n";
				stream1 << joints[JointType_FootRight].Position.Y;
				stream1 >> str;
				str2 = "JointType_FootRight的高度是： " + str + " m\r\n";

				cout << "JointType_SpineBase的高度是  " << joints[JointType_SpineBase].Position.Y << "\tm" << endl;
				cout << "***************" << endl;
				cout << "JointType_FootRight的高度是  " << joints[JointType_FootRight].Position.Y << "\tm" << endl;
				cout << "***************" << "成功检测跌倒" << "***************" << endl;
				
				// 屏幕上显示高度信息
				char h1[20];
				sprintf_s(h1, "SpineBaseH:%.4f", joints[JointType_SpineBase].Position.Y);
				string strSpineBaseHInfo = h1;
				putText(BodyImg, strSpineBaseHInfo, cvPoint(0, 150), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));
				char h2[20];
				sprintf_s(h2, "RightFootH:%.4f", joints[JointType_FootRight].Position.Y);
				string strRightFootHInfo = h2;
				putText(BodyImg, strRightFootHInfo, cvPoint(0, 170), CV_FONT_HERSHEY_COMPLEX, 0.5, cvScalar(0, 0, 255));

				SaveBodyImg();	//检测到跌倒事件，保存当前图片信息
				SaveColorImg();
				SaveDepthImg();
				//SpeechDetection();		//语音询问
				SendEmail();
				curl_test();
			}
		}
		else
		{
			HeightDetection = FALSE;
		}
	}
} 
