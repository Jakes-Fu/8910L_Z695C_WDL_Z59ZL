/************************************************使用说明************************************************/
1.脚本组成
 1).assertMacroCheck.pl             :本脚本
 2).assertMacroCheck.properties     :脚本配置文件
 3).\Sprd_atest_pm\sprd_atest_extract_used_code.pm :主控脚本调用的公共模块
 4).\Sprd_atest_pm\sprd_atest_file_process.pm      :主控脚本调用的公共模块
 5).\Sprd_atest_pm\Sprd_atest_get_keyword.pm       :主控脚本调用的公共模块

2.使用方式
1).须将上述四个文件拷贝至本机同一目录下。
2).本脚本使用命令行参数方式运行，具体如下：
盘符:>perl assertMacroCheck.pl 检查路径（或文件）报告存放路径
或
盘符:>cqperl assertMacroCheck.pl 检查路径（或文件）报告存放路径

注: 需perl v5.6以上版本 或者 cqperl v5.6以上版本。建议使用高版本的Perl解析器,如Perl 5.8等，运行时间会缩短。
整个工程扫描时间在5.8版本以上为10分钟，以下为30分钟左右。
注1: 若路径或文件名中包含空格，需要加上双引号。如"D:\test_enviroment\mingxuan.zhu_view\MS_Code"。
注2: 请确保MS_Code下包含make文件夹及其子目录
注3: assertMacroCheck.properties文件为配置文件，文件分为第三方代码路径区和过滤路径区
     在# The 3PF path start和# The 3PF path end之间添加检查的第三方代码路径
     在# The FF path start和# The FF path end之间添加需要过滤的代码路径
注4: 请确保报告存放路径的存在。


3.运行步骤
 1).命令行方式运行程序，具体如下：
 盘符:>perl assertMacroCheck.pl D:\test_enviroment\mingxuan.zhu_mocor_view\MS_Code D:
 出现如下画面:
  ============Please select the switch of assert statements check:===========
  == 1:new       #renew check assert statements.                           ==
  == 2:update    #check assert statements after annotate assert statements.==
  ===========================================================================
  Please enter switch[1|2]1
  ====================  Please select the range of search  :=================
  == 1:ALL       # all module                                              ==
  == 2:MODULE    # single module                                           ==
  ===========================================================================
  Please enter switch[1|2]1
  Assert Macro Check has started at Thu Apr 14 09:38:09 2011
  Please waiting......

注1：根据文件和文件夹的大小本脚本运行时间会不一致。

注2：支持扫描各个模块的Assert语句使用情况，即检查路径设为具体模块路径即可。
     如DAPS模块，命令格式如下
     perl assertMacroCheck.pl D:\test_enviroment\mingxuan.zhu_mocor_view\MS_Code\DAPS D:(回车)
     ============Please select the switch of assert statements check:===========
     == 1:new       #renew check assert statements.                           ==
     == 2:update    #check assert statements after annotate assert statements.==
     ===========================================================================
     Please enter switch[1|2]1  ---- 第一次运行脚本时选择1，另外注释掉转义宏后也需要选择1，其他情况可选择2
     ====================  Please select the range of search  :=================
     == 1:ALL       # all module                                              ==
     == 2:MODULE    # single module                                           ==
     ===========================================================================
     Please enter switch[1|2]2

/************************************如何过滤确认过的合法的Assert语句************************************/
/*======================================================================================================*/
注3：可以添加注释关键字语句，可以过滤掉一些确认正确的Assert语句。具体格式如下
     SCI_ASSERT(....); /*assert verified*/
     SCI_ASSERT(....); /*assert to do*/
     需要注意注释必须和SCI_ASSERT及其转义宏在一行上，即写成多行的assert语句需要把注释关键字写在第一行上。
      /*assert verified*/ 说明以后不需要再关注了，已经确认
      /*assert to do*/ 说明目前没有好的修改方案，直接修改风险很大，后续还需要进行修改
/*======================================================================================================*/

/************************************如何过滤确认过的合法的Assert转义宏**********************************/
/*======================================================================================================*/
注4：可以添加注释关键字语句，可以过滤掉一些确认正确的Assert转义宏。具体格式如下(一个例子)
     #define MACRO_XXX SCI_ASSERT /*assert verified*/
     需要注意注释必须和#define MACRO_XXX宏在一行上，即写成多行的Assert转义宏需要把注释关键字写在第一行上。
     这种在定义时进行注释的方式，会把所有用到MACRO_XXX 转义宏的地方过滤掉，不计算在统计结果内，谨慎使用。
     对于多行定义的转义宏，请将该注释写在有SCI_ASSERT/SCI_PASSERT/assert的一行上。
/*======================================================================================================*/

4.运行结果查看
结果将把包含非正确使用的SCI_ASSERT/SCI_PASSERT/assert宏及其转义宏信息写入当前脚本所在目录下的Report目录中，具体如下：
当前目录:\Report\*.xml                (* 代表具体的模块名称)
当前目录:\assertInfoFile.xls          (按照模块统计的SCI_ASSERT/SCI_PASSERT语句数，包含转义宏)
当前目录:\verifiedAssertInfoFile.xls  (按照文件统计过滤的assert/SCI_ASSERT/SCI_PASSERT宏，包含转义宏)
当前目录:\assertCheck.log             (MS_Code下所有的SCI_ASSERT/SCI_PASSERT宏及其转义宏列表)


//////////////////////////////////////////////////////////////////////////////////////////////////////////

一个完整的例子：
D:\TestCode\assertMacroCheck>perl assertMacroCheck.pl E:\test_enviroment\Mingxuan.Zhu_10A_view\MS_Code D:\TestCode\assertMacroCheck
============Please select the switch of assert statements check:===========
== 1:new       #renew check assert statements.                           ==
== 2:update    #check assert statements after annotate assert statements.==
===========================================================================
Please enter switch[1|2]1
====================  Please select the range of search  :=================
== 1:ALL       # all module                                              ==
== 2:MODULE    # single module                                           ==
===========================================================================
Please enter switch[1|2]1
Assert Macro Check has started at Thu Apr 14 09:38:09 2011
Please waiting......
Assert Macro Check has ended at Thu Apr 14 09:56:27 2011
The report file (D:\TestCode\assertMacroCheck) has been generated in current catalog.