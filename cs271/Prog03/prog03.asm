TITLE Programming Assignment 3 (prog03.asm)

; Author: David Rigert
; CS271 / #03               Date: 1/26/2015
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
namePrompt1 BYTE    "Enter your name (up to ", 0
namePrompt2 BYTE     " characters): ", 0
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

;------------------------------------------------------------------------------
; This section displays the program introduction, prompts the user for their
; name, and greets the user by name.
;------------------------------------------------------------------------------
     mov  edx, OFFSET intro
     call WriteString              ; display my name and program title
     call Crlf
     call Crlf

; prompt user for name, up to userName - 1 characters
     mov  edx, OFFSET namePrompt1
     call WriteString              ; display first half of prompt for name
     mov  eax, SIZEOF userName - 1 ; - 1 for null terminator
     call WriteDec                 ; display maximum name length
     mov  edx, OFFSET namePrompt2
     call WriteString              ; display second half of prompt for name

; get the user's name
     mov  edx, OFFSET userName     ; store input in userName
     mov  ecx, SIZEOF userName     ; maximum input length
     call ReadString               ; read user input

; display greeting
     mov  edx, OFFSET greeting
     call WriteString    ; display greeting message
     mov  edx, OFFSET userName
     call WriteString    ; address to user's name
     call Crlf

;------------------------------------------------------------------------------
; This section displays the program instructions.
;------------------------------------------------------------------------------
; display instructions
     mov  edx, OFFSET instruct1
     call WriteString    ; display first third of instructions
     mov  eax, NUM_MIN   ; minimum acceptable number
     call WriteDec       ; display minimum number value
     mov  edx, OFFSET instruct2
     call WriteString    ; display second third of instructions
     mov  eax, NUM_MAX   ; maximum acceptable number
     call WriteDec       ; display maximum number value
     mov  edx, OFFSET instruct3
     call WriteString    ; display last third of instructions
     call Crlf
     call Crlf

;------------------------------------------------------------------------------
; This section is the input loop where the user is prompted to enter numbers
; until a negative number is entered. The program counts the number of numbers
; and the cumulative sum as the numbers are entered.
; Blank input is counted as 0 due to limitations of ReadInt.
;------------------------------------------------------------------------------
; prompt user for input
InputLoop:
     mov  edx, OFFSET prompt
     call WriteString    ; display prompt for user input
     call ReadInt        ; read as signed integer to detect negatives
     mov  numInput, eax  ; store in numInput

; check if user input is negative
     cmp  eax, NUM_MIN   ; eax < NUM_MIN
     jl   NegativeInput  ; jump if less than NUM_MIN

; check if user input > NUM_MAX
     cmp  eax, NUM_MAX   ; eax > NUM_MAX
     jg   InvalidInput   ; jump if greater than NUM_MAX
     

; input was valid if execution reaches here
     add  eax, total     ; add running total to entered number
     mov  total, eax     ; copy new total to total variable
     inc  numCount       ; add 1 to input counter
     jmp  InputLoop      ; get another number from the user

;------------------------------------------------------------------------------
; Execution jumps here if the user enters invalid input.
;------------------------------------------------------------------------------
InvalidInput:
     mov  edx, OFFSET invalid1
     call WriteString    ; display first half of invalid input error message
     mov  eax, NUM_MIN
     call WriteDec       ; display minimum number value
     mov  edx, OFFSET invalid2
     call WriteString    ; display second half of invalid input error message
     mov  eax, NUM_MAX
     call WriteDec       ; display maximum number value
     mov  al, DOT
     call WriteChar      ; display a period character
     call Crlf
     jmp  InputLoop      ; go back to input prompt

;------------------------------------------------------------------------------
; Execution jumps here when the user enters a negative number.
; The count is checked in case the first number entered was negative.
;------------------------------------------------------------------------------
NegativeInput:
; Test if any values were entered
     cmp  numCount, 0    ; numCount == 0
     je   NoInput        ; jump to NoInput if no numbers were entered
     call Crlf

;------------------------------------------------------------------------------
; This section displays the count, sum, and average of the numbers entered.
; The average is rounded to 3 decimal places.
;------------------------------------------------------------------------------
; display number of numbers entered
     mov  edx, OFFSET countText
     call WriteString    ; display label for count
     mov  eax, numCount
     call WriteDec       ; display number count
     call Crlf
     
; display the sum of all positive numbers entered
     mov  edx, OFFSET sumText
     call WriteString    ; display label for sum
     mov  eax, total
     call WriteDec       ; display sum
     call Crlf

; display the average number, rounded to 3 decimal places
     mov  edx, OFFSET avgText
     call WriteString    ; display label for average

; calculate average
     mov  eax, total     ; copy the total to EAX
     cdq                 ; fill EDX
     div  numCount       ; divide total by numCount
     call WriteDec       ; Display integer part of average
     mov  al, DOT     
     call WriteChar      ; Display a decimal point

; display the fractional part of the average, up to 3 digits
     mov  ecx, 2         ; set counter to 2
FractionalLoop:
     mov  eax, edx       ; copy remainder to EAX
     cdq                 ; fill EDX
     mov  ebx, 10
     mul  ebx            ; multiply remainder by 10
     div  numCount       ; divide remainder by numCount
     call WriteDec       ; display integer part in EAX
     loop FractionalLoop ; decrement ECX and go back to beginning of loop
     call RoundUp        ; round the last digit to the nearest integer
     call WriteDec       ; write the last digit
     call Crlf           ; end with a newline
     jmp  EndOfReport    ; jump to the goodbye message

;------------------------------------------------------------------------------
; Execution only reaches this section when the first number entered is negative.
;------------------------------------------------------------------------------
NoInput:
; display message indicating no numbers were entered
     mov  edx, OFFSET noNumbers
     call WriteString    ; display message about no numbers being entered
     call Crlf

;------------------------------------------------------------------------------
; This section displays the goodbye message and the user's name.
;------------------------------------------------------------------------------
EndOfReport:
; say goodbye
     call Crlf
     mov  edx, OFFSET goodbye
     call WriteString    ; display the goodbye message
     mov  edx, OFFSET userName
     call WriteString    ; display the user's name
     mov  al, DOT
     call WriteChar      ; display a period character
     call Crlf

; exit to operating system
     exit

main ENDP

;------------------------------------------------------------------------------
; Rounds the value in EAX to the nearest integer based on the remainder in EDX
; and the divisor in numCount.
; Receives:    EAX contains the number to be rounded
;              EDX contains the remainder and numCount contains the divisor
; Returns:     EAX contains the rounded quotient
;------------------------------------------------------------------------------
RoundUp PROC
     push eax       ; push quotient onto stack
     mov  eax,edx   ; copy remainder to eax
     mov  ebx,10
     mul  ebx       ; multiply remainder by 10
     div  numCount  ; divide by numCount
     cmp  eax,5     ; test if quotient < 5
     jb   endRound  ; do not round up if < 5
     mov  eax,[esp] ; copy previous quotient to eax
     inc  eax       ; increment previous quotient on stack
     mov  [esp],eax ; update value on stack

endRound:
     pop  eax       ; put previous quotient back into eax
     ret
roundUp ENDP

END main