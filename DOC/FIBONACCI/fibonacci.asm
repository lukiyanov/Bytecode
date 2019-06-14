;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;	Исходники к задаче о строках Фибоначчи.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	import AllocConsole: "kernel32", "AllocConsole"
	import ReadConsole:  "kernel32", "ReadConsoleA"
	import WriteConsole: "kernel32", "WriteConsoleA"
	import GetStdHandle: "kernel32", "GetStdHandle"

;	import MessageBox: 	 "user32", "MessageBoxA"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
jmp entry
	tmp			dd 0
	hStdInput	dd 0
	hStdOutput	dd 0
	K			dd 0
	P			dd 0
	s			dd 0
	n			dd 0
	
	strLen_n_2		dd 0
	strLen_n_3		dd 0
	strLen_n_2_3	dd 0

	ans	db 0
	
	szNoSolution db "No solution"
	_0 db 0
	
	sz1 db "abcbc"				; Завершающий ноль не нужен
	sz2 db "bcabcabcbcabc"		; Завершающий ноль не нужен

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
label entry:
	; Запрашиваем консоль
	xlock	tmp
	xcall	AllocConsole
	cmpe	tmp, dword 0
	jct		exit

	; Получаем дескрипторы
	xlock	hStdInput
	push	dword 0FFFFFFF6h
	xcall	GetStdHandle
	
	xlock	hStdOutput
	push	dword 0FFFFFFF5h
	xcall	GetStdHandle

	; Считываем ввод, два числа типа dword: K, P
	call	ReadDword

	;if ( k < nMaxLineNum ) // Если k >= 44, то p точно не выйдет за пределы строки, т.к. длина строки будет больше 1000000000.
	cmpb K, dword 44
	jcf  i5_else
	;{
		;// Номер нашей строки.
		;// k меньше/равно максимуму?
		;dword n = (k <= nMaxLineNum)?
		;			// Если да, то используем k
		;				( k ): ...
		mov n, k

	;	l = arrStrLength[ k ];
		lea tmp,	arrStrLength
		mov s,		k
		mul s,		dword 4	; 4 == sizeof(dword)
		add tmp,	s
		mov tmp,	dword [tmp]
	;	if ( l < p )
		cmpb tmp, p
		jcf  i5_next
	;	{
	;		cout << "No solution" <<endl;
	;		return 0;
			xlock
			push	dword 0
			lea		tmp,	numOfChRead
			push	tmp
			push	dword 11
			lea		tmp,	szNoSolution
			push	tmp
			push	hStdOutput
			xcall	WriteConsole
			
			; Задержка перед закрытием
			xlock	tmp
			push	dword 0
			lea		tmp,	numOfChRead
			push	dword tmp
			push	dword 32
			lea		tmp,	buffer
			push	dword tmp
			push	dword hStdInput
			xcall	ReadConsole

			halt
	;	}
	;}
	;else
label i5_else:
	;// Номер нашей строки.
	;// k меньше/равно максимуму?
	;dword n =  ...
	;			// ...обрезаем до максимума, СОХРАНИВ ЧЕТНОСТЬ СТРОКИ
	;				( ( k % 2 /* нечет? */ )? nMaxLineNum/*нечет*/ : nMaxLineNum-1 /*чет*/ );
	mov		tmp,	k
	div		tmp,	dword 2
	mul		tmp,	dword 2
	mov		s,		k
	sub		s,		tmp
	cmpe	s,		dword 0
	jct		i6
	; Нечет
	mov		n,		dword 45
	
	jmp		i5_next
label i6:
	; Чет
	mov		n,		dword 44

label i5_next:

label c5_begin:
	;while ( 6 < n )
	cmpb	dword 6,	n
	jcf		c5_end
	;{
	;	const dword strLen_n_2 = arrStrLength[n - 2], strLen_n_3 = arrStrLength[n - 3];
	;	const dword strLen_n_2_3 = strLen_n_2 + strLen_n_3;
		lea strLen_n_2,	arrStrLength
		lea strLen_n_3,	arrStrLength

		mov tmp, n
		sub tmp, dword 2
		mul tmp, dword 4	; 4 == sizeof(dword)
		add strLen_n_2,	tmp
		
		mov strLen_n_3, strLen_n_2
		sub strLen_n_3, dword 4

		mov strLen_n_2, dword [strLen_n_2]
		mov strLen_n_3, dword [strLen_n_3]
		
		mov strLen_n_2_3, strLen_n_2
		add strLen_n_2_3, strLen_n_3

	;	// В какой из 3х отрезков входит p?	
	;	if ( p <= strLen_n_2 )
	cmpbe p, strLen_n_2
	jcf c5_elif
	;	{
	;		// первый отрезок: [0..l(n-2)]
	;		// p остается прежним
	;		n = n - 2;
			sub n, dword 2
			jmp c5_begin
	;	}
	;	else if( p <= strLen_n_2_3 )
label c5_elif:
	cmpbe p, strLen_n_2_3
	jcf c5_else
	;	{
	;		// второй отрезок: (l(n-2)..l(n-2)+l(n-3)]
	;		p -= strLen_n_2;	// Сдвигаем
			sub p, strLen_n_2
	;		n = n - 3;
			sub n, dword 3
			jmp c5_begin
	;	}
	;	else
	;	{
label c5_else:
	;		// третий отрезок: остальной щмоток.
	;		p -= strLen_n_2_3;	// Сдвигаем
			sub p, strLen_n_2_3
	;		n = n - 2;
			sub n, dword 2
			jmp c5_begin
	;	}
	;}
label c5_end:

	;if ( p <= 5 )
	cmpbe p, dword 5
	jcf i8_else
	;{
	;	if ( n % 2 )
	mov		tmp,	n
	div		tmp,	dword 2
	mul		tmp,	dword 2
	sub		n,		tmp
	cmpe	n,		dword 0
	jct		i9_else
	;	{
	;		// Нечетный номер строки
	;		ans = sz1[p-1];
			lea tmp, sz1
			jmp i8_result
	;	}
label i9_else:
	;	else
	;	{
	;		// Четный номер строки
	;		ans = sz2[p-1];
			lea tmp, sz2
			jmp i8_result
	;	}
	;}
label i8_else:
	;else
	;{
	;	ans = sz2[p-1];
		lea tmp, sz2
	;}
	
label i8_result:

	add tmp, p
	sub tmp, dword 1
	mov ans, byte [tmp]

	; Выводим ответ
	; Вывод результата на консоль
	xlock
	push	dword 0
	lea		tmp,	numOfChRead
	push	dword tmp
	push	dword 1
	lea		tmp,	ans
	push	dword tmp
	push	dword hStdOutput
	xcall	WriteConsole

	; Задержка перед закрытием
	xlock	tmp
	push	dword 0
	lea		tmp,	numOfChRead
	push	dword tmp
	push	dword 32
	lea		tmp,	buffer
	push	dword tmp
	push	dword hStdInput
	xcall	ReadConsole
	
label exit:
	halt;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	numOfChRead dd 0
	;			1234 5678
	buffer  db '01234567890123456789012345678912' ; size == 32
	pCurChr	dd 0
	ch		db 0
	divider dd 0
	
label ReadDword:
;	xlock
;	push	dword 0
;	push	dword 0
;	push	dword 0
;	push	dword 0
;	xcall	MessageBox

;	Формат считанных даннаых: <число><пробел><число>

	xlock	tmp
	push	dword 0
	lea		tmp,	numOfChRead
	push	dword tmp
	push	dword 32
	lea		tmp,	buffer
	push	dword tmp
	push	dword hStdInput
	xcall	ReadConsole

	cmpe	tmp, dword 0
	jct		exit
	
	; Находим пробел
	lea		pCurChr,	buffer
	mov		divider,	dword 0
label c0:
	mov		ch,			byte [ pCurChr ]
	cmpe	ch, 		byte 32 ; 32 == space
	jct		c0e
	
	add		pCurChr,	dword 1
	add		divider,	dword 1
	
	jmp		c0
label c0e:
	
	; Str1, [0, divider-1] => dword
	lock	K
	lea		tmp,			buffer
	push	dword 0
	sub		divider,		dword 1
	push	divider
	push	tmp
	call	StrToDword
	
	; Str2, [divider+1, numOfChRead-1] => dword
	lock	P
	lea		tmp,			buffer
	add		divider,		dword 2
	push	divider
	sub		numOfChRead,	dword 3
	push	numOfChRead
	push	tmp
	call	StrToDword
	
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; STR -> DWORD
	retaddr dd 0
	
	first   dd 0
	last    dd 0
	pStr	dd 0

	number	dd 0

	pos		dd 0
	bit		dd 0
	char	db 0
	
label StrToDword:
	pop retaddr	; Адрес возврата находится сверху
	pop pStr
	pop last
	pop first
	push retaddr ; Запихиваем его назад
	
	mov		number,	dword 0
	mov		pos,	last	; Устанавливаем счетчик на последний символ
	mov		bit,	dword 1	; Коэффициент
	add		pStr,	last		; Установим на последний символ
label c2:
	cmpl	pos,	first		; Если отведенная нам строка закончилась...
	jct		c2e					; ...то прерываем цикл
	
	mov		char,	byte [pStr]
	movzx	tmp,	char
	sub		tmp,	dword 48	; 48 == '0', tmp = (*pStr) - '0'
	mul		tmp,	bit
	add		number,	tmp
	
	mul		bit,	dword 10	; Увеличиваем коэффициент
	sub		pStr,	dword 1		; Сдвигаем указатель на символ ближе к началу
	sub		pos,	dword 1		; Уменьшаем счетчик
	jmp		c2
label c2e:

	ret	number
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	divisor  dd 0
	pStrOrig dd 0
label WriteDword:
	pop		retaddr		; Адрес возврата находится сверху
	pop		number		; Число для вывода
	push	retaddr		; Запихнем его назад
	
	mov		numOfChRead, dword 0
	lea		pStrOrig,	buffer

	lea		pStr,		buffer
	mov		divisor,	dword 1000000000
label c3:
	cmpe	divisor,	dword 0
	jct		c3e
	
	mov		tmp,		number
	div		tmp,		divisor	; tmp = number/divisor;

	; Если ни один символ еще не записан и результат деления - ноль, пропускаем
	cmpe	pStr,		pStrOrig
	jcf		i2_add
	cmpe	tmp,		dword 0
	jct		i3_next_divisor
	
label i2_add:
	mov		char,		byte tmp
	add		char,		byte 48
	mov		byte [pStr], char
	add		numOfChRead, dword 1

	; number = number % divisor
	; number = number - (number / divisor * divisor)
	mul		tmp,		divisor
	sub		number,		tmp
	add		pStr,		dword 1
	
label i3_next_divisor:
	div		divisor,	dword 10
	jmp		c3
label c3e:
	mov		byte [pStr], byte 0

	; Вывод результата на консоль
	xlock
	push	dword 0
	lea		tmp,	numOfChRead
	push	dword tmp
	push	dword numOfChRead
	lea		tmp,	buffer
	push	dword tmp
	push	dword hStdOutput
	xcall	WriteConsole

	; Задержка перед закрытием
	xlock	tmp
	push	dword 0
	lea		tmp,	numOfChRead
	push	dword tmp
	push	dword 32
	lea		tmp,	buffer
	push	dword tmp
	push	dword hStdInput
	xcall	ReadConsole

	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
arrStrLength dd 0
_01 dd 1
_02 dd 2
_03 dd 3
_04 dd 5
_05 dd 8
_06 dd 13
_07 dd 21
_08 dd 34
_09 dd 55
_10 dd 89
_11 dd 144
_12 dd 233
_13 dd 377
_14 dd 610
_15 dd 987
_16 dd 1597
_17 dd 2584
_18 dd 4181
_19 dd 6765
_20 dd 10946
_21 dd 17711
_22 dd 28657
_23 dd 46368
_24 dd 75025
_25 dd 121393
_26 dd 196418
_27 dd 317811
_28 dd 514229
_29 dd 832040
_30 dd 1346269
_31 dd 2178309
_32 dd 3524578
_33 dd 5702887
_34 dd 9227465
_35 dd 14930352
_36 dd 24157817
_37 dd 39088169
_38 dd 63245986
_39 dd 102334155
_40 dd 165580141
_41 dd 267914296
_42 dd 433494437
_43 dd 701408733
_44 dd 1134903170	; четный максимум, 44
_45 dd 1836311903	; нечетный максимум, 45

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
