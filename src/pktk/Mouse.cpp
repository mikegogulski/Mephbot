// mouse.cpp
#include "MString.h"

#include "Mouse.h"


///
///  ALL THIS WAS WRITTEN BY MOUSEPAD FROM THE CHEATLIST FORUMS.  There is only one part
///    you need to modify, just look for my comments.  I think ALL the comments that you
///	   can make any sense of are from me :p
///

//
// This code was generated by cheatlist.com member "mousepad" on 5/7/01
//   to decode the packet encoding started with D2 v1.06 (minor mods made by CN)
///////////////////////////////



//
//  find the specified program name
//////////////////////////////////////////////////////////////

HANDLE GetProcessHandle(char *szClass, char *szTitle)
{
	HWND hwnd = FindWindow(szClass, szTitle);

	// if we don't find the window, then terminate (CN)
	if (hwnd == NULL)
		return 0 ;

	DWORD pid = 0;
	GetWindowThreadProcessId(hwnd, &pid);
	return OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
}




//
//  memory access functions
//////////////////////////////////////////////////////////////

BOOL VirtualProtectEx(HANDLE hProcess, LPVOID lpAddress, DWORD dwSize, DWORD flNewProtect)
{
	DWORD dummy;
	return VirtualProtectEx(hProcess, lpAddress, dwSize, flNewProtect, &dummy);
}


void ReadProcessBYTES(HANDLE hProcess, DWORD lpAddress, void *buf, int len)
{
	DWORD oldprot = 0;
	VirtualProtectEx(hProcess, (void *)lpAddress, len, PAGE_READWRITE, &oldprot);
	ReadProcessMemory(hProcess, (void *)lpAddress, buf, len, 0);
	VirtualProtectEx(hProcess, (void *)lpAddress, len, oldprot);
}


DWORD ReadProcessDWORD(HANDLE hProcess, DWORD lpAddress)
{
	DWORD val = 0;
	ReadProcessBYTES(hProcess, lpAddress, &val, 4);
	return val;
}


void WriteProcessBYTES(HANDLE hProcess, DWORD lpAddress, void *buf, int len)
{
	DWORD oldprot = 0;
	VirtualProtectEx(hProcess, (void *)lpAddress, len, PAGE_READWRITE, &oldprot);
	WriteProcessMemory(hProcess, (void *)lpAddress, buf, len, 0);
	VirtualProtectEx(hProcess, (void *)lpAddress, len, oldprot);
}


void WriteProcessDWORD(HANDLE hProcess, DWORD lpAddress, DWORD val)
{
	WriteProcessBYTES(hProcess, lpAddress, &val, 4);
}


void FillProcessBYTES(HANDLE hProcess, DWORD lpAddress, BYTE ch, int len)
{
	 BYTE *fillbuf = new BYTE[len];
	 memset(fillbuf, ch, len);
	 WriteProcessBYTES(hProcess, lpAddress, fillbuf, len);
	 delete fillbuf;
}


BYTE ReadProcessBYTE(HANDLE hProcess, DWORD lpAddress)
{
	BYTE val = 0;
	ReadProcessBYTES(hProcess, lpAddress, &val, 1);
	return val;
}


void WriteProcessBYTE(HANDLE hProcess, DWORD lpAddress, BYTE val)
{
	WriteProcessBYTES(hProcess, lpAddress, &val, 1);
}


WORD ReadProcessWORD(HANDLE hProcess, DWORD lpAddress)
{
	WORD val = 0;
	ReadProcessBYTES(hProcess, lpAddress, &val, 2);
	return val;
}


void WriteProcessWORD(HANDLE hProcess, DWORD lpAddress, WORD val)
{
	WriteProcessBYTES(hProcess, lpAddress, &val, 2);
}




//
//  communiction back to b.net
//////////////////////////////////////////////////////////////

/*
// duplicate sockets have some compatibility issues with some systems and are no longer used
SOCKET DuplicateSocket(HANDLE hSourceProcess, SOCKET hSourceSocket, HANDLE hTargetProcess) 
{
	SOCKET newsocket = INVALID_SOCKET; 
	DuplicateHandle(hSourceProcess, (HANDLE)hSourceSocket, hTargetProcess, (LPHANDLE)&newsocket, 0, FALSE, DUPLICATE_SAME_ACCESS); 
	return newsocket; 
} 
*/

void SendPacketViaD2(DWORD len, BYTE *buf)
{
	COPYDATASTRUCT pcds;
	pcds.lpData = buf;
	pcds.cbData = len;
	pcds.dwData = 0;
	SendMessage(FindWindow("Diablo II", "Diablo II"), WM_COPYDATA, (WPARAM)hMessagesWnd, (LPARAM)&pcds);
}





/*
ÿc1 red 
ÿc2 green 
ÿc3 blue 
ÿc4 gold 
ÿc5 grey 
ÿc6 black 
ÿc7 darker yellow 
ÿc8 orange 
ÿc9 yellow 
ÿc0 white 
ÿc; purple 
ÿc: dark green 
*/

void SendTextToD2(wchar_t *buf)
{
 COPYDATASTRUCT pcds;
 pcds.lpData = buf;
 pcds.cbData = (wcslen(buf)+1)*2;
 pcds.dwData = 1;
 SendMessage(FindWindow("Diablo II", "Diablo II"), WM_COPYDATA, (WPARAM)hMessagesWnd, (LPARAM)&pcds);
}


void PrintMessage( char* tmp, int iColor)
{
	char tmp2[80] ;
	sprintf( tmp2, "%s", tmp) ;

	switch (iColor) {
	case 0: return ;		// don't write message
	case 1: 
			{
			wchar_t szText[80] = L"\xff" L"c1" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 2: 
			{
			wchar_t szText[80] = L"\xff" L"c2" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 3:
			{
			wchar_t szText[80] = L"\xff" L"c3" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 4: 
			{
			wchar_t szText[80] = L"\xff" L"c4" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 5: 
			{
			wchar_t szText[80] = L"\xff" L"c5" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 6: 
			{
			wchar_t szText[80] = L"\xff" L"c6" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 7: 
			{
			wchar_t szText[80] = L"\xff" L"c7" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 8: 
			{
			wchar_t szText[80] = L"\xff" L"c8" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 9: 
			{
			wchar_t szText[80] = L"\xff" L"c9" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 10: 
			{
			wchar_t szText[80] = L"\xff" L"c0" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 11: 
			{
			wchar_t szText[80] = L"\xff" L"c;" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	case 12: 
			{
			wchar_t szText[80] = L"\xff" L"c:" ;
			for (int n=0;n<80;n++)
				szText[n+3] = tmp2[n] ;
			SendTextToD2(szText);
			}
			break ;
	}
}




//
//  this is the code that patches the DLL's in memory for our own purposes
//////////////////////////////////////////////////////////////

void InterceptCode(HANDLE diablo2_ph, int inst, DWORD pOldCode, DWORD pNewCode, int nBytes)
{
	BYTE *buf1 = new BYTE[nBytes];
	buf1[0] = inst;
	*(DWORD *)(buf1+1) = pNewCode-(pOldCode+5);
	memset(buf1+5, 0x90, nBytes-5); //nops
	WriteProcessBYTES(diablo2_ph, pOldCode, buf1, nBytes);
	delete buf1;
}


void __declspec(naked) MessagesHack_ASM()
{
 __asm {
	jmp recvmessage //+0
	jmp sentmessage //+2
	jmp wndprochack //+4
	jmp keydownhack2 //+6

	DD(0) //+8 dest hwnd

keydownhack2: jmp keydownhack

recvmessage:
	//ebp = ptr message
	//eax = length
	//bl = message no
	CMP_DWORD(ADDR_MESSAGESHACK+8,0) //dest hwnd
	je recvmessage3


	cmp bl,0x9C			// item record
	je recvmessage2

	cmp bl,0x9D			// item owner record
	je recvmessage2

	cmp bl,0x59			// characer record
	je recvmessage2

/*
//cn-GI
cmp bl,0x96
je recvmessage2

cmp bl,0x0A
je recvmessage2
//cn-GI
*/

	jmp recvmessage3


recvmessage2:
	push ebp //ptr message
	push eax //length
	push -1 //type
	call sendcopydata

recvmessage3:
//orignal code
	and ebx,0xFF
	ret

sentmessage:
//[esp+0x0c] = ptr message
//[esp+0x10] = length
	CMP_DWORD(ADDR_MESSAGESHACK+8,0) //dest hwnd
	je sentmessage3

	mov eax,[esp+0x0c]
	mov al,[eax]

	cmp al,0x01
	je sentmessage2
	cmp al,0x03
	je sentmessage2
	jmp sentmessage3

sentmessage2:
	push dword ptr [esp+0x0c] //ptr message
	push dword ptr [esp+4+0x10] //length
	push -2 //type
	call sendcopydata

sentmessage3:
//orignal code
	mov eax,0x6FC0B06C
	mov eax,[eax]
	ret

wndprochack:
//[esp+0x08] = uMsg
//[esp+0x0c] = wParam
//[esp+0x10] = lParam
	cmp dword ptr [esp+0x08],WM_COPYDATA //uMsg
	jne oldwndproc

	pushad
	mov eax,[esp+0x20+0x10] //lParam, pcds
	mov ecx,[eax+8] //pcds->lpData
	xor edx,edx
	mov ebx,0x6FB20EB0 //display text (d2client)
	cmp dword ptr [eax+0],0 //pcds->dwData
	jne notsend
	push dword ptr [eax+4] //pcds->cbData
	push ecx //pcds->lpData
	push 0
	mov eax,0x6FC0B06C
	mov eax,[eax]
	mov ebx,0x6FC01785 //send message (d2net 2715)
notsend:
	call ebx
	popad

oldwndproc:
//orignal code
	sub esp,8
	xor ecx,ecx
	push 0x6F8AF56A //resume code
	ret

keydownhack:
//orignal code
	mov esi,[edi+8]
	xor edx,edx

	CMP_DWORD(ADDR_MESSAGESHACK+8,0) //dest hwnd
	je keydown3

	cmp esi,'G'
	je keydown2
	jmp keydown3

keydown2:
	push 0
	push 0
	push esi
	call sendcopydata
keydown3:
	ret

sendcopydata:
//params 3 = dwData, cbData, lpData
	pushad
	lea eax,[esp+0x20+4] //lParam, pcds
	push eax //lParam, pcds
	mov eax,0x6FB5F44E //get hwnd (d2gfx 272b)
	call eax
	push eax //wParam, source hwnd
	push WM_COPYDATA //uMsg
	mov eax,ADDR_MESSAGESHACK+8 //dest hwnd
	push dword ptr [eax] //dest hwnd
	mov eax,0x6FB6C0F4 //d2client's SendMessageA
	call [eax]
	popad
	ret 3*4
 }
}
void __declspec(naked) MessagesHack_ASMEND() {}


void MessagesHackStart(HWND hwnd, HANDLE diablo2_ph)
{
	VirtualProtectEx(diablo2_ph, (void *)ADDR_MESSAGESHACK, 0x1000, PAGE_EXECUTE_READWRITE);
	WriteProcessBYTES(diablo2_ph, ADDR_MESSAGESHACK, MessagesHack_ASM, FUNCTLEN(MessagesHack_ASM));
	InterceptCode(diablo2_ph, INST_CALL, 0x6FAB43DA, ADDR_MESSAGESHACK+0, 6); //recv (d2client)
	InterceptCode(diablo2_ph, INST_CALL, 0x6FC01780, ADDR_MESSAGESHACK+2, 5); //send (d2net)
	InterceptCode(diablo2_ph, INST_JMP, 0x6F8AF565, ADDR_MESSAGESHACK+4, 5); //wndproc (d2win)
	InterceptCode(diablo2_ph, INST_CALL, 0x6FACEF84, ADDR_MESSAGESHACK+6, 5); //keydown (d2client)
	WriteProcessDWORD(diablo2_ph, ADDR_MESSAGESHACK+8, (DWORD)hwnd);
	hMessagesWnd = hwnd;
}


void MessagesHackStop( HANDLE diablo2_ph)
{
	WriteProcessDWORD(diablo2_ph, ADDR_MESSAGESHACK+8, 0);
	hMessagesWnd = 0;
}





//
//  A little helper function written by Nuttzy for displaying bitfields
//////////////////////////////////////////////////////////////

CString BytesToString(const unsigned char *msg, int begin, int end, BOOL bWhiteSpaceSpecial, BOOL offset, DWORD pos, DWORD len)
{
	CString sOutput ;
	// print white spacesif this is less than 56 bytes
	if (!offset) {
		if (bWhiteSpaceSpecial) {
			for (int x=51; x>end; x--)
				sOutput += "        " ;
		} else {
			for (int x=56; x>end; x--)
				sOutput += "        " ;
		}

	} else {
		for (int x=56; x>end+12; x--)
			sOutput += "        " ;
		sOutput += "    " ;
	}

	// convert the bytes in question to a binary string
	char buffer[12] ;
	CString sTmp ;
	for (int x=end; x>=begin; x--) {
		itoa( msg[x], buffer, 2  );
		// add leading 0's if less than 8 char's
		for (int y=0; y<8-(int)strlen(buffer); y++)
			sTmp += "0" ;
		sTmp += buffer ;
	}

	// trim the binary string down to the bits we are looking for
	if (offset) {
		int diff = sTmp.GetLength()-pos%8 ;
		CString sTmp2 = sTmp.Left( diff) ;
		sTmp2 = sTmp2.Right( len) ;
		// replace the bits we chopped off with white space
		for (int y=0; y<(diff-(int)len); y++)
			sOutput += " " ;
		sOutput += sTmp2 ;
	} else {
		sOutput += sTmp ;
	}

	return sOutput ;
}



//
// BitFields written by Mousepad August 2001 and enhanced by Nuttzy November 2001
///////////////////////////

// private member does the "printing"
CString BitFields::PrintBitField( DWORD pos, DWORD len, DWORD dBitField)
{
	DWORD truPos = pos ;
	pos = pos+64 ;
	CString	sOutput ;

	// printing one bit
	if (len == 1) {
		sOutput.Format( "%12s  [%2d   ] (%3d    ) [%2X   ] %8X", m_sDebugComment, pos/8, pos, m_Data[truPos/8], dBitField) ;
		sOutput += BytesToString( m_Data, (truPos/8), (truPos/8), FALSE, TRUE, pos, len) ;

	// the bits to print are in one byte
	} else if ( pos/8 == (pos+len-1)/8 ) {
		sOutput.Format( "%12s  [%2d   ] (%3d:%3d) [%2X   ] %8X", m_sDebugComment, pos/8, pos, pos+len-1, m_Data[truPos/8], dBitField) ;
		sOutput += BytesToString( m_Data, (truPos/8), (truPos/8), FALSE, TRUE, pos, len) ;

	// the bitfield extends across at least 2 bytes
	} else {
		sOutput.Format( "%12s  [%2d:%2d] (%3d:%3d) [%2X:%2X] %8X", m_sDebugComment, pos/8,(pos+len-1)/8, pos, pos+len-1, m_Data[truPos/8], m_Data[(truPos+len-1)/8], dBitField) ;
		sOutput += BytesToString( m_Data, (truPos/8), ((truPos+len-1)/8), FALSE, TRUE, pos, len) ;
	}

	return sOutput ;
}


// constructors
BitFields::BitFields(BYTE *d)
{
	m_Data = d;
	m_Pos = 0;
}

BitFields::BitFields()
{
	m_Pos = 0;
}


// private member does the getting
DWORD BitFields::GetBitField( DWORD pos, DWORD len)
{
	DWORD dBitField = (DWORD)(*(unsigned __int64 *)(m_Data+pos/8)<<(64-len-(pos&7))>>(64-len));
//	m_sDebugLines.AddTail( PrintBitField( pos, len, dBitField)) ;
	return dBitField ;
}


// the primary function that is called to get the bits
DWORD BitFields::GetField(DWORD len, CString sDebug)
{
//	m_sDebugComment = sDebug ;
	return GetBitField( (m_Pos+=len)-len, len) ;
}

///////////////////////////
