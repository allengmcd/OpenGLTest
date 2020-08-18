#include <windows.h>
#include <GL/gl.h>
#include "curve.cpp"
#include "surfaces.cpp"

char *className = "OpenGL";
char *windowName = "OpenGL Cube";
int winX = 0, winY = 0;
int winWidth = 1000, winHeight = 700;

HDC hDC;
HGLRC hGLRC;
HPALETTE hPalette;

static bool Global_IsRunning;

GLfloat 
Global_RotationAngleY=0, 
Global_RotationAngleX=0,
Global_RotationAngleZ=0;

void
init(void)
{
    /* set viewing projection */
    glMatrixMode(GL_PROJECTION);
    glFrustum(-0.5F, 0.5F, -0.5F, 0.5F, 1.0F, 3.0F);
    
    /* position viewer */
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.0F, 0.0F, -2.0F);
    
    /* position object */
    glRotatef(30.0F, 1.0F, 0.0F, 0.0F);
    glRotatef(30.0F, 0.0F, 1.0F, 0.0F);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void
redraw(void)
{
    /* clear color and depth buffers */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    float curve[8][3] = {{-0.4f,0.0f,0.0f},{-0.2f,-0.4f,0.0f},{0.0f,0.4f,0.0f},{0.2f,0.6f,0.0f},{0.4f,0.0f,0.0f},
        {0.6f,0.4f,0.0f},{0.8f,0.2f,0.0f},{1.0f,0.2f,0.0f}};
    //float tangents[4][3] = {{1.0f,1.0f,0.0f},{0.0f,-1.0f,0.0f},{1.0f,0.1f,0.0f},{0.0f,1.0f,0.0f}};
    //float** curve = new float*[3];
    
    
    
    glPushMatrix();
    
    glTranslatef(0.0F, 0.0F, 0.0F);
    glRotatef(Global_RotationAngleY, 0.0F, 1.0F, 0.0F);
    glRotatef(Global_RotationAngleX, 1.0F, 0.0F, 0.0F);
    glRotatef(Global_RotationAngleZ, 0.0F, 0.0F, 1.0F);
    
    
    //BezierCurve(curve, 8, 100);
    //HermiteCurve(curve, tangents, 4, 100)
    
    //CatmullRomCurve(curve, 7, 100);
    
    DrawQuad();
    glPopMatrix();
    
    //CatmullRomCurve(curve, 7, 100);
    
    
    SwapBuffers(hDC);
}

void
resize(void)
{
    /* set viewport to cover the window */
    glViewport(0, 0, winWidth, winHeight);
}

void
setupPixelFormat(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  /* size */
        1,                              /* version */
        PFD_SUPPORT_OPENGL |
            PFD_DRAW_TO_WINDOW |
            PFD_DOUBLEBUFFER,               /* support double-buffering */
        PFD_TYPE_RGBA,                  /* color type */
        16,                             /* prefered color depth */
        0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
        0,                              /* no alpha buffer */
        0,                              /* alpha bits (ignored) */
        0,                              /* no accumulation buffer */
        0, 0, 0, 0,                     /* accum bits (ignored) */
        16,                             /* depth buffer */
        0,                              /* no stencil buffer */
        0,                              /* no auxiliary buffers */
        PFD_MAIN_PLANE,                 /* main layer */
        0,                              /* reserved */
        0, 0, 0,                        /* no layer, visible, damage masks */
    };
    int pixelFormat;
    
    pixelFormat = ChoosePixelFormat(hDC, &pfd);
    if (pixelFormat == 0) {
        MessageBox(WindowFromDC(hDC), "ChoosePixelFormat failed.", "Error",
                   MB_ICONERROR | MB_OK);
        exit(1);
    }
    
    if (SetPixelFormat(hDC, pixelFormat, &pfd) != TRUE) {
        MessageBox(WindowFromDC(hDC), "SetPixelFormat failed.", "Error",
                   MB_ICONERROR | MB_OK);
        exit(1);
    }
}

void
setupPalette(HDC hDC)
{
    int pixelFormat = GetPixelFormat(hDC);
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE* pPal;
    int paletteSize;
    
    DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
    
    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        paletteSize = 1 << pfd.cColorBits;
    } else {
        return;
    }
    
    pPal = (LOGPALETTE*)
        malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;
    
    /* build a simple RGB color palette */
    {
        int redMask = (1 << pfd.cRedBits) - 1;
        int greenMask = (1 << pfd.cGreenBits) - 1;
        int blueMask = (1 << pfd.cBlueBits) - 1;
        int i;
        
        for (i=0; i<paletteSize; ++i) {
            pPal->palPalEntry[i].peRed =
                (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
            pPal->palPalEntry[i].peGreen =
                (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
            pPal->palPalEntry[i].peBlue =
                (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
            pPal->palPalEntry[i].peFlags = 0;
        }
    }
    
    hPalette = CreatePalette(pPal);
    free(pPal);
    
    if (hPalette) {
        SelectPalette(hDC, hPalette, FALSE);
        RealizePalette(hDC);
    }
}

LRESULT APIENTRY
WndProc(
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam)
{
    switch (message) {
        case WM_CREATE:
        /* initialize OpenGL rendering */
        hDC = GetDC(hWnd);
        setupPixelFormat(hDC);
        setupPalette(hDC);
        hGLRC = wglCreateContext(hDC);
        wglMakeCurrent(hDC, hGLRC);
        init();
        return 0;
        case WM_DESTROY:
        /* finish OpenGL rendering */
        if (hGLRC) {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hGLRC);
        }
        if (hPalette) {
            DeleteObject(hPalette);
        }
        ReleaseDC(hWnd, hDC);
        PostQuitMessage(0);
        return 0;
        case WM_SIZE:
        /* track window size changes */
        if (hGLRC) {
            winWidth = (int) LOWORD(lParam);
            winHeight = (int) HIWORD(lParam);
            resize();
            return 0;
        }
        case WM_PALETTECHANGED:
        /* realize palette if this is *not* the current window */
        if (hGLRC && hPalette && (HWND) wParam != hWnd) {
            UnrealizeObject(hPalette);
            SelectPalette(hDC, hPalette, FALSE);
            RealizePalette(hDC);
            redraw();
            break;
        }
        break;
        case WM_QUERYNEWPALETTE:
        /* realize palette if this is the current window */
        if (hGLRC && hPalette) {
            UnrealizeObject(hPalette);
            SelectPalette(hDC, hPalette, FALSE);
            RealizePalette(hDC);
            redraw();
            return TRUE;
        }
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);
            if (hGLRC) {
                redraw();
            }
            EndPaint(hWnd, &ps);
            return 0;
        }
        break;
        case WM_CHAR:
        /* handle keyboard input */
        switch ((int)wParam) {
            case VK_ESCAPE:
            DestroyWindow(hWnd);
            return 0;
            default:
            break;
        }
        break;
        case WM_KEYDOWN:
        {
            switch(wParam)
            {
                case 0x41://A key
                Global_RotationAngleY+=2;
                
                break;
                case 0x42://B key
                case 0x43://C key
                case 0x44://D key
                Global_RotationAngleY-=2;
                
                break;
                case 0x45://E key
                Global_RotationAngleZ-=2;
                
                break;
                case 0x46://F key
                case 0x47://G key
                case 0x48://H key
                case 0x49://I key
                case 0x4A://J key
                case 0x4B://K key
                case 0x4C://L key
                case 0x4D://M key
                case 0x4E://N key
                case 0x4F://O key
                case 0x50://P key
                case 0x51://Q key
                Global_RotationAngleZ+=2;
                
                break;
                case 0x52://R key
                case 0x53://S key
                Global_RotationAngleX-=2;
                
                break;
                case 0x54://T key
                case 0x55://U key
                case 0x56://V key
                case 0x57://W key
                Global_RotationAngleX+=2;
                
                break;
                case 0x58://X key
                case 0x59://Y key
                case 0x5A://Z key
                default:
                break;
            }
        }
        break;
        default:
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY
WinMain(
        HINSTANCE hCurrentInst,
        HINSTANCE hPreviousInst,
        LPSTR lpszCmdLine,
        int nCmdShow)
{
    WNDCLASS wndClass;
    HWND hWnd;
    MSG msg;
    
    /* register window class */
    wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hCurrentInst;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = className;
    RegisterClass(&wndClass);
    
    /* create window */
    hWnd = CreateWindow(
                        className, windowName,
                        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
                        winX, winY, winWidth, winHeight,
                        NULL, NULL, hCurrentInst, NULL);
    
    /* display window */
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    Global_IsRunning = true;
    
    while(Global_IsRunning)
    {
        
        
        
        
        MSG Message = {  };
        while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
        {
            if(Message.message == WM_QUIT)
            {
                Global_IsRunning= false;
            }
            
            TranslateMessage(&Message);
            DispatchMessageA(&Message);
        }
        
        redraw();
        
        BitBlt(hDC , 0 , 0 , 1024,768 , hDC , 0 , 0 ,SRCCOPY);
        
    }
    
    return 0;
}