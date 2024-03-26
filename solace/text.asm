BITS    16
ORG     0x7c00
  
jmp start
  
start:
        call clear
        mov ax,cs
        mov ds,ax
        mov si,msg   
        call print

print:
        push ax
        cld
next:
        mov al,[si]
        cmp al,0
        je done
        call printc
        inc si
        jmp next
done:
        jmp $

printc:
        mov ah,0x0e
        int 0x10
        ret

clear:
        mov ah,0x07
        mov al,0x00
        mov bh,0x01
        mov cx,0x0000
        mov dx,0x184f
        int 0x10
        ret

msg:            db        "exampletextlineone", 13, 10, "exampletextlinetwo", 0
times 510 - ($-$$) db 0
dw        0xaa55