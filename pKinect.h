
#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "Kinect.h"
#include "KinectJointFilter.h"
#include <DirectXMath.h>
#include <cmath>
#include "smtp.h"
#include <time.h>
#include <string>
#include <G:\kinect\demo\0backup\falldetction_0426\libcurl-vc14-x64-debug-static-ipv6-sspi-winssl\include\curl/curl.h>   

#pragma comment(lib, "ws2_32.lib") 
#pragma comment(lib, "wldap32.lib")

// Safe release for interfaces
template<class Interface> inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}

class CBodyBasics
{
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;
	static const int        cColorWidth = 1920;
	static const int        cColorHeight = 1080;

public:
	/// Constructor
	CBodyBasics();

	/// Destructor
	~CBodyBasics();
	
	/// Main processing function
	void                    Update();

	/// Initializes the default Kinect sensor
	/// <returns>S_OK on success, otherwise failure code</returns>
	HRESULT                 InitKinect();

	/// Save body image
	void		SaveBodyImg();
	/// Save depth image
	void        SaveDepthImg();
	/// Save color image
	void		SaveColorImg();
	/// Detect sound
	void		SpeechDetection();
	/// Send email
	void        SendEmail();
	/// Get Time
	string		getTime();
	/// Get curl
	void		curl_test();

private:
	// Current Kinect
	IKinectSensor*			mySensor;
	ICoordinateMapper*		myMapper;

	//  Reader
	IColorFrameReader*		myColorReader;
	IDepthFrameReader*		myDepthReader;
	IBodyFrameReader*		myBodyReader;
	IInfraredFrameReader*   myInfraredReader;

	/// Handle new body data
	/// <param name="nBodyCount">body data count</param>
	/// <param name="aBodyData">body data in frame</param>
	void                    ProcessBody(int nBodyCount, IBody** aBodyData);

	/// Draws a body 
	/// <param name="joints">joint data</param>
	/// <param name="depthSpacePosition">joint positions converted to screen space</param>
	void                    DrawBody(const Joint* joints, const DepthSpacePoint*depthSpacePosition);

	/// Draws a hand symbol if the hand is tracked: red circle = closed, green circle = opened; blue circle = lasso
	/// <param name="handState">state of the hand</param>
	/// <param name="handPosition">position of the hand</param>
	void                    DrawHandState(HandState handState, const DepthSpacePoint handPosition);

	/// Draws one bone of a body (joint to joint)
	/// <param name="pJoints">joint data</param>
	/// <param name="pJointPoints">joint positions converted to screen space</param>
	/// <param name="pJointPoints">joint positions converted to screen space</param>
	/// <param name="joint0">one joint of the bone to draw</param>
	/// <param name="joint1">other joint of the bone to draw</param>
	void                    DrawBone(const Joint* pJoints, const DepthSpacePoint* depthSpacePosition, JointType joint0, JointType joint1);

	///Measure the angle between two joints
	DOUBLE					Angle(const DirectX::XMVECTOR* vec, JointType jointA, JointType jointB, JointType jointC);
	///Measure the distance between two joints
	double					Distance(Joint p1, Joint p2);
	///
	void					Detection(Joint joints[]);

	cv::Mat BodyImg;
	cv::Mat DepthImg;
	cv::Mat ColorImg;
	cv::Mat InfraredImg;

};