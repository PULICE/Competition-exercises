#-*- coding: utf-8 -*- 
import math #导入数学函数包
#for循环
names = ['michael','bob','Tracy']#创建了一个list
for name in names:
    print(name)

sum = 0
for x in range(101): #生成一个从0开始小于101的整数序列
    sum = sum + x
print(sum)#它的缩进确定了它并不参与当前for循环

#while循环
sum = 0
n= 9
while n>0:
    sum =sum +n
    n=n-2
    if sum % 2 == 0: # 如果sum是偶数，执行continue语句
        continue # continue语句会直接继续下一轮循环，后续的print()语句不会执行
    print(sum)#缩进与否决定了这一句是否加入当前循环，或者判断

#dictionary
d={'michael':96,'bob':80,'Tracy':90}
print("michael:",d['michael'])

#定义函数
#在Python中，定义一个函数要使用def语句，依次写出函数名、括号、括号中的参数和冒号:，然后，
#在缩进块中编写函数体，函数的返回值用return语句返回
def my_first_function(x):
    if x >=0:
        pass #此语句什么都不做，类似占位符，可以避免语法错误
        return x
    else:
        return -x
    
print(my_first_function(-99))
#设置默认参数n
'''此函数是用来计算传入参数的平方或多次方
参数 x:底数
参数 n:次幂
'''
def power(x,n=2):
    s=1
    while n>0:
        n=n-1
        s=s*x
    return s