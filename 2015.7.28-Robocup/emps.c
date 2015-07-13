#include <reg51.h>

/**
*	***预定义部分***
*/
#define  uint  unsigned int
#define  uchar unsigned char

/**
*  接口部分（按照目前接线写了一部分）
*/
sbit RA=P1^0;		//右电机后转
sbit RB=P1^1;		//右电机前转
sbit LA=P1^2;		//左电机前转
sbit LB=P1^3;		//左电机后转
sbit L0=P1^4;		//左电机开关
sbit R0=P1^5;		//右电机开关
sbit R1=P0^0;
sbit R2=P0^1;  	
sbit R3=P0^2;
sbit MID=P0^3;
sbit R5=P0^4;
sbit R6=P0^5;
sbit R7=P0^6;
unsigned char pwm_left=0;
unsigned char pwm_right=0;
unsigned char push_left=18;
unsigned char push_right=18;
bit right_pwm_on=1;
bit left_pwm_on=1;

/**
*  公共函数
*/
void Test();		//测试函数

void Running_1();		//执行函数，针对第一阶段比赛

void Running_2();		//执行函数，针对第二阶段比赛

void Running_3();		//执行函数，针对第三阶段比赛

void Delay(unsigned int t);

void InitTimer();

void Start();		//从出发点发动

void Home();		//回到出发点

void Stop();

/**
*	颜色传感器模块（根据实际自己修改）
*/
void InitColor();

void GetColorValue();

uchar GetColor(void);		//result：1-绿，2-白，3-红，4-黑，5-蓝

/**
*	红外循迹模块
*/
void Initspeed(int speed); 

void Forward(); 
 
void Backward(); 
 
void ForwardAdjust();
   
void BackwardAdjust(); 
 
void LeftTurn(); 				//转弯需要差速控制，这里为了简洁就不传速度进去了，实现时需要修改
 
void RightTurn(); 

void LeftPWM();

void RightPWM();

void LeftTurn_45();

void LeftTurn_90();

void LeftTurn_45_45();			//没用到，暂声明不实现

void LeftTurn_45_90();			//没用到，暂声明不实现

void LeftTurn_90_45();

void LeftTurn_90_90();			//没用到，暂声明不实现

void LeftTurn_45_45_45();		//没用到，暂声明不实现

void LeftTurn_45_45_90();		//没用到，暂声明不实现

void LeftTurn_45_90_45();		//没用到，暂声明不实现

void LeftTurn_90_45_45();		//没用到，暂声明不实现

void LeftTurn_45_90_90();		//没用到，暂声明不实现

void LeftTurn_90_45_90();		//没用到，暂声明不实现

void LeftTurn_90_90_45();		//没用到，暂声明不实现

void RightTurn_45();

void RightTurn_90();

void RightTurn_45_45();			//没用到，暂声明不实现

void RightTurn_45_90();			//没用到，暂声明不实现

void RightTurn_90_45();

void RightTurn_90_90();			//没用到，暂声明不实现

void RightTurn_45_45_45();		//没用到，暂声明不实现

void RightTurn_45_45_90();		//没用到，暂声明不实现

void RightTurn_45_90_45();		//没用到，暂声明不实现

void RightTurn_90_45_45();		//没用到，暂声明不实现

void RightTurn_45_90_90();		//没用到，暂声明不实现

void RightTurn_90_45_90();		//没用到，暂声明不实现

void RightTurn_90_90_45();		//没用到，暂声明不实现

void Turn_180();						//原地掉头

void Forward_S2O();						//前进，由S点至O点

void Forward_S2C();						//前进，由S点至C点(用于开始出发至C点)

void Forward_Get();						//前进，至抓取色块(即O点到A-E的距离)

void Forward_Get_Return();				//(掉头后),前进至原点(即A-E到O点的距离)

void Forward_Put();						//前进，至放下色块(即A-E到1-5的距离)

void Forward_Target();					//前进，从O点直接至A-E处(即Get+Put的距离，但是中途不停)

void Forward_Target_Get();				//前进，从O点直接至F、G，并get颜色准备搬运

void Backward_Point();					//后退，回到原点

uchar Effective(); 						//前进，用于判断车是否到达放置物体处

uchar Effective_Middle();				//后退，用于判断车是否到达原点

uchar Condition_Center_Forward();     	//前进，判断车是否偏离

uchar Condition_Center_Backward();		//后退，判断车是否偏离 

/**
*	机械臂模块
*/
void Catch();		//拾取色块

void Down();		//放下色块




/**
*  ***算法实现部分***
*/
//前进函数
void Forward()
{
    LA=1;
	LB=0;
	RA=0;
	RB=1;
}

//后退函数
void Backward()
{
	LA=1;
	LB=0;
	RA=0;
	RB=1;
}

//左转函数
void LeftTurn()
{
	LA=0;
	LB=1;
	RA=0;
	RB=1;
}

//右转函数
void RightTurn()
{
	LA=1;
	LB=0;
	RA=1;
	RB=0;
}

//PWM调制左电机转速函数
void LeftPWM(void)
{  
    if(left_pwm_on)
    {
        if(pwm_left<=push_left)
	    {
	       L0=1;
	    }
	    else 
	    {
	       L0=0;
        }
        if(pwm_left>=20)
	       pwm_left=0;
    }
    else    
    {
        L0=0;   //若未开启PWM，则左电机停止
    }
}

//PWM调制右电机转速函数
void RightPWM(void)
{ 
    if(right_pwm_on)
    { 
        if(pwm_right<=push_right)
	    {
	        R0=1; 							
        }
	    else 
	    {
	        R0=0;
        }
	    if(pwm_right>=20)
	    	pwm_right=0;
    }
    else    
    {
        R0=0;		//若未开启PWM，则右电机停止
    }
}
    
//PWM中断函数
void timer0() interrupt 1 
{
    TH0=0XFC;       //1ms定时
	TL0=0X66;
	pwm_left++;
	pwm_right++;
	LeftPWM();
	RightPWM();
}	

//执行函数,竞赛第一阶段
void Running_1()
{
	Start();				//出发
	Forward_S2C();			//前行至目标点C
	switch(GetColor())		//取得C点的颜色后，进行下一步动作
	{
		//***************C点为红色(3)***************
		case 3:			//C为红色，前行至放置点3
			Catch();			//抓起色块(抓取得在颜色识别后完成)
			Forward_Put();		//前行至3放置点
			Down();				//放下色块
			Backward_Point();	//退回至O点，此时车头与x轴负方向成90度
			//阶段1完成
			LeftTurn_90();		//左转90度，车头朝向A点
			Forward_Get();		//前行至A点
			switch(GetColor())		//取得A点的颜色后，进行下一步动作
			{
				case 1:		//A为绿色，前行至1
					Catch();
					Forward_Put();
					Down();
					Backward_Point();		//退回至O点，此时车头与x抽负方向成0度
					//阶段2完成
					Turn_180();		//原地掉头，车头朝向E
					Forward_Get();	//前行至E点
					switch(GetColor())		//取得E点的颜色后，进行下一步动作
					{
						case 5:		//E为蓝色，前行至5
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							LeftTurn_90();		//车头调直(调至与x轴负方向成90度)
							Home();		//退到出发点
							break;
							
						case 2:		//E为白色，退回至O，前行至2
							Catch();
							Turn_180();
							Forward_Get_Return();	//原路返回至O点
							RightTurn_45();			//车头指向B
							Forward_Target();		//从O点直接行至2点
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							RightTurn_45();			//调直车头
							Home();
							break;
						
						case 4:		//E为黑色，退回至O，前行至4
							Catch();
							Turn_180();
							Forward_Get_Return();	//原路返回至O点
							RightTurn_90_45();		//车头指向D
							Forward_Target();		//从O点直接行至4点
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							RightTurn_45();			//调直车头
							Home();
							break;
					}
					break;
					
				case 2:		//A为白色，退回至O，前行至2
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_90_45();
					Forward_Target();
					Down();
					Backward_Point();
					//阶段2完成
					RightTurn_90_45();		//车头朝向E
					Forward_Get();			//前行至E点
					switch(GetColor())		//取得E点的颜色后，进行下一步动作
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							LeftTurn_90();		//车头调直(调至与x轴负方向成90度)
							Home();		//退到出发点
							break;
							
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();		/*可优化*/
							Forward_Target();			/*这里可以合并，原点处没必要停下*/
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							RightTurn_90();
							Home();
							break;
						
						case 4:				//E为黑色，退回至O，前行至4
							Catch();
							Turn_180();
							Forward_Get_Return();	//原路返回至O点
							RightTurn_90_45();		//车头指向D
							Forward_Target();		//从O点直接行至4点
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							RightTurn_45();			//调直车头
							Home();
							break;
					}
					break;
					
				case 4:		//A为黑色，退回至O，前行至4
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_90_45();
					Forward_Target();
					Down();
					Backward_Point();
					//阶段2完成
					RightTurn_90_45();		//车头朝向E
					Forward_Get();			//前行至E点
					switch(GetColor())		//取得E点的颜色后，进行下一步动作					
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							LeftTurn_90();		//车头调直(调至与x轴负方向成90度)
							Home();		//退到出发点
							break;
							
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();		/*可优化*/
							Forward_Target();			/*这里可以合并，原点处没必要停下*/
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							RightTurn_90();
							Home();
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();	//原路返回至O点
							RightTurn_45();			//车头指向B
							Forward_Target();		//从O点直接行至2点
							Down();
							Backward_Point();
							//阶段3完成，任务完成
							RightTurn_45();			//调直车头
							Home();
							break;
					}
					break;
					
				case 5:		//A为蓝色,先放在暂存点H，然后搬运E之后再回头搬运A
					Catch();
					Turn_180();
					Forward_Get_Retrun();
					RightTurn_90();			//车头指向H(S)
					Forward_Get();			//复用已有的函数，假定一个TempPoint H
					Down();
					Turn_180();				/*可优化，以后可以把Turn_180()写进Return函数里*/
					Forward_Get_Return();	//从H点回到原点
					RightTurn_90();
					Forward_Get();
					switch(GetColor())		//取得E点的颜色后，进行下一步动作					
					{
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();		/*可优化*/
							Forward_Target();			/*这里可以合并，原点处没必要停下*/
							Down();
							Backward_Point();
							//阶段2完成
							//RightTurn_90();
							//Home();				//此处和往常不一样，不能直接回去，还有A没有搬运
							LeftTurn_90();			//车头指向H
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();	//原路返回至O点
							RightTurn_45();			//车头指向B
							Forward_Target();		//从O点直接行至2点
							Down();
							Backward_Point();
							//阶段2完成
							//RightTurn_45();			//调直车头
							//Home();
							LeftTurn_90_45();		//车头指向H
							break;
							
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();	//原路返回至O点
							RightTurn_90_45();		//车头指向D
							Forward_Target();		//从O点直接行至4点
							Down();
							Backward_Point();
							//阶段2完成
							//RightTurn_45();			//调直车头
							//Home();
							RightTurn_90_45();		//车头指向H
							break;
					}
					Forward_Get();
					Catch();
					Turn_180();
					Forward_Get_Return();
					RightTurn_90();
					Forward_Target();
					Down();
					Backward_Point();
					LeftTurn_90();
					Home();
					//阶段3完成，任务完成
					break;
			}
			break;
			
		//***************C点为白色(2)***************
		case 2:			//C为白色
			Catch();
			Turn_180();
			Forward_Get_Return();
			RightTurn_90_45();
			Forward_Target();
			Down();
			Backward_Point();
			LeftTurn_45();
			Forward_Get();
			switch(GetColor())		//取得A的颜色
			{
				case 1:
					Catch();
					Forward_Put();
					Down();
					Backward_Point();		/*可优化，没必要先回O点*/
					Turn_180();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							Home();	
							break;
						
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							Home();			/*可优化，直接回到S点没必要在O点停留*/
							break;
						
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90_45();
							Forward_Target();
							Down();
							Backward_Point();
							LeftTurn_45();
							Home();
							break;
					}
					break;
					
				case 3:
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_90();
					Forward_Target();
					Down();
					Backward_Point();
					RightTurn_90();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							Home();
							break;
							
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();
							Forward_Target();		/*可优化，没必要在原点停留*/
							Down();
							Backward_Point();
							RightTurn_90();
							Home();
							break;
							
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90_45();
							Forward_Target();		
							Down();
							Backward_Point();
							LeftTurn_45();
							Home();
							break;
							
					}
					break;
					
				case 4:
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_45();
					Forward_Target();
					Down();
					Backward_Point();
					RightTurn_45();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							Home();
							break;
							
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();
							Forward_Target();		/*可优化，没必要在原点停留*/
							Down();
							Backward_Point();
							RightTurn_90();
							Home();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							Home();			/*可优化，直接回到S点没必要在O点停留*/
							break;
					}
					break;
					
				case 5:
					Catch();
					Turn_180();
					Forward_Get_Retrun();
					RightTurn_90();			//车头指向H(S)
					Forward_Get();			//复用已有的函数，假定一个TempPoint H
					Down();
					Turn_180();				/*可优化，以后可以把Turn_180()写进Return函数里*/
					Forward_Get_Return();	//从H点回到原点
					RightTurn_90();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();
							Forward_Target();		/*可优化，没必要在原点停留*/
							Down();
							Backward_Point();
							//RightTurn_90();
							//Home();
							LeftTurn_90();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							//Home();		
							Turn_180();	
							break;
							
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90_45();
							Forward_Target();		
							Down();
							Backward_Point();
							//LeftTurn_45();
							//Home();
							RightTurn_90_45();
							break;
							
					}
					Forward_Get();
					Catch();
					Turn_180();
					Forward_Get_Return();
					RightTurn_90();
					Forward_Target();
					Down();
					Backward_Point();
					LeftTurn_90();
					Home();
					break;
					
			}
			break;
		
		//***************C点为黑色(4)***************		
		case 4:			//C为黑色
			Catch();
			Turn_180();
			Forward_Get_Return();
			RightTurn_45();
			Forward_Target();
			Down();
			Backward_Point();
			LeftTurn_90_45();
			Forward_Get();
			switch(GetColor())		//取得A的颜色
			{
				case 1:
					Catch();
					Forward_Put();
					Down();
					Backward_Point();		/*可优化，没必要先回O点*/
					Turn_180();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							Home();
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();	
							RightTurn_45();			
							Forward_Target();		
							Down();
							Backward_Point();
							RightTurn_45();			
							Home();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							Home();			/*可优化，直接回到S点没必要在O点停留*/
							break;
					}
					break;
					
				case 2:
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_90_45();
					Forward_Target();
					Down();
					Backward_Point();
					RightTurn_90_45();		
					Forward_Get();			
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							Home();
							break;
							
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();
							Forward_Target();		/*可优化，没必要在原点停留*/
							Down();
							Backward_Point();
							RightTurn_90();
							Home();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							Home();			/*可优化，直接回到S点没必要在O点停留*/
							break;
					}
					break;
					
				case 3:
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_90();
					Forward_Target();
					Down();
					Backward_Point();
					RightTurn_90();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							Home();
							break;
							
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();
							Forward_Target();		/*可优化，没必要在原点停留*/
							Down();
							Backward_Point();
							RightTurn_90();
							Home();
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();	
							RightTurn_45();			
							Forward_Target();		
							Down();
							Backward_Point();
							RightTurn_45();			
							Home();
							break;
					}
					break;
					
				case 5:
					Catch();
					Turn_180();
					Forward_Get_Retrun();
					RightTurn_90();			//车头指向H(S)
					Forward_Get();			//复用已有的函数，假定一个TempPoint H
					Down();
					Turn_180();				/*可优化，以后可以把Turn_180()写进Return函数里*/
					Forward_Get_Return();	//从H点回到原点
					RightTurn_90();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 1:
							Catch();
							Turn_180();
							Forward_Get_Return();
							Forward_Target();		/*可优化，没必要在原点停留*/
							Down();
							Backward_Point();
							//RightTurn_90();
							//Home();
							LeftTurn_90();
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_45();
							Forward_Target();
							Down();
							Backward_Point();
							//RightTurn_45();
							//Home();
							LeftTurn_90_45();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							//Home();		
							Turn_180();	
							break;
					}
					Forward_Get();
					Catch();
					Turn_180();
					Forward_Get_Return();
					RightTurn_90();
					Forward_Target();
					Down();
					Backward_Point();
					LeftTurn_90();
					Home();
					break;
					
			}
			break;
		
		//***************C点为绿色(1)***************
		case 1:						//这种情况比较特殊，需要将C搬至暂存点，最后再处理它
			Catch();
			Turn_180();
			Forward_Get_Return();
			Forward_Get();
			Down();
			Turn_180();
			Forward_Get_Return();
			LeftTurn_90();
			Forward_Get();
			switch(GetColor())		//取得A的颜色
			{
				case 2:
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_90_45();
					Forward_Target();
					Down();
					Backward_Point();
					RightTurn_90_45();
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							RightTurn_90();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							Turn_180();
							break;
							
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90_45();
							Forward_Target();
							Down();
							Backward_Point();
							RightTrun_90_45();
							break;
					}
					break;
					
				case 3:
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_90();
					Forward_Target();
					Down();
					Backward_Point();
					RightTurn_90();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							RightTurn_90();
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_45();
							Forward_Target();
							Down();
							Backward_Point();
							LeftTurn_90_45();
							break;
							
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90_45();
							Forward_Target();
							Down();
							Backward_Point();
							RightTrun_90_45();
							break;
					}
					break;
					
				case 4:
					Catch();
					Turn_180();
					Forward_Get_Return();
					LeftTurn_45();
					Forward_Target();
					Down();
					Backward_Point();
					RightTurn_45();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 5:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							RightTurn_90();
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_45();
							Forward_Target();
							Down();
							Backward_Point();
							LeftTurn_90_45();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							Turn_180();
							break;
					}
					break;
					
				case 5:		//C1A5，这种情况是特殊中的特殊，不再设立第二暂存点，而是忽略它调整搬运顺序
					Turn_180();
					Forward_Get_Return();
					Forward_Get();
					switch(GetColor())		//取得E的颜色
					{
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_45();
							Forward_Target();
							Down();
							Backward_Point();
							LeftTurn_45();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							LeftTurn_90();
							break;
							
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();
							RightTurn_90_45();
							Forward_Target();
							Down();
							Backward_Point();
							LeftTurn_90_45();
							break;
					}
					Forward_Get();
					Catch();
					Turn_180();
					Forward_Get_Return();
					Forward_Target();
					Down();
					Backward_Point();
					RightTurn_90();
					break;
					
			}
			Forward_Get();
			Catch();
			Turn_180();
			Forward_Get_Return();
			LeftTurn_90();
			Forward_Target();
			Down();
			Backward_Point();
			RightTurn_90();
			Home();
			break;
		
		//***************C点为蓝色(5)***************
		case 5:		//这种情况比较特殊，需要将C搬至暂存点，最后再处理它，并且需要调整搬运顺序，先去搬E
			Catch();
			Turn_180();
			Forward_Get_Return();
			Forward_Get();
			Down();
			Turn_180();
			Forward_Get_Return();
			RightTurn_90();		//车头指向E
			Forward_Get();
			switch(GetColor())		//取得E的颜色
			{
				case 2:
					Catch();
					Turn_180();
					Forward_Get_Return();
					RightTurn_45();
					Forward_Target();
					Down();
					Backward_Point();
					LeftTurn_45();
					Forward_Get();
					switch(GetColor())		//取得A的颜色
					{
						case 1:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							Turn_180();
							break;
							
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_45();
							Forward_Target();
							Down();
							Backward_Point();
							RightTurn_90_45();
							break;
					}
					break;
					
				case 3:
					Catch();
					Turn_180();
					Forward_Get_Return();
					RightTurn_90();
					Forward_Target();
					Down();
					Backward_Point();
					LeftTurn_90();
					Forward_Get();
					switch(GetColor())		//取得A的颜色
					{
						case 1:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_90_45();
							Forward_Target();
							Down();
							Backward_Point();
							LeftTurn_90_45();
							break;
							
						case 4:
							Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_45();
							Forward_Target();
							Down();
							Backward_Point();
							RightTurn_90_45();
							break;
					}
					break;
					
				case 4:
					Catch();
					Turn_180();
					Forward_Get_Return();
					RightTurn_90_45();
					Forward_Target();
					Down();
					Backward_Point();
					LeftTurn_90_45();
					Forward_Get();
					switch(GetColor())		//取得A的颜色
					{
						case 1:
							Catch();
							Forward_Put();
							Down();
							Backward_Point();
							LeftTurn_90();
							break;
							
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_90_45();
							Forward_Target();
							Down();
							Backward_Point();
							LeftTurn_90_45();
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							Turn_180();
							break;
					}
					break;
					
				case 1:		//C5E1，这种情况是特殊中的特殊，忽略E再次调整搬运顺序
					Turn_180();
					Forward_Get_Return();
					Forward_Get();
					switch(GetColor())		//取得A的颜色
					{
						case 2:
							Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_90_45();
							Forward_Target();
							Down();
							Backward_Point();
							RightTurn_90_45();	//车头指向E
							break;
							
						case 3:
							Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_90();
							Forward_Target();
							Down();
							Backward_Point();
							RightTurn_90();	
							break;
							
						case 4:
						 	Catch();
							Turn_180();
							Forward_Get_Return();
							LeftTurn_45();
							Forward_Target();
							Down();
							Backward_Point();
							RightTurn_45();	
							break;
					}
					Forward_Get();
					Catch();
					Turn_180();
					Forward_Target();
					Down();
					Backward_Point();
					LeftTurn_90();			//车头指向A
					break;
					
			}
			Forward_Get();
			Catch();
			Turn_180();
			Forward_Get_Return();
			RightTurn_90();
			Forward_Target();
			Down();
			Backward_Point();
			LeftTurn_90();
			Home();
			break;
		
	}
	
	
}
 
//初始化定时器函数
void InitTimer()
{
    TMOD=0X01;
    TH0= 0XFC;      //1ms定时
    TL0= 0X66;
    TR0= 1;
    ET0= 1;    
}

//测试PWM循迹用
void Test()
{
    InitTimer();
    Forward();
    while(1)	
	{ 	
        //有信号为0，没信号为1
        if(R1==0&&R2==0&&R3==0&&R5==0&&R6==0&&R7==0&&MID==1)
	       Forward();   
        else
	    {	
	       if(R1==0&&R2==0&&R3==1&&R5==0&&R6==0&&R7==0&&MID==1)
	            Forward();
				  
           if(R1==0&&R2==0&&R3==0&&R5==1&&R6==0&&R7==0&&MID==1)
	            Forward();		  
				  
	       if(R1==0&&R2==0&&R3==1&&R5==0&&R6==0&&R7==0)
		        LeftTurn();
				
           if(R1==0&&R2==1&&R3==1&&R5==0&&R6==0&&R7==0)
		        LeftTurn();
				
		   if(R1==0&&R2==1&&R3==0&&R5==0&&R6==0&&R7==0)
			    LeftTurn();
				
		   if(R1==1&&R2==1&&R3==0&&R5==0&&R6==0&&R7==0)
				LeftTurn();
				
		   if(R1==1&&R2==0&&R3==0&&R5==0&&R6==0&&R7==0)
				LeftTurn();
				
		   if(R1==1&&R2==1&&R3==1&&R5==0&&R6==0&&R7==0)
				LeftTurn();
				
		   if(R1==0&&R2==0&&R3==0&&R5==1&&R6==0&&R7==0)
				RightTurn();
				
	       if(R1==0&&R2==0&&R3==0&&R5==1&&R6==1&&R7==0)
				RightTurn();
				
		   if(R1==0&&R2==0&&R3==0&&R5==0&&R6==1&&R7==0)
				RightTurn();
				
		   if(R1==0&&R2==0&&R3==0&&R5==0&&R6==1&&R7==1)
				RightTurn();
				
		   if(R1==0&&R2==0&&R3==0&&R5==0&&R6==0&&R7==1)
				RightTurn();
				
		   if(R1==0&&R2==0&&R3==0&&R5==1&&R6==1&&R7==1)
				RightTurn();
		}
	}
}
 

/**
*	说明几点可优化的地方！有些实现可能比较复杂，有一些只是建议而已，如果没有时间可以不采用
*	1、Turn_180()写进Forward_Get_Return()里，反正每次回原点都是要掉头的
*	2、从A到5，或者从E到1，走直线的路可以单独写一个函数直达，没有必要跟往常一样再先回原点停一下
*	3、Turn_180()我觉得后期为了省时间可以略去，直接带着色块倒车就行了，可以自己尝试实现一下
*	4、暂存点H可以不要用Forward_Get()去取，替之以更快的函数，不过考虑到车体和赛道的比例，可以不优化
*/

/*************本算法没有执行函数Running1()的封装后代码********************
**************为了程序完整性，暂时先采用如上写法，因其具有可读性*************
**************后期调试完成之后可以把Running1()封装一下********************
**************会简洁一点，但是可读性略差，效率也没有太多提高****************/

/**
*	下面提供竞赛第二阶段的执行算法，采用内道循环搬运，但是时间上可能会慢一点
*	如果有时间，请自己写一个我在文档里提到的，外道轮转搬运+动态避障算法，时间上会快一些
*/

//执行函数，竞赛第二阶段
void Running_2()
{
	Forward_S2O();	/*可优化，这里不必从S点开始，可以从O点开始，即第一阶段不需要Home()*/
	LeftTurn_90_45();		//先搬F点的5个色块
	for(int i=0; i<5; i++){
		Forward_Target_Get();
		switch(GetColor())
		{
			case 1:
				Catch();
				Backward_Point();
				RightTurn_45();
				Forward_Target();
				Down();
				Backward_Point();
				LeftTurn_45();
				break;
		
			case 2:
				Catch();
				Backward_Point();
				RightTurn_90();
				Forward_Target();
				Down();
				Backward_Point();
				LeftTurn_90();
				break;
		
			case 3:
				Catch();
				Backward_Point();
				RightTurn_90_45();
				Forward_Target();
				Down();
				Backward_Point();
				LeftTurn_90_45();
				break;
		
			case 4:
				Catch();
				Backward_Point();
				Turn_180();
				Forward_Target();
				Down();
				Backward_Point();
				Turn_180();
				break;
		
			case 5：
				Catch();
				Backward_Point();
				LeftTurn_90_45();
				Forward_Target();
				Down();
				Backward_Point();
				RightTurn_90_45();
				break;
		}
	}
	
	LeftTurn_90();		//再搬运G点的5个色块
	for(int j=0; j<5; j++){
		Forward_Target_Get();
		switch(GetColor())
		{
			case 1:
				Catch();
				Backward_Point();
				RightTurn_90_45();
				Forward_Target();
				Down();
				Backward_Point();
				LeftTurn_90_45();
				break;
		
			case 2:
				Catch();
				Backward_Point();
				Turn_180();
				Forward_Target();
				Down();
				Backward_Point();
				Turn_180();
				break;
		
			case 3:
				Catch();
				Backward_Point();
				LeftTurn_90_45();
				Forward_Target();
				Down();
				Backward_Point();
				RightTurn_90_45();
				break;
		
			case 4:
				Catch();
				Backward_Point();
				LeftTurn_90();
				Forward_Target();
				Down();
				Backward_Point();
				RightTurn_90();
				break;
		
			case 5：
				Catch();
				Backward_Point();
				LeftTurn_45();
				Forward_Target();
				Down();
				Backward_Point();
				RightTurn_45();
				break;
		}
	}
	
	LeftTurn_90_45();
	Home();
	
}


//执行函数，竞赛第三阶段，创新赛
void Running_3()
{
	//相对于前两阶段，这个是最简单的，运用已有函数，选个可靠的路径自己写一个吧
}


//主函数
void main(void)
{
	Delay(3000);		//延时1000ms
    Test();		//测试PWM及循迹
	//Running_1();
	//Delay(3000);
	//Running_2();
	//Delay(3000);
	//Running_3();
	Stop();
}

