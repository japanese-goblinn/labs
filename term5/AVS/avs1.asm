.model small
Locals @@

    Descriptor struc
        limit dw 0
        address_low dw 0
        address_high db 0
        access db 0
        limit_extention db 0
        address_extention db 0
    ends

    CODE_SELECTOR       equ (gdt_code_seg - gdt_0)
    STACK_SELECTOR      equ (gdt_stack_seg - gdt_0)
    DATA_SELECTOR       equ (gdt_data_seg - gdt_0)
    VIDEO_SELECTOR      equ (gdt_video_seg - gdt_0)
    RMODE_CODE_SELECTOR equ (gdt_rmode_code - gdt_0)
    RMODE_DATA_SELECTOR equ (gdt_rmode_data - gdt_0)

    CMOS_PORT equ 70h
    REAL_MODE_VIDEO_ADDRESS equ 0b8000h

    CODE_SEGMENT_LIMIT  equ 1024
    STACK_SEGMENT_LIMIT equ 1024
    VIDEO_SEGMENT_LIMIT equ 4000
    DATA_SEGMENT_LIMIT  equ 0ffffh

    ACC_PRESENT  EQU 10000000b
    ACC_CSEG     EQU 00011000b
    ACC_DSEG     EQU 00010000b
    ACC_EXPDOWN  EQU 00000100b
    ACC_CONFORM  EQU 00000100b
    ACC_DATAWR   EQU 00000010b


    CODE_SEGMENT_ACCESS  equ ACC_PRESENT OR ACC_CSEG OR ACC_CONFORM
    STACK_SEGMENT_ACCESS equ ACC_PRESENT OR ACC_DSEG OR ACC_DATAWR OR ACC_EXPDOWN
    DATA_SEGMENT_ACCESS  equ ACC_PRESENT OR ACC_DSEG OR ACC_DATAWR
    VIDEO_SEGMENT_ACCESS equ ACC_PRESENT OR ACC_DSEG OR ACC_DATAWR

.data
    rmode_ss dw ?
    rmode_ds dw ?
    rmode_es dw ?
    rmode_fs dw ?
    rmode_gs dw ?
    rmode_sp dw ?

    ;регистр GDT
    gdtr label fword
    gdt_limit   dw GDT_SIZE
    gdt_address dd ?

    GDT equ $
    gdt_0           Descriptor <0,0,0,0,0,0>
    gdt_code_seg    Descriptor ?
    gdt_stack_seg   Descriptor ?
    gdt_data_seg    Descriptor ?
    gdt_video_seg   Descriptor ?
    gdt_rmode_code  Descriptor ?
    gdt_rmode_data  Descriptor ?
    GDT_SIZE equ ($ - GDT)

    protected_message db "Message from protected mode", '$'
    pre_protected_message db "Entered protected mode.", 13, 10, '$'
    press_any_to_continue db "Press ESC to continue...", '$'
    post_protected_message db "Exited protected mode.", 13, 10, '$'
    to_continue_message db "Press n|N to exit or any key to continue: $"

.stack STACK_SEGMENT_LIMIT
.code
.386p

InitDesctiptor macro Descriptor, Limit, Access
    push eax ebx
    mov bx, ax
    shr eax, 16

    mov [Descriptor].address_low, bx
    mov [Descriptor].address_high, al
    mov [Descriptor].address_extention, ah
    mov [Descriptor].limit, Limit
    mov [Descriptor].limit_extention, 0
    mov [Descriptor].access, Access
    pop ebx eax
endm

InitGDT proc

    xor eax, eax
    mov ax, cs
    shl eax, 4
    InitDesctiptor gdt_code_seg CODE_SEGMENT_LIMIT CODE_SEGMENT_ACCESS

    xor eax, eax
    mov ax, ss
    shl eax, 4
    InitDesctiptor gdt_stack_seg STACK_SEGMENT_LIMIT STACK_SEGMENT_ACCESS

    xor eax, eax
    mov ax, ds
    shl eax, 4
    InitDesctiptor gdt_data_seg DATA_SEGMENT_LIMIT DATA_SEGMENT_ACCESS


    mov eax, REAL_MODE_VIDEO_ADDRESS
    InitDesctiptor gdt_video_seg VIDEO_SEGMENT_LIMIT VIDEO_SEGMENT_ACCESS

    xor eax, eax
    mov ax, cs
    shl eax, 4
    InitDesctiptor gdt_rmode_code 0ffffh CODE_SEGMENT_ACCESS

    xor eax, eax
    mov ax, ds
    shl eax, 4
    InitDesctiptor gdt_rmode_data 0ffffh DATA_SEGMENT_ACCESS
    ret
endp

EnterProtectedMode proc
    push eax ecx edx

    xor eax, eax
    mov ax, seg GDT
    shl eax, 4
    xor edx, edx
    lea dx, GDT
    add eax, edx

    mov gdt_address, eax
    cli

    mov al, 8fh           ; запрещаем немаскируемые прерывания
    out CMOS_PORT, al

    lgdt gdtr   

    mov rmode_sp, sp

    mov eax, cr0
    or eax, 1
    mov cr0, eax

    db 0eah            ; машинный код команды jmp
    dw offset @@flush  ; смещение метки перехода в сегменте команд
    dw CODE_SELECTOR   ; селектор сегмента в таблице GDT

  @@flush:
    mov ax, VIDEO_SELECTOR
    mov es, ax
    mov ax, DATA_SELECTOR
    mov ds, ax
    mov ax, STACK_SELECTOR
    mov ss, ax

    pop edx ecx eax
    ret
endp

ExitProtectedMode proc
    push eax ebx edi

    mov ax, RMODE_DATA_SELECTOR
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, cr0
    and al, 0feh  
    mov cr0, eax

    db 0eah      
    dw @@flush
    rmode_cs dw ?

  @@flush:
    mov ax, 000dh  
    out CMOS_PORT, al

    sti
    mov ss, rmode_ss
    mov ds, rmode_ds
    mov es, rmode_es
    mov fs, rmode_fs
    mov gs, rmode_gs
    mov sp, rmode_sp

    pop ebx ecx eax
    ret
endp

InitRealModeSegments proc
    mov rmode_ss, ss
    mov rmode_ds, ds
    mov rmode_es, es
    mov rmode_fs, fs
    mov rmode_gs, gs
    mov rmode_cs, cs
    ret
endp

WriteString proc
    push eax ebx edi
    mov cx, 255
  @@_loop:
    mov al, [bx]
    cmp al, '$'
    je @@_end
    mov es:[di], word ptr ax
    add di, 2
    inc bx
    loop @@_loop

  @@_end:
    pop edi ebx eax
    ret
endp

WaitToPressEsc proc
    push ax
    xor al, al
    @_wait_for_escape:
        in al, 060h
        cmp al, 01h   ; esc scancode
        jne @_wait_for_escape
    pop ax
    ret
endp

NewLine proc
    push ax
    push dx

    mov dl, 10
    mov ah, 02h
    int 21h

    pop dx
    pop ax
    ret
endp

MAIN:
    mov ax, @data
    mov ds, ax

  @@main_loop:
    call InitRealModeSegments
    call InitGDT

	lea dx, pre_protected_message               
	mov ah, 09h
	int 21h

    call EnterProtectedMode

    mov ah, 10
    lea bx, protected_message
    mov di, 0
    call WriteString

    mov ah, 12
    lea bx, press_any_to_continue
    mov di, 160
    call WriteString
       
    call WaitToPressEsc

    call ExitProtectedMode

    lea dx, post_protected_message               
	mov ah, 09h
	int 21h

    lea dx, to_continue_message               
    mov ah, 09h
    int 21h

    push ax
    xor al, al
    mov ah, 01h
    int 21h
    call NewLine
    cmp al, 'N'
    je @end_main
    cmp al, 'n'
    je @end_main
    pop ax
    jmp @@main_loop

  @end_main:
    mov ax, 4c00h
    int 21h
end MAIN