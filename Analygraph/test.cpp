/*
Mat C = (Mat_<double>(3, 6) << 1, 2, 3, 4, 5, 6,
    7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18);
cout << "C = " << endl << " " << C << endl;
Matrix<double, 3, 3, RowMajor> ml;
ml << 1, 0, 0,
0, 0, 0,
0, 0, 1;
Matrix<double, 3, 6> Mat;
Mat << 1, 2, 3, 4, 5, 6,
7, 8, 9, 10, 11, 12,
13, 14, 15, 16, 17, 18;

Map<Matrix<double, Dynamic, Dynamic>> Mat_rm(Mat.data(), 6, 3);
MatrixXd resM;
cv2eigen(C, resM);

for (int i = 0; i < resM.size(); i++)
    cout << *(resM.data() + i) << "  ";
cout << endl << endl;
Matrix<double, Dynamic, Dynamic, RowMajor> resRM = resM;
for (int i = 0; i < resRM.size(); i++)
    cout << *(resRM.data() + i) << "  ";
cout << endl << endl;

Map<Matrix<double, Dynamic, Dynamic, RowMajor>> resRM_reshape(resRM.data(), 6, 3);
cout << "resRM_reshape: " << endl << resRM_reshape << endl;
Matrix<double, Dynamic, Dynamic, RowMajor> resRM_rotate = resRM_reshape.transpose().colwise().reverse();
cout << "resRM_rotate: " << endl << resRM_rotate << endl;
Matrix<double, Dynamic, Dynamic, RowMajor> resRM_rotback = resRM_rotate.transpose().rowwise().reverse();
cout << "resRM_rotback: " << endl << resRM_rotback << endl;
Map<Matrix<double, Dynamic, Dynamic, RowMajor>> resRM_restore(resRM_rotback.data(), 3, 6);
cout << "resRM_restore: " << endl << resRM_restore << endl;
*/