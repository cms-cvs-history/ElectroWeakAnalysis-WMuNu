import FWCore.ParameterSet.Config as cms

process = cms.Process("WMuNuPATAnalyzer")

process.maxEvents = cms.untracked.PSet(
      input = cms.untracked.int32(-1)
      #input = cms.untracked.int32(100)
)

# Input files (on disk)
import os
dirname = "/data4/Summer08_Wmunu_IDEAL_V11_redigi_v2_PAT/"
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
      cout = cms.untracked.PSet(
            threshold = cms.untracked.string('INFO'),
            default = cms.untracked.PSet(
                  limit = cms.untracked.int32(10) 
            )
      ),
      destinations = cms.untracked.vstring('cout')
)

process.wMuNuPATAnalyzer = cms.EDFilter("WMuNuPATAnalyzer",
    muonTag     = cms.untracked.InputTag("selectedLayer1Muons"),
    jetTag      = cms.untracked.InputTag("selectedLayer1Jets"),
    metTag      = cms.untracked.InputTag("selectedLayer1METs"),
    # Defaults for Selection cuts ->
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

process.TFileService = cms.Service("TFileService", fileName = cms.string('WMuNuPATAnalyzer.root') )

process.p = cms.Path(process.wMuNuPATAnalyzer)

