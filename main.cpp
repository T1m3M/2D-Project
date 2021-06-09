#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif
#include <tchar.h>
#include <windows.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
using namespace std;

#define MAXENTRIES 600
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


/** Parametric Algorithm to drew colored Line */
void para_line(HDC hdc,int x1,int y1,int x2,int y2, COLORREF color){
double dx=x2-x1;
double dy=y2-y1;
int x, y;
for(double t=0;t<1;t+=0.001){
     x = x1+(dx*t);
     y = y1+(dy*t);
    SetPixel(hdc,x,y,color);
}
}
void fill_q1(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc-y, color);
    para_line(hdc, xc, yc, xc+x, yc-y,  color);
    SetPixel(hdc, xc+y, yc-x, color);
    para_line(hdc, xc, yc, xc+y, yc-x,  color);
}
void fill_q2(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc-x, yc-y, color);
    para_line(hdc, xc, yc, xc-x, yc-y,  color);
    SetPixel(hdc, xc-y, yc-x, color);
    para_line(hdc, xc, yc, xc-y, yc-x,  color);
}
void fill_q3(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc-x, yc+y, color);
    para_line(hdc, xc, yc, xc-x, yc+y,  color);
    SetPixel(hdc, xc-y, yc+x, color);
    para_line(hdc, xc, yc, xc-y, yc+x,  color);
}
void fill_q4(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc+y, color);
    para_line(hdc, xc, yc, xc+x, yc+y,  color);
    SetPixel(hdc, xc+y, yc+x, color);
    para_line(hdc, xc, yc, xc+y, yc+x,  color);
}

void filling_algo(HDC hdc,int xc,int yc,int r,int q, COLORREF color)
{
    int x=0;
    int y=r;
    double d=1-r;
    while(x<y){
        if(d<=0){
            d=d+2*x+3;
            x++;
        }
        else{
            d=d+2*(x-y)+5;
            x++;
            y--;
        }
        if(q == 1)
            fill_q1(hdc,x,y,xc,yc,color);
        else if(q == 2)
            fill_q2(hdc,x,y,xc,yc,color);
        else if(q == 3)
            fill_q3(hdc,x,y,xc,yc,color);
        else
            fill_q4(hdc,x,y,xc,yc,color);
    }
}

int cunter=0;
int rr, x_1, y_1, x_2, y_2;



int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               544,                 /* The programs width */
               375,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/*  This function is called by the Windows function DispatchMessage()  */
/** WOo */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
HDC hdc=GetDC(hwnd);

    switch (message)                  /* handle the messages */
    {
    case WM_LBUTTONDBLCLK:

        if(cunter==0)
        {
            x_1 = LOWORD(lParam);
            y_1 = HIWORD(lParam);
            cunter++;
        }
        else if(cunter==1)
        {
            x_2 = LOWORD(lParam);
            y_2 = HIWORD(lParam);
            rr = sqrt(pow((x_2-x_1), 2) + pow((y_2-y_1), 2));
            filling_algo(hdc, x_1, y_1, rr,1 ,RGB(0,0,50));
            filling_algo(hdc, x_1, y_1, rr,3 ,RGB(90,200,200));
            filling_algo(hdc, x_1, y_1, rr,2 ,RGB(255,0,50));
            filling_algo(hdc, x_1, y_1, rr,4 ,RGB(200,100,0));
            cunter=0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
