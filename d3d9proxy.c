/* Lab-only proxy d3d9.dll: forwards Direct3DCreate9 to the system d3d9 and
   applies in-memory patches to PANZERS.EXE (file on disk stays original):
   1) resolution filter 4:3-only -> accept all modes (VA 0x42A50A: 74 17 -> EB 17)
   2) windowed style 0x00CF0000 (caption/frame) -> WS_POPUP 0x80000000 (borderless)
      at the immediates in FUN_005152a0 (create) and FUN_00515170 (fullscreen->windowed toggle). */
#include <windows.h>

typedef struct { DWORD va; BYTE len; BYTE from[4]; BYTE to[4]; } PATCH;

static const PATCH g_patches[] = {
    { 0x0042A50A, 1, { 0x74 },                   { 0xEB } },
    { 0x0051522D, 4, { 0x00, 0x00, 0xCF, 0x00 }, { 0x00, 0x00, 0x00, 0x80 } },
    { 0x00515337, 4, { 0x00, 0x00, 0xCF, 0x00 }, { 0x00, 0x00, 0x00, 0x80 } },
};

static void ApplyPatches(void)
{
    int i, k;
    for (i = 0; i < (int)(sizeof(g_patches) / sizeof(g_patches[0])); i++) {
        const PATCH *pt = &g_patches[i];
        BYTE *p = (BYTE *)pt->va;
        DWORD old;
        int match = 1;
        if (IsBadReadPtr(p, pt->len)) continue;
        for (k = 0; k < pt->len; k++) if (p[k] != pt->from[k]) match = 0;
        if (!match) continue;
        if (VirtualProtect(p, pt->len, PAGE_EXECUTE_READWRITE, &old)) {
            for (k = 0; k < pt->len; k++) p[k] = pt->to[k];
            VirtualProtect(p, pt->len, old, &old);
            FlushInstructionCache(GetCurrentProcess(), p, pt->len);
        }
    }
}

static HMODULE g_real;
typedef void *(WINAPI *PFN)(UINT);

void *WINAPI Direct3DCreate9(UINT v)
{
    PFN f;
    ApplyPatches();
    if (!g_real) {
        char path[MAX_PATH];
        GetSystemDirectoryA(path, MAX_PATH);
        lstrcatA(path, "\\d3d9.dll");
        g_real = LoadLibraryA(path);
    }
    if (!g_real) return 0;
    f = (PFN)GetProcAddress(g_real, "Direct3DCreate9");
    return f ? f(v) : 0;
}

BOOL WINAPI DllMain(HINSTANCE h, DWORD r, LPVOID x)
{
    if (r == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h); ApplyPatches(); }
    return TRUE;
}
