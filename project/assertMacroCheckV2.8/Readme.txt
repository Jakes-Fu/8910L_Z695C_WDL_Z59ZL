/************************************************ʹ��˵��************************************************/
1.�ű����
 1).assertMacroCheck.pl             :���ű�
 2).assertMacroCheck.properties     :�ű������ļ�
 3).\Sprd_atest_pm\sprd_atest_extract_used_code.pm :���ؽű����õĹ���ģ��
 4).\Sprd_atest_pm\sprd_atest_file_process.pm      :���ؽű����õĹ���ģ��
 5).\Sprd_atest_pm\Sprd_atest_get_keyword.pm       :���ؽű����õĹ���ģ��

2.ʹ�÷�ʽ
1).�뽫�����ĸ��ļ�����������ͬһĿ¼�¡�
2).���ű�ʹ�������в�����ʽ���У��������£�
�̷�:>perl assertMacroCheck.pl ���·�������ļ���������·��
��
�̷�:>cqperl assertMacroCheck.pl ���·�������ļ���������·��

ע: ��perl v5.6���ϰ汾 ���� cqperl v5.6���ϰ汾������ʹ�ø߰汾��Perl������,��Perl 5.8�ȣ�����ʱ������̡�
��������ɨ��ʱ����5.8�汾����Ϊ10���ӣ�����Ϊ30�������ҡ�
ע1: ��·�����ļ����а����ո���Ҫ����˫���š���"D:\test_enviroment\mingxuan.zhu_view\MS_Code"��
ע2: ��ȷ��MS_Code�°���make�ļ��м�����Ŀ¼
ע3: assertMacroCheck.properties�ļ�Ϊ�����ļ����ļ���Ϊ����������·�����͹���·����
     ��# The 3PF path start��# The 3PF path end֮����Ӽ��ĵ���������·��
     ��# The FF path start��# The FF path end֮�������Ҫ���˵Ĵ���·��
ע4: ��ȷ��������·���Ĵ��ڡ�


3.���в���
 1).�����з�ʽ���г��򣬾������£�
 �̷�:>perl assertMacroCheck.pl D:\test_enviroment\mingxuan.zhu_mocor_view\MS_Code D:
 �������»���:
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

ע1�������ļ����ļ��еĴ�С���ű�����ʱ��᲻һ�¡�

ע2��֧��ɨ�����ģ���Assert���ʹ������������·����Ϊ����ģ��·�����ɡ�
     ��DAPSģ�飬�����ʽ����
     perl assertMacroCheck.pl D:\test_enviroment\mingxuan.zhu_mocor_view\MS_Code\DAPS D:(�س�)
     ============Please select the switch of assert statements check:===========
     == 1:new       #renew check assert statements.                           ==
     == 2:update    #check assert statements after annotate assert statements.==
     ===========================================================================
     Please enter switch[1|2]1  ---- ��һ�����нű�ʱѡ��1������ע�͵�ת����Ҳ��Ҫѡ��1�����������ѡ��2
     ====================  Please select the range of search  :=================
     == 1:ALL       # all module                                              ==
     == 2:MODULE    # single module                                           ==
     ===========================================================================
     Please enter switch[1|2]2

/************************************��ι���ȷ�Ϲ��ĺϷ���Assert���************************************/
/*======================================================================================================*/
ע3���������ע�͹ؼ�����䣬���Թ��˵�һЩȷ����ȷ��Assert��䡣�����ʽ����
     SCI_ASSERT(....); /*assert verified*/
     SCI_ASSERT(....); /*assert to do*/
     ��Ҫע��ע�ͱ����SCI_ASSERT����ת�����һ���ϣ���д�ɶ��е�assert�����Ҫ��ע�͹ؼ���д�ڵ�һ���ϡ�
      /*assert verified*/ ˵���Ժ���Ҫ�ٹ�ע�ˣ��Ѿ�ȷ��
      /*assert to do*/ ˵��Ŀǰû�кõ��޸ķ�����ֱ���޸ķ��պܴ󣬺�������Ҫ�����޸�
/*======================================================================================================*/

/************************************��ι���ȷ�Ϲ��ĺϷ���Assertת���**********************************/
/*======================================================================================================*/
ע4���������ע�͹ؼ�����䣬���Թ��˵�һЩȷ����ȷ��Assertת��ꡣ�����ʽ����(һ������)
     #define MACRO_XXX SCI_ASSERT /*assert verified*/
     ��Ҫע��ע�ͱ����#define MACRO_XXX����һ���ϣ���д�ɶ��е�Assertת�����Ҫ��ע�͹ؼ���д�ڵ�һ���ϡ�
     �����ڶ���ʱ����ע�͵ķ�ʽ����������õ�MACRO_XXX ת���ĵط����˵�����������ͳ�ƽ���ڣ�����ʹ�á�
     ���ڶ��ж����ת��꣬�뽫��ע��д����SCI_ASSERT/SCI_PASSERT/assert��һ���ϡ�
/*======================================================================================================*/

4.���н���鿴
������Ѱ�������ȷʹ�õ�SCI_ASSERT/SCI_PASSERT/assert�꼰��ת�����Ϣд�뵱ǰ�ű�����Ŀ¼�µ�ReportĿ¼�У��������£�
��ǰĿ¼:\Report\*.xml                (* ��������ģ������)
��ǰĿ¼:\assertInfoFile.xls          (����ģ��ͳ�Ƶ�SCI_ASSERT/SCI_PASSERT�����������ת���)
��ǰĿ¼:\verifiedAssertInfoFile.xls  (�����ļ�ͳ�ƹ��˵�assert/SCI_ASSERT/SCI_PASSERT�꣬����ת���)
��ǰĿ¼:\assertCheck.log             (MS_Code�����е�SCI_ASSERT/SCI_PASSERT�꼰��ת����б�)


//////////////////////////////////////////////////////////////////////////////////////////////////////////

һ�����������ӣ�
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