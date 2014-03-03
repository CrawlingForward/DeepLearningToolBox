/*����ϡ����뻹û��ɣ������������²������д�����*/
#include <iostream>
#include <Eigen/Dense>
#include "FunctionBase.h"
#include "SparseCoding.h"
#include <windows.h>
#include "getConfig.h"
using namespace std;
using namespace Eigen;


int main()
{
	//timer
	clock_t start = clock();
#ifdef _WINDOWS_
	//set eigen threads
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	Eigen::setNbThreads(info.dwNumberOfProcessors);
#endif
	
	int imgWidth = 8;
	int batchNumPatches = 200;
	int maxIter = 500;
	int gdIter = 3000;
	double lambda = 1e-5;//ϡ����ϵ��
	double alpha = 1;//ѧϰ����
	double epsilon = 1e-5;//ϡ������
	double gamma = 1e-5;//Ȩ��˥��ϵ��
	int numFeatures = 121;
	char *fileBuf  = new char[4096*10];
	bool ret = loadFileToBuf("ParamConfig.ini",fileBuf,4096);
	if(ret)
	{
		getConfigIntValue(fileBuf,"batchNumPatches:",batchNumPatches);
		getConfigIntValue(fileBuf,"maxIter:",maxIter);
		getConfigIntValue(fileBuf,"gdIter:",gdIter);
		getConfigDoubleValue(fileBuf,"lambda:",lambda);
		getConfigDoubleValue(fileBuf,"alpha:",alpha);
		getConfigDoubleValue(fileBuf,"epsilon:",epsilon);
		getConfigDoubleValue(fileBuf,"gamma:",gamma);
		getConfigIntValue(fileBuf,"numFeatures:",numFeatures);
		getConfigIntValue(fileBuf,"imgWidth:",imgWidth);
		cout << "batchNumPatches: " << batchNumPatches << endl;
		cout << "maxIter: " << maxIter << endl;
		cout << "gdIter: " << gdIter << endl;
		cout << "lambda: " << lambda << endl;
		cout << "alpha: " << alpha << endl;
		cout << "epsilon: " << epsilon << endl;
		cout << "gamma: " << gamma << endl;
		cout << "numFeatures: " << numFeatures << endl;
		cout << "imgWidth: " << imgWidth << endl;
	}
	delete []fileBuf;
	MatrixXd data(1,1);
	loadDataSet(data,"data.txt");
	MatrixXd showData = data.topRows(100);
	buildImage(showData,imgWidth,"data.jpg");

	MatrixXd patches = data.transpose();
	SparseCoding sc(imgWidth * imgWidth,numFeatures,batchNumPatches);
	sc.train(patches,maxIter,batchNumPatches,numFeatures,
		gdIter,alpha,lambda,epsilon,gamma,imgWidth);
	MatrixXd wt = sc.getWeight().transpose();
	buildImage(wt,imgWidth,"weights.jpg");

	clock_t end = clock();
	cout << "The code ran for " << 
		(end - start)/(double)(CLOCKS_PER_SEC*60) <<
		" minutes on " << Eigen::nbThreads() << " thread(s)." << endl;

	system("pause");
	return 0;
}