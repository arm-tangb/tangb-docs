
/*
 * TangBin, 7120094@qq.com, 2022.09.14
 * Reference: https://www.cnblogs.com/tsingke/p/6194737.html
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define PI      3.141592654
#define NSUM    25

double guass_CLT(void)
{
	double x = 0;
	int i;

	for (i=0; i<NSUM; i++) {
		x += (double) rand() / RAND_MAX;
	}

	x -= NSUM / 2.0;
	x /= sqrt(NSUM / 12);

	return x;
}

double gauss_box_muller(void)
{
	static double U, V;
	static int phase = 0;
	double a, r, Z;

	if (phase == 0) {
		/* 产生两个均匀分布的 0~1 的随机序列 */ 
		U = rand() / (RAND_MAX + 1.0);
		V = rand() / (RAND_MAX + 1.0);

		/* 极坐标的两个随机变量分布序列 */
		/* a 是极坐标的角度：变成了 0~2 * pi 的均匀分布 */
		a = 2.0 * PI * V;
		/* r 是极坐标的距离：变成自然对数开根号的一种分布 */
		r = sqrt(-2.0 * log(U));

		/*用极坐标 (a,r) 转换成笛卡尔坐标 (x,y)，这就是产生的高斯白噪声 */
		Z = r * sin(a);
	} else {
		Z = r * cos(a);
	}

	phase = 1 - phase;

	return Z;
}

double gauss_marsaglia(void)
{
	static double V1, V2, S;
	static int phase = 0;
	double X;

	if (phase == 0) {
		do {
			double U1 = (double)rand() / RAND_MAX;
			double U2 = (double)rand() / RAND_MAX;

			V1 = 2 * U1 - 1;
			V2 = 2 * U2 - 1;

			S = V1 * V1 + V2 * V2;
		} while (S >=1 || S== 0);

		X = V1 * sqrt(-2 * log(S) / S);
	} else {
		X = V2 * sqrt(-2 * log(S) / S);
	}

	phase = 1 - phase;

	return X;
}

double gauss_rand(double mu, double theda)
{
	return mu + theda * guass_CLT();
	// return mu + theda * gauss_box_muller();
	// return mu + theda * gauss_marsaglia();
}

int main(int argc, char **argv)
{
	int mu, theda, gs_num, count, i, j;
    int *gs_array;

	if (argc > 2) {
		mu = atoi(argv[1]);
		theda = atoi(argv[2]);
    } else {
		mu = 40;
		theda = 10;
    }

	if ((argc > 3) || (argc == 2))
		gs_num = atoi(argv[argc-1]);
	else
		gs_num = 10000;

    /* 建立 Hash Table */
    gs_array = (int *)calloc(mu * 2, sizeof(int));

	srand(time(0));

    /* Gaussian Distribution 计数 */
	for (i=0; i<gs_num; i++) {
        j = (int)gauss_rand(mu, theda);
		gs_array[j]++;
    }

    /* 标准差区间统计 */
    for (i=1; i<=4; i++) {
        int td_s, td_e;
        count = 0;
        td_s = mu-i*theda;
        td_e = mu+i*theda;

        for (j=td_s; j<td_e; j++)
		    count += gs_array[j];

	    printf("%d * 标准差 (%d ~ %d):\t %d / %d\n", i, td_s, td_e, count, gs_num);
    }

    free(gs_array);

	return 0;
}
