//The copy right belongs to Yu chunjiang
//chjyu@zhhz.suda.edu.cn
//2002.5.8
//for BJ
// BJDATA.h: interface for the CBJDATA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BJDATA_H__CE14B8D7_62DC_11D6_A5DD_0001024E6AF6__INCLUDED_)
#define AFX_BJDATA_H__CE14B8D7_62DC_11D6_A5DD_0001024E6AF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_LINE 100
#define MAX_WORD 100
//#define maxver 50
#define maxheight 250

#include <stdio.h>
#include <string.h>
#include "symnclass.h"


struct points
{
	int *x;
	int *y;
};

struct edge_info
{
	int yt;
	float xs,m;
	struct edge_info *next;
};

class CBJDATA  
{
public:
	void close_file(void);
	void write_char(char* str,int i);
	int maxver;
	int writefile(char * filename);
	struct points vertex;
	struct edge_info *egtab[maxheight];
	CFile bj_data_file;

	void draw(XYZ drm_XYZ,int drm_col,int turn);
	void drawfaultage();//断层
	int big_than_cur(int i);
	void imerge(struct edge_info** upper,struct edge_info** down ,int i);
	void recount_xs(int i);
	void del_edge(struct edge_info** egtab,int i);
	void fill_prog(int i,XYZ fillm_XYZ,int fillm_col,int turn);
	void fill_prog(int i);
	void create_edge_table(struct edge_info* edge[],int n,points vertex);
	void insert(struct edge_info* node,struct edge_info** list);
	float inverse(points vertex,int i);
	void init_egtab(struct edge_info* egtab[],int n);
//--------------------------
	int readword();
	int readline();
	int readfile(char *filename);
	void free();
	int m_SizeFile;
	char *m_pBuffer; //该指针未分配内存就直接使用 xxzhu 20170313
	int m_IndexBuffer;
	int line_count;
	CString m_FileName;
	char m_pBufferLine[MAX_LINE];
	char m_pBufferWord[MAX_WORD];

	CBJDATA();
	virtual ~CBJDATA();

};

#endif // !defined(AFX_BJDATA_H__CE14B8D7_62DC_11D6_A5DD_0001024E6AF6__INCLUDED_)
