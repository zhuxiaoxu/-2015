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

//生油表的节点结构
struct SybStr{
	int x;
	int y;
	float z;
	float yl;
	float ylpec;
	int flag;
	int zeroflag;
	struct SybStr *next;
};//单向鏈表
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
};//双向鏈表
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
//整个空间的点结构
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
	int *flag;//0代表不是生油点,1代表是生油点
	float *ylpec;//原是油量分配百分比。
	int *zeroflag;//设置0标记。用来一层算完后，油量该为0的地方要设置为0

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
	stack *stackmn;//第归时用来存放路径的栈
//	char dirppath[200];
	SybStr *sybpointer;

	
public:
	symnclass(void);
	void loaddata(FILE *fp,FILE *fpbj);//从文件中捞取后台所有的空间点
	void loadsyb(FILE *fp2,FILE *fpoil);//给生油表建立一个队列,并且按z值由低到高排序
	void findpath(struct DOT dot,FILE *fp);
	void push(DOT dot);
	struct DOT pop(stack *stackmn);
	struct SybStr* compls(DOT dot);//为一个生油点去找它周围比它高的点(大于2个)
	struct SybStr* compls2(DOT dot);
	int stringtoint(char a[6]);//自己写的把字符串转为整形数，后来不用了
	void composeoil(XYZ m_XYZ);
	void dealdc(CString FileName,XYZ m_XYZ,int m_col,int m_row,int colmin,float colmax,int rowmin,float rowmax,int turn);
	
	~symnclass(void);
};





#endif // !defined(AFX_SYMNCLASS_H__009B606B_7923_4A6E_99AF_735D525691AD__INCLUDED_)
