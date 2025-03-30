#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;


const float gaussianMask[5][5] = {
    {0.0024, 0.0126, 0.0215, 0.0126, 0.0024},
    {0.0126, 0.0633, 0.1061, 0.0633, 0.0126},
    {0.0215, 0.1061, 0.1764, 0.1061, 0.0215},
    {0.0126, 0.0633, 0.1061, 0.0633, 0.0126},
    {0.0024, 0.0126, 0.0215, 0.0126, 0.0024}
};



//Funkcija koja prebacuje u crno-bijelu sliku
void cbFilter(const Mat& img, Mat& cbimg){

    cbimg= Mat(img.rows, img.cols,CV_8UC1); //prazna cb slika

    //konverzija svakog piksela
    for(int y=0; y< img.rows;y++){
     for(int x=0; x<img.cols; x++){
        Vec3b bgr= img.at<Vec3b>(y,x); //Pristup pikselima preko openCV-a
        uchar gray = static_cast<uchar>(0.299 * bgr[2] + 0.587 * bgr[1] + 0.114 * bgr[0]);
            cbimg.at<uchar>(y, x) = gray;
        }
    }

}

//Funkcija gausovo zamucenje za crnobijelu sliku
void gbFilter(const Mat& img,Mat& gbimg){

    gbimg= Mat(img.rows, img.cols,CV_8UC1);//prazna slika 

    int maskSize=5; //zadata 5x5 maska, moze da se koristi i 3x3,predstavlja samo koliko ce kvalitetno zamucenje biti
    int offset=maskSize / 2; // pozicija sredisnjeg piksela

    //konverzija svakog piksela
    for(int y=0; y<img.rows;y++){
        for(int x=0;x<img.cols;x++){
            float blurredValue=0.0f;


            //Prolazak kroz okolne piksele
            for(int ky=-offset; ky<=offset; ky++){
                for(int kx = -offset; kx <= offset;kx++){
                    int iy=y + ky;
                    int ix=x + kx;

                    //da li je unutar granice slike
                    if(iy >= 0 && iy <img.rows && ix >= 0 && ix<img.cols){
                        uchar pixel =img.at<uchar>(iy,ix);
                        blurredValue += static_cast<float>(pixel) * gaussianMask[ky + offset][kx + offset];

                    }
                }
            }
            gbimg.at<uchar>(y,x)= static_cast<uchar>(blurredValue);
        }
    }
}

//funkcija za morfolosku eroziju
void erFilter(const Mat& img,Mat& erimg,int maskSize){

    erimg= Mat(img.rows, img.cols,CV_8UC1);//prazna slika 

   ///int maskSize=5; //zadata 5x5 maska, moze da se koristi i 3x3,predstavlja samo koliko ce kvalitetno zamucenje biti
    int offset=maskSize / 2; // pozicija sredisnjeg piksela

    //konverzija svakog piksela
    for(int y=0; y<img.rows;y++){
        for(int x=0;x<img.cols;x++){
            bool eroded=true;


            //Prolazak kroz okolne piksele
            for(int ky=-offset; ky<=offset; ky++){
                for(int kx = -offset; kx <= offset;kx++){
                    int iy=y + ky;
                    int ix=x + kx;

                    //da li je unutar granice slike
                    if(iy >= 0 && iy <img.rows && ix >= 0 && ix<img.cols){
                        
                        if(img.at<uchar>(iy, ix) == 0){
                            eroded =false;
                            break;
                        }

                    }
                }
                if(!eroded) break;
            }
            erimg.at<uchar>(y,x)= eroded ? 255 : 0;
        }
    }

}

//funkcija za dilataciju-> kontra od erozije
void dlFilter(const Mat& img,Mat& dlimg,int maskSize){ 

    dlimg= Mat(img.rows, img.cols,CV_8UC1);//prazna slika 

   ///int maskSize=5; //zadata 5x5 maska, moze da se koristi i 3x3,predstavlja samo koliko ce kvalitetno zamucenje biti
    int offset=maskSize / 2; // pozicija sredisnjeg piksela

    //konverzija svakog piksela
    for(int y=0; y<img.rows;y++){
        for(int x=0;x<img.cols;x++){
            bool dilated=false;


            //Prolazak kroz okolne piksele
            for(int ky=-offset; ky<=offset; ky++){
                for(int kx = -offset; kx <= offset;kx++){
                    int iy=y + ky;
                    int ix=x + kx;

                    //da li je unutar granice slike
                    if(iy >= 0 && iy <img.rows && ix >= 0 && ix<img.cols){
                        
                        if(img.at<uchar>(iy, ix) == 0){
                            dilated=true;;
                            break;
                        }

                    }
                }
                if(dilated) break;
            }
            dlimg.at<uchar>(y,x)= dilated ? 255 : 0;
        }
    }

}



///////Glavni dio programa///////
int main() {

    
    cout << "Hello, World!" << endl;

    Mat cbimg,gbimg,erimg,dlimg;

    Mat img=imread("karta.jpeg");

    if(img.empty()){

        cout<<"Nije ucitana slika"<<endl;
        return -1;
    }

    cbFilter(img,cbimg);
    gbFilter(cbimg,gbimg);
    erFilter(gbimg,erimg,3);
    dlFilter(cbimg,dlimg,5);

    imshow("Prikazana slika",img);
    imshow("crnobijela slika",cbimg);
    imshow("Gaus blur slika",gbimg);
    imshow("Erozija slike",erimg);
    imshow("Dilatacija slike",dlimg);


    waitKey(0);


    return 0;
}
