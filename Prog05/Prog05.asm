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

     exit                 ; exit to operating system
main ENDP

;------------------------------------------------------------------------------
; Displays the program introduction message.
;------------------------------------------------------------------------------
displayIntro PROC USES eax edx
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
; Gets a number between MIN_REQUEST and MAX_REQUEST from the user
; and stores it in parameter.
; Receives:    Reference to DWORD to store user input (EBP + 8)
; Returns:     Parameter contains the validated user input
;------------------------------------------------------------------------------
getUserData PROC
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
; Makes sure the number in EAX is between MIN_REQUEST and MAX_REQUEST.
; Displays an error message and sets EAX to 0 if out of range.
; Receives:    EAX contains the number to be validated
; Returns:     EAX contains the validated number if valid or 0 if invalid
;------------------------------------------------------------------------------
validateNum PROC USES edx
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
; Fills an array containing the specified number of WORD elements with random
; numbers between LOW_RAND and HIGH_RAND.
; Receives:    (EBP+12) Requested number of random numbers
;              (EBP+8)  Reference to a WORD array 
; Returns:     Array is filled with random numbers between LOW_RAND and HIGH_RAND
;------------------------------------------------------------------------------
fillArray PROC
     push ebp            ; save original EBP on stack
     mov  ebp, esp       ; set up stack frame
     push eax            ; save old general-purpose register values
     push ecx
     push edi
     
     mov  ecx, [ebp+12]  ; set counter register to number of elements in array
     mov  edi, [ebp+8]   ; set EDI to address of first element in array
fillLoop:
     mov  eax, HIGH_RAND - LOW_RAND + 1
     call RandomRange               ; generate random number between 0 and EAX
     add  eax, LOW_RAND             ; add LOW_RAND to bring up to correct range
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
; Sorts a WORD array of the specified size into descending order.
; Uses the bubble sort algorithm.
; Receives:    (EBP+12) Reference to a WORD array 
;              (EBP+8)  Number of elements in the array
; Returns:     Array is filled with random numbers between LOW_RAND and HIGH_RAND
;------------------------------------------------------------------------------
sortList PROC
     push ebp            ; save old EBP value
     mov  ebp, esp       ; set up stack frame
     push eax            ; save general-purpose registers
     push ecx
     push edi
     
     mov  ecx, [ebp+8]   ; set counter register to number of elements
     mov  edi, [ebp+12]  ; set pointer to first element in EDI
     mov  edx, 0         ; set EDX to array offset of 0
     dec  ecx            ; decrement counter by 1 for swapping next element
sortLoop:
     mov  ax, [edi]      ; copy current element to AX
     push edi            ; save current element on stack
     add  edi, TYPE WORD ; increment by 1 element
     push edi            ; save next element on stack
     cmp  ax, [edi]      ; compare to next element
     ja   doSwap         ; if current > next, swap the values
     
     ; if execution reaches here, the values do not need to be swapped
     pop  eax            ; discard parameters
     pop  eax
     jmp  endSwap        ; jump past swap code
     
doSwap:
     ; parameters are already set
     call swap           ; swap the values
endSwap:
     loop sortLoop       ; loop through every pair in array
     
     pop  edi            ; restore general-purpose registers
     pop  edx
     pop  ecx
     pop  eax
     pop  ebp            ; restore old EBP
     
     ret 8
sortList ENDP

;------------------------------------------------------------------------------
; Swaps the values in the two specified WORD memory addresses.
; Receives:    (EBP+12) Reference to first WORD value 
;              (EBP+8)  Reference to second WORD value
; Returns:     The values have been swapped
;------------------------------------------------------------------------------
swap PROC
     push ebp            ; save old EBP value
     mov  ebp, esp       ; set up stack frame
     push eax            ; save general-purpose registers
     push esi
     push edi
     
     mov  esi, [ebp+12]  ; copy address of first value to ESI
     mov  edi, [ebp+8]   ; copy address of second value to EDI
     mov  ax, [esi]      ; copy first value to AX
     xchg ax, [edi]      ; swap with second value
     mov  [esi], ax      ; copy second value over first
     
     pop  edi            ; restore general-purpose registers
     pop  esi
     pop  eax
     pop  ebp            ; restore old EBP
     ret 8               ; return and clear params from stack
swap ENDP


; ;------------------------------------------------------------------------------
; ; Displays the number of primes contained in EAX in a formatted list of
; ; NUMS_PER_ROW in each row, with a "any key to continue" message every
; ; LINES_PER_PAGE rows.
; ; Receives:    EAX contains the number of primes to show
; ;------------------------------------------------------------------------------
; showPrimes PROC USES ebx ecx edx
     ; push eax            ; save number entered by user on stack
     ; mov  ecx, eax       ; set counter to number entered by user
     ; mov  eax, 1         ; set eax to 1
; displayPrime:
     ; call getNextPrime   ; place next prime number in eax
     ; call WriteDec       ; display prime in eax to terminal window
     ; call printWhiteSpace; display tab, line feed, or Press any key message
     ; loop displayPrime   ; loop for number of times entered by user
     ; call Crlf
     ; call Crlf
     ; pop  eax            ; restore user input
     ; ret
; showPrimes ENDP

; ;------------------------------------------------------------------------------
; ; Prints a tab character if the current prime count is not a multiple of 10,
; ; or a line feed if it is a multiple of 10. Also prints a "any key to continue"
; ; message after displaying LINES_PER_PAGE rows.
; ; Receives:    primeCount contains the number of primes printed so far
; ;------------------------------------------------------------------------------
; printWhiteSpace PROC USES eax ebx edx
     ; mov  eax,primeCount ; copy number displayed so far to eax
     ; cdq                 ; zero out edx
     ; mov  ebx, NUMS_PER_ROW
     ; div  ebx            ; divide by number of primes to display per row
     ; cmp  edx, 0         ; test if multiple of 10 by remainder of 0
     ; jne  not10th        ; jump if not a multiple of 10
     ; call Crlf           ; print line feed if multiple of 10
     ; jmp  anyKeyTest     ; test if Press any key message is required

; not10th:     
     ; call printTab       ; display a tab character
     ; jmp  endWhiteSpace  ; done with whitespace

; anyKeyTest:    ; now test if multiple of NUMS_PER_ROW * LINES_PER_PAGE
     ; mov  eax,primeCount ; copy number displayed so far to eax
     ; cdq                 ; zero out edx
     ; mov  ebx, NUMS_PER_ROW * LINES_PER_PAGE
     ; div  ebx            ; divide by NUMS_PER_ROW * LINES_PER_PAGE
     ; cmp  edx, 0         ; test if multiple by remainder
     ; jne  endWhiteSpace  ; jump to end if not a multiple
     ; mov  edx, OFFSET continue
     ; call WriteString    ; display Press any key message
     ; call ReadChar       ; block until any key is pressed
     ; call Crlf           ; display another line feed
     
; endWhiteSpace:
     ; ret
; printWhiteSpace ENDP

; ;------------------------------------------------------------------------------
; ; Gets the next prime number after the value in EAX.
; ; Receives:    EAX contains the current prime number
; ; Returns:     EAX contains the next prime number
; ;------------------------------------------------------------------------------
; getNextPrime PROC
; tryNext:
     ; inc  eax
     ; call isPrime
     ; cmp  ebx, 0         ; test if number is prime
     ; je   tryNext        ; try next number if ebx=0 (isPrime is false)
     ; ret                 ; number in eax is prime
; getNextPrime ENDP

; ;------------------------------------------------------------------------------
; ; Tests whether the number in EAX is prime by dividing it by every number in primeDivisors.
; ; If it is, stores it in the primeDivisors array and increments primeCount.
; ; Receives:    EAX contains the number to test
; ; Returns:     EBX is set to 1 if the number is prime, or 0 if it is not.
; ;------------------------------------------------------------------------------
; isPrime PROC USES ecx edx esi edi
     ; push eax            ; save number being tested on stack
     ; cmp  eax, 1         ; test if number <= 1
     ; jle  notPrime       ; return not prime if <= 1
     ; cmp  eax, 2         ; test if number is 2
     ; je   prime          ; number is prime if 2
     ; mov  edi,primeCount ; otherwise try to divide by numbers in primeDivisors
     
; tryDivide:
     ; mov  eax, [esp]     ; put original number back into eax
     ; cdq                 ; zero out edx
     ; dec  edi            ; decrement array index
     ; mov  ebx, primeDivisors[edi*4]     ; copy prime divisor to ebx
     ; div  ebx            ; divide number by prime divisor
     ; cmp  edx, 0         ; test if remainder is 0
     ; je   notPrime       ; return not prime if remainder is 0
     ; cmp  edi, 0         ; test if first element of array was reached
     ; jg   tryDivide      ; repeat divide loop with new counter value
     
     ; ; number is prime if execution reaches here
     ; mov  ebx, 1         ; set ebx to 1 (true) if prime
     ; jmp  prime

; notPrime:
     ; mov  ebx, 0         ; set ebx to 0 (false) if not prime
     ; jmp  endPrime       ; jump to end of procedure
     
; prime:
     ; mov  esi,primeCount ; index to store result at in primeDivisors array
     ; mov  eax, [esp]     ; copy original number back into eax
     ; mov  primeDivisors[esi*4], eax     ; save prime to primeDivisors array
     ; inc  primeCount     ; increment the prime counter

; endPrime:
     ; pop  eax; restore number
     ; ret
; isPrime ENDP

; ;------------------------------------------------------------------------------
; ; Prints a tab character to the terminal window.
; ;------------------------------------------------------------------------------
; printTab PROC USES eax
     ; mov  al, TAB
     ; call WriteChar      ; display tab character
     ; ret
; printTab ENDP

; ;------------------------------------------------------------------------------
; ; Prints the goodbye message to the terminal window.
; ;------------------------------------------------------------------------------
; farewell PROC USES edx
     ; mov  edx, OFFSET goodbye
     ; call WriteString
     ; call Crlf
     ; ret
; farewell ENDP

END main