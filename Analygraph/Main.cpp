#include <iostream>
#include <string>
#include <chrono>
#include "Analygraph.h"

using namespace std;

int main(int argc, char** argv)
{
    // by default
    String imageName("../image/Lantern.jpeg"); 
    string leftImage("../image/Mars_l.jpg"), rightImage("../image/Mars_r.jpg");
    bool isSingle = true;
    bool isMp = false;
    bool isMc = false;
    string func("Color"); 
    string imagePath("../image/");

    if (argc > 5)
    {
        if (strcmp(argv[1],"single")==0)
        {
            isSingle = true;
        }
        else if (strcmp(argv[1], "pair")==0)
        {
            isSingle = false;
        }

        isMp = strcmp(argv[2], "Y") == 0 ?  true: false;
        isMc = strcmp(argv[3], "Mc") == 0 ? true : false;
        func = argv[4];
        if (isSingle)
        {
            imageName =imagePath+ argv[5];
            
        }
        else
        {
            leftImage = imagePath + argv[5];
            rightImage = imagePath + argv[6];
        }
    }

    
    auto start = chrono::system_clock::now();

    Analygraph analygraph;
    if (isSingle) {
        if (!analygraph.LoadImage(imageName))
        {
            return -1;
        }
    }
    else
    {
        if (!analygraph.LoadImage(leftImage, rightImage)) 
        {
            return -1;
        }
    }

    if (isMc)
    {
        analygraph.CreateAnlygraphMc(func);
    }
    else if (isMp)
    {
        analygraph.CreateAnlygraphMP(func);
    }
    else
    {
        analygraph.CreateAnlygraph(func);
    }
    
    auto stop = chrono::system_clock::now();
    cout << "Complete: \n";
    cout << "Time taken: " << chrono::duration_cast<chrono::minutes>(stop - start).count() << " minutes\n";
    cout << "          : " << chrono::duration_cast<chrono::seconds>(stop - start).count() << " seconds\n";
    

    waitKey(0);
    return 0;
}
  
