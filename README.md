####################################################################

            ID Efficiencies of Very High Pt Muons
____________________________________________________________________

This is a Guidance to help running the Tag & Probe code to compute
efficiencies of very high pt muons, out of Z peak, but with counting
numbers instead of a fitting around Z ressonace.


To get the repository of this task, type:
   git clone https://github.com/SPRACE/CMS-MuonPOG_HighPT.git


There are different directories for different tasks:
(1) Setting up the Tag & Probe working area: production of trees
    -> Initial_Trees
(2) Check repeated files from CRAB
    -> Check_Repeated_Files
(3) Merging files
    -> Merging_Files
(4) Correction through vertex multiplicity
    -> Vertex_Reweighting
(5) Choose T&P pairs based on the best Chi2
    -> CleanUp_TnP_Pairs
(6) Cross checks comparing Data and MC distributions
    -> Data_vs_MC_Distributions
(7) Computing efficiencies
    -> Compute_Efficiencies

Go into each directory, starting from (1), and follow their instructions.
