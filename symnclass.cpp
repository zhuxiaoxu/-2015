// symnclass.cpp: implementation of the symnclass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "symnclass.h"
#include "BJDATA.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
symnclass::symnclass(void)
{
	
	//��ʼ���ṹ
	/*	symnclasslz.m_XYZ.x=new int [symnclasslz.m_col];
		symnclasslz.m_XYZ.y=new int [symnclasslz.m_row];
		symnclasslz.m_XYZ.z=new float [symnclasslz.m_col*symnclasslz.m_row];
		symnclasslz.m_XYZ.flag=new int [symnclasslz.m_col*symnclasslz.m_row];
		symnclasslz.m_XYZ.yl=new float [symnclasslz.m_col*symnclasslz.m_row];*/
	//��ʼ���ṹ

 

}
symnclass::~symnclass(void)
{
		//�ͷ�


}

void symnclass::findpath(struct DOT dot,FILE *fp)
{
	
	struct DOT dot2;
	struct SybStr *pp;

	pp=compls(dot);//��Χ�����ߵĵ���ɵĶ���
	
	while (pp!=NULL)
	{
			gdcount++;
			dot2.x=pp->x;
			dot2.y=pp->y;
			dot2.z=pp->z;
			dot2.flag=pp->flag;
			dot2.yl=pp->yl;
			dot2.ylpec=pp->ylpec;
			dot2.zeroflag=pp->zeroflag;
			push(dot2);
			sfpop=false;			//�Ƿ�pop�������ƴ�ӡ��·���ġ����� a b c d e �Ѿ���ӡ��������ôa b c d�Ͳ�Ҫ��ӡ��
			if (dot2.flag==0)
			{
				findpath(dot2,fp);
			}
			else
			{
				//��ͷ��β��ӡ��������popһ��
				if (dot2.flag==1 || dot2.flag==3 || dot2.flag==4)
				{
					struct stack *shyhead,*lshyhead;
					shyhead=stackmn;
					lshyhead=stackmn;
					float ylpec;
					
					while (shyhead!=NULL)//����õ���һ�������ߵĵ㣬��ô���shyhead�������������㣬���ֻ��һ���㣬��ô˵��������Ǿֲ���ߵ�
					{
						
						if (shyhead->next!=NULL) 
							lshyhead=shyhead->next;
						else
							lshyhead=shyhead;

					//	yl=m_XYZ.yl[shyhead->dot.y*m_col+shyhead->dot.x];
						ylpec=lshyhead->dot.ylpec;
						if (lshyhead!=shyhead)
						ylpec=lshyhead->dot.ylpec;  //��Ϊ�Ұ�ǰ��һ����������������ڱ����ߵĵ�����
						else
						ylpec=0;
						fprintf(fp,"(%d,%d,%f,%d,%f)",shyhead->dot.x,shyhead->dot.y,shyhead->dot.z,shyhead->dot.flag,ylpec);
						//fprintf(fp,"(%d,%d,%f,%d)",shyhead->dot.x,shyhead->dot.y,yl,shyhead->dot.flag);
						printf("(%d,%d,%f,%d)",shyhead->dot.x,shyhead->dot.y,shyhead->dot.z,shyhead->dot.flag);
						shyhead=shyhead->next;
					}
					fprintf(fp,"%c",'\n');
					printf("%c",'\n');
					pop(stackmn);
					sfpop=true;
				}


			}

			pp=pp->next;
			
	}

	if (pp==NULL && gdcount!=0)	//����Ǿֲ���ߵ�
	{
					if (sfpop==false)  //����ǰ�涼ֻ�н�ջ
					{
						struct stack *qstack,*lqstack;
						qstack=stackmn;
						lqstack=qstack;
						float lshylpec;
						while(qstack->next!=NULL)
						{
							lqstack=qstack->next;
							//lshyl=m_XYZ.yl[qstack->dot.y*m_col+qstack->dot.x];
							lshylpec=lqstack->dot.ylpec;
							fprintf(fp,"(%d,%d,%f,%d,%f)",qstack->dot.x,qstack->dot.y,qstack->dot.z,qstack->dot.flag,lshylpec);
							//fprintf(fp,"(%d,%d,%f,%d)",qstack->dot.x,qstack->dot.y,lshyl,qstack->dot.flag);
							printf("(%d,%d,%f,%d)",qstack->dot.x,qstack->dot.y,qstack->dot.z,qstack->dot.flag);
							qstack=qstack->next;
						}
						DOT ldot;
						ldot=pop(stackmn);
						sfpop=true;
						if (ldot.flag!=4)
						{
							//lshyl=m_XYZ.yl[ldot.y*m_col+ldot.x];
							lshylpec=0;//���һ���������ٷֱ�Ϊ0
							fprintf(fp,"(%d,%d,%f,%d,%f)\n",ldot.x,ldot.y,ldot.z,ldot.flag,lshylpec);
							//fprintf(fp,"(%d,%d,%f,%d)\n",ldot.x,ldot.y,lshyl,ldot.flag);
						}
						else
						{
						//	lshyl=m_XYZ.yl[ldot.y*m_col+ldot.x];
							lshylpec=0;
							fprintf(fp,"(%d,%d,%f,%d,%f)\n",ldot.x,ldot.y,ldot.z,4,lshylpec);
							//fprintf(fp,"(%d,%d,%f,%d)\n",ldot.x,ldot.y,lshyl,ldot.flag);
						}
						printf("(%d,%d,%f,%d,%f)",ldot.x,ldot.y,ldot.z,ldot.flag);
												
					}
					else
					{
						
						pop(stackmn);//˵�����ҲҪpop
					}
	}

					
}

struct SybStr* symnclass::compls(DOT dot)//��һ������Χ�����ߵĵ�
{
		//x��x�����꣬y��y������(x,y��0��ʼ)������Χ�����ߵĵ㣬���ض���

		float lz,lyl;//�ݴ棨x,y��Ӧ��z,yl��ֵ��
		int flag;
		int x=dot.x;
		int y=dot.y;
		float z=dot.z;
		float yl=dot.yl;
		float ylpec=dot.ylpec;
		SybStr *head;
		SybStr *p,*q;
//		SybStr *head30;
//		head30=NULL;
//		SybStr *p30,*q30;
//		q30=head30;
		
		float sum;//�������������ķ��䡣sum�Ǹ߶Ⱥͣ�ÿ���߶ȵı���������������
		head=NULL;
		q=head;
		int lcount=0;
		//��Χ�ĵ�
		if (x-1>=0)//���
		{

				if (y+1<m_row)//���½�
				{
					lz=m_XYZ.z[(y+1)*m_col+(x-1)];
					lyl=m_XYZ.yl[(y+1)*m_col+(x-1)];
					flag=m_XYZ.flag[(y+1)*m_col+(x-1)];
					if ((m_XYZ.flag[(y+1)*m_col+x]!=3 && m_XYZ.flag[y*m_col+(x-1)]!=3) && (m_XYZ.flag[(y+1)*m_col+x]!=4 && m_XYZ.flag[y*m_col+(x-1)]!=4))
					{
						if (lz>z && flag!=2 && flag!=30 && flag!=40)//�������͵㣨1��������Ч��0��(9)��Ϊ����·��ʱ������������ĵ�
						{
							p=new SybStr;
							p->x=x-1;
							p->y=y+1;
							p->z=lz;
							p->yl=lyl;
							p->ylpec=m_XYZ.ylpec[(y+1)*m_col+(x-1)];
							p->flag=flag;
							p->zeroflag=m_XYZ.zeroflag[(y+1)*m_col+(x-1)];
							p->next=NULL;
							lcount++;
							if (q==NULL)
							{
								head=p;
								q=head;
							}
							else
							{
								q->next=p;
								q=p;
							}
						}
					}
				}//���½�
				//�������
					lz=m_XYZ.z[y*m_col+(x-1)];
					lyl=m_XYZ.yl[y*m_col+(x-1)];
					flag=m_XYZ.flag[y*m_col+(x-1)];
					if ((m_XYZ.flag[(y-1)*m_col+x-1]!=3 && m_XYZ.flag[(y+1)*m_col+(x-1)]!=3) && (m_XYZ.flag[(y-1)*m_col+x-1]!=4 && m_XYZ.flag[(y+1)*m_col+(x-1)]!=4))
					{
						if (lz>z && flag!=2 && flag!=30 && flag!=40)
						{
							p=new SybStr;
							p->x=x-1;
							p->y=y;
							p->z=lz;
							p->yl=lyl;
							p->ylpec=m_XYZ.ylpec[y*m_col+(x-1)];
							p->zeroflag=m_XYZ.zeroflag[y*m_col+(x-1)];
							p->flag=flag;
							p->next=NULL;
						
							lcount++;
							if (q==NULL)
							{
								head=p;
								q=head;
							}
							else
							{
								q->next=p;
								q=p;
							}
						}//��߽���
					}
				if (y-1>=0)//���Ͻ�
				{
					lz=m_XYZ.z[(y-1)*m_col+(x-1)];
					lyl=m_XYZ.yl[(y-1)*m_col+(x-1)];
					flag=m_XYZ.flag[(y-1)*m_col+(x-1)];
					if ((m_XYZ.flag[y*m_col+x-1]!=3 && m_XYZ.flag[(y-1)*m_col+x]!=3) && (m_XYZ.flag[y*m_col+(x-1)]!=4 && m_XYZ.flag[(y-1)*m_col+x]!=4))
					{
						if (lz>z && flag!=2 && flag!=30 && flag!=40)
						{
							p=new SybStr;
							p->x=x-1;
							p->y=y-1;
							p->z=lz;
							p->yl=lyl;
							p->ylpec=m_XYZ.ylpec[(y-1)*m_col+(x-1)];
							p->zeroflag=m_XYZ.zeroflag[(y-1)*m_col+(x-1)];
							p->flag=flag;
							p->next=NULL;
						
							lcount++;
							if (q==NULL)
							{
								head=p;
								q=head;
							}
							else
							{
								q->next=p;
								q=p;
							}
						}
					}	
				}//���Ͻ�

		}//��߽���
		if (y-1>=0)//�ϱ�
		{
			
						//��������
			lz=m_XYZ.z[(y-1)*m_col+x];
			lyl=m_XYZ.yl[(y-1)*m_col+x];
			flag=m_XYZ.flag[(y-1)*m_col+x];
			if ((m_XYZ.flag[(y-1)*m_col+x-1]!=3 && m_XYZ.flag[(y-1)*m_col+(x+1)]!=3) && (m_XYZ.flag[(y-1)*m_col+x-1]!=4 && m_XYZ.flag[(y-1)*m_col+(x+1)]!=4))
			{
				if (lz>z && flag!=2 && flag!=30 && flag!=40 )
				{
					p=new SybStr;
					p->x=x;
					p->y=y-1;
					p->z=lz;
					p->yl=lyl;
					p->ylpec=m_XYZ.ylpec[(y-1)*m_col+x];
					p->zeroflag=m_XYZ.zeroflag[(y-1)*m_col+x];
					p->flag=flag;
					p->next=NULL;
					
					lcount++;
					if (q==NULL)
					{
						head=p;
						q=head;
					}
					else
					{
						q->next=p;
						q=p;
					}
				}//����
			}
			if (x+1<m_col)//���Ͻ�
			{
					lz=m_XYZ.z[(y-1)*m_col+(x+1)];
					lyl=m_XYZ.yl[(y-1)*m_col+(x+1)];
					flag=m_XYZ.flag[(y-1)*m_col+(x+1)];
					if ((m_XYZ.flag[(y-1)*m_col+x]!=3 && m_XYZ.flag[y*m_col+(x+1)]!=3) && (m_XYZ.flag[(y-1)*m_col+x]!=4 && m_XYZ.flag[y*m_col+(x+1)]!=4))
					{
						if (lz>z && flag!=2 && flag!=30 && flag!=40)
						{
							p=new SybStr;
							p->x=x+1;
							p->y=y-1;
							p->z=lz;
							p->yl=lyl;
							p->ylpec=m_XYZ.ylpec[(y-1)*m_col+(x+1)];
							p->zeroflag=m_XYZ.zeroflag[(y-1)*m_col+(x+1)];
							p->flag=flag;
							p->next=NULL;
						
							lcount++;
							if (q==NULL)
							{
								head=p;
								q=head;
							}
							else
							{
								q->next=p;
								q=p;
							}
						}
					}
			}//����
		}//�ϱ߽���
		if (x+1<m_col)	//��������
		{
					lz=m_XYZ.z[y*m_col+(x+1)];
					lyl=m_XYZ.yl[y*m_col+(x+1)];
					flag=m_XYZ.flag[y*m_col+(x+1)];
					if ((m_XYZ.flag[(y-1)*m_col+x+1]!=3 && m_XYZ.flag[(y+1)*m_col+(x+1)]!=3) && (m_XYZ.flag[(y-1)*m_col+x+1]!=4 && m_XYZ.flag[(y+1)*m_col+(x+1)]!=4))
					{
						if (lz>z && flag!=2 && flag!=30 && flag!=40)
						{
							p=new SybStr;
							p->x=x+1;
							p->y=y;
							p->z=lz;
							p->yl=lyl;
							p->ylpec=m_XYZ.ylpec[y*m_col+(x+1)];
							p->zeroflag=m_XYZ.zeroflag[y*m_col+(x+1)];
							p->flag=flag;
							p->next=NULL;
						
							lcount++;
							if (q==NULL)
							{
								head=p;
								q=head;
							}
							else
							{
								q->next=p;
								q=p;
							}
						}
					}
		}//�ұ߽���
		if (y+1<m_row)//�±�
		{
			if (x+1<m_col)
			{
					lz=m_XYZ.z[(y+1)*m_col+(x+1)];
					lyl=m_XYZ.yl[(y+1)*m_col+(x+1)];
					flag=m_XYZ.flag[(y+1)*m_col+(x+1)];
					if ((m_XYZ.flag[y*m_col+x+1]!=3 && m_XYZ.flag[(y+1)*m_col+x]!=3) && (m_XYZ.flag[y*m_col+x+1]!=4 && m_XYZ.flag[(y+1)*m_col+x]!=4))
					{
						if (lz>z && flag!=2 && flag!=30 && flag!=40)
						{
							p=new SybStr;
							p->x=x+1;
							p->y=y+1;
							p->z=lz;
							p->yl=lyl;
							p->ylpec=m_XYZ.ylpec[(y+1)*m_col+(x+1)];
							p->zeroflag=m_XYZ.zeroflag[(y+1)*m_col+(x+1)];
							p->flag=flag;
							p->next=NULL;
						
							lcount++;
							if (q==NULL)
							{
								head=p;
								q=head;
							}
							else
							{
								q->next=p;
								q=p;
							}
						}
					}//�������½�
			}
			//��������
					lz=m_XYZ.z[(y+1)*m_col+x];
					lyl=m_XYZ.yl[(y+1)*m_col+x];
					flag=m_XYZ.flag[(y+1)*m_col+x];
					if ((m_XYZ.flag[(y+1)*m_col+x-1]!=3 && m_XYZ.flag[(y+1)*m_col+(x+1)]!=3) && (m_XYZ.flag[(y+1)*m_col+x-1]!=4 && m_XYZ.flag[(y+1)*m_col+(x+1)]!=4))
					{
						if (lz>z && flag!=2 && flag!=30 && flag!=40)
						{
							p=new SybStr;
							p->x=x;
							p->y=y+1;
							p->z=lz;
							p->yl=lyl;
							p->ylpec=m_XYZ.ylpec[(y+1)*m_col+x];
							p->zeroflag=m_XYZ.zeroflag[(y+1)*m_col+x];
							p->flag=flag;
							p->next=NULL;
						
							lcount++;
							if (q==NULL)
							{
								head=p;
								q=head;
							}
							else
							{
								q->next=p;
								q=p;
							}
						}
					}
		};//�±߽���
		
//����˳����:�����½ſ�ʼ,˳ʱ����ת�õ�����,Ӧ�����������е�.
//Ŀǰp��q��ָ�����һ��ָ��,headָ��ͷָ��
	if (head!=NULL)
	{
			if (lcount==1)
			{
				int ly,lx;
				head->ylpec=1.0;
				ly=head->y;
				lx=head->x;
				head->yl=head->yl+yl;
				m_XYZ.yl[ly*m_col+lx]=head->yl;
				if (m_XYZ.zeroflag[y*m_col+x]==1)	//ȷ��ֻ�ܲ�һ��
				{
					hjdnode *hjdp;
					hjdp=new hjdnode;
					hjdp->vos=ly*m_col+lx;
					hjdp->ylpec=1.0;
					hjdp->next=NULL;
					if (m_YLT[y*m_col+x].firsthjd!=NULL)
					{
						hjdp->next=m_YLT[y*m_col+x].firsthjd->next;
						m_YLT[y*m_col+x].firsthjd->next=hjdp;
					}
					else
					{
						m_YLT[y*m_col+x].firsthjd=hjdp;
					}
				}
				m_XYZ.zeroflag[y*m_col+x]=0;//�����ȫ����ȥ��Ӧ������Ҫ��Ϊ0
				return head;
			}
			//��һ�ΰѵ�һ���ڵ��ٸ��Ƶ����һ����	
			p=new SybStr;
			p->x=head->x;
			p->y=head->y;
			p->z=head->z;
			p->yl=head->yl;
			p->ylpec=head->ylpec;
			p->zeroflag=head->zeroflag;
			p->flag=head->flag;
			p->next=NULL;

			q->next=p;
			q=p;
			//
			q=head;
			p=q;
			DOT lldot;//��ʱ������¼�ֲ���ߵ�
			lldot.x=head->x;
			lldot.y=head->y;
			lldot.z=head->z;
			lldot.flag=head->flag;
			lldot.yl=head->yl;
			lldot.ylpec=head->ylpec;
			lldot.zeroflag=head->zeroflag;
			SybStr *head2=NULL;//��Ŵ�����ı�����ĵ�ߵĵ㣨���������ߵ㣩
			SybStr *p2,*q2;

			while(p->next!=NULL)
			{
				if (q!=p)
				q=q->next;//q������p��
				
				p=p->next;
				
				if (abs(p->x-q->x)+abs(p->y-q->y)==1)//˵������
				{
					if (p->z>lldot.z)//�������е����ֵ
					{
						lldot.x=p->x;
						lldot.y=p->y;
						lldot.z=p->z;
						lldot.flag=p->flag;
						lldot.yl=p->yl;
						lldot.ylpec=p->ylpec;
						lldot.zeroflag=p->zeroflag;
						
					} //end if p->z>lldot.z
					if (p->next==NULL)//����������һ���㣬��ôҪ��head2�Ƚ�һ��
					{
						if (head2!=NULL)
						{
							if (head2->x!=lldot.x || head2->y!=lldot.y)//������ֵ����head2����ͷ��Ҫ�޸�ͷ
							{
								head2->x=lldot.x;
								head2->y=lldot.y;
								head2->z=lldot.z;
								head2->flag=lldot.flag;
								head2->yl=lldot.yl;
								head2->ylpec=lldot.ylpec;
								head2->zeroflag=lldot.zeroflag;
							}
						}
						else
						{
							p2=new SybStr;
							p2->x=lldot.x;
							p2->y=lldot.y;
							p2->z=lldot.z;
							p2->flag=lldot.flag;
							p2->yl=lldot.yl;
							p2->ylpec=lldot.ylpec;
							p2->zeroflag=lldot.zeroflag;
							p2->next=NULL;
							head2=p2;
							q2=p2;
							
						}
						break;
					}
				} //end if ==1
				else//������������ֻ��һ����
				{
					
						p2=new SybStr;
						p2->x=lldot.x;
						p2->y=lldot.y;
						p2->z=lldot.z;
						p2->flag=lldot.flag;
						p2->yl=lldot.yl;
						p2->ylpec=lldot.ylpec;
						p2->zeroflag=lldot.zeroflag;
						p2->next=NULL;
						if (head2==NULL)
						{
							head2=p2;
							q2=p2;
						}
						else
						{
							q2->next=p2;
							q2=p2;
						}
						if (p->next==NULL)//�����һ���㲻��������ô���һ����Ҫ�ٿ�����
						break;
						lldot.x=p->x;//�ӶϿ��������¶����µ���ߵ㣬������������
						lldot.y=p->y;
						lldot.z=p->z;
						lldot.flag=p->flag;
						lldot.yl=p->yl;
						lldot.ylpec=p->ylpec;
						lldot.zeroflag=p->zeroflag;
					
				} //end if else
				
				
			}
	
			//���洦�������ķ���
			SybStr *ylpt2;
			ylpt2=head2;
			sum=0;
			while (ylpt2!=NULL)//�õ���ĸ
			{
				sum=sum+(ylpt2->z-z);
				ylpt2=ylpt2->next ;
			}
			ylpt2=head2;
			int lx,ly;
			bool clylflag=false;
			while (ylpt2!=NULL)
			{
				clylflag=true;
				ylpt2->yl=ylpt2->yl+yl*((ylpt2->z-z)/sum);
				lx=ylpt2->x;
				ly=ylpt2->y;
				m_XYZ.yl[ly*m_col+lx]=ylpt2->yl;
				if (m_XYZ.zeroflag[y*m_col+x]==1)
				{
					hjdnode *hjdp;
					hjdp=new hjdnode;
					hjdp->vos=ly*m_col+lx;
					hjdp->ylpec=(ylpt2->z-z)/sum;
					hjdp->next=NULL;
					if (m_YLT[y*m_col+x].firsthjd!=NULL)
					{
						hjdp->next=m_YLT[y*m_col+x].firsthjd->next;
						m_YLT[y*m_col+x].firsthjd->next=hjdp;
					}
					else
					{
						m_YLT[y*m_col+x].firsthjd=hjdp;
					}
				}
				ylpt2->ylpec=(ylpt2->z-z)/sum;//�����͵ĵ�������������������ʱ���š�
				ylpt2=ylpt2->next;
			}
			if (clylflag==true)
				m_XYZ.zeroflag[y*m_col+x]=0;
			
			return head2;
	}
		return NULL;//head==NULL
}


void symnclass::loadsyb(FILE *fp2,FILE *fpoil)
{
	FILE *zfp;
	SybStr *p;
	SybStr *q;
//	SybStr *q2;
	sybpointer=NULL;
	zfp=fp2;
	fseek(zfp,5,SEEK_SET);
	int row,col;
	fscanf(zfp,"%d",&col);
	fscanf(zfp,"%d",&row);
	if (whole_m_col!=col || whole_m_row!=row)
	{
			AfxMessageBox("�����ļ�����ֵ��ƥ��");
	 		exit(0);
	}
	double lsd; //modify by xxzhu 20170317
	double lsf;
	double min;		//notice:�ն������������Сֵ�Ƕ��ġ����ǲ����м̳�֮�������Сֵ�ͱ����أ�
	double max;
	//fscanf(zfp,"%d%f%d%f%f%f",&lsd,&lsf,&lsd,&lsf,&min,&max);
	fscanf(zfp, "%lf%lf", &lsd, &lsf); //������Դ�����쳣���ڱ���Ӧ��д�����ĵط�д��double���¶�ȡ����
	fscanf(zfp, "%lf%lf%lf%lf",&lsd, &lsf, &min, &max);
	float tValue;
	int k;
	float yl;
    

	
	for( int i=0; i<row; i++)
	{
		for( int j=0; j<col; j++)
		{
			 k = i * col + j;
			 fscanf(zfp,"%f",&tValue);
			 if (fpoil!=NULL)
			 {
				 fprintf(fpoil,"%d,%d,%f\n",j,i,tValue);
			 }
			if ((tValue>=min && tValue<=max) && m_XYZ.flag[k]==0)      //���ȷ������Դ,�������ｨ�����Ұ��ߵ����� 0������Ч�ʣ��ڱ߽����棩
			{
				//�����ͱ���,���Ա�־Ϊ1
				if (tValue>0)			//��������Ǹ�ֵ��0����
				{
					yl=tValue;
					m_XYZ.flag[k]=1;
					m_XYZ.yl[k]=tValue;
					m_YLT[k].yl=tValue;
					p=new SybStr;
					p->x=j;
					p->y=i;
					p->z=m_XYZ.z[k];
					p->yl=tValue;
					p->ylpec=0.00;
					p->flag=1;
					p->zeroflag=m_XYZ.zeroflag[k];
					p->next=NULL;
					
					q=sybpointer;
					
					if (sybpointer==NULL)
					{
						sybpointer=p;
						q=sybpointer;
						p->next=NULL;
						
					}
					else
					{
						p->next=sybpointer->next;
						sybpointer->next=p;
						
						//��һ���ǰ��մӵ͵������򣬺���û�б�Ҫ
						/*while (q->z<p->z && q->next!=NULL)
						{
							q2=q;
							q=q->next;
						}
						if (p->z<=q->z)
						{
							if (sybpointer==q) 
							{
								sybpointer=p;
								p->next=q;
							}
							else
							{
								p->next=q;
								q2->next=p;
								
							}
						
						}
						else
						{
							q->next=p;
							p->next=NULL;
						}*/
												
					}//end else
				}
				else
				{
					m_XYZ.yl[k]=0;
					m_YLT[k].yl=0;
				}

	
			}
			else
			{
				if (tValue<min || tValue>max)//���������Χ��������Ϊ��Чֵ�����û������������޸�
				{
					//m_XYZ.flag[k]=2;
					//m_XYZ.flag[k]=0;
					m_XYZ.yl[k]=0;
					m_YLT[k].yl=0;
				}
			}//end if*/
			

		}//end for
	}//end for

	if (fpoil!=NULL)
		fclose(fpoil);
	fclose(zfp);

}
void symnclass::loaddata(FILE *fp,FILE *fpbj)
{

	//��ʼ���������κε㶼��Ч��flag��2�������������տ�ʼΪ(yl=0)
	//�ڶ����ڱ߽�����ĵ㶼��ЧΪ��flag��0��
	//�����������е㴦���������ж��Ƿ���Ч�㣬����ǣ�������flag��1��

	int col,row;
	int i,j,k;
	
    col=m_col;
	row=m_row;

	float tValue;
	for( i=0; i<row; i++)
	{
		m_XYZ.y[i]=i;
		for( j=0; j<col; j++)
		{
			k = i * col + j;
			fscanf(fp, "%f", &tValue);
			m_XYZ.z[k] = tValue*(-1);
			  
			m_XYZ.ylpec[k]=0.00;
			m_XYZ.zeroflag[k]=1;//�տ�ʼ����Ҫ���ó�0��
			m_YLT[k].firsthjd=NULL;
			m_YLT[k].yl=0;
			if (fpbj!=NULL)	//����б߽磬��
			m_XYZ.flag[k]=2;//��ʼ��,���еĵ㶼�������͵�,���е㶼��Ч.
			else		//���û�б߽磬ȫ�����Ƿ���Чֵͨ�����е������Сֵ���жϣ�������ΧΪ��Ч����������Ч��,��������ȥ�������������ֵ�ġ�
			{				
				if (tValue>=min && tValue<=max)	//�ޱ߽磬��û������Χ��
				{
					m_XYZ.flag[k]=0;
				}
				else			//�ޱ߽磬��������Χ
				m_XYZ.flag[k]=2;
			}
			m_XYZ.yl[k]=0;
			if (i==0)
			m_XYZ.x[j] =j;
		}
	}

	//������Ч��ΪflagΪ0,��ô�������͵�ʱ��һ���ж�,�������Ч����ô�ٰ�������Ϊ1(�����͵�)
	
	char ys[6],xs[6];
	int y1,x1,y2,x2;
	int loc1,loc2;
	
	if (fpbj!=NULL)             //�б߽��ļ���
	{
		while (!feof(fpbj))
		{
			fscanf(fpbj,"%s",ys);
			fscanf(fpbj,"%s",xs);
			y1=atoi(ys);
			x1=atoi(xs);
			//y1=stringtoint(ys);
			//x1=stringtoint(xs);
			loc1=ftell(fpbj);
			fscanf(fpbj,"%s",ys);
			fscanf(fpbj,"%s",xs);
			y2=atoi(ys);
			x2=atoi(xs);
			//y2=stringtoint(ys);
			//x2=stringtoint(xs);
			if (y1==y2 && y1<m_row)//����ǳɶԵ���ɱ߽�
			{
				for(i=x1;i<=x2;i++)
				{
					if (i<m_col)    //û�г�����Χ
					{
					
						if (m_XYZ.z[y1*m_col+x1]*(-1)>=min && m_XYZ.z[y1*m_col+x1]*(-1)<=max)
						{
							k=y1*col+i;
							m_XYZ.flag[k]=0;//0Ϊ��Чֵ
						}
					}
				}
			}
			else//������ɶԣ� �����ѳ��߽�
			{
				if (y1!=y2 && y1<m_row)  //���ɶԣ�����û�г���
				{
					if (x1<m_col)  //����x û�г���
					{
						if (m_XYZ.z[y1*m_col+x1]*(-1)>=min && m_XYZ.z[y1*m_col+x1]*(-1)<=max)
						{
							k=y1*col+x1;
							m_XYZ.flag[k]=0;//��һ����Ч,
						}
						loc2=ftell(fpbj);
						fseek(fpbj,loc1-loc2,SEEK_CUR);//ָ�����һ���ַ���;
					}
				}
				if (y1!=y2 && y1>=m_row)	//��Ϊy���������еģ�����y2�϶�Ҳ��Ч
				{
					//˵������������Чֵ
				}
				if (y1==y2 && y1>=m_row)
				{
					//˵������������Чֵ
				}
			}
		}//end while
		
		fclose(fpbj);
	}//end if fpbj!=NULL

}
struct SybStr* symnclass::compls2(DOT dot)//��һ������Χ�����ߵĵ�
{
		//x��x�����꣬y��y������(x,y��0��ʼ)������Χ�����ߵĵ㣬���ض���

		float lz,lyl;//�ݴ棨x,y��Ӧ��z,yl��ֵ��
		int flag;
		int x=dot.x;
		int y=dot.y;
		float z=dot.z;
		float yl=dot.yl;
		SybStrShX *head;
		SybStrShX *p,*q;
		
		float sum;//�������������ķ��䡣sum�Ǹ߶Ⱥͣ�ÿ���߶ȵı���������������
		head=NULL;
		q=head;
		int lcount=0;
		//��Χ�ĵ�
		if (x-1>=0)//���
		{

				if (y+1<m_row)
				{
					lz=m_XYZ.z[(y+1)*m_col+(x-1)];
					lyl=m_XYZ.yl[(y+1)*m_col+(x-1)];
					flag=m_XYZ.flag[(y+1)*m_col+(x-1)];
					if (lz>z && flag==1)//�����͵㣨1��
					{
						p=new SybStrShX;
						p->x=x-1;
						p->y=y+1;
						p->z=lz;
						p->yl=lyl;
						p->flag=flag;
						p->next=NULL;
					
							
						lcount++;
						if (q!=NULL)
						{
							q->next=p;
							p->prev=q;
						}
						else
						{
							p->prev=NULL;
							head=p;
						}

						q=p;
					}
				}//���½�
				//�������
					lz=m_XYZ.z[y*m_col+(x-1)];
					lyl=m_XYZ.yl[y*m_col+(x-1)];
					flag=m_XYZ.flag[y*m_col+(x-1)];
					if (lz>z && flag==1)//�����͵㣨1��
					{
						p=new SybStrShX;
						p->x=x-1;
						p->y=y;
						p->z=lz;
						p->yl=lyl;
						p->flag=flag;
						p->next=NULL;
					
						lcount++;
						if (q!=NULL)
						{
							q->next=p;
							p->prev=q;
						}
						else
						{
							p->prev=NULL;
							head=p;
						}
						q=p;
					}//��߽���
				if (y-1>=0)//���Ͻ�
				{
					lz=m_XYZ.z[(y-1)*m_col+(x-1)];
					lyl=m_XYZ.yl[(y-1)*m_col+(x-1)];
					flag=m_XYZ.flag[(y-1)*m_col+(x-1)];
					if (lz>z && flag==1)//�����͵㣨1��
					{
						p=new SybStrShX;
						p->x=x-1;
						p->y=y-1;
						p->z=lz;
						p->yl=lyl;
						p->flag=flag;
						p->next=NULL;
					
						lcount++;
						if (q!=NULL)
						{
							q->next=p;
							p->prev=q;
						}
						else
						{
							p->prev=NULL;
							head=p;
						}
						q=p;
					}
					
				}//���Ͻ�

		}//��߽�����
		if (y-1>=0)//�ϱ�
		{
			
						//��������
			lz=m_XYZ.z[(y-1)*m_col+x];
			lyl=m_XYZ.yl[(y-1)*m_col+x];
			flag=m_XYZ.flag[(y-1)*m_col+x];
			if (lz>z && flag==1)//�����͵㣨1��
			{
				p=new SybStrShX;
				p->x=x;
				p->y=y-1;
				p->z=lz;
				p->yl=lyl;
				p->flag=flag;
				p->next=NULL;
				
				lcount++;
				if (q!=NULL)
				{
					q->next=p;
					p->prev=q;
				}
				else
				{
					p->prev=NULL;
					head=p;
				}
				q=p;
			}//����
			if (x+1<m_col)//���Ͻ�
			{
					lz=m_XYZ.z[(y-1)*m_col+(x+1)];
					lyl=m_XYZ.yl[(y-1)*m_col+(x+1)];
					flag=m_XYZ.flag[(y-1)*m_col+(x+1)];
					if (lz>z && flag==1)//�����͵㣨1��
					{
						p=new SybStrShX;
						p->x=x+1;
						p->y=y-1;
						p->z=lz;
						p->yl=lyl;
						p->flag=flag;
						p->next=NULL;
					
						lcount++;
						if (q!=NULL)
						{
							q->next=p;
							p->prev=q;
						}
						else
						{
							p->prev=NULL;
							head=p;
						}
						q=p;
					}
			}//����
		}//�ϱ߽���
		if (x+1<m_col)	//��������
		{
					lz=m_XYZ.z[y*m_col+(x+1)];
					lyl=m_XYZ.yl[y*m_col+(x+1)];
					flag=m_XYZ.flag[y*m_col+(x+1)];
					if (lz>z && flag==1)//�����͵㣨1��
					{
						p=new SybStrShX;
						p->x=x+1;
						p->y=y;
						p->z=lz;
						p->yl=lyl;
						p->flag=flag;
						p->next=NULL;
					
						lcount++;
						if (q!=NULL)
						{
							q->next=p;
							p->prev=q;
						}
						else
						{
							p->prev=NULL;
							head=p;
						}
						q=p;
					}
		}//�ұ߽���
		if (y+1<m_row)//�±�
		{
			if (x+1<m_col)
			{
					lz=m_XYZ.z[(y+1)*m_col+(x+1)];
					lyl=m_XYZ.yl[(y+1)*m_col+(x+1)];
					flag=m_XYZ.flag[(y+1)*m_col+(x+1)];
					if (lz>z && flag==1)//�����͵㣨1��
					{
						p=new SybStrShX;
						p->x=x+1;
						p->y=y+1;
						p->z=lz;
						p->yl=lyl;
												
						p->flag=flag;
						p->next=NULL;
					
						lcount++;
						if (q!=NULL)
						{
							q->next=p;
							p->prev=q;
						}
						else
						{
							p->prev=NULL;
							head=p;
						}
						q=p;
					};
			}//�������½�
			//��������
					lz=m_XYZ.z[(y+1)*m_col+x];
					lyl=m_XYZ.yl[(y+1)*m_col+x];
					flag=m_XYZ.flag[(y+1)*m_col+x];
					if (lz>z && flag==1)//�����͵㣨1��
					{
						p=new SybStrShX;
						p->x=x;
						p->y=y+1;
						p->z=lz;
						p->yl=lyl;
						p->flag=flag;
						p->next=NULL;
					
						lcount++;
						if (q!=NULL)
						{
							q->next=p;
							p->prev=q;
						}
						else
						{
							p->prev=NULL;
							head=p;
						}
						q=p;
					}

		};//�±߽���
		
//����˳����:�����½ſ�ʼ,˳ʱ����ת�õ�����,Ӧ�����������е�.
//Ŀǰp��q��ָ�����һ��ָ��,headָ��ͷָ��
	if (head!=NULL)
	{
		SybStrShX *tail;
		tail=p;//��¼˫��朱��β��¼
		SybStrShX *pprev;
		pprev=tail;//��pprevʼ����p��
//���濼������������ֵ����
		q=head;//qָ��ͷ
		SybStrShX *qnext;
		qnext=head;//qnextʼ����qǰ
		DOT lldot;//�������������¼���������z�����Ǹ��㡣
		lldot.x=q->x;
		lldot.y=q->y;
		lldot.z=q->z;
		lldot.flag=q->flag;//��lldot����ֵ
		lldot.yl=q->yl;
		SybStr *head2;//׼���½�һ�����д�Ŵ���������ϵ��,����ļ���
		SybStr *p2,*q2;
		head2=NULL;
		p2=head2;
		q2=head2;
	if (head->next==NULL)//ֻ��һ���ڵ�,����Ͳ�Ҫ����
	{
		p2=new SybStr;
		p2->x=lldot.x;
		p2->y=lldot.y;
		p2->z=lldot.z;
		p2->yl=lldot.yl;
		p2->flag=lldot.flag;
		p2->next=NULL;
		head2=p2;
		return head2;
	}
	//����>=2���ڵ�
	if (lcount==2)
	{
		if (abs(p->x-q->x)+abs(p->y-q->y)==1)	//ֻ�������㣬����β����
		{
			if (p->z>q->z)
			{
				lldot.x=p->x;
				lldot.y=p->y;
				lldot.z=p->z;
				lldot.flag=p->flag;
				lldot.yl=p->yl;
			}

			p2=new SybStr;
			p2->x=lldot.x;
			p2->y=lldot.y;
			p2->z=lldot.z;
			p2->flag=lldot.flag;
			p2->yl=lldot.yl;
			p2->next=NULL;
			head2=p2;
			//�����㣬��β������ֻ��ѡ��һ��
									
		}
		else//�����㣬��β����������������
		{
			sum=0;
			sum=(lldot.z-z)+(p->z-z);
			p2=new SybStr;
			p2->x=lldot.x;
			p2->y=lldot.y;
			p2->z=lldot.z;
			p2->yl=lldot.yl+((lldot.z-z)/sum)*yl;
			p2->flag=lldot.flag;
			p2->next=NULL;
			head2=p2;	//��һ����
						
			p2=new SybStr;
			p2->x=p->x;
			p2->y=p->y;
			p2->z=p->z;
			p2->flag=p->flag;
			p2->yl=p->yl+((p->z-z)/sum)*yl;
			p2->next=NULL;//�ڶ�����
			head2->next=p2;//��һ������ڶ���������
			
		}
		return head2;

	}	//�����������������
	//�����Ǵ���2��������
	if (abs(p->x-q->x)+abs(p->y-q->y)==1)//˵����β����
	{
		//�����������headҪ�����ң�tailҪ��ǰ�ң����Ƿ��������ġ�
		if (p->z>q->z)//����β�Ƚ�
		{
			lldot.x=p->x;
			lldot.y=p->y;
			lldot.z=p->z;
			lldot.flag=p->flag;
			lldot.yl=p->yl;
		}
		while (p!=head)//��β����ʼ�������ģ������������������ĸ߶ȣ�ֱ������������
		{
			p=p->prev;
			if (abs(pprev->x-p->x)+abs(pprev->y-p->y)==1)
			{
				if (p->z>lldot.z)
				{
					lldot.x=p->x;
					lldot.y=p->y;
					lldot.z=p->z;
					lldot.yl=p->yl;
					lldot.flag=p->flag;
					if (p!=head)
					{
					pprev=pprev->prev;
					p=p->prev;
					}
					else
					break;
					
				}
				else	//����Ӻ�����ǰһ�����������߶�С
				{
					if (p!=head)//����û�е���ͷ��������ǰ
					{
					pprev=pprev->prev;
					p=p->prev;
					}
							
				}
			}
			else
			break;//������������
		}// end while p->prev!=head
		while (q!=p)//��ͷ����ʼ�������ģ��������������ң����������
		{
			q=q->next;//qnext ��q��ǰ��
			if (abs(qnext->x-q->x)+abs(qnext->y-q->y)==1)//������
			{
				if (q->z>lldot.z)//���������Ҵ���
				{
					lldot.x=q->x;
					lldot.y=q->y;
					lldot.z=q->z;
					lldot.flag=q->flag;
					lldot.yl=q->yl;
					if (q==p)
					{
								p2=new SybStr;
								p2->x=lldot.x;
								p2->y=lldot.y;
								p2->z=lldot.z;
								p2->flag=lldot.flag;
								p2->yl=lldot.yl;
								p2->next=NULL;
								if (head2==NULL)
								head2=p2;
								q2=p2;
								break;
					}

				}
				else//��������С��
				{
						if (q!=p)
						{
								q=q->next;
								qnext=qnext->next;
						}
						else
						{
									
								break;
						}
				}
			}
			else//��������
			{
						
						p2=new SybStr;
						p2->x=lldot.x;
						p2->y=lldot.y;
						p2->z=lldot.z;
						p2->flag=lldot.flag;
						p2->yl=lldot.yl;
						p2->next=NULL;
						if (head2==NULL)
						head2=p2;
						q2=p2;	//�����������ֵ����
				
				lldot.x=q->x;//�Ӳ������ĵط�����Ѱ�������ߵ㣬�Ѷ��˵ĵ�����Ϊ��ʼֵ
				lldot.y=q->y;
				lldot.z=q->z;
				lldot.flag=q->flag;
				lldot.yl=q->yl;
			}//end if else
			//if (q->next!=p)
			//qnext=qnext->next;
		}//end while (q!=p)
	}
	else//˵����β������
	{
		while (q!=NULL)	
		{
			q=q->next;
			if (q==NULL)
			break;
			if (abs(qnext->x-q->x)+abs(qnext->y-q->y)==1)//˵������
			{
				if (q->z>lldot.z)
				{
					lldot.x=q->x;
					lldot.y=q->y;
					lldot.z=q->z;
					lldot.flag=q->flag;
					lldot.yl=q->yl;
					if (q->next==NULL)
					{
							
							p2=new SybStr;
							p2->x=lldot.x;
							p2->y=lldot.y;
							p2->z=lldot.z;
							p2->flag=lldot.flag;
							p2->yl=lldot.yl;
							p2->next=NULL;
							if (head2==NULL)
							head2=p2;
							q2=p2;
							break;
					}
				}//��������
				else
				{
					
					if(q->next==NULL)
						break;
					else
					{
						q=q->next;
						qnext=qnext->next;
					}
					
				}
			
			}
			else//������
			{
				
						p2=new SybStr;
						p2->x=lldot.x;
						p2->y=lldot.y;
						p2->z=lldot.z;
						p2->flag=lldot.flag;
						p2->yl=lldot.yl;
						p2->next=NULL;
						if (head2==NULL) 
						head2=p2;
						q2=p2;
				
						lldot.x=q->x;//��¼��������
						lldot.y=q->y;
						lldot.z=q->z;
						lldot.flag=q->flag;
						lldot.yl=q->yl;
				
			}//end else
		//	if (q->next!=NULL)
		//	qnext=qnext->next;

		}//end while
			
			
	}//end else
	
	//���洦�������ķ���
	SybStr *ylpt2;
	ylpt2=head2;
	sum=0;
	while (ylpt2!=NULL)
	{
		sum=sum+(ylpt2->z-z);
		ylpt2=ylpt2->next ;
	}
	ylpt2=head2;
	int lx,ly;
	while (ylpt2!=NULL)
	{
		ylpt2->yl=ylpt2->yl+yl*((ylpt2->z-z)/sum);
		lx=ylpt2->x;
		ly=ylpt2->y;
		m_XYZ.yl[ly*m_col+lx]=ylpt2->yl;
		ylpt2=ylpt2->next;
	}
	//delete ylpt2;
	return head2;
}
		return NULL;
}
void symnclass::push(DOT dot)
{
	stack *qstack;
	stack *pdot;
	qstack=stackmn;
	if (stackmn==NULL)
	{
		pdot=new stack;
		pdot->dot=dot;
		pdot->next=NULL;
		stackmn=pdot;
		qstack=stackmn;
	}
	else
	{
	while (qstack->next!=NULL)
	qstack=qstack->next;
	pdot=new stack;
	pdot->dot=dot;
	pdot->next=NULL;
	qstack->next=pdot;
	}


}
struct DOT symnclass::pop(struct stack *stackmn)
{
	struct stack *pstack,*qstack;
	qstack=stackmn;
	pstack=stackmn;
	while(pstack->next!=NULL)
	{
		if (pstack!=stackmn)
		qstack=qstack->next;
		pstack=pstack->next;
	}
	DOT dot;
	if (pstack!=NULL&&pstack->next==NULL)
	{
		dot=pstack->dot;
		if (pstack!=stackmn)
		{
		qstack->next=NULL;
		delete pstack;
		}
		else
		stackmn=NULL;
	}
	return dot ;
	
}
int symnclass::stringtoint(char a[6])
{
	int ly;
	int i,j,t;
	int y;
	y=0;
		for(i=0;i<6;i++)
		{
			if (a[i]=='\0')
			break;
		}
		i=i-1;
		for(j=0;j<6;j++)
		{
			if (a[j]!='\0')
			{
				ly=(int)(a[j]-48);
				for (t=0;t<i;t++)
					ly=ly*10;
				y=y+ly;
				i=i-1;
			}
 			else
				break;

		}
		return(y);
		
}

void symnclass::composeoil(XYZ m_XYZ)
{
	//�ѵڶ��������������������m_XYZ�е�����������һ���µ������ļ�
	//���⣺��������������������������ô���
}

void symnclass::dealdc(CString FileName,XYZ dm_XYZ,int dm_col,int dm_row,int colmin,float colmax,int rowmin,float rowmax,int turn)
{
		
		//���ϲ����ݣ�����յĻ��߲���յģ�
	//turn��1�Ƿ��ʽ�ϲ�
	//turn��2�ǿ���ʽ�ϲ�
	//char ret[]="\r\n";
	//char sharp[]="##";
	//char zero[]="0";
	//char minx[]={0,0,0,0,0,0};
	CBJDATA	bj_data;
	

	int length=FileName.GetLength();
	length++;
	LPTSTR p = FileName.GetBuffer(length);
	char file[256];
	strcpy(file, ::globalDatPath);
	if (*p != '\\')
		strcat(file, "\\" );  //xxzhu 20170317
	strcat(file, p);

	FileName.ReleaseBuffer();
	bj_data.readfile(file);

	int success;
	int all_duanceng;  //���жϲ���
	int point_count;    //ÿһ��ĵ���
	bj_data.readline();//����Ч����wmp123
	bj_data.readline();//���ϲ���
	success=sscanf(bj_data.m_pBufferLine,"%d",&all_duanceng);
	ASSERT(success==1);

/*	if(!bj_data.writefile("C:\\WINDOWS\\Desktop\\data\\new5.25\\bj\\bjGtb1dcb.txt"))
	 {
		   AfxMessageBox("File could not be opened.");
		   return;
	 }*/

	for(int idc=1;idc<=all_duanceng;idc++)
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
		int minxx,minyy,maxxx,maxyy;//�õ�line_count=0��������
		if (point_count>0)
		{
			
			for(int j=0;j<bj_data.maxver;j++)
			{
				bj_data.readword();
				success=sscanf(bj_data.m_pBufferWord,"%f",&tempf);
				ASSERT(success==1);
				bj_data.vertex.x[j]=(int)(dm_col*(tempf-colmin)/(colmax-colmin)+0.5f);//�����ϵ�x
				if (j==0)
				{
					minxx=bj_data.vertex.x[j];
					maxxx=bj_data.vertex.x[j];
					minyy=bj_data.vertex.y[j];
					maxyy=bj_data.vertex.y[j];
				}
				if (j>0 && bj_data.vertex.x[j]<minxx)
					minxx=bj_data.vertex.x[j];
				if (j>0 && bj_data.vertex.x[j]>maxxx)
					maxxx=bj_data.vertex.x[j];
				bj_data.readword();
				success=sscanf(bj_data.m_pBufferWord,"%f",&tempf);
				bj_data.vertex.y[j]=(int)(dm_row*(tempf-rowmin)/(rowmax-rowmin)+0.5f);//�����ϵ�y
				if (j>0 && bj_data.vertex.y[j]<minyy)
					minyy=bj_data.vertex.y[j];
				if (j>0 && bj_data.vertex.y[j]>maxyy)
					maxyy=bj_data.vertex.y[j];
				if (j==0)
				{
					minxx=bj_data.vertex.x[j];
					minyy=bj_data.vertex.y[j];
					maxxx=minxx;
					maxyy=minyy;
				}
			}
		
		//ָ�����飬ÿ��Ԫ����ָ��struct edge_info�Ľṹ

			bj_data.init_egtab(bj_data.egtab,maxheight);
			bj_data.create_edge_table(bj_data.egtab,bj_data.maxver,bj_data.vertex);
			bj_data.line_count=0;
			bj_data.draw(dm_XYZ,dm_col,turn);	
		}
		else
			bj_data.line_count=0;
		if(bj_data.line_count==0) 	
		{
			//FILE *gp;
			//gp=fopen("d:\\tter.txt","a+");
			if (point_count!=0)
			{
				for(int xj=minxx;xj<=maxxx;xj++)
					for(int yj=minyy;yj<=maxyy;yj++)
					{
						 if (m_XYZ.flag[yj*m_col+xj]!=2)
							if (xj==minxx || yj==minyy ||xj==maxxx || yj==maxyy)//�����Ǳ߽���
							{
								if (turn==1)	//�����Ƿ�նϲ�
								{
									if (m_XYZ.flag[yj*m_col+xj]!=30 || m_XYZ.flag[yj*m_col+xj] !=40)
									m_XYZ.flag[yj*m_col+xj]=3;
									//fprintf(gp,"\n%d,%d",yj,xj);
									//fprintf(gp,"%c",'\n');
									//fprintf(gp,"%s%s","##");
									//fclose(gp);
								}							
								else			//�����ǿ���ʽ�ϲ�
								{
									if (m_XYZ.flag[yj*m_col+xj]!=30  || m_XYZ.flag[yj*m_col+xj] !=40)
									m_XYZ.flag[yj*m_col+xj]=4;
								}
							}
							else			//����߽��߰�Χ�Ĳ���
							{
								if (turn==1)		//�����Ƿ�նϲ�
								m_XYZ.flag[yj*m_col+xj]=30;
								else			//�����ǿ���ʽ�ϲ�
								m_XYZ.flag[yj*m_col+xj]=40;
							}
					}	
			}
		   int i=0;
	       int x,y;
		   while(i<(bj_data.maxver-1))
		   {
				if (turn==1)		//�����Ƿ�նϲ�
					if (m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]!=30 || m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]!=40)
					m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]=3;
				else
					if (m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]!=30 || m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]!=40)//�����ǿ���ʽ�ϲ�
					m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]=4;
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
						    if (turn==1)		//�����Ƿ�նϲ�
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=3;
							else			//�����ǿ���ʽ�ϲ�
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=4;
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
						   	if (turn==1)		//�����Ƿ�նϲ�
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=3;
							else			//�����ǿ���ʽ�ϲ�
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=4;
                           j++;
					   }					   
				   }
			   }
			   if((bj_data.vertex.y[i]!=bj_data.vertex.y[i+1])//����бֱ��
                &&(bj_data.vertex.x[i]!=bj_data.vertex.x[i+1]))
               {
				   int margin=bj_data.vertex.y[i]-bj_data.vertex.y[i+1];
				   if(abs(margin)>1)//�������д���
				   {
					   int yl;
					   int j=1;
					   int k;
					   k=(bj_data.vertex.y[i+1]-bj_data.vertex.y[i])/(bj_data.vertex.x[i+1]-bj_data.vertex.x[i]);
					   while(j<abs(margin))
					   {
						   if(margin>0) yl=bj_data.vertex.y[i]-j;
						   else yl=bj_data.vertex.y[i]+j;
						   ///��б����ɨ����yl�Ľ��㣬�ٽ��Ƶ��������
                           x=(int)((bj_data.vertex.x[i]*(yl-bj_data.vertex.y[i+1])-bj_data.vertex.x[i+1]*(yl-bj_data.vertex.y[i]))/(margin)+0.5f);
				           y=yl;
						   	if (turn==1)		//�����Ƿ�նϲ�
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=3;
							else			//�����ǿ���ʽ�ϲ�
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=4;
							if (k>0)  //��
							{
								if (margin>0) //����
								{
									//���������Խǵ�	
									if (turn==1)		//�����Ƿ�նϲ�
										m_XYZ.flag[y*m_col+x-1]=30;
									else			//�����ǿ���ʽ�ϲ�
										m_XYZ.flag[y*m_col+x-1]=40;
									if (turn==1)		//�����Ƿ�նϲ�
										m_XYZ.flag[(y+1)*m_col+x]=30;
									else			//�����ǿ���ʽ�ϲ�
										m_XYZ.flag[(y+1)*m_col+x]=40;	
								}
								else	//����
								{
									if (turn==1)		//�����Ƿ�նϲ�
									m_XYZ.flag[yl*m_col+x-1]=30;
									else			//�����ǿ���ʽ�ϲ�
									m_XYZ.flag[yl*m_col+x-1]=40;
									if (turn==1)		//�����Ƿ�նϲ�
									m_XYZ.flag[(yl-1)*m_col+x]=30;
									else			//�����ǿ���ʽ�ϲ�
									m_XYZ.flag[(yl-1)*m_col+x]=40;	
								}


							}
							else  //��
							{
								if (margin>0) //����
								{
									if (turn==1)		//�����Ƿ�նϲ�
									m_XYZ.flag[yl*m_col+x+1]=30;
									else			//�����ǿ���ʽ�ϲ�
									m_XYZ.flag[yl*m_col+x+1]=40;
									if (turn==1)		//�����Ƿ�նϲ�
									m_XYZ.flag[(yl+1)*m_col+x]=30;
									else			//�����ǿ���ʽ�ϲ�
									m_XYZ.flag[(yl+1)*m_col+x]=40;	
								}
								else	//����
								{
									if (turn==1)		//�����Ƿ�նϲ�
									m_XYZ.flag[yl*m_col+x+1]=30;
									else			//�����ǿ���ʽ�ϲ�
									m_XYZ.flag[yl*m_col+x+1]=40;
									if (turn==1)		//�����Ƿ�նϲ�
									m_XYZ.flag[(yl-1)*m_col+x]=30;
									else			//�����ǿ���ʽ�ϲ�
									m_XYZ.flag[(yl-1)*m_col+x]=40;	
								}
							}


						   j++;
					   }
				   }
			   }
			i++;
		   }
		   ///д���һ����
		   if (bj_data.maxver!=0)
		   {
			   x=bj_data.vertex.x[bj_data.maxver-1];
			   y=bj_data.vertex.y[bj_data.maxver-1];
			   if (turn==1)		//�����Ƿ�նϲ�
					if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
					m_XYZ.flag[y*m_col+x]=3;
			   else			//�����ǿ���ʽ�ϲ�
				   if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
					m_XYZ.flag[y*m_col+x]=4;

		   }



		}  			   
		
		bj_data.readline();//��һ�����һ��������Ч��Ҫȥ��
		delete[] bj_data.vertex.x;
		delete[] bj_data.vertex.y;
	}
//	bj_data.close_file();
	
		
}