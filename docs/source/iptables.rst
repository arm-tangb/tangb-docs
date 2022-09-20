

==============================
Iptalbes
==============================

+--------+------------+-----------------------+-----------------------------------------------+
| 版本   | 更新日期   | 编辑                  | 说明                                          |
+========+============+=======================+===============================================+
| v0.2.0 | 2022.09.20 | 唐斌                  | 转换为rst                                     |
+--------+------------+-----------------------+-----------------------------------------------+
| v0.1.0 | 2022.02.17 | 唐斌: 7120094@qq.com  | 初始版本                                      |
+--------+------------+-----------------------+-----------------------------------------------+

一、iptables 转发应用
==============================

1.1 转发前的配置和检查
------------------------------

**1）开启转发**

.. code-block:: shell

	echo 1 > /proc/sys/net/ipv4/ip_forward


**2）刷新和重置 iptables 到默认状态**

.. code-block:: shell

	iptables -F
	iptables -X
	iptables -t nat -F
	iptables -t nat -X
	iptables -t mangle -F
	iptables -t mangle -X
	iptables -t raw -F
	iptables -t raw -X
	iptables -t security -F
	iptables -t security -X
	iptables -P INPUT ACCEPT
	iptables -P FORWARD ACCEPT
	iptables -P OUTPUT ACCEPT


**3）查看已添加的iptables规则**

.. code-block:: shell

	iptables -t nat -nvL


1.2 转发配置示例
------------------------------

+ **iptables + SNAT(MASQUERADE)**

.. code-block:: shell

	echo 1 > /proc/sys/net/ipv4/ip_forward
	iptables -t nat -A POSTROUTING -s 192.168.0.1/24 -o eth0 -j MASQUERADE
	iptables -t nat -A POSTROUTING -s 192.168.12.1/24 -o eth2 -j MASQUERADE


+ **将本地的端口转发到本机端口**

.. code-block:: shell

	iptables -t nat -A PREROUTING -p tcp --dport 44444 -j REDIRECT --to-port 55555


.. code-block:: shell

	# -l      listen
	nc -l 55555
	# -v      Have nc give more verbose output.
	nc -v 192.168.199.70 44444


+ **将本机端口 55422 转发到目标机 55102**

.. code-block:: shell

	iptables -t nat -A PREROUTING -p tcp --dport 55422 -j DNAT --to-destination 192.168.199.70:55102
	iptables -t nat -A POSTROUTING -p tcp -d 192.168.199.70 --dport 55102 -j SNAT --to-source 192.168.199.20


.. 
	```mermaid
	graph LR
		转发(192.168.199.20)
		目标(192.168.199.70)
		起始(192.168.199.x)
		
		起始 --- 转发 --- 目标
	```



+ **本机的 eth1:554 转发到 192.168.0.22:554**

.. code-block:: shell

	iptables -t nat -A PREROUTING -i eno1 -p tcp --dport 44444  -j DNAT --to 192.168.199.70:55555


二、 iptables 基础
==============================

2.1 表
------------------------------

.. code-block:: log

	-t, --table table
	filter
	nat
	mangle
	raw

filter表——过滤数据包
Nat表——用于网络[地址转换](https://so.csdn.net/so/search?q=地址转换&spm=1001.2101.3001.7020)（IP、端口）
Mangle表——修改数据包的服务类型、TTL、并且可以配置路由实现QOS
Raw表——决定数据包是否被状态跟踪机制处理

2.2 链
------------------------------

.. code-block:: log

	PREROUTING: 进路由，目的DNAT规则
	INPUT: 进系统
	FORWARD: 转发
	OUTPUT: 出系统
	POSTROUTING: 出路由，源SNAT规则

	MASQUERADE: 动态IP


DNAT: 目的地址转换

在路由前（PREROUTING）将来自外网访问网关公网ip及对应端口的目的ip及端口修改为内部服务器的ip及端口，实现发布内部服务器。

SNAT: 源地址转换

定义在POSTROUTING链上，主要用于实现内网客户端访问外部主机时使用（局域网上网用）

REDIRECT: 重定向、映射、透明代理


.. code-block:: shell
	iptables -A INPUT -m state --state RELATED,ESTABLISHED -j ACCEPT

	iptables -A FORWARD -m state --state RELATED,ESTABLISHED -j ACCEPT

	iptables-save > /etc/sysconfig/iptables-rules


三、参考资料
==============================

https://man.linuxde.net/iptables
