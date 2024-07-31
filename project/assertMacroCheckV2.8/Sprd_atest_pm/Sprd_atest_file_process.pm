#!perl\win
# BEGIN{push @INC,'./Sprd_atest_pm'};

package Sprd_atest_file_process;

#-----------------------------------------------------------------------------------
# Description      : this perl module is used to process the file system.
# Author           : Mingxuan.Zhu
# Date             : 24/03/2011
# Note             :
#-----------------------------------------------------------------------------------
# Edit History
#    DATE    |    EDITOR NAME    |     DESCRIPTION
# 24/03/2011     Mingxuan.Zhu          Created
#-----------------------------------------------------------------------------------
use strict;

#-----------------------------------------------------------------------------------
# Perl Lib
#-----------------------------------------------------------------------------------
use Sprd_atest_get_keyword;

#-----------------------------------------------------------------------------------
# Constant Definition
#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Global Variable Definition
#-----------------------------------------------------------------------------------
our $win32_flag = 1;

#-----------------------------------------------------------------------------------
# Sub Function
#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Description      : get the directory list in the folder.
# Input Parameter  : the path of the folder.
# Output Parameter : NONE
# Return Value     : the directory list in the folder
# Note             :
# Editor           : Mingxuan.Zhu
# Modified Date    : 06/12/2010
#-----------------------------------------------------------------------------------
sub getDirList
{
	my($folderPath)=@_;

	opendir(FOLDER_DIR,$folderPath)||die "can't open the folder:$!";
  my @dirList=readdir FOLDER_DIR;
  @dirList=grep{$_ ne "." && $_ ne ".."}@dirList;
  closedir(FOLDER_DIR);

  return @dirList;
}

#-----------------------------------------------------------------------------------
# Description      : get the content of the file.
# Input Parameter  : the path of the file.
# Output Parameter : NONE
# Return Value     : the content of the file.
# Note             :
# Editor           : Mingxuan.Zhu
# Modified Date    : 24/03/2011
#-----------------------------------------------------------------------------------
sub getFileContent
{
	my($filePath)=@_;
	open(FILE_CONTENT,"<$filePath")||die "can't open the file:$filePath $!";
	my @fileContent = <FILE_CONTENT>;
	close(FILE_CONTENT);

  return @fileContent;
}

#-----------------------------------------------------------------------------------
# Description      : filter out the comment which is the form of "/*......*/"
# Input Parameter  : the related .c file 
# Output Parameter : the source file
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 03/03/2010
#-----------------------------------------------------------------------------------
sub filterSlashStarComment
{
	my($fileName,$cppCommentFilePath)=@_;
  #
  open(CPP_COMMENT_FILE,">$cppCommentFilePath")||die "can't open the file:$!";  
  my @fileContent = Sprd_atest_file_process::getFileContent($fileName);
  
  my $lineNo=1;
  my $skipLineNum=0;
  # 
  foreach my $eachline(@fileContent)
  { 
  	chomp $eachline;
  	$eachline =~ s/\r|\n/\n/g; # filter the LF/CR
  	my $commentStartPos = 0;
  	my $commentEndPos = 0;
  	my $commentStartLabelNumber = 0;
  	my $commentEndLabelNumber = 0;
  	my $filterKeyword = Sprd_atest_get_keyword::getFilterKeyword($eachline);
  	# 
  	if($skipLineNum!=0)
  	{
  		$skipLineNum--;
  		$lineNo++;
  		next;
  	}
  	# 
  	my $subLineNo=$lineNo;
  	#-------------------------------------------------------------
  	# recognise the comment which is the the form of "/*......*/"
  	# Example: the following is not comments , x:any character
  	# 1. ///*
  	# 2. //*
  	# 3. "xxxxx//xxxxx"
  	# 4. //xxxx/*xxxx
  	# 5. "xxxxx/*xxxxx"
  	# 6. /*xxxxxx//*/
  	#-------------------------------------------------------------
    if($eachline=~/\/\*/ && $eachline!~ /"\s*\S*\s*\/\/\s*\S*\s*"/
    && $eachline!~ /^\s*\/\/\s*\S*\s*\/*\*/ && $eachline!~ /"\s*\S*\s*\/\*\s*\S*\s*"/)
    { # the line which contains comments
    	my $doubleSlashStartPos = index($eachline,"//");
    	#filter /*/ mode
    	if($eachline =~ /\/\*\// && (index($eachline,"/*/")+1)!=rindex($eachline,"*/"))
    	{
    		$eachline =~ s/\/\*\//\/\*/g;
    	}
     	$commentStartPos = index($eachline,"/*");
     	$commentEndPos = rindex($eachline,"*/");
     	$commentStartLabelNumber = getLeftCommentPerLine($eachline);
     	if($commentEndPos != -1)
     	{
     		$commentEndLabelNumber = getRightCommentPerLine($eachline);
     	}
     	# filter the double slash comment.
     	if($doubleSlashStartPos > 0 && $doubleSlashStartPos < $commentStartPos)
     	{
     		print CPP_COMMENT_FILE "$eachline\n";
     		$lineNo++;
     		next;
     	}
      if($commentStartLabelNumber==$commentEndLabelNumber) 
     	{ # single-line comment
        my $tempLine = $eachline;
        my $headerOfLine = "";
        my $endOfLine = "";
        while($tempLine =~ /\/\*/)
        {
        	$commentStartPos = index($tempLine,"/*");
     	    $commentEndPos = index($tempLine,"*/");
        	$headerOfLine = substr($tempLine,0,$commentStartPos);
          $endOfLine = substr($tempLine,($commentEndPos+length("*/")));
          $tempLine = $headerOfLine.$endOfLine;
        }
        $tempLine =~ s/^\s*|\s*$//g;
        if($tempLine =~ /\\$/)
        {
        	$tempLine =~ s/\\//g;
        	print CPP_COMMENT_FILE $tempLine.$filterKeyword."  \\\n";
        }
        else
        {
        	print CPP_COMMENT_FILE $tempLine.$filterKeyword."\n";
        }
     	}
     	else  
     	{ # multi-line comment
     		chomp @fileContent[$subLineNo-1];
     		# filter single-line comment in current line.
     	  if($commentEndPos != -1)
     	  {
          my $headerOfLine = "";
          my $endOfLine = "";
     	  	while(@fileContent[$subLineNo-1] =~ /\/\*/)
          {
        	  $commentStartPos = index(@fileContent[$subLineNo-1],"/*");
     	      $commentEndPos = index(@fileContent[$subLineNo-1],"*/");
     	      if($commentEndPos==-1)
     	      {
     	      	last;
     	      }
        	  $headerOfLine = substr(@fileContent[$subLineNo-1],0,$commentStartPos);
            $endOfLine = substr(@fileContent[$subLineNo-1],($commentEndPos+length("*/")));
            @fileContent[$subLineNo-1] = $headerOfLine.$endOfLine;
          }
     	  }
        #    		
     		while(rindex(@fileContent[$subLineNo-1],"*/") == -1) # for line that does not icluded "*/"
     		{
     			$subLineNo = $subLineNo + 1;
     			chomp @fileContent[$subLineNo-1];
     		}
      	print CPP_COMMENT_FILE substr($eachline,0,$commentStartPos).$filterKeyword."\n";
     		for(my $iPos=($lineNo+1); $iPos <= $subLineNo; $iPos++)
     		{
     		  print CPP_COMMENT_FILE ""."\n";
     		}
     		$commentEndPos = rindex(@fileContent[$subLineNo-1],"*/");
     		$skipLineNum = $subLineNo - $lineNo;
     	}
    }
    else # the line which does not contain comments
    {
     	print CPP_COMMENT_FILE $eachline."\n";
    }
    $lineNo++;
  }
  close(CPP_COMMENT_FILE);
}

#-----------------------------------------------------------------------------------
# Description      : filter out the comment which is the form of "//...."
# Input Parameter  : the .c file 
# Output Parameter : the temp file which does not cotain the C comments
# Return Value     : $isIfMacroStatementFlag
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 02/03/2010
#-----------------------------------------------------------------------------------
sub filterDoubleSlashComment
{
	my($fileName,$noCommentFilePath)=@_;
	my $isIfMacroStatementFlag = 0;
	my $filterKeyword = "";
	my $lineNo = 1;
	#
	open(NO_COMMENT_FILE,">$noCommentFilePath")||die "can't open the file:$!";
	my @fileContent = Sprd_atest_file_process::getFileContent($fileName);
  # 
	foreach my $eachline(@fileContent)
	{
		chomp $eachline;
		# print "$lineNo=====$eachline\n";
		if($eachline !~ /^\s*\/\//)
		{
			$isIfMacroStatementFlag = getFlagOfIfMacro($isIfMacroStatementFlag,$eachline);
		}
  	# recognise the comment which is the form of "//...."
	  if($eachline=~/\/\// && $eachline!~ /"\s*\S*\s*\/\/\s*\S*\s*"/)
	  {
	  	if($eachline =~ /^\s*\/\//) # filter double slash comment.
	  	{
	  		print NO_COMMENT_FILE ""."\n";
	  	}
	  	else
	  	{ 
	  		my $doubleSlashStartPos = index($eachline,"//");
	  		my $slashStarStartPos = index($eachline,"/*");
	  		if($slashStarStartPos!=-1 && $slashStarStartPos < $doubleSlashStartPos)
	  		{
	  			print NO_COMMENT_FILE $eachline."\n";
	  		}
	  		else
	  		{
	  		  $filterKeyword = Sprd_atest_get_keyword::getFilterKeyword($eachline);
	  		  print NO_COMMENT_FILE substr($eachline,0,$doubleSlashStartPos).$filterKeyword."\n";		  		  	
	  		}
	  	}
	  }
	  else
	  {
	   	print NO_COMMENT_FILE $eachline."\n";
	  }
	  $lineNo++;
	}
	close(NO_COMMENT_FILE);
	# 
	return $isIfMacroStatementFlag;
}

#-----------------------------------------------------------------------------------
# Description      : filter out the if switch statement that contains 
#                    "#if 0" or "#ifdef WIN32"
# Input Parameter  : the .c/.h file
# Output Parameter : the temp file which does not cotain the C++ comments
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 23/03/2011
#                    11/05/2011 ( add "#ifdef WIN32" statement process )
#-----------------------------------------------------------------------------------
sub filterIfSwitchStatement
{
	my($fileName)=@_;
	my @fileredIfSwitchContent = ();
	my $lineNo = 1;
	my $isIfSwitchFlag = 0;
	my $endLineNo = 0;
	
	#
	my @fileContent = Sprd_atest_file_process::getFileContent($fileName);
	
	foreach my $currentLine(@fileContent)
	{
		chomp $currentLine;
		if($isIfSwitchFlag==0)
		{
      if( $currentLine =~ /^\s*\#\s*if\s+0\s*/ 
       || $currentLine =~ /^\s*\#\s*ifdef\s+WIN32\s*/i)
		  {
			  # print "$lineNo===>$currentLine\n";
			  $endLineNo = getEndLineNoOfIfMacroRange($lineNo,@fileContent);
			  # print "$endLineNo===>\n";
			  push(@fileredIfSwitchContent," ");
			  $isIfSwitchFlag = 1;
		  }
		  else
		  {
			  push(@fileredIfSwitchContent,$currentLine);
		  }			
		}
		else
		{
			push(@fileredIfSwitchContent," ");
			if($lineNo==$endLineNo)
			{
				$isIfSwitchFlag = 0;
			}
		}
		$lineNo++;
	}
	
	open(NO_COMMENT_FILE,">$fileName")||die "can't open the file:$!";
	foreach my $currentLine(@fileredIfSwitchContent)
	{
		print NO_COMMENT_FILE $currentLine."\n";
	}
	close(NO_COMMENT_FILE);
}


#-----------------------------------------------------------------------------------
# Description      : filter out the else switch statement that contains 
#                    "#if 1" or "#ifndef WIN32"
# Input Parameter  : the .c/.h file
# Output Parameter : the temp file which does not cotain the C++ comments
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 14/04/2011
#                    11/05/2011 ( add "#ifndef WIN32" statement process )
#-----------------------------------------------------------------------------------
sub filterElseSwitchStatement
{
	my($fileName)=@_;
	my @fileredElseSwitchContent = ();
	my $lineNo = 1;
	my $isElseSwitchFlag = 0;
	my $startLineNo = 0;
	my $endLineNo = 0;
	#
	my @fileContent = Sprd_atest_file_process::getFileContent($fileName);
	
	foreach my $currentLine(@fileContent)
	{
		chomp $currentLine;
	  # check the non if 1 statements.
    if($lineNo==$startLineNo)
		{
			$isElseSwitchFlag = 1;
		}
		elsif($lineNo==$endLineNo)
		{
			$isElseSwitchFlag = 0;
		}	
		# 
		if($isElseSwitchFlag==0)
		{
      if( $currentLine =~ /^\s*\#\s*if\s+1\s*/
       || $currentLine =~ /^\s*\#\s*ifndef\s+WIN32\s*/i)
		  { # get the start line of non if 1 statements.
			  $startLineNo = getEndLineNoOfIfMacroRange($lineNo,@fileContent);
			  if(@fileContent[$startLineNo-1] =~ /^\s*\#\s*endif\s*$/)
			  {
			  	$startLineNo = 0;
			  }
			  else
			  {
			    # get the end line of non if 1 statement. 
			    $endLineNo = getEndLineNoOfIfMacroRange($startLineNo,@fileContent);
			  }
		  }
		  push(@fileredElseSwitchContent,$currentLine);
		}
		else
		{
			push(@fileredElseSwitchContent," ");
		}
		$lineNo++;
	}
	
	open(NO_COMMENT_FILE,">$fileName")||die "can't open the file:$!";
	foreach my $currentLine(@fileredElseSwitchContent)
	{
		print NO_COMMENT_FILE $currentLine."\n";
	}
	close(NO_COMMENT_FILE);		
}

#-----------------------------------------------------------------------------------
# Description      : get the end line No. of the range which contains "#if 0"
# Input Parameter  : 1. current line No.
#                    2. the content of source file.
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 10/01/2011
#-----------------------------------------------------------------------------------
sub getEndLineNoOfIfMacroRange
{
	my($startLineNo,@currFileContent)=@_;
	my $endLineNo = $startLineNo;
  my $endSwitch = 1; # 
  # 
  while($endSwitch!=0)
  {
    $endLineNo++;
    chomp @currFileContent[$endLineNo-1];
    # 
    if(@currFileContent[$endLineNo-1] =~ /^\s*\#\s*if/) # matched if flag 
  	{
  	  $endSwitch++;
  	}
  	elsif(@currFileContent[$endLineNo-1] =~ /^\s*\#\s*endif/) # matched end flag
  	{
  	  $endSwitch--;
  	}
  	elsif(@currFileContent[$endLineNo-1] =~ /^\s*\#\s*el/ && $endSwitch == 1) # matched else/elif flag
  	{
  		last;
  	}
  }
  return $endLineNo;
}

#-----------------------------------------------------------------------------------
# Description      : get the number of the comment that format is "/*".
# Input Parameter  : the string. 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 17/02/2011
#-----------------------------------------------------------------------------------
sub getLeftCommentPerLine
{
	my($currentLine)=@_;
	my $commentNumber = 0;
	while($currentLine =~ /\/\*/)
	{
		my $frontCommentIndex = index($currentLine,"/*");
		my $frontContent = substr($currentLine,0,$frontCommentIndex);
		if($frontContent !~ /\/$/ || $frontContent =~ /\*\/$/)
		{
			$commentNumber++;
		}
		$currentLine = substr($currentLine,($frontCommentIndex+length("/*")));
	}
	return $commentNumber;
}

#-----------------------------------------------------------------------------------
# Description      : get the number of the comment that format is "*/".
# Input Parameter  : the string. 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 17/02/2011
#-----------------------------------------------------------------------------------
sub getRightCommentPerLine
{
	my($currentLine)=@_;
	my $commentNumber = 0;
	while($currentLine =~ /\*\//)
	{
		$commentNumber++;
		$currentLine = substr($currentLine,(index($currentLine,"*/")+length("*/")));
	}
	return $commentNumber;
}

#-----------------------------------------------------------------------------------
# Description      : get the flag of if macro statement.
# Input Parameter  : $currentLine 
# Output Parameter : NONE
# Return Value     : NONE 
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 14/04/2011
#-----------------------------------------------------------------------------------
sub getFlagOfIfMacro
{
	my($isIfMacroStatementFlag,$currentLine)=@_;
	
	if($currentLine =~ /^\s*\#if\s*0\s*/)
	{
		if($isIfMacroStatementFlag==0)
		{
			$isIfMacroStatementFlag = 1; # if 0
		}
		else
		{
			$isIfMacroStatementFlag = 3; # if 0 + if 1 + ifdef WIN32 + ifndef WIN32
		}
	}
	elsif($currentLine =~ /^\s*\#if\s*1\s*/)
	{
		if($isIfMacroStatementFlag==0)
		{
			$isIfMacroStatementFlag = 2; # if 1
		}
		else
		{
			$isIfMacroStatementFlag = 3; # if 0 + if 1 + ifdef WIN32 + ifndef WIN32
		}
	}
	# filter simulator code.
	if($win32_flag==1)
	{
	  if($currentLine =~ /^\s*\#ifdef\s*WIN32\s*/i)
	  {
		  if($isIfMacroStatementFlag==0)
		  {
			  $isIfMacroStatementFlag = 1; # ifdef WIN32
		  }
		  else
		  {
			  $isIfMacroStatementFlag = 3; # if 0 + if 1 + ifdef WIN32 + ifndef WIN32
		  }
	  }
	  elsif($currentLine =~ /^\s*\#ifndef\s*WIN32\s*/i)
	  {
		  if($isIfMacroStatementFlag==0)
		  {
			  $isIfMacroStatementFlag = 2; # ifndef WIN32
		  }
		  else
		  {
			  $isIfMacroStatementFlag = 3; # if 0 + if 1 + ifdef WIN32 + ifndef WIN32
		  }
	  }		
	}
	return $isIfMacroStatementFlag;	
}

1;

__END__
#-----------------------------------------------------------------------------------