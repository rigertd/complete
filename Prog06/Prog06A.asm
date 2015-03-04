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
MAX_NUM_LEN = 12    ; maximum digits of a 32-bit signed integer + 1 for validation

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
invalid   BYTE      "That number is larger than 32 bits or is not a valid integer.", 0
num_label BYTE      "You entered the following numbers: ",0
sum_label BYTE      "The sum is: ",0
avg_label BYTE      "The rounded average is: ",0
goodbye   BYTE      "Thank you for a fun quarter!",0
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
     
     mov  eax, num_array
     call WriteInt
     call Crlf

     exit ; exit to operating system
main ENDP

;----------------------------------------------------------------------
; Prompts the user for a 32-bit signed integer value and stores the
; value in the specified memory location. Displays an error and reprompts
; if the input is not a valid integer or is larger than 32 bits.
readVal PROC USES eax ebx ecx esi edi,
     pPrompt:PTR BYTE,   ; Address of prompt text
     pValue:PTR SDWORD   ; Address of memory location to store the value
     LOCAL input[MAX_NUM_LEN+2]:BYTE ; Buffer for user input string
; Outputs: Stores the signed value in address of 'value'.
;----------------------------------------------------------------------
startReading:
     mov  edi, pValue    ; copy array address to EDI
     mov  SDWORD PTR [edi], 0 ; initialize element to 0
     lea  eax, input     ; copy address of local input variable to EAX
     getString pPrompt, eax, LENGTHOF input  ; stores string it in input
     mov  esi, eax       ; copy address of first character to ESI
     mov  eax, 0         ; initialize EAX to 0
     lodsb               ; load first character to EAX
     cmp  al, '-'        ; compare first character to hyphen (negative)
     je   isNegative     ; skip next code if negative
     mov  ecx, 1         ; if positive, store a positive multiplier in ECX
     dec  esi            ; and move pointer back to first chracter
     jmp  beginParse
isNegative:
     mov  ecx, -1        ; if negative, store a negative multiplier in ECX
     
beginParse:
     lodsb               ; load current character into EAX and increment ESI
     cmp  al, '0'        ; test if less than 0
     jb   endParse
     cmp  al, '9'        ; test if greater than 9
     ja   endParse
     
     ; Valid number if execution reaches here
     mov  ebx, [edi]     ; copy current value to ebx
     imul ebx, 10        ; multiply by 10 (this works because 10*0=0)
     jc   endParse       ; number was too big if carry flag is set
     mov  [edi], ebx     ; copy new value back to element
     sub  eax, '0'       ; subtract ASCII value of '0' to get numeric value
     add  [edi], eax     ; add the value to element
     jc   endParse       ; number was too big if carry flag is set
     jmp  beginParse     ; repeat parsing loop
     
endParse:
     dec  esi            ; back up ESI pointer to last character
     mov  al, [esi]      ; copy character to AL
     cmp  al, 0          ; verify that it was NULL
     je   doneReading    ; string successfully parsed if NULL
     
     ; Invalid user input if execution reaches here
     displayString OFFSET invalid
     call Crlf
     mov  SDWORD PTR [edi], 0 ; reset value to 0
     jmp  startReading   ; go back to start for new input
     
doneReading:
     imul ecx, [edi]     ; apply sign multiplier to final result
     mov  [edi], ecx     ; store final value in array element

     ret
readVal ENDP

writeVal PROC
     ret
writeVal ENDP

END main