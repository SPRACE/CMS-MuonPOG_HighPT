import FWCore.ParameterSet.Config as cms

### USAGE:
###    cmsRun fitTrigger_Z.py <scenario>
### scenarios:
###   - data_all:    will fit tnpJPsi_Data.root with bins suitable for the current data
###   - datalike_mc: will fit tnpJPsi_{JPsiMuMu,ppMuX}_Spring10_0.117pb.root MC but
###                  with same config as data

import sys
args = sys.argv[1:]
print args
if (sys.argv[0] == "cmsRun"): args =sys.argv[2:]
scenario = "MC"
if len(args) > 0: scenario = args[0]
print "Will run scenario ", scenario 

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')

process.source = cms.Source("EmptySource")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1) )

process.TnP_MuonID = cms.EDAnalyzer("TagProbeFitTreeAnalyzer",
    NumCPU = cms.uint32(1),
    SaveWorkspace = cms.bool(False),

    InputFileNames = cms.vstring(
        'dcap://osg-se.sprace.org.br:/pnfs/sprace.org.br/data/cms/store/user/adesouza/MuonPOG/HighMassDimuonEff_WithBestPair_v6_BaselineSelection_Feb_25_2014/tnpZ_addBestPair_DY_mumu__Mass_mu1_mu2__gt_20_GeV.root', #===>>> Modify this line
    ),
    InputTreeName = cms.string("fitter_tree"),
    InputDirectoryName = cms.string("tpTree"),
    OutputFileName = cms.string("TnP_Z_CountHighMassDiMuID_%s.root" % scenario),
    WeightVariable = cms.string("weight"),
    Variables = cms.PSet(
        weight = cms.vstring("weight","0","10",""),
        pair_newTuneP_mass = cms.vstring("Tag-muon Mass", "20", "10000", "GeV/c^{2}"),
        pair_dz            = cms.vstring("Pair dZ", "-10.0", "10.0", ""),
        pair_deltaR        = cms.vstring("#Delta R(Tag,Probe)", "0.0", "10.0", ""),
        NewTuneP_pt        = cms.vstring("Probe p_{T}", "45", "5000", "GeV/c"),
        NewTuneP_eta       = cms.vstring("#eta(Probe)", "-10.0", "10.0", ""),
        NewTuneP_phi       = cms.vstring("#phi(Probe)", "-3.5", "3.5", ""),
        tag_nVertices      = cms.vstring("Number of vertices", "0", "999", ""),
#        dzPV               = cms.vstring("Probe dzPV", "-1.0", "1.0", ""),
#        tag_dzPV           = cms.vstring("Tag dzPV", "-1.0", "1.0", ""),
#        abseta             = cms.vstring("Probe |#eta|", "0", "2.5", ""),
#        pair_probeMultiplicity = cms.vstring("pair_probeMultiplicity", "0","30",""),
#        tag_pt             = cms.vstring("Tag p_{T}", "2.6", "1000", "GeV/c"),
#        phi                = cms.vstring("phi", "-3.0", "3.0", ""),
#        tkIsoRel           = cms.vstring("tkIso rel", "0.0", "1.0" , ""),
    ),

    Categories = cms.PSet(
        Tight2012                   = cms.vstring("TightID2012", "dummy[pass=1,fail=0]"),
        pair_passBaselineSelections = cms.vstring("BestPairWithBaselineSelections", "dummy[pass=1,fail=0]"),
    ),

    PDFs = cms.PSet(
        gaussPlusExpo = cms.vstring(
            "Voigtian::signal(pair_newTuneP_mass, mean[90,80,100], width[2.495], sigma[3,1,20])",
            "Exponential::backgroundPass(pair_newTuneP_mass, lp[0,-5,5])",
            "Exponential::backgroundFail(pair_newTuneP_mass, lf[0,-5,5])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        ),
        vpvPlusExpo = cms.vstring(
            "Voigtian::signal1(pair_newTuneP_mass, mean1[90,80,100], width[2.495], sigma1[2,1,3])",
            "Voigtian::signal2(pair_newTuneP_mass, mean2[90,80,100], width,        sigma2[4,2,10])",
            "SUM::signal(vFrac[0.8,0,1]*signal1, signal2)",
            "Exponential::backgroundPass(pair_newTuneP_mass, lp[-0.1,-1,0.1])",
            "Exponential::backgroundFail(pair_newTuneP_mass, lf[-0.1,-1,0.1])",
            "efficiency[0.9,0,1]",
            "signalFractionInPassing[0.9]"
        )
    ),
    
#    binnedFit = cms.bool(True),
#    binsForFit = cms.uint32(40),
    saveDistributionsPlot = cms.bool(False),

    Efficiencies = cms.PSet(), # will be filled later
)

PT_BINS = cms.PSet(
    NewTuneP_pt = cms.vdouble(45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 140, 300, 500, 1000),
    pair_passBaselineSelections = cms.vstring("pass"),
)

ETA_BINS = cms.PSet(
    NewTuneP_eta = cms.vdouble(-2.4, -2.1, -1.6, -1.2, -0.9, -0.6, -0.3, -0.2, 0.2, 0.3, 0.6, 0.9, 1.2, 1.6, 2.1, 2.4),
    pair_passBaselineSelections = cms.vstring("pass"),
)

PHI_BINS = cms.PSet(
    NewTuneP_phi = cms.vdouble(-3.0,-2.5,-2.0,-1.5,-1.0,-0.5,0.0,0.5,1.0,1.5,2.0,2.5,3.0),  
    pair_passBaselineSelections = cms.vstring("pass"),
)

VTX_BINS = cms.PSet(
    tag_nVertices = cms.vdouble(0.5,2.5,4.5,6.5,8.5,10.5,12.5,14.5,16.5,18.5,20.5,22.5,24.5,26.5,28.5,30.5, 32.5, 34.5, 36.5, 38.5, 40.5, 42.5, 44.5, 46.5, 48.5, 49.5),
    pair_passBaselineSelections = cms.vstring("pass"),
)

MASS_BINS = cms.PSet(
    pair_newTuneP_mass = cms.vdouble(20,120,200,500,800,1000,1500,2000,3000),
    pair_passBaselineSelections = cms.vstring("pass"),
)

deltaR_BINS = cms.PSet(
    pair_deltaR = cms.vdouble(0.0,0.5,0.75,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,6.0),
    pair_passBaselineSelections = cms.vstring("pass"),
)

#tkIsoRel_BINS = cms.PSet(
#    tkIsoRel = cms.vdouble(0.0,0.005,0.01,0.015,0.02,0.025,0.03,0.035,0.04,0.045,0.05,0.055,0.06,0.065,0.07,0.075,0.08,0.085,0.09,0.095,0.1,0.105,0.11),
#    pair_passBaselineSelections = cms.vstring("pass"),
#)

process.TnP_MuonID.Efficiencies.Tight2012_PT_BINS = cms.PSet(
      EfficiencyCategoryAndState = cms.vstring("Tight2012","pass"),
#      EfficiencyCategoryAndState = cms.vstring("MuonIDHPTGlobal","pass"), #include IsoVarCorr
      UnbinnedVariables = cms.vstring("pair_newTuneP_mass","weight"),
      BinnedVariables = PT_BINS,
  #    BinToPDFmap = cms.vstring("gaussPlusExpo")
)

process.TnP_MuonID.Efficiencies.Tight2012_ETA_BINS = cms.PSet(
      EfficiencyCategoryAndState = cms.vstring("Tight2012","pass"),
#      EfficiencyCategoryAndState = cms.vstring("MuonIDHPTGlobal","pass"), #include IsoVarCorr
      UnbinnedVariables = cms.vstring("pair_newTuneP_mass","weight"),
      BinnedVariables = ETA_BINS,
#      BinToPDFmap = cms.vstring("gaussPlusExpo")
)


process.TnP_MuonID.Efficiencies.Tight2012_VTX_BINS = cms.PSet(
      EfficiencyCategoryAndState = cms.vstring("Tight2012","pass"),
#      EfficiencyCategoryAndState = cms.vstring("MuonIDHPTGlobal","pass"), #include IsoVarCorr
      UnbinnedVariables = cms.vstring("pair_newTuneP_mass","weight"),
      BinnedVariables = VTX_BINS,
#      BinToPDFmap = cms.vstring("vpvPlusExpo")
)

process.TnP_MuonID.Efficiencies.Tight2012_PHI_BINS = cms.PSet(
      EfficiencyCategoryAndState = cms.vstring("Tight2012","pass"),
#      EfficiencyCategoryAndState = cms.vstring("MuonIDHPTGlobal","pass"), #include IsoVarCorr
      UnbinnedVariables = cms.vstring("pair_newTuneP_mass","weight"),
      BinnedVariables = PHI_BINS,
#      BinToPDFmap = cms.vstring("vpvPlusExpo")
)

process.TnP_MuonID.Efficiencies.Tight2012_MASS_BINS = cms.PSet(
      EfficiencyCategoryAndState = cms.vstring("Tight2012","pass"),
#      EfficiencyCategoryAndState = cms.vstring("MuonIDHPTGlobal","pass"), #include IsoVarCorr
      UnbinnedVariables = cms.vstring("NewTuneP_pt","weight"),
      BinnedVariables = MASS_BINS,
#      BinToPDFmap = cms.vstring("vpvPlusExpo")
)

process.TnP_MuonID.Efficiencies.Tight2012_deltaR_BINS = cms.PSet(
      EfficiencyCategoryAndState = cms.vstring("Tight2012","pass"),
      UnbinnedVariables = cms.vstring("pair_newTuneP_mass","weight"),
      BinnedVariables = deltaR_BINS,
)

#process.TnP_MuonID.Efficiencies.Tight2012_tkIsoRel_BINS = cms.PSet(
#      EfficiencyCategoryAndState = cms.vstring("Tight2012","pass"),
#      UnbinnedVariables = cms.vstring("pair_newTuneP_mass","weight"),
#      BinnedVariables = tkIsoRel_BINS,
#)


process.p = cms.Path(
    process.TnP_MuonID
)

