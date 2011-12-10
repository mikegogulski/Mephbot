#include <windows.h>

DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout ) ;

#define NUM_CLASS_NAMES	6
#define NUM_WINDOW_NAMES	14

char *ClassNames[NUM_CLASS_NAMES] = {
	"Diablo II",
	"D2Loader",
	"Game",
	"Diablo II.exe",
	"D2Loader.exe",
	"Game.exe" };

char *WindowNames[NUM_WINDOW_NAMES] = {
	"Diablo II.exe",
	"D2Loader.exe",
	"Game.exe",
	"Diablo II Exception",
	"Diablo II Error",
	"Loader Error!",
	"D2Loader Error!",
	"D2Loader Exception",
	"D2Loader Error",
	"Hey guys",
	"Microsoft Visual C++ Debug Library",
	"Game",
	"Diablo II",
	"D2Loader" };

VOID main(VOID) {
	DWORD pid = NULL;
	HWND h = NULL;
	int i = 0, j = 0;
	
	for (j = 0; j < 2; j++) {
		for (i = 0; i < NUM_WINDOW_NAMES; i++) {
			if (h = FindWindow(NULL, WindowNames[i])) {
				GetWindowThreadProcessId(h, &pid);
				if (pid) {
					TerminateApp(pid, 500); }
				pid = NULL;
				h = NULL; } }
		
		for (i = 0; i < NUM_CLASS_NAMES; i++) {
			if (h = FindWindow(ClassNames[i], NULL)) {
				GetWindowThreadProcessId(h, &pid);
				if (pid) {
					TerminateApp(pid, 500); }
				pid = NULL;
				h = NULL; } } }
	return; }


#define TA_FAILED 0
#define TA_SUCCESS_CLEAN 1
#define TA_SUCCESS_KILL 2

//******************
//Source
//******************

//#include "TermApp.h"
//#include <vdmdbg.h>

typedef struct
{
   DWORD   dwID ;
   DWORD   dwThread ;
} TERMINFO ;

// Declare Callback Enum Functions.
BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam ) ;

/*----------------------------------------------------------------
DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout )

Purpose:
   Shut down a 32-Bit Process

Parameters:
   dwPID
      Process ID of the process to shut down.

   dwTimeout
      Wait time in milliseconds before shutting down the process.

Return Value:
   TA_FAILED - If the shutdown failed.
   TA_SUCCESS_CLEAN - If the process was shutdown using WM_CLOSE.
   TA_SUCCESS_KILL - if the process was shut down with
      TerminateProcess().
   NOTE:  See header for these defines.
----------------------------------------------------------------*/ 
DWORD WINAPI TerminateApp( DWORD dwPID, DWORD dwTimeout )
{
   HANDLE   hProc ;
   DWORD   dwRet ;

   // If we can't open the process with PROCESS_TERMINATE rights,
   // then we give up immediately.
   hProc = OpenProcess(SYNCHRONIZE|PROCESS_TERMINATE, FALSE,
      dwPID);

   if(hProc == NULL)
   {
      return TA_FAILED ;
   }

   // TerminateAppEnum() posts WM_CLOSE to all windows whose PID
   // matches your process's.
   EnumWindows((WNDENUMPROC)TerminateAppEnum, (LPARAM) dwPID) ;

   // Wait on the handle. If it signals, great. If it times out,
   // then you kill it.
   if(WaitForSingleObject(hProc, dwTimeout)!=WAIT_OBJECT_0)
      dwRet=(TerminateProcess(hProc,0)?TA_SUCCESS_KILL:TA_FAILED);
   else
      dwRet = TA_SUCCESS_CLEAN ;

   CloseHandle(hProc) ;

   return dwRet ;
}

BOOL CALLBACK TerminateAppEnum( HWND hwnd, LPARAM lParam )
{
   DWORD dwID ;

   GetWindowThreadProcessId(hwnd, &dwID) ;

   if(dwID == (DWORD)lParam)
   {
      PostMessage(hwnd, WM_CLOSE, 0, 0) ;
	  PostMessage(hwnd, WM_QUIT, 0, 0);
   }

   return TRUE ;
}

