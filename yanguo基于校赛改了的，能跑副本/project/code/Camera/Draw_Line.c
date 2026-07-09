#include "Draw_Line.h"

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     输入两点坐标补左边线
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Patch_line_Left(int16 zuobiao_botton[2], int16 zuobiao_top[2])
{
    /* 斜率  常数项 */
    int16 K = 0, b = 0;
	int16 j;
    //k = (y2 - y1) / (x2 - x1);
    K = (zuobiao_top[1] - zuobiao_botton[1])*10 / (zuobiao_top[0] - zuobiao_botton[0]);

    b = (zuobiao_top[1]*10 - K * zuobiao_top[0]);

    for(j = zuobiao_top[1]; j < zuobiao_botton[1]; j ++)  //上到下 j是行
    {
        //x = (y - b) / k
        Left_Line[j] = (j*10 - b) / K;//Left_Line[j]是得到的列坐标
//		image_copy_out[j][Left_Line[j]] = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     输入两点坐标补右边线
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Patch_line_Right(int16 zuobiao_botton[2], int16 zuobiao_top[2])
{
    /* 斜率  常数项 */
    int16 K = 0.0f, b = 0.0f;
	int16 j;

    K = (zuobiao_top[1] - zuobiao_botton[1])*10 / (zuobiao_top[0] - zuobiao_botton[0]);

    b = (zuobiao_top[1]*10 - K * zuobiao_top[0]);

    for(j = zuobiao_top[1]; j < zuobiao_botton[1]; j ++)
    {
        Right_Line[j] = (j*10 - b) / K;
//		image_copy_out[j][Right_Line[j]] = 0;
    }
}


//-------------------------------------------------------------------------------------------------------------------
// 函数简介     根据右边界得到左边界
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Add_mid_left(int16 start, int16 end)
{
    int16 t;
	int16 H;
    //自动调整位置
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    for (H = start; H >= end; H--)
    {
        //左边界等于右边界加上赛道宽度
        Left_Line[H] = Right_Line[H] - 60;
        if(Left_Line[H] > 40) Left_Line[H] = 0;
//		image_copy_out[H][Left_Line[H]] = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     根据左边界得到右边界
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Patch_line_Right_Init(int16 start, int16 end)
{
	int16 t;
	int16 y;
	//自动调整位置
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    for(y = start; y < end; y++)
    {
        if(y >= reference_col_farthest) return;
        if(Right_Line[y] > Right_Coordinates[y]-10)
		{
			Right_Line[y] = Left_Line[y] + Road_Width_Table[y];
//			image_copy_out[y][Right_Line[y]] = 0;
		}
        // 左线保持不变
        // 右线 = 左线 + 当前行标准宽度
//        Right_Line[y] = Left_Line[y] + Ring_Width_Table[y];
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     根据右边界得到左边界
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Patch_line_Left_Init(int16 start, int16 end)
{
	int16 t;
	int16 y;
	//自动调整位置
    if (start < end)
    {
        t = start;
        start = end;
        end = t;
    }
    for(y = start; y < end; y++)
    {
        if(y >= reference_col_farthest) return;
        if(Left_Line[y] < Left_Coordinates[y]-10)
		{
			Left_Line[y] = Right_Line[y] - Road_Width_Table[y];
//			image_copy_out[y][Left_Line[y]] = 0;
		}
        // 右线保持不变
        // 左线 = 右线 - 当前行标准宽度
//        Left_Line[y] = Right_Line[y] - Ring_Width_Table[y];
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     根据左边界得到右边界
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Right_Patch_Init(void)
{
	int16 y;
    for(y = MT9V03X_H - 10; y >= reference_col_farthest; y-=2)
    {
        // 左线保持不变
        // 右线 = 左线 + 当前行标准宽度
        Right_Line[y] = Left_Line[y] + Road_Width_Table[y];
//		image_copy_out[y][Right_Line[y]] = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     根据右边界得到左边界
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Left_Patch_Init(void)
{
	int16 y;
    for(y = MT9V03X_H - 10; y >= reference_col_farthest; y-=2)
    {
        // 右线保持不变
        // 左线 = 右线 - 当前行标准宽度
        Left_Line[y] = Right_Line[y] - Road_Width_Table[y];
//		image_copy_out[y][Left_Line[y]] = 0;
    }
}
