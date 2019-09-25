# SoundCollector
录音电路。这是一个课程项目，需要±12V和+5V同时供电，因此没有实用价值。该电路用OP07CP做两级反向放大器和一级加法器。
用IAP15F2K61S2单片机采样，并用串口发送至电脑。
保存数据为bin文件后，用MATLAB播放。有两阶RC无源低通滤波（虽然这一步并没有实际意义）。
受限于串口的传输速率（最高波特率115200），无法保留高频的成分。



我已经用实际硬件调试成功。用MATLAB运行play.m即可播放sound.bin录音结果。
需要STC公司的软件stc-isp-15xx-v6.86q.exe来烧录单片机程序。其中的串口助手还可收取串口传来的录音数据。



stcmcu_altium_lib.IntLib由STC制作，含STC各种单片机的原理图，封装，footprint，用于Altium Designer设计PCB。



Mic.PrjPcb是实际电路图的PCB设计项目。最终采用的PCB设计是Mic2.PcbDoc。这个板子的纵向高度正好能装进淘宝上的8格塑料盒子，横向宽度也可以再缩一缩。可变电阻用VR5封装，可调节放大倍数和加法器。**原理图Mic.SchDoc把OP07CP的+12V和-12V供电画反了，但PCB上标注的+12V和-12V是正确的。**MAX232CPE和串口端可以用USB转串口的单片机编程器代替。淘宝上不到十块钱，可靠性绝佳。退耦电容C99，C100，C101完全可以不焊，反正是专门的直流电源和电脑主板供电。**电容C0的电压是下面比上面高（或者说靠近单片机的一侧电压比较高）。千万不要焊反了。**
MicOP07CP.ms14是模拟电路的仿真。可用Multisim 14打开。
