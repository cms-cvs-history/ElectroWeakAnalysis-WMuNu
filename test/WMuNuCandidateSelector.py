import FWCore.ParameterSet.Config as cms

# Process, how many events, inout files, ...
process = cms.Process("USER")
process.maxEvents = cms.untracked.PSet(
      #input = cms.untracked.int32(-1)
      input = cms.untracked.int32(10)
)
process.source = cms.Source("PoolSource",
      debugVerbosity = cms.untracked.uint32(0),
      debugFlag = cms.untracked.bool(False),
      fileNames = cms.untracked.vstring("file:/data4/Wmunu-Summer09-MC_31X_V2_preproduction_311-v1/0011/F4C91F77-766D-DE11-981F-00163E1124E7.root")
)

process.load("ElectroWeakAnalysis.WMuNu.WMuNuSelection_cff")

# Debug/info printouts
process.MessageLogger = cms.Service("MessageLogger",
      debugModules = cms.untracked.vstring('corMetWMuNus','selcorMet'),
      cout = cms.untracked.PSet(
            default = cms.untracked.PSet( limit = cms.untracked.int32(10) ),
            threshold = cms.untracked.string('INFO')
            #threshold = cms.untracked.string('DEBUG')
      ),
      destinations = cms.untracked.vstring('cout')
)

#process.TFileService = cms.Service("TFileService", fileName = cms.string('WMuNu.root') )

process.myEventContent = cms.PSet(
      outputCommands = cms.untracked.vstring(
            'keep *'
      )
)

process.wmnOutput = cms.OutputModule("PoolOutputModule",
      #process.AODSIMEventContent,
      process.myEventContent,
      SelectEvents = cms.untracked.PSet(
            SelectEvents = cms.vstring('path')
      ),
      fileName = cms.untracked.string('AOD_with_WCandidates.root')
)

# This Example uses only "corMetGlobalMuons". Modify to run over pf & tc Met
process.path = cms.Path(process.selectCaloMetWMuNus)

process.end = cms.EndPath(process.wmnOutput)


