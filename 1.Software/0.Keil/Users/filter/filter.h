#ifndef __FILTER_H
#define __FILTER_H

/**
 * @brief 1 Demension Kalman Filter
 * @param
 *		x:״̬,��ѹ���
 *		A:Ԥ�����
 *		H:��������
 *		q:�ⲿ���ŵ�Э�������Ҳ���ǹ�������
 *		r:��������������Э�������Ҳ���ǲ�������
 *		p:Э�������
 *		gain:����������
 **/
typedef struct Kalman1_state{
	double filterValue;  //k-1ʱ�̵��˲�ֵ������k-1ʱ�̵�ֵ
	double kalmanGain;   //   Kalamn����
	double A;   // x(n)=A*x(n-1)+u(n),u(n)~N(0,Q)
	double H;   // z(n)=H*x(n)+w(n),w(n)~N(0,R)
	double Q;   //Ԥ���������ƫ��ķ���
	double R;   //��������ƫ�(ϵͳ����Ժ�ͨ������ͳ��ʵ����)
	double P;   //�������Э����
}kalman1_state;

float Kalman1_Filter(const float ResrcData, float ProcessNiose_Q, float MeasureNoise_R, float InitialPrediction);


#endif /* __FILTER_H */

