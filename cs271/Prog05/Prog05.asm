TITLE Programming Assignment 5 (prog05.asm)

; Author: David Rigert
; CS271 / #05                 Date: 2/17/2015
; Description: This program prompts the user for a number (10 to 200) of random
;              integers to generate, generates numbers between 100 and 999, and
;              stores them in consecutive elements on an array.
;              The program then displays the unsorted numbers, 10 per line,
;              sorts the list in descending order, calculates and displays the
;              median rounded to the nearest integer, and then displays the
;              sorted list, 10 per line.

INCLUDE Irvine32.inc

.const
MIN_REQUEST = 10         ; minimum number of numbers to generate
MAX_REQUEST = 200        ; maximum number of numbers to generate
LOW_RAND = 100           ; lowest random number to generate
HIGH_RAND = 999          ; highest random number to generate
TAB = 9                  ; ASCII value of the tab character
NUMS_PER_ROW = 10        ; number of numbers to display per row

.data
intro1    BYTE      "Random Number Generator and Sorter     by David Rigert",13,10,13,10
          BYTE      "This program generates random numbers between ", 0
intro2    BYTE      " and ",0
intro3    BYTE      ".",13,10,"It then displays the original list, sorts it, "
          BYTE      "calculates and ",13,10,"displays the median value, "
          BYTE      "and finally displays the values sorted in descending order.",0
          BYTE      "This program can display up to ",0
prompt1   BYTE      "Number of random numbers to generate [",0
prompt2   BYTE      " .. ",0
prompt3   BYTE      "]: ",0
inputErr  BYTE      "Invalid input. Try again.",0
unsorted  BYTE      "Unsorted List",0
sorted    BYTE      "Sorted List",0
median    BYTE      "Median Value: ",0

request   SDWORD    ?    ; number of random numbers to generate
numArray  WORD      MAX_REQUEST DUP(0)  ; holds the random numbers
arrCount  DWORD     0    ; number of elements in array


.code
main PROC
     call Randomize       ; seed random number generator only once
     
     call displayIntro    ; display the program introduction
     
     push OFFSET request  ; pointer to variable that will hold user input
     call getUserData     ; get number from user and store in request argument
     
     push request         ; requested number of rands
     push OFFSET numArray ; pointer to first element in array
     call fillArray       ; fill array with random numbers
     
     push OFFSET numArray ; pointer to first element in array
     push request         ; number of elements in array
     push OFFSET unsorted ; unsorted list title
     call displayList     ; print array to terminal window
     
     push OFFSET numArray ; pointer to first element in array
     push request         ; number of elements in array
     call sortList        ; sort into ascending order using bubble sort
     
     push OFFSET numArray ; pointer to first element in array
     push request         ; number of elements in array
     push OFFSET median   ; text to display before median
     call displayMedian   ; print median value to terminal window
     
     push OFFSET numArray ; pointer to first element in array
     push request         ; number of elements in array
     push OFFSET sorted   ; sorted list title
     call displayList     ; print array to terminal window
     
     exit                 ; exit to operating system
main ENDP

;------------------------------------------------------------------------------
displayIntro PROC USES eax edx
; Displays the program introduction message.
;------------------------------------------------------------------------------
     mov  edx, OFFSET intro1
     call WriteString    ; display part 1 of intro
     mov  eax, LOW_RAND
     call WriteDec       ; display lowest possible random number
     mov  edx, OFFSET intro2
     call WriteString    ; display middle of intro
     mov  eax, HIGH_RAND
     call WriteDec       ; display highest possible random number
     mov  edx, OFFSET intro3
     call WriteString    ; display remainder of intro
     call Crlf
     call Crlf
     ret
displayIntro ENDP

;------------------------------------------------------------------------------
getUserData PROC
; Gets a number between MIN_REQUEST and MAX_REQUEST from the user
; and stores it in parameter.
; Receives:    Reference to DWORD to store user input (EBP + 8)
; Returns:     Parameter contains the validated user input
;------------------------------------------------------------------------------
     push ebp                 ; save original EBP on stack
     mov  ebp, esp            ; Set up stack frame
     push eax                 ; save current register values on stack
     push edx

getInput:
     mov  edx, OFFSET prompt1
     call WriteString         ; display input prompt
     mov  eax, MIN_REQUEST
     call WriteDec            ; min input value
     mov  edx, OFFSET prompt2
     call WriteString         ; middle of input prompt
     mov  eax, MAX_REQUEST
     call WriteDec            ; max input value
     mov  edx, OFFSET prompt3
     call WriteString         ; rest of input prompt
     call ReadInt             ; get numeric input
     call validateNum         ; verify validity of input
     cmp  eax, 0              ; test if input was valid
     je   getInput            ; go back to beginning if invalid
     
     ; we have valid input when execution reaches here
     mov  edx, [ebp+8]        ; copy pointer to edx
     mov  [edx], eax          ; copy valid input to memory address
     
     call Crlf                ; display line feed
     pop  edx                 ; restore registers
     pop  eax
     pop  ebp
     ret  4                   ; take parameter off of stack
getUserData ENDP

;------------------------------------------------------------------------------
validateNum PROC USES edx
; Makes sure the number in EAX is between MIN_REQUEST and MAX_REQUEST.
; Displays an error message and sets EAX to 0 if out of range.
; Receives:    EAX contains the number to be validated
; Returns:     EAX contains the validated number if valid or 0 if invalid
;------------------------------------------------------------------------------
     cmp  eax, MIN_REQUEST    ; compare against min valid input
     jl   invalidInput        ; jump if eax < MIN_REQUEST
     cmp  eax, MAX_REQUEST    ; compare against max valid input
     jg   invalidInput        ; jump if eax > MAX_REQUEST
     jmp  validInput          ; input was valid if execution reaches here

invalidInput:                      ; invalid input
     mov  eax, 0                   ; set EAX to 0
     mov  edx, OFFSET inputErr
     call WriteString              ; display error message
     call Crlf
     
validInput:    ; execution jumps to here if input was valid
     ret
validateNum ENDP

;------------------------------------------------------------------------------
fillArray PROC
; Fills an array containing the specified number of WORD elements with random
; numbers between LOW_RAND and HIGH_RAND.
; Receives:    (EBP+12) Requested number of random numbers
;              (EBP+8)  Reference to a WORD array 
; Returns:     Array is filled with random numbers between LOW_RAND and HIGH_RAND
;------------------------------------------------------------------------------
     push ebp            ; save original EBP on stack
     mov  ebp, esp       ; set up stack frame
     push eax            ; save old general-purpose register values
     push ecx
     push edi
     
     mov  ecx, [ebp+12]  ; set counter register to number of elements in array
     mov  edi, [ebp+8]   ; set EDI to address of first element in array
fillLoop:
     mov  eax, HIGH_RAND - LOW_RAND + 1
     call RandomRange    ; generate random number between 0 and EAX
     add  eax, LOW_RAND  ; add LOW_RAND to bring up to correct range
     mov  [edi], ax      ; copy random number to current element
     add  edi, TYPE WORD ; advance EDI to next element
     loop fillLoop       ; repeat for ECX iterations
     
     pop  edi            ; restore old general-purpose register values
     pop  ecx
     pop  eax
     pop  ebp            ; restore old EBP value
     ret 8               ; return and move ESP past 2 parameters
fillArray ENDP

;------------------------------------------------------------------------------
sortList PROC
; Sorts a WORD array of the specified size into descending order.
; Uses the bubble sort algorithm.
; Receives:    (EBP+12) Reference to a WORD array 
;              (EBP+8)  Number of elements in the array
; Returns:     Array is filled with random numbers between LOW_RAND and HIGH_RAND
;------------------------------------------------------------------------------
     push ebp            ; save old EBP value
     mov  ebp, esp       ; set up stack frame
     push eax            ; save general-purpose registers
     push ebx
     push ecx
     push edi
     
     mov  ecx, [ebp+8]   ; set counter register to number of elements
     mov  edi, [ebp+12]  ; set EDI to pointer to first element in array
     dec  ecx            ; decrement counter by 1 to avoid last element
outerLoop:
     push ecx            ; save current counter value
     push edi            ; save current pointer value
     mov  ebx, 0         ; EBX is flag for whether any elements were swapped
     
innerLoop:
     mov  eax, 0              ; zero out EAX
     mov  ax, [edi]           ; copy current element to AX
     cmp  ax, [edi+TYPE WORD] ; compare to next element
     jb   doSwap              ; if current < next, swap the values
     
     ; if execution reaches here, the values do not need to be swapped
     jmp  endSwap        ; jump past swap code
     
doSwap:
     push edi            ; pointer to first value to swap
     add  edi, TYPE WORD ; increment to next value
     push edi            ; pointer to second value to swap
     sub  edi, TYPE WORD ; decrement back to previous value
     call swap           ; swap the values
     mov  ebx, 1         ; set swapped flag to true
endSwap:
     add  edi, TYPE WORD ; increment pointer to next value
     loop innerLoop      ; loop through every pair in array
     
     pop  edi            ; restore starting pointer value
     pop  ecx            ; restore starting counter value
     cmp  ebx, 0         ; check swapped flag
     jz   doneSwap       ; done with sorting if false (0)
     loop outerLoop      ; otherwise run through again
     
     
doneSwap:
     pop  edi            ; restore general-purpose registers
     pop  ecx
     pop  ebx
     pop  eax
     pop  ebp            ; restore old EBP
     
     ret 8               ; return to caller and clear params from stack
sortList ENDP

;------------------------------------------------------------------------------
swap PROC
; Swaps the values in the two specified WORD memory addresses.
; Receives:    (EBP+12) Reference to first WORD value 
;              (EBP+8)  Reference to second WORD value
; Returns:     The values have been swapped
;------------------------------------------------------------------------------
     push ebp            ; save old EBP value
     mov  ebp, esp       ; set up stack frame
     push esi            ; save general-purpose registers
     push edi
     push eax
     
     mov  esi, [ebp+12]  ; copy address of first value to ESI
     mov  edi, [ebp+8]   ; copy address of second value to EDI
     mov  ax, [esi]      ; copy first value to AX
     xchg ax, [edi]      ; swap first value with second
     mov  [esi], ax      ; put second value back in location of first
     
     pop  eax            ; restore general-purpose registers
     pop  edi
     pop  esi
     pop  ebp            ; restore old EBP
     ret 8               ; return to caller and clear params from stack
swap ENDP

;------------------------------------------------------------------------------
displayList PROC
; Displays all values in a WORD array with the specified number of elements.
; Receives:    (EBP+44) Reference to a WORD array
;              (EBP+40) Number of elements in the array
;              (EBP+36) Reference to title of array
;------------------------------------------------------------------------------
     pushad              ; save general-purpose registers
     mov  ebp, esp       ; set up stack frame
     
     ; display title of array
     mov  edx, [ebp+36]  ; copy offset to first char of title
     call WriteString    ; print to terminal window
     call Crlf
     call Crlf
     
     ; calculate number of rows
     mov  eax, [ebp+40]  ; number of elements
     call calcNumRows    ; put number of rows into EAX and 
                         ; number of columns with extra value in EDX
     mov  ecx, eax       ; copy number of rows to counter register
     
     ; display values in array
     mov  ebx, 0         ; keep track of current row in EBX
     
startTable:
     mov  esi, [ebp+44]  ; set ESI to beginning of array
     shl  ebx, 1         ; shift EBX right for WORD multiplier
     add  esi, ebx       ; offset index by row number
     shr  ebx, 1         ; undo multiplier
     push ecx            ; save outer loop counter
     mov  ecx, edx       ; copy number of columns with extra row to counter
; This section handles columns that have one more row than the rest
extraRowColumns:
     call printElement   ; print current element pointed to by ESI
     call printTab
     shl  eax, 1         ; WORD multiplier
     add  esi, eax       ; increase index by number of rows
     shr  eax, 1         ; undo multiplier
     loop extraRowColumns
; This section checks if the current row is the last row
     inc  ebx            ; increase row counter by 1 so it is 1-based
     cmp  ebx, eax       ; compare current row to total rows
     je   endRow         ; end of row if last row
     dec  ebx            ; restore row counter to 0 - based
; This section handles columns that do not have an extra row
     mov  ecx, NUMS_PER_ROW ; copy number of columns to counter
     sub  ecx, edx       ; subtract columns with extra row
     call printTab       ; print tab character left off at end of previous loop
     dec  eax            ; subtract 1 from number of rows
standardRowColumns:
     call printElement   ; print current element pointed to by ESI
     call printTab
     shl  eax, 1         ; WORD multiplier
     add  esi, eax       ; increase index by number of rows - 1
     shr  eax, 1         ; undo multiplier
     loop standardRowColumns
     inc  eax            ; set number of rows back to original value
endRow:
     pop  ecx            ; restore outer loop counter
     inc  ebx            ; increment row counter
     call Crlf           ; print line feed
     loop startTable     ; go to next row
     
     call Crlf           ; one more line feed after list
     popad               ; restore general-purpose registers
     ret 12              ; remove 3 DWORD parameters from stack
displayList ENDP

;------------------------------------------------------------------------------
printTab PROC USES eax
; Prints a tab character to the terminal window unless ECX is 1.
;------------------------------------------------------------------------------
     cmp  ecx, 1         ; if ECX == 1
     je   noTab
     mov  ax, TAB        ; TAB char
     call WriteChar      ; print the TAB character
noTab:
     ret
printTab ENDP

;------------------------------------------------------------------------------
printElement PROC USES eax
; Prints the WORD value pointed to by ESI
; Receives:    (ESI) Pointer to WORD value in an array
;------------------------------------------------------------------------------
     mov  eax, 0         ; zero out EAX
     mov  ax, [esi]      ; copy value to AX
     call WriteDec       ; print value to terminal window

     ret
printElement ENDP

;------------------------------------------------------------------------------
calcNumRows PROC USES ebx
; Calculates the number of rows required to display an array with NUMS_PER_ROW
; elements per row.
; Receives:    (EAX) Number of elements in the array
; Returns:     (EAX) Number of rows required
;              (EDX) Number of columns with extra row
;------------------------------------------------------------------------------
     cdq                 ; zero out EDX
     mov  ebx, NUMS_PER_ROW   ; divisor
     div  ebx            ; divide number of elements by number shown per row
     cmp  edx, 0         ; check for remainder
     je   noRemainder    ; no partial row
     inc  eax            ; add 1 to row count for partial row
     jmp  endCalc
noRemainder:
     mov  edx, NUMS_PER_ROW   ; set EDX to number of columns if 0
endCalc:
     ret
calcNumRows ENDP

;------------------------------------------------------------------------------
displayMedian PROC
; Displays the median value of a sorted array. 
; Displays the average of the two middle values if even number of elements.
; Receives:    (EBP+44) Reference to a WORD array
;              (EBP+40) Number of elements in the array
;              (EBP+36) Reference to median label
;------------------------------------------------------------------------------
     pushad              ; all general-purpose registers (32 bytes)
     mov  ebp, esp       ; set up stack frame
     
     mov  esi, [ebp+44]  ; copy pointer to first element in array
     mov  eax, [ebp+40]  ; copy size of array
     cdq                 ; zero out EDX
     mov  ebx, 2         ; copy divisor to EBX
     div  ebx            ; divide EDX:EAX by 2
     mov  ecx, 0         ; zero out ECX
     mov  cx, [esi+eax*TYPE WORD] ; copy middle element to CX
     cmp  edx, 0         ; check for remainder
     je   avgMedian      ; find average of two middle values if even
     mov  eax, ecx       ; otherwise, copy median to EAX for display
     jmp  showResult     ; and show the result
avgMedian:
     dec  eax            ; decrement EAX to previous element
     add  cx, [esi+eax*TYPE WORD] ; add previous element to current
     mov  eax, ecx       ; copy sum of middle 2 elements to EAX
     cdq                 ; zero out EDX
     div  ebx            ; divide sum by 2 to get average
     cmp  edx,0          ; check for remainder
     je   showResult     ; show the result if no remainder
     inc  eax            ; otherwise round up the quotient
     
showResult:
     mov  edx, [ebp+36]  ; copy pointer to label to EDX
     call WriteString    ; display the label
     call WriteDec       ; display the value in EAX
     call Crlf           ; display 2 line feeds
     call Crlf

     popad               ; restore all general-purpose registers
     ret  12             ; remove 3 DWORD parameters from stack
displayMedian ENDP

END main