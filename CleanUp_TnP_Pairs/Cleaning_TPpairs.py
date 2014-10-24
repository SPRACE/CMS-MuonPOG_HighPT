#######################################################################################
# VERSION v5, June 14 2014
# _____________________________________________________________________________________
#
# Versions:
#   - v5:
#        * comes from version v4;
#        * includes vertex multiplity weight into new Trees.
#   - v4:
#        * comes from version v3;
#        * adding variables to handle with new ROOT macro file, which requires that good
#          T&P pairs are those ones with the Chi2/NDF closest to 1;
#        * no longer analyze if Tag becomes Probe and vice-versa, allowing have more than
#          one pair per event;
#        * now User can pass input paramenters via command line to choose if wanting to
#          label T&P pairs concering their Chi2/NDF, as well as if wanting to select events
#          with respect to User's pre-defined cuts.
#   - v3:
#        * comes from version v1 and has a list of 74 ROOT files from splitted DATA-RunC;
#        * allows to use variable "cosmics" in "TP_pair_selections" to remove cosmic-ray muons.
#   - v2: comes from version v2 and has a list of  8 ROOT files from splitted DATA-RunC.
# _____________________________________________________________________________________
#   - v1:
#
# This macro loops over the list of ROOT files defined by the USER:
#   - each ROOT file will be read into a ROOT macro file;
#   - each ROOT file corresponding to a job submited via CONDOR.
#
# The code will ask USER for providing an output folder. This folder is created by the
# the code and will gather all results which arrive from CONDOR.
#
# In CONDOR, the ROOT macro file will run in this way:
#          root -b -l -q 'addBestPair_RunEvent_v10.xx("dataset_definition", "dataset_path", debug, create_new_root, choose_pair, create_selection_variable)'
#
# This ROOT macro takes into account the selections of events defined here by the USER.
# If chosen by the USER, a new ROOT file is created containing a new variable called
# "pair_BestPair" (a variable of zeros or 1s depending on the quality of the Tag and Probe pairs).
# Otherwise, only a "*.txt" file will be produced. This files contains a list of entries of
# different types in a Python format:
#        (number of pairs with pT(Tag)  <  pT(Probe),
#         number of pairs with pT(Tag)  >  pT(Probe),
#         number of pairs with pT(Tag) ==  pT(Probe),
#         number of pairs in any strange case,
#         number of pairs failing the USER's selection,
#         "data set name w.r.t. each input ROOT file")
# 
# All of the inputs mentioned above have to be passed bellow by the USER.
#
#                       !!!!!  Good Luck  !!!!!
#
#######################################################################################
import os
import subprocess
import string
import stat
import shutil
import sys

#######################################################################################
#                             Fill inputs accordingly
# Instructions:
# - debug = 1 or 0.
#         - if you want just a debug mode (1), the code will read only 100 events of each ROOT file
#         - if you want to run over the total number of entries, choose (0).
#
# - new_folder = 1 or 0.
#         - choose (1) if wanting to produce a new ROOT file
#         - choose (0) otherwise
#
# - choose_pair = 1 or 0
#         - choose (1) if wanting to mark pairs as good or bad comparing their Chi2/NDF
#         - choose (0) otherwise
# - select_events = 1 or 0
#         - choose (1) if wanting to select events based on User's predefined cuts
#         - choose (0) otherwise
#
# - ROOT_macro_file = is the name of the ROOT macro file which will run in CONDOR and
#                     should be in the same directory of this Python macro file.
#
# - TP_pair_selections = is the way the ROOT macro file will select the pairs of Tag
#                        and Probe muons. There a few variables to use/combine:
#                        - run
#                        - event
#                        - lumi
#                        - NewTuneP_pt
#                        - tag_NewTuneP_pt
#                        - NewTuneP_eta
#                        - tag_NewTuneP_eta
#                        - NewTuneP_phi
#                        - tag_NewTuneP_phi
#                        - charge
#                        - tag_charge
#                        - tag_IsoMu24
#                        - pair_dz
#                        - pair_newTuneP_mass
#                        - pair_DimuonVtxFitNormQui2
#                        - tag_combRelIsoPF04dBeta
#                        - tag_innertrackPtRelError
#                        - tkIso
#                        - pair_collinearity1
#                        - tag_MET
#                        - tag_METphi
#                        Example: "((charge*tag_charge < 0) && (NewTuneP_pt > 15.))"
#                        In case of needing more variables, add them accordingly the
#                        the ROOT_macro_file.
#######################################################################################
debug              = 1
new_folder         = 1
choose_pair        = 0
select_events      = 1
ROOT_macro_file    = "addBestPair_RunEvent_v11.cxx"
TP_pair_selections = "(charge != tag_charge) && (NewTuneP_eta > -2.4) && (NewTuneP_eta < 2.4) && (tag_NewTuneP_eta > -2.1) && (tag_NewTuneP_eta < 2.1) && (tag_IsoMu24 == 1) && (pair_newTuneP_mass >= 60.) && (pair_DimuonVtxFitNormQui2 <= 1.0) && (tag_NewTuneP_pt > 45.) && (NewTuneP_pt > 45.) && (pair_dz > -0.05) && (pair_dz < 0.05) && (tag_combRelIsoPF04dBeta < 0.12) && (pair_collinearity1 < (TMath::Pi() - 0.02)) && ((tkIso/NewTuneP_pt) < 0.1) && (tag_MET10 < 50.) && ((TMath::Abs(tag_NewTuneP_pt - NewTuneP_pt) / (tag_NewTuneP_pt + NewTuneP_pt)) < 0.4)"


#######################################################################################
# The list of input root files must contain:
#      ("definitions", "mass window", number of events, "path to the input root files")
#
# It is not mandatory to fill the correct number of events. So just write "0".
# The paths to the input ROOT files should be those ones in "DCASH"
#######################################################################################
root_files = [
    ("Data-RunC",     "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 73100236,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_Data_RunC__Mass_mu1_mu2__gt_0_GeV.root"),
    ("DY->mumu",      "m(mu1, mu2) > 20 GeV", "(pair_newTuneP_mass >= 20.) && (pair_newTuneP_mass < 120.)", 4817072,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_20_GeV.root"),
### [[[ It's version 2 ]]]]--->    ("DY->mumu",      "m(mu1, mu2) > 20 GeV",   71411027, ""),
    ("DY->mumu",      "m(mu1, mu2) > 120 GeV", "(pair_newTuneP_mass >= 120.) && (pair_newTuneP_mass < 200.)", 265550,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_120_GeV.root"),
    ("DY->mumu",      "m(mu1, mu2) > 200 GeV", "(pair_newTuneP_mass >= 200.) && (pair_newTuneP_mass < 500.)", 309663,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_200_GeV.root"),
    ("DY->mumu",      "m(mu1, mu2) > 500 GeV", "(pair_newTuneP_mass >= 500.) && (pair_newTuneP_mass < 800.)", 379473,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_500_GeV.root"),
    ("DY->mumu",      "m(mu1, mu2) > 800 GeV", "(pair_newTuneP_mass >= 800.) && (pair_newTuneP_mass < 1000.)", 408182,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_800_GeV.root"),
    ("DY->mumu",      "m(mu1, mu2) > 1000 GeV", "(pair_newTuneP_mass >= 1000.) && (pair_newTuneP_mass < 1500.)", 419382,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1000_GeV.root"),
    ("DY->mumu",      "m(mu1, mu2) > 1500 GeV", "(pair_newTuneP_mass >= 1500.) && (pair_newTuneP_mass < 2000.)", 436012,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_1500_GeV.root"),
    ("DY->mumu",      "m(mu1, mu2) > 2000 GeV", "(pair_newTuneP_mass >= 2000.) && (pair_newTuneP_mass < 3000.)", 441181,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_2000_GeV.root"),
    ("DY->tautau",    "m(tau+, tau-) > 0 GeV", "(pair_newTuneP_mass > 0.)", 222238,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_tautau__Mass_tau__tau___gt_0_GeV.root"),
    ("ttbar",         "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 11837860,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_ttbar__Mass_mu1_mu2__gt_0_GeV.root"),
    ("tW",            "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 234414,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_tW__Mass_mu1_mu2__gt_0_GeV.root"),
    ("tbarW",         "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 227851,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_tbarW__Mass_mu1_mu2__gt_0_GeV.root"),
    ("WW",            "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 2643186,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_WW__Mass_mu1_mu2__gt_0_GeV.root"),
    ("WZ",            "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 2511807,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_WZ__Mass_mu1_mu2__gt_0_GeV.root"),
    ("ZZ",            "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 2592869,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_ZZ__Mass_mu1_mu2__gt_0_GeV.root"),
    ("W+Jets",        "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 4117043,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_W_Jets__Mass_mu1_mu2__gt_0_GeV.root"),
    ("Inclusive-QCD", "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 5213103,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_Inclusive_QCD__Mass_mu1_mu2__gt_0_GeV.root"),
    ("DY+Jets->ll",   "m(mu1, mu2) > 0 GeV", "(pair_newTuneP_mass > 0.)", 23611226,
     "dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/BaselineSelection_v10_Jun_08_2014/tnpZ_addBestPair_DY_Jets_ll__Mass_mu1_mu2__gt_0_GeV.root"),
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
# This function changes characters of a string
#######################################################################################
def changing_characters( output ):
    output = string.replace(output, " ", "_")
    output = string.replace(output, "->", "_")
    output = string.replace(output, ">", "gt")
    output = string.replace(output, "<", "lt")
    output = string.replace(output, "m(", "_Mass_")
    output = string.replace(output, ")", "_")
    output = string.replace(output, "-", "_")
    output = string.replace(output, "+", "_")
    output = string.replace(output, ",", "")
    return output


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
def create_script1( output ):
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
    output.write("    cp  "+ROOT_macro_file+"  $INPUT/.\n")
    output.write("\n")
    output.write("    tar -zcvf  $INPUT.tar.gz  $INPUT\n")
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
    output.write("Requirements = Memory >= 16*1024\n")
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
def create_script3( dataset_type, selection_of_pairs, data_path, output, folder, definition, path, debug, new_folder ):
    output.write("#!/bin/bash\n")
    output.write("\n")
    output.write("echo \"Host name: `hostname`\"\n")
    output.write("echo \"==========================\"\n")
    output.write("pwd\n")
    output.write("ls\n")
    output.write("echo \"==========================\"\n")
    output.write("echo \"Untar file\"\n")
    output.write("tar -zxvf "+folder+".tar.gz\n")
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
    output.write("export SCRAM_ARCH=slc6_amd64_gcc472\n")
    output.write("export OSG_APP=/osg/app\n")
    output.write("source $OSG_APP/cmssoft/cms/cmsset_default.sh\n")
    output.write("export MYCMSSW=CMSSW_5_3_12\n")
    output.write("\n")
    output.write("scramv1 project CMSSW ${MYCMSSW}\n")
    output.write("ls -lh\n")
    output.write("\n")
    output.write("cp  "+ROOT_macro_file+"  ${MYCMSSW}/src/.\n")
    output.write("cd  ${MYCMSSW}/src/\n")
    output.write("eval `scramv1 runtime -sh`\n")
    output.write("pwd\n")
    output.write("\n")
    output.write("echo \"==========================\"\n")
    output.write("echo \"Running macro file\"\n")
    selecting_pairs = TP_pair_selections+" && "+selection_of_pairs
    if "Data" in dataset_type:
        output.write("root -b -l -q '"+ROOT_macro_file+"(\"Data\", \""+definition+"\", \""+path+"\", "+"\""+data_path+"\", "+str(debug)+", "+str(new_folder)+", "+str(choose_pair)+", "+str(select_events)+", \""+selecting_pairs+"\")'\n")
    else:
        output.write("root -b -l -q '"+ROOT_macro_file+"(\"MC\", \""+definition+"\", \""+path+"\", "+"\""+data_path+"\", "+str(debug)+", "+str(new_folder)+", "+str(choose_pair)+", "+str(select_events)+", \""+selecting_pairs+"\")'\n")
    output.write("ls -lsh\n")
    output.write("\n")
    output.write("mkdir  Results\n")
    output.write("FILE=$( grep \"string output_file_name\" "+ROOT_macro_file+" | cut -d'\"' -f2- | cut -d'\"' -f1 )\n")
    output.write("cp $FILE  Results/.\n")
    output.write("NEWFILE=$( grep \"string new_file_name\" "+ROOT_macro_file+" | cut -d'\"' -f2- | cut -d'\"' -f1 )\n")
    output.write("if [[ -n $NEWFILE ]]; then\n")
    output.write("    cp $NEWFILE Results/.\n")
    output.write("    echo \"File $NEWFILE has been copyied to Results folder.\"\n")
    output.write("fi\n")
    output.write("ls Results/\n")
    output.write("\n")
    output.write("tar -zcvf  Results.tar.gz  Results\n")
    output.write("cp  Results.tar.gz  ../../../.\n")
    output.write("\n")
    output.write("cd ../../../.\n")
    output.write("rm -f  "+folder+".tar.gz\n")
    output.write("rm -rf "+folder+"\n")
    output.write("\n")
    output.write("ls\n")


#######################################################################################
# This function apply selections to the Tag and Probe pairs in the macro file
#######################################################################################
def apply_selections( macro_file_name, selection_of_pairs ):
    tmp_file_name = "tmp_macro_file.txt"
    tmp_file = open( tmp_file_name, "w" )

    macro_file = open( macro_file_name, "r" )

    # Loop over all lines of the macro file, copy them to a "tm" file.
    # The only different line is that one correspondent to the selections chosen by the USER.
    for line in macro_file:
        if "===>>>" in line:
            selecting_pairs = TP_pair_selections+" && "+selection_of_pairs
            newline = "        if ("+selecting_pairs+") { //===>>> Apply general selections here\n"
            tmp_file.write(newline)
        else:
            tmp_file.write(line)
    tmp_file.close()
    macro_file.close()

    # Copying contents of the "tmp" file to the macro file and removing "tm" in the end.
    shutil.copy( tmp_file.name, macro_file.name )
    os.remove( tmp_file.name )


#######################################################################################
# Defining the main function, looping over list of ROOT files:
#   - each ROOT file will be read into a ROOT macro file;
#   - each ROOT file corresponding to a job submited via CONDOR.
#
# Here an output folder is created to where all results will arrive from CONDOR.
#
# In CONDOR, the macro file will run is this way:
#          root -b -l -q 'macro.cxx("dataset_definition", "dataset_path", debug, create_new_root)'
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

    # Macro file is copied to the new directory
    shutil.copy( ROOT_macro_file, directory+"/"+ROOT_macro_file)

    # Moving to the new directory
    os.chdir( directory )

    # Looping over all ROOT files
    for dataset_definition, mass_window, selection_of_pairs, nEvents, dataset_path in root_files:
        # Apply the correct selections of Tag and Probe pairs to the macro file
        apply_selections( ROOT_macro_file, selection_of_pairs )

        # Creating a new directory w.r.t. each ROOT file.
        output_folder = "DataSet_"+dataset_definition+"_"+mass_window

        # First of all, need to handle with directory name through fuction "changing_characters()"
        output_folder = changing_characters( output_folder )
        print "New ROOT file folder:", output_folder
        os.makedirs(output_folder)

        # Copying macro file to the new ROOT file directory
        shutil.copy( ROOT_macro_file, output_folder+"/"+ROOT_macro_file)

        # Moving to the new ROOT file directory
        os.chdir( output_folder )

        outputFile = open("submit_TagAndProbe_to_condor.sh", "w")
        # This function creates a script to submit jobs to CONDOR
        create_script1( outputFile )
        bash_via_python( "chmod 755 "+outputFile.name )
        outputFile.close()

        outputFile = open("TagAndProbe_for_condor.submit", "w")
        # This function creates a script with parameters submit jobs to CONDOR
        create_script2( outputFile, output_folder )
        bash_via_python( "chmod 755 "+outputFile.name )
        outputFile.close()

        outputFile = open("TagAndProbe.sh", "w")
        # This function creates a script which runs inside CONDOR's nodes
        create_script3( dataset_definition, selection_of_pairs, root_files[0][4], outputFile, output_folder, dataset_definition+" - "+mass_window, dataset_path, debug, new_folder )
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
