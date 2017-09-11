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

class CFaultageProcess  //�ϲ㴦��
{
public:
	CFaultageProcess(int col,int row,float minx,float maxx,float miny,float maxy);
	virtual ~CFaultageProcess();

	float m_minx,m_miny,m_maxx,m_maxy;
	int m_col,m_row;
    VERTEXPOS *m_HeadArray[Max];//ָ��ĳһ����ε����жϲ������ͷָ�����ָ�룬��m_pHeadArray[1]ָ���1���ϲ������ͷָ��
    DVERTEXPOS m_DouHeadArray[Max];
	int faultage_total;//���ļ��ж���Ķϲ���������
	int m_faultagenum;//ʵ�ʿɵõ��Ķϲ���

	void Clear(VERTEXPOS* headarray[],int n);
    void Clear(DVERTEXPOS douheadarray[],int n);

	int GetFaultageNum(){return m_faultagenum;}
	VERTEXPOS** GetFaultageHead(){return m_HeadArray;}
	DVERTEXPOS* GetFaultageDouHead(){return m_DouHeadArray;}
	VERTEXPOS* Commerge(VERTEXPOS* pHead1,VERTEXPOS* pHead2);//�ϲ����������ֱ��ж��������׺�β�Ƿ�������������������ӵ�ʹ֮�������Ա�֤��һ�պ��߶�
    VERTEXPOS* Insert(int n,VERTEXPOS* pS,VERTEXPOS* pHead,XYZ dm_XYZ,int turn);//��pSǰ����n�����
    VERTEXPOS* Insert(VERTEXPOS* pS,int n,XYZ dm_XYZ,int turn);//��pS�����n�����
	void ProcessFaultage(char* FileName,XYZ dm_XYZ,int turn,CString writefile);//���ϲ��ļ�������ϲ�����
    void ReadFaultageTempFile1(CString FileName,XYZ dm_XYZ,int turn);//���ϲ���ʱ�ļ������ϲ㹴���߿�
    void ReadFaultageTempFile(char *FileName);//���ϲ���ʱ�ļ������ϲ���ɫ(������)
    void MakeVertexConsistent(VERTEXPOS* pHead1,VERTEXPOS *pHead2,XYZ dm_XYZ,int turn);//�ֱ�ʹpHead1 and pHead2�������еĵ�����
 
	CString GetTempFilePath(char *OldFileName);//OldFileName��ԭʼ�����ļ�·��
    
};

#endif // !defined(AFX_FAULTAGEPROCESS_H__DBED182C_D750_4778_A542_2FCB73236CB6__INCLUDED_)
