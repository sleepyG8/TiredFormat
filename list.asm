; Listing all entries inside the tired format
; Later on we will use the struct to pull data as well
; Sleepy 2025

extern WriteConsoleA :PROC
extern GetStdHandle :PROC

.data
written dq 0
newline    db 0Dh,0Ah,0
.code

writeConsole PROC

    mov rcx, -11       
    sub rsp, 20h     
    call GetStdHandle
    add rsp, 20h
    mov rcx, rax             ; Handle in RCX
    
    ;mov rdx, r10   
    lea r9, written  

    sub rsp, 28h 
    xor rax, rax
    mov qword ptr [rsp+20h], rax     ; shadow space              
    call WriteConsoleA
    add rsp, 28h

    ret
writeConsole ENDP


tiredWalk PROC

test rcx, rcx ; is 0 ; check if rcx is null and return
jz errorCode

mov r12, rcx ; getting en struct its 10 entries at 0 bytes
test r12, r12      ; error check
jz errorCode

mov r13, 0
loopThrough:       ; loop through entries

cmp r13, 10
jge endLoop           ; exit when i >= 10

mov  r10, r13       
imul r10, 68        ; r10 * 68 = r10
lea  r10, [r12+r10] ; rdx+r13*68  ; 68 is sizeof(entry)

lea rdx, [r10+4] ; getting name from en struct entry

mov al, [rdx]
test al, al
jz endLoop

mov r8, 10;
call writeConsole ; write newline

lea rdx, newline
mov r8, 2;
call writeConsole ; write newline

inc r13 ; +1
jmp loopThrough

errorCode:
mov rax, 0
ret

endLoop:
mov rax, 1
ret
tiredWalk ENDP
END
