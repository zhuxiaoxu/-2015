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

struct ceng_xx  //层信息
{
	char *sd;  //深度
	char *bj;  //边界
	char *bj_created; //边界生成
	char *dc;  //封闭断层
	char *dc_open;  //开放断层
	char *bsm; //剥蚀面
	char *wd;  //温度
	char *pyl; //排油量
	char *lj;  //路径
	char *amount; //增量
	char *oil; //9.21
	ceng_xx() //避免内存泄漏，xxzhu 添加了构造函数和释放内存的函数
	{
		sd=0;  //深度
		bj=0;  //边界
		bj_created=0; //边界生成
		dc=0;  //封闭断层
		dc_open=0;  //开放断层
		bsm=0; //剥蚀面
		wd=0;  //温度
		pyl=0; //排油量
		lj=0;  //路径
		amount=0; //增量
		oil=0; //9.21
	}
	void reset()
	{
		if (sd != 0)  //深度
			delete[]sd;
		if (bj != 0)  //边界
			delete[]bj;
		if (bj_created != 0) //边界生成
			delete[]bj_created;
		if (dc != 0) 		  //封闭断层
			delete[] dc;
		if (dc_open != 0) 	  //开放断层
			delete[]dc_open;
		if (bsm != 0)		 //剥蚀面
			delete[]bsm;
		if (wd != 0)		 //温度
			delete[]wd;
		if (pyl != 0)		 //排油量
			delete[]pyl;
		if (lj != 0)		 //路径
			delete[]lj;
		if (amount != 0)		 //增量
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
	char buffer[64]; //本来是10，小心为妙 xxzhu
	char *sjd,*sdcs,*fah,*sjds,*fazdr,*fazdrq,*faxgr,*faxgrq;
	//时间段，深度层数，方案号，时间段数，方案制定人，方案制定日期，方案修改人，方案修改日期,描述信息
	char discription[101];  //放描述信息最大50个中文
	struct ceng_xx ceng;

	int readfile(char *filename);
	CAna_file();
	virtual ~CAna_file();

};

#endif // !defined(AFX_ANA_FILE_H__BA7FF361_812C_11D6_A5DD_0001024E6AF6__INCLUDED_)
