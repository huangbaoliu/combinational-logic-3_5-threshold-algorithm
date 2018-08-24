# 算法理论
#1、对16字节密钥进行拆分成10份，前6份为2字节，后四份为1字节；
# 
#2、每份的长度定为3字节，第一字节为序号；
#3、对0~9的序号按照（3,5）门限的原理，分配给5个用户，每个用户6份数据组成一个份密钥，占18字节；
  分配示例：
        0	1	2	3	4	5	6	7	8	9
  用户1	√	√	√	√	√	√				
  用户2	√	√	√				√	√	√	
  用户3	√			√	√		√	√		√
  用户4		√		√		√	√		√	√
  用户5			√		√	√		√	√	√
#4、密钥组合时，随机选择3份，根据3字节数据中的序号，重组密钥数据

# to compile code on a linux mechine
#gcc *.c -o main
# to execute 
#./main
