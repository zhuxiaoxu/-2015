#pragma once
#include "StdAfx.h"
class CLogClass
{
	char m_filename[256];
public:
	CLogClass();
	~CLogClass();
	void LogOut(char *str);
	void LogOut(char *str1, char *str2);
};

