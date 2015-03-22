TITLE Programming Assignment 4 (prog04.asm)

; Author: David Rigert
; CS271 / #04                 Date: 2/11/2015
; Description: This program prompts the user for the number of primes to display
;              and displays that number of primes with 10 primes per line.

INCLUDE Irvine32.inc

.const
MIN_PRIME_COUNT = 1      ; minimum number of primes to display
MAX_PRIME_COUNT = 9500   ; maximum number of primes to display
TAB = 9                  ; ASCII value of the tab character
NUMS_PER_ROW = 10        ; number of numbers to display per row
LINES_PER_PAGE = 22      ; lines of primes to display per page

.data
intro1    BYTE      "Prime Number Calculator     by David Rigert",13,10,13,10
          BYTE      "Enter the number of prime numbers you want to display.",13,10
          BYTE      "This program can display up to ",0
intro2    BYTE      " prime numbers.",0
prompt1   BYTE      "Number of primes [",0
prompt2   BYTE      " .. ",0
prompt3   BYTE      "]: ",0
inputErr  BYTE      "Invalid input. Try again.",0
continue  BYTE      "Press any key to continue...",0
goodbye   BYTE      "Thank you for playing. Goodbye.",0

primeCount     SDWORD    0    ; number of primes displayed so far
primeDivisors  DWORD     MAX_PRIME_COUNT DUP(0)


.code
main PROC
     call displayIntro   ; display the program introduction
     call getUserData    ; get validated number from the user and store in eax
     call showPrimes     ; calculate and display primes based on user input in eax
     call farewell       ; display goodbye message

     exit                ; exit to operating system
main ENDP

;------------------------------------------------------------------------------
; Displays the program introduction message.
;------------------------------------------------------------------------------
displayIntro PROC USES eax edx
     mov  edx, OFFSET intro1
     call WriteString
     mov  eax, MAX_PRIME_COUNT
     call WriteDec
     mov  edx, OFFSET intro2
     call WriteString
     call Crlf
     call Crlf
     ret
displayIntro ENDP

;------------------------------------------------------------------------------
; Gets a number between MIN_PRIME_COUNT and MAX_PRIME_COUNT from the user
; and stores it in EAX.
; Returns:     EAX contains the user input if valid or 0 if invalid
;------------------------------------------------------------------------------
getUserData PROC USES edx
getInput:
     mov  edx, OFFSET prompt1      ; display input prompt
     call WriteString
     mov  eax, MIN_PRIME_COUNT     ; min input value
     call WriteDec
     mov  edx, OFFSET prompt2
     call WriteString
     mov  eax, MAX_PRIME_COUNT     ; max input value
     call WriteDec
     mov  edx, OFFSET prompt3
     call WriteString
     call ReadInt                  ; get numeric input
     call validateNum              ; verify validity of input
     cmp  eax, 0                   ; test if input was valid
     je   getInput                 ; go back to beginning if invalid
     call Crlf
     ret
getUserData ENDP

;------------------------------------------------------------------------------
; Makes sure the number in EAX is between MIN_PRIME_COUNT and MAX_PRIME_COUNT.
; Displays an error message and sets EAX to 0 if out of range.
; Receives:    EAX contains the number to be validated
; Returns:     EAX contains the validated number if valid or 0 if invalid
;------------------------------------------------------------------------------
validateNum PROC USES edx
     cmp  eax, MIN_PRIME_COUNT     ; compare against min valid input
     jl   invalidInput             ; jump if eax < MIN_PRIME_COUNT
     cmp  eax, MAX_PRIME_COUNT     ; compare against max valid input
     jg   invalidInput             ; jump if eax > MAX_PRIME_COUNT
     jmp  validInput               ; input was valid if execution reaches here

invalidInput:                      ; invalid input
     mov  eax, 0                   ; set EAX to 0
     mov  edx, OFFSET inputErr
     call WriteString              ; display error message
     call Crlf
     
validInput:    ; execution jumps to here if input was valid
     ret
validateNum ENDP

;------------------------------------------------------------------------------
; Displays the number of primes contained in EAX in a formatted list of
; NUMS_PER_ROW in each row, with a "any key to continue" message every
; LINES_PER_PAGE rows.
; Receives:    EAX contains the number of primes to show
;------------------------------------------------------------------------------
showPrimes PROC USES ebx ecx edx
     push eax            ; save number entered by user on stack
     mov  ecx, eax       ; set counter to number entered by user
     mov  eax, 1         ; set eax to 1
displayPrime:
     call getNextPrime   ; place next prime number in eax
     call WriteDec       ; display prime in eax to terminal window
     call printWhiteSpace; display tab, line feed, or Press any key message
     loop displayPrime   ; loop for number of times entered by user
     call Crlf
     call Crlf
     pop  eax            ; restore user input
     ret
showPrimes ENDP

;------------------------------------------------------------------------------
; Prints a tab character if the current prime count is not a multiple of 10,
; or a line feed if it is a multiple of 10. Also prints a "any key to continue"
; message after displaying LINES_PER_PAGE rows.
; Receives:    primeCount contains the number of primes printed so far
;------------------------------------------------------------------------------
printWhiteSpace PROC USES eax ebx edx
     mov  eax,primeCount ; copy number displayed so far to eax
     cdq                 ; zero out edx
     mov  ebx, NUMS_PER_ROW
     div  ebx            ; divide by number of primes to display per row
     cmp  edx, 0         ; test if multiple of 10 by remainder of 0
     jne  not10th        ; jump if not a multiple of 10
     call Crlf           ; print line feed if multiple of 10
     jmp  anyKeyTest     ; test if Press any key message is required

not10th:     
     call printTab       ; display a tab character
     jmp  endWhiteSpace  ; done with whitespace

anyKeyTest:    ; now test if multiple of NUMS_PER_ROW * LINES_PER_PAGE
     mov  eax,primeCount ; copy number displayed so far to eax
     cdq                 ; zero out edx
     mov  ebx, NUMS_PER_ROW * LINES_PER_PAGE
     div  ebx            ; divide by NUMS_PER_ROW * LINES_PER_PAGE
     cmp  edx, 0         ; test if multiple by remainder
     jne  endWhiteSpace  ; jump to end if not a multiple
     mov  edx, OFFSET continue
     call WriteString    ; display Press any key message
     call ReadChar       ; block until any key is pressed
     call Crlf           ; display another line feed
     
endWhiteSpace:
     ret
printWhiteSpace ENDP

;------------------------------------------------------------------------------
; Gets the next prime number after the value in EAX.
; Receives:    EAX contains the current prime number
; Returns:     EAX contains the next prime number
;------------------------------------------------------------------------------
getNextPrime PROC
tryNext:
     inc  eax
     call isPrime
     cmp  ebx, 0         ; test if number is prime
     je   tryNext        ; try next number if ebx=0 (isPrime is false)
     ret                 ; number in eax is prime
getNextPrime ENDP

;------------------------------------------------------------------------------
; Tests whether the number in EAX is prime by dividing it by every number in primeDivisors.
; If it is, stores it in the primeDivisors array and increments primeCount.
; Receives:    EAX contains the number to test
; Returns:     EBX is set to 1 if the number is prime, or 0 if it is not.
;------------------------------------------------------------------------------
isPrime PROC USES ecx edx esi edi
     push eax            ; save number being tested on stack
     cmp  eax, 1         ; test if number <= 1
     jle  notPrime       ; return not prime if <= 1
     cmp  eax, 2         ; test if number is 2
     je   prime          ; number is prime if 2
     mov  edi,primeCount ; otherwise try to divide by numbers in primeDivisors
     
tryDivide:
     mov  eax, [esp]     ; put original number back into eax
     cdq                 ; zero out edx
     dec  edi            ; decrement array index
     mov  ebx, primeDivisors[edi*4]     ; copy prime divisor to ebx
     div  ebx            ; divide number by prime divisor
     cmp  edx, 0         ; test if remainder is 0
     je   notPrime       ; return not prime if remainder is 0
     cmp  edi, 0         ; test if first element of array was reached
     jg   tryDivide      ; repeat divide loop with new counter value
     
     ; number is prime if execution reaches here
     mov  ebx, 1         ; set ebx to 1 (true) if prime
     jmp  prime

notPrime:
     mov  ebx, 0         ; set ebx to 0 (false) if not prime
     jmp  endPrime       ; jump to end of procedure
     
prime:
     mov  esi,primeCount ; index to store result at in primeDivisors array
     mov  eax, [esp]     ; copy original number back into eax
     mov  primeDivisors[esi*4], eax     ; save prime to primeDivisors array
     inc  primeCount     ; increment the prime counter

endPrime:
     pop  eax; restore number
     ret
isPrime ENDP

;------------------------------------------------------------------------------
; Prints a tab character to the terminal window.
;------------------------------------------------------------------------------
printTab PROC USES eax
     mov  al, TAB
     call WriteChar      ; display tab character
     ret
printTab ENDP

;------------------------------------------------------------------------------
; Prints the goodbye message to the terminal window.
;------------------------------------------------------------------------------
farewell PROC USES edx
     mov  edx, OFFSET goodbye
     call WriteString
     call Crlf
     ret
farewell ENDP

END main