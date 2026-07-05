#ifndef __IMAGE_PROCESSING_H_
#define __IMAGE_PROCESSING_H_

#include "zf_common_headfile.h"

/* 曝光设置 */
extern uint16 exp_time;

/* 图像宏定义 */
#define Image_H             MT9V03X_H / 2  //60
#define Image_W             MT9V03X_W / 2  //94

#define IMG_WHITE 255
#define IMG_BLACK 0

/* 图像变量 */
extern uint8 image_two_value[Image_H][Image_W];  //二值化
extern uint8 Image_Use[Image_H][Image_W];  //压缩图像

extern int Adapt_Threshold; //自适应阈值

void Get_Use_Image(void);
int My_Adapt_Threshold(uint8 *image, uint16 col, uint16 row);
void Image_Binarization(int threshold, uint16 col, uint16 row);
void Image_Blur (void);

#endif /* CODE_CAMERA_IMAGE_IMAGE_PROCESSING_H_ */
