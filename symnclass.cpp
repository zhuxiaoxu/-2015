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
	
	//初始化结构
	/*	symnclasslz.m_XYZ.x=new int [symnclasslz.m_col];
		symnclasslz.m_XYZ.y=new int [symnclasslz.m_row];
		symnclasslz.m_XYZ.z=new float [symnclasslz.m_col*symnclasslz.m_row];
		symnclasslz.m_XYZ.flag=new int [symnclasslz.m_col*symnclasslz.m_row];
		symnclasslz.m_XYZ.yl=new float [symnclasslz.m_col*symnclasslz.m_row];*/
	//初始化结构

 

}
symnclass::~symnclass(void)
{
		//释放


}

void symnclass::findpath(struct DOT dot,FILE *fp)
{
	
	struct DOT dot2;
	struct SybStr *pp;

	pp=compls(dot);//周围比它高的点组成的队列
	
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
			sfpop=false;			//是否pop用来控制打印的路径的。比如 a b c d e 已经打印出来，那么a b c d就不要打印了
			if (dot2.flag==0)
			{
				findpath(dot2,fp);
			}
			else
			{
				//从头至尾打印出，并且pop一个
				if (dot2.flag==1 || dot2.flag==3 || dot2.flag==4)
				{
					struct stack *shyhead,*lshyhead;
					shyhead=stackmn;
					lshyhead=stackmn;
					float ylpec;
					
					while (shyhead!=NULL)//如果该点有一个比它高的点，那么这个shyhead中起码有两个点，如果只有一个点，那么说明这个点是局部最高点
					{
						
						if (shyhead->next!=NULL) 
							lshyhead=shyhead->next;
						else
							lshyhead=shyhead;

					//	yl=m_XYZ.yl[shyhead->dot.y*m_col+shyhead->dot.x];
						ylpec=lshyhead->dot.ylpec;
						if (lshyhead!=shyhead)
						ylpec=lshyhead->dot.ylpec;  //因为我把前面一个点的油量增量放在比它高的点里面
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

	if (pp==NULL && gdcount!=0)	//如果是局部最高点
	{
					if (sfpop==false)  //并且前面都只有进栈
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
							lshylpec=0;//最后一个点油量百分比为0
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
						
						pop(stackmn);//说明这个也要pop
					}
	}

					
}

struct SybStr* symnclass::compls(DOT dot)//找一个点周围比它高的点
{
		//x是x轴坐标，y是y轴坐标(x,y从0开始)。找周围比它高的点，返回队列

		float lz,lyl;//暂存（x,y对应的z,yl的值）
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
		
		float sum;//用来计算油亮的分配。sum是高度和，每个高度的比例用来分配油亮
		head=NULL;
		q=head;
		int lcount=0;
		//周围的点
		if (x-1>=0)//左边
		{

				if (y+1<m_row)//左下脚
				{
					lz=m_XYZ.z[(y+1)*m_col+(x-1)];
					lyl=m_XYZ.yl[(y+1)*m_col+(x-1)];
					flag=m_XYZ.flag[(y+1)*m_col+(x-1)];
					if ((m_XYZ.flag[(y+1)*m_col+x]!=3 && m_XYZ.flag[y*m_col+(x-1)]!=3) && (m_XYZ.flag[(y+1)*m_col+x]!=4 && m_XYZ.flag[y*m_col+(x-1)]!=4))
					{
						if (lz>z && flag!=2 && flag!=30 && flag!=40)//不是生油点（1）并且有效（0）(9)是为了走路径时不穿过而插入的点
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
				}//左下脚
				//处理左边
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
						}//左边结束
					}
				if (y-1>=0)//左上脚
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
				}//左上脚

		}//左边结束
		if (y-1>=0)//上边
		{
			
						//处理正上
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
				}//结束
			}
			if (x+1<m_col)//右上脚
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
			}//结束
		}//上边结束
		if (x+1<m_col)	//处理正右
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
		}//右边结束
		if (y+1<m_row)//下边
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
					}//处理右下脚
			}
			//处理正下
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
		};//下边结束
		
//以上顺序按照:从左下脚开始,顺时针旋转得到队列,应该是连续排列的.
//目前p与q都指向最后一个指针,head指向头指针
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
				if (m_XYZ.zeroflag[y*m_col+x]==1)	//确保只能插一次
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
				m_XYZ.zeroflag[y*m_col+x]=0;//这个点全部出去后应该油量要置为0
				return head;
			}
			//这一段把第一个节点再复制到最后一个点	
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
			DOT lldot;//临时变量记录局部最高点
			lldot.x=head->x;
			lldot.y=head->y;
			lldot.z=head->z;
			lldot.flag=head->flag;
			lldot.yl=head->yl;
			lldot.ylpec=head->ylpec;
			lldot.zeroflag=head->zeroflag;
			SybStr *head2=NULL;//存放处理过的比输入的点高的点（连续点的最高点）
			SybStr *p2,*q2;

			while(p->next!=NULL)
			{
				if (q!=p)
				q=q->next;//q领先于p；
				
				p=p->next;
				
				if (abs(p->x-q->x)+abs(p->y-q->y)==1)//说明连续
				{
					if (p->z>lldot.z)//找连续中的最大值
					{
						lldot.x=p->x;
						lldot.y=p->y;
						lldot.z=p->z;
						lldot.flag=p->flag;
						lldot.yl=p->yl;
						lldot.ylpec=p->ylpec;
						lldot.zeroflag=p->zeroflag;
						
					} //end if p->z>lldot.z
					if (p->next==NULL)//如果这是最后一个点，那么要与head2比较一下
					{
						if (head2!=NULL)
						{
							if (head2->x!=lldot.x || head2->y!=lldot.y)//如果最大值大于head2链的头，要修改头
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
				else//不连续，或者只有一个点
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
						if (p->next==NULL)//与最后一个点不连续，那么最后一个不要再考虑了
						break;
						lldot.x=p->x;//从断开处，重新定义新的最高点，继续找连续点
						lldot.y=p->y;
						lldot.z=p->z;
						lldot.flag=p->flag;
						lldot.yl=p->yl;
						lldot.ylpec=p->ylpec;
						lldot.zeroflag=p->zeroflag;
					
				} //end if else
				
				
			}
	
			//下面处理油量的分配
			SybStr *ylpt2;
			ylpt2=head2;
			sum=0;
			while (ylpt2!=NULL)//得到分母
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
				ylpt2->ylpec=(ylpt2->z-z)/sum;//比它低的点分配给它的油量，它暂时记着。
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
			AfxMessageBox("油量文件行列值不匹配");
	 		exit(0);
	}
	double lsd; //modify by xxzhu 20170317
	double lsf;
	double min;		//notice:刚读进来的最大最小值是定的。但是不是有继承之后最大最小值就变了呢？
	double max;
	//fscanf(zfp,"%d%f%d%f%f%f",&lsd,&lsf,&lsd,&lsf,&min,&max);
	fscanf(zfp, "%lf%lf", &lsd, &lsf); //数据来源发生异常，在本来应该写整数的地方写了double导致读取错误
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
			if ((tValue>=min && tValue<=max) && m_XYZ.flag[k]==0)      //如果确定是油源,就在这里建链表并且按高低排序 0代表有效质（在边界里面）
			{
				//在生油表中,所以标志为1
				if (tValue>0)			//如果油量是负值当0计算
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
						
						//这一段是按照从低到高排序，好像没有必要
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
				if (tValue<min || tValue>max)//如果超出范围，假设其为无效值，看用户需求这里做修改
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

	//初始化，首先任何点都无效（flag＝2），并且油量刚开始为(yl=0)
	//第二，在边界里面的点都有效为（flag＝0）
	//第三，在生有点处，（首先判断是否有效点，如果是，则设置flag＝1）

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
			m_XYZ.zeroflag[k]=1;//刚开始都不要设置成0。
			m_YLT[k].firsthjd=NULL;
			m_YLT[k].yl=0;
			if (fpbj!=NULL)	//如果有边界，先
			m_XYZ.flag[k]=2;//初始化,所有的点都不是生油点,所有点都无效.
			else		//如果没有边界，全部点是否有效值通过类中的最大最小值来判断，超出范围为无效，否则都是有效点,到后面再去掉油量大于最大值的。
			{				
				if (tValue>=min && tValue<=max)	//无边界，但没超出范围。
				{
					m_XYZ.flag[k]=0;
				}
				else			//无边界，但超出范围
				m_XYZ.flag[k]=2;
			}
			m_XYZ.yl[k]=0;
			if (i==0)
			m_XYZ.x[j] =j;
		}
	}

	//设置有效点为flag为0,那么设置生油点时做一个判断,如果是有效点那么再把它设置为1(是生油点)
	
	char ys[6],xs[6];
	int y1,x1,y2,x2;
	int loc1,loc2;
	
	if (fpbj!=NULL)             //有边界文件，
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
			if (y1==y2 && y1<m_row)//如果是成对的组成边界
			{
				for(i=x1;i<=x2;i++)
				{
					if (i<m_col)    //没有超出范围
					{
					
						if (m_XYZ.z[y1*m_col+x1]*(-1)>=min && m_XYZ.z[y1*m_col+x1]*(-1)<=max)
						{
							k=y1*col+i;
							m_XYZ.flag[k]=0;//0为有效值
						}
					}
				}
			}
			else//如果不成对， 或者已出边界
			{
				if (y1!=y2 && y1<m_row)  //不成对，并且没有出界
				{
					if (x1<m_col)  //并且x 没有出界
					{
						if (m_XYZ.z[y1*m_col+x1]*(-1)>=min && m_XYZ.z[y1*m_col+x1]*(-1)<=max)
						{
							k=y1*col+x1;
							m_XYZ.flag[k]=0;//第一个有效,
						}
						loc2=ftell(fpbj);
						fseek(fpbj,loc1-loc2,SEEK_CUR);//指针后退一个字符串;
					}
				}
				if (y1!=y2 && y1>=m_row)	//因为y是增大排列的，所以y2肯定也无效
				{
					//说明两个都是无效值
				}
				if (y1==y2 && y1>=m_row)
				{
					//说明两个都是无效值
				}
			}
		}//end while
		
		fclose(fpbj);
	}//end if fpbj!=NULL

}
struct SybStr* symnclass::compls2(DOT dot)//找一个点周围比它高的点
{
		//x是x轴坐标，y是y轴坐标(x,y从0开始)。找周围比它高的点，返回队列

		float lz,lyl;//暂存（x,y对应的z,yl的值）
		int flag;
		int x=dot.x;
		int y=dot.y;
		float z=dot.z;
		float yl=dot.yl;
		SybStrShX *head;
		SybStrShX *p,*q;
		
		float sum;//用来计算油亮的分配。sum是高度和，每个高度的比例用来分配油亮
		head=NULL;
		q=head;
		int lcount=0;
		//周围的点
		if (x-1>=0)//左边
		{

				if (y+1<m_row)
				{
					lz=m_XYZ.z[(y+1)*m_col+(x-1)];
					lyl=m_XYZ.yl[(y+1)*m_col+(x-1)];
					flag=m_XYZ.flag[(y+1)*m_col+(x-1)];
					if (lz>z && flag==1)//是生油点（1）
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
				}//左下脚
				//处理左边
					lz=m_XYZ.z[y*m_col+(x-1)];
					lyl=m_XYZ.yl[y*m_col+(x-1)];
					flag=m_XYZ.flag[y*m_col+(x-1)];
					if (lz>z && flag==1)//是生油点（1）
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
					}//左边结束
				if (y-1>=0)//左上脚
				{
					lz=m_XYZ.z[(y-1)*m_col+(x-1)];
					lyl=m_XYZ.yl[(y-1)*m_col+(x-1)];
					flag=m_XYZ.flag[(y-1)*m_col+(x-1)];
					if (lz>z && flag==1)//是生油点（1）
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
					
				}//左上脚

		}//左边结束、
		if (y-1>=0)//上边
		{
			
						//处理正上
			lz=m_XYZ.z[(y-1)*m_col+x];
			lyl=m_XYZ.yl[(y-1)*m_col+x];
			flag=m_XYZ.flag[(y-1)*m_col+x];
			if (lz>z && flag==1)//是生油点（1）
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
			}//结束
			if (x+1<m_col)//右上脚
			{
					lz=m_XYZ.z[(y-1)*m_col+(x+1)];
					lyl=m_XYZ.yl[(y-1)*m_col+(x+1)];
					flag=m_XYZ.flag[(y-1)*m_col+(x+1)];
					if (lz>z && flag==1)//是生油点（1）
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
			}//结束
		}//上边结束
		if (x+1<m_col)	//处理正右
		{
					lz=m_XYZ.z[y*m_col+(x+1)];
					lyl=m_XYZ.yl[y*m_col+(x+1)];
					flag=m_XYZ.flag[y*m_col+(x+1)];
					if (lz>z && flag==1)//是生油点（1）
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
		}//右边结束
		if (y+1<m_row)//下边
		{
			if (x+1<m_col)
			{
					lz=m_XYZ.z[(y+1)*m_col+(x+1)];
					lyl=m_XYZ.yl[(y+1)*m_col+(x+1)];
					flag=m_XYZ.flag[(y+1)*m_col+(x+1)];
					if (lz>z && flag==1)//是生油点（1）
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
			}//处理右下脚
			//处理正下
					lz=m_XYZ.z[(y+1)*m_col+x];
					lyl=m_XYZ.yl[(y+1)*m_col+x];
					flag=m_XYZ.flag[(y+1)*m_col+x];
					if (lz>z && flag==1)//是生油点（1）
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

		};//下边结束
		
//以上顺序按照:从左下脚开始,顺时针旋转得到队列,应该是连续排列的.
//目前p与q都指向最后一个指针,head指向头指针
	if (head!=NULL)
	{
		SybStrShX *tail;
		tail=p;//记录双向鏈表的尾记录
		SybStrShX *pprev;
		pprev=tail;//把pprev始终在p后
//下面考虑连续点的最大值问题
		q=head;//q指向头
		SybStrShX *qnext;
		qnext=head;//qnext始终在q前
		DOT lldot;//这个变量用来记录如果连续，z最大的那个点。
		lldot.x=q->x;
		lldot.y=q->y;
		lldot.z=q->z;
		lldot.flag=q->flag;//给lldot赋初值
		lldot.yl=q->yl;
		SybStr *head2;//准备新建一个队列存放处理后的数据系列,单向的即可
		SybStr *p2,*q2;
		head2=NULL;
		p2=head2;
		q2=head2;
	if (head->next==NULL)//只有一个节点,下面就不要做了
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
	//若有>=2个节点
	if (lcount==2)
	{
		if (abs(p->x-q->x)+abs(p->y-q->y)==1)	//只有两个点，但首尾相连
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
			//两个点，首尾相连，只能选择一个
									
		}
		else//两个点，首尾不相连，有两个点
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
			head2=p2;	//第一个点
						
			p2=new SybStr;
			p2->x=p->x;
			p2->y=p->y;
			p2->z=p->z;
			p2->flag=p->flag;
			p2->yl=p->yl+((p->z-z)/sum)*yl;
			p2->next=NULL;//第二个点
			head2->next=p2;//第一个点与第二个点连接
			
		}
		return head2;

	}	//两个点的情况处理完毕
	//下面是处理》2个点的情况
	if (abs(p->x-q->x)+abs(p->y-q->y)==1)//说明首尾连续
	{
		//如果连续，则head要往后找，tail要向前找，看是否还有连续的。
		if (p->z>q->z)//先首尾比较
		{
			lldot.x=p->x;
			lldot.y=p->y;
			lldot.z=p->z;
			lldot.flag=p->flag;
			lldot.yl=p->yl;
		}
		while (p!=head)//从尾部开始找连续的，若连续，继续找最大的高度，直到不连续跳出
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
				else	//如果从后再向前一个，连续但高度小
				{
					if (p!=head)//并且没有到达头，继续向前
					{
					pprev=pprev->prev;
					p=p->prev;
					}
							
				}
			}
			else
			break;//不连续跳出，
		}// end while p->prev!=head
		while (q!=p)//从头部开始找连续的，若连续，继续找，不连续存放
		{
			q=q->next;//qnext 在q的前面
			if (abs(qnext->x-q->x)+abs(qnext->y-q->y)==1)//若连续
			{
				if (q->z>lldot.z)//若连续并且大于
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
				else//若连续但小于
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
			else//若不连续
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
						q2=p2;	//把连续的最大值保存
				
				lldot.x=q->x;//从不连续的地方继续寻找相对最高点，把断了的点再作为初始值
				lldot.y=q->y;
				lldot.z=q->z;
				lldot.flag=q->flag;
				lldot.yl=q->yl;
			}//end if else
			//if (q->next!=p)
			//qnext=qnext->next;
		}//end while (q!=p)
	}
	else//说明首尾不连续
	{
		while (q!=NULL)	
		{
			q=q->next;
			if (q==NULL)
			break;
			if (abs(qnext->x-q->x)+abs(qnext->y-q->y)==1)//说明连续
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
				}//若不大于
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
			else//不连续
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
				
						lldot.x=q->x;//记录不连续点
						lldot.y=q->y;
						lldot.z=q->z;
						lldot.flag=q->flag;
						lldot.yl=q->yl;
				
			}//end else
		//	if (q->next!=NULL)
		//	qnext=qnext->next;

		}//end while
			
			
	}//end else
	
	//下面处理油量的分配
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
	//把第二层的数据油量增量加上m_XYZ中的数量，产生一个新的油量文件
	//问题：如果有增量，如果无增量，该怎么多待
}

void symnclass::dealdc(CString FileName,XYZ dm_XYZ,int dm_col,int dm_row,int colmin,float colmax,int rowmin,float rowmax,int turn)
{
		
		//读断层数据，（封闭的或者不封闭的）
	//turn＝1是封闭式断层
	//turn＝2是开放式断层
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
	int all_duanceng;  //所有断层数
	int point_count;    //每一层的点数
	bj_data.readline();//读无效数据wmp123
	bj_data.readline();//读断层数
	success=sscanf(bj_data.m_pBufferLine,"%d",&all_duanceng);
	ASSERT(success==1);

/*	if(!bj_data.writefile("C:\\WINDOWS\\Desktop\\data\\new5.25\\bj\\bjGtb1dcb.txt"))
	 {
		   AfxMessageBox("File could not be opened.");
		   return;
	 }*/

	for(int idc=1;idc<=all_duanceng;idc++)
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
		int minxx,minyy,maxxx,maxyy;//得到line_count=0的外区域
		if (point_count>0)
		{
			
			for(int j=0;j<bj_data.maxver;j++)
			{
				bj_data.readword();
				success=sscanf(bj_data.m_pBufferWord,"%f",&tempf);
				ASSERT(success==1);
				bj_data.vertex.x[j]=(int)(dm_col*(tempf-colmin)/(colmax-colmin)+0.5f);//网格上的x
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
				bj_data.vertex.y[j]=(int)(dm_row*(tempf-rowmin)/(rowmax-rowmin)+0.5f);//网格上的y
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
		
		//指针数组，每个元素是指向struct edge_info的结构

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
							if (xj==minxx || yj==minyy ||xj==maxxx || yj==maxyy)//代表是边界线
							{
								if (turn==1)	//代表是封闭断层
								{
									if (m_XYZ.flag[yj*m_col+xj]!=30 || m_XYZ.flag[yj*m_col+xj] !=40)
									m_XYZ.flag[yj*m_col+xj]=3;
									//fprintf(gp,"\n%d,%d",yj,xj);
									//fprintf(gp,"%c",'\n');
									//fprintf(gp,"%s%s","##");
									//fclose(gp);
								}							
								else			//代表是开放式断层
								{
									if (m_XYZ.flag[yj*m_col+xj]!=30  || m_XYZ.flag[yj*m_col+xj] !=40)
									m_XYZ.flag[yj*m_col+xj]=4;
								}
							}
							else			//代表边界线包围的部分
							{
								if (turn==1)		//代表是封闭断层
								m_XYZ.flag[yj*m_col+xj]=30;
								else			//代表是开放式断层
								m_XYZ.flag[yj*m_col+xj]=40;
							}
					}	
			}
		   int i=0;
	       int x,y;
		   while(i<(bj_data.maxver-1))
		   {
				if (turn==1)		//代表是封闭断层
					if (m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]!=30 || m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]!=40)
					m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]=3;
				else
					if (m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]!=30 || m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]!=40)//代表是开放式断层
					m_XYZ.flag[bj_data.vertex.y[i]*m_col+bj_data.vertex.x[i]]=4;
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
						    if (turn==1)		//代表是封闭断层
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=3;
							else			//代表是开放式断层
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=4;
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
						   	if (turn==1)		//代表是封闭断层
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=3;
							else			//代表是开放式断层
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=4;
                           j++;
					   }					   
				   }
			   }
			   if((bj_data.vertex.y[i]!=bj_data.vertex.y[i+1])//处理斜直线
                &&(bj_data.vertex.x[i]!=bj_data.vertex.x[i+1]))
               {
				   int margin=bj_data.vertex.y[i]-bj_data.vertex.y[i+1];
				   if(abs(margin)>1)//非相邻行处理
				   {
					   int yl;
					   int j=1;
					   int k;
					   k=(bj_data.vertex.y[i+1]-bj_data.vertex.y[i])/(bj_data.vertex.x[i+1]-bj_data.vertex.x[i]);
					   while(j<abs(margin))
					   {
						   if(margin>0) yl=bj_data.vertex.y[i]-j;
						   else yl=bj_data.vertex.y[i]+j;
						   ///求斜线与扫描行yl的交点，再近似到网格点上
                           x=(int)((bj_data.vertex.x[i]*(yl-bj_data.vertex.y[i+1])-bj_data.vertex.x[i+1]*(yl-bj_data.vertex.y[i]))/(margin)+0.5f);
				           y=yl;
						   	if (turn==1)		//代表是封闭断层
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=3;
							else			//代表是开放式断层
								if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
								m_XYZ.flag[y*m_col+x]=4;
							if (k>0)  //右
							{
								if (margin>0) //右上
								{
									//处理两个对角点	
									if (turn==1)		//代表是封闭断层
										m_XYZ.flag[y*m_col+x-1]=30;
									else			//代表是开放式断层
										m_XYZ.flag[y*m_col+x-1]=40;
									if (turn==1)		//代表是封闭断层
										m_XYZ.flag[(y+1)*m_col+x]=30;
									else			//代表是开放式断层
										m_XYZ.flag[(y+1)*m_col+x]=40;	
								}
								else	//右下
								{
									if (turn==1)		//代表是封闭断层
									m_XYZ.flag[yl*m_col+x-1]=30;
									else			//代表是开放式断层
									m_XYZ.flag[yl*m_col+x-1]=40;
									if (turn==1)		//代表是封闭断层
									m_XYZ.flag[(yl-1)*m_col+x]=30;
									else			//代表是开放式断层
									m_XYZ.flag[(yl-1)*m_col+x]=40;	
								}


							}
							else  //左
							{
								if (margin>0) //左上
								{
									if (turn==1)		//代表是封闭断层
									m_XYZ.flag[yl*m_col+x+1]=30;
									else			//代表是开放式断层
									m_XYZ.flag[yl*m_col+x+1]=40;
									if (turn==1)		//代表是封闭断层
									m_XYZ.flag[(yl+1)*m_col+x]=30;
									else			//代表是开放式断层
									m_XYZ.flag[(yl+1)*m_col+x]=40;	
								}
								else	//左下
								{
									if (turn==1)		//代表是封闭断层
									m_XYZ.flag[yl*m_col+x+1]=30;
									else			//代表是开放式断层
									m_XYZ.flag[yl*m_col+x+1]=40;
									if (turn==1)		//代表是封闭断层
									m_XYZ.flag[(yl-1)*m_col+x]=30;
									else			//代表是开放式断层
									m_XYZ.flag[(yl-1)*m_col+x]=40;	
								}
							}


						   j++;
					   }
				   }
			   }
			i++;
		   }
		   ///写最后一个点
		   if (bj_data.maxver!=0)
		   {
			   x=bj_data.vertex.x[bj_data.maxver-1];
			   y=bj_data.vertex.y[bj_data.maxver-1];
			   if (turn==1)		//代表是封闭断层
					if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
					m_XYZ.flag[y*m_col+x]=3;
			   else			//代表是开放式断层
				   if (m_XYZ.flag[y*m_col+x]!=30 || m_XYZ.flag[y*m_col+x]!=40)
					m_XYZ.flag[y*m_col+x]=4;

		   }



		}  			   
		
		bj_data.readline();//这一层最后一个点是无效点要去掉
		delete[] bj_data.vertex.x;
		delete[] bj_data.vertex.y;
	}
//	bj_data.close_file();
	
		
}