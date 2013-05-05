//三天前它就像我亲生的一样
//三天后我就像他亲生的一样

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


//参数设定
int ScreenW=1280,ScreenH=720;    //分辨率
double speed=100;                   //初始速度设置，具体的速度计算方式请见FrameFunc()

//乱七八糟全局变量定义区
double rx,ry;
float x[3]={0};
float y[3]={0};
float mx,my;
double distance1;
float distance2[3]={0};
float speed_dirx[3]={0},speed_diry[3]={0};
bool flag=true;

clock_t score;


int CDECL MessageBoxPrintf (TCHAR * szCaption, TCHAR * szFormat, ...)    //弹窗模块。。有什么用我也不知道，反正这样就能正常显示了~不纠结
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

void mian_ball()//跟随球的运动方式
{
	float dt=hge->Timer_GetDelta();//从上次开始渲染到结束渲染所需要的时间，为了统一不同配置的运行速度
	double xv,yv;//坐标分向量
	xv=rx-mx;
	yv=ry-my;
	distance1=sqrt(pow(xv,2)+pow(yv,2));//两点间的距离公式~
	xv /= distance1;//其实我数学不好，上面只是卖个萌
	yv /= distance1;
	xv *= speed*dt;//同上
	yv *= speed*dt;//感觉乘一个dt是为了统一速度
	rx -= xv;
	ry -= yv;//恩，设定新的坐标！
}

void three_ball()//这里什么也没有，再怎么看也没有。
{
	float dt=hge->Timer_GetDelta();//从上次开始渲染到结束渲染所需要的时间，为了统一不同配置的运行速度
	double speedx,speedy;
	int i;
	for(i=1;i<=3;i++)
	{
		speedx=speed*0.707107*speed_dirx[i]*dt;
		speedy=speed*0.707107*speed_diry[i]*dt;
		x[i] += speedx;
		y[i] += speedy;
		distance2[i]=sqrt(pow((x[i]-mx),2)+pow((y[i]-my),2));
		if(x[i]<25 || x[i]>ScreenW-25)//反弹处理
		{
			speed_dirx[i]=-speed_dirx[i];
		}
		if(y[i]<25 ||y[i]>ScreenH-25)
		{
			speed_diry[i]=-speed_diry[i];
		}
	}
}



bool RenderFunc()//绘制函数，程序开始后HGE将不停调用它 
{ 
	int i;
	hge->Gfx_BeginScene();//开始渲染 
	hge->Gfx_Clear(0xFF000000);//以某颜色清屏，OxFF000000为透明度为0的黑色
	score = clock()/10;//设置分数
	fnt->SetColor(0xFF00FFFF); //设置字体颜色为那什么颜色
	fnt->printf(5, 5, HGETEXT_LEFT, "SCORE:%u\nFPS:%d\nSPEED:%.0f",score,hge->Timer_GetFPS(),speed);
	hge->Input_GetMousePos(&mx,&my);//获取鼠标坐标

	if(mx<15) //为了不让焦点贴图跑到窗口外面，修正鼠标的坐标
		mx=15;
	if(my<15)
		my=15;
	if(mx>ScreenW-15)
		mx=ScreenW-15;
	if(my>ScreenH-15)
		my=ScreenH-15;

	par->Render();//开始渲染粒子
	spr1->Render(mx,my);//在鼠标焦点处渲染贴图
	spr2->Render(rx,ry);//开始渲染跟随球

	for(i=1;i<=3;i++)//开始渲染三个反弹球
		spr2->Render(x[i],y[i]);

	hge->Gfx_EndScene();//结束渲染 
	return false;//总是返回false 
} 

bool FrameFunc()//逻辑函数，程序开始后HGE将不停调用它，一些逻辑判断或者处理可以写在这里。 
{ 
	float dt=hge->Timer_GetDelta();//从上次开始渲染到结束渲染所需要的时间，为了统一不同配置的运行速度
	mian_ball();//调用跟随球
	three_ball();//调用反弹球

	if(score<=1000)//速度计算，那个数学不好，写成这样了别怪我
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

	par->info.nEmission=50;//粒子生命周期
	par->MoveTo(mx,my);//粒子运动方式
	par->Update(dt);//更新粒子

	if(hge->Input_GetKeyState(HGEK_ESCAPE) || distance1<40 || distance2[1]<40 || distance2[2]<40 || distance2[3]<40)//ESC结束,撞球结束
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
	hge->System_SetState(HGE_SCREENWIDTH,ScreenW);//设置屏幕宽度
	hge->System_SetState(HGE_SCREENHEIGHT,ScreenH);//设置屏幕高度
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);//设置逻辑函数为FrameFunc函数 
	hge->System_SetState(HGE_RENDERFUNC,RenderFunc);//设置绘制函数为RenderFunc函数 
	hge->System_SetState(HGE_FPS, 100);//设置最大FPS为100
	hge->System_SetState(HGE_TITLE, "蛋疼⑨号");//设置窗口标题 
	hge->System_SetState(HGE_WINDOWED,true);//设置使用窗口模式 
	hge->System_SetState(HGE_USESOUND,false);//设置不使用声音（第二个程序我们先不讲解声音的知识）
	hge->System_SetState(HGE_SHOWSPLASH, false);
	if(hge->System_Initiate())//用hge类的System_Initiate()方法，检测初始化是否有错误出现。 
	{ 
		tex1=hge->Texture_Load("./resources/cursow.png");//根据路径载入图片 
		tex2=hge->Texture_Load("./resources/ball.png");
		tex3=hge->Texture_Load("./resources/a.png");
		if(tex1 && tex2 && tex3)
		{//检测是否图片成功载入 
			spr1=new hgeSprite(tex1,0,0,30,30);//初始化精灵（鼠标）
			spr1->SetHotSpot(16,16);//将焦点设定为中心
			spr2=new hgeSprite(tex2,0,0,50,50);//初始化精灵（球）
			spr2->SetHotSpot(25,25);//将焦点设定为中心
			spr3=new hgeSprite(tex3,32,32,32,32);////初始化精灵（粒子）
			spr3->SetHotSpot(16,16);//将焦点设定为中心
			par=new hgeParticleSystem("./resources/a.psi",spr3);//初始化粒子，使用精灵spr3
			par->Fire();//设定粒子发射模式
			fnt = new hgeFont("./resources/font.fnt");//初始化文字

			//初始化三个反弹球的坐标，及运动方向
			rx=rand()%ScreenW-25;//避免生成在窗口外
			ry=rand()%ScreenH-25;
			for(k=1;k<=3;k++)
			{
				x[k]=rand()%ScreenW-25;//随机数不解释了
				y[k]=rand()%ScreenH-25;
				while(!speed_dirx[k] || !speed_diry[k])//随机的方向
				{
					speed_dirx[k]=rand()%3-1;
					speed_diry[k]=rand()%3-1;
				}
				if(x[k]<25)//避免生成在窗口外
				{
					x[k]=25;
				}
				if(y[k]<25)
				{
					y[k]=25;
				}
				if(x[k]>ScreenW-25)
				{
					x[k]=ScreenW-25;
				}
				if(y[k]>ScreenH-25)
				{
					y[k]=ScreenH-25;
				}
			}
			hge->System_Start();//如果没有问题，则使用System_Start方法，开始程序。
			MessageBoxPrintf (TEXT ("Game Over"), TEXT ("你的分数是：%u"),score) ;//游戏结束输出分数
		}
	} 
	hge->Texture_Free(tex1);//释放纹理 
	hge->Texture_Free(tex2);//释放纹理 
	delete spr1;//释放精灵 
    delete spr2;
	delete spr3;
	delete par;//释放粒子
	delete fnt;//释放文字
	hge->System_Shutdown();//程序停止 
	hge->Release();//释放HGE所占用的内存。 
	return 0;
}