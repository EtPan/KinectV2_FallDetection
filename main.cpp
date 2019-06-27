#include <iostream>
#include "pKinect.h"


using namespace std;
using namespace cv;

int main() 
{
	CBodyBasics pKinect;
	HRESULT hr;
	hr = pKinect.InitKinect();
	if (SUCCEEDED(hr)) {

		while(1){
		pKinect.Update();}
		
	}
	else {
		cout << "kinect initialization failed!" << endl;
		system("pause");
	}
}