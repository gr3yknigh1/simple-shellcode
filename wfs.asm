;
; FILE			wfs.asm
;
; AUTHORS
;
;				Ilya Akkuzin <gr3yknigh1@gmail.com>
;
; NOTICE        Copyright (c) 2025 Ilya Akkuzin. All rights reserved.
;
; REFERENCE
;
;               https://print3m.github.io/blog/x64-winapi-shellcoding
;				https://dennisbabkin.com/blog/?t=cpp-and-assembly-coding-in-x86-x64-assembly-language-in-visual-studio
;               https://learn.microsoft.com/en-us/cpp/assembler/inline/writing-functions-with-inline-assembly?view=msvc-170
;

.data

.code
ALIGN 16

;
; PPEB GetPEB(void);
;
; @brief Returns pointer to PEB structure.
;
; @return Obviously, returns pointer to PEB structure.
;
GetPEB PROC
	sub rsp, 28h
	mov rax, gs:[60h]  		; 0x60 - PEB_OFFSET
	add rsp, 28h
	ret
GetPEB ENDP

END