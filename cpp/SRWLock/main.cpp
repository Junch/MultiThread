#include <stdio.h>
#include <process.h>
#include <windows.h>

// http://blog.csdn.net/morewindows/article/details/7650574
// 秒杀多线程第十四篇 读者写者问题继 读写锁SRWLock

BOOL SetConsoleColor(WORD wAttributes)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
        return FALSE;
    return SetConsoleTextAttribute(hConsole, wAttributes);
}
const int READER_NUM = 5;
CRITICAL_SECTION g_cs;
SRWLOCK          g_srwLock;

void ReaderPrintf(char *pszFormat, ...)
{
    va_list   pArgList;
    va_start(pArgList, pszFormat);
    EnterCriticalSection(&g_cs);
    vfprintf(stdout, pszFormat, pArgList);
    LeaveCriticalSection(&g_cs);
    va_end(pArgList);
}

unsigned int __stdcall ReaderThreadFun(PVOID pM)
{
    ReaderPrintf("No:%d Reader begins to wait...\n", GetCurrentThreadId());

    AcquireSRWLockShared(&g_srwLock);

    ReaderPrintf("No:%d Reader begins to read...\n", GetCurrentThreadId());
    Sleep(rand() % 100);
    ReaderPrintf("No:%d Reader completes reading\n", GetCurrentThreadId());

    ReleaseSRWLockShared(&g_srwLock);
    return 0;
}

void WriterPrintf(char *pszStr)
{
    EnterCriticalSection(&g_cs);
    SetConsoleColor(FOREGROUND_GREEN);
    printf("     %s\n", pszStr);
    SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    LeaveCriticalSection(&g_cs);
}

unsigned int __stdcall WriterThreadFun(PVOID pM)
{
    WriterPrintf("  Writer begins to wait...");

    AcquireSRWLockExclusive(&g_srwLock);
        
    WriterPrintf("  Writer begins to write.....");
    Sleep(rand() % 100);
    WriterPrintf("  Writer completes writing");

    ReleaseSRWLockExclusive(&g_srwLock);
    return 0;
}

int main()
{
    printf("  Reader Writer Probelm: SRWLock\n");
    printf(" -- by MoreWindows( http://blog.csdn.net/MoreWindows ) --\n\n");

    InitializeCriticalSection(&g_cs);
    InitializeSRWLock(&g_srwLock);

    HANDLE hThread[READER_NUM + 1];
    int i;
    //Start two reader threads
    for (i = 1; i <= 2; i++)
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThreadFun, NULL, 0, NULL);
    //Start a writer thread
    hThread[0] = (HANDLE)_beginthreadex(NULL, 0, WriterThreadFun, NULL, 0, NULL);
    Sleep(50);
    //Start other reader threads
    for ( ; i <= READER_NUM; i++)
        hThread[i] = (HANDLE)_beginthreadex(NULL, 0, ReaderThreadFun, NULL, 0, NULL);
    WaitForMultipleObjects(READER_NUM + 1, hThread, TRUE, INFINITE);
    for (i = 0; i < READER_NUM + 1; i++)
        CloseHandle(hThread[i]);

    DeleteCriticalSection(&g_cs);
    return 0;
}
