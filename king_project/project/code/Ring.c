#include "Ring.h"

//圆环的7个状态(用个蛋的枚举浪费算力,卧槽)
//left_control_line[SEARCH_IMAGE_H]
//right_control_line[SEARCH_IMAGE_H]补线就是操作这个两个数组
uint8 left_control_line[SEARCH_IMAGE_H];	
uint8 right_control_line[SEARCH_IMAGE_H];
uint8 current_step=0;
uint8 key_anlysis1=0,key_anlysis2=0,key_anlysis3=0;
//圆环状态机
void Ring(void)
{
	switch (current_step)
	{
		//看看有没圆环
		case 0:
			Ring_Pre_Meet();
			if(ring_preMeet_flag)
			{
				current_step=1;
			}
		case 1:
			Ring_First_meeting();
			if(first_meeting_flag)//key_anlysis1
			{
				current_step=2;
			}
			break;
		//是否是状态2，现在已经进入圆环
		case 2:
			Ring_Enter();
			if(ring_enter_flag)
			{
				current_step=3;
			}
			break;
		case 3:
			Ring_Turing();
			if(ring_turn_flag)
			{
				current_step=4;
			}
			break;
		case 4:
			Ring_Ring_Ring();//爱的和弦铃，耶！
			if(Out_flag)
			{
				current_step=5;
			}
			break;
		case 5:
			Ring_Out();
			if(Straighten_flag)
			{
				current_step=6;
			}
			break;
		case 6:
			Ring_Straighten();
			if(ring_over_flag)
			{
				current_step=7;
			}
			break;
		case 7:
			Ring_Over();
			break;
	}
		

}

// 限制列坐标在 0 - SEARCH_IMAGE_W-1
uint8 clamp_col_int16(int16 val)
{
    if(val < 0) return 0;
    if(val >= SEARCH_IMAGE_W)return (SEARCH_IMAGE_W - 1);
    return (uint8)val;
}

void draw_line(uint8 left_point, uint8 right_point, uint8 to_flag)
{
    float slope;
    uint8 i;
    uint8 start_row, end_row;
    uint8 start_col, end_col;
    
    // 确定起点和终点
    if(left_point > right_point) {
        start_row = right_point;
        end_row = left_point;
        start_col = right_control_line[right_point];  // 右控制线的值
        end_col = left_control_line[left_point];      // 左控制线的值
    } else {
        start_row = left_point;
        end_row = right_point;
        start_col = left_control_line[left_point];    // 左控制线的值
        end_col = right_control_line[right_point];    // 右控制线的值
    }
    
    // 检查是否在同一行
    if(end_row == start_row) return;
    
    // 计算斜率
    slope = (float)(end_col - start_col) / (end_row - start_row);
    
    // 根据to_flag选择填充哪条控制线
    if(!to_flag) { // 填充左控制线
        for(i = start_row + 1; i < end_row; i++) {
            left_control_line[i] = (uint8)(start_col + slope * (i - start_row) + 0.5f);
        }
    }
    else { // 填充右控制线
        for(i = start_row + 1; i < end_row; i++) {
            right_control_line[i] = (uint8)(start_col + slope * (i - start_row) + 0.5f);
        }
    }
}


void connect_point(uint8 under, uint8 top, uint8 l_r)
{
    float slope;
    uint8 i;
    
    if(!l_r) { // 左边
        // 计算斜率: (下方值 - 上方值) / (下方行 - 上方行)
        slope = (float)(left_control_line[under] - left_control_line[top]) / (under - top);
        
        // 从上方的下一行到下方的上一行进行插值
        for(i = top + 1; i < under; i++) {
            left_control_line[i] = left_control_line[top] + slope * (i - top);
        }
    }
    else { // 右边
        slope = (float)(right_control_line[under] - right_control_line[top]) / (under - top);
        
        for(i = top + 1; i < under; i++) {
            right_control_line[i] = right_control_line[top] + slope * (i - top);
        }
    }
}


//判断是否连续的函数
uint8 isContinueLine(uint8* arr)
{
	int16 i;
	uint8 bad_cnt = 0;
	uint8 valid_cnt = 0;
	for(i = SEARCH_IMAGE_H-1-PIXEL_OFFSET; i > 7*PIXEL_OFFSET; i -= PIXEL_OFFSET)
	{
		uint8 curr = arr[i];
		uint8 prev = arr[i - PIXEL_OFFSET];
		if(abs(curr-prev)>40){
			return 0;
		}
	}
	return 1;
}

uint8 ring_preMeet_flag=0;
uint8 Ring_Pre_Meet(void)
{
	uint8 r_con,l_con;
	r_con=isContinueLine(right_edge_line);
	l_con=isContinueLine(left_edge_line);

	if(r_con != l_con)
	{
		ring_preMeet_flag=1;
		return 1;
	}
	else
	{
		ring_preMeet_flag=0;
		return 0;
	}
		
}

uint8 Ring_Pre_Meet_use(void)
{
	uint8 r_con,l_con;
	r_con=isContinueLine(right_edge_line);
	l_con=isContinueLine(left_edge_line);

	if(r_con != l_con)
	{
		return 1;
	}
	else
	{
		return 0;
	}
		
}



uint8 ring_l,ring_r;	//圆环左右标志位
uint8 first_meeting_flag=0;	//初见圆环标志位
void Ring_First_meeting(void)
{
	uint8 i;//,j;//遍历索引  j是找上拐点的索引
	uint8 under,top,mid;
	
	uint8 turnPoint=119,midPoint=0;
	
	if(!Ring_Pre_Meet_use()){
		current_step=0;
		ring_preMeet_flag=0;
	}
	
	for(i=SEARCH_IMAGE_H-1-PIXEL_OFFSET;i>30;i-=PIXEL_OFFSET)//下拐点
	{
		under=left_edge_line[i];
		top=left_edge_line[i-PIXEL_OFFSET];
	
		if((under-top) >=15){
			turnPoint=i;
			break;
		}
	}
	

	for(i=turnPoint-PIXEL_OFFSET;i>PIXEL_OFFSET;i-=PIXEL_OFFSET)
	{
		under=left_edge_line[i+PIXEL_OFFSET];
		mid=  left_edge_line[i];
		top=  left_edge_line[i-PIXEL_OFFSET];
	
		if(mid>=under && mid>=top && mid>30){
			midPoint=i;
		}
	}


	
	if(turnPoint && midPoint)
		connect_point(turnPoint,midPoint,0);
	
	
	//终止条件    //少了弯道的元素互斥
	if(midPoint && left_edge_line[115]<=2 && left_edge_line[85]<=2 &&left_edge_line[55]<=2)
	{
		first_meeting_flag=1;
	}

	
}


uint8 ring_enter_flag;
static uint8 mid_under_flag=0;
void Ring_Enter(void)
{
	uint8 i,under,mid,top,ttop,uunder;//行索引  j是为了找到最左端边界设置的索引
	uint8 midPoint;
	
	for(i=SEARCH_IMAGE_H -1 - 2 * PIXEL_OFFSET;i>PIXEL_OFFSET*2;i-=PIXEL_OFFSET)
	{	
		uunder=left_edge_line[i+PIXEL_OFFSET+PIXEL_OFFSET];
		under=left_edge_line[i+PIXEL_OFFSET];
		mid=  left_edge_line[i]; 
		top=  left_edge_line[i-PIXEL_OFFSET];
		ttop=left_edge_line[i-PIXEL_OFFSET-PIXEL_OFFSET];
		if(mid<8)continue;
		if(    mid>=under && (mid-under)<20
			&& under>=uunder && (under-uunder)<20
			&& mid>=top   && (mid-top)<20
			&& top>=ttop   && (top-ttop)<20
			&& mid>30)
		{
			midPoint=i;
			break;
		}
		
	}
	if(midPoint)
		connect_point(119,midPoint,0);
	else{//回到上一步
		first_meeting_flag = 0;
		current_step = 1;
	}
	if(midPoint>=60)
		mid_under_flag=1;
	
	if(mid_under_flag && midPoint<45)
	{
		ring_enter_flag=1;
	}
		
}


//状态3补线拉到右下角，找左边上拐点
uint8 ring_turn_flag=0;
void Ring_Turing(void)
{
	uint8 i,under,top;
	uint8 leftTopPoint;

	for(i=SEARCH_IMAGE_H-1;i>PIXEL_OFFSET;i-=PIXEL_OFFSET)
	{
		top=left_edge_line[i-PIXEL_OFFSET];
		under=left_edge_line[i];
	
		if(abs(top-under)>30){
			leftTopPoint=i-PIXEL_OFFSET;
			break;
		}																	
	}										
	draw_line(leftTopPoint,119,1);

	
	if(leftTopPoint>=controlReferenceLine-20){
		ring_turn_flag=1;														
	}													
															
}																
	

//今天从这里开始
//现在是在环里转圈，不断检测是否到达边界
uint8 Out_flag=0;//出环状态的标志
void Ring_Ring_Ring(void)
{
	uint8 i;//索引
	uint8 minPoint,index,minn=188;
	
	if(left_control_line[controlReferenceLine]>Mid_Col){
		left_control_line[controlReferenceLine]=0;
	}
	
	for(i=SEARCH_IMAGE_H-1;i>PIXEL_OFFSET*3;i-=PIXEL_OFFSET)
	{
		index=right_edge_line[i];
		if(index<minn){
			minn=index;
			minPoint=i;
		}																
	}
	if(
		(right_edge_line[minPoint-PIXEL_OFFSET]-right_edge_line[minPoint]<30
	  && right_edge_line[minPoint+PIXEL_OFFSET]-right_edge_line[minPoint]<30)
		||  minPoint<40 ){
		draw_line(0,119,1);
	}
	
	if(	right_edge_line[50]>180	&&
		right_edge_line[60]>180	&&
		right_edge_line[70]>180	&&
		right_edge_line[80]>180	&&
		right_edge_line[90]>180	&&
		left_edge_line[50]<2){
		Out_flag=1;
	}

}

	

uint8 Straighten_flag=0;//出环摆正标志位
void Ring_Out(void)//这个十分的对
{
	
	
	draw_line(0,119,1);
	if(	(left_edge_line[30]>16   || left_edge_line[40]>16   || left_edge_line[50]>16) 
		&&(right_edge_line[40]-left_edge_line[40]<170)
		&&(right_edge_line[50]-left_edge_line[50]<170)
		&&(right_edge_line[60]-left_edge_line[60]<170)
	)
	{
		Straighten_flag=1;
	}
	
	
}


//找左边上拐点,补线到左下角
uint8 ring_over_flag=0;
uint16 cnt_over=0;
void Ring_Straighten(void)
{
	uint8 i;//i是右边的索引，j是左边的索引
	uint8 under,mid,top;//遍历x右边线
	uint8 midPoint;

	for(i=SEARCH_IMAGE_H-1-PIXEL_OFFSET;i>PIXEL_OFFSET;i-=PIXEL_OFFSET)
	{
		under=left_edge_line[i+PIXEL_OFFSET];
		mid=  left_edge_line[i];
		top=  left_edge_line[i-PIXEL_OFFSET];
	
		if(mid>=under && mid>=top && mid>30){
			midPoint=i;
		}
	}
	if(midPoint){
		connect_point(119,midPoint,0);
	}
		
	
	if(isContinueLine(left_edge_line) && isContinueLine(right_edge_line)){	
		ring_over_flag=1;
	}

}

//结束标志位清零，然后重返状态一
void Ring_Over(void)
{
    first_meeting_flag = 0;
    ring_l = 0;
    ring_r = 0;
    ring_enter_flag = 0;
    ring_turn_flag = 0;
    Out_flag = 0;
    Straighten_flag = 0;
    ring_over_flag = 0;
    mid_under_flag = 0;
    current_step = 0;
    cnt_over = 0;
}



