import os
import subprocess
import string
import stat
import shutil
import sys


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
def main():

    # Code asks USER for a directory name in STORAGE
    print "===============================================================\n"
    print "\t --> You are going to check how many files are repeated in results from CRAB."
    print "\t --> But make sure you have setup the GRID via"
    print "\t -->     voms-proxy-init --voms cms"
    print "\t --> before proceeding.\n\n"
    print "\t --> You need to provide the directory path where are the files. For example:"
    print "\t --> if your ROOT files are inside the paths like"
    print "\t -->     /pnfs/sprace.org.br/data/cms/store/user/MrSmith/my/directory/Data/"
    print "\t -->     /pnfs/sprace.org.br/data/cms/store/user/MrSmith/my/directory/ttbar/"
    print "\t -->     /pnfs/sprace.org.br/data/cms/store/user/MrSmith/my/directory/W+Jets/"
    print "\t --> etc..."
    print "\t --> just type"
    print "\t -->     /pnfs/sprace.org.br/data/cms/store/user/MrSmith/my/directory/"
    directory = raw_input( "\t --> Please, provide the name of the directory: " )
    command = "source /home/assantos/scripts/list_storage.sh "+directory
    command = command+" | cut -d' ' -f8- | head -n -1 | tail -n +2 > list_of_files.txt"

    bash_via_python(command)
    empty = os.path.getsize("list_of_files.txt") > 0
    if empty == False:
        print "\n-->\t ERROR: Directory <", directory, "> does not exist. Please, provide the correct path! \n"
        exit()

    file_list = open( "list_of_files.txt", "r" )
    for line in file_list:
        print "_______________________________________________________________________"
        print "\t --> ", line
        command = "source /home/assantos/scripts/list_storage.sh "+line.replace("\n", "")
        command = command+" > lll.txt"
        bash_via_python(command)

        print "Number of ROOT files for this sample:",
        print (open( "lll.txt", "r" ).read().count("\n") - 2)

        command = "cat lll.txt | cut -d'/' -f12- | cut -d'_' -f3- | rev | cut -d'_' -f3- |"
        command = command+" rev | sort | tail -n +3 | uniq -d"
        bash_via_python(command)

        command = "cat lll.txt | cut -d'/' -f12- | cut -d'_' -f3- | rev | cut -d'_' -f3- |"
        command = command+" rev | sort | tail -n +3 | uniq -d > root_files.txt"
        bash_via_python(command)

        repeated_files = open( "root_files.txt", "r" )
        empty = os.path.getsize( repeated_files.name ) > 0
        if empty == False:
            print "There is no repeated files for this sample."
        else:
            print "There are", open( repeated_files.name, "r" ).read().count("\n"), "repeated files."
            for file_id in repeated_files:
                command = "grep _"+file_id.replace("\n", "")+"_ lll.txt"
                print "--->>>", file_id
                bash_via_python(command)
        repeated_files.close()
    file_list.close()


#######################################################################################
# Here we call the main funtion
#######################################################################################
main()
