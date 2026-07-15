#include "Road_Feature.h"

/* ================================================================ */
/* 全局结果变量定义                                                     */
/* ================================================================ */
RoadSeg_Type      road_seg[3]       = {0};
int16             road_diff[MT9V03X_H]   = {0};
int16             road_diff2[MT9V03X_H]  = {0};
uint8             road_valid[MT9V03X_H]  = {0};
TrackFeature_Type g_track_feature        = {0};

/* 滤波后的数据副本（特征分析使用，不修改原始 Road_Wide） */
static int16 road_filtered[MT9V03X_H];

/* 出环检测历史 */
static int16 last_near_avg = 0;

/* ================================================================ */
/* 内部函数声明                                                        */
/* ================================================================ */
static void Validate_Road_Wide(void);
static void Median_Filter_Road(void);
static void Calc_Diff(void);
static void Calc_Segment_Stats(void);
static int16 Find_Zero_Crossing(void);
static uint8 Is_Entering_Curve(int16 start_row, int16 end_row);
static uint8 Is_Exiting_Curve(int16 start_row, int16 end_row);
static void Classify_Track(void);

/* ================================================================ */
// 函数名称    清空特征
// 函数说明    将所有特征结果归零
// ================================================================ */
void Clear_Road_Features(void)
{
    uint8 i;

    for(i = 0; i < 3; i++)
    {
        road_seg[i].avg          = 0;
        road_seg[i].peak         = 0;
        road_seg[i].peak_row     = 0;
        road_seg[i].straight_cnt = 0;
        road_seg[i].sample_cnt   = 0;
        road_seg[i].total_cnt    = 0;
    }

    for(i = 0; i < MT9V03X_H; i++)
    {
        road_diff[i]     = 0;
        road_diff2[i]    = 0;
        road_valid[i]    = 1;
        road_filtered[i] = 0;
    }

    g_track_feature.type           = TRACK_UNKNOWN;
    g_track_feature.confidence     = 0;
    g_track_feature.peak_row       = 0;
    g_track_feature.zero_cross_row = -1;
    g_track_feature.is_entering    = 0;
    g_track_feature.is_exiting     = 0;
}

/* ================================================================ */
// 函数名称    主入口：提取 Road_Wide 波形特征
// 函数说明    不修改 Road_Wide 数组，不影响 Mid_Error_Processing
// 调用时机    Find_Boundry_LongWhiteCol() 之后
// ================================================================ */
void Extract_Road_Features(void)
{
    Clear_Road_Features();

    /* 1. 数据有效性过滤 */
    Validate_Road_Wide();

    /* 2. 一维中值滤波到独立缓冲区 road_filtered */
    Median_Filter_Road();

    /* 3. 一阶/二阶差分 */
    Calc_Diff();

    /* 4. 分段统计 */
    Calc_Segment_Stats();

    /* 5. 赛道分类 */
    Classify_Track();
}

/* ================================================================ */
// 函数名称    数据有效性过滤
// 函数说明    标记一行 Road_Wide 是否可信
// ================================================================ */
static void Validate_Road_Wide(void)
{
    int16 i;

    for(i = ROAD_NEAR_END; i >= reference_col_farthest; i -= 2)
    {
        road_valid[i] = 1;

        /* 规则1：左右任意一侧丢到图像边界 → 此行不可信 */
        if(Left_Line[i] == 8 || Right_Line[i] >= MT9V03X_W - 8)
        {
            road_valid[i] = 0;
        }

        /* 规则2：Road_Wide 超出物理可能范围 → 跳变不可信 */
        if(abs(Road_Wide[i]) > 50)
        {
            road_valid[i] = 0;
        }
    }
}

/* ================================================================ */
// 函数名称    中值滤波
// 函数说明    3 点中值，结果写入 road_filtered
//             不修改原始 Road_Wide 数组
// ================================================================ */
static void Median_Filter_Road(void)
{
    int16 i, a, b, c, tmp;

    /* 复制原始数据到 road_filtered */
    for(i = ROAD_NEAR_END; i >= reference_col_farthest; i -= 2)
    {
        road_filtered[i] = Road_Wide[i];
    }

    /* 3 点中值滤波 */
    for(i = ROAD_NEAR_END - 2; i >= reference_col_farthest + 2; i -= 2)
    {
        if(!road_valid[i]) continue;

        a = road_filtered[i - 2];
        b = road_filtered[i];
        c = road_filtered[i + 2];

        if(a > b) { tmp = a; a = b; b = tmp; }
        if(b > c) { tmp = b; b = c; c = tmp; }
        if(a > b) { tmp = a; a = b; b = tmp; }

        road_filtered[i] = b;
    }
}

/* ================================================================ */
// 函数名称    计算差分
// 函数说明    基于 road_filtered 计算，不影响原始 Road_Wide
// ================================================================ */
static void Calc_Diff(void)
{
    int16 i;

    /* 一阶差分 */
    for(i = ROAD_NEAR_END; i >= reference_col_farthest + 2; i -= 2)
    {
        road_diff[i] = road_filtered[i] - road_filtered[i - 2];
    }
    road_diff[reference_col_farthest] = 0;
    if((reference_col_farthest + 2) <= ROAD_NEAR_END)
    {
        road_diff[reference_col_farthest + 2] = 0;
    }

    /* 二阶差分 */
    for(i = ROAD_NEAR_END; i >= reference_col_farthest + 4; i -= 2)
    {
        road_diff2[i] = road_diff[i] - road_diff[i - 2];
    }
}

/* ================================================================ */
// 函数名称    分段统计
// 函数说明    远/中/近三段，每段计算 avg / peak / straight_cnt
//             基于 road_filtered，跳过 road_valid=0 的行
// ================================================================ */
static void Calc_Segment_Stats(void)
{
    int16  i, sum, cnt, peak_abs;
    uint8  straight_len, max_straight;
    int16  seg_start[3], seg_end[3];
    uint8  s;

    /* 分段边界 [start, end]，start=大(近)，end=小(远) */
    seg_start[0] = ROAD_MID_END;
    seg_end[0]   = reference_col_farthest;  /* 远场 */

    seg_start[1] = ROAD_NEAR_END - 2;
    seg_end[1]   = ROAD_MID_START;           /* 中场 */

    seg_start[2] = ROAD_NEAR_END;
    seg_end[2]   = ROAD_NEAR_START;          /* 近场 */

    for(s = 0; s < 3; s++)
    {
        sum = 0; cnt = 0;
        peak_abs = 0;
        straight_len = 0; max_straight = 0;

        road_seg[s].peak      = 0;
        road_seg[s].peak_row  = 0;
        road_seg[s].total_cnt = 0;

        for(i = seg_start[s]; i >= seg_end[s]; i -= 2)
        {
            road_seg[s].total_cnt++;

            if(road_valid[i])
            {
                sum += road_filtered[i];
                cnt++;

                /* 找峰值 */
                if(abs(road_filtered[i]) > peak_abs)
                {
                    peak_abs = abs(road_filtered[i]);
                    road_seg[s].peak     = road_filtered[i];
                    road_seg[s].peak_row = i;
                }

                /* 连续直道计数 */
                if(abs(road_filtered[i]) < ROAD_STRAIGHT_THR)
                {
                    straight_len += 2;
                    if(straight_len > max_straight)
                        max_straight = straight_len;
                }
                else
                {
                    straight_len = 0;
                }
            }
        }

        road_seg[s].avg          = (cnt > 0) ? sum / cnt : 0;
        road_seg[s].sample_cnt   = cnt;
        road_seg[s].straight_cnt = max_straight;
    }
}

/* ================================================================ */
// 函数名称    入弯检测
// 函数说明    一段波形中超过60%的diff为正 → 入弯
// ================================================================ */
static uint8 Is_Entering_Curve(int16 start_row, int16 end_row)
{
    int16  i;
    uint8  pos_cnt, total;

    pos_cnt = 0; total = 0;

    for(i = start_row; i >= end_row; i -= 2)
    {
        if(!road_valid[i]) continue;
        total++;
        if(road_diff[i] > ROAD_SLOPE_THR)
            pos_cnt++;
    }
    return (total > 3 && pos_cnt > total * 3 / 5);
}

/* ================================================================ */
// 函数名称    出弯检测
// 函数说明    一段波形中超过60%的diff为负 → 出弯
// ================================================================ */
static uint8 Is_Exiting_Curve(int16 start_row, int16 end_row)
{
    int16  i;
    uint8  neg_cnt, total;

    neg_cnt = 0; total = 0;

    for(i = start_row; i >= end_row; i -= 2)
    {
        if(!road_valid[i]) continue;
        total++;
        if(road_diff[i] < -ROAD_SLOPE_THR)
            neg_cnt++;
    }
    return (total > 3 && neg_cnt > total * 3 / 5);
}

/* ================================================================ */
// 函数名称    滞回过零检测
// 函数说明    从正→负（或反之）的切换点
//             死区 ±ROAD_ZC_HYSTERESIS 过滤噪声
//             最小间距 ROAD_ZC_MIN_SPAN 过滤跳变
// 返 回 值    过零点行号，-1=无过零
// ================================================================ */
static int16 Find_Zero_Crossing(void)
{
    int16 i, last_cross_row;
    int8  last_zone, current_zone;

    last_zone      = 0;
    last_cross_row = -1;

    for(i = ROAD_NEAR_END; i >= reference_col_farthest; i -= 2)
    {
        if(!road_valid[i]) continue;

        if(road_filtered[i] > ROAD_ZC_HYSTERESIS)
            current_zone = 1;
        else if(road_filtered[i] < -ROAD_ZC_HYSTERESIS)
            current_zone = -1;
        else
            current_zone = 0;  /* 死区，不触发 */

        /* 有效过零：正区→负区，或负区→正区 */
        if(last_zone != 0 && current_zone != 0 && last_zone != current_zone)
        {
            /* 间距太短 → 跳变噪声，忽略 */
            if(last_cross_row != -1 && (last_cross_row - i) < ROAD_ZC_MIN_SPAN)
            {
                last_cross_row = i;
                continue;
            }
            return i;  /* 第一个真实过零点 */
        }

        if(current_zone != 0)
            last_zone = current_zone;
    }
    return -1;
}

/* ================================================================ */
// 函数名称    赛道分类
// 函数说明    综合所有特征做最终赛道元素判定
//             环岛检测基于 Road_Wide 波形特征（幅度+方向一致性+阶跃跳变）
// ================================================================ */
static void Classify_Track(void)
{
    int16  zero_cross;
    uint8  straight_segments, i;
    int16  ref_avg, ref_peak, ref_peak_row;

    /* ============================================================ */
    /* 优先级0：环岛检测 — 基于 Road_Wide 波形特征                    */
    /*                                                              */
    /* 环岛与普通弯道的波形差异：                                     */
    /*   ① 偏移幅度更大：环岛入口处道路中心偏移远超普通弯道            */
    /*   ② 三场同向：远/中/近三段偏移方向一致                         */
    /*      （S弯会过零变号，普通弯道近场可能已回正）                  */
    /*   ③ 阶跃突变：中远场交界处出现大的差分跳变（环岛入口瞬间）     */
    /*                                                              */
    /* 三个特征加权打分，总分 >= ROAD_RING_MIN_CONF 判定为环岛        */
    /* ============================================================ */
    {
        uint8 ring_score = 0;
        uint8 ring_side  = 0;  /* 1=左环, 2=右环 */
        int16 i;
        uint8 jump_cnt;

        /* ---- 特征①：中场峰值幅度（权重35） ---- */
        /* 环岛级偏移 >= ROAD_RING_PEAK_THR(24) → 满分 */
        /* 介于弯道和环岛之间(18~23) → 半分            */
        if(road_seg[1].sample_cnt >= 5 && abs(road_seg[1].peak) >= ROAD_PEAK_MIN + 6)
        {
            ring_side = (road_seg[1].peak > 0) ? 1 : 2;

            if(abs(road_seg[1].peak) >= ROAD_RING_PEAK_THR)
            {
                ring_score += 35;
            }
            else
            {
                ring_score += 20;
            }
        }
        /* 若中场数据不足，参考近场峰值 */
        else if(road_seg[2].sample_cnt >= 5 && abs(road_seg[2].peak) >= ROAD_RING_PEAK_THR)
        {
            ring_side  = (road_seg[2].peak > 0) ? 1 : 2;
            ring_score += 25;
        }

        /* ---- 特征②：远中近三场方向一致性（权重30） ---- */
        /* 三段全偏同一侧 → 强烈环岛特征（普通弯道近场往往已回正） */
        /* 若 ring_side 已确定，必须同向才加分（防止方向矛盾）   */
        {
            uint8 all_left  = 0;
            uint8 all_right = 0;

            if(road_seg[0].sample_cnt >= 3 && road_seg[1].sample_cnt >= 5 && road_seg[2].sample_cnt >= 5)
            {
                if(road_seg[0].avg > ROAD_STRAIGHT_THR
                   && road_seg[1].avg > ROAD_STRAIGHT_THR
                   && road_seg[2].avg > ROAD_STRAIGHT_THR)
                {
                    all_left = 1;
                }
                else if(road_seg[0].avg < -ROAD_STRAIGHT_THR
                        && road_seg[1].avg < -ROAD_STRAIGHT_THR
                        && road_seg[2].avg < -ROAD_STRAIGHT_THR)
                {
                    all_right = 1;
                }

                if(all_left || all_right)
                {
                    /* 若 ring_side 未确定，从三场一致性推断方向 */
                    if(ring_side == 0)
                    {
                        ring_side = all_left ? 1 : 2;
                        ring_score += 20;  /* 方向首次确定，降权 */
                    }
                    /* 若已确定，检查是否同向 */
                    else if((ring_side == 1 && all_left) || (ring_side == 2 && all_right))
                    {
                        ring_score += 30;  /* 同向确认，满分 */
                    }
                    /* 方向矛盾：不加分，不降分（可能是噪声） */
                }
            }
        }

        /* ---- 特征③：中远场阶跃跳变（权重25） ---- */
        /* 环岛入口处 Road_Wide 发生突变，一阶差分出现大值 */
        jump_cnt = 0;
        for(i = ROAD_MID_END; i >= ROAD_MID_START; i -= 2)
        {
            if(!road_valid[i]) continue;
            if(abs(road_diff[i]) >= ROAD_RING_JUMP_THR)
                jump_cnt++;
        }
        if(jump_cnt >= 3)
        {
            ring_score += 25;
        }
        else if(jump_cnt >= 1)
        {
            ring_score += 10;
        }

        /* ---- 综合判定 ---- */
        if(ring_score >= ROAD_RING_MIN_CONF && ring_side > 0)
        {
            g_track_feature.type = (ring_side == 1) ? TRACK_LEFT_RING : TRACK_RIGHT_RING;
            g_track_feature.confidence = ring_score;
            if(g_track_feature.confidence > 100)
                g_track_feature.confidence = 100;
            return;
        }
    }

    /* ===== 优先级1：严重丢线 ===== */
    if(road_seg[0].sample_cnt < 3 && road_seg[1].sample_cnt < 5)
    {
        g_track_feature.type = TRACK_LOST;
        g_track_feature.confidence = 80;
        return;
    }

    /* ===== 优先级2：直道 ===== */
    straight_segments = 0;
    for(i = 0; i < 3; i++)
    {
        if(road_seg[i].sample_cnt > 0
           && abs(road_seg[i].avg) < ROAD_STRAIGHT_THR
           && road_seg[i].straight_cnt > road_seg[i].total_cnt * 6 / 10)
        {
            straight_segments++;
        }
    }
    if(straight_segments >= 2)
    {
        g_track_feature.type = TRACK_STRAIGHT;
        g_track_feature.confidence = 60 + straight_segments * 15;
        return;
    }

    /* ===== 优先级3：S弯 ===== */
    if(road_seg[0].sample_cnt > 0 && road_seg[2].sample_cnt > 0)
    {
        if((road_seg[0].avg > ROAD_STRAIGHT_THR && road_seg[2].avg < -ROAD_STRAIGHT_THR)
           || (road_seg[0].avg < -ROAD_STRAIGHT_THR && road_seg[2].avg > ROAD_STRAIGHT_THR))
        {
            zero_cross = Find_Zero_Crossing();
            if(zero_cross != -1)
            {
                g_track_feature.type = TRACK_S_CURVE;
                g_track_feature.zero_cross_row = zero_cross;
                g_track_feature.confidence = 70
                    + (abs(road_seg[0].avg) + abs(road_seg[2].avg)) / 2;
                if(g_track_feature.confidence > 100)
                    g_track_feature.confidence = 100;
                return;
            }
        }
    }

    /* ===== 优先级4：弯道 ===== */
    if(road_seg[1].sample_cnt > 0 || road_seg[2].sample_cnt > 0)
    {
        if(road_seg[1].sample_cnt > 0)
        {
            ref_avg     = road_seg[1].avg;
            ref_peak    = road_seg[1].peak;
            ref_peak_row = road_seg[1].peak_row;
        }
        else
        {
            ref_avg     = road_seg[2].avg;
            ref_peak    = road_seg[2].peak;
            ref_peak_row = road_seg[2].peak_row;
        }

        if(abs(ref_avg) > ROAD_STRAIGHT_THR && abs(ref_peak) > ROAD_PEAK_MIN)
        {
            if(ref_avg > 0)
                g_track_feature.type = TRACK_LEFT_CURVE;
            else
                g_track_feature.type = TRACK_RIGHT_CURVE;

            g_track_feature.peak_row = ref_peak_row;

            g_track_feature.confidence = (abs(ref_avg) - ROAD_STRAIGHT_THR) * 3;
            if(g_track_feature.confidence > 100)
                g_track_feature.confidence = 100;

            g_track_feature.is_entering = Is_Entering_Curve(ROAD_MID_END, ROAD_MID_START);
            g_track_feature.is_exiting  = Is_Exiting_Curve(ROAD_NEAR_END, ROAD_NEAR_START);

            return;
        }
    }

    /* ===== 保底：未知 ===== */
    g_track_feature.type = TRACK_UNKNOWN;
    g_track_feature.confidence = 30;
}

/* ================================================================ */
// 函数名称    出环趋势检测
// 函数说明    Road_Wide 近场偏移从高值快速回落 → 正在出环
//             仅在 Find_Ring 状态机处于环内时调用才有意义
// 返 回 值    1 = 近场偏移正在回落（可能出环）
// ================================================================ */
uint8 Is_Ring_Exiting(void)
{
    int16  i, drop_rate;
    uint8  neg_count, total;

    /* 条件1：近场avg从高值快速回落 */
    if(last_near_avg != 0 && road_seg[2].sample_cnt > 0)
    {
        drop_rate = last_near_avg - road_seg[2].avg;
        if(drop_rate > 15)
        {
            last_near_avg = road_seg[2].avg;
            return 1;
        }
    }
    if(road_seg[2].sample_cnt > 0)
        last_near_avg = road_seg[2].avg;

    /* 条件2：近场负diff增多（回正趋势） */
    neg_count = 0; total = 0;
    for(i = ROAD_NEAR_END; i >= ROAD_NEAR_START; i -= 2)
    {
        if(!road_valid[i]) continue;
        total++;
        if(road_diff[i] < -ROAD_SLOPE_THR)
            neg_count++;
    }
    if(total > 0 && neg_count > total * 6 / 10)
        return 1;

    return 0;
}
