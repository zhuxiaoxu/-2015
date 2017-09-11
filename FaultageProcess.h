// FaultageProcess.h: interface for the CFaultageProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FAULTAGEPROCESS_H__DBED182C_D750_4778_A542_2FCB73236CB6__INCLUDED_)
#define AFX_FAULTAGEPROCESS_H__DBED182C_D750_4778_A542_2FCB73236CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "symnclass.h"
#define Max 500
struct VERTEXPOS{
   int vpos;
   VERTEXPOS* next;

};

struct DVERTEXPOS{
	VERTEXPOS* pHead1;
	VERTEXPOS* pHead2;
};

class CFaultageProcess  //断层处理
{
public:
	CFaultageProcess(int col,int row,float minx,float maxx,float miny,float maxy);
	virtual ~CFaultageProcess();

	float m_minx,m_miny,m_maxx,m_maxy;
	int m_col,m_row;
    VERTEXPOS *m_HeadArray[Max];//指向某一层地形的所有断层点链表头指针的总指针，如m_pHeadArray[1]指向第1条断层链表的头指针
    DVERTEXPOS m_DouHeadArray[Max];
	int faultage_total;//从文件中读入的断层数（有误差）
	int m_faultagenum;//实际可得到的断层数

	void Clear(VERTEXPOS* headarray[],int n);
    void Clear(DVERTEXPOS douheadarray[],int n);

	int GetFaultageNum(){return m_faultagenum;}
	VERTEXPOS** GetFaultageHead(){return m_HeadArray;}
	DVERTEXPOS* GetFaultageDouHead(){return m_DouHeadArray;}
	VERTEXPOS* Commerge(VERTEXPOS* pHead1,VERTEXPOS* pHead2);//合并两链表，并分别判断两链表首和尾是否连续，若不连续，则加点使之连续，以保证成一闭合线段
    VERTEXPOS* Insert(int n,VERTEXPOS* pS,VERTEXPOS* pHead,XYZ dm_XYZ,int turn);//在pS前插入n个结点
    VERTEXPOS* Insert(VERTEXPOS* pS,int n,XYZ dm_XYZ,int turn);//在pS后插入n个结点
	void ProcessFaultage(char* FileName,XYZ dm_XYZ,int turn,CString writefile);//读断层文件并处理断层数据
    void ReadFaultageTempFile1(CString FileName,XYZ dm_XYZ,int turn);//读断层临时文件并画断层勾勒线框
    void ReadFaultageTempFile(char *FileName);//读断层临时文件并画断层填色(暂无用)
    void MakeVertexConsistent(VERTEXPOS* pHead1,VERTEXPOS *pHead2,XYZ dm_XYZ,int turn);//分别使pHead1 and pHead2相邻两行的点连续
 
	CString GetTempFilePath(char *OldFileName);//OldFileName是原始数据文件路径
    
};

#endif // !defined(AFX_FAULTAGEPROCESS_H__DBED182C_D750_4778_A542_2FCB73236CB6__INCLUDED_)
