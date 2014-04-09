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
            for dirname in fnmatch.filter( dirnames, 'DataSet_*' ):
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
        from_bash = os.path.isfile( "Results/tnpZ_addBestPair.root" )
        if from_bash == True:
            new_root_file = "tnpZ_addBestPair_"+list_of_folders[i].replace("DataSet_", "")+".root"
            shutil.move( "Results/tnpZ_addBestPair.root", new_root_file )

        # For the first content of the list, just copy the *.txt file to the current directory.
        # Otherwise, append lines to first one.
        if i == 0:
            shutil.copy( "Results/output_nEvents_after_selection.txt", "." )
#            sed -i '1s/^/results = [\n/' directory
        else:
            bash_command = "cat Results/output_nEvents_after_selection.txt"
            bash_command += " >> output_nEvents_after_selection.txt"
            bash_via_python( bash_command )
    print "\t --> There is a total of", len(list_of_files), "tar.gz files."


    # Making sure to get the file in python format
    #
    # This should go in the first line
    bash_command = "sed -i \'1s/^/"+directory.replace("/", "_")+" = [\\n/\' output_nEvents_after_selection.txt"
    bash_via_python( bash_command )
    # And this should go in the last line
    bash_command = "echo \"        ]\n\" | cat >> output_nEvents_after_selection.txt"
    bash_via_python( bash_command )
    # Rename the *.txt file to avoid loosing its information in the next time when running this macro
    shutil.move( "output_nEvents_after_selection.txt", "output_"+directory.replace("/", "_")+".txt" )


    # Remove "Results" file
    shutil.rmtree( "Results" )

    print "\n\t\t Done!!!   \\o/\n\n"
    print "\n\t\t Please, check if the files were properly created here."
    print "\t\t At least, a file called \"output_nEvents_after_selection.txt\""
    print "\t\t should have been created. If User has asked to create new ROOT"
    print "\t\t files via CONDOR, then a number of ROOT files should be here"
    print "\t\t corresponding to each *.tar.gz file listed above.\n\n"


#######################################################################################
# Here we call the main funtion
#######################################################################################
main()
