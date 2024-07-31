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

#获取当前路径
chomp (my $cur_dir=`cd`);

my @files=`dir /A-D/B \"$cur_dir\"`;

my $ass_file = undef;
my $ass_file_cnt = 0;

foreach (@files){
	chomp;
	
	if (/\.ass$/) {
		$ass_file = "$cur_dir\\$_";
		$ass_file_cnt++;
	}
}

die "Can not find ass file" unless defined $ass_file;

print "\nWARNING: There are $ass_file_cnt ass files searched...\n" if ($ass_file_cnt > 1);

open(ass_file, "< $ass_file") or die "$ass_file: $!";

print "\nass file: $ass_file\n";

my %mem_data;

while (<ass_file>) {
	if (/^\s+(\d+)\s+(\d+)\s+([\w\d_]+.c) \(Line (\d+)\),addr:([0-9a-f]+)\s+$/) {
		$mem_data{$1}{size} = $2;
		$mem_data{$1}{file} = $3;
		$mem_data{$1}{line} = $4;
		$mem_data{$1}{addr} = $5;
	}
}

close (ass_file);


my $size_all = 0;

$size_all += $mem_data{$_}{size} foreach (keys %mem_data);


my $mem_statist_file;

$mem_statist_file = "__mem_statist_list_by_size.txt";

open(MEM_FILE, ">", $mem_statist_file) or die "$mem_statist_file: $!";

print MEM_FILE "totle: $size_all\n\n";

print MEM_FILE "$_\t$mem_data{$_}{size}\t $mem_data{$_}{file}($mem_data{$_}{line})\n" foreach (sort {$mem_data{$b}{size} <=> $mem_data{$a}{size}} keys %mem_data);

close(MEM_FILE);


$mem_statist_file = "__mem_statist_list_by_file.txt";

open(MEM_FILE, ">", $mem_statist_file) or die "$mem_statist_file: $!";

print MEM_FILE "totle: $size_all\n\n";

print MEM_FILE "$_\t$mem_data{$_}{size}\t $mem_data{$_}{file}($mem_data{$_}{line})\n" foreach (sort {$mem_data{$a}{file} cmp $mem_data{$b}{file}} keys %mem_data);

close(MEM_FILE);


__END__
:endofperl
pause
