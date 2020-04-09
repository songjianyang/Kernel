
# 简易内核实现


实现的功能：GDT、IDT、时钟中断、分页机制、堆管理、用户态

大部分代码是跟着James的[教程](http://www.jamesmolloy.co.uk/)写的，然后再加上东拼西凑，小部分自己发挥。


关键代码我写了文章来解释，但只讲了关键的部分，想要完整学习可以看James的教程。

[内核笔记 - Hello World](https://zero-day.cn/2020/02/25/kernel-1/)

[内核笔记 - GDT](https://zero-day.cn/2020/02/27/kernel-gdt/)

[内核笔记 - IDT](https://zero-day.cn/2020/02/28/kernel-idt/)

[内核笔记 - IRQs and PIT](https://zero-day.cn/2020/03/04/kernel-irqs-pit/)

[内核笔记 - 分页](https://zero-day.cn/2020/03/09/kernel-paging/)

[内核笔记 - 堆管理](https://zero-day.cn/2020/03/12/kernel-heap/)

[内核笔记 - 用户态](https://zero-day.cn/2020/03/12/kernel-user-mod/)


## 运行方法

重新编译：

```
make
```

调试方法:（我用的peda,自行修改Makefile）

```
make debug
```

运行系统：

```
make qemu
```

![](/image/kernel.png)


### 参考资料
[JamesM's kernel development tutorials](http://www.jamesmolloy.co.uk/)

[x86架构操作系统内核的实现](http://wiki.0xffffff.org/)
