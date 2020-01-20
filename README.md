# nilm_SAM4C_Embedded
(1)Embedded porting algorithm是做家用电器分类的一个项目，输入为电器的特征，输出为电器的类别

(2)data.c是训练的权重文件，保存的权重，

(3)shuju.c是输入一次特征，识别标签为2，是我定义的电器类别

(4)model.c是输入的为【1，40，6】(长度为40，宽1，通道数为6的输入数据)，经过两层卷积，flatten和dense,求10分类种最大的标签，也就是我电器的类别

