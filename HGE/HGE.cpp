//����ǰ��������������һ��
//������Ҿ�����������һ��

#include "hge.h" 
#include "hgeSprite.h"
#include "hgeFont.h"
#include "hgeparticle.h"
#include<time.h>
#include<math.h>
#include<windows.h>
#include<stdio.h>
#include "StdAfx.h"
#include <tchar.h> 


HGE *hge=0;//����HGEָ�� 
hgeSprite *spr1,*spr2,*spr3;//����������ָ�� 
hgeParticleSystem *par;
hgeFont *fnt;//��������ָ��,
HTEXTURE tex1,tex2,tex3;//����һ��texture(����)����


//�����趨
int ScreenW=1280,ScreenH=720;    //�ֱ���
int speed=100;                   //��ʼ�ٶ����ã�������ٶȼ��㷽ʽ���FrameFunc()

//���߰���ȫ�ֱ���������
double rx,ry;
float x[3]={0};
float y[3]={0};
float mx,my,distance1;
float distance2[3]={0};
float start_x[3]={0},start_y[3]={0};
bool flag=true;

float dt=hge->Timer_GetDelta();//���ϴο�ʼ��Ⱦ��������Ⱦ����Ҫ��ʱ�䣬Ϊ��ͳһ��ͬ���õ������ٶ�

clock_t score;


int CDECL MessageBoxPrintf (TCHAR * szCaption, TCHAR * szFormat, ...)    //����ģ�顣����ʲô����Ҳ��֪����������������������ʾ��~������
{
     TCHAR   szBuffer [1024] ;
     va_list pArgList ;
          // The va_start macro (defined in STDARG.H) is usually equivalent to:
          // pArgList = (char *) &szFormat + sizeof (szFormat) ;
     va_start (pArgList, szFormat) ;
          // The last argument to wvsprintf points to the arguments
     _vsntprintf_s (szBuffer, sizeof (szBuffer) / sizeof (TCHAR), 
                  szFormat, pArgList) ;
          // The va_end macro just zeroes out pArgList for no good reason
     va_end (pArgList) ;
     return MessageBox (NULL, szBuffer, szCaption, 0|1) ;
}

void mian_ball()//��������˶���ʽ
{
	double xv,yv;//���������
	xv=rx-mx;
	yv=ry-my;
	distance1=sqrt(pow(xv,2)+pow(yv,2));//�����ľ��빫ʽ~
	xv /= distance1;//��ʵ����ѧ���ã�����ֻ��������
	yv /= distance1;
	xv *= speed*dt;//ͬ��
	yv *= speed*dt;//�о���һ��dt��Ϊ��ͳһ�ٶ�
	rx -= xv;
	ry -= yv;//�����趨�µ����꣡
}

void three_ball()//����ʲôҲû�У�����ô��Ҳû�С�
{
	float dt=hge->Timer_GetDelta();
	int i;
	double start_xv,start_yv,stepxv,stepyv;

}



bool RenderFunc()//���ƺ���������ʼ��HGE����ͣ������ 
{ 
	int i;
	hge->Gfx_BeginScene();//��ʼ��Ⱦ 
	hge->Gfx_Clear(0xFF000000);//��ĳ��ɫ������OxFF000000Ϊ͸����Ϊ0�ĺ�ɫ
	score = clock()/10;//���÷���
	fnt->SetColor(0xFF00FFFF); //����������ɫΪ��ʲô��ɫ
	fnt->printf(5, 5, HGETEXT_LEFT, "SCORE:%u\nFPS:%d\nSPEED:%d",score,hge->Timer_GetFPS(),speed);
	hge->Input_GetMousePos(&mx,&my);//��ȡ�������

	if(mx<15) //Ϊ�˲��ý�����ͼ�ܵ��������棬������������
		mx=15;
	if(my<15)
		my=15;
	if(mx>ScreenW-15)
		mx=ScreenW-15;
	if(my>ScreenH-15)
		my=ScreenH-15;

	par->Render();//��ʼ��Ⱦ����
	spr1->Render(mx,my);//����꽹�㴦��Ⱦ��ͼ
	spr2->Render(rx,ry);//��ʼ��Ⱦ������

	//for(i=1;i<=3;i++)//��ʼ��Ⱦ����������
		//spr2->Render(x[i],y[i]);

	hge->Gfx_EndScene();//������Ⱦ 
	return false;//���Ƿ���false 
} 

bool FrameFunc()//�߼�����������ʼ��HGE����ͣ��������һЩ�߼��жϻ��ߴ������д����� 
{ 
	mian_ball();//���ø�����
	//three_ball();//���÷�����

	if(score<=1000)//�ٶȼ��㣬�Ǹ���ѧ���ã�д�������˱����
	{
		speed=150;
	}
	if(score>1000 && score <=1500)
	{
		speed=140+score*0.01;
	}
	if(score>1500 && score <=3000)
	{
		speed=125+score*0.02;
	}
	if(score>3000)
	{
		speed=95+score*0.03;
	}

	par->info.nEmission=50;//������������
	par->MoveTo(mx,my);//�����˶���ʽ
	par->Update(dt);//��������

	if(hge->Input_GetKeyState(HGEK_ESCAPE) || distance1<40)//ESC����
	{
		return true;
	}else{
		return false;//��������ʱ���Ƿ���false������true����System_Start����ִ�� 
	}
} 



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)//WinMain�������������ڡ� 
{ 
	srand((int)time(0));
	int k;
	hge=hgeCreate(HGE_VERSION);//ʹ��hgeCreate��������HGE�ӿڣ��������봫����ȷ��HGE_VERSION,������hge.h�ж���� 
	hge->System_SetState(HGE_SCREENWIDTH,ScreenW);//������Ļ���
	hge->System_SetState(HGE_SCREENHEIGHT,ScreenH);//������Ļ�߶�
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//�����߼�����ΪFrameFunc���� 
	hge->System_SetState(HGE_RENDERFUNC,RenderFunc);//���û��ƺ���ΪRenderFunc���� 
	hge->System_SetState(HGE_FPS, 100);//�������FPSΪ100
	hge->System_SetState(HGE_TITLE, "���ۢ��");//���ô��ڱ��� 
	hge->System_SetState(HGE_WINDOWED,true);//����ʹ�ô���ģʽ 
	hge->System_SetState(HGE_USESOUND,false);//���ò�ʹ���������ڶ������������Ȳ�����������֪ʶ��
	hge->System_SetState(HGE_SHOWSPLASH, false);
	if(hge->System_Initiate())//��hge���System_Initiate()����������ʼ���Ƿ��д�����֡� 
	{ 
		tex1=hge->Texture_Load("1.png");//����·������ͼƬ 
		tex2=hge->Texture_Load("2.png");
		tex3=hge->Texture_Load("particles.png");
		if(tex1 && tex2)
		{//����Ƿ�ͼƬ�ɹ����� 
			spr1=new hgeSprite(tex1,0,0,30,30);//��ʼ�����飨��꣩
			spr1->SetHotSpot(16,16);//�������趨Ϊ����
			spr2=new hgeSprite(tex2,0,0,50,50);//��ʼ�����飨��
			spr2->SetHotSpot(25,25);//�������趨Ϊ����
			spr3=new hgeSprite(tex3,32,32,32,32);////��ʼ�����飨���ӣ�
			spr3->SetHotSpot(16,16);//�������趨Ϊ����
			par=new hgeParticleSystem("trail.psi",spr3);//��ʼ�����ӣ�ʹ�þ���spr3
			par->Fire();//�趨���ӷ���ģʽ
			fnt = new hgeFont("font1.fnt");//��ʼ������

			//��ʼ�����������������
			rx=rand()%ScreenW-25;//���������ڴ�����
			ry=rand()%ScreenH-25;
			for(k=1;k<=3;k++)
			{
				x[k]=rand()%ScreenW-25;//�������������
				y[k]=rand()%ScreenH-25;
				start_x[k]=rand()%ScreenW-50;
				start_y[k]=rand()%ScreenH-50;

				if(x[k]<25)//���������ڴ�����
				{
					x[k]=25;
					start_x[k];
				}
				if(y[k]<15)
				{
					y[k]=25;
					start_y[k];
				}
				if(x[k]>ScreenW-25)
				{
					x[k]=ScreenW-25;
					start_x[k];
				}
				if(y[k]>ScreenH-25)
				{
					y[k]=ScreenH-25;
					start_y[k];
				}
			}
			hge->System_Start();//���û�����⣬��ʹ��System_Start��������ʼ����
			MessageBoxPrintf (TEXT ("Game Over"), TEXT ("��ķ����ǣ�%u"),score) ;//��Ϸ�����������
		}
	} 
	hge->Texture_Free(tex1);//�ͷ����� 
	hge->Texture_Free(tex2);//�ͷ����� 
	delete spr1;//�ͷž��� 
    delete spr2;
	delete spr3;
	delete par;//�ͷ�����
	delete fnt;//�ͷ�����
	hge->System_Shutdown();//����ֹͣ 
	hge->Release();//�ͷ�HGE��ռ�õ��ڴ档 
	return 0;
}