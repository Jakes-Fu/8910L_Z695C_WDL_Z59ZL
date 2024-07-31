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
0 参数必须按照首字母大写的形式给出，因为首字母大写的可能会使两个词的首字母都需要
大写，程序无法辨别第二个大写字母在哪里。
1 复制文件：dummy->cltr_name
2 将文件中的字符串替换：DUMMY->NAME, Dummy->Name, dummy->name
3 如果没有parent name，则parent默认为base