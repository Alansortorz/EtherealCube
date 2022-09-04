#ifndef __FILTER_H
#define __FILTER_H

/**
 * @brief 1 Demension Kalman Filter
 * @param
 *		x:状态,最佳估计
 *		A:预测矩阵
 *		H:测量矩阵
 *		q:外部干扰的协方差矩阵，也就是过程噪声
 *		r:传感器测量误差的协方差矩阵，也就是测量矩阵
 *		p:协方差矩阵
 *		gain:卡尔曼增益
 **/
typedef struct Kalman1_state{
	double filterValue;  //k-1时刻的滤波值，即是k-1时刻的值
	double kalmanGain;   //   Kalamn增益
	double A;   // x(n)=A*x(n-1)+u(n),u(n)~N(0,Q)
	double H;   // z(n)=H*x(n)+w(n),w(n)~N(0,R)
	double Q;   //预测过程噪声偏差的方差
	double R;   //测量噪声偏差，(系统搭建好以后，通过测量统计实验获得)
	double P;   //估计误差协方差
}kalman1_state;

float Kalman1_Filter(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction);


#endif /* __FILTER_H */

