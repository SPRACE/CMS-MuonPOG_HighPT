import FWCore.ParameterSet.Config as cms

process = cms.Process("TagProbe")

process.load('FWCore.MessageService.MessageLogger_cfi')
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.source = cms.Source("PoolSource", 
    fileNames = cms.untracked.vstring( ),
)
#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )    
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(500) )

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")

import os
if   "CMSSW_5_3_" in os.environ['CMSSW_VERSION']:
#    process.GlobalTag.globaltag = cms.string('GR_P_V42_AN2::All')
    process.GlobalTag.globaltag = cms.string('FT_53_V21_AN4::All')
    process.source.fileNames = [
        '/store/data/Run2012A//SingleMu/AOD/22Jan2013-v1/20000/FEC16A3C-8A70-E211-9AC8-1CC1DE040FE8.root',
        '/store/data/Run2012B/SingleMu//AOD/22Jan2013-v1/20001/FE31658E-8A72-E211-8661-20CF3027A596.root',
        '/store/data/Run2012C/SingleMu//AOD/22Jan2013-v1/20059/24C4169E-8D78-E211-B9E6-00259073E4AC.root',
        '/store/data/Run2012D/SingleMu/AOD/22Jan2013-v1/10000/FE618190-93A7-E211-B50C-90E6BA19A215.root',       
#        '/store/data/Run2012D/DoubleMu/AOD/PromptReco-v1/000/207/924/0A79B825-B339-E211-869D-BCAEC5364C93.root',
#        '/store/data/Run2012D/DoubleMu/AOD/PromptReco-v1/000/207/924/3AB8635A-C339-E211-AD34-003048D2BC30.root',
#        '/store/data/Run2012D/DoubleMu/AOD/PromptReco-v1/000/207/924/74791AB5-B639-E211-A2B4-0030486780B8.root',
#        '/store/data/Run2012D/DoubleMu/AOD/PromptReco-v1/000/207/924/883C07A5-9E39-E211-A669-001D09F248F8.root',
#        '/store/data/Run2012D/DoubleMu/AOD/PromptReco-v1/000/207/924/92B75860-B739-E211-80AC-001D09F242EF.root',
#        '/store/data/Run2012D/DoubleMu/AOD/PromptReco-v1/000/207/924/E03B2BD3-B339-E211-9CC5-001D09F29114.root',
#        '/store/data/Run2012D/DoubleMu/AOD/PromptReco-v1/000/207/924/E439CDE0-DE39-E211-8727-0025901D631E.root',
#        '/store/data/Run2012D/DoubleMu/AOD/PromptReco-v1/000/207/924/E4C70744-A439-E211-89FC-003048678098.root',
    ]
#     process.source.fileNames = [
#         '/store/data/Run2012C/SingleMu/AOD/PromptReco-v1/000/198/208/EA6F61C2-4AC7-E111-9190-001D09F27003.root',
#         '/store/data/Run2012C/SingleMu/AOD/PromptReco-v1/000/198/208/E8AF1970-2EC7-E111-A30E-001D09F29146.root',
#         '/store/data/Run2012C/SingleMu/AOD/PromptReco-v1/000/198/208/7421AA75-27C7-E111-9FFA-5404A63886AF.root',
#     ]
#    process.source.fileNames = [
#        '/store/data/Run2012C/SingleMu/RECO/PromptReco-v2/000/201/678/E24492CF-60F1-E111-B1A3-0025901D624A.root',
#        '/store/data/Run2012C/SingleMu/RECO/PromptReco-v2/000/201/678/CC6F5333-5EF1-E111-BB26-BCAEC5364C62.root',
#        '/store/data/Run2012C/SingleMu/RECO/PromptReco-v2/000/201/678/A0C46F23-6AF1-E111-8E4D-001D09F34488.root',
#        '/store/data/Run2012C/SingleMu/RECO/PromptReco-v2/000/201/678/78F208A3-6EF1-E111-BC73-001D09F23D1D.root',
#        '/store/data/Run2012C/SingleMu/RECO/PromptReco-v2/000/201/678/52FE8A6E-77F1-E111-A026-00215AEDFCCC.root',
#        '/store/data/Run2012C/SingleMu/RECO/PromptReco-v2/000/201/678/3A358B5D-69F1-E111-9FB3-001D09F2441B.root',
#        '/store/data/Run2012C/SingleMu/RECO/PromptReco-v2/000/201/678/166B73E0-64F1-E111-A723-5404A63886D2.root',
#    ]
elif "CMSSW_5_2_" in os.environ['CMSSW_VERSION']:
    process.GlobalTag.globaltag = cms.string('GR_P_V39_AN1::All')
    process.source.fileNames = [
        '/store/data/Run2012B/SingleMu/AOD/PromptReco-v1/000/196/531/C64F808D-10BB-E111-AD32-0030486780B4.root',
        '/store/data/Run2012B/SingleMu/AOD/PromptReco-v1/000/196/531/B6EC67C6-0DBB-E111-BEC9-5404A6388697.root',
        '/store/data/Run2012B/SingleMu/AOD/PromptReco-v1/000/196/531/AEF38851-2BBB-E111-AE15-001D09F2447F.root',
    ]
else: raise RuntimeError, "Unknown CMSSW version %s" % os.environ['CMSSW_VERSION']

## SELECT WHAT DATASET YOU'RE RUNNING ON
TRIGGER="SingleMu"
#TRIGGER="DoubleMu"

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

process.load("HLTrigger.HLTfilters.triggerResultsFilter_cfi")

if TRIGGER == "SingleMu":
    process.triggerResultsFilter.triggerConditions = cms.vstring( 'HLT_IsoMu24_v*', 'HLT_IsoMu24_eta2p1_v*', 'HLT_Mu40_v*', 'HLT_Mu40_eta2p1_v*' )
elif TRIGGER == "DoubleMu":
    process.triggerResultsFilter.triggerConditions = cms.vstring( 'HLT_Mu8_v*', 'HLT_Mu17_v*', 'HLT_Mu17_TkMu8_NoDZ_v*', 'HLT_Mu13_Mu8_NoDZ_v*' )
else:
    raise RuntimeError, "TRIGGER must be 'SingleMu' or 'DoubleMu'"

process.triggerResultsFilter.l1tResults = ''
process.triggerResultsFilter.throw = False
process.triggerResultsFilter.hltResults = cms.InputTag( "TriggerResults", "", "HLT" )

#process.triggerResultsFilterFake = process.triggerResultsFilter.clone(
#    triggerConditions = cms.vstring( 'HLT_Mu40_v*', 'HLT_Mu5_v*', 'HLT_Mu12_v*', 'HLT_Mu24_v*')
#)

process.fastFilter     = cms.Sequence(process.goodVertexFilter + process.noScraping + process.triggerResultsFilter)
#process.fastFilterFake = cms.Sequence(process.goodVertexFilter + process.noScraping + process.triggerResultsFilterFake)
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


from MuonAnalysis.TagAndProbe.common_variables_cff import *
process.load("MuonAnalysis.TagAndProbe.common_modules_cff")

process.tagMuons = cms.EDFilter("PATMuonSelector",
##    src = cms.InputTag("patMuonsWithTrigger"),
    src = cms.InputTag("patMuonsWithTriggerOurMuons"),
    cut = cms.string("pt > 20 && "+MuonIDFlags.Tight2012.value()+
                     " && !triggerObjectMatchesByCollection('hltL3MuonCandidates').empty()"+
                     " && pfIsolationR04().sumChargedHadronPt/pt < 0.2"),
)
if TRIGGER == "DoubleMu":
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
        l1rate = cms.InputTag("l1rate"),
        bx     = cms.InputTag("l1rate","bx"),
        mu17ps = cms.InputTag("l1hltprescale","HLTMu17TotalPrescale"), 
        mu8ps  = cms.InputTag("l1hltprescale","HLTMu8TotalPrescale"), 
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
        DimuonVtxFitNormQui2             = cms.InputTag("DimuonVtxModule", "vtxNormQui2"),
        DimuonVtxZcoordinate             = cms.InputTag("DimuonVtxModule", "vtxZcoordinate"),
        DimuonVtxZcoordinateFromBS       = cms.InputTag("DimuonVtxModule", "vtxZcoordinateFromBS"),
        DimuonVtxRdistance               = cms.InputTag("DimuonVtxModule", "vtxRdistance"),
        DimuonVtxRdistanceFromBS         = cms.InputTag("DimuonVtxModule", "vtxRdistanceFromBS"),
        DimuonVtxTagPtAtTheVtx           = cms.InputTag("DimuonVtxModule", "tagPtAtTheVtx"),
        DimuonVtxProbePtAtTheVtx         = cms.InputTag("DimuonVtxModule", "probePtAtTheVtx"),
        DimuonVtxTagPtBefore             = cms.InputTag("DimuonVtxModule", "tagPtBefore"),
        DimuonVtxProbePtBefore           = cms.InputTag("DimuonVtxModule", "probePtBefore"),
        collinearity1                = cms.InputTag("VetoCosmicMuonsModule", "collinearity1"),
        dx      = cms.string("daughter(0).vx - daughter(1).vx"),
        dy      = cms.string("daughter(0).vy - daughter(1).vy"),
        dxy     = cms.string(" sqrt( pow((daughter(0).vx - daughter(1).vx),2) + pow((daughter(0).vy - daughter(1).vy),2) ) "),
    ),
    pairFlags = cms.PSet(
        BestZ = cms.InputTag("bestPairByZMass"),
    ),
    isMC           = cms.bool(False),
    addRunLumiInfo = cms.bool(True),
)
if TRIGGER == "DoubleMu":
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
    process.tagMuons +
    process.oneTag     +
    process.probeMuons +
    process.tpPairs    +
    process.onePair    +
    process.nverticesModule +
    process.njets30Module +
    ### Cesar and Angelo Begin
    process.MetModule +
    process.DimuonVtxModule +
    process.VetoCosmicMuonsModule +
    ### Cesar and Angelo End
    process.extraProbeVariablesSeq +
    process.l1rate +
    process.l1hltprescale + 
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
)

process.TFileService = cms.Service("TFileService", fileName = cms.string("tnpZ_HighMassDimuon.root"))
