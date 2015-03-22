TITLE Programming Assignment 2 (prog02.asm)

; Author: David Rigert
; Course / Project ID : CS271 / #02               Date: 1 / 15 / 2015
; Description: Displays my name and program title to output screen,
;              gets the user's name and greets the user,
;              prompts the user to enter the number of Fibonacci numbers,
;              to display (between 1 and 46), gets the input (n)
;              and checks that it is between 1 and 46,
;              calculates and displays all Fibonacci numbers up to 
;              the nth term, and displays goodbye with the user's name.

INCLUDE Irvine32.inc

.data
NUM_MAX = 46
COLUMN_MAX = 5
TAB = 9

intro      BYTE     "Fibonacci Numbers    by David Rigert", 0
namePrompt BYTE     "Enter your name (up to 30 characters): ", 0
userName   BYTE     31 DUP(0)
greeting   BYTE     "Hello, ", 0
instruct1  BYTE     "Enter the number of Fibonacci terms to display, between 1 and ", 0
instruct2  BYTE     ".", 0
prompt     BYTE     "Number of Fibonacci terms: ", 0
invalid1   BYTE     "Invalid number. Enter a number from 1 to ", 0
invalid2   BYTE     ".", 0
nums       DWORD    ?
fn2        DWORD    0
fn1        DWORD    1
columnNo   DWORD    0
goodbye    BYTE     "So long and thanks for all the fish.", 0

.code
main PROC

; display my name and program title
     mov  edx, OFFSET intro
     call WriteString
     call Crlf
     call Crlf

; prompt user for name
     mov  edx, OFFSET namePrompt
     call WriteString

; get the user's name
     mov  edx, OFFSET userName
     mov  ecx, SIZEOF userName
     call ReadString

; display greeting
     mov  edx, OFFSET greeting
     call WriteString
     mov  edx, OFFSET userName
     call WriteString
     call Crlf

; display instructions
     mov  edx, OFFSET instruct1
     call WriteString
     mov  eax, NUM_MAX   ; max number from constant
     call WriteDec
     mov  edx, OFFSET instruct2
     call WriteString
     call Crlf

; prompt user for input
InputLoop:
     mov  edx, OFFSET prompt
     call WriteString
     call ReadInt
     mov  nums, eax      ; number of Fibonacci nums to display

; check if user input < 1 or > NUM_MAX
     cmp  eax, 1
     jb   InvalidInput   ; below 1
     cmp  eax, NUM_MAX
     ja   InvalidInput   ; above NUM_MAX (46)
     jmp  InputOk        ; else, between 1 and NUM_MAX

; input was invalid, display error and reprompt
InvalidInput:
     mov  edx, OFFSET invalid1
     call WriteString
     mov  eax, NUM_MAX   ; max number from constant
     call WriteDec
     mov  edx, OFFSET invalid2
     call WriteString
     call Crlf
     jmp  InputLoop      ; try input again

InputOk:                 ; input was valid
     call Crlf

; set ECX variable to user input value
     mov  ecx, nums

; call subroutine to print Fibonacci numbers
     call printFibNums

; say goodbye
     call Crlf
     mov  edx, OFFSET goodbye
     call WriteString
     call Crlf

; exit to operating system
     exit

main ENDP

;-----------------------------------------------------------------------------
; Computes and displays up to the nth Fibonacci number to the console.
; Receives: ECX = the number of Fibonacci numbers to display
; Preconditions: ECX > 0
;-----------------------------------------------------------------------------
printFibNums PROC
; push all registers and variables modified by proc to the stack
; this allows the procedure to be called multiple times within the same program
     pushad         ; push all general registers
     push fn2       ; push initial Fibonacci numbers
     push fn1
     push columnNo  ; for counting columns

; display first Fibonacci number, increment column number and decrement ecx
     mov  eax, 1         ; first number is 1
     call WriteDec
     mov  al, TAB        ; display 2 tab characters
     call WriteChar
     call WriteChar
     inc  columnNo       ; add one to column number
     dec  ecx            ; decrement counter register

; only enter loop if ecx != 0
     cmp  ecx, 0
     je   EndLoop   ; ecx == 0, so jump to end

; display remaining Fibonacci numbers
CalculateFib:
     mov  eax, fn2  ; copy f(n-2) to eax
     add  eax, fn1  ; add f(n-1) to eax to get f(n)
     call WriteDec  ; print number
     inc  columnNo  ; increment the column counter
     mov  ebx, fn1  ; store f(n-1) in temp register
     mov  fn2, ebx  ; copy f(n-1) to f(n-2)
     mov  fn1, eax  ; copy f(n) to f(n-1)

; add linefeed and no padding if 5th column
     cmp  columnNo, COLUMN_MAX
     jb   Padding        ; jump if not 5th column
     call Crlf           ; print linefeed
     mov  columnNo, 0    ; reset column counter to 0
     jmp  EndLinefeed

; add one tab if more than 8 digits, 2 tabs otherwise
Padding:
     mov  al, TAB
     call WriteChar      ; print tab
     cmp  fn1, 9999999   ; test if more than 7 digits
     ja   EndLinefeed    ; jump if > 7 digits
     call WriteChar      ; otherwise print one more tab

EndLinefeed:
     loop CalculateFib   ; subtract one from ecx and jump if > 0

EndLoop:
     call Crlf

; restore all registers and variables
     pop  columnNo
     pop  fn1
     pop  fn2
     popad

; return to caller
     ret

printFibNums ENDP

END main