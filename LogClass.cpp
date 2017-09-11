#include "StdAfx.h"
#include "LogClass.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

bool  CheckFolderExist(char * strPath)
{
	WIN32_FIND_DATA  wfd;
	bool rValue = false;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = true;
	}
	FindClose(hFind);
	return rValue;
}

CLogClass::CLogClass()
{
	char filename[256];
	::SHGetSpecialFolderPathA(NULL, filename, CSIDL_APPDATA, FALSE);
	if (filename[strlen(filename) - 1] != '\\')
		strcat(filename, "\\");
	strcat(filename, "TSM2.0");
	if (CheckFolderExist(filename) == false)
	{
		if (!CreateDirectory(filename, NULL))
		{
			m_filename[0] = '\0';
			return;
		}
	}
	strcat(filename, "\\log-����.txt");
	strcpy(m_filename, filename);
	FILE * fp = fopen(m_filename, "w");//����һ�����ļ�
	fclose(fp);
}



CLogClass::~CLogClass()
{
}

void CLogClass::LogOut(char *str)
{
	if (m_filename[0] == '\0')
		return;
	FILE * fp = fopen(m_filename, "a");//����һ�����ļ�

	time_t temp = time(NULL);/*��ȡ��1970��1��1����ʱ�����ڵ����������浽����t��*/    
	tm *p = localtime(&temp); /*����t��ֵת��Ϊʵ������ʱ��ı�ʾ��ʽ*/
	fprintf(fp,"[%d��%02d��%02d��", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
	fprintf(fp,"%02d:%02d:%02d]", p->tm_hour, p->tm_min, p->tm_sec);
	fprintf(fp," %s\n", str);
	fclose(fp);
}

void CLogClass::LogOut(char *str1,char *str2)
{
	if (m_filename[0] == '\0')
		return;

	FILE * fp = fopen(m_filename, "a");//����һ�����ļ�
	time_t temp = time(NULL);/*��ȡ��1970��1��1����ʱ�����ڵ����������浽����t��*/
	tm *p = localtime(&temp); /*����t��ֵת��Ϊʵ������ʱ��ı�ʾ��ʽ*/
	fprintf(fp, "[%d��%02d��%02d��", (1900 + p->tm_year), (1 + p->tm_mon), p->tm_mday);
	fprintf(fp, "%02d:%02d:%02d]", p->tm_hour, p->tm_min, p->tm_sec);
	fprintf(fp, " %s", str1);
	fprintf(fp, " %s\n", str2);
	fclose(fp);
}

