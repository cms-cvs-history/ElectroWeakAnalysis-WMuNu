import FWCore.ParameterSet.Config as cms

process = cms.Process("wmnana")

process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(-1)
      #input = cms.untracked.int32(100)
)

# Input files (on disk)
import os
dirname = "/data4/Summer08_Wmunu_IDEAL_V11_redigi_v2_AODRED/"
index_first_file = 0
files_per_job = 1

process.source = cms.Source("PoolSource",
      debugVerbosity = cms.untracked.uint32(0),
      debugFlag = cms.untracked.bool(False),
      fileNames = cms.untracked.vstring()
)

index = -1
basenamelist = os.listdir(dirname)
for basename in basenamelist:
      if (len(process.source.fileNames)>=files_per_job): break
      index += 1
      if (index<index_first_file): continue
      this_file = "file:" + dirname + basename
      process.source.fileNames.append(this_file)
      print "File to be used: %s" % (this_file)
print "Number of files to process is %s" % (len(process.source.fileNames))

process.MessageLogger = cms.Service("MessageLogger",
      debugModules = cms.untracked.vstring('wmnAnalyzer'),
      cout = cms.untracked.PSet(
            threshold = cms.untracked.string('INFO'),
            default = cms.untracked.PSet(
                  limit = cms.untracked.int32(10) 
            )
      ),
      destinations = cms.untracked.vstring('cout')
)

process.wmnAnalyzer = cms.EDFilter("WMuNuAnalyzer",
    # Defaults follow...
    #
    # Input collections ->
    MuonTag = cms.untracked.InputTag("muons"),
    METTag = cms.untracked.InputTag("met"),
    JetTag = cms.untracked.InputTag("iterativeCone5CaloJets"),
    UseOnlyGlobalMuons = cms.untracked.bool(True),
    #
    # Main cuts ->
    PtCut = cms.untracked.double(25.0),
    EtaCut = cms.untracked.double(2.1),
    IsRelativeIso = cms.untracked.bool(True),
    IsoCut03 = cms.untracked.double(0.1),
    MassTMin = cms.untracked.double(50.0),
    MassTMax = cms.untracked.double(200.0),
    #
    # To suppress Zmm ->
    PtThrForZCount = cms.untracked.double(20.0),
    #
    # To suppress ttbar ->
    AcopCut = cms.untracked.double(999999.),
    EJetMin = cms.untracked.double(999999.),
    NJetMax = cms.untracked.int32(999999)
)

process.load("Configuration.EventContent.EventContent_cff")

process.wmnOutput = cms.OutputModule("PoolOutputModule",
    process.AODSIMEventContent,
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('wmnana')
    ),
    fileName = cms.untracked.string('/data1/jalcaraz/WMuNuAnalyzer.root')
)

process.TFileService = cms.Service("TFileService", fileName = cms.string('WMuNuAnalyzer.root') )

process.wmnana = cms.Path(process.wmnAnalyzer)
process.end = cms.EndPath(process.wmnOutput)
