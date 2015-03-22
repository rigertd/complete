TITLE Programming Assignment 1 (prog01.asm)

; Author: David Rigert
; Course / Project ID : CS271 / #01               Date: 1 / 9 / 2015
; Description: Displays my name and program title to output screen,
;              displays instructions for the user,
;              prompts the user for 2 numbers,
;              checks if number 1 is larger and if number 2 is 0,
;              calculates the sum, difference, product,
;              and integer quotient and remainder of the numbers,
;              asks user if they want to quit,
;              and displays a terminating message if they do.

INCLUDE Irvine32.inc

.data

intro     BYTE      "Assembly Arithmetic    by David Rigert", 0
instruct  BYTE      "Enter 2 positive numbers and I'll show the sum, difference, "
          BYTE      "product, quotient,", 10, 13, "and remainder. "
          BYTE      "The 1st number must be larger and the 2nd cannot be 0.", 0
prompt1   BYTE      "First number: ", 0
prompt2   BYTE      "Second number: ", 0
operand1  DWORD     ?
operand2  DWORD     ?
invalid   BYTE      "The first number must be larger than the second.", 0
divide0   BYTE      "The second number cannot be 0.", 0
plus      BYTE      " + ", 0
equal     BYTE      " = ", 0
minus     BYTE      " - ", 0
times     BYTE      " * ", 0
divide    BYTE      " / ", 0
remainMsg BYTE      " remainder ", 0
sum       DWORD     ?
diff      DWORD     ?
product   DWORD     ?
quotient  DWORD     ?
remain    DWORD     ?
quitMsg   BYTE      "Do you want to quit? (y/n) ", 0
response  BYTE      ?
goodbye   BYTE      "Aren't I good at math? So long!", 0

.code
main PROC

; show introduction
     mov  edx, OFFSET intro
     call WriteString
     call Crlf
     call Crlf

; show instructions
     mov  edx, OFFSET instruct
     call WriteString
     call Crlf
     call Crlf

getInput :

; get number 1
     mov  edx, OFFSET prompt1
     call WriteString
     call ReadInt
     mov  operand1, eax

; get number 2
     mov  edx, OFFSET prompt2
     call WriteString
     call ReadInt
     mov  operand2, eax
     call Crlf

; check if number2 == 0
     mov  eax, operand2
     cmp  eax, 0
     je   divideByZero

; check if number 2 < number 1. operand2 is already in eax
     cmp  eax, operand1
     jae  invalidInput
     jmp  validInput

invalidInput:
; display warning message about invalid input and jump back to prompt
     mov  edx, OFFSET invalid
     call WriteString
     call Crlf
     call Crlf
     jmp  getInput

divideByZero:
; display warning message about divide by zero and jump back to prompt
     mov  edx, OFFSET divide0
     call WriteString
     call Crlf
     call Crlf
     jmp  getInput

validInput:

; calculate the sum
     mov  eax, operand1
     add  eax, operand2
     mov  sum, eax

; calculate the difference
     mov  eax, operand1
     sub  eax, operand2
     mov  diff, eax

; calculate the product
     mov  eax, operand1
     mul  operand2
     mov  product, eax

; calculate the quotient and remainder
     mov  eax, operand1
     cdq  ; convert eax to qword to ensure edx is clear
     div  operand2
     mov  quotient, eax
     mov  remain, edx

; display the sum
     mov  eax, operand1
     call WriteDec
     mov  edx, OFFSET plus
     call WriteString
     mov  eax, operand2
     call WriteDec
     mov  edx, OFFSET equal
     call WriteString
     mov  eax, sum
     call WriteDec
     call Crlf

; display the difference
     mov  eax, operand1
     call WriteDec
     mov  edx, OFFSET minus
     call WriteString
     mov  eax, operand2
     call WriteDec
     mov  edx, OFFSET equal
     call WriteString
     mov  eax, diff
     call WriteDec
     call Crlf

; display the product
     mov  eax, operand1
     call WriteDec
     mov  edx, OFFSET times
     call WriteString
     mov  eax, operand2
     call WriteDec
     mov  edx, OFFSET equal
     call WriteString
     mov  eax, product
     call WriteDec
     call Crlf

; display the quotient and remainder
     mov  eax, operand1
     call WriteDec
     mov  edx, OFFSET divide
     call WriteString
     mov  eax, operand2
     call WriteDec
     mov  edx, OFFSET equal
     call WriteString
     mov  eax, quotient
     call WriteDec
     mov  edx, OFFSET remainMsg
     call WriteString
     mov  eax, remain
     call WriteDec
     call Crlf
     call Crlf

; ask to quit and convert input to uppercase (pg. 192 in textbook)
     mov  edx, OFFSET quitMsg
     call WriteString
     call ReadChar
     and  al, 11011111b
     mov  response, al
     call Crlf

; check response and jump to getInput if anything but 'Y'
     mov  al, response
     cmp  al, 'Y'
     jne  getInput

; say goodbye
     call Crlf
     mov  edx, OFFSET goodbye
     call WriteString
     call Crlf

; exit to operating system
     exit

main ENDP

END main