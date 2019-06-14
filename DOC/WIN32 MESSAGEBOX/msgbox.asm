;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; dword MessageBoxA(dword hWnd, dword lpTextAddr, dword lpCaptionAddr, dword uType);
import MessageBox: "user32", "MessageBoxA"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; -----------------------------------------------------------------------------
; NB: аргументы заталкиваются в стек в обратном порядке.
; ret_value = MessageBox(0, message, caption, MB_YESNOCANCEL | MB_ICONASTERISK)
xlock ret_value

mov  tmp, MB_YESNOCANCEL
or   tmp, MB_ICONASTERISK
push tmp

lea  tmp, caption	; tmp = физический адрес начала строки caption
push tmp

lea  tmp, message	; tmp = физический адрес начала строки message
push tmp

push dword 0

xcall MessageBox

; -----------------------------------------------------------------------------
; if      ( ret_value == IDYES ) ret_value = &selected_yes
; else if ( ret_value == IDNO )  ret_value = &selected_no
; else 							 ret_value = &selected_cancel
cmpe ret_value, IDYES
jct branch_yes
cmpe ret_value, IDNO
jct branch_no
	; cancel
	lea ret_value, selected_cancel
	jmp branch_exit
label branch_no:
	lea ret_value, selected_no
	jmp branch_exit
label branch_yes:
	lea ret_value, selected_yes
label branch_exit:

; -----------------------------------------------------------------------------
; MessageBox(0, tmp, caption, MB_OK | MB_ICONASTERISK)
xlock

mov  tmp, MB_OK
or   tmp, MB_ICONASTERISK
push tmp

lea  tmp, caption	; msg_addr = физический адрес начала строки caption
push tmp

push ret_value

push dword 0

xcall MessageBox
	

halt

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
tmp       dd 0
ret_value dd 0

; Строки с нулём на конце:
caption			db "Пример 'msgbox.asm'"
zero1_			db 0
message			db "Нажмите любую из кнопок или закройте окно."
zero2_			db 0
selected_yes	db "Вы нажали 'Да'"
zero3_			db 0
selected_no		db "Вы нажали 'Нет'"
zero4_			db 0
selected_cancel	db "Вы нажали 'Отмена' или закрыли окно."
zero5_			db 0

MB_OK			dd 0
MB_YESNOCANCEL	dd 3
MB_ICONASTERISK	dd 64
IDYES           dd 6
IDNO            dd 7