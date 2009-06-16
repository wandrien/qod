define   IDI_APPLICATION         $00007F00                // default icon
define   IDI_WINLOGO             $00007F05

define   IDC_ARROW               $00007F00                // arrow cusror
define   IDC_WAIT                $00007F02                // hourglass cusror
define   IDC_CROSS               $00007F03                // cross cusror

define   CS_VREDRAW              $00000001                // class styles
define   CS_HREDRAW              $00000002

define   WS_MAXIMIZEBOX          $00010000                // window styles
define   WS_TABSTOP              $00010000
define   WS_MINIMIZEBOX          $00020000
define   WS_GROUP                $00020000
define   WS_THICKFRAME           $00040000
define   WS_SYSMENU              $00080000
define   WS_HSCROLL              $00100000
define   WS_VSCROLL              $00200000
define   WS_DLGFRAME             $00400000
define   WS_BORDER               $00800000
define   WS_CAPTION              $00C00000                // WS_BORDER | WS_DLGFRAME
define   WS_MAXIMIZE             $01000000
define   WS_CLIPCHILDREN         $02000000
define   WS_CLIPSIBLINGS         $04000000
define   WS_DISABLED             $08000000
define   WS_VISIBLE              $10000000
define   WS_MINIMIZE             $20000000
define   WS_CHILD                $40000000

define   ES_LEFT                 $00000000                // edit styles
define   ES_CENTER               $00000001
define   ES_RIGHT                $00000002
define   ES_MULTILINE            $00000004
define   ES_UPPERCASE            $00000008
define   ES_LOWERCASE            $00000010
define   ES_PASSWORD             $00000020
define   ES_AUTOVSCROLL          $00000040
define   ES_AUTOHSCROLL          $00000080
define   ES_NOHIDESEL            $00000100
define   ES_OEMCONVERT           $00000400
define   ES_READONLY             $00000800
define   ES_WANTRETURN           $00001000

define   MF_STRING               $00000000                // menu flags
define   MF_POPUP                $00000010
define   MF_SEPARATOR            $00000800

define   SW_SHOWNORMAL           $00000001                // ShowWindow flag

define   WM_DESTROY              $00000002                // messages
define   WM_SIZE                 $00000005
define   WM_PAINT                $0000000F
define   WM_ERASEBKGND           $00000014
define   WM_COMMAND              $00000111

define   SIZE_MINIMIZED          $00000001

define   MB_OK                   $00000000                // msgbox styles

define   WHITE_BRUSH             $00000000                // stock objects
define   LTGRAY_BRUSH            $00000001

define   COLOR_WINDOW            $00000005                // desktop colors

define   LF_FACESIZE                    32                // font attributes
define   FW_BOLD                       700
define   DEFAULT_CHARSET                 1
define   OUT_DEFAULT_PRECIS              0
define   CLIP_DEFAULT_PRECIS             0
define   PROOF_QUALITY                   2
define   DEFAULT_PITCH                   0

define   TRANSPARENT                     0

define   FALSE                           0
define   TRUE                            1


synonym  TCHAR                        char
synonym  BYTE                         byte
synonym  UINT                         word
synonym  DWORD                        word
synonym  HANDLE                       word
synonym  HINSTANCE                    word
synonym  HICON                        word
synonym  HCURSOR                      word
synonym  HBRUSH                       word
synonym  HFONT                        word
synonym  HGDIOBJ                      word
synonym  HMENU                        word
synonym  HWND                         word
synonym  HDC                          word
synonym  WPARAM                       word
synonym  LPARAM                       word
synonym  LONG                         word
synonym  ATOM                         word
synonym  BOOL                         word
synonym  LRESULT                      word


struct   POINT
  LONG     x;
  LONG     y;
end

struct   RECT
  LONG     left;
  LONG     top;
  LONG     right;
  LONG     bottom;
end

struct   WNDCLASS
  UINT     style;
  void    @lpfnWndProc;
  int      cbClsExtra;
  int      cbWndExtra;
  HANDLE   hInstance;
  HICON    hIcon;
  HCURSOR  hCursor;
  HBRUSH   hbrBackground;
  char    @lpszMenuName;
  char    @lpszClassName;
end

struct   PAINTSTRUCT
  HDC      hdc;
  BOOL     fErase;
  RECT     rcPaint;
  BOOL     fRestore;
  BOOL     fIncUpdate;
  BYTE     rgbReserved[32];
end

struct   LOGFONT
  LONG     lfHeight;
  LONG     lfWidth;
  LONG     lfEscapement;
  LONG     lfOrientation;
  LONG     lfWeight;
  BYTE     lfItalic;
  BYTE     lfUnderline;
  BYTE     lfStrikeOut;
  BYTE     lfCharSet;
  BYTE     lfOutPrecision;
  BYTE     lfClipPrecision;
  BYTE     lfQuality;
  BYTE     lfPitchAndFamily;
  TCHAR    lfFaceName[LF_FACESIZE];
end

struct   SIZE
  LONG     cx;
  LONG     cy;
end

struct   MSG
  HWND     hWnd;
  UINT     message;
  WPARAM   wParam;
  LPARAM   lParam;
  DWORD    time;
  POINT    pt;
end


char    @GetCommandLine    ()                                external KERNEL32.GetCommandLineA;

BOOL     GetEnvVariable    (char        @Name;
                            char        @Buff;
                            word         BuffLength)         external KERNEL32.GetEnvironmentVariableA;

HANDLE   GetModuleHandle   (char        @ModuleName)         external KERNEL32.GetModuleHandleA;

HANDLE   GetStdHandle      (DWORD        nStdHandle)         external KERNEL32;

HANDLE   CreateFile        (char        @lpFileName;
                            DWORD        dwDesiredAccess;
                            DWORD        dwShareMode;
                            void        @SecurityAttributes;
                            DWORD        dwCreationDistribution;
                            DWORD        dwFlagsAndAttributes;
                            HANDLE       hTemplateFile)      external KERNEL32.CreateFileA;

BOOL     ReadFile          (HANDLE       hFile;
                            void        @Buff;
                            DWORD        nNumberOfBytesToRead;
                            DWORD       @lpNumberOfBytesRead;
                            void        @lpOverlapped)       external KERNEL32;

BOOL     WriteFile         (HANDLE       hFile;
                            void        @Buff;
                            DWORD        nNumberOfBytesToWrite;
                            DWORD       @lpNumberOfBytesWritten;
                            void        @lpOverlapped)       external KERNEL32;

BOOL     CloseHandle       (HANDLE       hObject)            external KERNEL32;

HICON    LoadIcon          (HINSTANCE    hInstance;
                            int          IconID)             external USER32.LoadIconA;

HICON    LoadCursor        (HINSTANCE    hInstance;
                            int          CursorID)           external USER32.LoadCursorA;

ATOM     RegisterClass     (WNDCLASS    @lpWndClass)         external USER32.RegisterClassA;

HWND     CreateWindowEx    (DWORD        dwExStyle;       // extended window style
                            char        @lpClassName;     // pointer to registered class name
                            char        @lpWindowName;    // pointer to window name
                            DWORD        dwStyle;         // window style
                            int          x;               // horizontal position of window
                            int          y;               // vertical position of window
                            int          nWidth;          // window width
                            int          nHeight;         // window height
                            HWND         hWndParent;      // handle to parent or owner window
                            HMENU        hMenu;           // handle to menu or child-window identifier
                            HANDLE       hInstance;       // handle to application instance
                            void        @lpParam)            external USER32.CreateWindowExA;

HMENU    CreateMenu        ()                                external USER32;

HMENU    CreatePopupMenu   ()                                external USER32;

BOOL     AppendMenu        (HMENU        hMenu;           // handle to menu to be changed
                            UINT         uFlags;          // menu-item flags
                            UINT         uIDNewItem;      // menu-item identifier or handle of drop-down menu or submenu
                            char        @lpNewItem)          external USER32.AppendMenuA;

BOOL     SetMenu           (HWND         hWnd;
                            HMENU        hMenu)              external USER32;

BOOL     ShowWindow        (HWND         hWnd;
                            UINT         nCmdShow)           external USER32;

BOOL     UpdateWindow      (HWND         hWnd)               external USER32;

BOOL     GetMessage        (MSG         @lpMsg;
                            HWND         hWnd;
                            UINT         wMsgFilterMin;
                            UINT         wMsgFilterMax)      external USER32.GetMessageA;

BOOL     TranslateMessage  (MSG         @lpmsg)              external USER32;

LONG     DispatchMessage   (MSG         @lpmsg)              external USER32.DispatchMessageA;

HDC      BeginPaint        (HWND         hWnd;
                            PAINTSTRUCT @PS)                 external USER32;

BOOL     EndPaint          (HWND         hWnd;
                            PAINTSTRUCT @PS)                 external USER32;

void     PostQuitMessage   (int          nExitCode)          external USER32;

LRESULT  DefWindowProc     (HWND         hWnd;
                            UINT         Msg;
                            WPARAM       wParam;
                            LPARAM       lParam)             external USER32.DefWindowProcA;

int      MessageBox        (HWND         hWnd;
                            char        @lpText;
                            char        @lpCaption;
                            UINT         uType)              external USER32.MessageBoxA;

HGDIOBJ  GetStockObject    (int          fnObject)           external GDI32;

HGDIOBJ  SelectObject      (HDC          hDC;
                            HGDIOBJ      hObj)               external GDI32;

HFONT    CreateFontIndirect(LOGFONT     @lpfn)               external GDI32.CreateFontIndirectA;

BOOL     DeleteObject      (HGDIOBJ      hObj)               external GDI32;

BOOL     Rectangle         (HDC          hDC;             // handle of device context
                            int          nLeftRect;       // x-coord. of bounding rectangle's upper-left corner
                            int          nTopRect;        // y-coord. of bounding rectangle's upper-left corner
                            int          nRightRect;      // x-coord. of bounding rectangle's lower-right corner
                            int          nBottomRect)        external GDI32;

int      SetBkMode         (HDC          hdc;             // handle of device context
                            int          iBkMode)            external GDI32;

BOOL     TextOut           (HDC          hDC;
                            int          nXStart;
                            int          nYStart;
                            char        @lpString;
                            int          cbString)           external GDI32.TextOutA;

BOOL     GetTextExtentPoint(HDC          hDC;             // handle of device context
                            char        @lpString;        // address of text string
                            int          cbString;        // number of characters in string
                            SIZE        @lpSize)             external GDI32.GetTextExtentPointA;
