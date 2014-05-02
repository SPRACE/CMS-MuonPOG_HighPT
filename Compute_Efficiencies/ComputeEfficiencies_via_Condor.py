import os
import subprocess
import string
import stat
import shutil
import sys

PYTHON_macro_file_Data = "CountEffHighMassDimuon_DATA.py"
PYTHON_macro_file_MC   = "CountEffHighMassDimuon_MC.py"

#root_files = [
#    ("Data", "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_Data_RunC__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_20_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_120_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_200_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_500_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_800_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1000_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1500_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_2000_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_tautau__Mass_tau__tau___gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_ttbar__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_tW__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_tbarW__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_WW__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_WZ__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_ZZ__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_W_Jets__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_Inclusive_QCD__Mass_mu1_mu2__gt_0_GeV.root"),
#    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_v9_Apr_02_2014/tnpZ_addBestPair_DY_Jets_ll__Mass_mu1_mu2__gt_0_GeV.root"),
#    ]

root_files = [
    ("Data", "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_Data_RunC__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_20_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_120_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_200_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_500_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_800_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1000_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1500_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_2000_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_tautau__Mass_tau__tau___gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_ttbar__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_tW__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_tbarW__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_WW__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_WZ__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_ZZ__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_W_Jets__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_Inclusive_QCD__Mass_mu1_mu2__gt_0_GeV.root"),
    ("MC"  , "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_BaselineSelection_WithBestPair_v9_Apr_02_2014/tnpZ_addBestPair_DY_Jets_ll__Mass_mu1_mu2__gt_0_GeV.root"),
    ]


#######################################################################################
#                                                                                     #
#   WARNING: From this point, there is no need to modify the lines bellow.            #
#                                                                                     #
#                        ------      ||                                               #
#                       /      \     ||                                               #
#                      / PLEASE \____||                                               #
#                      \  STOP  /____||                                               #
#                       \      /     ||                                               #
#                        ------      ||                                               #
#                                    ||                                               #
#######################################################################################


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
# This function creates a script to submit jobs to CONDOR
#######################################################################################
def create_script1( output, PYTHON_macro_file ):
    output.write("#!/bin/bash\n")
    output.write("\n")
    output.write("INPUT=$1\n")
    output.write("\n")
    output.write("if [[ -z $INPUT ]]; then\n")
    output.write("    echo -e \"\\n To run this script, run:\\n\"\n")
    output.write("    echo -e \"\t submit_TagAndProbe_to_condor.sh  <directory>\\n\"\n")
    output.write("    echo -e \"\\n where <directory> is an input name for a directory which will be\"\n")
    output.write("    echo -e \" create by this script and where the final results will be saved in.\\n\\n\"\n")
    output.write("    return 1\n")
    output.write("elif [[ -d $INPUT ]]; then\n")
    output.write("    echo -e \"\\n This directory name already exists\\n\"\n")
    output.write("    echo -e \" Please, provide a different name.\\n\"\n")
    output.write("    return 1\n")
    output.write("else\n")
    output.write("    mkdir  $INPUT\n")
    output.write("    cp  "+PYTHON_macro_file+"  $INPUT/.\n")
    output.write("    cp  -r /home/assantos/MuonPOG/TagAndProbe/Task_02/CMSSW_5_3_10/src/PhysicsTools  $INPUT/.\n")
    output.write("    cp  -r /home/assantos/MuonPOG/TagAndProbe/Task_02/CMSSW_5_3_10/src/MuonAnalysis  $INPUT/.\n")
    output.write("\n")
    output.write("    tar -zcf  $INPUT.tar.gz  $INPUT\n")
    output.write("fi\n")
    output.write("\n")
    output.write("condor_submit  TagAndProbe_for_condor.submit")


#######################################################################################
# This function creates a script with parameters submit jobs to CONDOR
#######################################################################################
def create_script2( output, folder ):
    output.write("Universe = grid\n")
    output.write("grid_resource = gt2 osg-ce.sprace.org.br/jobmanager-condor\n")
    output.write("Executable = TagAndProbe.sh\n")
    output.write("Requirements = Memory >= 4*1024\n")
    output.write("should_transfer_files = YES\n")
    output.write("when_to_transfer_output = ON_EXIT\n")
    output.write("transfer_input_files = "+folder+".tar.gz\n")
    output.write("transfer_output_files = Results.tar.gz\n")
    output.write("Error = err.$(Process)\n")
    output.write("Input = "+folder+".tar.gz\n")
    output.write("Output = out.$(Process)\n")
    output.write("Log = foo.log\n")
    output.write("Queue 1\n")


#######################################################################################
# This function creates a script which runs inside CONDOR's nodes
#######################################################################################
def create_script3( output, folder, PYTHON_macro_file ):
    output.write("#!/bin/bash\n")
    output.write("\n")
    output.write("echo \"Host name: `hostname`\"\n")
    output.write("echo \"==========================\"\n")
    output.write("pwd\n")
    output.write("ORIGIN=$PWD\n")
    output.write("ls\n")
    output.write("echo \"==========================\"\n")
    output.write("echo \"Untar file\"\n")
    output.write("tar -zxf "+folder+".tar.gz\n")
    output.write("\n")
    output.write("echo \"==========================\"\n")
    output.write("echo \"Executing application\"\n")
    output.write("\n")
    output.write("cd "+folder+"\n")
    output.write("ls\n")
    output.write("\n")
    output.write("echo \"==========================\"\n")
    output.write("echo \"Preparing a working area\"\n")
    output.write("\n")
    output.write("export SCRAM_ARCH=slc5_amd64_gcc462\n")
    output.write("export OSG_APP=/osg/app\n")
    output.write("source $OSG_APP/cmssoft/cms/cmsset_default.sh\n")
    output.write("export MYCMSSW=CMSSW_5_3_10\n")
    output.write("\n")
    output.write("scramv1 project CMSSW ${MYCMSSW}\n")
    output.write("ls -lh\n")
    output.write("\n")
    output.write("cp  "+PYTHON_macro_file+"  ${MYCMSSW}/src/.\n")
    output.write("cp  -r  PhysicsTools  ${MYCMSSW}/src/.\n")
    output.write("cp  -r  MuonAnalysis  ${MYCMSSW}/src/.\n")
    output.write("\n")
    output.write("sed -i 's/<use name=\\\"self\\\"\\/>/<use name=\\\"self\\\"\\/>\\n\\n<flags CPPDEFINES=\\\"__USE_XOPEN2K8\\\"\\/>/g' ${MYCMSSW}/config/BuildFile.xml\n")
    output.write("cd  ${MYCMSSW}/src/\n")
    output.write("eval `scramv1 runtime -sh`\n")
    output.write("scramv1 b\n")
    output.write("cp  "+PYTHON_macro_file+"  MuonAnalysis/TagAndProbe/test/zmumu/.\n")
    output.write("cd  MuonAnalysis/TagAndProbe/test/zmumu/.\n")
    output.write("pwd\n")
    output.write("\n")
    output.write("echo \"==========================\"\n")
    output.write("echo \"Running macro file\"\n")
    output.write("cmsRun  "+PYTHON_macro_file+"\n")
    output.write("ls -lsh\n")
    output.write("\n")
    output.write("mkdir  Results\n")
    output.write("FILE=$( ls TnP_Z_CountHighMassDiMuID_*.root )\n")
    output.write("cp $FILE  Results/.\n")
    output.write("ls Results/\n")
    output.write("\n")
    output.write("tar -zcvf  Results.tar.gz  Results\n")
    output.write("cp  Results.tar.gz  $ORIGIN\n")
    output.write("\n")
    output.write("cd  $ORIGIN\n")
    output.write("rm -f  "+folder+".tar.gz\n")
    output.write("rm -rf "+folder+"\n")
    output.write("\n")
    output.write("ls\n")


#######################################################################################
# This function create a folder name correspondent to the ROOT file
#######################################################################################
def create_folder_name ( root_path ):
    # Transform string in list
    ll = list( root_path )

    # Loop over the list of characters
    for i in range(len(ll) - 1):
        if ll[i+1] == "/":
            # Loop over the list of characters until the "slash" (/) position
            for j in range(0, i+2):
                # Delete all characters until the "slash"
                ll[j] = ""
    # Transform list of characters back into a string
    root_path = "".join(ll)
    root_path = root_path.replace(".root", "")

    return root_path


#######################################################################################
# This function writes the ROOT file path into the PYTHON macro file
#######################################################################################
def write_root_file ( dataset_path, macro_file_name ):

    tmp_file_name = "tmp_macro_file.txt"
    tmp_file      = open( tmp_file_name, "w" )

    macro_file    = open( macro_file_name, "r" )

    # Loop over all lines of the macro file, copy them to a "tm" file.
    # The only different line is that one correspondent to the ROOT file chosen by the USER.
    for line in macro_file:
        if "===>>>" in line:
            newline = "        '"+dataset_path+"', #===>>> Modify this line\n"
            tmp_file.write(newline)
        else:
            tmp_file.write(line)

    tmp_file.close()
    macro_file.close()

    # Copying contents of the "tmp" file to the macro file and removing "tmp" in the end.
    shutil.copy( tmp_file.name, macro_file.name )
    os.remove( tmp_file.name )


#######################################################################################
# Defining the main function, looping over list of ROOT files:
#   - each ROOT file will be read into a PYTHON macro file;
#   - each ROOT file corresponding to a job submited via CONDOR.
#
# Here an output folder is created to where all results will arrive from CONDOR.
#
# In CONDOR, the macro file will run is this way:
#          python  CountEffHighMassDimuon_DATA.py
#          python  CountEffHighMassDimuon_MC.py
#
#######################################################################################
def main():

    # Code asks USER for a directory name
    print "===============================================================\n"
    print "\t --> You are going to submit jobs via CONDOR, but..."
    print "\t --> ... if you have not already setup the GRID, then please type"
    print "\t -->     voms-proxy-init --voms cms"
    print "\t --> before proceeding.\n\n"
    print "\t --> A new directory will be created here."
    directory = raw_input( "\t --> Please, provide the name of the directory: " )
    from_bash = os.path.isdir( directory )
    if from_bash == True:
        print "\n-->\t ERROR: Directory <", directory, "> already exists. Please, provide other name! \n"
        exit()
    else:
        os.makedirs( directory )
        print "\n-->\t Directory <", directory, "> has been successfully created.\n"

    # Macro files are copied to the new directory
    shutil.copy( PYTHON_macro_file_Data, directory+"/"+PYTHON_macro_file_Data)
    shutil.copy( PYTHON_macro_file_MC, directory+"/"+PYTHON_macro_file_MC)

    # Moving to the new directory
    os.chdir( directory )

    # Looping over all ROOT files
    for sample, dataset_path in root_files:

        # Creating a new directory w.r.t. each ROOT file.
        output_folder = create_folder_name( dataset_path )
        print "New ROOT file folder:", output_folder
        os.makedirs(output_folder)

        # Check if it is Data or MC sample
        if sample == "Data":
            PYTHON_macro_file = PYTHON_macro_file_Data
        elif sample == "MC":
            PYTHON_macro_file = PYTHON_macro_file_MC
        else:
            print "\n-->\t ERROR: Make sure the list of root files has been properly filled in."
            print "\n-->\t        There is only two possibilities for the first entry:"
            print "\n-->\t        - Data"
            print "\n-->\t        - MC"
            exit()

        # Write the ROOT file path into the PYTHON macro and
        # copy the later the new directory
        write_root_file( dataset_path, PYTHON_macro_file )
        shutil.copy( PYTHON_macro_file, output_folder+"/"+PYTHON_macro_file)

        # Moving to the new ROOT file directory
        os.chdir( output_folder )

        outputFile = open("submit_TagAndProbe_to_condor.sh", "w")
        # This function creates a script to submit jobs to CONDOR
        create_script1( outputFile, PYTHON_macro_file )
        bash_via_python( "chmod 755 "+outputFile.name )
        outputFile.close()

        outputFile = open("TagAndProbe_for_condor.submit", "w")
        # This function creates a script with parameters submit jobs to CONDOR
        create_script2( outputFile, output_folder )
        bash_via_python( "chmod 755 "+outputFile.name )
        outputFile.close()

        outputFile = open("TagAndProbe.sh", "w")
        # This function creates a script which runs inside CONDOR's nodes
        create_script3( outputFile, output_folder, PYTHON_macro_file )
        bash_via_python( "chmod 755 "+outputFile.name )
        outputFile.close()

        # Submiting job to CONDOR (each job corresponds to each ROOT file)
        bash_command = "./submit_TagAndProbe_to_condor.sh  "+output_folder
        bash_via_python( bash_command )

        # Going back one folder
        os.chdir("..")

    print "===============================================================\n"
    print "\n\t These folders have been created. Each folder"
    print "\t corresponts to one submited job:\n\n"
    os.chdir("..")
    bash_command = "ls -d1 "+directory+"/*/"
    bash_via_python( bash_command )
    print "\n\t\t Done!!!   \\o/\n\n"
    print "===============================================================\n"


#######################################################################################
# Here we call the main funtion
#######################################################################################
main()
