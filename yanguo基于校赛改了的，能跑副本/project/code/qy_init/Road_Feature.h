#ifndef __ROAD_FEATURE_H_
#define __ROAD_FEATURE_H_

#include "qy_headfile.h"

/* ================================================================ */
/* Road_Wave 波形特征提取模块                                          */
/* 从 Road_Wide 数组中提取直道、弯道、S弯的波形特征                     */
/* 环岛检测使用你已经有的 Find_Ring() 状态机，本模块不做重复检测          */
/* 调用时机：Find_Boundry_LongWhiteCol() 之后，Mid_Error_Processing() 之前 */
/* ================================================================ */

/* ==================== 阈值配置（可按需调整） ==================== */
#define ROAD_STRAIGHT_THR       8      // |Road_Wide| < 此值视为直道
#define ROAD_SLOPE_THR          3      // 一阶差分超过此值视为有变化趋势
#define ROAD_PEAK_MIN           12     // |peak| 小于此值不算有效弯道
#define ROAD_ZC_HYSTERESIS      8      // 过零检测滞回区间
#define ROAD_ZC_MIN_SPAN        10     // 过零最小间距（过滤跳变）

/* ==================== 分段边界 ==================== */
/* 图像行号 0(远) → 119(近)，车在行号大的一侧         */
/* 参考线 reference_col_farthest 动态变化，近场固定    */
#define ROAD_FAR_END            52
#define ROAD_MID_START          54
#define ROAD_MID_END            86
#define ROAD_NEAR_START         88
#define ROAD_NEAR_END           (MT9V03X_H - 10)

/* ==================== 赛道元素枚举 ==================== */
typedef enum
{
    TRACK_UNKNOWN      = 0,     // 未知
    TRACK_STRAIGHT,              // 直道
    TRACK_LEFT_CURVE,            // 左弯
    TRACK_RIGHT_CURVE,           // 右弯
    TRACK_S_CURVE,               // S弯
    TRACK_LEFT_RING,             // 左环岛（由 Find_Ring 状态决定）
    TRACK_RIGHT_RING,            // 右环岛
    TRACK_LOST                   // 严重丢线

}TrackType_Type;

/* ==================== 分段统计结构体 ==================== */
/* seg[0]=远场，seg[1]=中场，seg[2]=近场                 */
typedef struct
{
    int16  avg;                  // 平均偏移（>0偏左，<0偏右）
    int16  peak;                 // 峰值（带符号）
    int16  peak_row;             // 峰值所在行号（弯道顶点）
    int16  straight_cnt;         // 该段内连续直道的行数
    int16  sample_cnt;           // 有效采样点数
    int16  total_cnt;            // 该段总行数

}RoadSeg_Type;

/* ==================== 总体特征结构体 ==================== */
typedef struct
{
    TrackType_Type type;         // 赛道类型
    int16  confidence;           // 置信度 0~100
    int16  peak_row;             // 弯道顶点行号（用于前瞻控制）
    int16  zero_cross_row;       // S弯过零点，-1=无
    uint8  is_entering;          // 是否正在入弯
    uint8  is_exiting;           // 是否正在出弯

}TrackFeature_Type;

/* ==================== 全局结果变量 ==================== */
extern RoadSeg_Type      road_seg[3];             // 三段统计
extern int16             road_diff[MT9V03X_H];    // 一阶差分
extern int16             road_diff2[MT9V03X_H];   // 二阶差分
extern uint8             road_valid[MT9V03X_H];   // 每行有效性标记
extern TrackFeature_Type g_track_feature;         // 总体特征

/* ==================== 函数声明 ==================== */
void Extract_Road_Features(void);   // 主入口：提取所有特征
void Clear_Road_Features(void);     // 清空特征
uint8 Is_Ring_Exiting(void);        // 出环趋势检测（基于Road_Wide回落）

#endif /* __ROAD_FEATURE_H_ */
