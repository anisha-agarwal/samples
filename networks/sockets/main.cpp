#include <stdio.h>
#include <stdlib.h>
#include "image.h"
#include <GL/glut.h>
#include <unistd.h>
#include <math.h>
#include<time.h>

void encode(int quant,Image<unsigned char>* in, Image<int>* out);
void decode_sequential(int quant,Image<int>* in, Image<unsigned char>* out);
void decode_progressive_spectral(int quant,Image<int>* in, Image<unsigned char>* out, int k);
void decode_progressive_bit(int quant,Image<int>* in, Image<unsigned char>* out, int k);
void cache();

void init(int argc, char** argv);

Image<unsigned char>* decoded = 0;
Image<unsigned char>* input = 0;
Image<int>* encoded = 0;
int n = 0;
struct timespec interval;

// 64 cos values that are needed
double _cos[64][64];


int main(int argc, char** argv)
{
    if(argc!=4)
    {
        printf("\n Usage : %s <input image> <quantization level> <sleep time> \n", argv[0]);
        return 1;
    }

    n = (atoi)(argv[2]);
    if((n<0)||(n>7))
    {
        printf("\n Invalid Quantization Value, Allowed Range is 0 to 7 \n");
        return(0);
    }

    interval.tv_sec = 0;
    interval.tv_nsec = (atoi(argv[3])*1000*1000);

    cache();
    input = new Image<unsigned char>(352,288);
    input->ReadImage(argv[1]);
    input->WritePPM("input.ppm");

    encoded = new Image<int>(352,288);
    encode(n,input,encoded);   
    encoded->WritePPM("encoded.ppm");

    decoded = new Image<unsigned char>(352,288);
    init(argc, argv);
}


void display(void)
{
    //printf("display\n");
    //glClear(GL_COLOR_BUFFER_BIT);
    //glRasterPos2i(0, 0);

    glDrawPixels(352, 288, GL_RGB,
                 GL_UNSIGNED_BYTE, decoded->m_image);
    nanosleep(&interval,NULL);
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 27:
        exit(0);
        break;
    case 'q':
        exit(0);
        break;
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        decoded->Nuke();
        decode_sequential(n,encoded,decoded);
        decoded->WritePPM("decoded.ppm");
        break;

    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        decoded->Nuke();
        for(int i=0 ; i<64 ; ++i)
        {
            printf("Using AC coeff %d\n", i);
            decode_progressive_spectral(n,encoded,decoded, i);
            display();
            
        }
        decoded->WritePPM("decoded.ppm");
        break;

    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
        decoded->Nuke();
        for(int i=0 ; i<=8 ; ++i)
        {
            printf("Using %d bits\n", i);
            decode_progressive_bit(n,encoded,decoded, i);
            display();
        }
        decoded->WritePPM("decoded.ppm");
        break;

    default:
        break;
    }
}

void init(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(352, 288);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}

void cache()
{
    double pi = 3.1415;
    for(int x=0 ; x<64 ; x++)
        for(int y=0 ; y<64 ; y++)
        {
            _cos[x][y] = cos((((2*x)+1)*y*pi)/16);
        }
}
 | GLUT_RGB);
    glutInitWindowSize(352, 288);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    glClearColor (0.0, 0.0, 0.0, 0.0);
    gl