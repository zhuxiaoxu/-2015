// TimedProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimedProgress.h"
#include "TimedProgressDlg.h"
#include "Ana_file.h"
#include "symnclass.h"
#include "FaultageProcess.h"
#include "LogClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
int countcc1=0;
int countcc2=0;
int sjglobe;
int sdglobe;
bool jsflag=false;
bool zchang=true;

threadInfo Info;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
    CAboutDlg();

// Dialog Data
    //{{AFX_DATA(CAboutDlg)
    enum { IDD = IDD_ABOUTBOX };
    //}}AFX_DATA

    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    //{{AFX_MSG(CAboutDlg)
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
    //{{AFX_DATA_INIT(CAboutDlg)
    //}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDlg)
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
    //{{AFX_MSG_MAP(CAboutDlg)
        // No message handlers
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimedProgressDlg dialog

CTimedProgressDlg::CTimedProgressDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CTimedProgressDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CTimedProgressDlg)
	//}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTimedProgressDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTimedProgressDlg)
//	DDX_Control(pDX, IDC_SPEED, m_slider);
//	DDX_Control(pDX, IDC_GO, m_btnGo);
//	DDX_Control(pDX, IDC_STOP, m_btnStop);
	//DDX_Control(pDX, IDC_PROGRESS8, m_progress8);
	//DDX_Control(pDX, IDC_PROGRESS7, m_progress7);
	//DDX_Control(pDX, IDC_PROGRESS6, m_progress6);
	//DDX_Control(pDX, IDC_PROGRESS5, m_progress5);
	DDX_Control(pDX, IDC_PROGRESS4, m_progress4);
	DDX_Control(pDX, IDC_PROGRESS3, m_progress3);
	//DDX_Control(pDX, IDC_PROGRESS2, m_progress2);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTimedProgressDlg, CDialog)
    //{{AFX_MSG_MAP(CTimedProgressDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimedProgressDlg message handlers

BOOL Process(LPVOID lpParam)
{
	threadInfo* pInfo = (threadInfo*)lpParam;
	
	CString sDir = famc;//得到方案文件名称
	int length = sDir.ReverseFind('\\'); //找最后一个斜杠
	//CString dirpath;	//xxzhu 发现该变量后续并未使用，因此注释，20170314
	//dirpath=sDir.Left(length+1); //得到方案的路径部分,发现该变量后续并未使用，因此注释，20170314
	CString fanganhao;
	fanganhao = sDir.Right(sDir.GetLength() - length - 1);
	fanganhao = fanganhao.Left(fanganhao.Find('.')); //得到方案文件的文件名作为方案号
	TRACE("%s", fanganhao);
	/*char logFile[1024];
	strcpy(logFile, globalDatPath);
	strcat(logFile, "\\log.txt");*/
	CLogClass  loger; //日志文件

	FILE *fp1, *fp2, *fp3, *fp4;
	struct DOT dot;
	symnclass  symnclasslz;
	int pos;//该变量记录bdepb1.grd的有效数字从什么地方开始
	XYZ tm_XYZ;//用来存放开放断层点的油

			   //首先打开方案文件
	LPTSTR p = sDir.GetBuffer(sDir.GetLength());
	CAna_file CFangan; //构造方案文件对象
	CFangan.readfile(p); //把文件内容一次性全部读取到类内部指针m_pBuffer所申请的内存中
	sDir.ReleaseBuffer(); //add by xxzhu 20170324
	CString lpath;
	int kscs = 0;
	char tempbuff[128]; //用于logout的int转字符串 xxzhu 20170328
	loger.LogOut("打开方案文件成功");
	//之前的代码重复调用fasx，第一效率低下，第二，内部方案号和时间段数内存泄漏
	int ret = CFangan.fasx();//读取方案信息
	if (ret != 1)
	{
		zchang = false;
		loger.LogOut("读方案信息出错，方案文件头不正确，程序退出运行");
		AfxMessageBox("读方案信息出错，方案文件头不正确");
		//没法干活了
		exit(0);//因为当前是线程，用此法结束整个程序
		return 0;
	}
	else //读取方案信息正确
		//if (CFangan.fasx()==1)//得到时间段数，这是最外重循环
	{
		loger.LogOut("得到时间段数", CFangan.sjds);
		int local_sjd = atoi(CFangan.sjds);
		pInfo->pctrlProgress1->SetRange(0, local_sjd); //时间段的进度条
		pInfo->pctrlProgress1->SetStep(1);//步长为1
		for (int sjd = 1; sjd <= local_sjd; sjd++)// for 1 xxzhu ，利用时间段循环
		{
			
			loger.LogOut("开始处理时间段", _itoa(sjd, tempbuff, 10));
			
			if (kscs > 1)
			{
				delete[] tm_XYZ.x;
				delete[] tm_XYZ.y;
				delete[] tm_XYZ.yl;
				delete[] tm_XYZ.z;
				delete[] tm_XYZ.flag;
			}
			kscs = 0;//因为顾客可能第一到某层不进行计算，
					 //所以在每个时间段内部的层与层之间的油量继承
					 //一定要从开始计算路径后的时候才计算

			if (CFangan.xinxi_a(sjd) == 1)//得到该时间段的深度层次（内循环循环） //（循环次数=段数范围*深度层次）							 
			{
				pInfo->pctrlProgress3->SetRange(0, atoi(CFangan.sdcs)); //时间段的进度条
				pInfo->pctrlProgress3->SetStep(1);
				loger.LogOut("共有深度层数数量", CFangan.sdcs);
				for (int sdcs = 1; sdcs <= atoi(CFangan.sdcs); sdcs++) //for 2
				{
					loger.LogOut("开始处理深度层", _itoa(sdcs,tempbuff,10));
					ret = CFangan.xinxi_a_a(sjd, sdcs); //读取指定时间段的指定层信息到CFangan的数据成员ceng_xx
					if (ret != 1)
					{
						zchang = false;
						//char tempbuff[128];
						sprintf(tempbuff, "读时间段%d的%d层方案信息出错！", sjd, sdcs);// add by xxzhu 提示精确化
						AfxMessageBox(tempbuff);
						loger.LogOut(tempbuff);
						exit(0);
						return 0;
					}
					else
						//if (CFangan.xinxi_a_a(sjd,sdcs)==1) //读方案信息出错信息
					{
						sprintf(tempbuff, "读时间段%d的%d层方案信息成功！", sjd, sdcs);
						loger.LogOut(tempbuff);
					
						//得到个深度文件
						if (CFangan.ceng.amount == NULL || CFangan.ceng.lj == NULL)
						{
							loger.LogOut("增量为空或者路径为空，无需处理，准备跳下一层");
							pInfo->pctrlProgress3->StepIt(); 
							continue;//说明该层路径不用计算。所以跳到下一层
									 //	zchang=false;
									 //	AfxMessageBox("");
									 //	return 0;
						}
						kscs++;
						if (CFangan.ceng.lj != NULL)		//如果 如果虚拟的路径存在，说明要计算路径
						{
							if (CFangan.ceng.sd == NULL)
							{
								sprintf(tempbuff, "读时间段%d的%d层，缺少深度文件，计算停止并返回。", sjd, sdcs);
								loger.LogOut(tempbuff);
								//	CTimedProgressDlg::CloseWindow();
								zchang = false;
								AfxMessageBox("缺少深度文件");
								exit(0);
								return 0;
							}
							else
							{
								//========================================
								//add by xxzhu 20170310
								char temp[1024];
								strcpy_s(temp,1024, globalDatPath);
								if (CFangan.ceng.sd[0] != '\\')
									strcat_s(temp, "\\");

								strcat_s(temp, CFangan.ceng.sd);

								//========================================

								if ((fp1 = fopen(temp, "r")) == NULL)
								{
									//	CTimedProgressDlg::CloseWindow();
									zchang = false;
									loger.LogOut("打开深度文件出错！", temp);
									AfxMessageBox("打开深度文件出错！" + CString(CFangan.ceng.sd));
									exit(0);
									return 0;
								}
							}
							//xxzhu 20170325 14:19
							//读出该层次有多少行多少列
							fseek(fp1, 5, SEEK_SET);//最前面是 DSAA 和 换行， 因此跳过五个
							fscanf(fp1, "%d", &symnclasslz.m_col);
							fscanf(fp1, "%d", &symnclasslz.m_row);
							sprintf(tempbuff, "该层共有%d列%d行", symnclasslz.m_col, symnclasslz.m_row);
							loger.LogOut(tempbuff);
							if (sjd == 1 && sdcs == 1)
							{//第一时间段、第一深度层次
								symnclasslz.whole_m_col = symnclasslz.m_col;
								symnclasslz.whole_m_row = symnclasslz.m_row;
							}
							if (symnclasslz.whole_m_col != symnclasslz.m_col || symnclasslz.whole_m_row != symnclasslz.m_row)
							{
								zchang = false;
								AfxMessageBox("数据文件行列值不匹配");
								loger.LogOut("数据文件行列值和该时间段第一层不匹配，数据异常，即将退出");
								exit(0);
								return 0;
							}
							symnclasslz.m_XYZ.x = new int[symnclasslz.m_col];
							symnclasslz.m_XYZ.y = new int[symnclasslz.m_row];
							symnclasslz.m_XYZ.z = new float[symnclasslz.m_col*symnclasslz.m_row];
							symnclasslz.m_XYZ.flag = new int[symnclasslz.m_col*symnclasslz.m_row];
							symnclasslz.m_XYZ.yl = new float[symnclasslz.m_col*symnclasslz.m_row];
							symnclasslz.m_XYZ.ylpec = new float[symnclasslz.m_col*symnclasslz.m_row];
							symnclasslz.m_XYZ.zeroflag = new int[symnclasslz.m_col*symnclasslz.m_row];
							symnclasslz.m_YLT = new struct hjdtable[symnclasslz.m_col*symnclasslz.m_row];
							//申请了一大段内存用于数据处理

							int colmin;//列最小值
							float colmax;//列最大值
							int rowmin;//行最小值
							float rowmax;//行最大值
							char *ssjd;
							char *ssdcs;
							ssjd = new char[8];//此处本来是2，不可靠，不能处理略大的数字
							ssdcs = new char[8];//此处本来是2，不可靠，不能处理略大的数字

							fscanf(fp1, "%d%f%d%f%f%f", &colmin, &colmax, &rowmin, &rowmax, &symnclasslz.min, &symnclasslz.max);
							pos = ftell(fp1);
							//lpath=dirpath;//取得创建文件的路径
							_itoa(sjd, ssjd, 10);//时间段转字符串
							_itoa(sdcs, ssdcs, 10);//深度层次转字符串
							//lpath=lpath+"data"+ssjd+"-"+ssdcs+".txt";
							lpath = CFangan.ceng.lj;
							//========================================
							//add by xxzhu 20170310
							char temp[1024];
							strcpy(temp, globalDatPath);
							if (lpath[0] != '\\')
								strcat(temp, "\\");
							strcat(temp, lpath);
							//========================================

							if ((fp2 = fopen(temp, "w")) == NULL)
							{
								zchang = false;
								AfxMessageBox("创建写路径文件出错" + lpath);
								return 0;
							}
							loger.LogOut("以写方式打开文本文件：",temp);
							//读出边界

							if (CFangan.ceng.bj != NULL)
							{
								//========================================
								//add by xxzhu 20170310
								//char temp[1024];
								strcpy(temp, globalDatPath);
								if (CFangan.ceng.bj_created[0] != '\\')
									strcat(temp, "\\");
								strcat(temp, CFangan.ceng.bj_created);
								//========================================
								if ((fp3 = fopen(temp, "r")) == NULL)
								{
									zchang = false;
									AfxMessageBox("打开边界文件出错" + CString(CFangan.ceng.bj_created));
									loger.LogOut("打开边界文件出错：", temp);
									exit(0);
									return 0;
								}
							}
							loger.LogOut("打开边界文件：", temp);
							if (CFangan.ceng.bj == NULL)
							{
								fp3 = NULL;
							}
							fseek(fp1, pos, SEEK_SET);

							symnclasslz.loaddata(fp1, fp3);
							FILE *foptmyl;//打开临时的油量表
							char *tsjd;
							//打开排油量文件
							FILE *fyshyl;//生成原始油量文件

							if (CFangan.ceng.pyl == NULL && sjd == 1 && kscs == 1)
							{
								zchang = false;
								AfxMessageBox("开始时间段的开始层缺少油量文件");
								return 0;
							}
							else
							{
								symnclasslz.sybpointer = NULL;
								if (CFangan.ceng.pyl != NULL)//如果有排油量
								{
									//========================================
									//add by xxzhu 20170310
									char temp[1024];
									strcpy(temp, globalDatPath);
									if (CFangan.ceng.pyl[0] != '\\')
										strcat(temp, "\\");
									strcat(temp, CFangan.ceng.pyl);
									//========================================

									if ((fp4 = fopen(temp, "r")) == NULL)
									{
										zchang = false;
										AfxMessageBox("不能打开排油量文件");
										loger.LogOut("不能打开油量文件：", temp);
										return 0;
									}
									loger.LogOut("打开油量文件：", temp);

									CString cyshyl;
									cyshyl = CFangan.ceng.pyl;
									cyshyl = cyshyl.Left(cyshyl.ReverseFind('.'));
									TRACE(cyshyl + '\n');

									//========================================
									//add by xxzhu 20170310
									//char temp[1024];
									strcpy(temp, globalDatPath);
									if (cyshyl[0] != '\\')
										strcat(temp, "\\");
									strcat(temp, cyshyl);
									//========================================

									fyshyl = fopen(temp, "w");
									loger.LogOut("写文本方式打开文件：", temp);
									if (sjd == 1)//如果是时间段1
									{
										symnclasslz.loadsyb(fp4, fyshyl);
										//加载输油表？
									}
									else//大于时间1，并且有油的增量
									{
										symnclasslz.loadsyb(fp4, fyshyl);
										//加载输油表？次代码貌似和if中一样。
										//xxzhu 20170325 14:55
										tsjd = new char[8]; // 原来是2，安全起见修改为8
										lpath = CFangan.ceng.amount;
										lpath = lpath.Left(lpath.ReverseFind('\\') + 1);
										lpath = lpath + fanganhao + "_" + _itoa(sjd - 1, tsjd, 10) + "_" + ssdcs + "_amount.txt";
										TRACE("%s", lpath);
										//lpath=dirpath+"temp\\oil"+_itoa(sjd-1,tsjd,10)+"-"+ssdcs+".txt";
										//========================================
										//add by xxzhu 20170310
										char temp[1024];
										strcpy(temp, globalDatPath);
										if (lpath[0] != '\\')
											strcat(temp, "\\");
										strcat(temp, lpath);
										//========================================

										if ((foptmyl = fopen(temp, "r")) != NULL)
										{
											loger.LogOut("读文本方式打开文件：", temp);
											while (!feof(foptmyl))
											{
												int ylx, yly, ylflag;
												float yll;
												fscanf(foptmyl, "%d,%d,%f,%d", &ylx, &yly, &yll, &ylflag);
												if (yll > 0)
												{

													if (ylflag != 4 && symnclasslz.m_XYZ.flag[yly*symnclasslz.m_col + ylx] != 2)
													{
														//symnclasslz.m_XYZ.yl[yly*symnclasslz.m_col+ylx]=yll;
														symnclasslz.m_XYZ.flag[yly*symnclasslz.m_col + ylx] = 1;
														symnclasslz.m_XYZ.yl[yly*symnclasslz.m_col + ylx] = symnclasslz.m_XYZ.yl[yly*symnclasslz.m_col + ylx] + yll;
														symnclasslz.m_YLT[yly*symnclasslz.m_col + ylx].yl = symnclasslz.m_YLT[yly*symnclasslz.m_col + ylx].yl + yll;
														SybStr *p;
														p = new SybStr;
														p->x = ylx;
														p->y = yly;
														p->z = symnclasslz.m_XYZ.z[yly*symnclasslz.m_col + ylx];
														p->yl = symnclasslz.m_YLT[yly*symnclasslz.m_col + ylx].yl;
														p->ylpec = 0.00;
														p->flag = 1;
														p->zeroflag = symnclasslz.m_XYZ.zeroflag[yly*symnclasslz.m_col + ylx];
														if (symnclasslz.sybpointer == NULL)
														{
															symnclasslz.sybpointer = p;
															p->next = NULL;

														}
														else
														{
															p->next = symnclasslz.sybpointer->next;
															symnclasslz.sybpointer->next = p;
														}

													}
												}
											}
											fclose(foptmyl);
										}


									}
								}
								else//如果没有排油量，但时间大于2
								{
									if (sjd > 1)		//从前面一个时间段继承
									{

										tsjd = new char[2];
										lpath = CFangan.ceng.amount;
										lpath = lpath.Left(lpath.ReverseFind('\\') + 1);
										lpath = lpath + fanganhao + "_" + _itoa(sjd - 1, tsjd, 10) + "_" + ssdcs + "_amount.txt";
										TRACE("%s", lpath);
										//lpath=dirpath+"temp\\oil"+_itoa(sjd-1,tsjd,10)+"-"+ssdcs+".txt";
										//========================================
										//add by xxzhu 20170310
										char temp[1024];
										strcpy(temp, globalDatPath);
										if (lpath[0] != '\\')
											strcat(temp, "\\");
										strcat(temp, lpath);
										//========================================

										if ((foptmyl = fopen(temp, "r")) != NULL)
										{
											while (!feof(foptmyl))
											{
												int ylx, yly, ylflag;
												float yll;
												fscanf(foptmyl, "%d,%d,%f,%d", &ylx, &yly, &yll, &ylflag);
												if (yll > 0)
												{
													//if (symnclasslz.m_XYZ.flag[yly*symnclasslz.m_col+ylx]!=2 && symnclasslz.m_XYZ.flag[yly*symnclasslz.m_col+ylx]!=4)//该时间的这层的这个点是生油点，但有效,但不是生油点
													if (ylflag != 4 && symnclasslz.m_XYZ.flag[yly*symnclasslz.m_col + ylx] != 2)
													{
														//symnclasslz.m_XYZ.yl[yly*symnclasslz.m_col+ylx]=yll;
														symnclasslz.m_XYZ.flag[yly*symnclasslz.m_col + ylx] = 1;
														symnclasslz.m_XYZ.yl[yly*symnclasslz.m_col + ylx] = yll;
														symnclasslz.m_YLT[yly*symnclasslz.m_col + ylx].yl = yll;
														SybStr *p;
														p = new SybStr;
														p->x = ylx;
														p->y = yly;
														p->z = symnclasslz.m_XYZ.z[yly*symnclasslz.m_col + ylx];
														p->yl = symnclasslz.m_YLT[yly*symnclasslz.m_col + ylx].yl;
														p->ylpec = 0.00;
														p->flag = 1;
														p->next = NULL;
														p->zeroflag = symnclasslz.m_XYZ.zeroflag[yly*symnclasslz.m_col + ylx];
														if (symnclasslz.sybpointer == NULL)
														{
															symnclasslz.sybpointer = p;
															p->next = NULL;

														}
														else
														{
															p->next = symnclasslz.sybpointer->next;
															symnclasslz.sybpointer->next = p;
														}

													}
												}
											}
											fclose(foptmyl);
										}
										/*	else
										{
										zchang=false;
										AfxMessageBox("该时间段既没有油量文件又无法继承，所以无法计算");
										return 0;
										}*/
									}
								}
							}

							//	if (sdcs>1)//如果是第二层以上，需要把开放断层的石油加到目前的油量中来
							if (kscs > 1)
							{

								for (int dcylcount = 0; dcylcount < symnclasslz.m_col*symnclasslz.m_row; dcylcount++)
									if (tm_XYZ.flag[dcylcount] == 4 && tm_XYZ.yl[dcylcount]>0)
									{

										if (symnclasslz.m_XYZ.flag[dcylcount] != 2)
										{
											symnclasslz.m_XYZ.yl[dcylcount] = symnclasslz.m_XYZ.yl[dcylcount] + tm_XYZ.yl[dcylcount];
											symnclasslz.m_YLT[dcylcount].yl = symnclasslz.m_YLT[dcylcount].yl + tm_XYZ.yl[dcylcount];
											symnclasslz.m_XYZ.flag[dcylcount] = 1;
											SybStr *p;
											p = new SybStr;
											p->x = dcylcount%symnclasslz.m_col;
											p->y = dcylcount / symnclasslz.m_col;
											p->z = symnclasslz.m_XYZ.z[dcylcount];
											p->yl = symnclasslz.m_YLT[dcylcount].yl;
											p->ylpec = 0.00;
											p->flag = 1;
											p->zeroflag = symnclasslz.m_XYZ.zeroflag[dcylcount];
											p->next = NULL;
											if (symnclasslz.sybpointer == NULL)
											{
												symnclasslz.sybpointer = p;
												p->next = NULL;

											}
											else
											{
												p->next = symnclasslz.sybpointer->next;
												symnclasslz.sybpointer->next = p;
											}


										}
									}
								delete[] tm_XYZ.x;
								delete[] tm_XYZ.y;
								delete[] tm_XYZ.yl;
								delete[] tm_XYZ.z;
								delete[] tm_XYZ.flag;

							}
							if (symnclasslz.sybpointer == NULL)
							{
								zchang = false;
								AfxMessageBox("该时间段既没有油量文件又无法继承，所以无法计算");
								loger.LogOut("该时间段既没有油量文件又无法继承，所以无法计算");
								exit(0);
								return 0;
							}

							/*这里打印证明数据，主要是继承油量的打印*/
/*rem by xxzhu 无需证明 20170325
							FILE *Fjcyl;
							tsjd = new char[2];
							lpath = CFangan.ceng.amount;
							lpath = lpath.Left(lpath.ReverseFind('\\') + 1);
							lpath = lpath + fanganhao + "_" + _itoa(sjd, tsjd, 10) + "_" + ssdcs + "_jcyl.txt";
							TRACE("%s", lpath);

							//========================================
							//add by xxzhu 20170310
							//char temp[1024];
							strcpy(temp, globalDatPath);
							if (lpath[0] != '\\')
								strcat(temp, "\\");
							strcat(temp, lpath);
							//========================================
							loger.LogOut("写文本方式打开文件：", temp);
							if ((Fjcyl = fopen(temp, "w")) != NULL)
							{
								for (int i = 0; i < symnclasslz.m_row; i++)
								{
									for (int j = 0; j < symnclasslz.m_col; j++)
									{
										if (j == 23 && i == 18)
											TRACE("hello");
										//fprintf(Fjcyl,"%d,%d,%f\n",j,i,symnclasslz.m_XYZ.yl[i*symnclasslz.m_col+j]);
										fprintf(Fjcyl, "%d,%d,%f\n", j, i, symnclasslz.m_YLT[i*symnclasslz.m_col + j].yl);
									}
								}
							}
							fclose(Fjcyl);//	
							*/
							/*if (CFangan.ceng.dc!=NULL)	//封闭断层
										  {
										  CString dcfile;
										  dcfile=CFangan.ceng.dc;
										  symnclasslz.dealdc(dcfile,symnclasslz.m_XYZ,symnclasslz.m_col,symnclasslz.m_row,colmin,colmax,rowmin,rowmax,1);//主要是改标志
										  }
										  if (CFangan.ceng.dc_open!=NULL)  //开放式断层
										  {
										  CString dcfile;
										  dcfile=CFangan.ceng.dc_open;
										  symnclasslz.dealdc(dcfile,symnclasslz.m_XYZ,symnclasslz.m_col,symnclasslz.m_row,colmin,colmax,rowmin,rowmax,2);
										  }*/
							CFaultageProcess  CFaultage(symnclasslz.m_col, symnclasslz.m_row, colmin, colmax, rowmin, rowmax);
							//产生断层处理类对象
							if (CFangan.ceng.dc != NULL)	//封闭断层
							{
								char* dcfile;
								dcfile = CFangan.ceng.dc;
								CString dcname;
								dcname = dcfile;
								dcname = dcname.Right(dcname.GetLength() - dcname.ReverseFind('\\') - 1);
								char* bj_createdpath;
								bj_createdpath = CFangan.ceng.bj_created;
								lpath = bj_createdpath;
								lpath = lpath.Left(lpath.ReverseFind('\\'));
								lpath = lpath.Left(lpath.ReverseFind('\\'));
								lpath = lpath + "\\Temp\\gdc_" + dcname;
								try
								{
									loger.LogOut("开始处理断层数据文件", dcfile);
									CFaultage.ProcessFaultage(dcfile, symnclasslz.m_XYZ, 1, lpath);
									CFaultage.ReadFaultageTempFile1(lpath, symnclasslz.m_XYZ, 1);
									loger.LogOut("断层处理是封闭断层");
								}
								catch (int flag)
								{
									sprintf(tempbuff, "断层处理时,第%d号线数据异常，程序退出", flag);
									loger.LogOut(tempbuff);
									AfxMessageBox(tempbuff);
									exit(0);
								}

							}
							if (CFangan.ceng.dc_open != NULL)  //开放式断层
							{
								char* dcfile;
								dcfile = CFangan.ceng.dc_open;
								CString dcname;
								dcname = dcfile;
								dcname = dcname.Right(dcname.GetLength() - dcname.ReverseFind('\\') - 1);
								char* bj_createdpath;
								bj_createdpath = CFangan.ceng.bj_created;
								lpath = bj_createdpath;
								lpath = lpath.Left(lpath.ReverseFind('\\'));
								lpath = lpath.Left(lpath.ReverseFind('\\'));
								lpath = lpath + "\\Temp\\gdc_" + dcname;
								try
								{
									loger.LogOut("开始处理断层数据文件", dcfile);
									CFaultage.ProcessFaultage(dcfile, symnclasslz.m_XYZ, 2, lpath);
									CFaultage.ReadFaultageTempFile1(lpath, symnclasslz.m_XYZ, 2);
									loger.LogOut("断层处理是开放式断层");
								}
								catch (int flag)
								{
									sprintf(tempbuff, "断层处理时,第%d号线数据异常，程序退出", flag);
									loger.LogOut(tempbuff);
									AfxMessageBox(tempbuff);
									exit(0);
								}
							}
							/*for(int ct=0;ct<symnclasslz.m_col*symnclasslz.m_row;ct++)
							if (symnclasslz.m_XYZ.flag[ct]==3||symnclasslz.m_XYZ.flag[ct]==30||symnclasslz.m_XYZ.flag[ct]==4||symnclasslz.m_XYZ.flag[ct]==40)
							printf("%d%d%f\n",ct%symnclasslz.m_col,ct/symnclasslz.m_col,symnclasslz.m_XYZ.z[ct]);
							exit(0);*/
							symnclasslz.gdcount = 0;
							SybStr *q;
							q = symnclasslz.sybpointer;
							loger.LogOut("开始用链表循环处理计算断层中的每个生油点");
							while (q != NULL)//对每个生油点都要考虑
							{
								dot.x = q->x;
								dot.y = q->y;
								dot.z = q->z;
								if (symnclasslz.m_XYZ.flag[dot.y*symnclasslz.m_col + dot.x] == 1)//因为断层的计算会把生油堆栈中的性质改变掉
								{
									dot.flag = 1;
									//dot.yl=symnclasslz.m_XYZ.yl[dot.y*symnclasslz.m_col+dot.x];
									dot.yl = symnclasslz.m_YLT[dot.y*symnclasslz.m_col + dot.x].yl;
									dot.ylpec = 0;
									dot.zeroflag = symnclasslz.m_XYZ.zeroflag[dot.y*symnclasslz.m_col + dot.x];
									symnclasslz.stackmn = NULL;
									symnclasslz.push(dot);
									symnclasslz.sfpop = false;
									symnclasslz.findpath(dot, fp2);
									symnclasslz.gdcount = 0;
								}
								q = q->next;
							}
							delete symnclasslz.sybpointer;
							loger.LogOut("结束用链表循环处理计算断层中的每个生油点");
							/*for(int c=0;c<symnclasslz.m_col*symnclasslz.m_row;c++)//对每个生油点都要考虑
							{
							if (symnclasslz.m_XYZ.flag[c]==1)
							{
							dot.x=c%symnclasslz.m_col;
							dot.y=c/symnclasslz.m_col;
							dot.z=symnclasslz.m_XYZ.z[c];
							dot.flag=1;
							dot.yl=symnclasslz.m_XYZ.yl[c];
							symnclasslz.stackmn=NULL;
							symnclasslz.push(dot);
							symnclasslz.sfpop=false;
							symnclasslz.findpath(dot,fp2);
							symnclasslz.gdcount=0;
							}
							}*/
							zquque *p, *qh, *q2;
							symnclasslz.m_zquque = NULL;
							qh = symnclasslz.m_zquque;
							pInfo->pctrlProgress4->SetRange(1, symnclasslz.m_col*symnclasslz.m_row/100); //时间段的进度条
							pInfo->pctrlProgress4->SetStep(1);
							sprintf(tempbuff, "进行%dX%d=%d次的运算", symnclasslz.m_col, symnclasslz.m_row, symnclasslz.m_col*symnclasslz.m_row);
							loger.LogOut(tempbuff);
							for (int c = 0; c < symnclasslz.m_col*symnclasslz.m_row; c++)//对每个生油点都要考虑
							{
								

								if (symnclasslz.m_XYZ.flag[c] != 2)
								{
									p = new zquque;
									p->vpos = c;
									p->z = symnclasslz.m_XYZ.z[c];

									qh = symnclasslz.m_zquque;

									if (symnclasslz.m_zquque == NULL)
									{
										symnclasslz.m_zquque = p;
										qh = symnclasslz.m_zquque;
										p->next = NULL;

									}
									else
									{

										while (qh->z < p->z && qh->next != NULL)
										{
											q2 = qh;
											qh = qh->next;
										}
										if (p->z <= qh->z)
										{
											if (symnclasslz.m_zquque == qh)
											{
												p->next = qh;
												symnclasslz.m_zquque = p;

											}
											else
											{
												p->next = qh;
												q2->next = p;

											}

										}
										else
										{
											qh->next = p;
											p->next = NULL;
										}

									}//end else
								}
								if((c+1)%100==0)
									pInfo->pctrlProgress4->StepIt();
							}//end for 
							
							sprintf(tempbuff, "结束%dX%d=%d次的运算", symnclasslz.m_col, symnclasslz.m_row, symnclasslz.m_col*symnclasslz.m_row);
							loger.LogOut(tempbuff);

							qh = symnclasslz.m_zquque;
							hjdnode *hjdp1;
							while (qh != NULL)
							{
								int vpos;
								float yl;
								vpos = qh->vpos;
								yl = symnclasslz.m_YLT[vpos].yl;
								if (symnclasslz.m_YLT[vpos].firsthjd != NULL)
								{
									hjdp1 = symnclasslz.m_YLT[vpos].firsthjd;
									symnclasslz.m_YLT[hjdp1->vos].yl = symnclasslz.m_YLT[hjdp1->vos].yl + yl*hjdp1->ylpec;
									while (hjdp1 != NULL)
									{
										if (hjdp1->next != NULL)
											hjdp1 = hjdp1->next;
										else
											break;
										symnclasslz.m_YLT[hjdp1->vos].yl = symnclasslz.m_YLT[hjdp1->vos].yl + yl*hjdp1->ylpec;
									}
								}
								qh = qh->next;
							}
							//这里处理最终油量。首先一个点最终能从几条路径中得出油增量，我已经在每次compare
							//时分次加入。要把每个点都走遍后的确切油量值传给显示的程序。让其在读路径的时候，用百分比去乘现在的点所带
							//的油量。然后我还要生成一个最终油量文件，让其应该为0的地方设置为0。
							FILE *fpsjyl;
							//这里要改成CFangan.ceng.tempoil
							//	tsjd=new char [2];
							//	lpath=CFangan.ceng.amount;
							//	lpath=lpath.Left(lpath.ReverseFind('\\')+1);
							//	lpath=lpath+fanganhao+"_"+_itoa(sjd,tsjd,10)+"_"+ssdcs+"_oil.txt";
							TRACE(CFangan.ceng.oil);

							//========================================
							//add by xxzhu 20170310
							//char temp[1024];
							strcpy(temp, globalDatPath);
							if (CFangan.ceng.oil[0] != '\\')
								strcat(temp, "\\");
							strcat(temp, CFangan.ceng.oil);
							//========================================

							if ((fpsjyl = fopen(temp, "w")) == NULL)
							{
								zchang = false;
								AfxMessageBox("不能生成临时文件");
								return 0;
							}
							else
							{
								for (int c = 0; c < symnclasslz.m_col*symnclasslz.m_row; c++)//对每个生油点都要考虑
									if (symnclasslz.m_XYZ.flag[c] != 2)
										fprintf(fpsjyl, "%d,%f\n", c, symnclasslz.m_YLT[c].yl);
								//	fprintf(fpsjyl,"%d,%f\n",c,symnclasslz.m_XYZ.yl[c]);
							}
							fclose(fpsjyl);


							fclose(fp1);
							fclose(fp2);
							//存储每一层的油量，放在临时文件夹中,主要是处理层与层之间的关系

							FILE *fptmp;

							if (CFangan.ceng.amount != NULL)
								lpath = CFangan.ceng.amount;
							else
								lpath = "";
							//lpath=dirpath+"temp\\oil"+_itoa(sjd,ssjd,10)+"-"+_itoa(sdcs,ssdcs,10)+".txt";
							//========================================
							//add by xxzhu 20170310
							//char temp[1024];
							strcpy_s(temp,1024,globalDatPath);
							if (lpath[0] != '\\')
								strcat(temp, "\\");
							strcat(temp, lpath);
							//========================================

							if ((fptmp = fopen(temp, "w")) == NULL)
							{
								zchang = false;
								AfxMessageBox("创建临时存油文件出错");
								return 0;
							}

							tm_XYZ.x = new int[symnclasslz.m_col];
							tm_XYZ.y = new int[symnclasslz.m_row];
							tm_XYZ.z = new float[symnclasslz.m_col*symnclasslz.m_row];
							tm_XYZ.flag = new int[symnclasslz.m_col*symnclasslz.m_row];
							tm_XYZ.yl = new float[symnclasslz.m_col*symnclasslz.m_row];
							for (int c1 = 0; c1 < symnclasslz.m_col*symnclasslz.m_row; c1++)//对每个生油点都要考虑
							{
								if (symnclasslz.m_XYZ.zeroflag[c1] == 0)
								{
									symnclasslz.m_XYZ.yl[c1] = 0.00;
									symnclasslz.m_YLT[c1].yl = 0;
								}
								tm_XYZ.x[c1%symnclasslz.m_col] = symnclasslz.m_XYZ.x[c1%symnclasslz.m_col];
								tm_XYZ.y[c1 / symnclasslz.m_col] = symnclasslz.m_XYZ.y[c1 / symnclasslz.m_col];
								tm_XYZ.z[c1] = symnclasslz.m_XYZ.z[c1];
								tm_XYZ.flag[c1] = symnclasslz.m_XYZ.flag[c1];
								tm_XYZ.yl[c1] = symnclasslz.m_YLT[c1].yl;
								if (symnclasslz.m_XYZ.flag[c1] != 2)
									fprintf(fptmp, "%d,%d,%f,%d\n", c1%symnclasslz.m_col, c1 / symnclasslz.m_col, symnclasslz.m_YLT[c1].yl, symnclasslz.m_XYZ.flag[c1]);


							}

							fclose(fptmp);
							delete symnclasslz.m_zquque;
							delete symnclasslz.stackmn;
							delete[] symnclasslz.m_XYZ.x;
							delete[] symnclasslz.m_XYZ.y;
							delete[] symnclasslz.m_XYZ.yl;
							delete[] symnclasslz.m_XYZ.z;
							delete[] symnclasslz.m_XYZ.ylpec;
							delete[] symnclasslz.m_XYZ.zeroflag;
							delete[] symnclasslz.m_XYZ.flag;
							delete[] symnclasslz.m_YLT;
						}//end if 路径存在
					}//end if 读配置信息正确（内层）
					pInfo->pctrlProgress3->StepIt();
				} //end for2	深度循环

			} //end if 该时间段的深度层次，得到了深度层次
			
			pInfo->pctrlProgress1->StepIt();
		};//end for1 时间段的循环
	}//最外层if结束

	jsflag = true;
	loger.LogOut("已经完成整个数据的处理，程序退出");
	exit(0);
	return true;
}


BOOL CTimedProgressDlg::OnInitDialog()
{
	//===========================================================
	//add by xxzhu 20170314
	//现在从命令行参数中提取方案文件，并向前追溯两个\的之前部分作为数据文件的文件夹
	if (__argc <=1 )
	{
		AfxMessageBox("对不起，必须带一个参数");
		return 0;
	}
	famc = CString(__argv[1]);  //得到方案文件的完整路径
	famc.TrimLeft();
	famc.TrimRight();

	strcpy(globalDatPath, __argv[1]);//获取路径
	int len = strlen(globalDatPath);
	int count = 0;
	while (len > 0)
	{
		len--;
		if (globalDatPath[len] != '\\')
		{
			globalDatPath[len] = '\0';			
		}
		else
		{
			globalDatPath[len] = '\0'; //第一次遇到的斜杠也会变成结束标记
			count++;
			if (count == 2)  //倒数第二个反斜杠是数据文件的路径
				break;
		}
	}
	//end add
	//========================================================================

//	CString famc="E:\\石油演示\\shiyou\\addfunction\\Fang_an_7.20\\project\\fa\\gongtry.txt";
	CDialog::OnInitDialog();

    // Add "About..." menu item to system menu.

    // IDM_ABOUTBOX must be in the system command range.
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        CString strAboutMenu;
        strAboutMenu.LoadString(IDS_ABOUTBOX);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
	
	//Process(famc);
	


//    m_btnGo.ShowWindow(SW_HIDE);
 //   m_btnStop.ShowWindow(SW_SHOW);
 //   m_btnStop.SetFocus();

	SetTimer(5000, 1000, NULL);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTimedProgressDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialog::OnSysCommand(nID, lParam);
    }
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTimedProgressDlg::OnPaint() 
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTimedProgressDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CTimedProgressDlg::OnCancel() 
{
    // prevent Esc from closing app
}

void CTimedProgressDlg::OnDestroy() 
{
    //this->KillTimer(0);
    CDialog::OnDestroy();
}

//DEL void CTimedProgressDlg::OnGo() 
//DEL {
//DEL 
//DEL     
//DEL 
//DEL 
//DEL }

//DEL void CTimedProgressDlg::OnStop() 
//DEL {
//DEL //    m_btnStop.ShowWindow(SW_HIDE);
//DEL  //   m_btnGo.ShowWindow(SW_SHOW);
//DEL  //   m_btnGo.SetFocus();
//DEL   //  this->KillTimer(0);
//DEL }

void CTimedProgressDlg::OnTimer(UINT nIDEvent) 
{
//	if (zchang==false)
//		CDialog::OnOK();
    
	/*if (m_progress3.GetPos() == sdglobe*50)
	{
		countcc2++;
		m_progress1.SetPos(countcc2*sdglobe*50);
		m_progress3.ResetStartTime();
        m_progress3.SetPos(0);
	}
	
	if (m_progress4.GetPos() == 50 )
    {
		countcc1++;
		m_progress4.ResetStartTime();
        m_progress4.SetPos(0);
			
	}
    else
    {
        srand(time(NULL));
        int nOffset = rand() % 10;

		m_progress1.OffsetPos(nOffset);
	    m_progress3.OffsetPos(nOffset);
        m_progress4.OffsetPos(nOffset);

    }*/
	//if (jsflag==true)
	//if (countcc1*50==sjglobe*50 && jsflag==true)
	//	CDialog::OnOK();

	KillTimer(5000);//此定时器只用一次就可以了，

	Info.pctrlProgress1 = &m_progress1;
	Info.pctrlProgress3 = &m_progress3;
	Info.pctrlProgress4 = &m_progress4;

	hThread = CreateThread(NULL,
	0,
	(LPTHREAD_START_ROUTINE)Process,
	&Info,
	0,
	&ThreadID);

	//WaitForSingleObject(hThread, INFINITE);
	//CDialog::OnOK();

    CDialog::OnTimer(nIDEvent);
}

//DEL void CTimedProgressDlg::OnSpeedReleasedCapture(NMHDR *pNMHDR, LRESULT *pResult)
//DEL {
//DEL     if (m_btnStop.IsWindowVisible())
//DEL     {
//DEL         this->KillTimer(0);
//DEL         this->SetTimer(0, 117*95, NULL);
//DEL     }
//DEL 
//DEL 	*pResult = 0;
//DEL }

//DEL void CTimedProgressDlg::OnOK() 
//DEL {
//DEL 	// TODO: Add extra validation here
//DEL 	
//DEL 	CDialog::OnOK();
//DEL }

void CTimedProgressDlg::OnClose() 
{
	if (::MessageBox(NULL, "确定要退出吗？","程序退出", MB_YESNO) != IDYES)
	{
		return;
	}
	if(hThread)
		TerminateThread(hThread, 0);
	CDialog::DestroyWindow();
	CDialog::OnClose();
}
