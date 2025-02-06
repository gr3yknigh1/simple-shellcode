
#include <windows.h>  // Required for `winternl.h`, because he defines `_WIN32_WINNT` macro.
#include <winternl.h>

#define USE_HIDDEN_API 1


#if !defined(RTL_INIT_CONSTANT_STRING)
#define RTL_CONSTANT_STRING(s) { sizeof((s)) - sizeof((s)[0]), sizeof((s)), (PWCH)(s) }
#endif

typedef UINT(WINAPI* WinExec_Type)(LPCSTR lpCmdLine, UINT uCmdShow);

//
// @brief Returns the pointer to PEB structure.
// 
// @remark Defines in `wsf.asm`.
// 
extern "C" PPEB __cdecl GetPEB(void) noexcept;

bool CompareUnicodeString(PUNICODE_STRING a, PUNICODE_STRING b) noexcept;
bool IsStringEquals(const char* a, const char* b) noexcept;

#if USE_HIDDEN_API
typedef struct _LDR_MODULE {
	LIST_ENTRY              InLoadOrderModuleList;
	LIST_ENTRY              InMemoryOrderModuleList;
	LIST_ENTRY              InInitializationOrderModuleList;

	PVOID                   BaseAddress;
	PVOID                   EntryPoint;
	ULONG                   SizeOfImage;
	UNICODE_STRING          FullDllName;
	UNICODE_STRING          BaseDllName;
	ULONG                   Flags;
	SHORT                   LoadCount;
	SHORT                   TlsIndex;
	LIST_ENTRY              HashTableEntry;
	ULONG                   TimeDateStamp;
} LDR_MODULE, * PLDR_MODULE;
#endif

int
main(void)
{
	int ret = EXIT_SUCCESS;

	/* Process Environment Block */
	PPEB peb = GetPEB();
	PPEB_LDR_DATA ldr = peb->Ldr;

#if USE_HIDDEN_API
	/* Hidden field: peb->InLoadOrderModuleList */
	PLIST_ENTRY start_ldr_entry = (PLIST_ENTRY)(ldr->Reserved2 + 1);
#else
	PLIST_ENTRY start_ldr_entry = &(ldr->InMemoryOrderModuleList);
#endif
	PLIST_ENTRY current_ldr_entry = start_ldr_entry;

	UNICODE_STRING kernel32_module_name = RTL_CONSTANT_STRING(L"KERNEL32.DLL");
	PIMAGE_DOS_HEADER kernel32_module_base = nullptr;

	do {
#if USE_HIDDEN_API
		PLDR_MODULE ldr_entry = (PLDR_MODULE)CONTAINING_RECORD(current_ldr_entry, LDR_MODULE, InLoadOrderModuleList);
#else
		PLDR_DATA_TABLE_ENTRY ldr_entry = (PLDR_DATA_TABLE_ENTRY)CONTAINING_RECORD(current_ldr_entry, LDR_DATA_TABLE_ENTRY, Reserved1);
#endif

#if USE_HIDDEN_API
		if (CompareUnicodeString(&kernel32_module_name, &ldr_entry->BaseDllName)) {
			kernel32_module_base = (PIMAGE_DOS_HEADER)ldr_entry->BaseAddress;
#else
		if (CompareUnicodeString(&kernel32_module_name, &ldr_entry->FullDllName)) {
			kernel32_module_base = *(PIMAGE_DOS_HEADER*)(ldr_entry->Reserved2);
#endif
			break;
		}
		
		current_ldr_entry = current_ldr_entry->Flink;
	} while (current_ldr_entry != start_ldr_entry);

	if (kernel32_module_base == nullptr) {
		return EXIT_FAILURE;
	}

	// Get address of PE headers
	intptr_t pe_headers = (intptr_t)((intptr_t)kernel32_module_base + kernel32_module_base->e_lfanew);

	constexpr size_t export_address_table_offset = 0x88;

	// Get Export Address Table (RVA - Relative Virtual Address)
	DWORD export_address_table_rva = *(PDWORD)(pe_headers + export_address_table_offset);

	// Get address of Export Address Table (RVA - Relative Virtual Address)
	PIMAGE_EXPORT_DIRECTORY export_address_table = (PIMAGE_EXPORT_DIRECTORY)((intptr_t)kernel32_module_base + export_address_table_rva);

	// Get address of function names table
	PDWORD function_name_rva = (PDWORD)((intptr_t)kernel32_module_base + export_address_table->AddressOfNames);

	// Get function name
	const char* target_function_name = "WinExec";
	size_t function_index = 0;

	do {
		char* current_function_name = (char*)((intptr_t)kernel32_module_base + function_name_rva[function_index]);

		if (IsStringEquals(current_function_name, target_function_name)) {
			break;
		}
		function_index++;
	} while (true);


	// Get function ordinal
	PWORD ordinals = (PWORD)((intptr_t)kernel32_module_base + export_address_table->AddressOfNameOrdinals);
	WORD ordinal = ordinals[function_index];

	// Get function pointer
	PDWORD func_rvas = (PDWORD)((intptr_t)kernel32_module_base + export_address_table->AddressOfFunctions);
	DWORD func_rva = func_rvas[ordinal];
	WinExec_Type win_exe_pointer = (WinExec_Type)((intptr_t)kernel32_module_base + func_rva);

	// Run WinAPI function
	const char* command_line = "calc.exe";
	win_exe_pointer(command_line, SW_SHOWNORMAL);

	return ret;
}

bool
CompareUnicodeString(PUNICODE_STRING a, PUNICODE_STRING b) noexcept
{
	if (a->Length != b->Length) {
		return false;
	}

	for (USHORT i = 0; a->Buffer[i] != 0 && b->Buffer[i] != 0; ++i) {
		if (a->Buffer[i] != b->Buffer[i]) {
			return false;
		}
	}

	return true;
}

bool
IsStringEquals(const char* a, const char* b) noexcept
{
	while (*a != 0 && *b != 0) {
		if (*a != *b) {
			return false;
		}

		a++;
		b++;
	}

	return true;
}