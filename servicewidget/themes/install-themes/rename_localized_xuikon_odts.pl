#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: 
#
#!/usr/bin/perl
use File::Copy;
use File::Find;

print("Start to move localized xuikon odts to folders named by language number\n");

# go through all directories and subdirectories
find(\&MoveLocalized, '.');

sub MoveLocalized
{
  # if we are in hsps folder move localized xuikon odts
  if ($File::Find::name =~ /\/hsps\//g)
  {
    #print("Processing folder $File::Find::dir\n");
    $File::Find::prune = 1;
    # move localized xuikon odts to folders named by language index
    while(<*.o*>)
    {
      # Find position of suffix (search for ".o")
      $suffix = $_;
      $suffix =~ /\.o/g;
    
      # name of localized dir is last four characters of file
      $dirname = substr($_, pos($suffix), 4);
      # don't move engineering english version
      if ($dirname != "0000")
      {
        print("\nold file = $File::Find::dir/$_\n");
        # remove zeros from beginning
        $dirname += 0;
        # create folder
        mkdir($dirname);
        # move file to folder and rename it
        $newlocation = $dirname ."/" .$_;
        move($_, $newlocation);
        $oldname = $newlocation;
        $newlocation =~ s/\.o(.*)$/\.o0000/;
        print("new file = $File::Find::dir/$newlocation\n");
        rename $oldname, $newlocation;
      }
    }
  }
}


exit;