#!perl\win
BEGIN{push @INC,'./Sprd_atest_pm'};

#-----------------------------------------------------------------------------------
# Description      : this script is used for checking the dountful assert macro 
#                    in the mocor project.
# Author           : Mingxuan.Zhu
# Date             : 01/03/2010
# Note             : search condition is follows:
#                    1. SCI_ASSERT / SCI_PASSERT.
#                    2. redefined macro for SCI_ASSERT / SCI_PASSERT.
#-----------------------------------------------------------------------------------
# Edit History
#    DATE    |    EDITOR NAME    |     DESCRIPTION
# 01/03/2010     Mingxuan.Zhu          Created    
# 10/01/2011     Mingxuan.Zhu          Updated ( added the statistics function ) 
# 24/01/2011     Mingxuan.Zhu          Updated ( added redefined macro search  )
# 04/03/2011     Mingxuan.Zhu          Updated ( added assert verified and assert to do statistics ) 
# 22/03/2011     Mingxuan.Zhu          Updated ( added generate macro list and makefile file )
# 25/04/2011     Mingxuan.Zhu          Updated ( added assert keyword named TRACE in function )
#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Perl Lib 
#-----------------------------------------------------------------------------------
use Sprd_atest_extract_used_code;

#-----------------------------------------------------------------------------------
# Main Program 
#-----------------------------------------------------------------------------------
# receive the check path from command line
my $path            = $ARGV[0];
my $resultPath      = $ARGV[1];
my $searchMacroFlag = $ARGV[2]; # 1:new 2:update
my $branchFlag      = $ARGV[3]; # 1:09a 2:68h 3:10a 4:mt
my $searchRangeFlag = $ARGV[4]; # 1:ALL 2:MODULE
my $crossModuleFlag = $ARGV[5]; # 1:cross module 0:no cross module
# set default value of cross module flag.
if($crossModuleFlag eq "")
{
  $crossModuleFlag = 1;
}
# global variable initialization.
my $preProcessSwitch = 1;
my $currentModuleName = "";
my @redefinedMacroList = ();      # the list of redefined macro.
my @definedMacroList = ();        # the list of defined macro.
my @multiRedefinedMacroList = (); # the list of multi redefined macro.
my @thirdPartyPathList = ();      # the path list of third-party folders and files.
my @filteredPathList = ();        # the path list of filtered folders and files.
# global constant initialization.
my $assertFilterKeyword = Sprd_atest_get_keyword::getAssertVerifiedValue();
my $assertDoubtfulKeyword = Sprd_atest_get_keyword::getAssertToDoValue();
my $toolsPropertiesFilePath = ".\\assertMacroCheck.properties";
my $currentReportDir = $resultPath."\\Report";
my $logFileDir = $resultPath."\\assertCheck.log";
my $assertInfoFilePath = $resultPath."\\assertInfoFile.xls";
my $verifedAssertInfoFilePath = $resultPath."\\verifiedAssertInfoFile.xls";
my $mainPath = substr($path,0,(index($path,"MS_Code")+length("MS_Code")));
my $makeFolderPath = "$mainPath\\make";
my $usedCodeList = "";
my $cppCommentFilePath = ".\\cppCommentFile.txt";
my $noCommentFilePath = ".\\noCommentFile.txt";
my $assertInfoTempFilePath = ".\\assertInfoTempFile.txt";

if(-d $resultPath)
{
  if(-d $makeFolderPath)
  {
	# check the flag of search macro define.
    $searchMacroFlag = checkSearchMacroFlag($searchMacroFlag);
    # check the flag of branch.
    $branchFlag = "mt";
    # $branchFlag = checkBranchFlag($branchFlag);
    # check the flag of search range
    $searchRangeFlag = checkSearchRangeFlag($searchRangeFlag);
	# print check start time
    my $systime = localtime(time);
    print "Assert Macro Check has started at ".$systime."\n";
    print "Please waiting......\n";
    # extract used code in project named MOCOR
    $usedCodeList = Sprd_atest_extract_used_code::extractUsedCode($mainPath,$branchFlag);
    # create report directory
    if(-d $currentReportDir && $crossModuleFlag==1)
    {
	  system("rd /q/s $currentReportDir"); 
    }
    # 
    mkdir($currentReportDir,0001);

    # delete the old files.
    unlink($cppCommentFilePath);
    unlink($noCommentFilePath);
    if($searchMacroFlag =~ /^new$/i)
    {
	  unlink($logFileDir);
	  if($crossModuleFlag==1)
	  {
	  	unlink($verifedAssertInfoFilePath);
	  }
    }
    # delete stat files.
    unlink($assertInfoTempFilePath);
    if($crossModuleFlag==1)
    {
      unlink($assertInfoFilePath);
    }
    # get the properties information of tools.
    getToolsPropertiesInfo();
    # generate the report file title of verified macro. 
    generateTitleOfVerifedMacroStatFile();
    # add original assert macro.
    push(@redefinedMacroList,"SCI_ASSERT");
    push(@redefinedMacroList,"SCI_PASSERT");
    push(@redefinedMacroList,"assert");
    # the main program which checks the macro named SCI_ASSERT
    if(-d $path)
    { # folder process logic
	  # save inputed path
	  my $subPath=$path."\\";
	  if($searchMacroFlag =~ /^new$/i)
	  {
	    # search redefined macro.
	    if($searchRangeFlag =~ /^all$/i)
	    { # all ms_code module
	  	  searchCodeFilePath($mainPath."\\");
	    }
	    else
	    { # single module
	  	  searchCodeFilePath($subPath);
	    }
      # get multi redefined macro list 
      getMultiRedefinedMacroList();
	  }
	  # get all redefined macro list
      getRedefinedMacroList($searchMacroFlag);
      # reset the switch of pre-process.
      $preProcessSwitch = 0;
      # search assert statement.
      searchCodeFilePath($subPath);
    }
    else
    { # file process logic
      print "The first parameter of this script must be folder path.";
    }
    # delete the temp file
    unlink($cppCommentFilePath);
    unlink($noCommentFilePath);
    # generate stat file about assert info.
    if(-f $assertInfoTempFilePath)
    {
      generateAssertInfoStatFile();
    }
    # format report file
    formatReportFile();
    if($searchMacroFlag =~ /^new$/i)
    { # generate log file
      generateLogFile(); 	
    }
    # print search end time
    $systime = localtime(time);
    print "Assert Macro Check has ended at ".$systime."\n";
    print "The report file ($resultPath) has been generated in current catalog.\n\n";
  }
  else
  {
    print "The folder named \"make\" does not exist in MS_Code project!\n";
    print "Please mount the folder named \"make\"!\n";
  }	
}
else
{
  print "The folder named \"$resultPath\" does not exist!\n";
  print "Please ensure that the folder named \"$resultPath\" exists!\n";
}

#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Sub Function 
#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Description      : check the flag of search macro define.
# Input Parameter  : the flag of search macro define. 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 28/03/2011
#-----------------------------------------------------------------------------------
sub checkSearchMacroFlag
{
	my($searchMacroFlag)=@_;
  # 
	if($searchMacroFlag =~ /^\s*$/)
	{
	  print "============Please select the switch of assert statements check:===========\n";
	  print "== 1:new       #renew check assert statements.                           ==\n";
	  print "== 2:update    #check assert statements after annotate assert statements.==\n";         
	  print "===========================================================================\n";
	  print "Please enter switch[1|2]";
	  $_ = <STDIN>;
	  chomp;
	  $searchMacroFlag = $_;
		
	  if($searchMacroFlag eq "1")
	  {
		  $searchMacroFlag = "new";
	  }
	  else
	  {
		  $searchMacroFlag = "update";
	  }
	}
	return $searchMacroFlag;
}

#-----------------------------------------------------------------------------------
# Description      : check the flag of branch.
# Input Parameter  : the flag of branch. 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 31/03/2011
#-----------------------------------------------------------------------------------
sub checkBranchFlag
{
  my($branchFlag)=@_;
  # 
  if($branchFlag =~ /^\s*$/)
  {
    print "=================Please select the switch of mocor branch:=================\n";
    print "== 1:09a       # 09a branch                                              ==\n";
    print "== 2:68h       # sc6800h branch                                          ==\n";
    print "== 3:10a       # 10a branch                                              ==\n";
    print "== 4:mt        # mt branch                                               ==\n";
    print "===========================================================================\n";
    print "Please enter switch[1|2|3|4]";
    $_ = <STDIN>;
    chomp;
    $branchFlag = $_;
  	
    if($branchFlag eq "1")
    {
  	  $branchFlag = "09a";
    }
    elsif($branchFlag eq "2")
    {
  	  $branchFlag = "68h";
    }
    elsif($branchFlag eq "3")
    {
  	  $branchFlag = "10a";
    }
    elsif($branchFlag eq "4")
    {
  	  $branchFlag = "mt";
    }
  }
  return $branchFlag;
}

#-----------------------------------------------------------------------------------
# Description      : check the flag of search range
# Input Parameter  : the flag of search range. 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 13/04/2011
#-----------------------------------------------------------------------------------
sub checkSearchRangeFlag
{
  my($searchRangeFlag)=@_;
  # 
	if($searchRangeFlag =~ /^\s*$/)
	{
	  print "====================  Please select the range of search  :=================\n";
	  print "== 1:ALL       # all module                                              ==\n";
	  print "== 2:MODULE    # single module                                           ==\n";
	  print "===========================================================================\n";
	  print "Please enter switch[1|2]";
	  $_ = <STDIN>;
	  chomp;
	  $searchRangeFlag = $_;
		
	  if($searchRangeFlag eq "1")
	  {
		  $searchRangeFlag = "ALL";
	  }
	  elsif($searchRangeFlag eq "2")
	  {
		  $searchRangeFlag = "MODULE";
	  }
	}
	return $searchRangeFlag;
}

#-----------------------------------------------------------------------------------
# Description      : check whether the file path is third-party code path or not.
# Input Parameter  : the path of file.
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 18/02/2011
#-----------------------------------------------------------------------------------
sub isThirdPartyCode
{
	my($filePath)=@_;
	my $isTPSwitch = 0;
	
	foreach $eachPath(@thirdPartyPathList)
	{
		my $currentPath = $eachPath; 
		$currentPath =~ s/\\/\\\\/g;
    
		if($filePath =~ /$currentPath/i)
		{
			$isTPSwitch = 1;
			last;
		}
	}
	return $isTPSwitch;
}

#-----------------------------------------------------------------------------------
# Description      : generate the condition string from the array of assert macro list.
# Input Parameter  : the array of assert macro list. 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 24/01/2011
#-----------------------------------------------------------------------------------
sub generateConditionExpression
{
	my(@assertMacroList)=@_;
	
	foreach $eachMacroName(@assertMacroList)
	{
  	chomp $eachMacroName;
  	$conditionExpression = $conditionExpression."(\$currentStatement =~ /$eachMacroName/ && \$currentStatement !~ /#define/ && (\$currentStatement !~ /$eachMacroName\\S+/ || \$currentStatement =~ /$eachMacroName\\(/))"."||";
	}
	$conditionExpression = substr($conditionExpression,0,(length($conditionExpression)-length("||"))); 
}

#-----------------------------------------------------------------------------------
# Description      : check whether the current statement contains assert key or not.
# Input Parameter  : the content of current line 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 28/01/2011
#-----------------------------------------------------------------------------------
sub isAssertKeyword
{
	my($codeStatement)=@_;
	my $assertKeyword = 0;
	
	if( $codeStatement =~ /ASSERT/i || $codeStatement =~ /VERIFY/i
	 || $codeStatement =~ /CKECK/i || $codeStatement =~ /STATE/i
	 || $codeStatement =~ /CHECK/i || $codeStatement =~ /WARNING/i
	 || $codeStatement =~ /Fail/i || $codeStatement =~ /DEBUG/i
	 || $codeStatement =~ /MEMCHK/i || $codeStatement =~ /_mul/i
	 || $codeStatement =~ /Mutex/i || $codeStatement =~ /TRACE/i 
	 || $codeStatement =~ /VAL/i )
	{ 
		if($codeStatement =~ /SCI_MEMSET/)
		{
			$assertKeyword = 0;
		}
		else
		{
			$assertKeyword = 1;
		}
	}
	else
	{
		$assertKeyword = 0;
	}
	return $assertKeyword;
}

#-----------------------------------------------------------------------------------
# Description      : check whether the current statement is the assert statement or not.
# Input Parameter  : the content of current line 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 22/01/2011
#-----------------------------------------------------------------------------------
sub isAssertStatement
{
	my($currentStatement)=@_;
	# local variable initialization.
	my $assertType = "NO";
	my $isAssert = 0;
	
	if(!isAssertKeyword($currentStatement))
	{ # the switch of normal statement
		$assertType = "NO";
	}
	else
	{ # the switch of assert statement which contains redefined macro.
	  foreach $eachMacroName(@redefinedMacroList)
      {
 	  chomp $eachMacroName;  	
 	  #----------------------------------------
 	  # Assert Statement Style
 	  # 1. assert
 	  #    (
 	  #     )
 	  # 2. assert (
 	  #    )
 	  # 3.assert (
 	  #    )
 	  #----------------------------------------  
 	  if( $currentStatement =~ /$eachMacroName/i
 	   && $currentStatement !~ /#define/
 	   &&( $currentStatement =~ /^\s*$eachMacroName\s*$/i
 	    || $currentStatement =~ /^\s*$eachMacroName\s*\(/i )
 	   && $currentStatement !~ /^\s*$eachMacroName\s*\(\s*\"/i  # filter trace.
 	    )
	  {
	      if($eachMacroName eq "assert" && $currentStatement =~ /ASSERT/)
	      {
	        $assertType = "ASSERT";
	      }
	      else
	      {
	        $assertType = $eachMacroName; # get the name of redefined macro.
	      }
	      last;
	  }
    }
    if( $assertType eq "NO" && $currentStatement =~ /^\s*SCI_P?Assert\s*\(*/
     && $currentStatement !~ /\\\s*$/ )
    {
    	$assertType = "SCI_Assert";
    }
	}
	return $assertType;
}

#-----------------------------------------------------------------------------------
# Description      : recognise the function which contains the dountful assert macro
# Input Parameter  : 1. temp file (noCommentFile.txt)
#                    2. source file 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 03/03/2010
#                    22/01/2011 ( add redefined macro process )
#-----------------------------------------------------------------------------------
sub generateAssertInfoFile
{
	my(@file)=@_;
	
	# the .c file name which does not contain the comments
	my $fileName = @file[0];
	# the .c source file path and name which contains the comments
	my $filePath = @file[1];
	# print "$filePath\n";
  # 
  open(ASSERT_INFO_TEMP_FILE,">>$assertInfoTempFilePath")||die "can't open the file:$!";
  my @fileContent = Sprd_atest_file_process::getFileContent($fileName);
  my $maxLineNo = @fileContent;
  
  # local variable initialization.
  $filePath =~ s/MS_Code\\(\S+)/MS_Code\\$1/;
  my $issueFileName = $1;
  my $moduleName = substr($issueFileName,0,index($issueFileName,"\\"));
    
  my $assertStatementNumber = 0;
  my $assertVerifiedStatementNumber = 0;
  my $assertToDoStatementNumber = 0;
  my $moduleNameSwitch = 0;
  my $assertStatementSwitch = 0;
  my $assertTypeName = "";
  my $thirdPartySwitch = isThirdPartyCode($filePath);
  
  if(!$thirdPartySwitch)
  {
  	open(ERROR_ASSERT,">>$currentReportDir\\$moduleName.xml")||die "can't open the file:$!";
  }
  else
  {
  	my $moduleNameForTP = $moduleName."_3Party.xml";
  	open(ERROR_ASSERT,">>$currentReportDir\\$moduleNameForTP")||die "can't open the file:$!";
  }  
  # 
  $lineNo = 1;
  $braceRange = 0;
  # 
  foreach $eachline(@fileContent)
  {
  	chomp $eachline;
  	my $filterFlag = 0;
  	my $assertStatement = "";
    # 
  	$assertTypeName = isAssertStatement($eachline);
  	# get all of assert statements from files that suffix is c.
  	if($assertTypeName ne "NO")
  	{ # get the doubtful assert statements.
  		my $assertLineNo = $lineNo;
  		chomp @fileContent[$assertLineNo-1];
      if(@fileContent[$assertLineNo-1] =~ /\/\*$assertFilterKeyword\*\//)
      {
    	  @fileContent[$assertLineNo-1] =~ s/\/\*$assertFilterKeyword\*\///g;
    	  $filterFlag = 1; # assert verified
      }
      elsif(@fileContent[$assertLineNo-1] =~ /\/\*$assertDoubtfulKeyword\*\//)
      {
        @fileContent[$assertLineNo-1] =~ s/\/\*$assertDoubtfulKeyword\*\///g;
        $filterFlag = 2; # assert to do    	
      }
      @fileContent[$assertLineNo-1] =~ s/^\s*|\s*$//g;
      if(@fileContent[$assertLineNo-1] =~ /\\$/) # filter multiline macro define.
      {
      	$lineNo++;
      	next;
      }
  		while(@fileContent[$assertLineNo-1] !~ /\)\s*;$/ && $assertLineNo <= $maxLineNo && $filterFlag==0)
  		{
  			$assertStatement = $assertStatement.@fileContent[$assertLineNo-1];
  			$assertLineNo++;
  			chomp @fileContent[$assertLineNo-1];
        if(@fileContent[$assertLineNo-1] =~ /\/\*$assertFilterKeyword\*\//)
        {
    	    @fileContent[$assertLineNo-1] =~ s/\/\*$assertFilterKeyword\*\///g;
    	    $filterFlag = 1; # assert verified
        }
        elsif(@fileContent[$assertLineNo-1] =~ /\/\*$assertDoubtfulKeyword\*\//)
        {
          @fileContent[$assertLineNo-1] =~ s/\/\*$assertDoubtfulKeyword\*\///g;
          $filterFlag = 2; # assert to do    	
        }
  	    @fileContent[$assertLineNo-1] =~ s/^\s*|\s*$//g;
  		}
  		if($assertLineNo > $maxLineNo)
  		{
  			$assertStatement = $assertStatement.@fileContent[$assertLineNo-1];
  		}  		
  		# 
  		if($filterFlag==1)
      {
    	  $assertStatement = $assertStatement." /*$assertFilterKeyword*/";
      }
      elsif($filterFlag==2)
      {
      	$assertStatement = $assertStatement." /*$assertDoubtfulKeyword*/";
      }
  		# 
  		if($assertStatement =~ /$assertFilterKeyword/i)
  	  { # skip the verified assert statements.
  		  $assertStatementSwitch = 1;
  		  $assertVerifiedStatementNumber++;
  	  }
  	  elsif($assertStatement =~ /$assertDoubtfulKeyword/i)
  	  { # skip the assert to do statements.
  		  $assertStatementSwitch = 1;
  		  $assertToDoStatementNumber++;			
  	  }
  	  else
  	  {
  		  $assertStatementSwitch = 1;
  		  print ERROR_ASSERT "<issue file =\"$issueFileName\" line = \"$lineNo\" number = \"001\"  desc = \"$assertTypeName\"/>\n";
  		  $assertStatementNumber++;    	  	
  	  }
  	}
  	$lineNo++;
  }
  # 
  if($assertStatementSwitch==1)
  {
 		# print ERROR_ASSERT "</assert_analysis>\n\n";
 		if(!$thirdPartySwitch)
 		{
 			print ASSERT_INFO_TEMP_FILE "$moduleName,$assertStatementNumber,$assertVerifiedStatementNumber,$assertToDoStatementNumber\n";
 		}
  	else
  	{
  		my $moduleNameForTP = $moduleName."_3Party";
  		print ASSERT_INFO_TEMP_FILE "$moduleNameForTP,$assertStatementNumber,$assertVerifiedStatementNumber,$assertToDoStatementNumber\n";
  	}
  }
  # close the file which does not contain the comments
  close(ERROR_ASSERT); 
  close(ASSERT_INFO_TEMP_FILE);
}

#-----------------------------------------------------------------------------------
# Description      : get the file which is pre
# Input Parameter  : the path of the code file. 
# Output Parameter : 
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 11/01/2011
#-----------------------------------------------------------------------------------
sub getPreprocessedFile
{
	my($filePath)=@_;
	my $ifMacroFlag = 0;
	# filter slash star comment.
  Sprd_atest_file_process::filterSlashStarComment($filePath,$cppCommentFilePath);
  # filter double slash comment. 
  $ifMacroFlag = Sprd_atest_file_process::filterDoubleSlashComment($cppCommentFilePath,$noCommentFilePath);
  # 
  if($ifMacroFlag==1 || $ifMacroFlag==3)
  {
  	Sprd_atest_file_process::filterIfSwitchStatement($noCommentFilePath);
  }
  elsif($ifMacroFlag==2 || $ifMacroFlag==3)
  {
  	Sprd_atest_file_process::filterElseSwitchStatement($noCommentFilePath);
  }
}

#-----------------------------------------------------------------------------------
# Description      : check single file which contains PUBLIC function
# Input Parameter  : the related .c file 
# Output Parameter : the source file
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 03/03/2010
#-----------------------------------------------------------------------------------
sub getAssertInfo
{
  my($filePath)=@_;
  my $filterFlag = 0;
  my $moduleInfo = substr($filePath,(index($filePath,"MS_Code")+length("MS_Code\\"))); 
  #
  foreach $filterKeyword(@filteredPathList)
  {
  	chomp $filterKeyword;
  	my $currentPath = $filterKeyword;
   	$currentPath =~ s/\\/\\\\/g;
   	if( $moduleInfo =~ /$currentPath/i 
   	&& (!isThirdPartyCode($filePath) || $currentPath =~ /\.c$/i))
   	{
   		$filterFlag = 1;
   		last;
   	}
  }
  # 
  if($filterFlag==0)
  {
   	getPreprocessedFile($filePath);
    # generate assert information file.
    generateAssertInfoFile($noCommentFilePath,$filePath);
  }
}

#-----------------------------------------------------------------------------------
# Description      : get redefined macro list.
# Input Parameter  : 1. SCI_ASSERT redefined line
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 22/01/2011
#-----------------------------------------------------------------------------------
sub getNameListOfRedefinedMacro
{
	my($lineContent)=@_;
	my $sameMacroNameSwitch = 0;
	# get redefined macro name.
	$redefinedMacroName = substr($lineContent,(index($lineContent," ")+1));
	$macroNameEndPos = index($redefinedMacroName," ");
	# filter the macro that style is as follows:
	# "#define XXXXX"
	if($macroNameEndPos!=-1)
	{
		$redefinedMacroName = substr($redefinedMacroName,0,index($redefinedMacroName," "));
	  if($redefinedMacroName =~ /\(/)
	  {
		  $redefinedMacroName = substr($redefinedMacroName,0,index($redefinedMacroName,"("));
	  }
	  # filter the same macro name.
    for($i=0;$i<@redefinedMacroList;$i++)
    {
  	  if(@redefinedMacroList[$i] eq $redefinedMacroName)
  	  {
  		  $sameMacroNameSwitch = 1;
  		  last;
  	  }
    }
    # 
    if($sameMacroNameSwitch==0)
    {
  	  push(@redefinedMacroList,$redefinedMacroName);
    }
	}
}

#-----------------------------------------------------------------------------------
# Description      : get macro define of multi-line macro.
# Input Parameter  : 1. the line No. of current line
#                    2. the content of the file which contains "#define" statement.
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 09/02/2011
#-----------------------------------------------------------------------------------
sub getMultiLineMacroDefine
{
	my($currentLineNo,@fileContent)=@_;
	my $nextLineNo = $currentLineNo+1;
	my $currentLine = @fileContent[$currentLineNo-1];
	my $nextLine = @fileContent[$nextLineNo-1];
	chomp $nextLine;
	if($nextLine !~ /\\$/)
	{
		$currentLine =~ s/\\$/$nextLine/;
	}
	else
	{
		while($nextLine =~ /\\$/)
    {
      $nextLine =~ s/\s/ /g;
      $nextLine =~ tr/ / /s;
      $currentLine =~ s/\\$/$nextLine/;
      $nextLineNo++;
      $nextLine = @fileContent[$nextLineNo-1];
      chomp $nextLine;
    }
    $currentLine =~ s/\\$/$nextLine/;
	}
	return $currentLine;
}


#-----------------------------------------------------------------------------------
# Description      : get the content of the line that keyword named SCI_ASSERT is redefined.
# Input Parameter  : 1. $fileSuffix
#                    2. $filePath 
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 20/01/2011
#-----------------------------------------------------------------------------------
sub getLineContentOfRedefinedMacro
{
	my($filePath)=@_;
	my $lineNo = 1;
	my $blankSpaceNumber = 0;
	
  # get the code file which does not contain the comments.
	getPreprocessedFile($filePath);
	
  my @fileContent = Sprd_atest_file_process::getFileContent($noCommentFilePath);

	foreach my $eachLine(@fileContent)
	{
	  chomp $eachLine;
    # initialize the contents of each line.
	  $eachLine =~ s/^\s+//g;
		$eachLine =~ s/\s+$//g;
		$eachLine =~ s/\s/ /g;
		$eachLine =~ tr/ / /s;
		$blankSpaceNumber = $eachLine =~ tr/ / /;
		# 
	  if( isAssertKeyword($eachLine)  && $eachLine =~ /^\s*\#define/
	   && $eachLine !~ /SCI_ASSERT/   && $eachLine !~ /SCI_PASSERT/
	   && $eachLine !~ /\d$/          && $eachLine !~ /\(\d\)/
	   && $eachLine !~ /#define\s*VI/ && $eachLine !~ /SDL/  # filter the VI and SDL code
	   && $blankSpaceNumber != 1      && $eachLine !~ /DWORD/
	   && $eachLine !~ /0x/           && $eachLine !~ /\#define\s*y/ ) # filter hexadecimal number and y prefix format.
	  {
	  	# print $filePath."\n";
	  	if($eachLine =~ /\\$/)
	  	{ 		
	  		$eachLine = getMultiLineMacroDefine($lineNo,@fileContent);
	  	}
	  	if($eachLine !~ /$assertFilterKeyword/i)
	  	{
	  		push(@definedMacroList,$eachLine);
	  	}
	  	else
	  	{
	  		generateVerifedMacroStatFile($filePath,$lineNo,$eachLine);
	  		# print "$filePath\n===>||$eachLine||\n";
	  	}
	  }
		#  
	  if( ( $eachLine =~ /#define/ && $eachLine =~ /SCI_ASSERT/i && ( $eachLine !~ /SCI_ASSERT\S+/ || $eachLine =~ /\s+SCI_ASSERT\s*\(/ ) )
	    ||( $eachLine =~ /#define/ && $eachLine =~ /SCI_PASSERT/i && ( $eachLine !~ /SCI_PASSERT\S+/ || $eachLine =~ /\s+SCI_PASSERT\s*\(/ ) )
	    ||( $eachLine =~ /#define/ && $eachLine =~ /assert/ && ( ($eachLine !~ /assert\S+/ && $eachLine !~ /\S+assert/) || $eachLine =~ /\s+assert\s*\(/ ) )
	    )
	  {
		  if( $eachLine =~ /#define\s+SCI_ASSERT/ 
		   || $eachLine =~ /#define\s+SCI_PASSERT/ )
		  {
		  	$lineNo++;
		  	next;
		  }
		  elsif($eachLine =~ /$assertFilterKeyword/i)
		  {
		  	generateVerifedMacroStatFile($filePath,$lineNo,$eachLine);
		  	# print "$filePath\n=+=><<$eachLine>>\n";
		  	$lineNo++;
		  	next;
		  }
      getNameListOfRedefinedMacro($eachLine);
	  }
	  $lineNo++;
	}
}

#-----------------------------------------------------------------------------------
# Description      : search the .c files in project's folder
# Input Parameter  : the .c file 
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 08/01/2010
#                    20/01/2010 ( add redefined macro search function )
#-----------------------------------------------------------------------------------
sub searchCodeFilePath
{
	my($input)=@_;

  # save file and folder to arrayList
  my @folderList = Sprd_atest_file_process::getDirList($input);
  
  foreach my $item(@folderList)
  {
    if(-d $input.$item)
    { # open folder and recursively call sub function      
      my @subDirList;
      if(!IsFilterPath($input.$item))
      {
      	@subDirList = Sprd_atest_file_process::getDirList($input.$item);
      }
      # check the folder is empty or not empty
      if(@subDirList>0)
      {
        searchCodeFilePath($input.$item."\\");
      }
   	}
   	else
   	{ # read file's contents   
   	  # split filename  	   
   	  $suffixStartPos = rindex($item,".")+1;
   	  $suffix = substr($item,$suffixStartPos);
   	  if($preProcessSwitch==1)
   	  {
   	  	# print "$input.$item\n=====$suffix\n";
   	  	if(($suffix =~ /^c$/i || $suffix =~ /^h$/i) && !IsFilterPath($input.$item))
   	  	{
   	  		getLineContentOfRedefinedMacro($input.$item);
   	  	}
   	  }
   	  else
   	  {
   	  	if($item =~ /\.c$/i && $usedCodeList =~ /\s+$item\s+/)
   	  	{
   	  		getAssertInfo($input.$item);
   	  	}
   	  }	    
   	}
  }   
}

#-----------------------------------------------------------------------------------
# Description      : genereate the statistics file about assert statement.
# Input Parameter  : the temp file which contains the number information of assert statement  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 10/01/2011
#-----------------------------------------------------------------------------------
sub generateAssertInfoStatFile
{ # 
	my @assertInfoList = Sprd_atest_file_process::getFileContent($assertInfoTempFilePath);
	# 
	my $assertInfoListLength = @assertInfoList;
	my @filterModuleName = ();
	# 
	open(ASSERT_INFO_FILE,">>$assertInfoFilePath")||die "can't open the file:$!";
	my @fileContent = Sprd_atest_file_process::getFileContent($assertInfoFilePath);
	if(@fileContent==0)
	{
		print ASSERT_INFO_FILE "Module Name\tAssert Statement Total Quantity\tAssert Verified Total Quantity\tAssert ToDo Total Quantity\n";
	}
	# 
  for($i=0; $i<$assertInfoListLength; $i++)
  {
  	my $assertNumber = 0;
  	my $assertVerifiedNumber = 0;
  	my $assertToDoNumber = 0;
  	my $filterModuleNameFlag = 0;
  	chomp @assertInfoList[$i];	
    $currentFirstCommaPos = index(@assertInfoList[$i],",");
    $currentModuleName = substr(@assertInfoList[$i],0,$currentFirstCommaPos);
    $currentModuleParam = substr(@assertInfoList[$i],($currentFirstCommaPos+1));
    $currentSecondCommaPos = ($currentFirstCommaPos + 1) + index($currentModuleParam,",");
    $currentThirdCommaPos = rindex(@assertInfoList[$i],",");   
    $assertNumber =  $assertNumber + substr(@assertInfoList[$i],($currentFirstCommaPos+1),$currentSecondCommaPos);
    $assertVerifiedNumber = $assertVerifiedNumber + substr(@assertInfoList[$i],($currentSecondCommaPos+1),$currentThirdCommaPos);
    $assertToDoNumber = $assertToDoNumber + substr(@assertInfoList[$i],($currentThirdCommaPos + 1));
    foreach $eachName(@filterModuleName)
    {
    	if($currentModuleName eq $eachName)
      {
    	  $filterModuleNameFlag = 1;
    	  last;
      }
    }

    if($filterModuleNameFlag==1)
    {
    	next;
    }
    # 
    for($j=$i+1; $j<$assertInfoListLength; $j++)
    {
    	chomp @assertInfoList[$j];
    	$nextFirstCommaPos = index(@assertInfoList[$j],",");
    	$nextModuleName = substr(@assertInfoList[$j],0,$nextFirstCommaPos);
    	$nextModuleParam = substr(@assertInfoList[$j],($nextFirstCommaPos+1));
      $nextSecondCommaPos = ($nextFirstCommaPos + 1) + index($nextModuleParam,",");
      $nextThirdCommaPos = rindex(@assertInfoList[$j],","); 	
      # 
    	if($nextModuleName eq $currentModuleName)
    	{
    		$assertNumber =  $assertNumber + substr(@assertInfoList[$j],($nextFirstCommaPos+1),$nextSecondCommaPos);
    		$assertVerifiedNumber =  $assertVerifiedNumber + substr(@assertInfoList[$j],($nextSecondCommaPos+1),$nextThirdCommaPos);
    		$assertToDoNumber =  $assertToDoNumber + substr(@assertInfoList[$j],($nextThirdCommaPos+1));
    	}
    }
    push(@filterModuleName,$currentModuleName);
    print ASSERT_INFO_FILE "$currentModuleName\t$assertNumber\t$assertVerifiedNumber\t$assertToDoNumber\n";
  }
  #  
	close(ASSERT_INFO_FILE);
	unlink($assertInfoTempFilePath);
}

#-----------------------------------------------------------------------------------
# Description      : format xml file.
# Input Parameter  : NONE  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 25/01/2011
#-----------------------------------------------------------------------------------
sub formatReportFile
{ # 
  my @xmlFileList = Sprd_atest_file_process::getDirList($currentReportDir);
  
  foreach my $eachFile(@xmlFileList)
  {
  	my $xmlFilePath = $currentReportDir."\\".$eachFile;
	  my @xmlContent = Sprd_atest_file_process::getFileContent($xmlFilePath);
  	
  	if(@xmlContent>0 && @xmlContent[0] !~ /assert_analysis/)
    {
  	  open(XML_FILE,">$xmlFilePath")||die "can't open the file:$!";
  	  print XML_FILE "<assert_analysis>\n";
  	  foreach my $eachLine(@xmlContent)
  	  {  	  
  	    chomp $eachLine;
  	    print XML_FILE $eachLine."\n";
  	  }  
  	  print XML_FILE "</assert_analysis>\n";
      close(XML_FILE);
    }
    elsif(@xmlContent>0)
    {
    	# do nothing.
    }
    else
    {
  	  unlink($xmlFilePath);
    }
  }
}

#-----------------------------------------------------------------------------------
# Description      : generate the log file which contains assert macro name.
# Input Parameter  : NONE  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 28/01/2011
#-----------------------------------------------------------------------------------
sub generateLogFile
{
	open(LOG_FILE,">$logFileDir")||die "can't open the file:$!";
	
	foreach my $eachAssertName(@redefinedMacroList)
	{
		chomp $eachAssertName;
		print LOG_FILE $eachAssertName."\n";
	}
	close(LOG_FILE);
}

#-----------------------------------------------------------------------------------
# Description      : get multi redefined macro list.
# Input Parameter  : NONE  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 09/02/2011
#-----------------------------------------------------------------------------------
sub getMultiRedefinedMacroList
{
	foreach $element(@definedMacroList)
  {  
	  my $macroDefName = substr($element,(index($element,"#define ")+length("#define ")));
	  
	  foreach $assertMacroName(@redefinedMacroList)
	  { # filter the same macro.
	    if($element =~ /\s+$assertMacroName/ && $macroDefName !~ /^$assertMacroName/)
	    {
			  $element =~ s/\s/ /g;
			  $element =~ tr/ / /s;
			  $macroDefName = substr($macroDefName,0,index($macroDefName," "));
			  if($macroDefName =~ /\(/)
			  {
			  	$macroDefName = substr($macroDefName,0,index($macroDefName,"("));
			  }
			  #
			  if(@multiRedefinedMacroList==0)
			  { # first element.
			  	push(@multiRedefinedMacroList,$macroDefName);
			  }
			  else
			  { # others element.
			  	my $isSameMacro = 0;
			  	for($i=0;$i<@multiRedefinedMacroList;$i++)
			    {
			  	  if($macroDefName eq @multiRedefinedMacroList[$i])
			  	  { # filter the same macro name.
			  		  $isSameMacro = 1;
			  		  last;
			  	  }
			    }
			    # 
			    if(!$isSameMacro)
			    {
			    	push(@multiRedefinedMacroList,$macroDefName);
			    }
			  }
	      last;
	    }
	  }
  }
}

#-----------------------------------------------------------------------------------
# Description      : get all redefined macro list.
# Input Parameter  : NONE  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 10/02/2011
#-----------------------------------------------------------------------------------
sub getRedefinedMacroList
{
	my($searchMacroFlag)=@_;
	
	if($searchMacroFlag =~ /^new$/i)
	{
	  foreach my $element(@multiRedefinedMacroList)
	  {
		  my $isSameMacro = 0;
		  for($i=0;$i<@redefinedMacroList;$i++)
		  {
			  if($element eq @redefinedMacroList[$i])
			  { # filter the same macro name.
			    $isSameMacro = 1;
			    last;
			  }
		  }
		  # 
		  if(!$isSameMacro)
		  {
			  push(@redefinedMacroList,$element);
		  }
	  }		
	}
	else
	{
	  my @LogfileContent = Sprd_atest_file_process::getFileContent($logFileDir);
	  
	  foreach my $eachMacroName(@LogfileContent)
	  {
		  chomp $eachMacroName;
		  push(@redefinedMacroList,$eachMacroName);
	  }		
	}
}

#-----------------------------------------------------------------------------------
# Description      : get the path of the third-party folder and file.
# Input Parameter  : NONE  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 18/02/2011
#-----------------------------------------------------------------------------------
sub getToolsPropertiesInfo
{ # get the content of properties file.
	my @propertiesFileContent = Sprd_atest_file_process::getFileContent("$toolsPropertiesFilePath");
	
	my $areaSwitch = 0; # 0:Third-party Area 1:Filtered Area
	
	foreach my $eachLine(@propertiesFileContent)
	{
		chomp $eachLine;
		if($eachLine =~ /^\s*$/)
		{ # skip the blank line.
			next;
		}
		else
		{ # skip the comment line.
			if($eachLine =~ /#/)
			{
				if($eachLine =~ /3PF/)
				{
					if($eachLine =~ /start/i)
					{
						$areaSwitch = 0;
					}
					elsif($eachLine =~ /end/i)
					{
						$areaSwitch = 2;
					}
				}
				elsif($eachLine =~ /FF/)
				{
					if($eachLine =~ /start/i)
					{
						$areaSwitch = 1;
					}
					elsif($eachLine =~ /end/i)
					{
						$areaSwitch = 2;
					}
				}
				next;
			}
		}
		# get paths of folders and files by area switch.
		if($areaSwitch==0)
		{
			push(@thirdPartyPathList,$eachLine);
		}
		elsif($areaSwitch==1)
		{
			push(@filteredPathList,$eachLine);
		}
	}
}

#-----------------------------------------------------------------------------------
# Description      : generate the report file of verified macro.
# Input Parameter  : $filePath  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 24/03/2011
#-----------------------------------------------------------------------------------
sub generateVerifedMacroStatFile
{
	my($filePath,$lineNo,$currentLine)=@_;
	$filePath =~ s/MS_Code\\(\S+)/MS_Code\\$1/;
	my $relativeFilePath = $1;
	my $moduleName = substr($relativeFilePath,0,index($relativeFilePath,"\\"));
	my $fileName = substr($relativeFilePath,(rindex($relativeFilePath,"\\")+1));
	
	open(VERIFIED_ASSERT_INFO_FILE,">>$verifedAssertInfoFilePath")||die "can't open the file:$!";
	print VERIFIED_ASSERT_INFO_FILE "$moduleName\t$fileName\t$lineNo\t$currentLine\n";
	close(VERIFIED_ASSERT_INFO_FILE);
}

#-----------------------------------------------------------------------------------
# Description      : generate the report file title of verified macro.
# Input Parameter  : NONE  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 24/03/2011
#-----------------------------------------------------------------------------------
sub generateTitleOfVerifedMacroStatFile
{
	open(VERIFIED_ASSERT_INFO_FILE,">>$verifedAssertInfoFilePath")||die "can't open the file:$!";
	my @fileContent = Sprd_atest_file_process::getFileContent($verifedAssertInfoFilePath);
	if(@fileContent==0)
	{
		print VERIFIED_ASSERT_INFO_FILE "moduleName\tfileName\tLineNo\tlineContent\n";
	}
	close(VERIFIED_ASSERT_INFO_FILE);
}

#-----------------------------------------------------------------------------------
# Description      : filter the trace statements.
# Input Parameter  : $currentLine  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 11/05/2011
#-----------------------------------------------------------------------------------
sub isTraceStatement
{
	my($currentLine)=@_;
	my $isTraceFlag = 0;
	if($currentLine =~ /\(\s*\"/)
	{
		$isTraceFlag = 1;
	}
	return $isTraceFlag;
}

#-----------------------------------------------------------------------------------
# Description      : check whether the file path is filtered file.
# Input Parameter  : $filePath  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 15/07/2011
#-----------------------------------------------------------------------------------
sub IsFilterPath
{
  my($filePath)=@_;
  my $isFilterFlag = 0;
  my $moduleInfo = substr($filePath,(index($filePath,"MS_Code")+length("MS_Code")));
  #
  foreach $filterKeyword(@filteredPathList)
  {
  	chomp $filterKeyword;
  	my $currentPath = $filterKeyword;
   	$currentPath =~ s/\\/\\\\/g;
   	if($moduleInfo =~ /\\$currentPath\\/i)
   	{
   		$isFilterFlag = 1;
   		last;
   	}
  }
  # 
  return $isFilterFlag;	
}