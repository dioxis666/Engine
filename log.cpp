#pragma once
#include "Globals.h"

using namespace std;

string logBuffer;

void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);

	logBuffer += tmp_string2; //Stores all log chars into logBuffer string
}

//Return logBuffer string. Used to get the log buffer in ModuleEditor console
string GetLogBuffer(){
	return logBuffer;
}