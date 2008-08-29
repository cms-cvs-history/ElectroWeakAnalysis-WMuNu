import FWCore.ParameterSet.Config as cms

process = cms.Process("PATAnalyzer")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:data1/example.pat.root')
)

process.MessageLogger = cms.Service("MessageLogger")

process.PATAnalyzer = cms.EDFilter("PATAnalyzer",
    electronTag = cms.untracked.InputTag("selectedLayer1Electrons"),
    tauTag      = cms.untracked.InputTag("selectedLayer1Taus"),
    muonTag     = cms.untracked.InputTag("selectedLayer1Muons"),
    jetTag      = cms.untracked.InputTag("selectedLayer1Jets"),
    photonTag   = cms.untracked.InputTag("selectedLayer1Photons"),
    metTag      = cms.untracked.InputTag("selectedLayer1METs")
)

process.TFileService = cms.Service("TFileService", fileName = cms.string('PATAnalyzer.root') )

process.p = cms.Path(process.PATAnalyzer)

