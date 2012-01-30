#include "stdafx.h"
#include <sstream>
#include "Error.h"
#include "Pipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

Pipe::Pipe()
	:m_hChildStdInputWrDup(INVALID_HANDLE_VALUE),
	 m_hChildStdOutputRdDup(INVALID_HANDLE_VALUE)
{
}

Pipe::~Pipe()
{
	Close();
}

bool Pipe::Open(const string& command, const string& argument)
{
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	HANDLE hChildStdInputRd;
	HANDLE hChildStdInputWr;

	if (!CreatePipe(&hChildStdInputRd, &hChildStdInputWr, &sa, 0))
	{
		throw Error("can't create pipe!");
	}

	if (!DuplicateHandle(GetCurrentProcess(),
		                 hChildStdInputWr,
						 GetCurrentProcess(),
						 &m_hChildStdInputWrDup,
						 0,
						 FALSE,
						 DUPLICATE_SAME_ACCESS))
	{
		throw Error("can't duplicate handle!");
	}

	CloseHandle(hChildStdInputWr);

	HANDLE hChildStdOutputRd;
	HANDLE hChildStdOutputWr;

	if (!CreatePipe(&hChildStdOutputRd, &hChildStdOutputWr, &sa, 0))
	{
		throw Error("can't create pipe!");
	}

	if (!DuplicateHandle(GetCurrentProcess(),
		                 hChildStdOutputRd,
						 GetCurrentProcess(),
						 &m_hChildStdOutputRdDup,
						 0,
						 FALSE,
						 DUPLICATE_SAME_ACCESS))
	{
		throw Error("can't duplicate handle!");
	}

	CloseHandle(hChildStdOutputRd);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_HIDE;
	si.hStdInput = hChildStdInputRd;
	si.hStdOutput = hChildStdOutputWr;
	si.hStdError = hChildStdOutputWr;
	si.dwFlags |= STARTF_USESTDHANDLES;

	string cmdline = command + " " + argument;

	BOOL br = CreateProcess(NULL,
		                    const_cast<LPSTR>(cmdline.c_str()),
							NULL,
							NULL,
							TRUE,
							0,
							NULL,
							NULL,
							&si,
							&pi);
	if (br != 0)
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		CloseHandle(hChildStdInputRd);
		CloseHandle(hChildStdOutputWr);
	}
	else
	{
		ostringstream oss;
		oss << "can't create pipe process!" 
			<< "(cmd=" << cmdline << ")"
			<< "(error=" << GetLastError() << ")";

		throw Error(oss.str());
	}

	return true;
}

void Pipe::Close()
{
	if (m_hChildStdInputWrDup != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hChildStdInputWrDup);
		m_hChildStdInputWrDup = INVALID_HANDLE_VALUE;
	}

	if (m_hChildStdOutputRdDup != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hChildStdOutputRdDup);
		m_hChildStdOutputRdDup = INVALID_HANDLE_VALUE;
	}
}

int Pipe::Read(char* buf, const int size)
{
	DWORD read = 0;

	if (!ReadFile(m_hChildStdOutputRdDup, buf, size, &read, 0))
	{
		return -1;
	}
	else
	{
		return read;
	}
}
