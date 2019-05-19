 #file for asm code to calculations
.globl anyAddWithCarry
.type anyAddWithCarry, @function
anyAddWithCarry:
push %rbp
mov %rsp, %rbp
mov %rdi, %rax #pierwszy parametr do rax - pierwsza liczba
#drugi parametr rsi - druga liczba
#trzeci parametr rdx - tablica przechowujaca wynik
#czwarty parametr rcx - dlugosc a
mov %r8, %r10 #dlugsc b
mov %r9, %r11 #piaty parametr dlugosc tablicy na wynik do r11

pushf

cmp %rcx, %r10 #wybieramy krotsza liczbe jako ogarnicznik petli
jg add_aIsSmaller
jmp add_bIsSmaller
add_aIsSmaller:
mov %rcx, %rdi
jmp anyAddWithCarry_Loop
add_bIsSmaller:
mov %r10, %rdi

anyAddWithCarry_Loop:
mov (%rsi,%r10,1), %r8b
mov (%rax,%rcx,1), %r9b
sub $0x30, %r8b 
sub $0x30, %r9b 

dec %rdi

popf

adc %r8b, %r9b

cmp $10, %r9b
jge setCF
jmp clearCF
clearCF:
clc
jmp continue
setCF:
sub $10, %r9b
stc
continue:
pushf

add $0x30, %r9b #konwersja na ascii
mov %r9b, (%rdx,%r11,1)  #odkladamy cyfre do ciagu z wynikiem
dec %r10
dec %rcx
dec %r11
cmp $-1, %rdi
jg anyAddWithCarry_Loop
endCarry:
popf
jnc end_anyAddWithCarry #spr czy przeniesienie wejdzie na ostatnia pozycje
mov $0, %r8b
adc $0x30, %r8b
mov %r8b, (%rdx,%r11,1) 

end_anyAddWithCarry:
clc
mov %rbp, %rsp
pop %rbp
ret

.globl anyAddWithCarryRestPositions
.type anyAddWithCarryRestPositions, @function
anyAddWithCarryRestPositions:
push %rbp
mov %rsp, %rbp
mov %rdi, %rax #pierwszy parametr do rax - liczba
#mov %rsi, %rbx #drugi parametr do rbx - tablica z wynikiem
#trzeci parametr rdx - ilosc iteracji

pushf
anyAddWithCarryRestPositions_Loop:
mov (%rsi,%rdx,1), %r8b
mov (%rax,%rdx,1), %r9b
sub $0x30, %r8b 
sub $0x30, %r9b 

popf

adc %r8b, %r9b

cmp $10, %r9b
jge setCFRestPositions
jmp clearCFRestPositions
clearCFRestPositions:
clc
jmp continueRestPositions
setCFRestPositions:
sub $10, %r9b
stc
continueRestPositions:
pushf

add $0x30, %r9b #konwersja na ascii
mov %r9b, (%rsi,%rdx,1)  #odkladamy cyfre do ciagu z wynikiem
dec %rdx
cmp $-1, %rdx
jg anyAddWithCarryRestPositions_Loop
jmp endCarryRestPositions
endCarryRestPositions:
popf
jnc end_anyAddWithCarryRestPositions #spr czy przeniesienie wejdzie na ostatnia pozycje
mov $0, %r8b
adc $0x30, %r8b
mov %r8b, (%rsi,%rdx,1) 

end_anyAddWithCarryRestPositions:
clc
mov %rbp, %rsp
pop %rbp
ret

.globl anySubWithBorrow
.type anySubWithBorrow, @function
anySubWithBorrow:
push %rbp
mov %rsp, %rbp
mov %rdi, %rax #pierwszy parametr do rax - pierwsza liczba
#mov %rsi, %rbx #drugi parametr do rbx - druga liczba
#trzeci parametr rdx - tablica przechowujaca wynik
#czwarty parametr rcx - dlugosc a
mov %r8, %r10 #dlugsc b
mov %r9, %r11 #piaty parametr dlugosc tablicy na wynik do r11
mov $0x39, %r14b

pushf

cmp %rcx, %r10 #wybieramy krotsza liczbe jako ogarnicznik petli
jg aIsSmaller
jmp bIsSmaller
aIsSmaller:
mov %rcx, %rdi
jmp anySubWithBorrow_Loop
bIsSmaller:
mov %r10, %rdi

anySubWithBorrow_Loop:
mov (%rsi,%r10,1), %r8b
mov (%rax,%rcx,1), %r9b

dec %rdi

popf

sbb %r8b, %r9b

cmpb $0x39, %r9b
ja sub_setCF
jmp sub_clearCF
sub_clearCF:
add $0x30, %r9b #konwersja na ascii
clc
jmp sub_continue
sub_setCF:
sub $0xC6, %r9b
stc
sub_continue:
pushf

mov %r9b, (%rdx,%r11,1)  #odkladamy cyfre do ciagu z wynikiem
dec %r10
dec %rcx
dec %r11
cmp $-1, %rdi
jg anySubWithBorrow_Loop
sub_endBorrow:
popf
jnc end_anySubWithBorrow #spr czy przeniesienie wejdzie na ostatnia pozycje
mov $0, %r8b
adc $0x30, %r8b
mov %r8b, (%rdx,%r11,1) 

end_anySubWithBorrow:
clc
mov %rbp, %rsp
pop %rbp
ret

.globl anySubWithBorrowRestPositions
.type anySubWithBorrowRestPositions, @function
anySubWithBorrowRestPositions:
push %rbp
mov %rsp, %rbp
mov %rdi, %rax #pierwszy parametr do rax - liczba
#mov %rsi, %rbx #drugi parametr do rbx - tablica z wynikiem
#trzeci parametr rdx - ilosc iteracji

pushf
anySubWithBorrowRestPositions_Loop:
mov (%rsi,%rdx,1), %r8b
mov (%rax,%rdx,1), %r9b

popf

sbb %r8b, %r9b

cmpb $0x39, %r9b
ja sub_setCFRestPositions
jmp sub_clearCFRestPositions
sub_clearCFRestPositions:
add $0x30, %r9b #konwersja na ascii
clc
jmp sub_continueRestPositions
sub_setCFRestPositions:
sub $0xC6, %r9b
stc
sub_continueRestPositions:
pushf

mov %r9b, (%rsi,%rdx,1)  #odkladamy cyfre do ciagu z wynikiem
dec %rdx
cmp $-1, %rdx
jg anySubWithBorrowRestPositions_Loop
jmp sub_endBorrowRestPositions
sub_endBorrowRestPositions:
popf
jnc end_anySubWithBorrowRestPositions #spr czy przeniesienie wejdzie na ostatnia pozycje
mov $0, %r8b
adc $0x30, %r8b
mov %r8b, (%rsi,%rdx,1) 

end_anySubWithBorrowRestPositions:
clc
mov %rbp, %rsp
pop %rbp
ret
