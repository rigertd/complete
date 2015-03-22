TITLE Dog Years   (Project00.asm)

; Author: David Rigert
; Course / Project ID: Demo #0                 Date :
; Description: This program will introduce the programmer, get the user name and age
;              calculate the user's dog age, and report the result.

INCLUDE Irvine32.inc

DOG_FACTOR = 7

.data

userName  BYTE      33 DUP(0) ; string to be entered by user
userAge   DWORD     ?         ; integer to be entered by user
intro_1   BYTE      "Hi, my name is Lassie, and I'm here to help.", 0
prompt_1  BYTE      "What's your name? ", 0
intro_2   BYTE      "Nice to meet you, ", 0
prompt_2  BYTE      "How old are you? ", 0
dogAge    DWORD     ?
result_1  BYTE      "Wow... that's ", 0
result_2  BYTE      " in dog years!", 0
goodBye   BYTE      "Good-bye, ", 0

.code
main PROC

; Introduce programmer
     MOV  EDX, OFFSET intro_1
     CALL WriteString
     CALL CrLf

; Get user name
     MOV  EDX, OFFSET prompt_1
     CALL WriteString
     MOV  EDX, OFFSET userName; address to store user input in
     MOV  ECX, 32             ; max characters to read into EDX
     CALL ReadString

; Get user age
     MOV EDX, OFFSET prompt_2
     CALL WriteString
     CALL ReadInt
     MOV  userAge, EAX

; Calculate user "dog years"
     MOV  EAX, userAge
     MOV  EBX, DOG_FACTOR
     MUL  EBX
     MOV  dogAge, EAX

; Report result
     MOV  EDX, OFFSET result_1
     CALL WriteString
     MOV  EAX, dogAge
     CALL WriteDec
     MOV  EDX, OFFSET result_2
     CALL WriteString
     CALL CrLf

; Say "good-bye"
     MOV  EDX, OFFSET goodBye
     CALL WriteString
     MOV  EDX, OFFSET userName
     CALL WriteString
     CALL CrLf

     exit ; exit to operating system
main ENDP

; (insert additional procedures here)

END main