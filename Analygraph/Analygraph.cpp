#include "Analygraph.h"

Analygraph::Analygraph()
{
    rowSize = 0;
    colSize = 0;
    Matrix3d M_l, M_r;
    //True Anaglyph
    M_l << 0.299f, 0.587, 0.144,
        0, 0, 0,
        0, 0, 0;
    M_r << 0, 0, 0,
        0, 0, 0,
        0.299, 0.587, 0.114;
    funcMap.emplace("True", make_pair(M_l, M_r));
    //Gray Anaglyph
    M_l << 0.299, 0.587, 0.114,
        0, 0, 0,
        0, 0, 0;
    M_r << 0, 0, 0,
        0.299, 0.587, 0.114,
        0.299, 0.587, 0.114;
    funcMap.emplace("Gray", make_pair(M_l, M_r));
    //Color Anaglyph
    M_l << 1, 0, 0,
        0, 0, 0,
        0, 0, 0;
    M_r << 0, 0, 0,
        0, 1, 0,
        0, 0, 1;
    funcMap.emplace("Color", make_pair(M_l, M_r));
    //Half Color Anaglyph
    M_l << 0.299, 0.587, 0.114,
        0, 0, 0,
        0, 0, 0;
    M_r << 0, 0, 0,
        0, 1, 0,
        0, 0, 1;
    funcMap.emplace("HalfColor", make_pair(M_l, M_r));
    //Optimized Anaglyph
    M_l << 0, 0.7, 0.3,
        0, 0, 0,
        0, 0, 0;
    M_r << 0, 0, 0,
        0, 1, 0,
        0, 0, 1;
    funcMap.emplace("Optimized", make_pair(M_l, M_r));
    //DuBois Anaglyph
    M_l << 0.437, 0.449, 0.164,
        -0.062, -0.062, -0.024,
        -0.048, -0.050, -0.017;
    M_r << -0.011, -0.032, -0.007,
        0.377, 0.761, 0.009,
        -0.026, -0.093, 1.234;
    funcMap.emplace("DuBois", make_pair(M_l, M_r));
    //Roscolux Anaglyph
    M_l << 0.3185, 0.0769, 0.0109,
        0.1501, 0.0767, 0.0056,
        0.0007, 0.0020, 0.0156;
    M_r << 0.0174, 0.0484, 0.1402,
        0.0184, 0.1807, 0.0458,
        0.0286, 0.0991, 0.7662;
    funcMap.emplace("Roscolux", make_pair(M_l, M_r));
}

bool Analygraph::LoadImage(string imagePath)
{
	lImage = imread(imagePath, IMREAD_COLOR);
    if (lImage.empty())                      // Check for invalid input
    {
        cout << "Could not open or find the image" << endl;
        return false;
    }
    //Crop image
    int oriCols = lImage.cols;
    rowSize = lImage.rows;
    colSize = oriCols / 2;
    rImage = lImage(Range::all(), Range(colSize, oriCols));
    lImage = lImage(Range::all(), Range(0, colSize));
    return true;

}

bool Analygraph::LoadImage(string leftPath, string rightPath)
{
	lImage = imread(leftPath, IMREAD_COLOR); 
	rImage = imread(rightPath, IMREAD_COLOR);
    if (lImage.empty()||rImage.empty())                      // Check for invalid input
    {
        cout << "Could not open or find the image" << endl;
        return false;
    }
    //Crop image
	int l_rowSize = lImage.rows;
    int l_colSize = lImage.cols;
    int r_rowSize = rImage.rows;
    int r_colSize = rImage.cols;
    rowSize = min(l_rowSize, r_rowSize);
    colSize = min(l_colSize, r_colSize);
    rImage = rImage(Range(0,rowSize), Range(0, colSize));
    lImage = lImage(Range(0, rowSize), Range(0, colSize));
    return true;
}

void Analygraph::CreateAnlygraph(string func)
{
    Mat res;
    Tensor<double, 3, RowMajor> lTensor, rTensor, resTensor(rowSize, colSize, 3);
    cv2eigen(lImage, lTensor);
    cv2eigen(rImage, rTensor);

    Matrix3d M_l, M_r;
    if (funcMap.find(func) != funcMap.end()) {       // Check for invalid function input
        pair<Matrix3d, Matrix3d> funcPair = funcMap[func];
        M_l = funcPair.first;
        M_r = funcPair.second;
    }
    else {
        cout << "Invalid Function" << endl;
        return;
    }


    Vector3d lColor, rColor, resColor;
    for (int i = 0; i < rowSize; i++)
    {
        for (int j = 0; j < colSize; j++)
        {

            for (int k = 0; k < 3; k++)
            {
                lColor(k) = lTensor(i, j, 2 - k);    //read BRG color to RGB Vector
                rColor(k) = rTensor(i, j, 2 - k);
            }
            resColor = M_l * lColor + M_r * rColor;
            for (int k = 0; k < 3; k++)
            {
                resTensor(i, j, k) = resColor(2 - k);  //Write the result back
            }
        }
    }

    eigen2cv(resTensor, res);
    imshow("res.png", res * (1.0 / 255));
    imwrite("../image/Output.jpg", res);
}

void Analygraph::CreateAnlygraphMP(string func)
{
    Mat res;
    Tensor<double, 3, RowMajor> lTensor, rTensor, resTensor(rowSize, colSize, 3);
    cv2eigen(lImage, lTensor);
    cv2eigen(rImage, rTensor);

    Matrix3d M_l, M_r;
    if (funcMap.find(func) != funcMap.end()) {       // Check for invalid function input
        pair<Matrix3d, Matrix3d> funcPair = funcMap[func];
        M_l = funcPair.first;
        M_r = funcPair.second;
    }
    else {
        cout << "Invalid Function" << endl;
        return;
    }


#pragma omp parallel
    {
#pragma omp for collapse(2)
        for (int i = 0; i < rowSize; i++)
        {

            for (int j = 0; j < colSize; j++)
            {
                Vector3d lColor, rColor, resColor;
                for (int k = 0; k < 3; k++)
                {
                    lColor(k) = lTensor(i, j, 2 - k);    //read BRG color to RGB Vector
                    rColor(k) = rTensor(i, j, 2 - k);
                }
                resColor = M_l * lColor + M_r * rColor;
                for (int k = 0; k < 3; k++)
                {
                    resTensor(i, j, k) = resColor(2 - k);  //Write the result back
                }
            }
        }
    }
    eigen2cv(resTensor, res);
    imshow("res.png", res * (1.0 / 255));
    imwrite("../image/Output.jpg", res);
}

void Analygraph::CreateAnlygraphMc(string func)
{
    Matrix3d M_l, M_r;
    if (funcMap.find(func) != funcMap.end()) {       // Check for invalid function input
        pair<Matrix3d, Matrix3d> funcPair = funcMap[func];
        M_l = funcPair.first;
        M_r = funcPair.second;
    }
    else {
        cout << "Invalid Function" << endl;
        return;
    }

    int chs = lImage.channels();

    lImage = lImage.reshape(1, 0);
    rImage = rImage.reshape(1, 0);
    Matrix<double, Dynamic, Dynamic> lMat_CM, rMat_CM;
    cv2eigen(lImage, lMat_CM);
    cv2eigen(rImage, rMat_CM);

    Matrix<double, Dynamic, Dynamic, RowMajor> lMat = lMat_CM, rMat = rMat_CM; //convert colMajor to rowMajor
    
    Map<Matrix<double, Dynamic, Dynamic, RowMajor>> lMat_reshaped(lMat.data(), rowSize * colSize, 3), rMat_reshaped(rMat.data(), rowSize * colSize, 3); //reshape to [rowSize * colSize, 3]

    Matrix<double, Dynamic, Dynamic, RowMajor> lMat_rotated = lMat_reshaped.transpose().colwise().reverse(), rMat_rotated = rMat_reshaped.transpose().colwise().reverse();
    Matrix<double, Dynamic, Dynamic, RowMajor> resMat_rotated = M_l * lMat_rotated + M_r * rMat_rotated;
    Matrix<double, Dynamic, Dynamic, RowMajor> resMat = resMat_rotated.transpose().rowwise().reverse();
    Map<Matrix<double, Dynamic, Dynamic, RowMajor>> ResMatMap(resMat.data(), rowSize, colSize *3);
    Matrix<double, Dynamic, Dynamic> R = ResMatMap; //convert rowMajor to colMajor


    /*
    Eigen::array<bool, 3> rev = { false,false,true };  //convert BRG to RGB
    lTensor = lTensor.reverse(rev);
    rTensor = rTensor.reverse(rev);
    
    Eigen::array<DenseIndex, 2> newDim = { {rowSize * colSize,3} };  //reshape to {rowSize * colSize,3}
    Eigen::array<DenseIndex, 2> shuffling= { {1,0} };     //Transpose matrix
    Tensor<double, 2, RowMajor> lTran, rTran,resTran;
    lTran = lTensor.reshape(newDim).shuffle(shuffling);
    rTran = rTensor.reshape(newDim).shuffle(shuffling);
    */
    /*
    Eigen::Map<Matrix<double, Dynamic, Dynamic,RowMajor>> lMat(lTensor.data(),rowSize * colSize, 3), rMat(rTensor.data(),rowSize * colSize, 3);  //reshape to {rowSize * colSize,3} Matrix
    Matrix<double, Eigen::Dynamic, Eigen::Dynamic, RowMajor> resMat = M_l * lMat.transpose() + M_r * rMat.transpose();
    resMat.transposeInPlace();
    TensorMap<Tensor<double, 3, RowMajor>>  resTensor(resMat.data(), rowSize, colSize, 3);
    resTensor = resTensor.reverse(rev); //convert RGB to BRG
    Tensor<double, 3, RowMajor> resT= resTensor;
    */

    Mat res;
    eigen2cv(R, res);
    res = res.reshape(chs, 0);
    imshow("res.jpg", res * (1.0 / 255));
    imwrite("../image/Output.jpg", res);
}
