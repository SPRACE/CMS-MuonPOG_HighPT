import os
import subprocess
import glob
import fnmatch
import tarfile
import zipfile
import shutil
import sys

#######################################################################################
# In this macro, User is asked for providing a folder containing sub-folders
# with "Results.tar.gz" files.
#
# The code will check if the folder and *.tar.gz files exist, creating a list of them.
# The files are extracted from the "Results.tar.gz" files and copied into the current
# directory.
#
#######################################################################################


#######################################################################################
# This recipe can be used to extract any zip or tar.gz/tar.bz2 file
#######################################################################################
def extract_file(path, to_directory='.'):
    if path.endswith('.zip'):
        opener, mode = zipfile.ZipFile, 'r'
    elif path.endswith('.tar.gz') or path.endswith('.tgz'):
        opener, mode = tarfile.open, 'r:gz'
    elif path.endswith('.tar.bz2') or path.endswith('.tbz'):
        opener, mode = tarfile.open, 'r:bz2'
    else: 
        raise ValueError, "Could not extract `%s` as no appropriate extractor is found" % path
    
    cwd = os.getcwd()
    os.chdir(to_directory)
    
    try:
        file = opener(path, mode)
        try: file.extractall()
        finally: file.close()
    finally:
        os.chdir(cwd)


#######################################################################################
# This function enable to run bash commands via Python and read its results pop up
# on screen in real time.
#######################################################################################
def bash_via_python(command_line):
    p2 = subprocess.Popen(command_line, shell=True, stderr=subprocess.PIPE)
    while True:
        out = p2.stderr.read(1)
        if out == '' and p2.poll() != None:
            break
        if out != '':
            sys.stdout.write(out)
            sys.stdout.flush()


#######################################################################################
# In this main function, User is asked for providing a folder containing sub-folders
# with Results.tar.gz files.
#
# The code will check if the folder and *.tar.gz files exist, creating a list of
# later. The files are extracted from the Results.tar.gz files and copied into the
# current directory.
#
#######################################################################################
def main():

    # Code asks USER for a folder name
    print "===============================================================\n"
    print "\t --> Where are your Results.tar.gz files?"
    print "\t --> If they are located under a sub-folder like"
    print "\t -->    folder/sub-folder1/Results.tar.gz"
    print "\t -->    folder/sub-folder2/Results.tar.gz"
    print "\t -->    ..."
    print "\t --> just type:"
    print "\t -->    folder"
    directory = raw_input( "\t --> Please, provide the path to them: " )
    from_bash = os.path.isdir( directory )

    # Checking if the folder exists. If "yes", it is created a list of "Results.tar.gz"
    # and also a list of their paths.
    if from_bash == True:
        list_of_files = []
        list_of_folders = []
        for root, dirnames, filenames in os.walk( directory ):
            # Creating a list of "Results.tar.gz" files
            for filename in fnmatch.filter( filenames, 'Results.tar.gz' ):
                list_of_files.append(os.path.join( root, filename ))

            # Creating a list of paths to the "Results.tar.gz" files
            for dirname in fnmatch.filter( dirnames, 'tnpZ_addBestPair_*' ):
                list_of_folders.append( dirname )
        if len(list_of_files) == 0:
            print "\n\t -->\t ERROR:  There is no *.tar.gz file inside <", directory, ">."
            exit()
    else:
        print "\n\t -->\t ERROR:  <", directory, "> does not exist. Please, provide other path! \n"
        exit()


    print "\n\t --> This is the list of *.tar.gz files found under <", directory, ">:"
    # Loop over the list of tar.gz files, extracting their contents
    for i in range(len(list_of_files)):
        print "\t   -", list_of_files[i]
        extract_file( list_of_files[i] ) # Extraction of contents

        # If new ROOT files exist, rename and copy them to the current directory
        from_bash = os.path.isfile( "Results/TnP_Z_CountHighMassDiMuID_MC.root" )
        if from_bash == True:
            new_root_file = "TnP_Z_CountHighMassDiMuID_MC_"+(list_of_folders[i].replace("tnpZ_addBestPair_", "")).replace("__Mass_mu1_mu2__gt_0_GeV", "")+".root"
            shutil.move( "Results/TnP_Z_CountHighMassDiMuID_MC.root", new_root_file )
        else:
            from_bash = os.path.isfile( "Results/TnP_Z_CountHighMassDiMuID_DATA.root" )
            if from_bash == True:
                new_root_file = "TnP_Z_CountHighMassDiMuID_"+(list_of_folders[i].replace("tnpZ_addBestPair_", "")).replace("__Mass_mu1_mu2__gt_0_GeV", "")+".root"
                shutil.move( "Results/TnP_Z_CountHighMassDiMuID_DATA.root", new_root_file )
            else:
                continue

    print "\t --> There is a total of", len(list_of_files), "tar.gz files."


    # Remove "Results" file
    shutil.rmtree( "Results" )

    print "\n\t\t Done!!!   \\o/\n\n"
    print "\n\t\t Please, check if the files were properly created here."
    print "\t\t A number of ROOT files should be here"
    print "\t\t corresponding to each *.tar.gz file listed above.\n\n"


#######################################################################################
# Here we call the main funtion
#######################################################################################
main()
