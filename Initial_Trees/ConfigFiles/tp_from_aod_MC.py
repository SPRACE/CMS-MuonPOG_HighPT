import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring(),
)
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

import os
if   "CMSSW_5_3_" in os.environ['CMSSW_VERSION']:
    process.GlobalTag.globaltag = cms.string('START53_V14::All')
    process.source.fileNames = [
        #  'root://osg-se.sprace.org.br//store/mc/Summer12_DR53X/DYToMuMu_M-20_CT10_TuneZ2star_v2_8TeV-powheg-pythia6/AODSIM/PU_S10_START53_V7A-v1/00001/DEE52826-1413-E211-BD58-90E6BA442F12.root'
        '/store/relval/CMSSW_5_3_6-START53_V14/RelValZMM/GEN-SIM-RECO/v2/00000/76156813-F529-E211-917B-003048678FA6.root',
        '/store/relval/CMSSW_5_3_6-START53_V14/RelValZMM/GEN-SIM-RECO/v2/00000/08C1D822-F629-E211-A6B1-003048679188.root',
    ]
elif "CMSSW_5_2_" in os.environ['CMSSW_VERSION']:
    process.GlobalTag.globaltag = cms.string('START52_V5::All')
    process.source.fileNames = [
        '/store/relval/CMSSW_5_2_3/RelValZMM/GEN-SIM-RECO/START52_V5-v1/0043/A29B9025-0E7A-E111-97E7-001A928116DE.root',
        '/store/relval/CMSSW_5_2_3/RelValZMM/GEN-SIM-RECO/START52_V5-v1/0043/5CAA0235-0F7A-E111-BA3E-0018F3D09690.root',
        '/store/relval/CMSSW_5_2_3/RelValZMM/GEN-SIM-RECO/START52_V5-v1/0043/1011EE9E-2B7A-E111-9349-0018F3D0970C.root',
        '/store/relval/CMSSW_5_2_3/RelValZMM/GEN-SIM-RECO/START52_V5-v1/0043/0E187509-0D7A-E111-8FA3-001A928116C2.root',
    ]
else: raise RuntimeError, "Unknown CMSSW version %s" % os.environ['CMSSW_VERSION']

## SELECT WHAT DATASET YOU'RE RUNNING ON
TRIGGER="SingleMu"
#TRIGGER="Any"

## ==== Fast Filters ====
process.goodVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 25 && position.Rho <= 2"),
    filter = cms.bool(True),
)
process.noScraping = cms.EDFilter("FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False), ## Or 'True' to get some per-event info
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
)
process.fastFilter = cms.Sequence(process.goodVertexFilter + process.noScraping)
##    __  __                       
##   |  \/  |_   _  ___  _ __  ___ 
##   | |\/| | | | |/ _ \| '_ \/ __|
##   | |  | | |_| | (_) | | | \__ \
##   |_|  |_|\__,_|\___/|_| |_|___/
##                                 
## ==== Merge CaloMuons and Tracks into the collection of reco::Muons  ====
from RecoMuon.MuonIdentification.calomuons_cfi import calomuons;
process.mergedMuons = cms.EDProducer("CaloMuonMerger",
    mergeTracks = cms.bool(True),
    mergeCaloMuons = cms.bool(False), # AOD
    muons     = cms.InputTag("muons"), 
    caloMuons = cms.InputTag("calomuons"),
    tracks    = cms.InputTag("generalTracks"),
    minCaloCompatibility = calomuons.minCaloCompatibility,
    ## Apply some minimal pt cut
    muonsCut     = cms.string("pt > 3 && track.isNonnull"),
    caloMuonsCut = cms.string("pt > 3"),
    tracksCut    = cms.string("pt > 3"),
)

## ==== Trigger matching
process.load("MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff")
## with some customization
process.muonMatchHLTL2.maxDeltaR = 0.3 # Zoltan tuning - it was 0.5
process.muonMatchHLTL3.maxDeltaR = 0.1
from MuonAnalysis.MuonAssociators.patMuonsWithTrigger_cff import *
changeRecoMuonInput(process, "mergedMuons")
useExtendedL1Match(process)
addHLTL1Passthrough(process)
changeTriggerProcessName(process, "*") # auto-guess

from MuonAnalysis.TagAndProbe.common_variables_cff import *
process.load("MuonAnalysis.TagAndProbe.common_modules_cff")

process.tagMuons = cms.EDFilter("PATMuonSelector",
##    src = cms.InputTag("patMuonsWithTrigger"),
    src = cms.InputTag("patMuonsWithTriggerOurMuons"),
    cut = cms.string("pt > 20 && "+MuonIDFlags.Tight2012.value()+
                     " && !triggerObjectMatchesByCollection('hltL3MuonCandidates').empty()"+
                     " && pfIsolationR04().sumChargedHadronPt/pt < 0.2"),
)
if TRIGGER != "SingleMu":
    process.tagMuons.cut = ("pt > 6 && (isGlobalMuon || isTrackerMuon) && isPFMuon "+
                            " && !triggerObjectMatchesByCollection('hltL3MuonCandidates').empty()"+
                            " && pfIsolationR04().sumChargedHadronPt/pt < 0.2")

process.oneTag  = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("tagMuons"), minNumber = cms.uint32(1))

process.probeMuons = cms.EDFilter("PATMuonSelector",
##    src = cms.InputTag("patMuonsWithTrigger"),
    src = cms.InputTag("patMuonsWithTriggerOurMuons"),
    #cut = cms.string("track.isNonnull"),  # no real cut now
    cut = cms.string("track.isNonnull && pt > 20"),
)

process.tpPairs = cms.EDProducer("CandViewShallowCloneCombiner",
    #cut = cms.string('60 < mass < 140 && abs(daughter(0).vz - daughter(1).vz) < 4'),
    #cut = cms.string('60 < mass && abs(daughter(0).vz - daughter(1).vz) < 4'),
    cut = cms.string('20 < mass && abs(daughter(0).vz - daughter(1).vz) < 4'),
    #decay = cms.string('tagMuons@+ probeMuons@-'),
    decay = cms.string('tagMuons probeMuons'),
    checkCharge = cms.bool(False)
)
process.onePair = cms.EDFilter("CandViewCountFilter", src = cms.InputTag("tpPairs"), minNumber = cms.uint32(1))

process.tagMuonsMCMatch = cms.EDProducer("MCTruthDeltaRMatcherNew",
    src = cms.InputTag("tagMuons"),
    matched = cms.InputTag("genParticles"),
    pdgId = cms.vint32(13),
    distMin = cms.double(0.3),
)
process.probeMuonsMCMatch = process.tagMuonsMCMatch.clone(src = "probeMuons")

from MuonAnalysis.TagAndProbe.muon.tag_probe_muon_extraIso_cff import ExtraIsolationVariables

process.load("MuonAnalysis.TagAndProbe.mvaIsoVariables_cff")
from MuonAnalysis.TagAndProbe.mvaIsoVariables_cff import MVAIsoVariablesPlain, MVAIsoVariablesPlainTag
process.load("MuonAnalysis.TagAndProbe.radialIso_cfi")


process.tpTree = cms.EDAnalyzer("TagProbeFitTreeProducer",
    # choice of tag and probe pairs, and arbitration
    tagProbePairs = cms.InputTag("tpPairs"),
    arbitration   = cms.string("None"),
    # probe variables: all useful ones
    variables = cms.PSet(
        AllVariables,
    #    ExtraIsolationVariables,
    #    MVAIsoVariablesPlain, 
        isoTrk03Abs = cms.InputTag("probeMuonsIsoValueMaps","probeMuonsIsoFromDepsTk"),
        isoTrk03Rel = cms.InputTag("probeMuonsIsoValueMaps","probeMuonsRelIsoFromDepsTk"),
        dxyBS = cms.InputTag("muonDxyPVdzmin","dxyBS"),
        dxyPVdzmin = cms.InputTag("muonDxyPVdzmin","dxyPVdzmin"),
        dzPV = cms.InputTag("muonDxyPVdzmin","dzPV"),
        radialIso = cms.InputTag("radialIso"), 
        nSplitTk  = cms.InputTag("splitTrackTagger"),
        ###New Variables by Cesar and Angelo
        IsNewHPTmuon  = cms.string("userFloat('IsNewHPTmuonGlobal')"),
        NewTuneP_pt = cms.string("userFloat('NEWTUNEPpt')"),
        NewTuneP_ptRelError = cms.string("userFloat('NEWTUNEPptRelError')"),
        NewTuneP_eta = cms.string("userFloat('NEWTUNEPeta')"),
        NewTuneP_phi = cms.string("userFloat('NEWTUNEPphi')"),
    ),
    flags = cms.PSet(
       TrackQualityFlags,
       MuonIDFlags,
       HighPtTriggerFlags,
       HighPtTriggerFlagsDebug,
    ),
    tagVariables = cms.PSet(
        TriggerVariables, 
    #    MVAIsoVariablesPlainTag, 
        pt = cms.string("pt"),
        eta = cms.string("eta"),
        phi = cms.string("phi"),
        nVertices   = cms.InputTag("nverticesModule"),
        combRelIso = cms.string("(isolationR03.emEt + isolationR03.hadEt + isolationR03.sumPt)/pt"),
        chargedHadIso04 = cms.string("pfIsolationR04().sumChargedHadronPt"),
        neutralHadIso04 = cms.string("pfIsolationR04().sumNeutralHadronEt"),
        photonIso04 = cms.string("pfIsolationR04().sumPhotonEt"),
        combRelIsoPF04dBeta = IsolationVariables.combRelIsoPF04dBeta,
        dzPV = cms.InputTag("muonDxyPVdzminTags","dzPV"),
        ###New Variables by Cesar and Angelo
        ##MET  = cms.InputTag("MetModule"),
        MET     = cms.InputTag("MetModule", "MET"),
        METx    = cms.InputTag("MetModule", "METx"),
        METy    = cms.InputTag("MetModule", "METy"),
        METphi  = cms.InputTag("MetModule", "METphi"),
        IsNewHPTmuon  = cms.string("userFloat('IsNewHPTmuonGlobal')"),
        NewTuneP_pt = cms.string("userFloat('NEWTUNEPpt')"),
        NewTuneP_ptRelError = cms.string("userFloat('NEWTUNEPptRelError')"),
        NewTuneP_eta = cms.string("userFloat('NEWTUNEPeta')"),
        NewTuneP_phi = cms.string("userFloat('NEWTUNEPphi')"),
        innertrackPtRelError = TrackQualityVariables.innertkSigmaPtOverPt,
        charge = cms.string("charge"),
    ),
    tagFlags = cms.PSet(HighPtTriggerFlags,HighPtTriggerFlagsDebug),
    pairVariables = cms.PSet(
        nJets30 = cms.InputTag("njets30Module"),
        dz      = cms.string("daughter(0).vz - daughter(1).vz"),
        pt      = cms.string("pt"), 
        rapidity = cms.string("rapidity"),
        deltaR   = cms.string("deltaR(daughter(0).eta, daughter(0).phi, daughter(1).eta, daughter(1).phi)"), 
        probeMultiplicity = cms.InputTag("probeMultiplicity"),
        ## New TuneP variables
        newTuneP_probe_pt            = cms.InputTag("newTunePVals", "pt"),
        newTuneP_probe_sigmaPtOverPt = cms.InputTag("newTunePVals", "ptRelError"),
        newTuneP_probe_trackType     = cms.InputTag("newTunePVals", "trackType"),
        newTuneP_mass                = cms.InputTag("newTunePVals", "mass"),
        ###New Variables by Cesar and Angelo
        DimuonVtxFitNormQui2         = cms.InputTag("DimuonVtxModule", "vtxNormQui2"),
        DimuonVtxZcoordinate         = cms.InputTag("DimuonVtxModule", "vtxZcoordinate"),
        DimuonVtxZcoordinateFromBS   = cms.InputTag("DimuonVtxModule", "vtxZcoordinateFromBS"),
        DimuonVtxRdistance           = cms.InputTag("DimuonVtxModule", "vtxRdistance"),
        DimuonVtxRdistanceFromBS     = cms.InputTag("DimuonVtxModule", "vtxRdistanceFromBS"),
        DimuonVtxTagPtAtTheVtx       = cms.InputTag("DimuonVtxModule", "tagPtAtTheVtx"),
        DimuonVtxProbePtAtTheVtx     = cms.InputTag("DimuonVtxModule", "probePtAtTheVtx"),
        DimuonVtxTagPtBefore         = cms.InputTag("DimuonVtxModule", "tagPtBefore"),
        DimuonVtxProbePtBefore       = cms.InputTag("DimuonVtxModule", "probePtBefore"),
        collinearity1                = cms.InputTag("VetoCosmicMuonsModule", "collinearity1"),
        GenPVar_MuPlusPt             = cms.InputTag("GenPVariablesModule", "vMuPluspt"),
        GenPVar_MuPlusEta            = cms.InputTag("GenPVariablesModule", "vMuPluseta"),
        GenPVar_MuPlusPhi            = cms.InputTag("GenPVariablesModule", "vMuPlusphi"),
        GenPVar_MuMinusPt            = cms.InputTag("GenPVariablesModule", "vMuMinuspt"),
        GenPVar_MuMinusEta           = cms.InputTag("GenPVariablesModule", "vMuMinuseta"),
        GenPVar_MuMinusPhi           = cms.InputTag("GenPVariablesModule", "vMuMinusphi"),
        GenPVar_DimuMass             = cms.InputTag("GenPVariablesModule", "vDimuMass"),
        dx                           = cms.string("daughter(0).vx - daughter(1).vx"),
        dy                           = cms.string("daughter(0).vy - daughter(1).vy"),
        dxy                          = cms.string(" sqrt( pow((daughter(0).vx - daughter(1).vx),2) + pow((daughter(0).vy - daughter(1).vy),2) ) "),
    ),
    pairFlags = cms.PSet(
        BestZ = cms.InputTag("bestPairByZMass"),
    ),
    isMC           = cms.bool(True),
    addRunLumiInfo = cms.bool(True),
    tagMatches       = cms.InputTag("tagMuonsMCMatch"),
    probeMatches     = cms.InputTag("probeMuonsMCMatch"),
    motherPdgId      = cms.vint32(22, 23),
    makeMCUnbiasTree       = cms.bool(False), 
    checkMotherInUnbiasEff = cms.bool(True),
    allProbes              = cms.InputTag("probeMuons"),
)
if TRIGGER != "SingleMu":
    for K,F in MuonIDFlags.parameters_().iteritems():
        setattr(process.tpTree.tagFlags, K, F)


process.load("MuonAnalysis.TagAndProbe.muon.tag_probe_muon_extraIso_cfi")

process.extraProbeVariablesSeq = cms.Sequence(
    process.probeMuonsIsoSequence +
    process.computeCorrectedIso + 
    process.mvaIsoVariablesSeq * process.mvaIsoVariablesTag * process.radialIso +
    process.splitTrackTagger +
    process.muonDxyPVdzmin 
)

process.tnpSimpleSequence = cms.Sequence(
    process.tagMuons   * process.tagMuonsMCMatch   +
    process.oneTag     +
    process.probeMuons * process.probeMuonsMCMatch +
    process.tpPairs    +
    process.onePair    +
    process.nverticesModule +
    process.njets30Module +
    ### Cesar and Angelo Begin
    process.MetModule +
    process.DimuonVtxModule +
    process.VetoCosmicMuonsModule +
    process.GenPVariablesModule +
    ### Cesar and Angelo End
    process.extraProbeVariablesSeq +
    process.probeMultiplicity + 
    process.bestPairByZMass + 
    process.newTunePVals +
    process.muonDxyPVdzminTags +
    process.tpTree
)

process.tagAndProbe = cms.Path( 
    process.fastFilter +
    process.mergedMuons                 *
    process.patMuonsWithTriggerSequence +
    ### Cesar and Angelo Begin
    process.patMuonsWithTriggerOurMuons +
    ### Cesar and Angelo End
    process.tnpSimpleSequence
)


process.schedule = cms.Schedule(
   process.tagAndProbe, 
#   process.tagAndProbeSta,
#   process.fakeRateJetPlusProbe,
#   process.fakeRateWPlusProbe,
#   process.fakeRateZPlusProbe,
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("tnpZ_HighMassDimuon.root"))
