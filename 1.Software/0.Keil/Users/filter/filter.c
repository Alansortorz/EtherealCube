/**
 * @create 常见滤波算法
 * @time   2020.12.15
 **/
#include "filter.h"
#include "bsp_systick.h"
/**
 * @brief 限幅滤波法
 * @param value:有效值 new_value:当前采样值
 * @retval 有效的实际值
 * @note
 *	优点：可以抑制因偶然因素造成的脉冲干扰
 *	缺点：无法抑制周期性的脉冲干扰
 **/
#define	Nums	10
short Clipping_Filter(short value,short new_value){
	if((new_value - value > Nums) || (value -  new_value) > Nums)
		return value;
	else
		return new_value;
}

/**
 * @brief 中值滤波算法
 * @param 函数sort() - 快速排序算法	buf:传递进来的数据	low:低位数据下标		high:高位数据下标
 * 		  函数Median_Filter() 		buf:传递进来的数据
 * 		  N: 一次传入的数据量
 * @retval 中值
 * @note  优点：可以抑制偶然因素引起的波动干扰，对温度、液位等变化缓慢的数据量有良好的滤波效果
 *		  缺点：不易对流量、速度等快速变化的数据使用
 **/
void sort(short* buf, unsigned short low, unsigned short high){
	int pivot = buf[low];
	while(low < high){
		while(low < high && buf[high] >= pivot)
			high--;
		while(low < high && buf[low] <= pivot)
			low++;
	}
	buf[low] = pivot;
}

#define N	10
short Median_Filter(short *buf){
	short value_buf[N];
	int i;
	for(i=0; i<N; ++i){
		value_buf[i] = buf[i];
		buf++;
		Delay_ms(5);
	}
	sort(value_buf, 0, N-1);
	Delay_ms(2);
	return value_buf[(N-1)/2];
}

/**
 * @brief 算数平均滤波
 * @param buf 传入的数据
 * @retval 算术平均值
 * @note 优点：能抑制一般的随机干扰，数据在一个范围内波动
 *		 缺点：不适用与测量速率较慢或者计算速度要求快的实时控制场景
 **/
#define Num		10
short Arithmetic_Filter(short* buf){
	int sum=0, i;
	for(i=0; i<Num; ++i){
		sum += buf[i];
		Delay_ms(1);
	}
	return sum/N;	
}

/**
 * @brief 卡尔曼滤波算法
 * @param
 * @retval
 * @note
 *	X(k) = A * X(k-1) + B * U(k) + W(k)  状态方程
 *		X(k)为当前时刻的状态，X(k-1)为上一时刻的状态，U(k)为当前时刻外部控制量，W(k)为过程噪声Q
 *		过程噪声表示的是一个误差，原因是物体现时刻真实的状态与计算理论状态的差值
 *  	计算理论状态就是上时刻的状态加上控制量的状态改变，这个值是否等于真实的状态呢？不一定，因此就出现这个W(k)来修正这个状态
 *	Z(k) = H * X(k) + V(k)	
 *  P(k) = A * P(k-1)*A^T + Q	预测方程
 *	K(k) = P(k-1) * H^T * (H * P(k-1) * H^T + R)^(-1)  卡尔曼增益
 *	X(k) = X(k-1) * K(k) * (Z(k) - H(X(k-1)))	更新方程
 *	P(k) = (1 - K(k) * H) * P(k-1)
 **/
float Kalman1_Filter(const float ResrcData, 
					 float ProcessNiose_Q, 
					 float MeasureNoise_R, 
					 float InitialPrediction){
	float R = MeasureNoise_R;
	float Q = ProcessNiose_Q;
	static float x_last, p_last;
	float x_mid = x_last, x_now, p_mid, p_now, kg;    

	x_mid=x_last; 		//x_last=x(k-1|k-1),x_mid=x(k|k-1)
	p_mid=p_last+Q; 	//p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声
	kg=p_mid/(p_mid+R); //kg为kalman filter，R为噪声
	x_now=x_mid+kg*(ResrcData-x_mid);//估计出的最优值
	p_now=(1-kg)*p_mid;	//最优值对应的covariance        
	p_last = p_now; 	//更新covariance值
	x_last = x_now; 	//更新系统状态值
	return x_now;                
}


