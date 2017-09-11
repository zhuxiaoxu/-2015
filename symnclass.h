// symnclass.h: interface for the symnclass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYMNCLASS_H__009B606B_7923_4A6E_99AF_735D525691AD__INCLUDED_)
#define AFX_SYMNCLASS_H__009B606B_7923_4A6E_99AF_735D525691AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct hjdnode{
	int vos;
	float ylpec;
	struct hjdnode *next;
};
struct hjdtable{
	float yl;
	struct hjdnode *firsthjd;
};

//���ͱ�Ľڵ�ṹ
struct SybStr{
	int x;
	int y;
	float z;
	float yl;
	float ylpec;
	int flag;
	int zeroflag;
	struct SybStr *next;
};//����朱�
struct SybStrShX{
	int x;
	int y;
	float z;
	float yl;
	float ylpec;
	int flag;
	int zeroflag;
	struct SybStrShX *prev;
	struct SybStrShX *next;
};//˫��朱�
struct DOT{
	int x;
	int y;
	float z;
	float yl;
	float ylpec;
	int flag;
	int zeroflag;
//	struct hjdtable *hjdtable;
};
struct stack{
		struct DOT dot;
		struct stack *next;
	};
//�����ռ�ĵ�ṹ
struct zquque{
	int vpos;
	float z;
	struct zquque *next;
};
class XYZ {
public:
	int *x;
	int *y;
	float *z;
	float *yl;
	int *flag;//0���������͵�,1���������͵�
	float *ylpec;//ԭ����������ٷֱȡ�
	int *zeroflag;//����0��ǡ�����һ�������������Ϊ0�ĵط�Ҫ����Ϊ0

};

class symnclass
{
public:
	int m_col;
	int m_row;
	int whole_m_col;
	int whole_m_row;
	XYZ m_XYZ;
	struct hjdtable *m_YLT;
	struct zquque *m_zquque;
	int count;
	bool sfpop;
	int gdcount;
	float min;
	float max;

public:
	stack *stackmn;//�ڹ�ʱ�������·����ջ
//	char dirppath[200];
	SybStr *sybpointer;

	
public:
	symnclass(void);
	void loaddata(FILE *fp,FILE *fpbj);//���ļ�����ȡ��̨���еĿռ��
	void loadsyb(FILE *fp2,FILE *fpoil);//�����ͱ���һ������,���Ұ�zֵ�ɵ͵�������
	void findpath(struct DOT dot,FILE *fp);
	void push(DOT dot);
	struct DOT pop(stack *stackmn);
	struct SybStr* compls(DOT dot);//Ϊһ�����͵�ȥ������Χ�����ߵĵ�(����2��)
	struct SybStr* compls2(DOT dot);
	int stringtoint(char a[6]);//�Լ�д�İ��ַ���תΪ������������������
	void composeoil(XYZ m_XYZ);
	void dealdc(CString FileName,XYZ m_XYZ,int m_col,int m_row,int colmin,float colmax,int rowmin,float rowmax,int turn);
	
	~symnclass(void);
};





#endif // !defined(AFX_SYMNCLASS_H__009B606B_7923_4A6E_99AF_735D525691AD__INCLUDED_)
