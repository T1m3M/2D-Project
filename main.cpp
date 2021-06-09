#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>
#include <iostream>
//#include <bits/stdc++.h>

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

#define MAXENTRIES 600
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


/** Function filed */

/// Point clipping
void clip_point_algo(HDC hdc, int x,int y,int xleft,int ytop,int xright,int ybottom,COLORREF color)
{
if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)

SetPixel(hdc,x,y,color);
}

/// line clipping for a rectangle shape
// union structure for the 4 bits
union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};
OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode obj;
    obj.All = 0;
    if(x > xright)
      obj.right = 1;
    else if(x < xleft)
      obj.left = 1;
    if(y < ytop)
      obj.top = 1;
    else if(y > ybottom)
      obj.bottom = 1;
    return obj;
}
void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}
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
void clip_line_algo(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs, y1=ys, x2=xe, y2=ye;
    OutCode out1 = GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2 = GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {

        //MoveToEx(hdc,round(x1),round(y1),NULL);
        //LineTo(hdc,round(x2),round(y2));
        para_line(hdc, round(x1), round(y1), round(x2), round(y2), color);
    }
}
/** Filling a quarter of a circle with lines */
void fill_Q1(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc-y, color);
    para_line(hdc, xc, yc, xc+x, yc-y,  color);
    SetPixel(hdc, xc+y, yc-x, color);
    para_line(hdc, xc, yc, xc+y, yc-x,  color);
}
void fill_Q2(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc-x, yc-y, color);
    para_line(hdc, xc, yc, xc-x, yc-y,  color);
    SetPixel(hdc, xc-y, yc-x, color);
    para_line(hdc, xc, yc, xc-y, yc-x,  color);
}
void fill_Q3(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc-x, yc+y, color);
    para_line(hdc, xc, yc, xc-x, yc+y,  color);
    SetPixel(hdc, xc-y, yc+x, color);
    para_line(hdc, xc, yc, xc-y, yc+x,  color);
}
void fill_Q4(HDC hdc,int x,int y,int xc,int yc,COLORREF color)
{
    SetPixel(hdc, xc+x, yc+y, color);
    para_line(hdc, xc, yc, xc+x, yc+y,  color);
    SetPixel(hdc, xc+y, yc+x, color);
    para_line(hdc, xc, yc, xc+y, yc+x,  color);
}
void filling_algo(HDC hdc,int xc,int yc,int x_2, int y_2, int q, COLORREF color)
{
      int rr = sqrt(pow((x_2-xc), 2) + pow((y_2-yc), 2));
    int x=0;
    int y=rr;
    double d=1-rr;
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
            fill_Q1(hdc,x,y,xc,yc,color);
        else if(q == 2)
            fill_Q2(hdc,x,y,xc,yc,color);
        else if(q == 3)
            fill_Q3(hdc,x,y,xc,yc,color);
        else
            fill_Q4(hdc,x,y,xc,yc,color);
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
            int x_1, y_1, x_2, y_2;
            int X_left, Y_top, X_right, Y_bottom;
            int X_start, Y_start, X_end, Y_end;

            switch (alg)
            {
                case line_DDA:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else
                    {
                        // code for end point
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case line_mid:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else
                    {
                        // code for end point
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case line_para:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else
                    {
                        // code for end point
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_dir:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else
                    {
                        // code for end point
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_polar:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else
                    {
                        // code for end point
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_iter:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else
                    {
                        // code for end point
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_mid:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else
                    {
                        // code for end point
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case circle_mod_mid:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else
                    {
                        // code for end point
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case fill_q1:
                {
                      x_1 = x; y_1 = y;
                      x_2 = x; y_2 = y;
                    // code for quarter fill
                    //rr = sqrt(pow((x_2-x_1), 2) + pow((y_2-y_1), 2));
                    filling_algo(hdc, x_1, y_1,x_2 ,y_2 ,1 ,color);
                    break;
                }
                case fill_q2:
                {
                      x_1 = x; y_1 = y;
                      x_2 = x; y_2 = y;
                      filling_algo(hdc, x_1, y_1, x_2 ,y_2 ,2 ,color);
                    // code for quarter fill
                    break;
                }

                case fill_q3:
                {
                      x_1 = x; y_1 = y;
                      x_2 = x; y_2 = y;
                      filling_algo(hdc, x_1, y_1, x_2 ,y_2,3 ,color);
                    // code for quarter fill
                    break;
                }

                case fill_q4:
                {
                      x_1 = x; y_1 = y;
                      x_2 = x; y_2 = y;
                      filling_algo(hdc, x_1, y_1, x_2 ,y_2 ,4 ,color);
                    // code for quarter fill
                    break;
                }
                case ellipse_dir:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else if (click_count == 1)
                    {
                        // code for point 1
                        click_count++;
                    }
                    else
                    {
                        // code for point 2
                        click_count = 0; // reset click count
                    }
                    break;
                }

                case ellipse_polar:
                {
                    if (click_count == 0)
                    {
                        // code for start point
                        click_count++;
                    }
                    else if (click_count == 1)
                    {
                        // code for point 1
                        click_count++;
                    }
                    else
                    {
                        // code for point 2
                        click_count = 0; // reset click count
                    }
                    break;
                }


                case clip_point:
                {
                    if (click_count == 0)
                    {
                        // code for start point of rectangular
                        X_left=LOWORD(lParam);
                        Y_top=HIWORD(lParam);
                        click_count++;
                    }
                    else if (click_count == 1)
                    {
                        // code for end point of rectangular
                        X_right=LOWORD(lParam);
                        Y_bottom=HIWORD(lParam);
                        Rectangle(hdc, X_left,Y_top,X_right,Y_bottom);
                        click_count++;
                    }
                    else
                    {
                        // code for points
                        //x1 = x;
                        //y1 = y;
                        x_1=LOWORD(lParam);
                        y_1=HIWORD(lParam);
                        clip_point_algo(hdc, x_1, y_1, X_left,Y_top,X_right,Y_bottom, color);
                        // resets by default when changing the algorithm (many points)
                    }
                    break;
                }

                case clip_line:
                {
                    if (click_count == 0)
                    {
                        // code for start point of rectangular
                        X_left=LOWORD(lParam);
                        Y_top=HIWORD(lParam);
                    }
                    else if (click_count == 1)
                    {
                        // code for end point of rectangular
                        X_right=LOWORD(lParam);
                        Y_bottom=HIWORD(lParam);
                        Rectangle(hdc, X_left,Y_top,X_right,Y_bottom);
                    }
                    else if (click_count % 2 == 0)
                    {
                        // code for start point of lines
                        X_start=LOWORD(lParam);
                        Y_start=HIWORD(lParam);
                    }
                    else
                    {
                        // code for end point of lines
                        X_end=LOWORD(lParam);
                        Y_end=HIWORD(lParam);
                        clip_line_algo(hdc,X_start,Y_start,X_end,Y_end,X_left,Y_top,X_right,Y_bottom);
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
