#!perl\win
# BEGIN{push @INC,'./Sprd_atest_pm'};

package Sprd_atest_extract_used_code;

#-----------------------------------------------------------------------------------
# Description      : extract used code in MS_Code.
# Author           : Mingxuan.Zhu
# Date             : 23/03/2011
# Note             :
#-----------------------------------------------------------------------------------
# Edit History
#    DATE    |    EDITOR NAME    |     DESCRIPTION
# 23/03/2011     Mingxuan.Zhu          Created
#-----------------------------------------------------------------------------------
use strict;

#-----------------------------------------------------------------------------------
# Perl Lib
#-----------------------------------------------------------------------------------
use Sprd_atest_file_process;

#-----------------------------------------------------------------------------------
# Constant Definition
#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Global Variable Definition
#-----------------------------------------------------------------------------------
my @usedCodeList = ();

#-----------------------------------------------------------------------------------
# Sub Function
#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Description      : extract used code in MS_Code.
# Input Parameter  : $mscode_path
# Output Parameter : NONE
# Return Value     : the list of used code.
# Note             :
# Editor           : Mingxuan.Zhu
# Modified Date    : 23/03/2011
#-----------------------------------------------------------------------------------
sub extractUsedCode
{
	my($mscode_path,$branchFlag)=@_; # MS_Code's main path.
	my @folderPathListOfNonGsmMk = ();
	my @mkFilePathListOfNonGsmMk = ();
	my $usedCodeListStr = "";
	my $makeFolderPath = $mscode_path."\\make\\";
	my $branchProjList = Sprd_atest_get_keyword::getBranchProjList($branchFlag);
	# get the directory tree of current folder and get used 2.5G code in MS_Code.
	getDirTree($makeFolderPath,$branchProjList);
	# extract the makefile folder path of non-2.5G code.
	@folderPathListOfNonGsmMk = extractFolderPathOfNonGsmMk($mscode_path);
	# extract the makefile path of non-2.5G code.
	@mkFilePathListOfNonGsmMk = extractFilePathOfNonGsmMk(@folderPathListOfNonGsmMk);
	# get used non-2.5G code in MS_Code.
	extractUsedNonGsmCodeInMk($branchProjList,@mkFilePathListOfNonGsmMk);
	# get wre code in MS_Code.
	extractUsedCodeInWre($mscode_path);
	#
	$usedCodeListStr = "@usedCodeList";
	return $usedCodeListStr;
}

#-----------------------------------------------------------------------------------
# Description      : get the directory tree of current folder.
# Input Parameter  : $folderPath
# Output Parameter : NONE
# Return Value     : NONE
# Note             :
# Editor           : Mingxuan.Zhu
# Modified Date    : 08/01/2010
#-----------------------------------------------------------------------------------
sub getDirTree
{
	my($folderPath,$branchProjList)=@_;
  # save file and folder to arrayList
  my @folderList = Sprd_atest_file_process::getDirList($folderPath);

  foreach my $item(@folderList)
  {
    if(-d $folderPath.$item)
    { # open folder and recursively call sub function
    	my @subDirList = Sprd_atest_file_process::getDirList($folderPath.$item);
      # check the folder is empty or not empty
      if(@subDirList>0)
      {
        getDirTree($folderPath.$item."\\",$branchProjList);
      }
   	}
   	else
   	{ # read file's contents
   	  # split filename
   	  if($item =~ /.mk$/i)
   	  {
   	  	extractUsedCodeInMk($folderPath.$item,$branchProjList);
   	  }
   	}
  }
}

#-----------------------------------------------------------------------------------
# Description      : extract used 2.5G code in MS_Code.
# Input Parameter  : the mk file.
# Output Parameter : NONE
# Return Value     : NONE
# Note             :
# Editor           : Mingxuan.Zhu
# Modified Date    : 23/03/2011
#-----------------------------------------------------------------------------------
sub extractUsedCodeInMk
{
	my($mkFilePath,$branchProjList)=@_;
	my @mkFileContent = Sprd_atest_file_process::getFileContent($mkFilePath);
	my $platformName = "";
	my $usedCodeFlag = 1;
  # 
  foreach my $eachLine(@mkFileContent)
  {
  	chomp $eachLine;
  	if( $eachLine =~ /^\s*ifeq\s*\(\s*\$\s*\(\s*strip\s*\$\s*\(\s*PLATFORM\s*\)\s*\)\s*\,\s*(\S+)\s*\)/
  	 || $eachLine =~ /^\s*ifeq\s*\(\s*\$\s*\(\s*strip\s*\$\s*\(\s*(PLATFORM\S+)\s*\)\s*\)\,\s*TRUE\s*\)/ )
  	{
  		$platformName = $1;
  		if($branchProjList =~ /$platformName/)
  		{
  			$usedCodeFlag = 1;
  		}
  		else
  		{
  			$usedCodeFlag = 0;
  		}
  	}
  	elsif($eachLine =~ /^\s*endif/ || $eachLine =~ /^\s*else/)
  	{
  		$usedCodeFlag = 1;
  	}
  	
  	if($usedCodeFlag==1)
  	{
      if($eachLine =~ /\.c\s*\\*\n*\r*/i && $eachLine !~ /\%+|\$+\S*\.c/i
      && $eachLine !~ /^\s*\#\S*\.c/i)
  	  {
  		  my $codeInfo = $eachLine;
  		  # print $codeInfo."\n";
  		  while($codeInfo =~ /(\S+\.c)/)
  		  {
  			  my $codeFileName = $1;
  			  if($codeFileName =~ /\=(\S+)/)
  			  {
  				  $codeFileName = $1;
  			  }
          # print "FilterCodeName===>$codeFileName\n";
  			  push(@usedCodeList,$codeFileName);
  			  $codeInfo = substr($codeInfo,(index($codeInfo,$codeFileName)+length($codeFileName)));
  			  # print "FilterCodeInfo===>$codeInfo\n";
  		  }
  	  }  		
  	}
  }
}

#-----------------------------------------------------------------------------------
# Description      : extract the makefile folder path of non-2.5G code.
# Input Parameter  : Makefile.rules.
# Output Parameter : NONE
# Return Value     : the list of makefile folder path in non-2.5G code
# Note             :
# Editor           : Mingxuan.Zhu
# Modified Date    : 24/03/2011
#-----------------------------------------------------------------------------------
sub extractFolderPathOfNonGsmMk
{
	my($filePath)=@_;
	my @pathOfNonGsmMk = ();
	my $mkRulesFilePath = $filePath."\\Makefile.rules";
	my $mkFilePathFlag = 0;
  # 
  my @mkRulesContent = Sprd_atest_file_process::getFileContent($mkRulesFilePath);

  foreach my $eachLine(@mkRulesContent)
  {
  	chomp $eachLine;
  	if($eachLine =~ /\s*\#\s*makefile\s*location/ && $mkFilePathFlag == 0)
  	{
  		$mkFilePathFlag = 1;
  	}
  	elsif($mkFilePathFlag==1)
  	{
  		if( $eachLine =~ /^\s*MAKEFILE_DIR_\S+\s*=\s*(\S+)/
  		 && $eachLine !~ /make/
  		 && $eachLine !~ /XSR_STLMP/)
  		{
  			my $nonGsmMkPath = $1;
  			$nonGsmMkPath =~ s/^\s+|\s+$//g;
  			$nonGsmMkPath =~ s/\//\\/g;
  			$nonGsmMkPath = "$filePath\\$nonGsmMkPath";
  			# print $nonGsmMkPath."\n";
  			if(-d $nonGsmMkPath)
  			{
  			  if(@pathOfNonGsmMk==0)
  			  {
  				  push(@pathOfNonGsmMk,$nonGsmMkPath);
  			  }
  			  else
  			  {
  				  my $isSameFlag = 0;
  				  for(my $i=0; $i < @pathOfNonGsmMk; $i++)
  				  {
  					  if(@pathOfNonGsmMk[$i] eq $nonGsmMkPath)
  					  {
  						  $isSameFlag = 1;
  						  last;
  					  }
  				  }
  				  if($isSameFlag==0)
  				  {
  					  push(@pathOfNonGsmMk,$nonGsmMkPath);
  				  }
  			  }  				
  			}
  			else
  			{
  				next;
  			}
  		}
  		elsif($eachLine =~ /\s*\#\s*public\s*function/)
  		{
  			$mkFilePathFlag = 0;
  			last;
  		}
  	}
  }
  #
  return @pathOfNonGsmMk;
}

#-----------------------------------------------------------------------------------
# Description      : extract the makefile path of non-2.5G code.
# Input Parameter  : Makefile.rules.
# Output Parameter : NONE
# Return Value     : the list of makefile path in non-2.5G code
# Note             :
# Editor           : Mingxuan.Zhu
# Modified Date    : 24/03/2011
#-----------------------------------------------------------------------------------
sub extractFilePathOfNonGsmMk
{
	my(@folderList)=@_;
	my @mkFilePathList = ();

	foreach my $eachFolder(@folderList)
	{
		chomp $eachFolder;
		my @currentFolderList = Sprd_atest_file_process::getDirList($eachFolder);
		foreach my $eachItem(@currentFolderList)
		{
			if($eachItem =~ /.mk$/i)
			{
				push(@mkFilePathList,"$eachFolder\\$eachItem");
			}
		}
	}
	return @mkFilePathList;
}

#-----------------------------------------------------------------------------------
# Description      : extract used non-2.5G code.
# Input Parameter  : the mk file path of non-2.5G code.
# Output Parameter : NONE
# Return Value     : NONE
# Note             :
# Editor           : Mingxuan.Zhu
# Modified Date    : 24/03/2011
#-----------------------------------------------------------------------------------
sub extractUsedNonGsmCodeInMk
{
	my($branchProjList,@mkFileList)=@_;

	foreach my $eachFile(@mkFileList)
	{
		chomp $eachFile;
		extractUsedCodeInMk($eachFile,$branchProjList);
	}
}

#-----------------------------------------------------------------------------------
# Description      : extract used code in WRE.
# Input Parameter  : $filePath : path of MS_Code
# Output Parameter : NONE
# Return Value     : NONE
# Note             : WRE makefile path : MS_Code\wre\wrekernel\hw_prj\wrekernel_DebugRel.mk
# Editor           : Mingxuan.Zhu
# Modified Date    : 09/12/2011
#-----------------------------------------------------------------------------------
sub extractUsedCodeInWre
{
  my($filePath)=@_;
  my $mkFilePathOfWre = $filePath."\\wre\\wrekernel\\hw_prj\\wrekernel_DebugRel.mk";
  my @mkFileContent = Sprd_atest_file_process::getFileContent($mkFilePathOfWre);
  # my @dotCList = ();
  foreach my $eachLine(@mkFileContent)
  {
    chomp $eachLine;
    my $currentLine = $eachLine;
    if($currentLine =~ /(\w+\.c)/)
    {
      if("@usedCodeList" !~ /$1/)
      {
        push(@usedCodeList,$1);
      }
      $currentLine =~ s/$1//g;
    }
  }
}

1;

__END__
#-----------------------------------------------------------------------------------