#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include<math.h>
#include <iostream>

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


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
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
            AppendMenu(hFile, MF_STRING, NULL, "Clear");
            AppendMenu(hFile, MF_STRING, NULL, "Save");
            AppendMenu(hFile, MF_STRING, NULL, "Load");

            /* Color sub-menus */
            AppendMenu(hColor, MF_STRING, NULL, "Black");
            AppendMenu(hColor, MF_STRING, NULL, "White");
            AppendMenu(hColor, MF_STRING, NULL, "Red");
            AppendMenu(hColor, MF_STRING, NULL, "Orange");
            AppendMenu(hColor, MF_STRING, NULL, "Yellow");
            AppendMenu(hColor, MF_STRING, NULL, "Green");
            AppendMenu(hColor, MF_STRING, NULL, "Blue");
            AppendMenu(hColor, MF_STRING, NULL, "Indigo");
            AppendMenu(hColor, MF_STRING, NULL, "Violet");

            /* Line sub-menus */
            AppendMenu(hLine, MF_STRING, NULL, "DDA");
            AppendMenu(hLine, MF_STRING, NULL, "Midpoint");
            AppendMenu(hLine, MF_STRING, NULL, "parametric");

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

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
