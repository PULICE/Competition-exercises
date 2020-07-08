#-*- coding: utf-8 -*-    #让python解释器以utf-8编码读取源代码
ag=input('''plase enter ... your age\r\n''')#变量 类型可以是静态语言，也可以是静态语言，
#区别仅在于变量在定义时是否指定类型
age=int(ag)#将指定的数据转换为整型并返回

if age <= 2000 and age >=1900:
    print('woll are you ',age)
else:
    print("input erro\r\n")

doc=ord('a')#将字符以十进制输出
print("'a'的十进制是",doc)
string=chr(97)#输出对应十进制所表示的字符
print("'97'表示的字符是",string)
"中文".encode('utf-8')#指定编码类型
print("计算str包含字符长度",len('中文'))
print("计算bytes包含字符长度",len(b'abc'))#如果换成bytes,len()计算的就是字节的数。
print("计算bytes包含字符长度",len('中文'.encode('utf-8')))#如果换成bytes,len()计算的就是字节的数。