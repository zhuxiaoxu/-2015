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
	
	CString sDir = famc;//�õ������ļ�����
	int length = sDir.ReverseFind('\\'); //�����һ��б��
	//CString dirpath;	//xxzhu ���ָñ���������δʹ�ã����ע�ͣ�20170314
	//dirpath=sDir.Left(length+1); //�õ�������·������,���ָñ���������δʹ�ã����ע�ͣ�20170314
	CString fanganhao;
	fanganhao = sDir.Right(sDir.GetLength() - length - 1);
	fanganhao = fanganhao.Left(fanganhao.Find('.')); //�õ������ļ����ļ�����Ϊ������
	TRACE("%s", fanganhao);
	/*char logFile[1024];
	strcpy(logFile, globalDatPath);
	strcat(logFile, "\\log.txt");*/
	CLogClass  loger; //��־�ļ�

	FILE *fp1, *fp2, *fp3, *fp4;
	struct DOT dot;
	symnclass  symnclasslz;
	int pos;//�ñ�����¼bdepb1.grd����Ч���ִ�ʲô�ط���ʼ
	XYZ tm_XYZ;//������ſ��Ŷϲ�����

			   //���ȴ򿪷����ļ�
	LPTSTR p = sDir.GetBuffer(sDir.GetLength());
	CAna_file CFangan; //���췽���ļ�����
	CFangan.readfile(p); //���ļ�����һ����ȫ����ȡ�����ڲ�ָ��m_pBuffer��������ڴ���
	sDir.ReleaseBuffer(); //add by xxzhu 20170324
	CString lpath;
	int kscs = 0;
	char tempbuff[128]; //����logout��intת�ַ��� xxzhu 20170328
	loger.LogOut("�򿪷����ļ��ɹ�");
	//֮ǰ�Ĵ����ظ�����fasx����һЧ�ʵ��£��ڶ����ڲ������ź�ʱ������ڴ�й©
	int ret = CFangan.fasx();//��ȡ������Ϣ
	if (ret != 1)
	{
		zchang = false;
		loger.LogOut("��������Ϣ���������ļ�ͷ����ȷ�������˳�����");
		AfxMessageBox("��������Ϣ���������ļ�ͷ����ȷ");
		//û���ɻ���
		exit(0);//��Ϊ��ǰ���̣߳��ô˷�������������
		return 0;
	}
	else //��ȡ������Ϣ��ȷ
		//if (CFangan.fasx()==1)//�õ�ʱ�����������������ѭ��
	{
		loger.LogOut("�õ�ʱ�����", CFangan.sjds);
		int local_sjd = atoi(CFangan.sjds);
		pInfo->pctrlProgress1->SetRange(0, local_sjd); //ʱ��εĽ�����
		pInfo->pctrlProgress1->SetStep(1);//����Ϊ1
		for (int sjd = 1; sjd <= local_sjd; sjd++)// for 1 xxzhu ������ʱ���ѭ��
		{
			
			loger.LogOut("��ʼ����ʱ���", _itoa(sjd, tempbuff, 10));
			
			if (kscs > 1)
			{
				delete[] tm_XYZ.x;
				delete[] tm_XYZ.y;
				delete[] tm_XYZ.yl;
				delete[] tm_XYZ.z;
				delete[] tm_XYZ.flag;
			}
			kscs = 0;//��Ϊ�˿Ϳ��ܵ�һ��ĳ�㲻���м��㣬
					 //������ÿ��ʱ����ڲ��Ĳ����֮��������̳�
					 //һ��Ҫ�ӿ�ʼ����·�����ʱ��ż���

			if (CFangan.xinxi_a(sjd) == 1)//�õ���ʱ��ε���Ȳ�Σ���ѭ��ѭ���� //��ѭ������=������Χ*��Ȳ�Σ�							 
			{
				pInfo->pctrlProgress3->SetRange(0, atoi(CFangan.sdcs)); //ʱ��εĽ�����
				pInfo->pctrlProgress3->SetStep(1);
				loger.LogOut("������Ȳ�������", CFangan.sdcs);
				for (int sdcs = 1; sdcs <= atoi(CFangan.sdcs); sdcs++) //for 2
				{
					loger.LogOut("��ʼ������Ȳ�", _itoa(sdcs,tempbuff,10));
					ret = CFangan.xinxi_a_a(sjd, sdcs); //��ȡָ��ʱ��ε�ָ������Ϣ��CFangan�����ݳ�Աceng_xx
					if (ret != 1)
					{
						zchang = false;
						//char tempbuff[128];
						sprintf(tempbuff, "��ʱ���%d��%d�㷽����Ϣ����", sjd, sdcs);// add by xxzhu ��ʾ��ȷ��
						AfxMessageBox(tempbuff);
						loger.LogOut(tempbuff);
						exit(0);
						return 0;
					}
					else
						//if (CFangan.xinxi_a_a(sjd,sdcs)==1) //��������Ϣ������Ϣ
					{
						sprintf(tempbuff, "��ʱ���%d��%d�㷽����Ϣ�ɹ���", sjd, sdcs);
						loger.LogOut(tempbuff);
					
						//�õ�������ļ�
						if (CFangan.ceng.amount == NULL || CFangan.ceng.lj == NULL)
						{
							loger.LogOut("����Ϊ�ջ���·��Ϊ�գ����账��׼������һ��");
							pInfo->pctrlProgress3->StepIt(); 
							continue;//˵���ò�·�����ü��㡣����������һ��
									 //	zchang=false;
									 //	AfxMessageBox("");
									 //	return 0;
						}
						kscs++;
						if (CFangan.ceng.lj != NULL)		//��� ��������·�����ڣ�˵��Ҫ����·��
						{
							if (CFangan.ceng.sd == NULL)
							{
								sprintf(tempbuff, "��ʱ���%d��%d�㣬ȱ������ļ�������ֹͣ�����ء�", sjd, sdcs);
								loger.LogOut(tempbuff);
								//	CTimedProgressDlg::CloseWindow();
								zchang = false;
								AfxMessageBox("ȱ������ļ�");
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
									loger.LogOut("������ļ�����", temp);
									AfxMessageBox("������ļ�����" + CString(CFangan.ceng.sd));
									exit(0);
									return 0;
								}
							}
							//xxzhu 20170325 14:19
							//�����ò���ж����ж�����
							fseek(fp1, 5, SEEK_SET);//��ǰ���� DSAA �� ���У� ����������
							fscanf(fp1, "%d", &symnclasslz.m_col);
							fscanf(fp1, "%d", &symnclasslz.m_row);
							sprintf(tempbuff, "�ò㹲��%d��%d��", symnclasslz.m_col, symnclasslz.m_row);
							loger.LogOut(tempbuff);
							if (sjd == 1 && sdcs == 1)
							{//��һʱ��Ρ���һ��Ȳ��
								symnclasslz.whole_m_col = symnclasslz.m_col;
								symnclasslz.whole_m_row = symnclasslz.m_row;
							}
							if (symnclasslz.whole_m_col != symnclasslz.m_col || symnclasslz.whole_m_row != symnclasslz.m_row)
							{
								zchang = false;
								AfxMessageBox("�����ļ�����ֵ��ƥ��");
								loger.LogOut("�����ļ�����ֵ�͸�ʱ��ε�һ�㲻ƥ�䣬�����쳣�������˳�");
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
							//������һ����ڴ��������ݴ���

							int colmin;//����Сֵ
							float colmax;//�����ֵ
							int rowmin;//����Сֵ
							float rowmax;//�����ֵ
							char *ssjd;
							char *ssdcs;
							ssjd = new char[8];//�˴�������2�����ɿ������ܴ����Դ������
							ssdcs = new char[8];//�˴�������2�����ɿ������ܴ����Դ������

							fscanf(fp1, "%d%f%d%f%f%f", &colmin, &colmax, &rowmin, &rowmax, &symnclasslz.min, &symnclasslz.max);
							pos = ftell(fp1);
							//lpath=dirpath;//ȡ�ô����ļ���·��
							_itoa(sjd, ssjd, 10);//ʱ���ת�ַ���
							_itoa(sdcs, ssdcs, 10);//��Ȳ��ת�ַ���
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
								AfxMessageBox("����д·���ļ�����" + lpath);
								return 0;
							}
							loger.LogOut("��д��ʽ���ı��ļ���",temp);
							//�����߽�

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
									AfxMessageBox("�򿪱߽��ļ�����" + CString(CFangan.ceng.bj_created));
									loger.LogOut("�򿪱߽��ļ�����", temp);
									exit(0);
									return 0;
								}
							}
							loger.LogOut("�򿪱߽��ļ���", temp);
							if (CFangan.ceng.bj == NULL)
							{
								fp3 = NULL;
							}
							fseek(fp1, pos, SEEK_SET);

							symnclasslz.loaddata(fp1, fp3);
							FILE *foptmyl;//����ʱ��������
							char *tsjd;
							//���������ļ�
							FILE *fyshyl;//����ԭʼ�����ļ�

							if (CFangan.ceng.pyl == NULL && sjd == 1 && kscs == 1)
							{
								zchang = false;
								AfxMessageBox("��ʼʱ��εĿ�ʼ��ȱ�������ļ�");
								return 0;
							}
							else
							{
								symnclasslz.sybpointer = NULL;
								if (CFangan.ceng.pyl != NULL)//�����������
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
										AfxMessageBox("���ܴ��������ļ�");
										loger.LogOut("���ܴ������ļ���", temp);
										return 0;
									}
									loger.LogOut("�������ļ���", temp);

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
									loger.LogOut("д�ı���ʽ���ļ���", temp);
									if (sjd == 1)//�����ʱ���1
									{
										symnclasslz.loadsyb(fp4, fyshyl);
										//�������ͱ�
									}
									else//����ʱ��1���������͵�����
									{
										symnclasslz.loadsyb(fp4, fyshyl);
										//�������ͱ��δ���ò�ƺ�if��һ����
										//xxzhu 20170325 14:55
										tsjd = new char[8]; // ԭ����2����ȫ����޸�Ϊ8
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
											loger.LogOut("���ı���ʽ���ļ���", temp);
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
								else//���û������������ʱ�����2
								{
									if (sjd > 1)		//��ǰ��һ��ʱ��μ̳�
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
													//if (symnclasslz.m_XYZ.flag[yly*symnclasslz.m_col+ylx]!=2 && symnclasslz.m_XYZ.flag[yly*symnclasslz.m_col+ylx]!=4)//��ʱ�����������������͵㣬����Ч,���������͵�
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
										AfxMessageBox("��ʱ��μ�û�������ļ����޷��̳У������޷�����");
										return 0;
										}*/
									}
								}
							}

							//	if (sdcs>1)//����ǵڶ������ϣ���Ҫ�ѿ��Ŷϲ��ʯ�ͼӵ�Ŀǰ����������
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
								AfxMessageBox("��ʱ��μ�û�������ļ����޷��̳У������޷�����");
								loger.LogOut("��ʱ��μ�û�������ļ����޷��̳У������޷�����");
								exit(0);
								return 0;
							}

							/*�����ӡ֤�����ݣ���Ҫ�Ǽ̳������Ĵ�ӡ*/
/*rem by xxzhu ����֤�� 20170325
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
							loger.LogOut("д�ı���ʽ���ļ���", temp);
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
							/*if (CFangan.ceng.dc!=NULL)	//��նϲ�
										  {
										  CString dcfile;
										  dcfile=CFangan.ceng.dc;
										  symnclasslz.dealdc(dcfile,symnclasslz.m_XYZ,symnclasslz.m_col,symnclasslz.m_row,colmin,colmax,rowmin,rowmax,1);//��Ҫ�Ǹı�־
										  }
										  if (CFangan.ceng.dc_open!=NULL)  //����ʽ�ϲ�
										  {
										  CString dcfile;
										  dcfile=CFangan.ceng.dc_open;
										  symnclasslz.dealdc(dcfile,symnclasslz.m_XYZ,symnclasslz.m_col,symnclasslz.m_row,colmin,colmax,rowmin,rowmax,2);
										  }*/
							CFaultageProcess  CFaultage(symnclasslz.m_col, symnclasslz.m_row, colmin, colmax, rowmin, rowmax);
							//�����ϲ㴦�������
							if (CFangan.ceng.dc != NULL)	//��նϲ�
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
									loger.LogOut("��ʼ����ϲ������ļ�", dcfile);
									CFaultage.ProcessFaultage(dcfile, symnclasslz.m_XYZ, 1, lpath);
									CFaultage.ReadFaultageTempFile1(lpath, symnclasslz.m_XYZ, 1);
									loger.LogOut("�ϲ㴦���Ƿ�նϲ�");
								}
								catch (int flag)
								{
									sprintf(tempbuff, "�ϲ㴦��ʱ,��%d���������쳣�������˳�", flag);
									loger.LogOut(tempbuff);
									AfxMessageBox(tempbuff);
									exit(0);
								}

							}
							if (CFangan.ceng.dc_open != NULL)  //����ʽ�ϲ�
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
									loger.LogOut("��ʼ����ϲ������ļ�", dcfile);
									CFaultage.ProcessFaultage(dcfile, symnclasslz.m_XYZ, 2, lpath);
									CFaultage.ReadFaultageTempFile1(lpath, symnclasslz.m_XYZ, 2);
									loger.LogOut("�ϲ㴦���ǿ���ʽ�ϲ�");
								}
								catch (int flag)
								{
									sprintf(tempbuff, "�ϲ㴦��ʱ,��%d���������쳣�������˳�", flag);
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
							loger.LogOut("��ʼ������ѭ���������ϲ��е�ÿ�����͵�");
							while (q != NULL)//��ÿ�����͵㶼Ҫ����
							{
								dot.x = q->x;
								dot.y = q->y;
								dot.z = q->z;
								if (symnclasslz.m_XYZ.flag[dot.y*symnclasslz.m_col + dot.x] == 1)//��Ϊ�ϲ�ļ��������Ͷ�ջ�е����ʸı��
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
							loger.LogOut("����������ѭ���������ϲ��е�ÿ�����͵�");
							/*for(int c=0;c<symnclasslz.m_col*symnclasslz.m_row;c++)//��ÿ�����͵㶼Ҫ����
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
							pInfo->pctrlProgress4->SetRange(1, symnclasslz.m_col*symnclasslz.m_row/100); //ʱ��εĽ�����
							pInfo->pctrlProgress4->SetStep(1);
							sprintf(tempbuff, "����%dX%d=%d�ε�����", symnclasslz.m_col, symnclasslz.m_row, symnclasslz.m_col*symnclasslz.m_row);
							loger.LogOut(tempbuff);
							for (int c = 0; c < symnclasslz.m_col*symnclasslz.m_row; c++)//��ÿ�����͵㶼Ҫ����
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
							
							sprintf(tempbuff, "����%dX%d=%d�ε�����", symnclasslz.m_col, symnclasslz.m_row, symnclasslz.m_col*symnclasslz.m_row);
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
							//���ﴦ����������������һ���������ܴӼ���·���еó������������Ѿ���ÿ��compare
							//ʱ�ִμ��롣Ҫ��ÿ���㶼�߱���ȷ������ֵ������ʾ�ĳ��������ڶ�·����ʱ���ðٷֱ�ȥ�����ڵĵ�����
							//��������Ȼ���һ�Ҫ����һ�����������ļ�������Ӧ��Ϊ0�ĵط�����Ϊ0��
							FILE *fpsjyl;
							//����Ҫ�ĳ�CFangan.ceng.tempoil
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
								AfxMessageBox("����������ʱ�ļ�");
								return 0;
							}
							else
							{
								for (int c = 0; c < symnclasslz.m_col*symnclasslz.m_row; c++)//��ÿ�����͵㶼Ҫ����
									if (symnclasslz.m_XYZ.flag[c] != 2)
										fprintf(fpsjyl, "%d,%f\n", c, symnclasslz.m_YLT[c].yl);
								//	fprintf(fpsjyl,"%d,%f\n",c,symnclasslz.m_XYZ.yl[c]);
							}
							fclose(fpsjyl);


							fclose(fp1);
							fclose(fp2);
							//�洢ÿһ���������������ʱ�ļ�����,��Ҫ�Ǵ�������֮��Ĺ�ϵ

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
								AfxMessageBox("������ʱ�����ļ�����");
								return 0;
							}

							tm_XYZ.x = new int[symnclasslz.m_col];
							tm_XYZ.y = new int[symnclasslz.m_row];
							tm_XYZ.z = new float[symnclasslz.m_col*symnclasslz.m_row];
							tm_XYZ.flag = new int[symnclasslz.m_col*symnclasslz.m_row];
							tm_XYZ.yl = new float[symnclasslz.m_col*symnclasslz.m_row];
							for (int c1 = 0; c1 < symnclasslz.m_col*symnclasslz.m_row; c1++)//��ÿ�����͵㶼Ҫ����
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
						}//end if ·������
					}//end if ��������Ϣ��ȷ���ڲ㣩
					pInfo->pctrlProgress3->StepIt();
				} //end for2	���ѭ��

			} //end if ��ʱ��ε���Ȳ�Σ��õ�����Ȳ��
			
			pInfo->pctrlProgress1->StepIt();
		};//end for1 ʱ��ε�ѭ��
	}//�����if����

	jsflag = true;
	loger.LogOut("�Ѿ�����������ݵĴ��������˳�");
	exit(0);
	return true;
}


BOOL CTimedProgressDlg::OnInitDialog()
{
	//===========================================================
	//add by xxzhu 20170314
	//���ڴ������в�������ȡ�����ļ�������ǰ׷������\��֮ǰ������Ϊ�����ļ����ļ���
	if (__argc <=1 )
	{
		AfxMessageBox("�Բ��𣬱����һ������");
		return 0;
	}
	famc = CString(__argv[1]);  //�õ������ļ�������·��
	famc.TrimLeft();
	famc.TrimRight();

	strcpy(globalDatPath, __argv[1]);//��ȡ·��
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
			globalDatPath[len] = '\0'; //��һ��������б��Ҳ���ɽ������
			count++;
			if (count == 2)  //�����ڶ�����б���������ļ���·��
				break;
		}
	}
	//end add
	//========================================================================

//	CString famc="E:\\ʯ����ʾ\\shiyou\\addfunction\\Fang_an_7.20\\project\\fa\\gongtry.txt";
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

	KillTimer(5000);//�˶�ʱ��ֻ��һ�ξͿ����ˣ�

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
	if (::MessageBox(NULL, "ȷ��Ҫ�˳���","�����˳�", MB_YESNO) != IDYES)
	{
		return;
	}
	if(hThread)
		TerminateThread(hThread, 0);
	CDialog::DestroyWindow();
	CDialog::OnClose();
}
