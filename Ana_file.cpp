// Ana_file.cpp: implementation of the CAna_file class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ana_file.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAna_file::CAna_file()
{
	m_pBuffer = nullptr; //指针赋空 add by xxzhu 20170314
	fah = nullptr;
	sjds = nullptr;
	sjd = nullptr;
	sdcs = nullptr;
	m_IndexBuffer = 0;
}

CAna_file::~CAna_file()
{
	if (m_pBuffer)		//释放内存 add by xxzhu 20170314
		delete[]m_pBuffer;
	if (fah)
		delete[]fah;
	if (sjds)
		delete[]sjds;

}




int CAna_file::readfile(char *filename)
{
	m_FileName = filename;

	CFile file;
	CFileException ex;

	// Opening
	TRACE("  opening...");
	if (!file.Open(filename, CFile::modeRead | CFile::typeBinary, &ex))
	{
		MessageBox(NULL, filename, "can not open", MB_OK);
#ifdef _DEBUG
		afxDump << "File could not be opened " << ex.m_cause << "\n";
#endif
		TRACE("unable to open file for reading\n");
		return 0;
	}
	TRACE("ok\n");

	// Size file
	m_SizeFile = file.GetLength();
	TRACE("  length : %d bytes\n", m_SizeFile);

	// Alloc
	TRACE("  alloc...");
	//============================================================================
	if (m_pBuffer)  //防止多次调用本函数，如果这样会出现内存泄漏，xxzhu 20170314
		delete[]m_pBuffer;
	//============================================================================
	m_pBuffer = new char[m_SizeFile];
	m_IndexBuffer = 0;
	if (m_pBuffer == NULL)
	{
		TRACE("insuffisant memory\n");
		return 0;
	}
	TRACE("ok\n");

	// Reading
	TRACE("  reading...");
	TRY
	{
		file.Read(m_pBuffer,m_SizeFile);
	}
		CATCH(CFileException, e)
	{
#ifdef _DEBUG
		afxDump << "Error during reading " << e->m_cause << "\n";
#endif
		TRACE("error during reading\n");
		file.Close();
		return 0;
	}
	END_CATCH
		TRACE("ok\n");

	// Closing
	file.Close();

	return 1;

}

int CAna_file::readline()
{
	for (int index = 0; index < MAX_LINE; index++)
		m_pBufferLine[index] = '\0'; //清空行缓冲

	int i = 0;
	do
		m_pBufferLine[i++] = m_pBuffer[m_IndexBuffer++];
	while (m_pBuffer[m_IndexBuffer - 1] != '\n' &&
		i < MAX_LINE &&
		m_IndexBuffer < m_SizeFile);

	m_pBufferLine[i - 2] = '\0';

	//TRACE("  line : %s\n",m_pBufferLine);

	return 1;
}

int CAna_file::readword()
{
	for (int index = 0; index < MAX_WORD; index++)
		m_pBufferWord[0] = '\0'; //清空行缓冲

	int i = 0;

	// Jump to next valid character
	while ((m_pBuffer[m_IndexBuffer] == '\n' ||
		m_pBuffer[m_IndexBuffer] == '\t' ||
		m_pBuffer[m_IndexBuffer] == '\r' ||
		m_pBuffer[m_IndexBuffer] == ' ' ||
		m_pBuffer[m_IndexBuffer] == ',') &&
		m_IndexBuffer < m_SizeFile)
		m_IndexBuffer++;

	// Check eof
	if (m_IndexBuffer >= m_SizeFile)
		return 0;

	do
		m_pBufferWord[i++] = m_pBuffer[m_IndexBuffer++];
	while (m_pBuffer[m_IndexBuffer - 1] != '\n' &&
		m_pBuffer[m_IndexBuffer - 1] != '\t' &&
		m_pBuffer[m_IndexBuffer - 1] != '\r' &&
		m_pBuffer[m_IndexBuffer - 1] != ' ' &&
		m_pBuffer[m_IndexBuffer] != ','  &&
		i < MAX_WORD &&
		m_IndexBuffer < m_SizeFile);

	m_pBufferWord[i] = '\0';

	//TRACE("  word : %s\n",m_pBufferWord);

	return 1;

}

/*
//读取指定时间段的信息，读取到：一个时间段和它的深度层次数量
//参数是时间段编号
//返回值：1读取成功
//		: 0读取失败
*/
int CAna_file::xinxi_a(int index)
{
	int result;
	CString str;
	str = "[xx";
	_itoa(index, buffer, 10);
	str = str + buffer;
	str = str + "]"; //for example [xx1]
	int str_length;
	str_length = str.GetLength();
	LPTSTR p = str.GetBuffer(str_length);
	str.ReleaseBuffer();

	char *temp;
	while (m_IndexBuffer < m_SizeFile)
	{
		readline();//读取一行
		result = strcmp(p, m_pBufferLine);
		if (result == 0)  //equal  ，看是不是当前深度层的标记
		{
			readline();
			char *p= string_right_value(m_pBufferLine, "sjd:");
			int length;
			length = strlen(p);
			temp = new char[length + 1];
			memset(temp, '\0', length + 1);
			memcpy(temp, p, length);
			if (sjd) 
				delete []sjd;
			sjd = temp;  //时间段

			readline();//读取一行
			p = string_right_value(m_pBufferLine, "sdcs:");
			length = strlen(p);
			temp = new char[length + 1];
			memset(temp, '\0', length + 1);
			memcpy(temp, p, length);
			if (sdcs)
				delete[] sdcs;//深度层数
			sdcs = temp;

			m_IndexBuffer = 0;
			return 1; //success
		}
	}

	return 0;  //failure

}

/*
读取指定时间段和指定层次的数据信息, 1个时间段可以多个层
参数：indexa 是时间段编号
    ：indexb 是层次编号
返回值	：1 读取成功
		：0 读取失败
*/
int CAna_file::xinxi_a_a(int indexa, int indexb)
{
	int result;
	CString str;
	str = "[xx";
	_itoa(indexa, buffer, 10);
	str = str + buffer;
	str = str + "-";
	_itoa(indexb, buffer, 10);
	str = str + buffer;
	str = str + "]"; //for example [xx1-1]
	int str_length;
	str_length = str.GetLength();
	LPTSTR p = str.GetBuffer(str_length);
	str.ReleaseBuffer();

	char *temp;
	while (m_IndexBuffer < m_SizeFile)
	{
		readline();

		result = strcmp(p, m_pBufferLine);
		if (result == 0)  //equal
		{
			ceng.reset();// add by xxzhu 释放该结构体变量内部指针之前申请的内存，否则内存泄漏 20170314
			readline();
			ceng.sd = string_right_value(m_pBufferLine, "sd:");
			if (ceng.sd[0] == '\0')
				ceng.sd = NULL;
			else
			{
				int length;
				length = strlen(ceng.sd);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.sd, length);
				ceng.sd = temp;
			}

			readline();
			ceng.bj = string_right_value(m_pBufferLine, "bj:");
			if (ceng.bj[0] == '\0')
				ceng.bj = NULL;
			else
			{
				int length;
				length = strlen(ceng.bj);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.bj, length);
				ceng.bj = temp;
			}

			readline();
			ceng.bj_created = string_right_value(m_pBufferLine, "bj_created:");
			if (ceng.bj_created[0] == '\0')
				ceng.bj_created = NULL;
			else
			{
				int length;
				length = strlen(ceng.bj_created);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.bj_created, length);
				ceng.bj_created = temp;
			}

			readline();
			ceng.dc = string_right_value(m_pBufferLine, "dc:");
			if (ceng.dc[0] == '\0')
				ceng.dc = NULL;
			else
			{
				int length;
				length = strlen(ceng.dc);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.dc, length);
				ceng.dc = temp;
			}

			readline();
			ceng.dc_open = string_right_value(m_pBufferLine, "dc_open:");
			if (ceng.dc_open[0] == '\0')
				ceng.dc_open = NULL;
			else
			{
				int length;
				length = strlen(ceng.dc_open);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.dc_open, length);
				ceng.dc_open = temp;
			}

			readline();
			ceng.bsm = string_right_value(m_pBufferLine, "bsm:");
			if (ceng.bsm[0] == '\0')
				ceng.bsm = NULL;
			else
			{
				int length;
				length = strlen(ceng.bsm);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.bsm, length);
				ceng.bsm = temp;
			}

			readline();
			ceng.wd = string_right_value(m_pBufferLine, "wd:");
			if (ceng.wd[0] == '\0')
				ceng.wd = NULL;
			else
			{
				int length;
				length = strlen(ceng.wd);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.wd, length);
				ceng.wd = temp;
			}

			readline();
			ceng.pyl = string_right_value(m_pBufferLine, "pyl:");
			if (ceng.pyl[0] == '\0')
				ceng.pyl = NULL;
			else
			{
				int length;
				length = strlen(ceng.pyl);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.pyl, length);
				ceng.pyl = temp;
			}

			readline();
			ceng.lj = string_right_value(m_pBufferLine, "lj:");
			if (ceng.lj[0] == '\0')
				ceng.lj = NULL;
			else
			{
				int length;
				length = strlen(ceng.lj);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.lj, length);
				ceng.lj = temp;
			}

			readline();
			ceng.amount = string_right_value(m_pBufferLine, "amount:");
			if (ceng.amount[0] == '\0')
				ceng.amount = NULL;
			else
			{
				int length;
				length = strlen(ceng.amount);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.amount, length);
				ceng.amount = temp;
			}

			readline();
			ceng.oil = string_right_value(m_pBufferLine, "tempoil:");
			if (ceng.oil[0] == '\0')
				ceng.oil = NULL;
			else
			{
				int length;
				length = strlen(ceng.oil);
				temp = new char[length + 1];
				memset(temp, '\0', length + 1);
				memcpy(temp, ceng.oil, length);
				ceng.oil = temp; //9.21
			}

			m_IndexBuffer = 0;
			return 1; //success
		}
	}

	return 0;  //failure
}
/*
add by xxzhu 20170314
本函数用于读取方案的属性信息
参数：	无
返回值：成功读取数据，则返回1
		否则返回0
*/
int CAna_file::fasx()
{
	int result;
	char *fasx = "[fasx]";  //fang'an shu xing.
	char *temp;

	readline();//读取一行

	result = strcmp(fasx, m_pBufferLine); //判断第一行是不是正规文件头[fasx]
	if (result == 0)  //equal
	{
		readline();//读取一行
		char *p= string_right_value(m_pBufferLine, "fah:"); //方案号,本来的内存使用有问题，已经修正 xxzhu 20170314
		int length= strlen(p);
		temp = new char[length + 1];
		memset(temp, '\0', length + 1);
		memcpy(temp, p, length);
		if (fah)	//防止多次调用导致内存泄漏
			delete[]fah;
		fah = temp;

		readline();//读取一行
		p = string_right_value(m_pBufferLine, "sjds:"); //时间段数，本来的内存使用有问题，已经修正 xxzhu 20170314
		length = strlen(p);
		temp = new char[length + 1];
		memset(temp, '\0', length + 1);
		memcpy(temp, p, length);
		if (sjds)	//防止多次调用导致内存泄漏
			delete[]sjds;
		sjds = temp;

		m_IndexBuffer = 0;
		return 1; //success
	}

	return 0;  //failure
}

int CAna_file::zdxx()
{
	int result;
	char *zdxx = "[zdxx]";
	char *temp;

	while (m_IndexBuffer < m_SizeFile)
	{
		readline();

		result = strcmp(zdxx, m_pBufferLine);
		if (result == 0)  //equal
		{
			readline();
			fazdr = string_right_value(m_pBufferLine, "fazdr:");
			int length;
			length = strlen(fazdr);
			temp = new char[length + 1];
			memset(temp, '\0', length + 1);
			memcpy(temp, fazdr, length);
			fazdr = temp;

			readline();
			fazdrq = string_right_value(m_pBufferLine, "fazdrq:");
			length = strlen(fazdrq);
			temp = new char[length + 1];
			memset(temp, '\0', length + 1);
			memcpy(temp, fazdrq, length);
			fazdrq = temp;

			m_IndexBuffer = 0;
			return 1; //success
		}
	}

	return 0;  //failure
}

int CAna_file::xgxx()
{
	int result;
	char *xgxx = "[xgxx]";
	char *temp;

	while (m_IndexBuffer < m_SizeFile)
	{
		readline();

		result = strcmp(xgxx, m_pBufferLine);
		if (result == 0)  //equal
		{
			readline();
			faxgr = string_right_value(m_pBufferLine, "faxgr:");
			int length;
			length = strlen(faxgr);
			temp = new char[length + 1];
			memset(temp, '\0', length + 1);
			memcpy(temp, faxgr, length);
			faxgr = temp;

			readline();
			faxgrq = string_right_value(m_pBufferLine, "faxgrq:");
			length = strlen(faxgrq);
			temp = new char[length + 1];
			memset(temp, '\0', length + 1);
			memcpy(temp, faxgrq, length);
			faxgrq = temp;

			m_IndexBuffer = 0;
			return 1; //success
		}
	}

	return 0;  //failure
}

int CAna_file::msxx()
{
	int result;
	char *ms = "[msxx]";
	int i = 0;

	while (m_IndexBuffer < m_SizeFile)
	{
		readline();
		memset(discription, '\0', 101);
		result = strcmp(ms, m_pBufferLine);
		if (result == 0)  //equal
		{

			while (m_pBuffer[m_IndexBuffer] != '\r' && m_pBuffer[m_IndexBuffer] != '\n' && i < 101)
			{
				discription[i++] = m_pBuffer[m_IndexBuffer];
				m_IndexBuffer++;
			}
			discription[i] = '\0';

			m_IndexBuffer = 0;
			return 1; //success
		}
	}

	return 0;  //failure
}

char* CAna_file::string_right_value(char *a, char *b)  //:右面的字符串
{
	while ((*a != ':') && (*a == *b))
	{
		a++;
		b++;
	}

	if (*a == ':')
	{
		a++;
		return a;
	}
	else
		AfxMessageBox("字符串不相同！");

	return NULL;

}
