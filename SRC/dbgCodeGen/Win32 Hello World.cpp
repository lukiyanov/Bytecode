#if 0 != 0
#include "..\LIEng\COP.h"
using namespace interpreter;

#include <string>
#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Определения, используемые в байт-коде
#define jmp		copJMP32v, 0x00,
#define halt	copHALT,   0x00,
#define xlock	copXLOCKm, 0x00,
#define xlock_0	copXLOCK,	0x00,
#define push_v	copPUSH32v, 0x00,
#define push_m	copPUSH32m, 0x00,
#define lea		copLEA32mv, 0x00,
#define lea_f	copLEA32mfv, 0x00,
#define xcall	copXCALL32v, 0x00,
#define mov_v	copMOV32mv, 0x00,
#define mov_m	copMOV32mm, 0x00,
#define cmp_e	copCMPE32mv, 0x00,
#define jct		copJCT32v, 0x00,
#define jcf		copJCF32v, 0x00,
#define pop		copPOP32m, 0x00,
#define xret	copXRET32m, 0x00,

#define bto_dword(value) value, 0x00, 0x00, 0x00,


#define retvar	bto_dword(0x06)
#define hWnd	bto_dword(0x0a)

#define msg		bto_dword(0xe)
	#define msg_hWnd		bto_dword(0xe)
	#define msg_message		bto_dword(0x12)
	#define msg_wParam		bto_dword(0x16)
	#define msg_lParam		bto_dword(0x1a)
	#define msg_time		bto_dword(0x1e)
	#define msg_point_x		bto_dword(0x22)
	#define msg_point_y		bto_dword(0x26)

#define wcl		bto_dword(0x2a)
	#define wcl_cbSize			bto_dword(0x2a)
	#define wcl_style			bto_dword(0x2e)
	#define wcl_lpfnWndProc		bto_dword(0x32)
	#define wcl_cbClsExtra		bto_dword(0x36)
	#define wcl_cbWndExtra		bto_dword(0x3a)
	#define wcl_hInstance		bto_dword(0x3e)
	#define wcl_hIcon			bto_dword(0x42)
	#define wcl_hCursor			bto_dword(0x46)
	#define wcl_hbrBackground	bto_dword(0x4a)
	#define wcl_lpszMenuName	bto_dword(0x4e)
	#define wcl_lpszClassName	bto_dword(0x52)
	#define wcl_hIconSm			bto_dword(0x56)

#define szClassName	bto_dword(0x5a)
#define szWndTitle	bto_dword(0x63)
#define hInst		bto_dword(0x70)

#define CodeBlockOffset 0x96, 0x00, 0x00, 0x00,
#define ErrorOffset		0x74, 0x00, 0x00, 0x00,
#define MsgCycleEnd		0x24, 0x02, 0x00, 0x00,
#define MsgCycleBeg		0xba, 0x01, 0x00, 0x00,
#define	WindowProc		0x4e, 0x02, 0x00, 0x00,

#define dwWndProc		0x24e
#define WindowProcExternalID bto_dword(0)

#define fRegisterClassExA	bto_dword(0)
#define fShowWindow			bto_dword(1)
#define fGetMessageA		bto_dword(2)
#define fTranslateMessage	bto_dword(3)
#define fDispatchMessageA	bto_dword(4)
#define fPostQuitMessage	bto_dword(5)
#define fDefWindowProcA		bto_dword(6)
#define fCreateWindowExA	bto_dword(7)
#define fGetModuleHandleA	bto_dword(8)
#define fMessageBoxA		bto_dword(9)

#define constSW_SHOW				bto_dword(5)
#define constWM_DESTROY				bto_dword(2)
#define constCW_USEDEFAULT			0x00, 0x00, 0x00, 0x80,
#define constWS_OVERLAPPEDWINDOW	0x00, 0x00, 0xcf, 0x00,

#define loc1_hWnd		0x36, 0x02, 0x00, 0x00,
#define loc1_message	0x3a, 0x02, 0x00, 0x00,
#define loc1_wParam		0x3e, 0x02, 0x00, 0x00,
#define loc1_lParam		0x42, 0x02, 0x00, 0x00,

#define loc1_1	0x046, 0x02, 0x00, 0x00,
#define loc1_2	0x04a, 0x02, 0x00, 0x00,

#define WndProc_DESTROY	0xa0, 0x02, 0x00, 0x00,

////////////////////////////////////////////////////////////////////////////////
// Байт-код
unsigned char aCode[] = {
	// Пропускаем блок данных
	jmp CodeBlockOffset

	// .data
	// Блок данных
	0x00, 0x00, 0x00, 0x00,	// return dw 0 [dword: 06h]
	0x00, 0x00, 0x00, 0x00,	// hWnd HWND 0 [dword: 0ah]

	0x00, 0x00, 0x00, 0x00,	// msg  MSG:   hWnd HWND 0      [dword:0eh]
	0x00, 0x00, 0x00, 0x00,	// msg  MSG:   message UINT   0 [dword:12h]
	0x00, 0x00,	0x00, 0x00, // msg  MSG:   wParam  WPARAM 0 [dword:16h]
	0x00, 0x00, 0x00, 0x00, // msg  MSG:   lParam  LPARAM 0 [dword:1ah]
	0x00, 0x00, 0x00, 0x00, // msg  MSG:   time    DWORD  0 [dword:1eh]
	0x00, 0x00, 0x00, 0x00, // msg  MSG:   point POINT: x LONG 0 [dword:22h]
	0x00, 0x00, 0x00, 0x00, // msg  MSG:   point POINT: y LONG 0 [dword:26h]

	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: cbSize UINT 0			[dword:2ah]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: style  UINT 0			[dword:2eh]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: lpfnWndProc WNDPROC 0	[dword:32h]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: cbClsExtra  dd 0		[dword:36h]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: cbWndExtra  dd 0		[dword:3ah]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: hInstance HINSTANCE 0	[dword:3eh]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: hIcon HICON 0			[dword:42h]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: hCursor HCURSOR 0		[dword:46h]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: hbrBackground HBRUSH 0	[dword:4ah]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: lpszMenuName  LPCTSTR 0	[dword:4eh]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: lpszClassName LPCTSTR 0	[dword:52h]
	0x00, 0x00, 0x00, 0x00,	// wcl WINDCLASSEX: hIconSm HICON 0			[dword:56h]

	'M', 'y', 'C', 'l', 'a', 's', 's', '4', 0,						//	[byte:5ah]
	'H', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd', '!', 0 ,	//	[byte:63h]

	0x00, 0x00, 0x00, 0x00,	// hInst HINSTANCE 0						[dword:70h]

	// .code
	xlock_0
	push_v	bto_dword(0x30)
	push_v	bto_dword(0)
	push_v	bto_dword(0)
	push_v	bto_dword(0)
	xcall	fMessageBoxA
	halt

	// .code
	// [*:74]

	// --------------------------------------------------------
	// WinMain ( )

	// Вызов: retvar = GetModuleHandle(0)
	xlock	retvar
	push_v	bto_dword(0)
	xcall	fGetModuleHandleA
	mov_m	hInst			retvar



	// Заполнение wcl
	mov_v	wcl_cbSize		bto_dword( 0x30 /*sizeof(WNDCLASSEX)*/ )
	mov_m	wcl_hInstance	hInst
	mov_v	wcl_style		bto_dword( 1 | 2 )
	mov_v	wcl_hbrBackground bto_dword(0x06)

	lea		retvar			szClassName
	mov_m	wcl_lpszClassName retvar

	lea_f	retvar			WindowProcExternalID
	mov_m	wcl_lpfnWndProc retvar


	// Вызов: retvar = RegisterClassExA(&wcl)
	lea		retvar			wcl
	xlock	retvar
	push_m	retvar
	xcall	fRegisterClassExA

	// if ( retvar == 0 ) goto ErrorOffset;
	cmp_e	retvar			bto_dword(0)
	jct		ErrorOffset



	// Вызов: retvar = CreateWindowExA(0, szClassName, szWndTitle, 0x00cf0000 /*WS_OVERLAPPEDWINDOW*/,
	// 0x80000000 /*CW_USEDEFAULT*/, 0, 0x80000000, 0, 0, 0, hInst, 0)
	xlock	hWnd
	push_v	bto_dword(0)
	push_m	hInst
	push_v	bto_dword(0)
	push_v	bto_dword(0)
	push_v	bto_dword(0)
	push_v	constCW_USEDEFAULT
	push_v	bto_dword(0)
	push_v	constCW_USEDEFAULT
	push_v	constWS_OVERLAPPEDWINDOW
	lea		retvar			szWndTitle
	push_m	retvar
	lea		retvar			szClassName
	push_m	retvar
	push_v	bto_dword(0)
	xcall	fCreateWindowExA

	// if ( retvar == 0 ) goto ErrorOffset;
	cmp_e	hWnd			bto_dword(0)
	jct		ErrorOffset



	// ShowWindow(hWnd, SW_SHOW);
	xlock_0
	push_v	constSW_SHOW
	push_m	hWnd
	xcall	fShowWindow



// MsgCycleBeg:
	// retvar = GetMessage(&msg, NULL, 0, 0)
	lea retvar msg
	xlock retvar
	push_v	bto_dword(0)
	push_v	bto_dword(0)
	push_v	bto_dword(0)
	push_m	retvar
	xcall	fGetMessageA

	// if (retvar == 0) goto MsgCycleEnd	/*0 == S_OK*/
	cmp_e	retvar		bto_dword(0)
	jct		MsgCycleEnd

	//{
	lea		retvar		msg

	xlock_0
	push_m	retvar
	xcall	fTranslateMessage

	xlock_0
	push_m	retvar
	xcall	fDispatchMessageA
	//}

	jmp		MsgCycleBeg

// MsgCycleEnd:
	halt	// Завершение алгоритма

	// Разделитель на всякий случай (Зарезервировано)
	0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,

	// --------------------------------------------------------
	// LRESULT CALLBACK WindowProc( ... )
	// .data
	0xFF, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,

	// .code offset = 0x24e
	pop		loc1_hWnd
	pop		loc1_message
	pop		loc1_wParam
	pop		loc1_lParam

	// switch (loc1_message)
	// {
	// case WM_DESTROY: goto WndProc_DESTROY;
	cmp_e	loc1_message constWM_DESTROY
	jct		WndProc_DESTROY

	// default:
	xlock	retvar
	push_m	loc1_lParam
	push_m	loc1_wParam
	push_m	loc1_message
	push_m	loc1_hWnd
	xcall	fDefWindowProcA
	xret	retvar
	// }

// WndProc_DESTROY:
	xlock	retvar
	push_v	bto_dword(0)
	xcall	fPostQuitMessage
	xret	retvar
};

const int nCodeSize = sizeof( aCode );

////////////////////////////////////////////////////////////////////////////////
// Функции обратного вызова
struct Callback
{
	unsigned offset;
	unsigned stack;
};

Callback cbcks[] = {
	{ dwWndProc, 4 }								// id == 0
};

const int nCallbackCount = sizeof( cbcks ) / sizeof( Callback );

////////////////////////////////////////////////////////////////////////////////
// Импортируемые функции
struct ExternProc
{
	string module;
	string name;
};

ExternProc extprocs[] = {
	{ "user32", "RegisterClassExA"	 },		// id == 0
	{ "user32", "ShowWindow"		 },		// id == 1
	{ "user32", "GetMessageA"		 },		// id == 2
	{ "user32", "TranslateMessage"	 },		// id == 3
	{ "user32", "DispatchMessageA"	 },		// id == 4
	{ "user32", "PostQuitMessage"	 },		// id == 5
	{ "user32", "DefWindowProcA"	 },		// id == 6
	{ "user32", "CreateWindowExA"	 },		// id == 7
	{ "kernel32", "GetModuleHandleA" },		// id == 8
	{ "user32", "MessageBoxA"		 }		// id == 9
};

const int nExternProcCount = sizeof( extprocs ) / sizeof( ExternProc );

////////////////////////////////////////////////////////////////////////////////

int main()
{
	ofstream fout( "program.bytecode", ios::out | ios::binary );
	fout.write( (char*)aCode, nCodeSize );
	fout.flush();
	fout.close();

	fout.open( "program.extern", ios::out );

	fout << nCallbackCount << endl;
	for (int i = 0; i < nCallbackCount; i++)
	{
		fout << cbcks[i].offset << ' ' << cbcks[i].stack << endl;
	}

	fout << nExternProcCount << endl;
	for (int i = 0; i < nExternProcCount; i++)
	{
		fout << extprocs[i].module.c_str() << ' ' << extprocs[i].name.c_str() << endl;
	}

	fout.flush();
	fout.close();
}

////////////////////////////////////////////////////////////////////////////////
#endif