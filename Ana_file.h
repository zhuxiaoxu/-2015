// Ana_file.h: interface for the CAna_file class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANA_FILE_H__BA7FF361_812C_11D6_A5DD_0001024E6AF6__INCLUDED_)
#define AFX_ANA_FILE_H__BA7FF361_812C_11D6_A5DD_0001024E6AF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_LINE 200
#define MAX_WORD 200

struct ceng_xx  //����Ϣ
{
	char *sd;  //���
	char *bj;  //�߽�
	char *bj_created; //�߽�����
	char *dc;  //��նϲ�
	char *dc_open;  //���Ŷϲ�
	char *bsm; //��ʴ��
	char *wd;  //�¶�
	char *pyl; //������
	char *lj;  //·��
	char *amount; //����
	char *oil; //9.21
	ceng_xx() //�����ڴ�й©��xxzhu ����˹��캯�����ͷ��ڴ�ĺ���
	{
		sd=0;  //���
		bj=0;  //�߽�
		bj_created=0; //�߽�����
		dc=0;  //��նϲ�
		dc_open=0;  //���Ŷϲ�
		bsm=0; //��ʴ��
		wd=0;  //�¶�
		pyl=0; //������
		lj=0;  //·��
		amount=0; //����
		oil=0; //9.21
	}
	void reset()
	{
		if (sd != 0)  //���
			delete[]sd;
		if (bj != 0)  //�߽�
			delete[]bj;
		if (bj_created != 0) //�߽�����
			delete[]bj_created;
		if (dc != 0) 		  //��նϲ�
			delete[] dc;
		if (dc_open != 0) 	  //���Ŷϲ�
			delete[]dc_open;
		if (bsm != 0)		 //��ʴ��
			delete[]bsm;
		if (wd != 0)		 //�¶�
			delete[]wd;
		if (pyl != 0)		 //������
			delete[]pyl;
		if (lj != 0)		 //·��
			delete[]lj;
		if (amount != 0)		 //����
			delete[]amount;
		if (oil != 0) //9.21
			delete[]oil;
	}
};

class CAna_file  
{
public:
	char* string_right_value(char *a,char *b);
	int msxx();
	int xgxx();
	int zdxx();
	int fasx();
	int xinxi_a_a(int indexa,int indexb);
	int xinxi_a(int index);
	int readword();
	int readline();
	char *m_pBuffer;
	char m_pBufferLine[MAX_LINE];
	char m_pBufferWord[MAX_WORD];
	int m_SizeFile;
	int m_IndexBuffer;
	CString m_FileName;
	char buffer[64]; //������10��С��Ϊ�� xxzhu
	char *sjd,*sdcs,*fah,*sjds,*fazdr,*fazdrq,*faxgr,*faxgrq;
	//ʱ��Σ���Ȳ����������ţ�ʱ������������ƶ��ˣ������ƶ����ڣ������޸��ˣ������޸�����,������Ϣ
	char discription[101];  //��������Ϣ���50������
	struct ceng_xx ceng;

	int readfile(char *filename);
	CAna_file();
	virtual ~CAna_file();

};

#endif // !defined(AFX_ANA_FILE_H__BA7FF361_812C_11D6_A5DD_0001024E6AF6__INCLUDED_)
