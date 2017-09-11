// BJDATA.cpp: implementation of the CBJDATA class.
//copyright belongs to Yu chunjiang
//chjyu@zhhz.suda.edu.cn
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BJDATA.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBJDATA::CBJDATA()
{
	m_pBuffer = NULL;
	m_pBufferWord[0] = '\0';
	m_pBufferLine[0] = '\0';
	m_IndexBuffer = 0;

	vertex.x=NULL;
	vertex.y=NULL;
	init_egtab(egtab,maxheight);
	line_count=0;
	//ָ�����飬ÿ��Ԫ����ָ��struct edge_info�Ľṹ

	m_pBuffer = nullptr; //add by xxzhu 
}

CBJDATA::~CBJDATA()
{
	free();
}

void CBJDATA::free()
{
	if(m_pBuffer != NULL)
		delete [] m_pBuffer;
	m_IndexBuffer = 0;
}

int CBJDATA::readfile(char *filename)
{
	m_FileName = filename;

	CFile file;
	CFileException ex;

	// Opening
	TRACE("  opening...");
	if(!file.Open(filename, CFile::modeRead | CFile::typeBinary,&ex))
	{
		//MessageBox(NULL,NULL, filename, MB_OK);
		#ifdef _DEBUG
		  afxDump << "File could not be opened " << ex.m_cause << "\n";
		#endif
		TRACE("unable to open file for reading\n");
		return 0;
	}
	TRACE("ok\n");

	// Size file
	m_SizeFile = file.GetLength();
	TRACE("  length : %d bytes\n",m_SizeFile);

	// Alloc
	TRACE("  alloc...");
	if (m_pBuffer) //��ֹ�ظ�����
		delete[]m_pBuffer;
	m_pBuffer = new char[m_SizeFile];
	if(m_pBuffer == NULL)
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

int CBJDATA::readline()
{
	//for(int index=0;index<MAX_LINE;index++)
	//	m_pBufferLine[index] = '\0'; //����л���
	memset(m_pBufferLine, 0, sizeof(char)*MAX_LINE);//��������ڴ�

	int i=0;
	do                       //m_pBufferΪ��ָ�룬xxzhu 20170313
		m_pBufferLine[i++] = m_pBuffer[m_IndexBuffer++];
	while(m_pBuffer[m_IndexBuffer-1] != '\n' && 
		    i < MAX_LINE &&
				m_IndexBuffer < m_SizeFile);

	m_pBufferLine[i-2] = '\0';

	//TRACE("  line : %s\n",m_pBufferLine);

	return 1;
}

int CBJDATA::readword()
{
	//for(int index=0;index<MAX_WORD;index++)
	//	m_pBufferWord[0] = '\0'; //����л���
	memset(m_pBufferWord, 0, sizeof(char)*MAX_WORD);
	
	int i=0;

	// Jump to next valid character
	while((m_pBuffer[m_IndexBuffer] == '\n' || 
		     m_pBuffer[m_IndexBuffer] == '\t' || 
		     m_pBuffer[m_IndexBuffer] == '\r' || 
		     m_pBuffer[m_IndexBuffer] == ' '  ||
			 m_pBuffer[m_IndexBuffer] == ',') &&
				 m_IndexBuffer < m_SizeFile)
		m_IndexBuffer++;

	// Check eof
	if(m_IndexBuffer >= m_SizeFile)
		return 0;

	do
		m_pBufferWord[i++] = m_pBuffer[m_IndexBuffer++];
	while(m_pBuffer[m_IndexBuffer-1] != '\n' && 
		    m_pBuffer[m_IndexBuffer-1] != '\t' && 
		    m_pBuffer[m_IndexBuffer-1] != '\r' && 
		    m_pBuffer[m_IndexBuffer-1] != ' ' && 
			m_pBuffer[m_IndexBuffer] !=','  &&
		    i < MAX_WORD &&
				m_IndexBuffer < m_SizeFile);

	m_pBufferWord[i] = '\0';

	//TRACE("  word : %s\n",m_pBufferWord);

	// if next char is '\n' then jump it
	while(m_pBuffer[m_IndexBuffer] == '\n' && 
		 	 m_IndexBuffer < m_SizeFile)
		m_IndexBuffer++;


	return 1;

}

void CBJDATA::init_egtab(edge_info *egtab[], int n)
{
	for(int i=0;i<n;i++)
	   {
		egtab[i]=NULL;
	   }
}

float CBJDATA::inverse(points vertex, int i)
{
	float m;
	if(vertex.y[i]==vertex.y[i+1]) //ˮƽ�ĵ�б��Ϊ0
		return 0;
	else
	{
		m=(float)(vertex.x[i]-vertex.x[(i+1)%maxver])/(float)(vertex.y[i]-vertex.y[(i+1)%maxver]);
		return m;
	}
}

void CBJDATA::insert(edge_info *node, edge_info **list)
{
	struct edge_info *trav,*trl;
	if(*list==NULL) *list=node;  //list ��һ��ָ��struct edge_info��ָ��
	    else if(node->xs<=(*list)->xs)  //����listǰ
				{
				   node->next=*list;
				   *list=node;
				}
			 else
			 {
			   trav=*list;
			   while(trav->next!=NULL && trav->xs<node->xs)
					{
					trl=trav;
					trav=trav->next;
					 }
				if(trav->next==NULL && trav->xs<node->xs)
					trav->next=node;//�嵽���
				else
					{
					   node->next=trav; //����trl �� trav֮��
					   trl->next=node;
					}
			 }
}

void CBJDATA::create_edge_table(edge_info *edge[], int n, points vertex)
{
	//��űߵ�����edge������ι��ɵĵ���n������ĵ�vertex
	struct edge_info *e;
	for(int i=0;i<n;i++)
	  {
	    e=new struct edge_info[1];   //����һ��struct edge_info���ڴ�ռ�
	    e->m=inverse(vertex,i);
	    e->next=NULL;
	    if(vertex.y[i] >vertex.y[(i+1)%maxver])
		{
			  e->yt=vertex.y[i];
			  if(vertex.y[(i+1) % maxver] < vertex.y[(i+2) % maxver])
			  {
				e->xs=(float)vertex.x[(i+1) % maxver]; //����
				insert(e,&edge[vertex.y[(i+1) % maxver]]); //�ò��뷨
			  }
		  	  else
			  {
			    e->xs=vertex.x[(i+1) % maxver]+inverse(vertex,i); //ˮƽ���㡢�յ�
				insert(e,&edge[vertex.y[(i+1) % maxver]+1]); //�ò��뷨
			  }
		}
	 else if(vertex.y[i] != vertex.y[(i+1)%maxver])
	   {
	      e->yt=vertex.y[(i+1)%maxver];

			  int index;//��ֹi==-1
			  if(i==0)
				  index=maxver-1;
			  else
				  index=(i-1) % maxver;

			  if(vertex.y[i] < vertex.y[index])
				  {
						e->xs=(float)vertex.x[i];//����
						insert(e,&edge[vertex.y[i]]);
				  }
			  else
				  {
						e->xs=vertex.x[i]+inverse(vertex,i); //ˮƽ���㡢�յ�
						insert(e,&edge[vertex.y[i]+1]); //�ò��뷨
			  }
	    }
	  }
}

void CBJDATA::fill_prog(int i)
{
struct edge_info* scan_line;
struct edge_info *trav;
char bufx[6]={0,0,0,0,0,0};
char bufy[6]={0,0,0,0,0,0};
char ret[]="\r\n";
char spc[]=" ";

int k;
  k=0;
  if(egtab[i]!=NULL)
      scan_line=egtab[i];
  else
	  return;

  if(scan_line!=NULL)
   {
     trav=scan_line;
     while(trav != NULL)
	{
		 line_count++;
	       k=k+1;
	   if(k%2==1)
		   {

//			  moveto((int)trav->xs,i); //�Ƶ�(trav.xs,i)��
			  _itoa(i+1,bufy,10);
			  bj_data_file.Write(bufy,strlen(bufy));

			  bj_data_file.Write(spc,1);

			  _itoa((int)trav->xs,bufx,10);
			  bj_data_file.Write(bufx,strlen(bufx));
//			  bj_data_file.Write(buffer,sizeof(buffer));
//			  bj_data_file.Write(buffers,sizeof(buffers));
			  bj_data_file.Write(ret,strlen(ret));
			  TRACE("moveto: x=%f,y=%d\n",trav->xs,i);
			  trav=trav->next;
		   }
	       else
			  {
//				lineto((int)trav->xs,i); //������֮�仭ֱ��
				_itoa(i+1,bufy,10); //�д�1��ʼ
			    bj_data_file.Write(bufy,strlen(bufy));
				
			    bj_data_file.Write(spc,1);

			   _itoa((int)trav->xs,bufx,10);
			    bj_data_file.Write(bufx,strlen(bufx));
//			    bj_data_file.Write(buffer,sizeof(buffer));
//			   	bj_data_file.Write(buffere,sizeof(buffere));
				bj_data_file.Write(ret,strlen(ret));
				TRACE("lineto: x=%f,y=%d\n",trav->xs,i);
				trav=trav->next;
			  }
	}
   }
   
}

void CBJDATA::fill_prog(int i,XYZ fillm_XYZ,int fillm_col,int turn)
{
	struct edge_info* scan_line;
	struct edge_info *trav;
	int beginxx;
	int endxx;


  int k;
  
  if(egtab[i]!=NULL)
      scan_line=egtab[i];
  else
	  return;

  if(scan_line!=NULL)
   {
     trav=scan_line;
	 k=0;
     while(trav != NULL)
	{
	    line_count++;
	    k=k+1;
	    if(k%2==1)//���������ڵ�
		{
			beginxx=(int)trav->xs;	  
		}
	    else//��ż�����ڵ�
		{
			endxx=(int)trav->xs;			
		};
		if (k%2==0)
		{
				int flagtz;
				for(int ccbe=beginxx;ccbe<=endxx;ccbe++)
				{
					flagtz=fillm_XYZ.flag[(i+1)*fillm_col+ccbe];
					if (flagtz!=2)
						if (ccbe==beginxx || ccbe==endxx)
						{	
							
							if (turn==1)
							{
								if (fillm_XYZ.flag[(i+1)*fillm_col+ccbe]!=30 || fillm_XYZ.flag[(i+1)*fillm_col+ccbe]!=40)
								fillm_XYZ.flag[(i+1)*fillm_col+ccbe]=3;
								//fprintf(gp,"%d,%d",beginyy,ccbe);
							}
							else
							{
								if (fillm_XYZ.flag[(i+1)*fillm_col+ccbe]!=30 || fillm_XYZ.flag[(i+1)*fillm_col+ccbe]!=40)
								fillm_XYZ.flag[(i+1)*fillm_col+ccbe]=4;	//3,4�����Ƕϲ�߽磬����3�Ƿ��ʽ�ϲ㡣4�ǿ�����
							}
						}
						else
						{
							if (turn==1)
							{
								
								fillm_XYZ.flag[(i+1)*fillm_col+ccbe]=30;
							}
							else
							{
								fillm_XYZ.flag[(i+1)*fillm_col+ccbe]=40;		//30��40������ڶϲ����ڲ����ݡ�
							}
						}//���ﻻ��flag���
						
				}//end for
		}

		trav=trav->next;
	}//end while
   }//end if
}

void CBJDATA::del_edge(edge_info **egtab, int i)
{
	struct edge_info *trav,*trl;
	struct edge_info *scan_line;
	scan_line=*egtab;
	trav=scan_line;
	while(trav!=NULL)
	{
	 if(trav==scan_line)  //��ͷ�����ͬ
	 {
	   if(trav->yt==i)  //ɾ����һ�����
		{
		  scan_line=scan_line->next;
		  trav=scan_line;
		  trl=trav;
		}
	 else
	  {
		trl=trav;
		trav=trav->next; //��ָ���ƶ�
	  }
	 }
	 else if(trav->yt==i)  //�ڶ�������
		   {
			 trl->next=trav->next;//ɾȥ
			 trav=trl->next;
		   }
		  else
			{
			 trl=trav;
			 trav=trav->next;//��ָ���ƶ�
		   }
	}  //end while

	  *egtab=scan_line;
}

void CBJDATA::recount_xs(int i)
{
  struct edge_info *scan_line;
    struct edge_info *p,*q;

    scan_line=egtab[i];//��ʼֵ
    if(scan_line!=NULL)
	{
		scan_line->xs=scan_line->xs+scan_line->m;//xs ֵ����m
		p=scan_line->next;//pָ��ڶ���ֵ
		scan_line->next=NULL;//xs�޸Ĺ��ĵ�һ����ѹ�������������
		while(p!=NULL)
		 {
		   p->xs=p->xs+p->m;
		   q=p->next;
		   p->next=NULL;//�ڶ�������Ҳ�ѹ�
		   insert(p,&egtab[i]);
		   p=q;
		 }
	}
}

void CBJDATA::imerge(edge_info **upper, edge_info **down, int i)
{
	struct edge_info *upper_line;  //y+1ɨ����
	struct edge_info *down_line;
	struct edge_info *p,*q;

	upper_line=*upper;
	down_line=*down;
	if(upper_line==NULL && down_line!=NULL)
	  {
	   upper_line=down_line;
	   *upper=*down;
	   return;
	  }
	else
	   p=down_line;

	while(p!=NULL)
	{
	  q=p->next;
	  p->next=NULL;
	  insert(p,&upper_line);
	  p=q;
	}

	*upper=upper_line;
}

int CBJDATA::big_than_cur(int i)
{
	struct edge_info *scan_line,*p;
	scan_line=egtab[i];
	if(scan_line!=NULL)
	   p=scan_line;
	else
	   return 2;//��û������

	while(p!=NULL)
	{
	  if(p->yt<i)
		p=p->next;  //�ж�i�Ƿ񶼴���p->yt
	  else
		return 0;  //i�Ƕ�����p->yt
	}

	  return 1; //i������p->yt
}

void CBJDATA::draw(XYZ drm_XYZ,int drm_col,int turn)
{
// if(!writefile("C:\\WINDOWS\\Desktop\\data\\new5.25\\bj\\bjGtb1dcb.txt"))
// {
//	   AfxMessageBox("File could not be opened.");
//	   return;
//}
  
 for(int i=0;i<maxheight;i++)
 {
   if(big_than_cur(i)==1) //��ǰˮƽɨ����i > ��ǰɨ��߱��и��߽���yt������
    return;
   else if(big_than_cur(i)!=2)		
    {
      fill_prog(i,drm_XYZ,drm_col,turn);//���i��
      del_edge(&egtab[i],i);//ɾ��i���е�yt=y�ı߽��
      recount_xs(i);//���������ı߽�㣬����xs=xs+m�ĵ������㣬����xs�����򣨴�С����
      imerge(&egtab[i+1],&egtab[i],i);//y=y+1,egtab[y]!=NULL�ĵ������뵱ǰɨ��߱�scan_line����������ϲ�
    }
 }
//	bj_data_file.Close();
}

void CBJDATA::drawfaultage()
{
// char star[]="#\n";
 for(int i=0;i<maxheight;i++)
 {
   if(big_than_cur(i)==1) //��ǰˮƽɨ����i > ��ǰɨ��߱��и��߽���yt������
    return;
   else if(big_than_cur(i)!=2)
    {

      fill_prog(i);//���i��
      del_edge(&egtab[i],i);//ɾ��i���е�yt=y�ı߽��
      recount_xs(i);//���������ı߽�㣬����xs=xs+m�ĵ������㣬����xs�����򣨴�С����
      imerge(&egtab[i+1],&egtab[i],i);//y=y+1,egtab[y]!=NULL�ĵ������뵱ǰɨ��߱�scan_line����������ϲ�
    }
 }
//    bj_data_file.Write(star,2);
//	bj_data_file.Close();
}

int CBJDATA::writefile(char *filename)
{
	//bj_data_file
	CFileException ex;

	// Opening
	TRACE("  opening...");
	if(!bj_data_file.Open(filename, CFile::modeWrite | CFile::typeBinary|CFile::modeCreate,&ex))
	{
		#ifdef _DEBUG
		  afxDump << "File could not be opened " << ex.m_cause << "\n";
		#endif
		TRACE("unable to open file for reading\n");
		return 0;
	}
	TRACE("ok\n");

	return 1;

}



void CBJDATA::write_char(char *str, int i)
{
	bj_data_file.Write(str,i);
}

void CBJDATA::close_file()
{
	bj_data_file.Close();
}


