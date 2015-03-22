TITLE I/O Procedures   (Prog06A.asm)

; Author: David Rigert
; CS271 / Programming Assignment #6 Option A            3/5/2015
; Description: This program prompts the user for 10 valid 32-bit signed integers,
;              stores the values in an array, and then displays the integers,
;              their sums, and their average.
;              User input is read as a string and converted to numeric form.
;              The input is validated during the conversion process.
; Extra Credit: 1) Each line of user input is numbered and has a running subtotal
;               2) Handles signed integers

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
count_label    BYTE      "Entering value number ",0
subtotal_label BYTE      "Sum of entries so far: ",0
invalid_num    BYTE      "That number is larger than 32 bits or is not a valid integer.", 0
num_label      BYTE      "You entered the following numbers: ",0
delimiter      BYTE      ", ",0
sum_label      BYTE      "The sum is: ",0
avg_label      BYTE      "The average (rounded down) is: ",0
goodbye        BYTE      "Thank you for a fun quarter!",0
num_array      SDWORD    NUM_COUNT DUP(0)    ; stores the converted numbers
sum_val        SDWORD    0  ; sum of values in array

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
     push OFFSET invalid_num            ; invalid input message
     push OFFSET num_prompt             ; prompt for user input
     push OFFSET subtotal_label         ; label for subtotal
     push OFFSET count_label            ; label for value number
     push NUM_COUNT                     ; elements in array
     push OFFSET num_array              ; array to store values
     call GetVals
     
; Display the values to the terminal window
     displayString OFFSET num_label     ; display label for numbers entered
     call Crlf
     push OFFSET delimiter              ; delimiter between values
     push NUM_COUNT                     ; elements in array
     push OFFSET num_array              ; array to read values from
     call DisplayList
     call Crlf
     
; Display the sum to the terminal window
     displayString OFFSET sum_label     ; label for sum
     push OFFSET sum_val                ; stores the sum
     push NUM_COUNT                     ; elements in array
     push OFFSET num_array              ; array to get sum of
     call GetSum                        ; put sum into sum_val
     push sum_val                       ; sum value to display
     call WriteVal
     call Crlf

; Display the average to the terminal window
     displayString OFFSET avg_label     ; label for average
     push NUM_COUNT                     ; elements in array
     push OFFSET num_array              ; array to get average of
     call DisplayAverage
     call Crlf
     call Crlf

; Display goodbye message
     displayString OFFSET goodbye
     call Crlf

     exit ; exit to operating system
main ENDP

;----------------------------------------------------------------------
; Displays the average of the SDWORD values in an array.
DisplayAverage PROC USES eax ebx edx edi,
     pArray:PTR SDWORD,  ; Address of array to display
     count:DWORD         ; Number of elements in array
     LOCAL sum:SDWORD    ; Holds the sum
; Outputs: Displays the values of the array in the terminal window.
;----------------------------------------------------------------------
     lea  edi, sum       ; copy address of sum to EDI
     push edi            ; out parameter for sum
     push count          ; number of elements in array
     push pArray         ; address of array
     call GetSum         ; puts sum in 'sum' variable
     mov  eax, sum       ; copy sum to EAX
     mov  ebx, count     ; copy divisor to EBX
     cdq                 ; sign extend value into EDX
     idiv ebx            ; divide sum by count
     push eax            ; push quotient on stack
     call WriteVal       ; display the average
     
     ret
DisplayAverage ENDP

;----------------------------------------------------------------------
; Displays all of the SDWORD values in an array.
DisplayList PROC USES eax ecx esi,
     pArray:PTR SDWORD,  ; Address of array to display
     count:DWORD,        ; Number of elements in array
     pDelim:PTR BYTE     ; Delimiter between values
; Outputs: Displays the values of the array in the terminal window.
;----------------------------------------------------------------------
     mov  esi, pArray    ; copy address to ESI for reading
     mov  ecx, count     ; set loop counter
     
; Display first value outside of loop so that last value will not have delimiter
     lodsd               ; load value into EAX and increment ESI
     push eax
     call WriteVal       ; display value
     dec  ecx            ; subtract 1 from counter
startList:
     displayString pDelim ; display delimiter
     lodsd               ; load value into EAX and increment ESI
     push eax
     call WriteVal       ; display value
     loop startList
     ret
DisplayList ENDP

;----------------------------------------------------------------------
; Gets the sum of the SDWORD values in an array.
GetSum PROC USES eax ebx ecx esi edi,
     pArray:PTR SDWORD,  ; Address of array to sum values of
     count:DWORD,        ; Number of elements in array
     pResult:PTR SDWORD  ; Stores the sum
; Outputs: Displays the sum in the terminal window.
;----------------------------------------------------------------------
     mov  esi, pArray    ; copy address to ESI for reading
     mov  ebx, 0         ; initialize EBX as accumulator
     mov  ecx, count     ; set loop counter
     cld                 ; clear direction flag (forward direction)

; Calculate the sum of all values currently in the array
startLoop:
     lodsd               ; load value into EAX and increment ESI
     add  ebx, eax       ; add value to EBX
     loop startLoop      ; repeat for NUM_COUNT
     
; Copy sum to out parameter
     mov  edi, pResult
     mov  [edi], ebx
     ret
GetSum ENDP

;----------------------------------------------------------------------
; Prompts the user for 'count' 32-bit signed integer values and stores
; them in the array pointed to by pArray. Displays the number of values
; entered so far and a running subtotal.
GetVals PROC,
     pArray:PTR SDWORD,       ; Address of array to store values in
     count:DWORD,             ; Number of elements in array
     pCountLabel:PTR BYTE,    ; Label for values entered so far
     pSubtotalLabel:PTR BYTE, ; Subtotal label
     pNumPrompt:PTR BYTE,     ; Prompt for user input of value
     pInvalid:PTR BYTE        ; Invalid input message
     LOCAL subtotal:SDWORD    ; Holds subtotal of values entered so far
; Outputs: Fills the array with user input.
;----------------------------------------------------------------------
     mov  ebx, 1         ; keep track of number of values entered
     mov  edi, pArray    ; copy address of array to EDI for writing
     mov  ecx, count     ; set counter to number of values to enter
fetchNumbers:
; Display number of values entered so far
     displayString pCountLabel
     push ebx            ; value count
     call WriteVal       ; display value count
     call Crlf
; Get value from user
     push pInvalid       ; invalid input message
     push pNumPrompt     ; prompt text
     push edi            ; element to save value to
     call ReadVal        ; read value from user and store in element
     inc  ebx            ; increment counter
     add  edi, 4         ; increment EDI to next element
; Display subtotal
     displayString pSubtotalLabel
     lea  eax, subtotal  ; copy address to EAX
     push eax            ; out parameter for sum
     push count          ; elements in array
     push pArray         ; address of array
     call GetSum         ; get the running subtotal
     push subtotal
     call WriteVal       ; display the subtotal
     call Crlf
     call Crlf
     loop fetchNumbers   ; repeat 'count' times
     ret
GetVals ENDP

;----------------------------------------------------------------------
; Prompts the user for a 32-bit signed integer value and stores the
; value in the specified memory location. Displays an error and reprompts
; if the input is not a valid integer or is larger than 32 bits.
ReadVal PROC USES eax ebx ecx esi edi,
     pValue:PTR SDWORD,  ; Memory location to store the value
     pNumPrompt:PTR BYTE,; Prompt for user input of value
     pInvalid:PTR BYTE   ; Invalid input message
     LOCAL input[MAX_NUM_LEN+2]:BYTE ; Buffer for user input string
; Outputs: Stores the signed value in address of 'value'.
;----------------------------------------------------------------------
startReading:
     mov  edi, pValue    ; copy value address to EDI
     mov  SDWORD PTR [edi], 0 ; initialize value to 0
     lea  eax, input     ; copy address of local input variable to EAX
     getString pNumPrompt, eax, LENGTHOF input  ; stores string in input
     mov  esi, eax       ; copy address of first character to ESI
     mov  eax, 0         ; initialize EAX to 0
     cld                 ; clear direction flag (forward direction)
     lodsb               ; load first character to EAX
     cmp  al, 0          ; test for blank input
     je   isInvalid      ; blank input is invalid
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
isInvalid:
     displayString pInvalid
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
     value:SDWORD        ; Signed integer value to write
     LOCAL output[MAX_NUM_LEN+1]:BYTE, ; Holds string representation of value
           sign:BYTE     ; Holds the sign of the number
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