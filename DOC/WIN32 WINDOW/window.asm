;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; TODO: на windows 8.1 не работает, отладить!

	import GetModuleHandle:		"kernel32", "GetModuleHandleA"

	import MessageBox:			"user32", "MessageBoxA"
	import RegisterClassEx:		"user32", "RegisterClassExA"
	import ShowWindow:			"user32", "ShowWindow"
	import GetMessage:			"user32", "GetMessageA"
	import TranslateMessage:	"user32", "TranslateMessage"
	import DispatchMessage:		"user32", "DispatchMessageA"
	import PostQuitMessage:		"user32", "PostQuitMessage"
	import DefWindowProc:		"user32", "DefWindowProcA"
	import CreateWindowEx:		"user32", "CreateWindowExA"
	import LoadCursor:			"user32", "LoadCursorA"
	import LoadIcon:			"user32", "LoadIconA"
	import GetClientRect:		"user32", "GetClientRect"
	import BeginPaint:			"user32", "BeginPaint"
	import DrawText:			"user32", "DrawTextA"
	import EndPaint:			"user32", "EndPaint"

	import GetStockObject:		"gdi32", "GetStockObject"

	export WndProc_ExternID:		WndProc, 4     ; 4 - argument size in dwords

	; Пропускаем блок данных
	jmp Main

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	; Блок данных

	tmp					dd 0
	hInst				dd 0
	hWnd				dd 0

	; msg:
	msg_hWnd			dd 0
	msg_message			dd 0
	msg_wParam			dd 0
	msg_lParam			dd 0
	msg_time			dd 0
	msg_point_x			dd 0
	msg_point_y			dd 0

	; wcl
	wcl_cbSize			dd 0
	wcl_style			dd 0
	wcl_lpfnWndProc		dd 0
	wcl_cbClsExtra		dd 0
	wcl_cbWndExtra  	dd 0
	wcl_hInstance		dd 0
	wcl_hIcon			dd 0
	wcl_hCursor			dd 0
	wcl_hbrBackground	dd 0
	wcl_lpszMenuName	dd 0
	wcl_lpszClassName	dd 0
	wcl_hIconSm			dd 0

	szMyClassName	db "dcLoutWnd_ClassName"
	b1 db 0
	szHelloWorld	db "Hello world!"
	b2 db 0
	szError			db "Error in program"
	b3 db 0
	szWndTitle		db "Example program"
	b4 db 0

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;main proc
label Main:

	; ------------------------------------------------------------------------------------
	; hInst = GetModuleHandle( 0 )
	xlock	hInst				; В hInst будет помещено возвращаемое значение (точнее, eax)
	push	dword 0				; >> lpModuleName
	xcall	GetModuleHandle		; Получаем описатель нашего процесса

	; ------------------------------------------------------------------------------------
	; Инициализируем wcl
	mov	wcl_cbSize,			dword 30h	; wcl.cbSize        = sizeof( WNDCLASSEX )
	mov	wcl_style,			dword 3		; wcl.style         = CS_HREDRAW | CS_VREDRAW
	mov	wcl_hInstance,		hInst		; wcl.hInstance     = hInst

	xlock wcl_hIcon
	push  dword 32512	; IDI_APPLICATION
	push  dword 0		; NULL
	xcall LoadIcon						; wcl.hIcon			= LoadIcon( NULL, IDI_APPLICATION )

	mov wcl_hIconSm, wcl_hIcon			; wcl.hIconSm		= wcl.hIcon

	xlock wcl_hCursor
	push  dword 32512	; IDC_ARROW
	push  dword 0		; NULL
	xcall LoadCursor					; wcl.hCursor		= LoadCursor( NULL, IDC_ARROW )

	xlock wcl_hbrBackground
	push  dword 0		; WHITE_BRUSH
	xcall GetStockObject				; wcl.hbrBackground = GetStockObject( WHITE_BRUSH )

	lea wcl_lpszClassName, szMyClassName; wcl.lpszClassName = szMyClassName

	leaf wcl_lpfnWndProc, WndProc_ExternID; wcl.lpfnWndProc	= WndProc

	; ------------------------------------------------------------------------------------
	; tmp = RegisterClassEx(&wcl)
	lea		tmp, wcl_cbSize		; tmp = &wcl
	xlock	tmp
	push	tmp
	xcall	RegisterClassEx

	; if ( tmp == 0 ) goto ReportAnError;
	cmpe	tmp, dword 0
	jct		ReportAnError

	; ------------------------------------------------------------------------------------
	; hWnd = CreateWindowExA(0, szMyClassName, szWndTitle, 0x00cf0000 /*WS_OVERLAPPEDWINDOW*/,
	; 0x80000000 /*CW_USEDEFAULT*/, 0, 0x80000000, 0, 0, 0, hInst, 0)
	xlock	hWnd
	push	dword 0
	push	hInst
	push	dword 0
	push	dword 0
	push	dword 0
	push	dword 80000000h
	push	dword 0
	push	dword 80000000h
	push	dword 00cf0000h
	lea		tmp, szWndTitle
	push	tmp
	lea		tmp, szMyClassName
	push	tmp
	push	dword 0
	xcall	CreateWindowEx

	; if ( hWnd == 0 ) goto ReportAnError;
	cmpe	hWnd, dword 0
	jct		ReportAnError

	; ------------------------------------------------------------------------------------
	; ShowWindow(hWnd, SW_SHOW);
	xlock
	push	dword 5		; SW_SHOW
	push	hWnd
	xcall	ShowWindow


	; ------------------------------------------------------------------------------------
label lblMsgCycleBeg:
	; tmp = GetMessage(&msg, NULL, 0, 0)
	lea		tmp,	msg_hWnd
	xlock	tmp
	push	dword 0
	push	dword 0
	push	dword 0
	push	tmp
	xcall	GetMessage

; if (tmp == 0) goto lblMsgCycleEnd	/*0 == S_OK*/
	cmpe	tmp,	dword 0
	jct		lblMsgCycleEnd

; while ( tmp )
;{
	lea		tmp,	msg_hWnd	; tmp = &msg

	xlock
	push	tmp
	xcall	TranslateMessage

	xlock
	push	tmp
	xcall	DispatchMessage

	jmp		lblMsgCycleBeg
;}

label lblMsgCycleEnd:
	halt						; Завершение потока
;main endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;ReportAnError proc
label ReportAnError:
	; ------------------------------------------------------------------------------------
	xlock						; Переменная не указана - значит возвращаемый функцией результат нем не нужен
	push	dword 30h			; >> uStyle:      MB_ICONWARNING

	lea		tmp, szWndTitle		; Указатель на szWndTitle помещаем в tmp
	push	tmp					; >> lpszCaption: Закладываем в стек значение tmp

	lea		tmp, szError		; Указатель на szError помещаем в tmp
	push	tmp					; >> lpszTitle:   Закладываем в стек значение tmp

	push	dword 0				; >> hWnd:        Закладываем в стек значение 0
	xcall	MessageBox			; Собственно, вызов сообщения об ошибке

	; ------------------------------------------------------------------------------------
	halt						; Завершение потока, вызвавшего эту функцию
;ReportAnError endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;WndProc proc (CALLBACK)
	; Блок данных,
	WndProc_hWnd		dd 0
	WndProc_message		dd 0
	WndProc_wParam		dd 0
	WndProc_lParam		dd 0
	WndProc_tmp			dd 0
	WndProc_HDC			dd 0

	; paintstruct - 64 байта
    paintstruct  db '0123456789012345678901234567890123456789012345678901234567891234'
	; rect - 16 байт
	WndProc_rect db '0123456789123456'

label WndProc:
	pop		WndProc_hWnd
	pop		WndProc_message
	pop		WndProc_wParam
	pop		WndProc_lParam

	; switch ( WndProc_message )
	; {
	cmpe	WndProc_message, dword 2 	;	case WM_DESTROY:
	jct		lblWndProc_DESTROY			;		goto lblWndProc_DESTROY;
	cmpe	WndProc_message, dword 0Fh 	;	case WM_PAINT:
	jct		lblWndProc_PAINT			;		goto lblWndProc_PAINT;
										;	default:
	xlock	WndProc_tmp					; 		tmp = DefWindowProc( nWnd, message, wParam, lParam )
	push	WndProc_lParam
	push	WndProc_wParam
	push	WndProc_message
	push	WndProc_hWnd
	xcall	DefWindowProc
	xret	WndProc_tmp					; 		return tmp;

label lblWndProc_DESTROY:				;	lblWndProc_DESTROY:
	xlock	WndProc_tmp					;		tmp = PostQuitMessage( 0 )
	push	dword 0
	xcall	PostQuitMessage
	xret	WndProc_tmp					;		return tmp

label lblWndProc_PAINT:					;	lblWndProc_PAINT:
	; WndProc_HDC = BeginPaint( WndProc_hWnd, &paintstruct )
	xlock	WndProc_HDC
	lea		WndProc_tmp,		paintstruct
	push	WndProc_tmp
	push	WndProc_hWnd
	xcall	BeginPaint

	;GetClientRect( WndProc_hWnd, &WndProc_rect )
	xlock
	lea		WndProc_tmp,	WndProc_rect
	push	WndProc_tmp
	push	WndProc_hWnd
	xcall	GetClientRect

	;DrawText( WndProc_hDC, szHelloWorld, -1, &WndProc_rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER )
	xlock
	push	dword 25h
	lea		WndProc_tmp,	WndProc_rect	; Эту строку можно закомментировать, но я оставлю...
	push	WndProc_tmp
	push	dword 0ffffffffffffffffh
	lea		WndProc_tmp, szHelloWorld
	push	WndProc_tmp
	push	WndProc_HDC
	xcall	DrawText

	; EndPaint( WndProc_hWnd, &paintstruct )
	xlock
	lea		WndProc_tmp,		paintstruct
	push	WndProc_tmp
	push	WndProc_hWnd
	xcall	BeginPaint

	xret	dword 0
	; }
;WndProc endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
