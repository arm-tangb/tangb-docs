
==============================
高斯分布
==============================

+--------+------------+-----------------------+-----------------------------------------------+
| 版本   | 更新日期   | 编辑                  | 说明                                          |
+========+============+=======================+===============================================+
| v1.1.0 | 2022.09.22 | 唐斌                  | 整理                                          |
+--------+------------+-----------------------+-----------------------------------------------+
| v1.0.0 | 2022.09.17 | 唐斌: 7120094@qq.com  | 初始版本                                      |
+--------+------------+-----------------------+-----------------------------------------------+

一、中心极限定理非浮点运算版本
==============================

+ 代码

.. code:: c

    /* *************************************************************************
     * TangBin, 2022.09.22
     * mean: 50, std: 10 
     ************************************************************************ */
    #include <stdlib.h>

    #define RESUM   0x80    /* > MODEL */
    #define MODEL   100
    #define COUNT   8       /* 1000b */
    #define SHIFT   3

    unsigned char gauss_easy(void)
    {
        unsigned char i = COUNT;
        unsigned short x = 0;

        while (i--)
            x += rand() % MODEL;

        return x >> SHIFT;
    }

+ 测试结果

.. code:: log

    tangbin@1655H62:~/gauss$ gcc gauss_easy.c gauss_test.c 
    tangbin@1655H62:~/gauss$ time ./a.out 50 10 100000000
    1 * 标准差 [40 ~ 60]:	 68842369
    2 * 标准差 [30 ~ 70]:	 95599241
    3 * 标准差 [20 ~ 80]:	 99815536
    4 * 标准差 [10 ~ 90]:	 99999309
    5 * 标准差 [0 ~ 100]:	 100000000

    real	0m11.217s
    user	0m11.213s
    sys	0m0.000s


二、中心极限定理数学库版本
==============================

+ 代码

.. code:: c

    #include <stdlib.h>
    #include <math.h>

    #define NSUM    25

    double guass_clt(void)
    {
        double x = 0;
        int i;

        for (i=0; i<NSUM; i++)
            x += (double) rand() / RAND_MAX;

        x -= NSUM / 2.0;
        x /= sqrt(NSUM / 12);

        return x;
    }

+ 测试结果

.. code:: log

    tangbin@1655H62:~/gauss$ gcc gauss_clt.c gauss_test.c 
    tangbin@1655H62:~/gauss$ time ./a.out 50 10 100000000
    1 * 标准差 [40 ~ 60]:	 69395172
    2 * 标准差 [30 ~ 70]:	 95560963
    3 * 标准差 [20 ~ 80]:	 99749313
    4 * 标准差 [10 ~ 90]:	 99995442
    5 * 标准差 [0 ~ 100]:	 99999978

    real	0m31.287s
    user	0m31.277s
    sys	0m0.000s

三、Box & Muller
==============================

+ 代码

.. code:: c

    #include <stdlib.h>
    #include <math.h>

    #define PI      3.141592654

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

+ 测试结果

.. code:: log

    tangbin@1655H62:~/gauss$ gcc gauss_box_muller.c gauss_test.c -lm
    tangbin@1655H62:~/gauss$ time ./a.out 50 10 100000000
    1 * 标准差 [40 ~ 60]:	 70575702
    2 * 标准差 [30 ~ 70]:	 95939142
    3 * 标准差 [20 ~ 80]:	 99768247
    4 * 标准差 [10 ~ 90]:	 99994774
    5 * 标准差 [0 ~ 100]:	 99999967

    real	0m6.913s
    user	0m6.910s
    sys	0m0.000s


四、Marsaglia
==============================

+ 代码

.. code:: c

    #include <stdlib.h>
    #include <math.h>

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

+ 测试结果

.. code:: log

    tangbin@1655H62:~/gauss$ gcc gauss_marsaglia.c gauss_test.c -lm
    tangbin@1655H62:~/gauss$ time ./a.out 50 10 100000000
    1 * 标准差 [40 ~ 60]:	 70565969
    2 * 标准差 [30 ~ 70]:	 95938427
    3 * 标准差 [20 ~ 80]:	 99768140
    4 * 标准差 [10 ~ 90]:	 99994789
    5 * 标准差 [0 ~ 100]:	 99999966

    real	0m7.364s
    user	0m7.361s
    sys	0m0.001s


五、测试代码
==============================

.. code:: c

    #include <stdlib.h>
    #include <stdio.h>
    #include <time.h>

    unsigned char gauss_easy(void);
    double gauss_clt(void);
    double gauss_box_muller(void);
    double gauss_marsaglia(void);

    double gauss_rand(double mu, double theda)
    {
        //return mu + theda * (gauss_easy()/10.0-5);
        //return mu + theda * gauss_clt();
        //return mu + theda * gauss_box_muller();
        return mu + theda * gauss_marsaglia();
    }

    int main(int argc, char **argv)
    {
        unsigned long int mu, theda, gs_num, count, i, j;
        unsigned long int *gs_array;

        if (argc > 2) {
            mu = atoi(argv[1]);
            theda = atoi(argv[2]);
        } else {
            mu = 50;
            theda = 10;
        }

        if ((argc > 3) || (argc == 2))
            gs_num = atoi(argv[argc-1]);
        else
            gs_num = 100000000;

        /* 建立 Hash Table */
        gs_array = (unsigned long int*)calloc(mu*2+1, sizeof(unsigned long int));

        srand(time(0));

        /* Gaussian Distribution 计数 */
        for (i=0; i<gs_num; i++) {
            j = (int)gauss_rand(mu, theda);
            if ((j >= 101) || (j < 0))
                continue;
            gs_array[j]++;
        }

        /* 标准差区间统计 */
        for (i=1; i<=5; i++) {
            int td_s, td_e;
            count = 0;
            td_s = mu-i*theda;
            td_e = mu+i*theda;

            for (j=td_s; j<=td_e; j++)
                count += gs_array[j];

            printf("%ld * 标准差 [%d ~ %d]:\t %ld\n", i, td_s, td_e, count);
        }

        free(gs_array);

        return 0;
    }
