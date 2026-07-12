#include "Laser.h"


uint16 tar_i;   //看关灯时间索引计时
uint8  tar_flag=0;//发现目标flag
uint8  tar_x;    //坐标x
uint8  tar_y;	 //坐标y

void laser_init(void)
{
	gpio_init(IO_P67, GPO, 0, GPO_PUSH_PULL);
}

void laser_on(void)
{
	gpio_set_level(IO_P67, 1);
}

void laser_off(void)
{
	gpio_set_level(IO_P67,0);
}


uint8 tar_th;
uint8 top_d=0,under_d=0,l_d=0,r_d=0;


uint8 debug_stage = 0;

#define Pre_Line   45
uint8 pre_find_offset=0;
//预扫描函数  找到找到后偏移量赋值给寻靶函数
void Pre_Scan(void){
	uint8 i,x,pre_l=0,pre_r=0;
	uint8 l_edge=left_edge_line[Pre_Line]+4;
	uint8 r_edge=right_edge_line[Pre_Line]-4;
	
	//从左向右
	for(i=l_edge;i<r_edge;i+=2){
		x=mt9v03x_image[Pre_Line][i];
		if(x<tar_th){
			pre_l=i;
			break;
		}
	}
	//从右向左
	for(i=r_edge;i<l_edge;i-=2){
		x=mt9v03x_image[Pre_Line][i];
		if(x<tar_th){
			pre_r=i;
			break;
		}
	}
	

	//判断可不可以用
	if(pre_r-pre_l<8 || pre_l > pre_r || pre_l == 0 || pre_r == 0){
		pre_find_offset=0;
		return ;
	}
	else {
		
		pre_find_offset=((pre_l+pre_r)>>1)-(SEARCH_IMAGE_W>>1);
	}
}


#define TopFind      25
#define BottomFind   110
#define MinTargetH   10
#define MinTargetW   10
#define FindOffset   4

void Target_find(int8 midOffset)
{
    uint8 i,j;
    uint8 center_x;
    uint8 tar_y_local;
    uint8 far *row_ptr;

    tar_flag = 0;
    debug_stage = 0;

    top_d = 0;
    under_d = 0;
    l_d = 0;
    r_d = 0;//都是圆的边界信息

    center_x =( SEARCH_IMAGE_W >> 1) + midOffset;      // 94
																		
    /************* 1. 从 TopFind 往下找上边界 *************/
    for(i = TopFind; i < BottomFind; i+=FindOffset)									
    {																		
        if(mt9v03x_image[i][center_x] < tar_th)									
        {																		
            top_d = i;									
            debug_stage = 1;									
            break;									
        }									
    }																	
    if(i >= SEARCH_IMAGE_H) return;		

    /************* 2. 从 BottomFind 往上找下边界 *************/
    for(i = BottomFind; i > TopFind; i-=FindOffset)
    {
        if(mt9v03x_image[i][center_x] < tar_th)
        {
            under_d = i;
            debug_stage = 2;
            break;
        }
    }
    if(i == 0) return;

    /************* 3. 合法性判断 *************/
    if(under_d <= top_d) return;
    if((under_d - top_d) < MinTargetH) return;

    tar_y_local = (top_d + under_d) >> 1;
    debug_stage = 3;

    /************* 4. 取目标中心行指针 *************/
    row_ptr = mt9v03x_image[tar_y_local];

    /************* 5. 从中间往左找左边界 *************/
    for(j = center_x; j >= FindOffset; j -= FindOffset)
    {
        if(row_ptr[j] < tar_th)
        {
            l_d = j;
            debug_stage = 4;
            break;
        }

        if(j == 0) return;
    }

    /************* 6. 从中间往右找右边界 *************/
    for(j = center_x; j < SEARCH_IMAGE_W - FindOffset; j+=FindOffset)
    {
        if(row_ptr[j] < tar_th)
        {
            r_d = j;
            debug_stage = 5;
            break;
        }
    }
    if(j >= SEARCH_IMAGE_W) return;

    /************* 7. 宽度判断 *************/
    if(r_d <= l_d) return;
    if((r_d - l_d) < MinTargetW) return;

    /************* 8. 贴边过滤 *************/
    if(l_d == 0) return;
    if(r_d >= (SEARCH_IMAGE_W - 1)) return;
    if(top_d == 0) return;
    if(under_d >= (SEARCH_IMAGE_H - 1)) return;

    /************* 9. 求中心 *************/
    tar_x = (l_d + r_d) >> 1;
    tar_y = tar_y_local;

    tar_flag = 1;
    debug_stage = 6;
	
}

