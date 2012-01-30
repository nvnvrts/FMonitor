#pragma once

#include <string>

using namespace std;

class Pipe
{
	HANDLE m_hChildStdInputWrDup;
	HANDLE m_hChildStdOutputRdDup;

public:
	Pipe();
	virtual ~Pipe();

	bool Open(const string& command, const string& argument);

	void Close();

	int Read(char* buf, const int size);
};
