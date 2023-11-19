概述：本仓库为鸿蒙的hi3861应用开发源码仓库

本仓库需遵循以下的开发要求:

1.所有的文件，无论是文本文件还是代码文件，都要统一使用!!!UTF-8!!!编码格式

2.代码编写需要遵循合格的代码规范，比如定义变量名要通俗易懂，拒绝中文式定义变量名

3.无论是谁编写的代码，为了能让别人看懂，都需要为其加上注释！！！，以便更好的进行团队协作

4.为了防止git push起了冲突，不要将还未实现的功能进行push,在修改同一个文件的时候，要提前进行商榷

5.为了能最大程度提高串口烧录的速度以及防止在使用串口通信时出现汉字乱码的情况，统一将串口的波特率设置为!!!468000!!!

-------------------------------------------------------------------------------------------------------
issue by du:
存在问题:
用vscode编写代码时，跳转函数定义和实现时间过长

reference:
https://zhuanlan.zhihu.com/p/648921461
https://www.hmxthome.com/develop/2166.html

statement:
(1) 生成编译数据库compdb(compile database)
(2) 对于小项目，c/c++(微软官方的插件)毫无障碍
(3) 对于大工程，生成compile_commands.json文件，使用clangd(一种语言服务器,作用是代码补全，高亮关键字，函数跳转)扫描该文件并为当前项目中的源码生成索引。
(4) 由于clangd与c/c++会产生冲突，因为在编写OH代码的时候无法使用c/c++所提供的代码格式化程序，因为这里采用clang-format进行平替，其中将格式化style设置为"WebKit"

steps to generate compile_commands.json:
(1) 进入build.ninja所在目录: cd out/hispark_pegasus/wifiiot_hispark_pegasus/
(2) compile_commands.json生成指令: ninja -C ./ -t compdb cxx cc > compile_commands.json

actual effect:
函数定义与声明跳转时几乎不会有延迟，速度远远高于使用c/c++插件
-------------------------------------------------------------------------------------------------------
