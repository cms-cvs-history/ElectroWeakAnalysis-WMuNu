import FWCore.ParameterSet.Config as cms

process = cms.Process("wmnsel")

process.wmnSelFilter = cms.EDFilter("WMuNuSelector",
    # Defaults follow...
    #
    # Input collections ->
    #MuonTag = cms.InputTag("muons"),
    #METTag = cms.InputTag("met"),
    #JetTag = cms.InputTag("iterativeCone5CaloJets"),
    #UseOnlyGlobalMuons = cms.bool(True),
    #
    # Main cuts ->
    #PtCut = cms.double(25.0),
    #EtaCut = cms.double(2.1),
    #IsRelativeIso = cms.bool(True),
    #IsoCut03 = cms.double(0.1),
    #IsoCut05 = cms.double(999999.),
    #MassTMin = cms.double(50.0),
    #MassTMax = cms.double(200.0),
    #
    # To suppress Zmm ->
    #PtThrForZCount = cms.double(20.0),
    #
    # To suppress ttbar ->
    #AcopCut = cms.double(999999.),
    #EJetMin = cms.double(999999.),
    #NJetMax = cms.int32(999999)
)


process.load("Configuration.EventContent.EventContent_cff")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('wmnSelFilter'),
    categories = cms.untracked.vstring('FwkJob','FwkReport','FwkSummary','Root_NoDictionary'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    destinations = cms.untracked.vstring('cout')
)

process.AdaptorConfig = cms.Service("AdaptorConfig")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.source = cms.Source("PoolSource",
    debugVerbosity = cms.untracked.uint32(0),
    debugFlag = cms.untracked.bool(False),
    fileNames = cms.untracked.vstring('file:/afs/cern.ch/cms/PRS/top/cmssw-data/relval200-for-pat-testing/FullSimTTBar-2_1_X_2008-07-08_STARTUP_V4-AODSIM.100.root')
)

process.wmnOutput = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('wmnsel')
    ),
    fileName = cms.untracked.string('data1/wmnsel.root')
)

process.wmnsel = cms.Path(process.wmnSelFilter)
process.end = cms.EndPath(process.wmnOutput)


