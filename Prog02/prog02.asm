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
fn1        DWORD    0
fn2        DWORD    1
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
     mov  eax, NUM_MAX
     call WriteDec
     mov  edx, OFFSET instruct2
     call WriteString
     call Crlf

; prompt user for input
InputLoop:
     mov  edx, OFFSET prompt
     call WriteString
     call ReadInt
     mov  nums, eax

; check if user input < 1 or > NUM_MAX
     cmp  eax, 1
     jb   InvalidInput
     cmp  eax, NUM_MAX
     ja   InvalidInput
     jmp  InputOk

; input was invalid, display error and reprompt
InvalidInput:
     mov  edx, OFFSET invalid1
     call WriteString
     mov  eax, NUM_MAX
     call WriteDec
     mov  edx, OFFSET invalid2
     call WriteString
     call Crlf
     jmp  InputLoop

InputOk:
     call Crlf

; display first Fibonacci number and increment column number
     mov  eax, 1
     call WriteDec
     mov  al, TAB
     call WriteChar
     call WriteChar
     inc  columnNo

; set loop counter and decrement by one
; only enter loop if ecx > 0
     mov  ecx, nums
     dec  ecx
     cmp  ecx, 0
     je   EndLoop

; display remaining Fibonacci numbers
CalculateFib:
     mov  eax, fn1
     add  eax, fn2
     call WriteDec
     inc  columnNo
     mov  ebx, fn2
     mov  fn1, ebx
     mov  fn2, eax

; add linefeed and no padding if 5th column
     cmp  columnNo, COLUMN_MAX
     jb   Padding
     call Crlf
     mov  columnNo, 0
     jmp  EndLinefeed

; add one tab if more than 8 digits, 2 tabs otherwise
Padding:
     mov  al, TAB
     call WriteChar
     cmp  eax, 9999999
     ja  EndLinefeed
     call WriteChar

EndLinefeed:
     loop CalculateFib

; say goodbye
EndLoop:
     call Crlf
     call Crlf
     mov  edx, OFFSET goodbye
     call WriteString
     call Crlf

; exit to operating system
     exit

main ENDP

END main