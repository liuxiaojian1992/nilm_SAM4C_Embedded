# nilm_SAM4C_Embedded
Embedded porting algorithm是做家用电器分类的一个项目，输入为电器的特征，输出为电器的类别
data.c是训练的权重文件，保存的权重，
shuju.c是输入一次特征，识别标签为2，是我定义的电器类别
model.c是输入的为1*40*6(长度为40，宽1，通道数为6的输入数据)，经过两层卷积，flatten和dense,求10分类种最大的标签，也就是我电器的类别
