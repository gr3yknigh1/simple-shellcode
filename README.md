# Shellcode

> This project is experimentation on how to operate in compiled shellcode, without having any linked libraries.

## Reference

- https://learn.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-peb
- https://learn.microsoft.com/en-us/windows/win32/api/winternl/ns-winternl-peb_ldr_data
- http://undocumented.ntinternals.net/index.html?page=UserMode%2FUndocumented%20Functions%2FNT%20Objects%2FProcess%2FPEB.html
- http://undocumented.ntinternals.net/index.html?page=UserMode%2FStructures%2FPEB_LDR_DATA.html
- http://undocumented.ntinternals.net/index.html?page=UserMode%2FStructures%2FLDR_MODULE.html
- https://en.wikipedia.org/wiki/Data_structure_alignment
- https://blog.filovirid.com/page/Windows-Portable-Executable-Files-Structure

## Source

- https://yelhamer.github.io/posts/Resolving-Windows-API-Functions-via-the-PEB_LDR_DATA-Structure/
- https://print3m.github.io/blog/x64-winapi-shellcoding
- https://print3m.github.io/blog/from-c-to-shellcode
- https://sandsprite.com/CodeStuff/Understanding_the_Peb_Loader_Data_List.html
- https://mohamed-fakroud.gitbook.io/red-teamings-dojo/windows-internals/peb
- https://www.stackzero.net/dll-injection-example/
- https://www.stackzero.net/how-to-embed-a-shellcode-payload-into-an-executable/
- https://www.stackzero.net/process-injection-by-example/
