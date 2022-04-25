#pragma once

#include <Eigen/Dense>
#include <Eigen/Core>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/eigen.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <utility>
#include <string>
#include <unordered_map>
#include <omp.h>

using namespace cv;
using namespace std;
using namespace Eigen;

class Analygraph
{
public:
	Analygraph();
	bool LoadImage(string imagePath);
	bool LoadImage(string leftPath, string rightPath);
	void CreateAnlygraph(string func);
	void CreateAnlygraphMP(string func);
	void CreateAnlygraphMc(string func);
private:
	Mat lImage, rImage; //left image and right image
	int rowSize, colSize;

	unordered_map<string, pair<Matrix3d, Matrix3d>> funcMap;
	
};

