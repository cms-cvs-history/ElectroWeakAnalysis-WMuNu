import FWCore.ParameterSet.Config as cms

process = cms.Process("PATAnalyzer")

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('file:/data1/jalcaraz/PAT_WMuNu_0.root')
)

process.MessageLogger = cms.Service("MessageLogger",
      debugModules = cms.untracked.vstring('PATAnalyzer'),
      cout = cms.untracked.PSet(
            threshold = cms.untracked.string('INFO'),
            default = cms.untracked.PSet(
                  limit = cms.untracked.int32(10) 
            )
      ),
      destinations = cms.untracked.vstring('cout')
)


process.PATAnalyzer = cms.EDFilter("PATAnalyzer"
    , electronTag = cms.untracked.InputTag("selectedLayer1Electrons")
    #, tauTag      = cms.untracked.InputTag("selectedLayer1Taus")
    , muonTag     = cms.untracked.InputTag("selectedLayer1Muons")
    , jetTag      = cms.untracked.InputTag("selectedLayer1Jets")
    , photonTag   = cms.untracked.InputTag("selectedLayer1Photons")
    , metTag      = cms.untracked.InputTag("selectedLayer1METs")
    , pfJetTag      = cms.untracked.InputTag("iterativeCone5PFJets")
    , pfMetTag      = cms.untracked.InputTag("pfMet")
)

process.TFileService = cms.Service("TFileService", fileName = cms.string('PATAnalyzer.root') )

process.p = cms.Path(process.PATAnalyzer)

