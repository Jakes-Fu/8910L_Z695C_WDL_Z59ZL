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

#��������
my $cpu_type = "ARM926EJ";
my $big_endian= "OFF";
my $t32_path = "C:\\T32sim\\";
my $t32_cmm = "T32.CMM";
my $t32cmm = $t32_path . $t32_cmm;

#��ȡ��ǰ·��
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
2012-3-28 11:53:23 ������ջ�Ĵ���ֳ�ȥ��һ���������ļ�����Ϊ����������Ҫ����������ջ��Ϣ
12:15 2012-3-21 �޸��쳣ջ���Ƶ����쳣֮���ֵӦ����+ CPSR+ r0,1,2,3,4,5,6,7,8,9,10,11,12 + x + r14 + PC + 0X*(�˴���Ӧ�ĵ�ַ��Ϊ׼ȷ��R13ֵ)������+x��ʾ����
18:28 2012-3-19 ������������ջ��ѯ���ܣ���Ӧ�ű��ڵ�ǰĿ¼�¡�__t32_task_stack_cmm__��Ŀ¼�У�����������������t32-sim�С�Run Batchfile��������˵�fileѡ������Ӧ�ļ����ɡ�
----
0 ����˵��
	1 Ĭ�ϵ�T32�ļ�����"T32sim"�������װĿ¼��ͬ�����޸�
	2 �����perl�����Խ���������ĳ�ccperl����cqperl
	3 ���������к�һ��������˵���д��󣬿���������ģʽ���в鿴������Ϣ��
1 �������
	1 ��
	2 �Զ���ȡ��ǰĿ¼��ass�ļ���mem�ļ�
	3 ���Ȼ�ȡ��ǰĿ¼�µ�axf�ļ�
	4 ��ǰĿ¼����axf�����Ե���Ŀ¼�²���
	5 Ĭ������cpuΪ"ARM926EJ"��������Ҫ�����޸�
	6 Ĭ����С�ˣ�������Ҫ�������޸ı���"$big_endian"Ϊ"ON"
2 �� ass file ����������
	1 �ڴ�ƫ�Ƶ�ַ - ���� memory file �Ĳ���
	2 ��ǰ���쳣���� - ���üĴ����������������ݣ���ʵ�ϣ��쳣�����ñȽϼĴ�������ȷ
	3 �쳣���ֵĵ�ַ - ���������쳣����
	4 �Ĵ�����ֵ - �������üĴ�������ͨ���ȽϼĴ���ֵ��õ�ǰ��ȷ���쳣����
3 дcmm�ļ�
	1 ��·
	2 ����cpu
	3 ���ô�С��
	4 ����axf file
	5 ����memory file
	6 register set���������current mode����ͣ�������mode
	7 �����SVC mode���� pc ������ assert ʱ
	8 ����ջ���ڡ��Ĵ������ں�����ֳ��Ĵ��봰��
4 �л�������ģʽ�Ĵ���
	"FIQ" => "0x11",
	"IRQ" => "0x12",
	"SVC" => "0x13",
	"Abort" => "0x17",
	"Undefined" => "0x1B",
5 �������ǽ�ģʽ����ΪSVC mode�����ң�����ֻ�ܴ����������:
	1 SVC mode
	2 SVC mode -> Abort mode
6 ͨ������������£�����Ҫ�ֶ����ã����߾��ǲ��ܽ�����ջ��
7 �����ظ���log����£����Ժ����log
=cut
__END__
:endofperl
pause
