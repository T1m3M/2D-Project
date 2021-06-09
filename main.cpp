#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>
#include <iostream>

using namespace std;

/* Colors */
#define BLACK   RGB(0,0,0)
#define WHITE   RGB(255,255,255)
#define RED     RGB(255,0,0)
#define ORANGE  RGB(255,128,0)
#define YELLOW  RGB(255,255,0)
#define GREEN   RGB(0,255,0)
#define BLUE    RGB(0,0,255)
#define INDIGO  RGB(75,0,130)
#define VIOLET  RGB(143,0,255)

/* Menu handles */
#define MI_CLEAR 1001
#define MI_SAVE 1002
#define MI_LOAD 1003

#define MI_BLACK_COLOR 1004
#define MI_WHITE_COLOR 1005
#define MI_RED_COLOR 1006
#define MI_ORANGE_COLOR 1007
#define MI_YELLOW_COLOR 1008
#define MI_GREEN_COLOR 1009
#define MI_BLUE_COLOR 1010
#define MI_INDIGO_COLOR 1011
#define MI_VIOLET_COLOR 1012

#define MI_LINE_DDA 1013
#define MI_LINE_MID 1014
#define MI_LINE_PARA 1015

#define MI_CIRCLE_DIRECT 1016
#define MI_CIRCLE_POLAR 1017
#define MI_CIRCLE_ITERATIVE 1018
#define MI_CIRCLE_MID 1019
#define MI_CIRCLE_MOD_MID 1020

#define MI_FILL_Q1 1021
#define MI_FILL_Q2 1022
#define MI_FILL_Q3 1023
#define MI_FILL_Q4 1024

#define MI_ELLIPSE_DIRECT 1025
#define MI_ELLIPSE_POLAR 1026

#define MI_CLIP_POINT 1027
#define MI_CLIP_LINE 1028
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

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
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) (COLOR_WINDOW+1);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("2D Project"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           900,                 /* The programs width */
           700,                 /* and height in pixels */
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

/* Global variables */
COLORREF color = BLACK;
enum algorithms{line_DDA, line_mid, line_para,
                circle_dir, circle_polar, circle_iter, circle_mid, circle_mod_mid,
                fill_q1, fill_q2, fill_q3, fill_q4,
                ellipse_dir, ellipse_polar,
                clip_point, clip_line};
int alg = line_DDA;
int click_count = 0;

/*
* Bahaa El-Deen Osama .
*/


int r;
int x_1,y_1,x_2,y_2;

/// Circle.

void Swap(int& x,int& y){
int tmp=x;
x=y;
y=tmp;
}
int Round(double x){ return (int)(x+0.5); }
void Draw8points(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc+y, color);
    SetPixel(hdc, xc-x, yc+y, color);
    SetPixel(hdc, xc+x, yc-y, color);
    SetPixel(hdc, xc-x, yc-y, color);
    SetPixel(hdc, xc-y, yc+x, color);
    SetPixel(hdc, xc+y, yc-x, color);
    SetPixel(hdc, xc+y, yc+x, color);
    SetPixel(hdc, xc-y, yc-x, color);
}
void Draw4points(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc,xc-x,yc-y,color);
    SetPixel(hdc,xc+x,yc-y,color);
    SetPixel(hdc,xc-x,yc+y,color);
    SetPixel(hdc,xc+x,yc+y,color);
}


int getRadius(int x1, int y1, int x2, int y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}


/// (1) MidPoint.
void midpoint(HDC hdc,int xc,int yc,int r,COLORREF color)
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
        Draw8points(hdc,x,y,xc,yc,color);
    }

}
void Circle_MidPoint(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
                r = getRadius(x1,y1,x2,y2);
                midpoint(hdc, x1, y1, r,color);
}



/// (2) Iterative Polar

void IterativePolar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
    double x=R,y=0;
    double dtheta=1.0/R;
    double cdtheta=cos(dtheta);
    double sdtheta=sin(dtheta);
    Draw8points(hdc,xc,yc,R,0,color);

    while(x>y)
    {
        double x1 = x*cdtheta-y*sdtheta;
        y = x*sdtheta+y*cdtheta;
        x = x1;
        Draw8points(hdc,Round(x),Round(y),xc,yc,color);
    }
}

void Circle_IterativePolar(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    r =  getRadius(x1,y1,x2,y2);
    IterativePolar(hdc,x1,y1,r,color);

}


/// (3) Direct.
void Direct(HDC hdc,int xc,int yc, int R,COLORREF color)
{
        int x=0,y=R;
        int R2=R*R;
        Draw8points(hdc,xc,yc,x,y,color);
        while(x<y)
        {
            x++;
            y=round(sqrt((double)(R2-x*x)));
            Draw8points(hdc,x,y,xc,yc,color);
        }
}

void Circle_Direct(HDC hdc , int x1,int y1,int x2,int y2,COLORREF color)
{
    int r = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
    Direct(hdc,x1,y1,r,color);

}



/// (4) Polar
void Polar(HDC hdc,int xc,int yc, int R,COLORREF color)
{
        int x=R,y=0;
        double theta=0,dtheta=1.0/R;
        Draw8points(hdc,xc,yc,x,y,color);
        while(x>y)
        {
            theta+=dtheta;
            x=round(R*cos(theta));
            y=round(R*sin(theta));
            Draw8points(hdc,x,y,xc,yc,color);
        }
}

void Circle_Polar(HDC hdc , int x1,int y1,int x2,int y2,COLORREF color)
{
    int r = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
    Polar(hdc,x1,y1,r,color);

}



/// (5) Modified Midpoint
void modified_midpoint(HDC hdc,int xc,int yc, int R,COLORREF color)
{
        int x=0,y=R;
        int d=1-R;
        Draw8points(hdc,xc,yc,x,y,color);
        while(x<y)
        {
            if(d<0)
            d+=2*x+2;
        else
        {
             d+=2*(x-y)+5;
              y--;
        }
        x++;
        Draw8points(hdc,x,y,xc,yc,color);
        }
}

void Circle_modified_midpoint(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
{
    int r = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
    modified_midpoint(hdc,x1,y1,r,color);
    /// midpoint(hdc,x1,y1,r,color);

}
///-----------------------------------------------------------------------------------------------------------------------------------


/// Line.

/// (1) DDA.
void DDA(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
{
    int dx=xe-xs;
    int dy=ye-ys;
    SetPixel(hdc,xs,ys,color);
    if(abs(dx)>=abs(dy))
    {
        int x=xs,xinc= dx>0?1:-1;
        double y=ys,yinc=(double)dy/dx*xinc;
        while(x!=xe)
        {
            x+=xinc;
            y+=yinc;
            SetPixel(hdc,x,round(y),color);
        }

    }

    else
    {
        int y=ys,yinc= dy>0?1:-1;
        double x=xs,xinc=(double)dx/dy*yinc;
        while(y!=ye)
        {
            x+=xinc;
            y+=yinc;
            SetPixel(hdc,round(x),y,color);
        }
    }
}


/// (2) Parametric .
void parametric(HDC hdc, double x1, double y1, double x2, double y2, COLORREF color) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    for (double t = 0; t < 1; t += 0.001) {
        int x = x1 + (dx * t);
        int y = y1 + (dy * t);
        SetPixel(hdc, x, y, color);
    }
}


/// (3) MidPoint.
void Swap2(int& xs, int& ys, int& xe, int& ye)
{
    int temp = xs;
    xs = xe;
    xe = temp;
    temp = ys;
    ys = ye;
    ye = temp;
}
void LineMidpoint (HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int d, d1, d2;

    if(dx == 0 && dy == 0)
    {
        SetPixel(hdc, x1, y1, c);
        return;
    }
    if (abs(dy) < abs(dx))
    {
        if (x1 > x2)
        {
            Swap2(x1, y1, x2, y2);
            dy *= -1;
            dx *= -1;
        }
        int x = x1, y = y1;
        SetPixel(hdc, x, y, c);

        if (dy > 0) //f(x+1,y+0.5)
        {
            d = dx - 2 * dy;
            d1 = -2 * dy;
            d2 = 2 * (dx - dy);
        }
        else //f(x-1,y+0.5)
        {
            d = dx + 2 * dy;
            d1 = 2 * dy;
            d2 = 2 * (dx + dy);
        }

        while (x < x2)
        {
            x++;
            if (d > 0)
            {
                d += d1;
            }
            else
            {
                d += d2;

                if (dy > 0)
                    y++;
                else
                    y--;
            }
            SetPixel(hdc, x, y, c);
        }
    }
    else if (abs(dx) < abs(dy))
    {
        if (y1 > y2)
        {
            Swap2(x1, y1, x2, y2);
            dy *= -1;
            dx *= -1;
        }
        int x = x1, y = y1;
        SetPixel(hdc, x, y, c);

        if (dx > 0)		//f(x+0.5,y+1)
        {
            d = dy - 2 * dx;
            d1 = -2 * dx;
            d2 = 2 * (dy - dx);
        }
        else //f(x-0.5,y+1)
        {
            d = dy + 2 * dx;
            d1 = 2 * dx;
            d2 = 2 * (dy + dx);
        }

        while (y < y2)
        {
            y++;
            if (d > 0)
            {
                d += d1;
            }
            else
            {
                d += d2;
                if (dx > 0)
                    x++;
                else
                    x--;
            }
            SetPixel(hdc, x, y, c);
        }
    }
}



/// Ellipse
/// (1) Polar
void polarEllipse(HDC hdc, int x1, int y1, int x_2, int y_2, COLORREF c)
{

    for(double theta = 0 ; theta<6.28 ; theta += 0.001)
    {
        int x = Round(x1 + x_1 * cos(theta));
        int y = Round(y1 + y_2 * sin(theta));
        SetPixel(hdc, x, y, c);
    }
}

/// (2) Direct
void DirectEllipse(HDC hdc,int x_1, int y_1, int x_2, int y_2, COLORREF c)
{
    float x=x_2;
    int y=0;
    while(x>=0)
    {
       Draw4points(hdc,x_1,y_1,x,y,color);
        x-=0.01;
        y=y_2*sqrt(1-((x*x)/(x_2*x_2)));
    }
}

/// ************************************************************* _End my Work_ **********************************************************************

/// ******* Tawaty *******

void fill_q1(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc-y, color);
    parametric(hdc, xc, yc, xc+x, yc-y,  color);
    SetPixel(hdc, xc+y, yc-x, color);
    parametric(hdc, xc, yc, xc+y, yc-x,  color);
}
void fill_q2(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc-x, yc-y, color);
    parametric(hdc, xc, yc, xc-x, yc-y,  color);
    SetPixel(hdc, xc-y, yc-x, color);
    parametric(hdc, xc, yc, xc-y, yc-x,  color);
}
void fill_q3(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc-x, yc+y, color);
    parametric(hdc, xc, yc, xc-x, yc+y,  color);
    SetPixel(hdc, xc-y, yc+x, color);
    parametric(hdc, xc, yc, xc-y, yc+x,  color);
}
void fill_q4(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc+y, color);
    parametric(hdc, xc, yc, xc+x, yc+y,  color);
    SetPixel(hdc, xc+y, yc+x, color);
    parametric(hdc, xc, yc, xc+y, yc+x,  color);
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


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);

    switch (message)                  /* handle the messages */
    {
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        case WM_CREATE:                        /* Create the menu bar */
        {
            HMENU hMenubar = CreateMenu();
            HMENU hFile = CreateMenu();
            HMENU hColor = CreateMenu();
            HMENU hLine = CreateMenu();
            HMENU hCircle = CreateMenu();
            HMENU hFill = CreateMenu();
            HMENU hEllipse = CreateMenu();
            HMENU hClip = CreateMenu();

            /* Menus */
            AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, "File");
            AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hColor, "Color");
            AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hLine, "Line");
            AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hCircle, "Circle");
            AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFill, "Fill");
            AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hEllipse, "Ellipse");
            AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hClip, "Clip");

            /* File sub-menus */
            AppendMenu(hFile, MF_STRING, MI_CLEAR, "Clear");
            AppendMenu(hFile, MF_STRING, MI_SAVE, "Save");
            AppendMenu(hFile, MF_STRING, MI_LOAD, "Load");

            /* Color sub-menus */
            AppendMenu(hColor, MF_STRING, MI_BLACK_COLOR, "Black");
            AppendMenu(hColor, MF_STRING, MI_WHITE_COLOR, "White");
            AppendMenu(hColor, MF_STRING, MI_RED_COLOR, "Red");
            AppendMenu(hColor, MF_STRING, MI_ORANGE_COLOR, "Orange");
            AppendMenu(hColor, MF_STRING, MI_YELLOW_COLOR, "Yellow");
            AppendMenu(hColor, MF_STRING, MI_GREEN_COLOR, "Green");
            AppendMenu(hColor, MF_STRING, MI_BLUE_COLOR, "Blue");
            AppendMenu(hColor, MF_STRING, MI_INDIGO_COLOR, "Indigo");
            AppendMenu(hColor, MF_STRING, MI_VIOLET_COLOR, "Violet");

            /* Line sub-menus */
            AppendMenu(hLine, MF_STRING, MI_LINE_DDA, "DDA");
            AppendMenu(hLine, MF_STRING, MI_LINE_MID, "Midpoint");
            AppendMenu(hLine, MF_STRING, MI_LINE_PARA, "parametric");

            /* Circle sub-menus */
            AppendMenu(hCircle, MF_STRING, MI_CIRCLE_DIRECT, "Direct");
            AppendMenu(hCircle, MF_STRING, MI_CIRCLE_POLAR, "Polar");
            AppendMenu(hCircle, MF_STRING, MI_CIRCLE_ITERATIVE, "Iterative Polar");
            AppendMenu(hCircle, MF_STRING, MI_CIRCLE_MID, "Midpoint");
            AppendMenu(hCircle, MF_STRING, MI_CIRCLE_MOD_MID, "Modified Midpoint");

            /* Fill sub-menus */
            AppendMenu(hFill, MF_STRING, MI_FILL_Q1, "Quarter 1");
            AppendMenu(hFill, MF_STRING, MI_FILL_Q2, "Quarter 2");
            AppendMenu(hFill, MF_STRING, MI_FILL_Q3, "Quarter 3");
            AppendMenu(hFill, MF_STRING, MI_FILL_Q4, "Quarter 4");

            /* Ellipse sub-menus */
            AppendMenu(hEllipse, MF_STRING, MI_ELLIPSE_DIRECT, "Direct");
            AppendMenu(hEllipse, MF_STRING, MI_ELLIPSE_POLAR, "Polar");

            /* Clip sub-menus */
            AppendMenu(hClip, MF_STRING, MI_CLIP_POINT, "Point");
            AppendMenu(hClip, MF_STRING, MI_CLIP_LINE, "Line");

            SetMenu(hwnd, hMenubar);

            break;
        }

        case WM_LBUTTONUP:              /* actions when left click is pressed */
        {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);



            switch (alg)
            {
                case line_DDA:
                {
                     if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;
                        click_count++;
                    }
                    else
                    {
                        x_2 = x;
                        y_2 = y;
                        DDA(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case line_mid:
                {
                    if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;
                        click_count++;
                    }
                    else
                    {
                         x_2 = x;
                         y_2 = y;
                        LineMidpoint(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case line_para:
                {
                     if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;
                        click_count++;
                    }
                    else
                    {
                        x_2 = x;
                        y_2 = y;
                        parametric(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_dir:
                {
                    if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;
                        click_count++;
                    }
                    else
                    {
                        x_2 = x;
                        y_2 = y;
                        Circle_Direct(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_polar:
                {
                    if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;

                        click_count++;
                    }
                    else
                    {
                        x_2 = x;
                        y_2 = y;
                        Circle_Polar(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_iter:
                {
                    if (click_count == 0)
                    {
                         x_1 = x;
                         y_1 = y;
                        click_count++;
                    }
                    else
                    {
                         x_2 = x;
                         y_2 = y;

                        Circle_IterativePolar(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_mid:
                {
                    if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;
                        click_count++;
                    }
                    else
                    {
                         x_2 = x;
                         y_2 = y;
                        Circle_MidPoint(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);

                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_mod_mid:
                {
                    if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;
                        click_count++;
                    }
                    else
                    {
                         x_2 = x;
                         y_2 = y;
                        Circle_modified_midpoint(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);

                        click_count = 0; // reset click count
                    }
                    break;
                }

                case fill_q1:
                {
                    // code for quarter fill
                    break;
                }

                case fill_q2:
                {
                    // code for quarter fill
                    break;
                }

                case fill_q3:
                {
                    // code for quarter fill
                    break;
                }

                case fill_q4:
                {

                    // code for quarter fill
                    break;
                }

                case ellipse_dir:
                {
                     if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;
                        click_count++;
                    }
                    else
                    {
                         x_2 = x;
                         y_2 = y;
                        polarEllipse(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);

                        click_count = 0; // reset click count
                    }
                    break;
                }

                case ellipse_polar:
                {
                    if (click_count == 0)
                    {
                        x_1 = x;
                        y_1 = y;
                        click_count++;
                    }
                    else
                    {
                         x_2 = x;
                         y_2 = y;
                        polarEllipse(hdc ,x_1 ,y_1 ,x_2 ,y_2,color);

                        click_count = 0; // reset click count
                    }
                    break;
                }

                case clip_point:
                {
                    if (click_count == 0)
                    {
                        // code for start point of rectangular
                        click_count++;
                    }
                    else if (click_count == 1)
                    {
                        // code for end point of rectangular
                        click_count++;
                    }
                    else
                    {
                        // code for points
                        // resets by default when changing the algorithm (many points)
                    }
                    break;
                }

                case clip_line:
                {
                    if (click_count == 0)
                    {
                        // code for start point of rectangular
                    }
                    else if (click_count == 1)
                    {
                        // code for end point of rectangular
                    }
                    else if (click_count % 2 == 0)
                    {
                        // code for start point of lines
                    }
                    else
                    {
                        // code for end point of lines
                        // resets by default when changing the algorithm (many lines)
                    }

                    click_count++;
                    break;
                }

                default:
                    break;
            }
        }

        case WM_COMMAND:                /* bind menus to actions */
        {

            switch (LOWORD(wParam))
            {
                /************** File sub-menus **************/
                case MI_CLEAR:
                {
                    /* Write clear code here */
                    std::cout << "Data is cleared!\n\n";
                    break;
                }

                case MI_SAVE:
                {
                    std::cout << "Saving the data...\n";
                    /* Write save code here */
                    std::cout << "Data is saved!\n\n";
                    break;
                }

                case MI_LOAD:
                {
                    std::cout << "Loading the data...\n";
                    /* Write load code here */
                    std::cout << "Data is loaded!\n\n";
                    break;
                }

                /************** Color sub-menus **************/
                case MI_BLACK_COLOR:
                {
                    color = BLACK;
                    std::cout << "Color changed to black!\n\n"; break;
                }
                case MI_WHITE_COLOR:
                {
                    color = WHITE;
                    std::cout << "Color changed to white!\n\n"; break;
                }
                case MI_RED_COLOR:
                {
                    color = RED;
                    std::cout << "Color changed to red!\n\n"; break;
                }
                case MI_ORANGE_COLOR:
                {
                    color = ORANGE;
                    std::cout << "Color changed to orange!\n\n"; break;
                }
                case MI_YELLOW_COLOR:
                {
                    color = YELLOW;
                    std::cout << "Color changed to yellow!\n\n"; break;
                }
                case MI_GREEN_COLOR:
                {
                    color = GREEN;
                    std::cout << "Color changed to green!\n\n"; break;
                }
                case MI_BLUE_COLOR:
                {
                    color = BLUE;
                    std::cout << "Color changed to blue!\n\n"; break;
                }
                case MI_INDIGO_COLOR:
                {
                    color = INDIGO;
                    std::cout << "Color changed to indigo!\n\n"; break;
                }
                case MI_VIOLET_COLOR:
                {
                    color = VIOLET;
                    std::cout << "Color changed to violet!\n\n"; break;
                }

                /************** Line sub-menus **************/
                case MI_LINE_DDA:
                {
                    click_count = 0;
                    alg = line_DDA;
                    std::cout << "algorithm changed to line DDA!\n\n"; break;
                }

                case MI_LINE_MID:
                {
                    click_count = 0;
                    alg = line_mid;
                    std::cout << "algorithm changed to line midpoint!\n\n"; break;
                }

                case MI_LINE_PARA:
                {
                    click_count = 0;
                    alg = line_para;
                    std::cout << "algorithm changed to line parametric!\n\n"; break;
                }

                /************** Circle sub-menus **************/
                case MI_CIRCLE_DIRECT:
                {
                    click_count = 0;
                    alg = circle_dir;
                    std::cout << "algorithm changed to circle direct!\n\n"; break;
                }

                case MI_CIRCLE_POLAR:
                {
                    click_count = 0;
                    alg = circle_polar;
                    std::cout << "algorithm changed to circle polar!\n\n"; break;
                }

                case MI_CIRCLE_ITERATIVE:
                {
                    click_count = 0;
                    alg = circle_iter;
                    std::cout << "algorithm changed to circle iterative!\n\n"; break;
                }

                case MI_CIRCLE_MID:
                {
                    click_count = 0;
                    alg = circle_mid;
                    std::cout << "algorithm changed to circle midpoint!\n\n"; break;
                }

                case MI_CIRCLE_MOD_MID:
                {
                    click_count = 0;
                    alg = circle_mod_mid;
                    std::cout << "algorithm changed to circle modified midpoint!\n\n"; break;
                }

                /************** Fill sub-menus **************/
                case MI_FILL_Q1:
                {
                    alg = fill_q1;
                    std::cout << "algorithm changed to fill circle quarter 1!\n\n"; break;
                }

                case MI_FILL_Q2:
                {
                    alg = fill_q2;
                    std::cout << "algorithm changed to fill circle quarter 2!\n\n"; break;
                }

                case MI_FILL_Q3:
                {
                    alg = fill_q3;
                    std::cout << "algorithm changed to fill circle quarter 3!\n\n"; break;
                }

                case MI_FILL_Q4:
                {
                    alg = fill_q4;
                    std::cout << "algorithm changed to fill circle quarter 4!\n\n"; break;
                }

                /************** Ellipse sub-menus **************/
                case MI_ELLIPSE_DIRECT:
                {
                    click_count = 0;
                    alg = ellipse_dir;
                    std::cout << "algorithm changed to ellipse direct!\n\n"; break;
                }

                case MI_ELLIPSE_POLAR:
                {
                    click_count = 0;
                    alg = ellipse_polar;
                    std::cout << "algorithm changed to ellipse polar!\n\n"; break;
                }

                /************** Clip sub-menus **************/
                case MI_CLIP_POINT:
                {
                    click_count = 0;
                    alg = clip_point;
                    std::cout << "algorithm changed to clip point!\n\n"; break;
                }

                case MI_CLIP_LINE:
                {
                    click_count = 0;
                    alg = clip_line;
                    std::cout << "algorithm changed to clip line!\n\n"; break;
                }

                default:
                    break;
            }
        }
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
