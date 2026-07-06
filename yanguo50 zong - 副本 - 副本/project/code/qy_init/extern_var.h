#ifndef __EXTERN_VAR_H_
#define __EXTERN_VAR_H_

#include "qy_headfile.h"

// 双缓冲DMA方案：typedef用于C251兼容的指针到数组类型转换
typedef uint8 (far *image_copy_out_ptr_t)[MT9V03X_W];

/* 模式锟斤拷辖峁癸拷锟?*/
typedef struct
{
    int16 Mode_Speed_Max[5];
    int16 Mode_Speed_Min[5];
    float Mode_S_p[5];
    float Mode_S_i[5];
    float Mode_T_p[5];
    float Mode_T_GKD[5];
} MODE;

extern MODE mode;

/* 锟劫讹拷模式锟叫伙拷 */
extern int out_Mode;

/* 锟斤拷压锟斤拷锟? */
extern uint16 ADC_Out;  //ADC锟斤拷锟?
extern float battery_voltage; //锟斤拷氐锟窖?

/* 锟叫断憋拷志位 */
extern uint8 pit_state_CCU6_0_CH0;

/* 锟斤拷锟竭憋拷锟斤拷锟斤拷志位 */
extern int Protection;

/* 图锟斤拷锟斤拷锟斤拷志 */
extern int image_flag;

/* 转锟斤拷PID锟斤拷锟街?*/
extern int16 Turn_Out;
extern int16 Turn_Out_MAX;
extern int16 Speed_Out;
extern int16 Speed_Left_Out;                    //锟劫度伙拷锟斤拷锟?
extern int16 Speed_Right_Out;
extern int16 Speed_Out;
extern int16 nowtargetSpeed;
/* 锟斤拷时锟斤拷锟斤拷 */
extern uint16 use_time;

extern double img_data;

extern uint8 COM_QY;

extern int my_Speed;

// 原 image_copy 已删除，22KB用于抵消新增的 image_dma_buf2
// ==================== 双缓冲DMA方案（新增） ====================
extern uint8 far image_copy[MT9V03X_H][MT9V03X_W];
extern image_copy_out_ptr_t image_copy_out;                  // 指针，指向DMA完成的安全缓冲区
extern uint8 far image_dma_buf3[MT9V03X_H][MT9V03X_W];
extern uint8 xdata image_dma_buf2[MT9V03X_H][MT9V03X_W];   // 第二DMA缓冲区
extern volatile uint8 dma_target_sel;                        // 下一帧DMA目标选择
extern volatile uint8 dma_done_sel;                          // 刚完成帧的选择
	extern volatile uint8 buf_locked;                        // 缓冲区锁定：0=无锁定, 1=buf0被锁强制写buf1, 2=buf1被锁强制写buf0
// ==================== 双缓冲DMA方案结束 ====================

void Interrupt(void);
void QQYY(void);

#endif /* CODE_EXTERN_VAR_EXTERN_VAR_H_ */
