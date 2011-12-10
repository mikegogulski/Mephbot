// mouse.h
#pragma once
#include "MString.h"


#define INST_NOP 0x90
#define INST_CALL 0xe8
#define INST_JMP 0xe9

#define DB(x) __asm _emit (x)
#define DW(x) __asm _emit (x)&0xff __asm _emit (x)>>8&0xff
#define DD(x) __asm _emit (x)&0xff __asm _emit (x)>>8&0xff __asm _emit (x)>>16&0xff __asm _emit (x)>>24&0xff
#define CMP_DWORD(addr,bval) DB(0x83) DB(0x3d) DD(addr) DB(bval)

#define FUNCTLEN(func) ((DWORD)func##END-(DWORD)func)
#define ADDR_INTERCEPTHACK 0x00400f00
#define ADDR_MESSAGESHACK 0x00400f00


HANDLE GetProcessHandle(char *szClass, char *szTitle) ;
DWORD ReadProcessDWORD(HANDLE hProcess, DWORD lpAddress) ;
WORD ReadProcessWORD(HANDLE hProcess, DWORD lpAddress) ;

//SOCKET DuplicateSocket(HANDLE hSourceProcess, SOCKET hSourceSocket, HANDLE hTargetProcess) ;
void SendPacketViaD2(DWORD len, BYTE *buf) ;

void SendTextToD2(wchar_t *buf) ;
void PrintMessage( char* tmp, int iColor) ;

void InterceptCode(HANDLE diablo2_ph, int inst, DWORD pOldCode, DWORD pNewCode, int nBytes) ;
void MessagesHackStart(HWND hwnd, HANDLE diablo2_ph) ;
void MessagesHackStop( HANDLE diablo2_ph) ;

static HWND hMessagesWnd = 0;



CString BytesToString(const unsigned char *msg, int begin, int end, BOOL bWhiteSpaceSpecial, BOOL offset=FALSE, DWORD pos=0, DWORD len=0) ;

// struct to decode bitfields
// written by Mousepad, 8/2001
class BitFields
{
public:
	BYTE*		m_Data;
	DWORD		m_Pos;
	CString		m_sDebugComment ;
//	CStringList	m_sDebugLines ;

	BitFields(BYTE *d) ;
	BitFields() ;

	DWORD GetBitField( DWORD pos, DWORD len) ;
	DWORD GetField( DWORD len, CString sDebug="-") ;
	CString PrintBitField( DWORD pos, DWORD len, DWORD dBitField) ;
} ;


