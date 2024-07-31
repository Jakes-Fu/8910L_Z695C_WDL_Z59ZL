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
	}
}

die "Can not find ass file" unless defined $ass_file;
die "Can not find mem file" unless defined $mem_file;

print "\nWARNING: There are $ass_file_cnt ass files searched...\n" if ($ass_file_cnt > 1);
print "\nWARNING: There are $mem_file_cnt mem files searched...\n" if ($mem_file_cnt > 1);

open(ass_file, "< $ass_file") or die "$ass_file: $!";

my $memory_begin_addr;
my %log_cnt;
my %cur_ptr;

while (<ass_file>) {
	if (/Memory Dumping Finished:begin addr=([xX\da-fA-F]+),total size/) {
		print "assert file - Memory Dumping Finished:begin addr=$1\n";
		if (defined $log_cnt{"Memory"}) {
			print "repeated Memory setting, ignored!!!\n"
		} else {
			$memory_begin_addr = $1;
			$log_cnt{"Memory"} = 1;
		}
	} elsif (/^\s+0x[0-9a-f]+\s+(.+?)\s+\d+\s+\d+\s+\d+\s+(0x[0-9a-f]+)\s+0x[0-9a-f]+\s+0x[0-9a-f]+\s+$/) {
		my $name = $1;
		my $ptr = $2;
		$name =~ s/ /_/g;
		$cur_ptr{$name} = $ptr;
		#print "$name: $ptr\n";
	}
}

close (ass_file);


#����taskջ
=pod
׼����������TRACE32 LOAD��AXF�ļ���MEMORY�ļ� 
�Ƶ����裨��app taskΪ������ 
(1)��view---register 
(2)�鿴ass�ļ���Tasks info: �ҵ�app task��Ӧ��Current_PCֵ����ΪPC 
(3)�鿴ass�ļ���stack info: �ҵ�app task��Ӧ��Cur_Ptrֵ����ΪR13 
Ȼ���һ��ȡ��R13��׼ȷֵ����TRACE32�ж�R13�Ҽ�ѡIndirect Dump 
��ǰֵΪ0��ʾ����suspend + CPSR+ r4,5,6,7,8,9,10,11,12 +PC + 0X000000(�˴���Ӧ�ĵ�ַ��Ϊ׼ȷ��R13ֵ) 
��ǰֵΪ1ʱ��Ӧ + CPSR+ r0,1,2,3,4,5,6,7,8,9,10,11,12 + x + r14 +PC + 0X*(�˴���Ӧ�ĵ�ַ��Ϊ׼ȷ��R13ֵ) 
�������Ϳ��Կ���app task�Ķ�ջ��������ˡ�һ������´�ʱ��Ӧ�Ĵ���ֵ��ASS�ļ���һ�µ� 
���register�е�ǰ��Ĵ���ֵ�в��Եģ������ֶ��޸��£��Կ���׼ȷ�ĺ�������ֵ�� 
�����ô˷�ʵ��һ��248798��app�ĵ�������� 
=cut

my $task_stack_dir = "__t32_task_stack_cmm__";

system("mkdir $task_stack_dir") unless -d $task_stack_dir;

open(MEM_FILE, "<:raw", $mem_file) or die "$mem_file: $!";

foreach (keys %cur_ptr) {
	my $task = $_;
	my $cmm_file = "$task_stack_dir\\$task.cmm";
	my $ptr = hex($cur_ptr{$task}) - hex($memory_begin_addr);
	my $mem;
	my $mem_len = 18*4;
	my $read_len;
	my @cur_reg_data;
	my $task_cpsr = 0;
	my $task_r0 = 0;
	my $task_r1 = 0;
	my $task_r2 = 0;
	my $task_r3 = 0;
	my $task_r4 = 0;
	my $task_r5 = 0;
	my $task_r6 = 0;
	my $task_r7 = 0;
	my $task_r8 = 0;
	my $task_r9 = 0;
	my $task_r10 = 0;
	my $task_r11 = 0;
	my $task_r12 = 0;
	my $task_r13 = 0;
	my $task_r14 = 0;
	my $task_pc = 0;
	
	seek(MEM_FILE, $ptr, 0);	
	$read_len = read(MEM_FILE, $mem, $mem_len);
	
	if ($big_endian eq "OFF") {
		@cur_reg_data = unpack("L*", $mem);
	} else {
		@cur_reg_data = unpack("N*", $mem);
	}
		
	if ($cur_reg_data[0] == 0) {
		#CPSR+ r4,5,6,7,8,9,10,11,12 +PC + 0X000000(�˴���Ӧ�ĵ�ַ��Ϊ׼ȷ��R13ֵ)
		$task_cpsr = sprintf("0x%x", $cur_reg_data[1]);
		$task_r4 = sprintf("0x%x", $cur_reg_data[2]);
		$task_r5 = sprintf("0x%x", $cur_reg_data[3]);
		$task_r6 = sprintf("0x%x", $cur_reg_data[4]);
		$task_r7 = sprintf("0x%x", $cur_reg_data[5]);
		$task_r8 = sprintf("0x%x", $cur_reg_data[6]);
		$task_r9 = sprintf("0x%x", $cur_reg_data[7]);
		$task_r10 = sprintf("0x%x", $cur_reg_data[8]);
		$task_r11 = sprintf("0x%x", $cur_reg_data[9]);
		$task_r12 = sprintf("0x%x", $cur_reg_data[10]);
		$task_pc = sprintf("0x%x", $cur_reg_data[11]);
		$task_r13 = sprintf("0x%x", hex($cur_ptr{$task}) + 12*4);		
	} else {
		#CPSR+ r0,1,2,3,4,5,6,7,8,9,10,11,12 + r14 +PC + 0X*(�˴���Ӧ�ĵ�ַ��Ϊ׼ȷ��R13ֵ)
		$task_cpsr = sprintf("0x%x", $cur_reg_data[1]);
		$task_r0 = sprintf("0x%x", $cur_reg_data[2]);
		$task_r1 = sprintf("0x%x", $cur_reg_data[3]);
		$task_r2 = sprintf("0x%x", $cur_reg_data[4]);
		$task_r3 = sprintf("0x%x", $cur_reg_data[5]);
		$task_r4 = sprintf("0x%x", $cur_reg_data[6]);
		$task_r5 = sprintf("0x%x", $cur_reg_data[7]);
		$task_r6 = sprintf("0x%x", $cur_reg_data[8]);
		$task_r7 = sprintf("0x%x", $cur_reg_data[9]);
		$task_r8 = sprintf("0x%x", $cur_reg_data[10]);
		$task_r9 = sprintf("0x%x", $cur_reg_data[11]);
		$task_r10 = sprintf("0x%x", $cur_reg_data[12]);
		$task_r11 = sprintf("0x%x", $cur_reg_data[13]);
		$task_r12 = sprintf("0x%x", $cur_reg_data[14]);
		$task_r14 = sprintf("0x%x", $cur_reg_data[16]);
		$task_pc = sprintf("0x%x", $cur_reg_data[17]);
		$task_r13 = sprintf("0x%x", hex($cur_ptr{$task}) + 18*4);		
	}

	open(T32CMM_FILE, "> $cmm_file") or die "$cmm_file: $!";
	
	print T32CMM_FILE ";Run this Batchfile for $task: $cur_ptr{$task}
R.S M 0x13
R.S CPSR $task_cpsr
R.S R0 $task_r0
R.S R1 $task_r1
R.S R2 $task_r2
R.S R3 $task_r3
R.S R4 $task_r4
R.S R5 $task_r5
R.S R6 $task_r6
R.S R7 $task_r7
R.S R8 $task_r8
R.S R9 $task_r9
R.S R10 $task_r10
R.S R11 $task_r11
R.S R12 $task_r12
R.S R13 $task_r13
R.S R14 $task_r14
R.S PC $task_pc
\n";
		
	close(T32CMM_FILE);
}

close(MEM_FILE);


__END__
:endofperl
pause
