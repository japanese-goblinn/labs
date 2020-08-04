; Вариант 11
; Модифицировать прерывание 21h так, чтобы при выводе строки на экран функцией 09h регистр
; букв в строке менялся с верхнего на нижний(или наоборот)

CSEG segment
assume cs:CSEG, ds:CSEG, es:CSEG, ss:CSEG
org 80h
    cmdLength db ? 
    cmdLine db ? 
org 100h
Start:

jmp init

Int_21h_proc proc
    cmp ah, 09h
    je itsOkayToBe9h
    jmp dword ptr cs:[Int_21h_vect]
    itsOkayToBe9h:
        push dx
        push di
        push si
        push es
        push ds
        pop es
        mov	di, dx
        mov si, dx
        veryCoolLoop:
            lodsb
            cmp al, '$'
            je finish
            cmp al, 'a'
            je space
            cmp al, 'e'
            je space
	        cmp al, 'i'
            je space
            cmp al, 'o'
            je space
	        cmp al, 'u'
            je space
	        cmp al, 'A'
            je space
            cmp al, 'E'
            je space
	        cmp al, 'I'
            je space
            cmp al, 'O'
            je space
	        cmp al, 'U'
            je space
            jmp ignore
        space:
            mov	al, ''
        ignore:
           stosb
           jmp veryCoolLoop
        finish:
            pushf
            call dword ptr cs:[Int_21h_vect]
            pop es
            pop si
            pop di
            pop dx
            iret
Int_21h_proc endp

installFlag dw 13579
Int_21h_vect dd ?
msgAlreadyInstalled db 'Already installed', 13, 10, '$'
msgCmdArgsErr db 'Command line arguments are invalid', 13, 10, '$'
msgNotInstalled db 'Not installed', 13,  10, '$'
msgUninstalled db 'Uninstalled', 13, 10, '$'
msgInstalled db 'Installed', 13, 10, '$'

init:
    mov ax, 3521h
    int 21h
    mov word ptr Int_21h_vect, bx
    mov word ptr Int_21h_vect + 2, es

    cmp cmdLength, 0
    je install
    cmp cmdLength, 3
    jne invalidParams

    cmp cmdLine[0], ' '
    jne invalidParams
    cmp cmdLine[1], '-'
    jne invalidParams
    cmp cmdLine[2], 'd'
    jne invalidParams

    cmp es:installFlag, 13579
    jne notInstalled

    mov dx, offset msgUninstalled
    mov ah, 09h
    int 21h
    mov ax, 2521h
    mov ds, word ptr es:Int_21h_vect + 2
    mov dx, word ptr es:Int_21h_vect
    int 21h
    mov ah, 4ch
    int 21h

    invalidParams:
        mov dx, offset msgCmdArgsErr
        jmp toEnd
    alreadyInstalled:
        mov dx, offset msgAlreadyInstalled
        jmp toEnd
    notInstalled:
        mov dx, offset msgNotInstalled
    toEnd:
        mov ah, 09h
        int 21h
        mov ah, 4ch
        int 21h
    install:
        cmp es:installFlag, 13579
        je alreadyInstalled
        mov ah, 09h
        mov dx, offset msgInstalled
        int 21h
        mov ax, 2521h
        mov dx, offset Int_21h_proc
        int 21h
        mov dx, offset Init
        int 27h
    
    mov ah, 35h 
    mov al, 21h 
    int 21h
    mov word ptr Int_21h_vect, bx
    mov word ptr Int_21h_vect + 2, es
    mov ah, 25h
    mov al, 21h
    mov dx, offset Int_21h_proc
    int 21h
    mov dx, offset Init
    int 27h
CSEG ends
end Start