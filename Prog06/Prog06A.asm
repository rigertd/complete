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
     displayString prompt          ;; display prompt
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
intro          BYTE      "Manual Integer Parsing Procedures     by David Rigert",0
instruct1      BYTE      "Specify ",0
instruct2      BYTE      " positive or negative integers that fit in a 32-bit register.",10,13,
                    "This program will then print each number, the sum of the numbers,",10,13,
                    "and the average value.",0
num_prompt     BYTE      "Please enter a signed 32-bit integer: ",0
count_prompt   BYTE      "Entering value number ",0
subtotal_label BYTE      "Sum of entries so far: ",0
invalid_num    BYTE      "That number is larger than 32 bits or is not a valid integer.", 0
num_label      BYTE      "You entered the following numbers: ",0
sum_label      BYTE      "The sum is: ",0
avg_label      BYTE      "The average (rounded down) is: ",0
goodbye        BYTE      "Thank you for a fun quarter!",0
num_array      SDWORD    NUM_COUNT DUP(0)    ; stores the converted numbers

.code
main PROC
; Display program title
     displayString OFFSET intro
     call Crlf
     call Crlf

; Display program instructions
     displayString OFFSET instruct1     ; display first part of instructions
     push NUM_COUNT                     ; display number of integers to get
     call WriteVal
     displayString OFFSET instruct2     ; display second part of instructions
     call Crlf
     call Crlf

; Read a value from the user
     push OFFSET num_array
     call GetVals
     
; Display the values to the terminal window

; Display the sum to the terminal window
     displayString OFFSET sum_label
     push OFFSET num_array
     call DisplaySum     ; display the sum
     call Crlf
     

; Display the average to the terminal window

     exit ; exit to operating system
main ENDP

;----------------------------------------------------------------------
; Prompts the user for NUM_COUNT 32-bit signed integer values and stores
; them in the array pointed to by pArray. Displays the number of values
; entered so far and a running subtotal.
GetVals PROC,
     pArray:PTR SDWORD   ; address of array to store values in
; Outputs: Fills the array with user input.
;----------------------------------------------------------------------
     mov  ebx, 1         ; keep track of number of values entered
     mov  edi, pArray    ; copy address of array to EDI for writing
     mov  ecx, NUM_COUNT ; set counter to number of values to enter
fetchNumbers:
; Display number of values entered so far
     displayString OFFSET count_prompt
     push ebx            ; count
     call WriteVal       ; display count
     call Crlf
; Get value from user
     push edi            ; element to save value to
     call ReadVal        ; read value from user and store in element
     inc  ebx            ; increment counter
     add  edi, 4         ; increment EDI to next element
; Display subtotal
     displayString OFFSET subtotal_label
     push pArray
     call DisplaySum     ; display the running subtotal
     call Crlf
     call Crlf
     loop fetchNumbers   ; repeat for NUM_COUNT times
     ret
GetVals ENDP

;----------------------------------------------------------------------
; Displays the sum of the SDWORD values in an array with NUM_COUNT elements.
DisplaySum PROC USES eax ebx ecx esi edi,
     pArray:PTR SDWORD   ; address of array to sum values of
; Outputs: Displays the sum in the terminal window.
;----------------------------------------------------------------------
     mov  esi, pArray    ; copy address to ESI for reading
     mov  ebx, 0         ; initialize EBX as accumulator
     mov  ecx, NUM_COUNT ; set loop counter
     cld                 ; clear direction flag (forward direction)

; Calculate the sum of all values currently in the array
startLoop:
     lodsd               ; load value into EAX and increment ESI
     add  ebx, eax       ; add value to EBX
     loop startLoop      ; repeat for NUM_COUNT
     
; Display the sum in the terminal window
     push ebx            ; pass sum as parameter to WriteVal
     call WriteVal       ; display sum to terminal window
     ret
DisplaySum ENDP

; DisplayList PROC,
     ; pArray:PTR SDWORD   ; address of array to display
     
     
     ; ret
; DisplayList ENDP

;----------------------------------------------------------------------
; Prompts the user for a 32-bit signed integer value and stores the
; value in the specified memory location. Displays an error and reprompts
; if the input is not a valid integer or is larger than 32 bits.
ReadVal PROC USES eax ebx ecx esi edi,
     pValue:PTR SDWORD   ; Address of memory location to store the value
     LOCAL input[MAX_NUM_LEN+2]:BYTE ; Buffer for user input string
; Outputs: Stores the signed value in address of 'value'.
;----------------------------------------------------------------------
startReading:
     mov  edi, pValue    ; copy value address to EDI
     mov  SDWORD PTR [edi], 0 ; initialize value to 0
     lea  eax, input     ; copy address of local input variable to EAX
     getString OFFSET num_prompt, eax, LENGTHOF input  ; stores string in input
     mov  esi, eax       ; copy address of first character to ESI
     mov  eax, 0         ; initialize EAX to 0
     cld                 ; clear direction flag (forward direction)
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
     mov  [edi], ebx     ; copy new value back to value
     sub  eax, '0'       ; subtract ASCII value of '0' to get numeric value
     add  [edi], eax     ; add the digit to the value
     jc   endParse       ; number was too big if carry flag is set
     jmp  beginParse     ; repeat parsing loop
     
endParse:
     dec  esi            ; back up ESI pointer to last character
     mov  al, [esi]      ; copy character to AL
     cmp  al, 0          ; verify that it was NULL
     je   doneReading    ; string successfully parsed if NULL
     
; Invalid user input if execution reaches here
     displayString OFFSET invalid_num
     call Crlf
     mov  SDWORD PTR [edi], 0 ; reset value to 0
     jmp  startReading   ; go back to start for new input
     
doneReading:
     imul ecx, [edi]     ; apply sign multiplier to final result
     mov  [edi], ecx     ; store final value at the memory address
     ret
ReadVal ENDP

;----------------------------------------------------------------------
; Converts the signed integer value at the specified memory address
; to an ASCII string and displays it to the terminal window.
WriteVal PROC USES eax ebx ecx edx edi,
     value:SDWORD        ; signed integer value to write
     LOCAL output[MAX_NUM_LEN+1]:BYTE, ; holds string representation of value
           sign:BYTE     ; holds the sign of the number
; Outputs: Prints string representation to terminal window
;----------------------------------------------------------------------
     lea  edi, output[MAX_NUM_LEN] ; copy address of end of string to EDI
     std                 ; set direction flag (reverse direction)
     mov  eax, 0         ; NULL terminator for end of string
     stosb               ; add NULL to output string
     mov  eax, value     ; copy value to EAX
     mov  ebx, 10        ; put divisor in EBX
     cmp  eax, 0         ; test if value is negative
     jl   negative
     mov  sign, '+'      ; value is positive
     jmp  conversion
negative:
     mov  sign, '-'      ; value is negative
     neg  eax            ; convert to positive for conversion process
     
conversion:
     cdq                 ; extend sign of value in EAX
     idiv ebx            ; divide value by 10
     mov  ecx, eax       ; save quotient in ECX
     mov  eax, edx       ; store remainder in EAX
     add  eax, '0'       ; add ASCII value of '0' to convert to char
     stosb               ; save char of remainder to output string
     mov  eax, ecx       ; put quotient back in EAX
     cmp  eax, 0         ; if quotient == 0, done with conversion
     jne  conversion
     
; Done with conversion when execution reaches here
     cmp  sign, '-'      ; test if negative
     jne  positive       ; do not add sign char if positive
     mov  al, sign       ; copy negative sign to AL
     mov  BYTE PTR [edi], al ; append sign char to front of string
     jmp  showString 
positive:
     inc  edi            ; move to next character in string if no sign
showString:
     displayString edi   ; print output string, starting from sign
     ret
WriteVal ENDP

END main