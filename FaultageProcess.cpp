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

		
		//���ϲ����ݣ�����յĻ��߲���յģ�
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
	int duanceng_total;  //���жϲ���
	int point_count;    //ÿһ��ĵ���
	bj_data.readline();//����Ч����wmp123  xxzhu 20170313ע�ͣ���������,��Ϊʹ������Чָ��
	bj_data.readline();//���ϲ���
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

		bj_data.readline();//����Ч����
		bj_data.readline();//���ò��ж��ٸ���
		if (isdigit(bj_data.m_pBufferLine[0]))		//��ʱ���ݲ���12�����û������������ü����س��������
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
				bj_data.vertex.x[j]=(int)((m_col-1)*(tempf-m_minx)/(m_maxx-m_minx)+0.5f);//�����ϵ�x

				bj_data.readword();
				success=sscanf(bj_data.m_pBufferWord,"%f",&tempf);
				ASSERT(success==1);
				bj_data.vertex.y[j]=(int)((m_row-1)*(tempf-m_miny)/(m_maxy-m_miny)+0.5f);//�����ϵ�y 

				//�����x[]��y[]����Ӧ�ó��ָ��� xxzhu 20170326
				if (bj_data.vertex.x[j] < 0 || bj_data.vertex.y[j] < 0)
				{
					throw i; //���ߺ���Ϊ�쳣�׻�ȥ
				}
			}
		
		//ָ�����飬ÿ��Ԫ����ָ��struct edge_info�Ľṹ
			
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
			   if((bj_data.vertex.x[i]==bj_data.vertex.x[i+1])//������ֱ��
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
			   if((bj_data.vertex.y[i]==bj_data.vertex.y[i+1])//�����ֱ��
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
			   if((bj_data.vertex.y[i]!=bj_data.vertex.y[i+1])//����бֱ��
                &&(bj_data.vertex.x[i]!=bj_data.vertex.x[i+1]))
               {
				   int margin=bj_data.vertex.y[i]-bj_data.vertex.y[i+1];
				   if(abs(margin)>=1)//�������д���
				   {
					   int yl;
					   int j=1;
					   int k;
					   k=(bj_data.vertex.y[i+1]-bj_data.vertex.y[i])/(bj_data.vertex.x[i+1]-bj_data.vertex.x[i]);
					   while(j<=abs(margin))
					   {
						   if(margin>0) yl=bj_data.vertex.y[i]-j;
						   else yl=bj_data.vertex.y[i]+j;
						   ///��б����ɨ����yl�Ľ��㣬�ٽ��Ƶ��������
                           x=(int)((bj_data.vertex.x[i]*(yl-bj_data.vertex.y[i+1])-bj_data.vertex.x[i+1]*(yl-bj_data.vertex.y[i]))/(margin)+0.5f);
				           y=yl+1;
						  /*if (turn==1)		//�����Ƿ�նϲ�
								if (dm_XYZ.flag[y*m_col+x]!=30 || dm_XYZ.flag[y*m_col+x]!=40)
								dm_XYZ.flag[y*m_col+x]=3;
							else			//�����ǿ���ʽ�ϲ�
								if (dm_XYZ.flag[y*m_col+x]!=30 || dm_XYZ.flag[y*m_col+x]!=40)
								dm_XYZ.flag[y*m_col+x]=4;
							if (k>0)  //��
							{
								if (margin>0) //����
								{
									//���������Խǵ�	
									if (turn==1)		//�����Ƿ�նϲ�
										dm_XYZ.flag[y*m_col+x-1]=30;
									else			//�����ǿ���ʽ�ϲ�
										dm_XYZ.flag[y*m_col+x-1]=40;
									if (turn==1)		//�����Ƿ�նϲ�
										dm_XYZ.flag[(y+1)*m_col+x]=30;
									else			//�����ǿ���ʽ�ϲ�
										dm_XYZ.flag[(y+1)*m_col+x]=40;	
								}
								else	//����
								{
									if (turn==1)		//�����Ƿ�նϲ�
									dm_XYZ.flag[yl*m_col+x-1]=30;
									else			//�����ǿ���ʽ�ϲ�
									dm_XYZ.flag[yl*m_col+x-1]=40;
									if (turn==1)		//�����Ƿ�նϲ�
									dm_XYZ.flag[(yl-1)*m_col+x]=30;
									else			//�����ǿ���ʽ�ϲ�
									dm_XYZ.flag[(yl-1)*m_col+x]=40;	
								}


							}
							else  //��
							{
								if (margin>0) //����
								{
									if (turn==1)		//�����Ƿ�նϲ�
									dm_XYZ.flag[yl*m_col+x+1]=30;
									else			//�����ǿ���ʽ�ϲ�
									dm_XYZ.flag[yl*m_col+x+1]=40;
									if (turn==1)		//�����Ƿ�նϲ�
									dm_XYZ.flag[(yl+1)*m_col+x]=30;
									else			//�����ǿ���ʽ�ϲ�
									dm_XYZ.flag[(yl+1)*m_col+x]=40;	
								}
								else	//����
								{
									if (turn==1)		//�����Ƿ�նϲ�
									dm_XYZ.flag[yl*m_col+x+1]=30;
									else			//�����ǿ���ʽ�ϲ�
									dm_XYZ.flag[yl*m_col+x+1]=40;
									if (turn==1)		//�����Ƿ�նϲ�
									dm_XYZ.flag[(yl-1)*m_col+x]=30;
									else			//�����ǿ���ʽ�ϲ�
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
		   ///д���һ����
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

		bj_data.readline();//��һ�����һ��������Ч��Ҫȥ��
		delete[] bj_data.vertex.x;
		delete[] bj_data.vertex.y;
	}
    bj_data.write_char(star,strlen(star));//��"**"��־�ļ�����
	bj_data.bj_data_file.Close();
	TRACE("%s","close ok");
}

void CFaultageProcess::ReadFaultageTempFile(char *FileName)//��ɫ
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
		AfxMessageBox("�ϲ���ʱ�ļ�����!");
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
			if(j%2==0)//���j��ż������������1
			{
			  if(!pHeadPos1)
			  {pHeadPos1=pVertexPos;pV1=pHeadPos1;}
			  else
			  {
				pV1->next=pVertexPos;
				pV1=pV1->next;
			  }	
			}
			else//���j����������������2
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

void CFaultageProcess::ReadFaultageTempFile1(CString FileName,XYZ dm_XYZ,int turn)//�����߿�
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
		AfxMessageBox("�ϲ���ʱ�ļ�����!");
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

			if(j%2==0)//���j��ż������������1
			{
			  if(!pHeadPos1)
			  {pHeadPos1=pVertexPos;pV1=pHeadPos1;}
			  else
			  {
				pV1->next=pVertexPos;
				pV1=pV1->next;
			  }	
			}
			else//���j����������������2
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
	{//�Ƚ��������ͷ���
	   int margin=abs(pHead1->vpos - pHead2->vpos);
       if(margin>1)//��������㲻�����������������ʹ֮����
	   {
		   if(pHead1->vpos > pHead2->vpos)
		   {
              pHead1=Insert(-(margin-1),pHead1,pHead1);//��pHead1ǰ����margin-1������ֵ���ٵĽ��
		   }
		   else
		      pHead2=Insert(-(margin-1),pHead2,pHead2);
	   }
	   //�Ƚ��������β���
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
			   pTail2=Insert(pTail2,margin-1);//��pTail2�����margin-1����������Ľ��
		   else
			   pTail1=Insert(pTail1,margin-1);
	   }
      MakeVertexConsistent(pHead1,pHead2);
	   ////head1��β����head2��β�����ӣ�����һ������
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
	   //�ֱ�Ƚ�������ǰ������������������е�λ���Ƿ�������������������
       //ѭ��2�Σ��ֱ�Ƚ�����1������2�������㣨�����е����㣩
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
		  //��Ϊ����ĵ�ͬʱ�������㣬����Ҫ��������ͬһ�еĵ�
		   
		   int margin=(pT->vpos%m_col)-(pV->vpos%m_col);
		  
		   if(abs(margin)>1)//���� margin-1 ����
		   {
			   int n=abs(margin)-1;
              if(margin>0)
			  {
				  
				  int ml=(int)(n/2);
				  int mr=n-ml;
				  
                  Insert(pV,ml,dm_XYZ,turn);//XС�ĵ�pV������ml������ֵ����ĵ�
				  Insert(-mr,pT,pHead,dm_XYZ,turn);//x��ĵ�pTǰ����mr������ֵ���ٵĵ�				  
				  
			  }
			  else//if margin<0
			  {
				  int mr=(int)(n/2);
				  int ml=n-mr;
				  Insert(ml,pT,pHead,dm_XYZ,turn);//XС�ĵ�pTǰ����ml������ֵ����ĵ�
				  Insert(pV,-mr,dm_XYZ,turn);//x��ĵ�pV������mr������ֵ���ٵĵ�				  
				 
			  }
		   }
	   }
	  }
}
VERTEXPOS* CFaultageProcess::Insert(int n,VERTEXPOS* pS,VERTEXPOS* pHead, XYZ dm_XYZ,int turn)//��pSǰ����n������ֵ���
{
     
   //����ĵ㴦����һ��
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
				dm_XYZ.flag[pV->vpos]=3;  //����ĵ㴦����һ��
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

VERTEXPOS* CFaultageProcess::Insert(VERTEXPOS* pS,int n,XYZ dm_XYZ,int turn)//��pS�����n������ֵ���
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
				dm_XYZ.flag[pV->vpos]=3;  //����ĵ㴦����һ��
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
CString CFaultageProcess::GetTempFilePath(char *OldFileName)//OldFileName��ԭʼ�����ļ�·��
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