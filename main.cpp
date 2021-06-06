#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>
#include <iostream>

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

# define MI_LINE_DDA 1013
# define MI_LINE_MID 1014
# define MI_LINE_PARA 1015

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
            AppendMenu(hCircle, MF_STRING, NULL, "Direct");
            AppendMenu(hCircle, MF_STRING, NULL, "Polar");
            AppendMenu(hCircle, MF_STRING, NULL, "Iterative Polar");
            AppendMenu(hCircle, MF_STRING, NULL, "Midpoint");
            AppendMenu(hCircle, MF_STRING, NULL, "Modified Midpoint");

            /* Fill sub-menus */
            AppendMenu(hFill, MF_STRING, NULL, "Quarter 1");
            AppendMenu(hFill, MF_STRING, NULL, "Quarter 2");
            AppendMenu(hFill, MF_STRING, NULL, "Quarter 3");
            AppendMenu(hFill, MF_STRING, NULL, "Quarter 4");

            /* Ellipse sub-menus */
            AppendMenu(hEllipse, MF_STRING, NULL, "Direct");
            AppendMenu(hEllipse, MF_STRING, NULL, "Polar");

            /* Clip sub-menus */
            AppendMenu(hClip, MF_STRING, NULL, "Point");
            AppendMenu(hClip, MF_STRING, NULL, "Line");

            SetMenu(hwnd, hMenubar);

            break;
        }

        case WM_COMMAND:                /* bind menus to actions */
        {
            switch (LOWORD(wParam))
            {
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

                default:
                    break;
            }
        }
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
