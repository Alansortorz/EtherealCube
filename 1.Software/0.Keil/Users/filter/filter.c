/**
 * @create �����˲��㷨
 * @time   2020.12.15
 **/
#include "filter.h"
#include "bsp_systick.h"
/**
 * @brief �޷��˲���
 * @param value:��Чֵ new_value:��ǰ����ֵ
 * @retval ��Ч��ʵ��ֵ
 * @note
 *	�ŵ㣺����������żȻ������ɵ��������
 *	ȱ�㣺�޷����������Ե��������
 **/
#define	Nums	10
short Clipping_Filter(short value,short new_value){
	if((new_value - value > Nums) || (value -  new_value) > Nums)
		return value;
	else
		return new_value;
}

/**
 * @brief ��ֵ�˲��㷨
 * @param ����sort() - ���������㷨	buf:���ݽ���������	low:��λ�����±�		high:��λ�����±�
 * 		  ����Median_Filter() 		buf:���ݽ���������
 * 		  N: һ�δ����������
 * @retval ��ֵ
 * @note  �ŵ㣺��������żȻ��������Ĳ������ţ����¶ȡ�Һλ�ȱ仯�����������������õ��˲�Ч��
 *		  ȱ�㣺���׶��������ٶȵȿ��ٱ仯������ʹ��
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
 * @brief ����ƽ���˲�
 * @param buf ���������
 * @retval ����ƽ��ֵ
 * @note �ŵ㣺������һ���������ţ�������һ����Χ�ڲ���
 *		 ȱ�㣺��������������ʽ������߼����ٶ�Ҫ����ʵʱ���Ƴ���
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
 * @brief �������˲��㷨
 * @param
 * @retval
 * @note
 *	X(k) = A * X(k-1) + B * U(k) + W(k)  ״̬����
 *		X(k)Ϊ��ǰʱ�̵�״̬��X(k-1)Ϊ��һʱ�̵�״̬��U(k)Ϊ��ǰʱ���ⲿ��������W(k)Ϊ��������Q
 *		����������ʾ����һ����ԭ����������ʱ����ʵ��״̬���������״̬�Ĳ�ֵ
 *  	��������״̬������ʱ�̵�״̬���Ͽ�������״̬�ı䣬���ֵ�Ƿ������ʵ��״̬�أ���һ������˾ͳ������W(k)���������״̬
 *	Z(k) = H * X(k) + V(k)	
 *  P(k) = A * P(k-1)*A^T + Q	Ԥ�ⷽ��
 *	K(k) = P(k-1) * H^T * (H * P(k-1) * H^T + R)^(-1)  ����������
 *	X(k) = X(k-1) * K(k) * (Z(k) - H(X(k-1)))	���·���
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
	p_mid=p_last+Q; 	//p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����
	kg=p_mid/(p_mid+R); //kgΪkalman filter��RΪ����
	x_now=x_mid+kg*(ResrcData-x_mid);//���Ƴ�������ֵ
	p_now=(1-kg)*p_mid;	//����ֵ��Ӧ��covariance        
	p_last = p_now; 	//����covarianceֵ
	x_last = x_now; 	//����ϵͳ״ֵ̬
	return x_now;                
}


