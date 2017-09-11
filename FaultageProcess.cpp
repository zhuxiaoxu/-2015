// FaultageProcess.cpp: implementation of the CFaultageProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FaultageProcess.h"
#include "BJDATA.h"
#include "math.h"
#include "symnclass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFaultageProcess::CFaultageProcess(int col,int row,float minx,float maxx,float miny,float maxy)
{
   m_col=col;
   m_row=row;
   m_minx=minx;
   m_maxx=maxx;
   m_miny=miny;
   m_maxy=maxy;

   faultage_total=0;
   m_faultagenum=0;

}

CFaultageProcess::~CFaultageProcess()
{
   Clear(m_DouHeadArray,m_faultagenum);
   Clear(m_HeadArray,m_faultagenum);
}

void CFaultageProcess::Clear(VERTEXPOS* headarray[],int n)
{
	for(int i=0;i<n;i++)
		headarray[i]=NULL;
}
void CFaultageProcess::Clear(DVERTEXPOS douheadarray[],int n)
{
	for(int i=0;i<n;i++)
	{
		douheadarray[i].pHead1=NULL;
    	douheadarray[i].pHead2=NULL;
	}
}
void CFaultageProcess::ProcessFaultage(char* FileName,XYZ dm_XYZ,int turn,CString writefile)
{

		
		//读断层数据，（封闭的或者不封闭的）
	char ret[]="\r\n";
	char sharp[]="##";
	char zero[]="-1";
	char star[]="**";
	CBJDATA	bj_data;
//	CBJDATA write_data;
	
    CString filename=(CString)FileName;
	int length=filename.GetLength();
	length++;
	LPTSTR p = filename.GetBuffer(length);
	char file[256];
	strcpy(file, ::globalDatPath); //add by xxzhu 20170314
	if(*p!='\\')
		strcat(file, "\\");		
	strcat(file, p);
	filename.ReleaseBuffer();
	//bj_data.readfile(p);
	bj_data.readfile(file);

	int success;
	int duanceng_total;  //所有断层数
	int point_count;    //每一层的点数
	bj_data.readline();//读无效数据wmp123  xxzhu 20170313注释，这里会出错,因为使用了无效指针
	bj_data.readline();//读断层数
	success=sscanf(bj_data.m_pBufferLine,"%d",&duanceng_total);
	ASSERT(success==1);
	faultage_total=duanceng_total;
    m_faultagenum=faultage_total;
	int newlength;
	newlength=writefile.GetLength();
	newlength++;
	LPTSTR newfilename=writefile.GetBuffer(newlength);
//	char file[256];
	strcpy(file, ::globalDatPath);
	if (*newfilename != '\\')
		strcat(file, "\\");
	strcat(file, newfilename);
	writefile.ReleaseBuffer();
	//if(!bj_data.writefile(newfilename))
	if (!bj_data.writefile(file))
	{
	   AfxMessageBox("FaultageWritingFile could not be opened.");
	   return;
	}
	for(int i=1;i<=duanceng_total;i++)
	{

		bj_data.readline();//读无效数据
		bj_data.readline();//读该层有多少个点
		if (isdigit(bj_data.m_pBufferLine[0]))		//有时数据不满12个，用户给的数据是用几个回车来代替的
		{
			success=sscanf(bj_data.m_pBufferLine,"%d",&point_count);
			ASSERT(success==1);
		}
		else
		  point_count=0;
		float tempf;
		bj_data.vertex.x=new int[point_count];
		bj_data.vertex.y=new int[point_count];
		bj_data.maxver=point_count;
		if (point_count>0)
		{
			for(int j=0;j<bj_data.maxver;j++)
			{
				bj_data.readword();
				success=sscanf(bj_data.m_pBufferWord,"%f",&tempf);
				ASSERT(success==1);
				bj_data.vertex.x[j]=(int)((m_col-1)*(tempf-m_minx)/(m_maxx-m_minx)+0.5f);//网格上的x

				bj_data.readword();
				success=sscanf(bj_data.m_pBufferWord,"%f",&tempf);
				ASSERT(success==1);
				bj_data.vertex.y[j]=(int)((m_row-1)*(tempf-m_miny)/(m_maxy-m_miny)+0.5f);//网格上的y 

				//这里的x[]和y[]都不应该出现负数 xxzhu 20170326
				if (bj_data.vertex.x[j] < 0 || bj_data.vertex.y[j] < 0)
				{
					throw i; //把线号作为异常抛回去
				}
			}
		
		//指针数组，每个元素是指向struct edge_info的结构
			
			bj_data.init_egtab(bj_data.egtab, maxheight);
			bj_data.create_edge_table(bj_data.egtab, bj_data.maxver, bj_data.vertex);
			bj_data.line_count = 0;
			bj_data.drawfaultage();
			TRACE("%d,%d ", bj_data.vertex.x[0], bj_data.vertex.y[0]);
			

		}
		else break;
		if(bj_data.line_count==0) 	
		{
           int i=0;
		   char bufx[6]={0,0,0,0,0,0};
           char bufy[6]={0,0,0,0,0,0};
           char spc[]=" ";
		   int x,y;
		   while(i<(bj_data.maxver-1))
		   {
			   for(int k=0;k<2;k++)
			   {_itoa(bj_data.vertex.y[i],bufy,10);
			   bj_data.write_char(bufy,strlen(bufy));
			   bj_data.write_char(spc,1);
			   _itoa(bj_data.vertex.x[i],bufx,10);
			   bj_data.write_char(bufx,strlen(bufx));
			   bj_data.write_char(ret,strlen(ret));}
			   if((bj_data.vertex.x[i]==bj_data.vertex.x[i+1])//处理竖直线
                &&(bj_data.vertex.y[i]!=bj_data.vertex.y[i+1]))                                                                  
               {
				   int margin=bj_data.vertex.y[i]-bj_data.vertex.y[i+1];
                   if(abs(margin)>1)
				   {
					   int j=1;
  					   while(j<abs(margin))
					   {
						   x=bj_data.vertex.x[i];
                           if(margin>0)
						      y=bj_data.vertex.y[i]-j;
						   else
                              y=bj_data.vertex.y[i]+j;	
						   for(int k=0;k<2;k++)
						   {_itoa(y,bufy,10);
						   bj_data.write_char(bufy,strlen(bufy));
						   bj_data.write_char(spc,1);
						   _itoa(x,bufx,10);
						   bj_data.write_char(bufx,strlen(bufx));
						   bj_data.write_char(ret,strlen(ret));
						   }
                           j++;
					   }
				   }
			   }
			   if((bj_data.vertex.y[i]==bj_data.vertex.y[i+1])//处理横直线
                &&(bj_data.vertex.x[i]!=bj_data.vertex.x[i+1]))                                                                  
               {
				   int margin=bj_data.vertex.x[i]-bj_data.vertex.x[i+1];
                   if(abs(margin)>1)
				   {
					   int j=1;
  					   while(j<abs(margin))
					   {
						   y=bj_data.vertex.y[i];
                           if(margin>0)
						      x=bj_data.vertex.x[i]-j;
						   else
                              x=bj_data.vertex.x[i]+j;						   
						   for(int k=0;k<2;k++)
						  { _itoa(y,bufy,10);
						   
						   bj_data.write_char(bufy,strlen(bufy));
						   bj_data.write_char(spc,1);
						   _itoa(x,bufx,10);
						   bj_data.write_char(bufx,strlen(bufx));
						   bj_data.write_char(ret,strlen(ret));}
                           j++;
					   }					   
				   }
			   }
			   if((bj_data.vertex.y[i]!=bj_data.vertex.y[i+1])//处理斜直线
                &&(bj_data.vertex.x[i]!=bj_data.vertex.x[i+1]))
               {
				   int margin=bj_data.vertex.y[i]-bj_data.vertex.y[i+1];
				   if(abs(margin)>=1)//非相邻行处理
				   {
					   int yl;
					   int j=1;
					   int k;
					   k=(bj_data.vertex.y[i+1]-bj_data.vertex.y[i])/(bj_data.vertex.x[i+1]-bj_data.vertex.x[i]);
					   while(j<=abs(margin))
					   {
						   if(margin>0) yl=bj_data.vertex.y[i]-j;
						   else yl=bj_data.vertex.y[i]+j;
						   ///求斜线与扫描行yl的交点，再近似到网格点上
                           x=(int)((bj_data.vertex.x[i]*(yl-bj_data.vertex.y[i+1])-bj_data.vertex.x[i+1]*(yl-bj_data.vertex.y[i]))/(margin)+0.5f);
				           y=yl+1;
						  /*if (turn==1)		//代表是封闭断层
								if (dm_XYZ.flag[y*m_col+x]!=30 || dm_XYZ.flag[y*m_col+x]!=40)
								dm_XYZ.flag[y*m_col+x]=3;
							else			//代表是开放式断层
								if (dm_XYZ.flag[y*m_col+x]!=30 || dm_XYZ.flag[y*m_col+x]!=40)
								dm_XYZ.flag[y*m_col+x]=4;
							if (k>0)  //右
							{
								if (margin>0) //右上
								{
									//处理两个对角点	
									if (turn==1)		//代表是封闭断层
										dm_XYZ.flag[y*m_col+x-1]=30;
									else			//代表是开放式断层
										dm_XYZ.flag[y*m_col+x-1]=40;
									if (turn==1)		//代表是封闭断层
										dm_XYZ.flag[(y+1)*m_col+x]=30;
									else			//代表是开放式断层
										dm_XYZ.flag[(y+1)*m_col+x]=40;	
								}
								else	//右下
								{
									if (turn==1)		//代表是封闭断层
									dm_XYZ.flag[yl*m_col+x-1]=30;
									else			//代表是开放式断层
									dm_XYZ.flag[yl*m_col+x-1]=40;
									if (turn==1)		//代表是封闭断层
									dm_XYZ.flag[(yl-1)*m_col+x]=30;
									else			//代表是开放式断层
									dm_XYZ.flag[(yl-1)*m_col+x]=40;	
								}


							}
							else  //左
							{
								if (margin>0) //左上
								{
									if (turn==1)		//代表是封闭断层
									dm_XYZ.flag[yl*m_col+x+1]=30;
									else			//代表是开放式断层
									dm_XYZ.flag[yl*m_col+x+1]=40;
									if (turn==1)		//代表是封闭断层
									dm_XYZ.flag[(yl+1)*m_col+x]=30;
									else			//代表是开放式断层
									dm_XYZ.flag[(yl+1)*m_col+x]=40;	
								}
								else	//左下
								{
									if (turn==1)		//代表是封闭断层
									dm_XYZ.flag[yl*m_col+x+1]=30;
									else			//代表是开放式断层
									dm_XYZ.flag[yl*m_col+x+1]=40;
									if (turn==1)		//代表是封闭断层
									dm_XYZ.flag[(yl-1)*m_col+x]=30;
									else			//代表是开放式断层
									dm_XYZ.flag[(yl-1)*m_col+x]=40;	
								}
							}*/
						   for(int k=0;k<2;k++)
						   {_itoa(y,bufy,10);

						   bj_data.write_char(bufy,strlen(bufy));
						    bj_data.write_char(spc,1);
						    _itoa(x,bufx,10);
						    bj_data.write_char(bufx,strlen(bufx));
						    bj_data.write_char(ret,strlen(ret));
						   }


						   j++;
					   }
				   }
			   }
			i++;
		   }
		   ///写最后一个点
		   x=bj_data.vertex.x[bj_data.maxver-1];
		   y=bj_data.vertex.y[bj_data.maxver-1];
		   for(int k=0;k<2;k++)
		   {_itoa(y,bufy,10);
			   bj_data.write_char(bufy,strlen(bufy));
		   bj_data.write_char(spc,1);
		   _itoa(x,bufx,10);
		   bj_data.write_char(bufx,strlen(bufx));
		   bj_data.write_char(ret,strlen(ret));}		   
		}
		bj_data.write_char(sharp,strlen(sharp));
		bj_data.write_char(ret,strlen(ret));

		bj_data.readline();//这一层最后一个点是无效点要去掉
		delete[] bj_data.vertex.x;
		delete[] bj_data.vertex.y;
	}
    bj_data.write_char(star,strlen(star));//以"**"标志文件结束
	bj_data.bj_data_file.Close();
	TRACE("%s","close ok");
}

void CFaultageProcess::ReadFaultageTempFile(char *FileName)//填色
{

	char* ret="\r\n";
	char *sharp="##";
	char *zero="-1";
	char *star="**";

	CBJDATA	faultage_data;
	
	CString filename=GetTempFilePath(FileName);
	int length=filename.GetLength();
	length++;
	LPTSTR p = filename.GetBuffer(length);

	char file[256];
	strcpy(file, ::globalDatPath);
	if (*p != '\\')
		strcat(file, "\\"); //modify xxzhu 20170317
	strcat(file, p);
	filename.ReleaseBuffer();
	faultage_data.readfile(file);

	faultage_data.readword();
	if(!isdigit(faultage_data.m_pBufferWord[0]))
	{
		AfxMessageBox("断层临时文件出错!");
		return;
	}

	int xpos,ypos;
	int i=0;

	int success;

    Clear(m_DouHeadArray,Max);
	while(*faultage_data.m_pBufferWord!=*star)
	{
	   VERTEXPOS *pHeadPos1=NULL;
	   VERTEXPOS *pHeadPos2=NULL;
	   VERTEXPOS *pV1=NULL;
	   VERTEXPOS *pV2=NULL;
	   VERTEXPOS* pVertexPos=NULL;
        int j=0;
	   do
	   {
		if(*faultage_data.m_pBufferWord==*sharp)
			break;
		else
		{
         if(*faultage_data.m_pBufferWord!=*zero)
		  {
			  
			pVertexPos=new VERTEXPOS[1];//
		    success=sscanf(faultage_data.m_pBufferWord,"%d",&ypos);
			ASSERT(success==1);
			faultage_data.readword();
			success=sscanf(faultage_data.m_pBufferWord,"%d",&xpos);
			ASSERT(success==1);

			pVertexPos->vpos=ypos*m_col+xpos;
			pVertexPos->next=NULL;
			if(j%2==0)//如果j是偶数，建立链表1
			{
			  if(!pHeadPos1)
			  {pHeadPos1=pVertexPos;pV1=pHeadPos1;}
			  else
			  {
				pV1->next=pVertexPos;
				pV1=pV1->next;
			  }	
			}
			else//如果j是奇数，建立链表2
			{
			  if(!pHeadPos2)
			  {pHeadPos2=pVertexPos;pV2=pHeadPos2;}
			  else
			  {
				pV2->next=pVertexPos;
				pV2=pV2->next;
			  }	

			}
//			delete pVertexPos;
		  }
		  else
		  { pHeadPos1=NULL;pHeadPos2=NULL;}
		}
		faultage_data.readword();
		j++;
	   }
	while(*faultage_data.m_pBufferWord!=*sharp);
/*
	while(*faultage_data.m_pBufferWord!=*star)
	{
	   VERTEXPOS *pHeadPos=NULL;
	   VERTEXPOS *pV=NULL;
	   do
	   {
		if(*faultage_data.m_pBufferWord==*sharp)
			break;
		else
		{
         if(*faultage_data.m_pBufferWord!=*zero)
		  {
			  
			VERTEXPOS* pVertexPos=new VERTEXPOS;
		    success=sscanf(faultage_data.m_pBufferWord,"%d",&ypos);
			ASSERT(success==1);
			faultage_data.readword();
			success=sscanf(faultage_data.m_pBufferWord,"%d",&xpos);
			ASSERT(success==1);

			pVertexPos->vpos=ypos*m_col+xpos;
			pVertexPos->next=NULL;

			  if(!pHeadPos)
			  {pHeadPos=pVertexPos;pV=pHeadPos;}
			  else
			  {
				pV->next=pVertexPos;
				pV=pV->next;
			  }	
			

		  }
		  else
		  { pHeadPos=NULL;}
		}
		faultage_data.readword();
	   }
	while(*faultage_data.m_pBufferWord!=*sharp);
	*/

	m_DouHeadArray[i].pHead1=pHeadPos1;
	m_DouHeadArray[i].pHead2=pHeadPos2;
	i++;
	faultage_data.readword();
//	delete[] pVertexPos;
	}
	m_faultagenum=i;
}

void CFaultageProcess::ReadFaultageTempFile1(CString FileName,XYZ dm_XYZ,int turn)//勾勒线框
{

	char* ret="\r\n";
	char *sharp="##";
	char *zero="-1";
	char *star="**";

	CBJDATA	faultage_data;
	
	//CString filename=GetTempFilePath(FileName);
	CString filename=FileName;
	int length=filename.GetLength();
	length++;
	LPTSTR p = filename.GetBuffer(length);
	char file[256];
	strcpy(file, globalDatPath);

	if (*p != '\\')
		strcat(file, "\\"); //modify xxzhu 20170317
	strcat(file, p);

	filename.ReleaseBuffer();

	faultage_data.readfile(file);

	faultage_data.readword();
	if(!isdigit(faultage_data.m_pBufferWord[0]))
	{
		AfxMessageBox("断层临时文件出错!");
		return;
	}

	int xpos,ypos;
	int i=0;

	int success;
    Clear(m_HeadArray,Max);
	while(*faultage_data.m_pBufferWord!=*star)
	{
	   VERTEXPOS *pHeadPos1=NULL;
	   VERTEXPOS *pHeadPos2=NULL;
	   VERTEXPOS *pV1=NULL;
	   VERTEXPOS *pV2=NULL;
//	   VERTEXPOS* pVertexPos=NULL;
	   

        int j=0;
	   do
	   {
		if(*faultage_data.m_pBufferWord==*sharp)
			break;
		else
		{
         if(*faultage_data.m_pBufferWord!=*zero)
		  {
			  
		    VERTEXPOS* pVertexPos=new VERTEXPOS[1];
		    success=sscanf(faultage_data.m_pBufferWord,"%d",&ypos);
			ASSERT(success==1);
			faultage_data.readword();
			success=sscanf(faultage_data.m_pBufferWord,"%d",&xpos);
			ASSERT(success==1);

			pVertexPos->vpos=ypos*m_col+xpos;
			pVertexPos->next=NULL;
			if (turn==1)
			{
				//if (dm_XYZ.flag[pVertexPos->vpos]!=30 && dm_XYZ.flag[pVertexPos->vpos]!=40)	
					dm_XYZ.flag[pVertexPos->vpos]=3;
			}
			else
			{
				//if (dm_XYZ.flag[pVertexPos->vpos]!=30 && dm_XYZ.flag[pVertexPos->vpos]!=40)
					dm_XYZ.flag[pVertexPos->vpos]=4;
			}

			if(j%2==0)//如果j是偶数，建立链表1
			{
			  if(!pHeadPos1)
			  {pHeadPos1=pVertexPos;pV1=pHeadPos1;}
			  else
			  {
				pV1->next=pVertexPos;
				pV1=pV1->next;
			  }	
			}
			else//如果j是奇数，建立链表2
			{
			  if(!pHeadPos2)
			  {pHeadPos2=pVertexPos;pV2=pHeadPos2;}
			  else
			  {
				pV2->next=pVertexPos;
				pV2=pV2->next;
			  }	

			}
		  }
		  else
		  { pHeadPos1=NULL;pHeadPos2=NULL;}
		}
		faultage_data.readword();
		j++;
	   }
	while(*faultage_data.m_pBufferWord!=*sharp);
	pV1=pHeadPos1;
	pV2=pHeadPos2;
	int colx1,colx2;
	int coly;
	while (pV1!=NULL)
	{
		coly=pV1->vpos/m_col;
		colx1=pV1->vpos%m_col;
		colx2=pV2->vpos%m_col;
		for(int count=colx1+1;count<colx2;count++)
		{
			if (turn==1)
			dm_XYZ.flag[coly*m_col+count]=30;
			else
			dm_XYZ.flag[coly*m_col+count]=40;
		}
		pV1=pV1->next;
		pV2=pV2->next;
	}

	MakeVertexConsistent(pHeadPos1,pHeadPos2,dm_XYZ,turn);
/*	FILE *fp;
	fp=fopen("d:\\tt.txt","w");
	VERTEXPOS *pVt=pHeadPos1;
	
	while(pVt!=NULL)
	{
		int xx,yy;
		xx=pVt->vpos%m_col;
		yy=pVt->vpos/m_col;
		fprintf(fp,"(%d,%d)\n",xx,yy);
		pVt=pVt->next;
		
	}
	fprintf(fp,"%c\n",'*');
	pVt=pHeadPos2;
	
	while(pVt!=NULL)
	{
		int xx,yy;
		xx=pVt->vpos%m_col;
		yy=pVt->vpos/m_col;
		fprintf(fp,"(%d,%d)\n",xx,yy);
		pVt=pVt->next;
		
	}
	fclose(fp);*/
	
   // pHeadPos1=Commerge(pHeadPos1,pHeadPos2);
	m_HeadArray[i]=pHeadPos1;
	i++;
	faultage_data.readword();
//	delete[] pVertexPos;
	}
	m_faultagenum=i;

}

VERTEXPOS* CFaultageProcess::Commerge(VERTEXPOS* pHead1,VERTEXPOS* pHead2)
{
/*	if(pHead1&&pHead2)
	{//比较两链表的头结点
	   int margin=abs(pHead1->vpos - pHead2->vpos);
       if(margin>1)//若两网格点不连续，则插入连续点使之连续
	   {
		   if(pHead1->vpos > pHead2->vpos)
		   {
              pHead1=Insert(-(margin-1),pHead1,pHead1);//在pHead1前插入margin-1个连续值减少的结点
		   }
		   else
		      pHead2=Insert(-(margin-1),pHead2,pHead2);
	   }
	   //比较两链表的尾结点
       VERTEXPOS* pTail1=NULL;
	   VERTEXPOS* pTail2=NULL;
	   for(VERTEXPOS* pV=pHead1;pV;pV=pV->next)
	   {if(pV->next==NULL) {pTail1=pV;break;}}
	   for(pV=pHead2;pV;pV=pV->next)
	   {if(pV->next==NULL) {pTail2=pV;break;}}	
       margin=abs(pTail1->vpos - pTail2->vpos);
	   if(margin>1)
	   {
		   if(pTail1->vpos > pTail2->vpos)
			   pTail2=Insert(pTail2,margin-1);//在pTail2后插入margin-1个连续增大的结点
		   else
			   pTail1=Insert(pTail1,margin-1);
	   }
      MakeVertexConsistent(pHead1,pHead2);
	   ////head1的尾部与head2的尾部连接，构成一条链表
	   VERTEXPOS* pT=NULL;
      do
	  {
		for(pV=pHead2;pV;pV=pV->next)
	   {
		   if(pHead2->next==NULL){pT=pHead2;break;}
		   if(pV->next->next==NULL){pT=pV->next;pV->next=NULL;break;}
	   }
       pTail1->next=pT;
	   pTail1=pTail1->next;
	  }
      while(pT!=pHead2);

	  return pHead1;
	}
    else*/
	  return NULL;
}
void CFaultageProcess::MakeVertexConsistent(VERTEXPOS* pHead1,VERTEXPOS *pHead2,XYZ dm_XYZ,int turn)
{
	   //分别比较两链表前后相邻两结点在网格中的位置是否连续，不连续则插入点
       //循环2次，分别比较链表1和链表2相邻两点（相邻行的两点）
	   VERTEXPOS *pT=NULL;
	   VERTEXPOS *pHead=NULL;
      for(int i=0;i<2;i++)
	  {
        if(i==0) pHead=pHead1;
		if(i==1) pHead=pHead2;
		
		for(VERTEXPOS *pV=pHead;pV->next;pV=pV->next)
	   {
		   pT=pV->next;
           if (pV->vpos/m_col==pT->vpos/m_col)
		    continue;
		  //因为插入的点同时进行运算，所以要跳过处在同一行的点
		   
		   int margin=(pT->vpos%m_col)-(pV->vpos%m_col);
		  
		   if(abs(margin)>1)//增加 margin-1 个点
		   {
			   int n=abs(margin)-1;
              if(margin>0)
			  {
				  
				  int ml=(int)(n/2);
				  int mr=n-ml;
				  
                  Insert(pV,ml,dm_XYZ,turn);//X小的点pV后增加ml个连续值增大的点
				  Insert(-mr,pT,pHead,dm_XYZ,turn);//x大的点pT前增加mr个连续值减少的点				  
				  
			  }
			  else//if margin<0
			  {
				  int mr=(int)(n/2);
				  int ml=n-mr;
				  Insert(ml,pT,pHead,dm_XYZ,turn);//X小的点pT前增加ml个连续值增大的点
				  Insert(pV,-mr,dm_XYZ,turn);//x大的点pV后增加mr个连续值减少的点				  
				 
			  }
		   }
	   }
	  }
}
VERTEXPOS* CFaultageProcess::Insert(int n,VERTEXPOS* pS,VERTEXPOS* pHead, XYZ dm_XYZ,int turn)//在pS前插入n个连续值结点
{
     
   //插入的点处设置一下
   VERTEXPOS* pV=NULL;
   for(int i=0;i<abs(n);i++)
   {
	   pV=new VERTEXPOS[1];
	   if(n>0)
	   {
           pV->vpos=pS->vpos+1;
	  
	   }
	   if(n<0)
	   {
		   pV->vpos=pS->vpos-1;
	   }
	   if (turn==1)	
	   {
			//if (dm_XYZ.flag[pV->vpos]!=30 && dm_XYZ.flag[pV->vpos]!=40)		
				dm_XYZ.flag[pV->vpos]=3;  //插入的点处设置一下
	   }
	   else//if turn=2
	   {
			//if (dm_XYZ.flag[pV->vpos]!=30 && dm_XYZ.flag[pV->vpos]!=40)	
				dm_XYZ.flag[pV->vpos]=4;
	   }
	   
	   if(pS==pHead)
	   {pHead=pV;pHead->next=pS;pS=pHead;}
	   else
	   {  
		   VERTEXPOS *pT;
		   for(pT=pHead;pT;pT=pT->next)
		   {
			   if(pT->next->vpos==pS->vpos) break;
		   }
		   if(!pT)
		   {
			   AfxMessageBox("Faultage link inserting node error!");
			   return NULL;
		   }
		   pT->next=pV;
		   pV->next=pS;
	       pS=pV;
	   }
   }
//	   delete[] pV;
   return pHead;
}

VERTEXPOS* CFaultageProcess::Insert(VERTEXPOS* pS,int n,XYZ dm_XYZ,int turn)//在pS后插入n个连续值结点
{
	  
	//   VERTEXPOS* pV=NULL;	 
   for(int i=0;i<abs(n);i++)
   {
	   VERTEXPOS* pV=new VERTEXPOS[1];
	   if(n>0)
       {
		   pV->vpos=pS->vpos+1;
	   }
	   if(n<0)
	   {
           pV->vpos=pS->vpos-1;
	   }
       if (turn==1)	
	   {
		  // if (dm_XYZ.flag[pV->vpos]!=30 && dm_XYZ.flag[pV->vpos]!=40)
				dm_XYZ.flag[pV->vpos]=3;  //插入的点处设置一下
	   }
	   else//if turn=2
	   {
		   //if (dm_XYZ.flag[pV->vpos]!=30 && dm_XYZ.flag[pV->vpos]!=40)
				dm_XYZ.flag[pV->vpos]=4;
	   }
	   pV->next=NULL;
	   if(pS->next==NULL)//if pS is the tail node
	   {
		   pS->next=pV;
	   }
	   else
	   {
           pV->next=pS->next;
		   pS->next=pV;
	   }
	   pS=pS->next;
   }
//    delete[] pV;
   return pS;
}
CString CFaultageProcess::GetTempFilePath(char *OldFileName)//OldFileName是原始数据文件路径
{
	//bj_data_file
	CFileException ex;
	char *newfilename;
///////////////////////////
	CString str1,str2,str3,str4,str5,str;
	str1 = OldFileName;   //str1 = c:\bj.wal
	int File_Length,Point_Length,Ex_Length;

	//////////
	int path_length = str1.Find("Data");
	if(path_length == -1 )
		path_length = str1.Find("DATA");

	CString path_faultage;
	path_faultage = str1.Left(path_length);
	path_faultage = path_faultage+"Temp\\";
	//////////

	File_Length = str1.ReverseFind('\\');
	int i = str1.GetLength()-File_Length-1;
	str2 = str1.Right(i);  //str2 = bj.wal
	Point_Length = str2.Find('.');

	Ex_Length = str2.ReverseFind('.');  
	int j = str2.GetLength() - Ex_Length-1;

	str4 = str2.Right(j);   //str4 = wal
	str3 = str2.Left(Point_Length);  //for example th1dcb.wal then str3 = th1dcb
	str3 = "gdc_" + str3 + "." + "wal";
   // int path_length = str1.GetLength() - File_Length; 
	str5 = str1.Left(File_Length);  //str5 = c:\//
	str5=str5.Left(str5.ReverseFind('\\')+1);
	int k;
	str = path_faultage+str3;
	str=str5+str;
	k = str.GetLength();
	newfilename =  new char[k+1];
//	newfilename = str;  LPTSTR p
	newfilename = str.GetBuffer( k+1 );
	*(newfilename+k) = '\0';
	str.ReleaseBuffer( );


	return (CString)newfilename;
}