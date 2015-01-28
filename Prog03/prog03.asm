TITLE Programming Assignment 3 (prog03.asm)

; Author: David Rigert
; Course / Project ID: CS271 / #03               Date: 1/26/2015
; Description: Displays my name and program title to output screen,
;              gets the user's name and greets the user,
;              prompts the user to enter a positive number less than
;              or equal to 100, and accumulates the entered numbers 
;              until a negative is entered (and discarded).
;              Calculates the rounded integer average of the non-negative numbers,
;              displays the number of non-negative numbers entered
;              (or a special message if none entered),
;              the sum of the non-negative numbers,
;              the rounded integer average, and a parting message.

INCLUDE Irvine32.inc

.data
DOT = 2Eh           ; a period char
NUM_MAX = 100       ; maximum user input value
NUM_MIN = 0         ; minimum user input value
NAME_MAX_LEN = 30   ; maximum length of user name

intro       BYTE    "Integer Accumulator    by David Rigert", 0
namePrompt1 BYTE    "Enter your name(up to ", 0
namePrompt2 BYTE     " characters) :", 0
userName    BYTE    (NAME_MAX_LEN + 1) DUP(0)
greeting    BYTE    "Hello, ", 0
instruct1   BYTE    "Enter a series of numbers between ", 0
instruct2   BYTE    " and ", 0
instruct3   BYTE    ", or a negative to exit.", 13, 10
            BYTE    "When you are finished, enter a negative number to see the results.", 0
prompt      BYTE    "Enter a number: ", 0
invalid1    BYTE    "Invalid number. Enter a number from ", 0
invalid2    BYTE    " to ", 0
noNumbers   BYTE    "No numbers were entered.", 0
countText   BYTE    "Numbers entered: ", 0
sumText     BYTE    "Sum of numbers: ", 0
avgText     BYTE    "Rounded average: ", 0
numInput    DWORD   ?    ; stores the current user input
numCount    DWORD   0    ; stores the number of numbers entered by user
total       DWORD   0    ; accumulates the user input
average     DWORD   ?    ; stores total / numCount
goodbye     BYTE    "Thank you for playing. Have a good day, ", 0

.code
main PROC

; display my name and program title
     mov  edx, OFFSET intro
     call WriteString
     call Crlf
     call Crlf

; prompt user for name, up to userName - 1 characters
     mov  edx, OFFSET namePrompt1
     call WriteString
     mov  eax, SIZEOF userName - 1  ; - 1 for null terminator
     call WriteDec
     mov  edx, OFFSET namePrompt2
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
     mov  eax, NUM_MIN
     call WriteDec       ; display minimum number value
     mov  edx, OFFSET instruct2
     call WriteString
     mov  eax, NUM_MAX
     call WriteDec       ; display maximum number value
     mov  edx, OFFSET instruct3
     call WriteString
     call Crlf
     call Crlf

; prompt user for input
InputLoop:
     mov  edx, OFFSET prompt
     call WriteString
     call ReadInt       ; read as signed integer to detect negatives
     mov  numInput, eax

; check if user input is negative
     cmp  eax, NUM_MIN
     jl   NegativeInput ; jump if less than NUM_MIN

; check if user input > NUM_MAX
     cmp  eax, NUM_MAX
     jg   InvalidInput  ; jump if greater than NUM_MAX
     
; input was valid if execution reaches here
     add  eax, total     ; add running total to entered number
     mov  total, eax     ; copy new total to total variable
     inc  numCount       ; add 1 to input counter
     jmp  InputLoop      ; get another number from the user

; input was invalid, display error message and reprompt
InvalidInput:
     mov  edx, OFFSET invalid1
     call WriteString
     mov  eax, NUM_MIN
     call WriteDec       ; display minimum number value
     mov  edx, OFFSET invalid2
     call WriteString
     mov  eax, NUM_MAX
     call WriteDec       ; display maximum number value
     mov  al, DOT
     call WriteChar      ; display a period character
     call Crlf
     jmp  InputLoop ; go back to input prompt

; Input was negative. 
NegativeInput:
; Test if any values were entered
     cmp  numCount, 0
     je   NoInput   ; jump if no numbers were entered
     call Crlf

; display number of numbers entered
     mov  edx, OFFSET countText
     call WriteString
     mov  eax, numCount
     call WriteDec
     call Crlf
     
; display the sum of all positive numbers entered
     mov  edx, OFFSET sumText
     call WriteString
     mov  eax, total
     call WriteDec
     call Crlf

; display the average number, rounded to 3 decimal places
     mov  edx, OFFSET avgText
     call WriteString

; calculate average
     mov  eax, total     ; copy the total to EAX
     cdq                 ; fill EDX
     div  numCount       ; divide total by numCount
     call WriteDec       ; Display integer part of average
     mov  al, DOT     
     call WriteChar      ; Display a decimal point

; display the fractional part of the average, up to 3 digits
     mov  ecx, 3         ; set counter to 3
FractionalLoop:
     mov  eax, edx       ; copy remainder to EAX
     cdq                 ; fill EDX
     mov  ebx, 10
     mul  ebx            ; multiply remainder by 10
     div  numCount       ; divide remainder by numCount
     call WriteDec       ; display integer part in EAX
     loop FractionalLoop ; decrement ECX and go back to beginning of loop
     call Crlf           ; end with a newline
     jmp  EndOfReport    ; jump to the goodbye message

NoInput:
; display message indicating no numbers were entered
     mov  edx, OFFSET noNumbers
     call WriteString
     call Crlf

EndOfReport:
; say goodbye
     call Crlf
     mov  edx, OFFSET goodbye
     call WriteString
     mov  edx, OFFSET userName
     call WriteString
     mov  al, DOT
     call WriteChar     ; display a period character
     call Crlf

; exit to operating system
     exit

main ENDP

END main