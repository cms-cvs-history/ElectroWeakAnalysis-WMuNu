import FWCore.ParameterSet.Config as cms

process = cms.Process("WMuNuPATAnalyzer")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:data1/example.pat.root')
)

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring('wMuNuPATAnalyzer'),
    cout = cms.untracked.PSet(
        threshold = cms.untracked.string('DEBUG')
    ),
    destinations = cms.untracked.vstring('cout')
)


process.wMuNuPATAnalyzer = cms.EDFilter("WMuNuPATAnalyzer",
    muonTag     = cms.untracked.InputTag("selectedLayer1Muons"),
    jetTag      = cms.untracked.InputTag("selectedLayer1Jets"),
    metTag      = cms.untracked.InputTag("selectedLayer1METs")
    # Defaults for Selection cuts ->
    #UseOnlyGlobalMuons = cms.bool(True),
    #
    # Main cuts ->
    #PtCut = cms.double(25.0),
    #EtaCut = cms.double(2.1),
    #IsRelativeIso = cms.bool(True),
    #IsoCut03 = cms.double(0.1),
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

process.TFileService = cms.Service("TFileService", fileName = cms.string('WMuNuPATAnalyzer.root') )

process.p = cms.Path(process.wMuNuPATAnalyzer)

