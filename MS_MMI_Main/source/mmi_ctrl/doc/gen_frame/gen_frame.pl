use strict;
use File::Copy;

die "perl gen_frame.pl CtrlName [ParentName]" unless (@ARGV == 1 or @ARGV == 2);

my $Name = $ARGV[0];
my $name = "\L$Name";
my $NAME = "\U$Name";

my $Parent = "Base";

$Parent = $ARGV[1] if @ARGV == 2;

my $parent = "\L$Parent";
my $PARENT = "\U$Parent";

#copy new file
copy("ctrldummy_export.h", "ctrl${name}_export.h") or die "copy failed: $!";
copy("ctrldummy.h", "ctrl${name}.h") or die "copy failed: $!";
copy("ctrldummy.c", "ctrl${name}.c") or die "copy failed: $!";

#replace [CTRL]->NAME, [Ctrl]->Name, [ctrl]->name
system("perl -p -i.bak -e \"s/\\[CTRL\\]/$NAME/g\" ctrl${name}_export.h ctrl${name}.h ctrl${name}.c");
system("perl -p -i.bak -e \"s/\\[Ctrl\\]/$Name/g\" ctrl${name}_export.h ctrl${name}.h ctrl${name}.c");
system("perl -p -i.bak -e \"s/\\[ctrl\\]/$name/g\" ctrl${name}_export.h ctrl${name}.h ctrl${name}.c");

#replace [PARENT]->NAME, [Parent]->Name, [parent]->name
system("perl -p -i.bak -e \"s/\\[PARENT\\]/$PARENT/g\" ctrl${name}_export.h ctrl${name}.h ctrl${name}.c");
system("perl -p -i.bak -e \"s/\\[Parent\\]/$Parent/g\" ctrl${name}_export.h ctrl${name}.h ctrl${name}.c");
system("perl -p -i.bak -e \"s/\\[parent\\]/$parent/g\" ctrl${name}_export.h ctrl${name}.h ctrl${name}.c");

#under dos, must give a backup file, so need delete it... tuokuzifangpi...
unlink("ctrl${name}_export.h.bak");
unlink("ctrl${name}.h.bak");
unlink("ctrl${name}.c.bak");

__END__
0 �������밴������ĸ��д����ʽ��������Ϊ����ĸ��д�Ŀ��ܻ�ʹ�����ʵ�����ĸ����Ҫ
��д�������޷����ڶ�����д��ĸ�����
1 �����ļ���dummy->cltr_name
2 ���ļ��е��ַ����滻��DUMMY->NAME, Dummy->Name, dummy->name
3 ���û��parent name����parentĬ��Ϊbase