#include "Find_Crossroads.h"

/* 借存储坐标变量 */
int16 top_L[2] = {0, 0};
int16 bot_L[2] = {0, 0};

int16 top_R[2] = {0, 0};
int16 bot_R[2] = {0, 0};

uint16 Crossroads_num = 0;
uint8 Crossroads_FLAG = 0;

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找十字
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
void Find_Crossroads(void)
{
	uint8 i = 0;
//	if(r_con == 0 && l_con != 0 && 
//		Right_Lost_Line_count <= 23 && Left_Lost_Line_count <= 5 && 
//			Right_Lost_Line_count >= 23 && Left_Lost_Line_count >= 5)
//	{
//		Right_Patch_Init();
//	}
//	if(l_con == 0 && r_con != 0 && 
//		Right_Lost_Line_count <= 23 && Left_Lost_Line_count <= 5 && 
//			Right_Lost_Line_count >= 23 && Left_Lost_Line_count >= 5)
//	{
//		Left_Patch_Init();
//	}
//	if(Left_Lost_Line_count >= 30 && Right_Lost_Line_count < 10)
//	{
//		Right_Patch_Init();
//	}
//	else if(Right_Lost_Line_count >= 30 && Left_Lost_Line_count < 10)
//	{
//		Left_Patch_Init();
//	}
//	else if(Left_Lost_Line_count >= 10 && Right_Lost_Line_count >= 10)
//	{
//		Lost_connection_left(40,110);
//	}
//	return;
	
	if(Left_Lost_Line_count >= 10 && Right_Lost_Line_count >= 10)
	{
		if(!Crossroads_FLAG)
		{
			Crossroads_FLAG = 1;
			Crossroads_num ++;
		}
		/* 找左上下角点 */
        Left_up_Patch = Find_left_up_point(30,110);
//        Left_dowm_Patch = Find_left_dowm_point(110,40);

        /* 找右上下角点 */
        Right_up_Patch = Find_Right_up_point(30,110);
//        Right_dowm_Patch = Find_Right_dowm_point(110,40);

        /* 第二级优先级判断十字情况 */
        //若四个点都存在
        if((Left_up_Patch != 0) && (Right_up_Patch != 0) && (Left_dowm_Patch != 0) && (Right_dowm_Patch != 0))
        {
            /* 左下 */
            bot_L[0] = (int16)Left_Line[Left_dowm_Patch];
            bot_L[1] = (int16)Left_dowm_Patch;

            /* 左上 */
            top_L[0] = (int16)Left_Line[Left_up_Patch];
            top_L[1] = (int16)Left_up_Patch;

            /* 右下 */
            bot_R[0] = (int16)Right_Line[Right_dowm_Patch];
            bot_R[1] = (int16)Right_dowm_Patch;

            /* 右上 */
            top_R[0] = (int16)Right_Line[Right_up_Patch];
            top_R[1] = (int16)Right_up_Patch;

            /* 画线 */
            Patch_line_Left(bot_L, top_L);
            Patch_line_Right(bot_R, top_R);
        }
//        //若上面两个角点存在
//        else if((Left_up_Patch != 0) && (Right_up_Patch != 0))
//        {
//            /* 左下 */
//            bot_L[0] = Left_Coordinates[118];
//            bot_L[1] = 118;

//            /* 左上 */
//            top_L[0] = (int16)Left_Line[Left_up_Patch];
//            top_L[1] = (int16)Left_up_Patch;

//            /* 右下 */
//            bot_R[0] = Right_Coordinates[118];
//            bot_R[1] = 118;

//            /* 右上 */
//            top_R[0] = (int16)Right_Line[Right_up_Patch];
//            top_R[1] = (int16)Right_up_Patch;

//            /* 画线 */
//            Patch_line_Left(bot_L, top_L);
//            Patch_line_Right(bot_R, top_R);
//        }
//        //若左边两个角点存在
//        else if((Left_up_Patch != 0) && (Left_dowm_Patch != 0))
//        {
//            /* 左下 */
//            bot_L[0] = (int16)Left_Line[Left_dowm_Patch];
//            bot_L[1] = (int16)Left_dowm_Patch;

//            /* 左上 */
//            top_L[0] = (int16)Left_Line[Left_up_Patch];
//            top_L[1] = (int16)Left_up_Patch;

//            /* 画线 */
//            Patch_line_Left(bot_L, top_L);
//        }
//        //若右边两个角点存在
//        else if((Right_up_Patch != 0) && (Right_dowm_Patch != 0))
//        {
//            /* 右下 */
//            bot_R[0] = (int16)Right_Line[Right_dowm_Patch];
//            bot_R[1] = (int16)Right_dowm_Patch;

//            /* 右上 */
//            top_R[0] = (int16)Right_Line[Right_up_Patch];
//            top_R[1] = (int16)Right_up_Patch;

//            /* 画线 */
//            Patch_line_Right(bot_R, top_R);
//        }
//		else
//		{
//			for (i = 110; i >= 10; i-=2)
//			{
//				if((Right_Line[i] >= MT9V03X_W - 20))
//				{
//					Right_Line[i] = Right_Coordinates[i];
//					image_copy_out[i][Right_Line[i]] = 0;
//				}
//				if((Left_Line[i] <= 20))
//				{
//					Left_Line[i] = Left_Coordinates[i];
//					image_copy_out[i][Left_Line[i]] = 0;
//				}
//			}
//		}
    }
	else
	{
		Crossroads_FLAG = 0;
	}
//	else
//	{
//		for (i = 110; i >= reference_col_farthest; i-=2)
//		{
//			if(Right_Line [i] >= MT9V03X_W - 8)
//			{
//				Right_Line[i] = Left_Line[i] + Road_Width_Table[i];
//				image_copy_out[i][Right_Line[i]] = 0;
//			}
//		}
//		
//		for (i = 110; i >= reference_col_farthest; i-=2)
//		{
//			if(Left_Line [i] == 8)
//			{
//				Left_Line[i] = Right_Line[i] - Road_Width_Table[i];
//				image_copy_out[i][Left_Line[i]] = 0;
//			}
//		}
//	}
//	Patch_line_Right_Init(30,100);
//	Patch_line_Left_Init(30,100);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     找十字
// 参数说明
// 返回参数
// 使用示例
// 备注信息
//------------------------------------------------------------------------------------------------------------------
//void Find_Crossroads(void)
//{
//    /* 第一级判断丢线是否符合十字 */
//    if((Left_Lost_Line_count >= 10) && (Right_Lost_Line_count >= 10))
//    {
//        /* 找左上下角点 */
//        Left_up_Patch = Find_left_up_point(30,100);
//        Left_dowm_Patch = Find_left_dowm_point(100,40);

//        /* 找右上下角点 */
//        Right_up_Patch = Find_Right_up_point(30,100);
//        Right_dowm_Patch = Find_Right_dowm_point(100,40);

//        /* 第二级优先级判断十字情况 */
//        //若四个点都存在
//        if((Left_up_Patch != 0) && (Right_up_Patch != 0) && (Left_dowm_Patch != 0) && (Right_dowm_Patch != 0))
//        {
//            /* 左下 */
//            bot_L[0] = (int16)Left_Line[Left_dowm_Patch];
//            bot_L[1] = (int16)Left_dowm_Patch;

//            /* 左上 */
//            top_L[0] = (int16)Left_Line[Left_up_Patch];
//            top_L[1] = (int16)Left_up_Patch;

//            /* 右下 */
//            bot_R[0] = (int16)Right_Line[Right_dowm_Patch];
//            bot_R[1] = (int16)Right_dowm_Patch;

//            /* 右上 */
//            top_R[0] = (int16)Right_Line[Right_up_Patch];
//            top_R[1] = (int16)Right_up_Patch;

//            /* 画线 */
//            Patch_line_Left(bot_L, top_L);
//            Patch_line_Right(bot_R, top_R);
//        }
//        //若上面两个角点存在
//        else if((Left_up_Patch != 0) && (Right_up_Patch != 0))
//        {
//            /* 左下 */
//            bot_L[0] = 30;
//            bot_L[1] = 118;

//            /* 左上 */
//            top_L[0] = (int16)Left_Line[Left_up_Patch];
//            top_L[1] = (int16)Left_up_Patch;

//            /* 右下 */
//            bot_R[0] = 158;
//            bot_R[1] = 118;

//            /* 右上 */
//            top_R[0] = (int16)Right_Line[Right_up_Patch];
//            top_R[1] = (int16)Right_up_Patch;

//            /* 画线 */
//            Patch_line_Left(bot_L, top_L);
//            Patch_line_Right(bot_R, top_R);
//        }
//        //若左边两个角点存在
//        else if((Left_up_Patch != 0) && (Left_dowm_Patch != 0))
//        {
//            /* 左下 */
//            bot_L[0] = (int16)Left_Line[Left_dowm_Patch];
//            bot_L[1] = (int16)Left_dowm_Patch;

//            /* 左上 */
//            top_L[0] = (int16)Left_Line[Left_up_Patch];
//            top_L[1] = (int16)Left_up_Patch;

//            /* 画线 */
//            Patch_line_Left(bot_L, top_L);
//        }
//        //若右边两个角点存在
//        else if((Right_up_Patch != 0) && (Right_dowm_Patch != 0))
//        {
//            /* 右下 */
//            bot_R[0] = (int16)Right_Line[Right_dowm_Patch];
//            bot_R[1] = (int16)Right_dowm_Patch;

//            /* 右上 */
//            top_R[0] = (int16)Right_Line[Right_up_Patch];
//            top_R[1] = (int16)Right_up_Patch;

//            /* 画线 */
//            Patch_line_Right(bot_R, top_R);
//        }
//    }



////    /* 找到角点并符合丢线要求 */
////    if((Left_dowm_Patch >= 30) && (Right_dowm_Patch >= 30) && (Left_Lost_Line_count >= 10) && (Right_Lost_Line_count >= 10) && (Find_Left_FLAG == Left_0) && (Find_Right_FLAG == Right_0))
////    {
//////        Buzzer_ON();
////        /* 左下坐标 */
////        bot_L[0] = (int16)Left_dowm_Patch_x;
////        bot_L[1] = (int16)Left_dowm_Patch;
////
////        top_L[0] = 40;
////        top_L[1] = 1;
////
////        /* 右下坐标 */
////        bot_R[0] = (int16)Right_dowm_Patch_x;
////        bot_R[1] = (int16)Right_dowm_Patch;
////
////        top_R[0] = 54;
////        top_R[1] = 1;
////
////        /* 画线 */
////        Patch_line_Left(bot_L, top_L);
////        Patch_line_Right(bot_R, top_R);
////    }
////    else Buzzer_OFF();
//}
