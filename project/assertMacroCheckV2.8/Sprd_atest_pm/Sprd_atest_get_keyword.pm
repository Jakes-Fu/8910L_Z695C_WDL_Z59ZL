#!perl\win
# BEGIN{push @INC,'./Sprd_atest_pm'};

package Sprd_atest_get_keyword;

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

#-----------------------------------------------------------------------------------
# Constant Definition
#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Global Variable Definition
#-----------------------------------------------------------------------------------
my $assertFilterKeyword = "assert verified";
my $assertDoubtfulKeyword = "assert to do";

#-----------------------------------------------------------------------------------
# Sub Function
#-----------------------------------------------------------------------------------

#-----------------------------------------------------------------------------------
# Description      : check whether current line contains filter keyword or not 
#                    and get that keyword.
# Input Parameter  : $eachLine  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 31/03/2011
#-----------------------------------------------------------------------------------
sub getFilterKeyword
{
	my($currentLine)=@_;
	my $filterKeyword = "";
	
  if($currentLine =~ /$assertFilterKeyword/i)
  {
  	$filterKeyword = " /*$assertFilterKeyword*/";
  }
  elsif($currentLine =~ /$assertDoubtfulKeyword/i)
  {
  	$filterKeyword = " /*$assertDoubtfulKeyword*/";
  }
  else
  {
  	# ADD NEW KEYWORD HERE
  }
	return $filterKeyword;
}

#-----------------------------------------------------------------------------------
# Description      : get value of assert verified keyword.
# Input Parameter  : NONE  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 31/03/2011
#-----------------------------------------------------------------------------------
sub getAssertVerifiedValue
{
	return $assertFilterKeyword;
}

#-----------------------------------------------------------------------------------
# Description      : get value of assert todo keyword.
# Input Parameter  : NONE  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 31/03/2011
#-----------------------------------------------------------------------------------
sub getAssertToDoValue
{
	return $assertDoubtfulKeyword;
}

#-----------------------------------------------------------------------------------
# Description      : get branch project name list.
# Input Parameter  : $eachLine  
# Output Parameter : NONE
# Return Value     : NONE
# Note             : 
# Editor           : Mingxuan.Zhu
# Modified Date    : 31/03/2011
#-----------------------------------------------------------------------------------
sub getBranchProjList
{
	my($branchFlag)=@_;
	my $branchProjList = "";
	# 
	if($branchFlag =~ /^09a$/i) # 09a project
	{
		$branchProjList = "SC6600L";
	}
	elsif($branchFlag =~ /^68h$/i) # 68h project
	{
		$branchProjList = "SC6800H";
	}
	elsif($branchFlag =~ /^10a$/i) # 10a project
	{
		$branchProjList = "SC6800H SC8800G PLATFORM_6800_OR_8800H";
	}
	elsif($branchFlag =~ /^mt$/i) # mt project
	{
		$branchProjList = "SC6530 PLATFORM_SC6530";
	}	
	else
	{
		# ADD NEW BRANCH CODE HERE
	}
	return $branchProjList;
}

1;

__END__
#-----------------------------------------------------------------------------------