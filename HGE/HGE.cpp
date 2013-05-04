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


HGE *hge=0;//创建HGE指针 
hgeSprite *spr1,*spr2,*spr3;//创建精灵类指针 
hgeParticleSystem *par;
hgeFont *fnt;//定义字体指针,
HTEXTURE tex1,tex2,tex3;//定义一个texture(纹理)对象



int ScreenW=1280,ScreenH=720;
int top=400;
int speed=100;
double rx,ry;
float x[3]={0};
float y[3]={0};
float mx,my,distance1;
float distance2[3]={0};
float start_x[3]={0},start_y[3]={0};
bool flag=true;

clock_t score;


int CDECL MessageBoxPrintf (TCHAR * szCaption, TCHAR * szFormat, ...)
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

void mian_ball()
{
	float dt=hge->Timer_GetDelta();
	double xv,yv;
	xv=rx-mx;
	yv=ry-my;
	distance1=sqrt(pow(xv,2)+pow(yv,2));
	xv /= distance1;
	yv /= distance1;
	xv *= speed*dt;
	yv *= speed*dt;
	rx -= xv;
	ry -= yv;
}

void three_ball()
{
	float dt=hge->Timer_GetDelta();
	int i;
	double start_xv,start_yv,stepxv,stepyv;

}



bool RenderFunc()//绘制函数，程序开始后HGE将不停调用它 
{ 
	int i;
	hge->Gfx_BeginScene();//开始渲染 
	hge->Gfx_Clear(0xFF000000);//以某颜色清屏，OxFF000000为透明度为0的黑色
	score = clock()/10;
	fnt->SetColor(0xFF00FFFF); 
	fnt->printf(5, 5, HGETEXT_LEFT, "SCORE:%u\nFPS:%d\nSPEED:%d",score,hge->Timer_GetFPS(),speed);
	hge->Input_GetMousePos(&mx,&my);
	if(mx<15)
		mx=15;
	if(my<15)
		my=15;
	if(mx>ScreenW-15)
		mx=ScreenW-15;
	if(my>ScreenH-15)
		my=ScreenH-15;
	par->Render();
	spr1->Render(mx,my);//在指定位置显示精灵
	spr2->Render(rx,ry);
	for(i=1;i<=3;i++)
		spr2->Render(x[i],y[i]);
	hge->Gfx_EndScene();//结束渲染 
	return false;//总是返回false 
} 

bool FrameFunc()//逻辑函数，程序开始后HGE将不停调用它，一些逻辑判断或者处理可以写在这里。 
{ 
	mian_ball();
	three_ball();
	if(score<=1000)
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
	float dt=hge->Timer_GetDelta();
	par->info.nEmission=50;
	par->MoveTo(mx,my);
	par->Update(dt);
	if(hge->Input_GetKeyState(HGEK_ESCAPE) || distance1<40)
	{
		return true;
	}else{
		return false;//程序正常时总是返回false，返回true将从System_Start往下执行 
	}
} 



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)//WinMain函数，程序的入口。 
{ 
	srand((int)time(0));
	int k;
	hge=hgeCreate(HGE_VERSION);//使用hgeCreate函数创建HGE接口，参数必须传递正确的HGE_VERSION,它是在hge.h中定义的 
	hge->System_SetState(HGE_SCREENWIDTH,ScreenW);//将屏幕宽度设置为800 
	hge->System_SetState(HGE_SCREENHEIGHT,ScreenH);//将屏幕高度设置为600 
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//设置逻辑函数为FrameFunc函数 
	hge->System_SetState(HGE_RENDERFUNC,RenderFunc);//设置绘制函数为RenderFunc函数 
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_TITLE, "蛋疼⑨号");//设置窗口标题为“显示图像” 
	hge->System_SetState(HGE_WINDOWED,true);//设置使用窗口模式 
	hge->System_SetState(HGE_USESOUND,false);//设置不使用声音（第二个程序我们先不讲解声音的知识）
	hge->System_SetState(HGE_SHOWSPLASH, false);
	if(hge->System_Initiate())//用hge类的System_Initiate()方法，检测初始化是否有错误出现。 
	{ 
		tex1=hge->Texture_Load("1.png");//根据路径载入图片 
		tex2=hge->Texture_Load("2.png");
		tex3=hge->Texture_Load("particles.png");
		if(tex1 && tex2)
		{//检测是否图片成功载入 
			spr1=new hgeSprite(tex1,0,0,30,30);//初始化精灵spr，并且指定tex为它的纹理 
			spr1->SetHotSpot(16,16);
			spr2=new hgeSprite(tex2,0,0,50,50);//初始化精灵spr，并且指定tex为它的纹理 
			spr2->SetHotSpot(25,25);
			spr3=new hgeSprite(tex3,32,32,32,32);
			spr3->SetHotSpot(16,16);
			par=new hgeParticleSystem("trail.psi",spr3);
			par->Fire();
			fnt = new hgeFont("font1.fnt");
			rx=rand()%ScreenW-25;
			ry=rand()%ScreenH-25;
			for(k=1;k<=3;k++)
			{
				x[k]=rand()%ScreenW-25;
				y[k]=rand()%ScreenH-25;
				start_x[k]=rand()%ScreenW-50;
				start_y[k]=rand()%ScreenH-50;
				if(x[k]<25)
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
			hge->System_Start();//如果没有问题，则使用System_Start方法，开始程序。
			MessageBoxPrintf (TEXT ("Game Over"), TEXT ("你的分数是：%u"),score) ;
		}
	} 
	hge->Texture_Free(tex1);//释放纹理 
	hge->Texture_Free(tex2);//释放纹理 
	delete spr1;//释放精灵 
    delete spr2;
	delete fnt;
	hge->System_Shutdown();//程序停止 
	hge->Release();//释放HGE所占用的内存。 
	return 0;
}