import FWCore.ParameterSet.Config as cms

# Process, how many events, inout files, ...
process = cms.Process("wmunuplots")
process.maxEvents = cms.untracked.PSet(
      #input = cms.untracked.int32(-1)
      input = cms.untracked.int32(1053)
)
process.source = cms.Source("PoolSource",
      debugVerbosity = cms.untracked.uint32(0),
      debugFlag = cms.untracked.bool(False),
      fileNames = cms.untracked.vstring("file:AOD_with_WCandidates.root")
)

# Debug/info printouts
process.MessageLogger = cms.Service("MessageLogger",
      debugModules = cms.untracked.vstring('corMetWMuNus','selcorMet'),
      cout = cms.untracked.PSet(
            default = cms.untracked.PSet( limit = cms.untracked.int32(-1) ),
            #threshold = cms.untracked.string('INFO')
            threshold = cms.untracked.string('DEBUG')
      ),
      destinations = cms.untracked.vstring('cout')
)

process.selcorMet = cms.EDFilter("WMuNuSelector",
      # Fill Basc Histograms? ->
      plotHistograms = cms.untracked.bool(True),

      # Input collections ->
      MuonTag = cms.untracked.InputTag("muons"),
      METTag = cms.untracked.InputTag("corMetGlobalMuons"),
      JetTag = cms.untracked.InputTag("sisCone5CaloJets"),
      WMuNuCollectionTag = cms.untracked.InputTag("corMetWMuNus:WMuNuCandidates"),

      # Preselection! 
      MuonTrig = cms.untracked.string("HLT_Mu9"),
      PtThrForZ1 = cms.untracked.double(20.0),
      PtThrForZ2 = cms.untracked.double(10.0),
      EJetMin = cms.untracked.double(40.),
      NJetMax = cms.untracked.int32(999999),

      # Main cuts ->
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

      # Select only W-, W+ ( default is all Ws)  
      SelectByCharge=cms.untracked.int32(0)

)
process.selpfMet = cms.EDFilter("WMuNuSelector",
      # Fill Basc Histograms? ->
      plotHistograms = cms.untracked.bool(True),

      # Preselection! 
      MuonTrig = cms.untracked.string("HLT_Mu9"),
      PtThrForZ1 = cms.untracked.double(20.0),
      PtThrForZ2 = cms.untracked.double(10.0),
      EJetMin = cms.untracked.double(40.),
      NJetMax = cms.untracked.int32(999999),

      # Input collections ->
      MuonTag = cms.untracked.InputTag("muons"),
      METTag = cms.untracked.InputTag("pfMet"),
      METIncludesMuons = cms.untracked.bool(True),
      JetTag = cms.untracked.InputTag("sisCone5CaloJets"),
      WMuNuCollectionTag = cms.untracked.InputTag("pfMetWMuNus:WMuNuCandidates"),

      # Main cuts ->
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

      # Select only W-, W+ ( default is all Ws)
      SelectByCharge=cms.untracked.int32(0)

)
process.seltcMet = cms.EDFilter("WMuNuSelector",
      # Fill Basc Histograms? ->
      plotHistograms = cms.untracked.bool(True),

      # Input collections ->
      MuonTag = cms.untracked.InputTag("muons"),
      METTag = cms.untracked.InputTag("tcMet"),
      METIncludesMuons = cms.untracked.bool(True),
      JetTag = cms.untracked.InputTag("sisCone5CaloJets"),
      WMuNuCollectionTag = cms.untracked.InputTag("tcMetWMuNus:WMuNuCandidates"),

      # Preselection! 
      MuonTrig = cms.untracked.string("HLT_Mu9"),
      PtThrForZ1 = cms.untracked.double(20.0),
      PtThrForZ2 = cms.untracked.double(10.0),
      EJetMin = cms.untracked.double(40.),
      NJetMax = cms.untracked.int32(999999),

      # Main cuts ->
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

      # Select only W-, W+ ( default is all Ws)
      SelectByCharge=cms.untracked.int32(0)

)

process.TFileService = cms.Service("TFileService", fileName = cms.string('WMuNuBasicPlots.root') )


# Steering the process
process.path1 = cms.Path(process.selcorMet)
process.path2 = cms.Path(process.selpfMet)
process.path3 = cms.Path(process.seltcMet)



