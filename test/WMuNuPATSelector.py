import FWCore.ParameterSet.Config as cms

# Process, how many events, inout files, ...
process = cms.Process("wmnsel")
process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(-1)
      #input = cms.untracked.int32(100)
)
process.source = cms.Source("PoolSource",
      debugVerbosity = cms.untracked.uint32(0),
      debugFlag = cms.untracked.bool(False),
      fileNames = cms.untracked.vstring("file:PAT_test.root")
)

# Debug/info printouts
process.MessageLogger = cms.Service("MessageLogger",
      debugModules = cms.untracked.vstring('wmnSelFilter'),
      cout = cms.untracked.PSet(
            default = cms.untracked.PSet( limit = cms.untracked.int32(10) ),
            threshold = cms.untracked.string('INFO')
            #threshold = cms.untracked.string('DEBUG')
      ),
      destinations = cms.untracked.vstring('cout')
)

# Selector and parameters
process.wmnSelFilter = cms.EDFilter("WMuNuPATSelector",
      # Input collections ->
      TrigTag = cms.untracked.InputTag("TriggerResults::HLT"),
      MuonTag = cms.untracked.InputTag("selectedLayer1Muons"),
      METTag = cms.untracked.InputTag("layer1METs"),
      METIncludesMuons = cms.untracked.bool(True),
      JetTag = cms.untracked.InputTag("selectedLayer1Jets"),
      
      # Main cuts ->
      MuonTrig = cms.untracked.string("HLT_Mu9"),
      UseTrackerPt = cms.untracked.bool(True),
      PtCut = cms.untracked.double(25.0),
      EtaCut = cms.untracked.double(2.1),
      IsRelativeIso = cms.untracked.bool(True),
      IsCombinedIso = cms.untracked.bool(False),
      IsoCut03 = cms.untracked.double(0.1),
      MtMin = cms.untracked.double(50.0),
      MtMax = cms.untracked.double(200.0),
      MetMin = cms.untracked.double(-999999.),
      MetMax = cms.untracked.double(999999.),
      AcopCut = cms.untracked.double(2.),

      # Muon quality cuts ->
      DxyCut = cms.untracked.double(0.2),
      NormalizedChi2Cut = cms.untracked.double(10.),
      TrackerHitsCut = cms.untracked.int32(11),
      IsAlsoTrackerMuon = cms.untracked.bool(True),
      
      # To suppress Zmm ->
      PtThrForZ1 = cms.untracked.double(20.0),
      PtThrForZ2 = cms.untracked.double(10.0),
      
      # To further suppress ttbar ->
      EJetMin = cms.untracked.double(40.),
      NJetMax = cms.untracked.int32(999999)
)

# Output
process.load("Configuration.EventContent.EventContent_cff")
process.wmnOutput = cms.OutputModule("PoolOutputModule",
      process.AODSIMEventContent,
      SelectEvents = cms.untracked.PSet(
            SelectEvents = cms.vstring('wmnsel')
      ),
      fileName = cms.untracked.string('root_files/wmnsel.root')
)

# Output histograms
process.TFileService = cms.Service("TFileService", fileName = cms.string('WMuNu_histograms.root') )

# Steering the process
process.wmnsel = cms.Path(process.wmnSelFilter)
process.end = cms.EndPath(process.wmnOutput)
