TITLE I/O Procedures   (Prog06A.asm)

; Author: David Rigert
; CS271 / Programming Assignment #6 Option A            3/2/2015
; Description: This program prompts the user for 10 valid 32-bit signed integers,
;              stores the values in an array, and then displays the integers,
;              their sums, and their average.
;              User input is read as a string and converted to numeric form.
;              The input is validated during the conversion process.

INCLUDE Irvine32.inc

.const
NUM_COUNT = 10      ; number of integers to get from user
MAX_NUM_LEN = 11    ; maximum digits of a 32-bit signed integer

;----------------------------------------------------------------------
; Prompts the user for a string and stores the value in the memory
; location specified by 'buffer'.
; Inputs:  prompt   - Address of prompt string
;          buffer   - Address of array to store user input
;          len      - Size of user input array
; Outputs: Stores user input in address of 'buffer'.
;----------------------------------------------------------------------
getString MACRO prompt, buffer, len
     push edx                      ;; save register values
     push ecx
     push eax
     mov  edx, prompt
     call WriteString              ;; display prompt
     mov  edx, buffer              ;; address to store input
     mov  ecx, len-1               ;; subtract 1 for null terminator
     call ReadString               ;; store user input in buffer
     pop  eax                      ;; restore register values
     pop  ecx
     pop  edx
ENDM

;----------------------------------------------------------------------
; Displays the string stored at the specified memory location.
; Inputs:  text     - Address of string to display
;----------------------------------------------------------------------
displayString MACRO text
     push edx                      ;; save register values
     mov  edx, text
     call WriteString              ;; display text
     pop  edx                      ;; restore register values
ENDM

.data
intro     BYTE      "Manual Integer Parsing Procedures     by David Rigert",0
instruct1 BYTE      "Specify ",0
instruct2 BYTE      " positive or negative integers that fit in a 32-bit register.",10,13,
                    "This program will then print each number, the sum of the numbers,",10,13,
                    "and the average value.",0
num_prmpt BYTE      "Please enter a positive or negative integer: ", 0
invalid   BYTE      "That is not a valid integer. Try again: ", 0
num_label BYTE      "You entered the following numbers: ",0
sum_label BYTE      "The sum is: ",0
avg_label BYTE      "The rounded average is: ",0
goodbye   BYTE      "Thank you for a fun quarter!",0
;input     BYTE      MAX_NUM_LEN+2 DUP(?)     ; stores user input string
num_array SDWORD    NUM_COUNT DUP(?)         ; stores the converted numbers

.code
main PROC
; Display program title
     displayString OFFSET intro
     call Crlf
     call Crlf

; Display program instructions
     displayString OFFSET instruct1     ; display first part of instructions
     mov  eax, NUM_COUNT                ; display number of integers to get
     call WriteDec
     displayString OFFSET instruct2     ; display second part of instructions
     call Crlf
     call Crlf

; Read a value from the user
     push OFFSET num_array
     push OFFSET num_prmpt
     call readVal

     exit ; exit to operating system
main ENDP

;----------------------------------------------------------------------
; Prompts the user for a 32-bit signed integer value and stores the
; value in the specified memory location.
readVal PROC,
     pPrompt:PTR BYTE,   ; Address of prompt text
     pValue:PTR SDWORD   ; Address of memory location to store the value
     LOCAL input[MAX_NUM_LEN+2]:BYTE ; Buffer for user input string
; Outputs: Stores the signed value in address of 'value'.
;----------------------------------------------------------------------
     lea  eax, input     ; copy address of local input variable to EAX
     getString pPrompt, eax, LENGTHOF input

     mov  [pValue], 0
     mov  edx, eax
     call WriteString

     ret
readVal ENDP

writeVal PROC
     ret  12
writeVal ENDP

END main