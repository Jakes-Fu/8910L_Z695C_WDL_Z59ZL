@rem = '--*-Perl-*--
@echo off
if "%OS%" == "Windows_NT" goto WinNT
ccperl -x -S "%0" %1 %2 %3 %4 %5 %6 %7 %8 %9
goto endofperl
:WinNT
ccperl -x -S %0 %*
if NOT "%COMSPEC%" == "%SystemRoot%\system32\cmd.exe" goto endofperl
if %errorlevel% == 9009 echo You do not have Perl in your PATH.
if errorlevel 1 goto script_failed_so_exit_with_non_zero_val 2>nul
goto endofperl
@rem ';
#!/usr/bin/perl -w
#line 15
use strict;

die "Usage: perl trace32cmm_auto.pl" if(@ARGV > 0);

#常量设置
my $cpu_type = "ARM926EJ";
my $big_endian= "OFF";
my $t32_path = "C:\\T32sim\\";
my $t32_cmm = "T32.CMM";
my $t32cmm = $t32_path . $t32_cmm;

#获取当前路径
chomp (my $cur_dir=`cd`);

my @files=`dir /A-D/B \"$cur_dir\"`;

my $ass_file = undef;
my $mem_file = undef;
my $axf_file = undef;
my $ass_file_cnt = 0;
my $mem_file_cnt = 0;
my $axf_file_cnt = 0;

foreach (@files){
	chomp;
	
	if (/\.ass$/) {
		$ass_file = "$cur_dir\\$_";
		$ass_file_cnt++;
	} elsif (/\.mem\d*$/) {
		$mem_file = "$cur_dir\\$_";
		$mem_file_cnt++;
	} elsif (/\.axf$/) {
		$axf_file = "$cur_dir\\$_";
		$axf_file_cnt++
	}
}

die "Can not find ass file" unless defined $ass_file;
die "Can not find mem file" unless defined $mem_file;

print "\nWARNING: There are $ass_file_cnt ass files searched...\n" if ($ass_file_cnt > 1);
print "\nWARNING: There are $mem_file_cnt mem files searched...\n" if ($mem_file_cnt > 1);

unless (defined $axf_file) {
	chdir "..";
	chomp (my $cur_dir=`cd`);
	my @files=`dir /A-D/B \"$cur_dir\"`;

	foreach (@files) {
		chomp;
		
		if (/\.axf$/) {
			$axf_file = "$cur_dir\\$_" ;
			$axf_file_cnt++;
		}
	}
}

die "Can not find axf file" unless defined $axf_file;
print "\nWARNING: There are $axf_file_cnt mem files searched...\n" if ($axf_file_cnt > 1);

open(ass_file, "< $ass_file") or die "$ass_file: $!";

print "\nass file: $ass_file
mem file: $mem_file
axf file: $axf_file
cpu: $cpu_type
big endian: $big_endian\n\n";


my $memory_begin_addr;
my $current_status;
my $exception_at;
my $reg_mode;
my %reg_current;
my %reg_svc;
my %reg_irq;
my %reg_abort;
my %reg_undefined;
my %reg_fiq;
my %log_cnt;

while (<ass_file>) {
	if (/Memory Dumping Finished:begin addr=([xX\da-fA-F]+),total size/) {
		print "assert file - Memory Dumping Finished:begin addr=$1\n";
		if (defined $log_cnt{"Memory"}) {
			print "repeated Memory setting, ignored!!!\n"
		} else {
			$memory_begin_addr = $1;
			$log_cnt{"Memory"} = 1;
		}
	} elsif (/Current status is (SVC|exception)/) {
		print "assert file - Current status is $1\n";
		if (defined $log_cnt{"status"}) {
			print "repeated status setting, ignored!!!\n"
		} else {
			$current_status = $1;			
			$log_cnt{"status"} = 1;
		}
	} elsif(/Exception at ([xX\da-fA-F]+)/) {
		print "assert file - Exception at $1 \n";
		if (defined $log_cnt{"Exception"}) {
			print "repeated Exception setting, ignored!!!\n"
		} else {
			$exception_at = $1;			
			$log_cnt{"Exception"} = 1;
		}
	} elsif (/(Current|SVC|IRQ|Abort|Undefined|FIQ) mode:/) {
		if (defined $log_cnt{$1}) {
			$reg_mode = "ALREADY_SETTING";
		} else {
			$reg_mode = $1;
			$log_cnt{$1} = 1;
		}
	} elsif (/^[\s]+([R\dPCS]+)[\s]*= (0x[\da-z]+)([\s]+([R\dPCS]+)[\s]*= (0x[\da-z]+))?/) {
		if ($reg_mode eq "Current") {
			$reg_current{$1} = $2;
			$reg_current{$4} = $5 if defined($3);
		} elsif ($reg_mode eq "SVC") {
			$reg_svc{$1} = $2;
			$reg_svc{$4} = $5 if defined($3);
		} elsif ($reg_mode eq "IRQ") {
			$reg_irq{$1} = $2;
			$reg_irq{$4} = $5 if defined($3);
		} elsif ($reg_mode eq "Abort") {
			$reg_abort{$1} = $2;
			$reg_abort{$4} = $5 if defined($3);
		} elsif ($reg_mode eq "Undefined") {
			$reg_undefined{$1} = $2;
			$reg_undefined{$4} = $5 if defined($3);
		} elsif ($reg_mode eq "FIQ") {
			$reg_fiq{$1} = $2;
			$reg_fiq{$4} = $5 if defined($3);
		}
	}
}

close (ass_file);

die "No register information!" unless defined $reg_current{"PC"};

open(T32CMM_FILE, "> $t32cmm") or die "$t32cmm: $!";

print T32CMM_FILE ";Default startup program for TRACE32
;
;This startup program can be modified according to your needs.

;choose hex mode for input
    radix hex

;Add some extra buttons to the toolbar

    menu.rp
    (
        add
        toolbar
        (
            separator
            toolitem \"Source/List\" \"list\" \"Data.List\"
            toolitem \"Memory dump\" \"dump\" \"Data.dump\"
            toolitem \"Register\" \"reg\" \"Register\"
            separator
            toolitem \"Watch\" \":varwatch\" \"Var.Watch\"
            toolitem \"Stack\" \"\:varframe\" \"Var.Frame /l /c\"
            toolitem \"Automatic Watch\" \":varref\" \"Var.Ref\"
            separator
            toolitem \"List Breakpoints\" \"break\" \"Break.List\"
            toolitem \"List Symbols\" \"symbols\" \"sYmbol.Browse\"
            separator
        )
    )

    if language()!=\"\"
    (
        local &menuname
        &menuname=\"~~/t32\"+language()
        menu.rp &menuname
    )

;Recall and Define History File
    autostore , history

SYStem.CPU $cpu_type
system.Up\
SYStem.Option BigEndian $big_endian

D.LOAD $axf_file /STRIPPART 4
DATA.LOAD.BINARY $mem_file $memory_begin_addr /noclear
\n";

#
#reg
#
#FIQ Mode
print T32CMM_FILE "\nR.S M 0x11\n\n";
print T32CMM_FILE "R.S $_ $reg_fiq{$_}\n" foreach (sort keys %reg_fiq);

#IRQ Mode
print T32CMM_FILE "\nR.S M 0x12\n\n";
print T32CMM_FILE "R.S $_ $reg_irq{$_}\n" foreach (sort keys %reg_irq);

#SVC Mode
print T32CMM_FILE "\nR.S M 0x13\n\n";
print T32CMM_FILE "R.S $_ $reg_svc{$_}\n" foreach (sort keys %reg_svc);

#Abort Mode
print T32CMM_FILE "\nR.S M 0x17\n\n";
print T32CMM_FILE "R.S $_ $reg_abort{$_}\n" foreach (sort keys %reg_abort);

#Undefined Mode
print T32CMM_FILE "\nR.S M 0x1B\n\n";
print T32CMM_FILE "R.S $_ $reg_undefined{$_}\n" foreach (sort keys %reg_undefined);

#Current Mode
my $current_mode;
my %mode_code = (
	"FIQ" => "0x11",
	"IRQ" => "0x12",
	"SVC" => "0x13",
	"Abort" => "0x17",
	"Undefined" => "0x1B",
);

if ($reg_current{"R13"} eq $reg_fiq{"R13"}
	and $reg_current{"R14"} eq $reg_fiq{"R14"}
	and $reg_current{"SPSR"} eq $reg_fiq{"SPSR"}) {
	$current_mode = "FIQ";
} elsif ($reg_current{"R13"} eq $reg_irq{"R13"}
	and $reg_current{"R14"} eq $reg_irq{"R14"}
	and $reg_current{"SPSR"} eq $reg_irq{"SPSR"}) {
	$current_mode = "IRQ";
} elsif ($reg_current{"R13"} eq $reg_svc{"R13"}
	and $reg_current{"R14"} eq $reg_svc{"R14"}
	and $reg_current{"SPSR"} eq $reg_svc{"SPSR"}) {
	$current_mode = "SVC";
} elsif ($reg_current{"R13"} eq $reg_abort{"R13"}
	and $reg_current{"R14"} eq $reg_abort{"R14"}
	and $reg_current{"SPSR"} eq $reg_abort{"SPSR"}) {
	$current_mode = "Abort";
} elsif ($reg_current{"R13"} eq $reg_undefined{"R13"}
	and $reg_current{"R14"} eq $reg_undefined{"R14"}
	and $reg_current{"SPSR"} eq $reg_undefined{"SPSR"}) {
	$current_mode = "Undefined";
}

unless (defined $current_mode) {
	print "\nWARNING: Unknow exception mode! Use SVC mode as the default.\n";
	$current_mode = "SVC";
}

print "Current mode is [$current_mode mode]\n";

if ($current_mode eq "Abort") {
	$reg_current{"CPSR"} = $reg_current{"SPSR"};
	$reg_current{"PC"} = sprintf("0x%x", hex($reg_abort{"R14"}) - 8);
} else {
	$reg_current{"PC"} = $reg_svc{"R14"};
}

$reg_current{"R14"} = $reg_svc{"R14"};
$reg_current{"R13"} = $reg_svc{"R13"};

print T32CMM_FILE "\nR.S M 0x13\n\n";
print T32CMM_FILE "R.S $_ $reg_current{$_}\n" foreach (sort keys %reg_current);

print T32CMM_FILE "
B::Var.frame /LOCALS
B::Register
\n\nenddo
";

close(T32CMM_FILE);


__END__
=pod
Change List
2012-3-28 11:53:23 将生成栈的代码分出去成一个单独的文件，因为并不总是需要生成这样的栈信息
12:15 2012-3-21 修改异常栈的推导，异常之后的值应该是+ CPSR+ r0,1,2,3,4,5,6,7,8,9,10,11,12 + x + r14 + PC + 0X*(此处对应的地址即为准确的R13值)，其中+x表示留空
18:28 2012-3-19 增加生成任务栈查询功能，对应脚本在当前目录下“__t32_task_stack_cmm__”目录中，以任务名命名。在t32-sim中“Run Batchfile”（点击菜单file选项）导入对应文件即可。
----
0 配置说明
	1 默认的T32文件夹是"T32sim"，如果安装目录不同，请修改
	2 如果无perl，可以将启动程序改称ccperl或者cqperl
	3 如果点击运行后一闪而过，说明有错误，可在命令行模式运行查看错误信息。
1 输入参数
	1 无
	2 自动获取当前目录下ass文件和mem文件
	3 优先获取当前目录下的axf文件
	4 当前目录下无axf，尝试到父目录下查找
	5 默认设置cpu为"ARM926EJ"，如有需要，可修改
	6 默认是小端，如有需要，可以修改变量"$big_endian"为"ON"
2 从 ass file 解析出数据
	1 内存偏移地址 - 载入 memory file 的参数
	2 当前的异常类型 - 设置寄存器及载入代码的依据，事实上，异常类型用比较寄存器更精确
	3 异常出现的地址 - 用于载入异常代码
	4 寄存器的值 - 用于设置寄存器，并通过比较寄存器值获得当前精确的异常类型
3 写cmm文件
	1 套路
	2 设置cpu
	3 设置大小端
	4 载入axf file
	5 载入memory file
	6 register set，最后设置current mode，并停留在这个mode
	7 如果是SVC mode，将 pc 调整至 assert 时
	8 弹出栈窗口、寄存器窗口和最后现场的代码窗口
4 切换到其他模式的代码
	"FIQ" => "0x11",
	"IRQ" => "0x12",
	"SVC" => "0x13",
	"Abort" => "0x17",
	"Undefined" => "0x1B",
5 我们总是将模式设置为SVC mode，并且，我们只能处理两种情况:
	1 SVC mode
	2 SVC mode -> Abort mode
6 通常在其他情况下，就需要手动设置，或者就是不能解析出栈了
7 在有重复的log情况下，忽略后面的log
=cut
__END__
:endofperl
pause
