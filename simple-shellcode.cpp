
#include <windows.h>  // Required for `winternl.h`, because he defines `_WIN32_WINNT` macro.
#include <winternl.h>


extern "C" PPEB __cdecl GetPEB(void);

int
main(void)
{
	int ret = EXIT_SUCCESS;

	/* Process Environment Block */
	PPEB peb = GetPEB();


	return ret;
}
