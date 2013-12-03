from hltgsfeleanalyser_cfg import *

#process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(20) )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        options.dataFile, 
        #'file:/afs/cern.ch/work/t/treis/hlt/gsftracking/CMSSW_6_2_0_patch1/src/HLTrigger/Configuration/test/outputQCD_lowEt_8tev50ns.root'
        #'file:/afs/cern.ch/work/t/treis/hlt/gsftracking/CMSSW_6_2_0_patch1/src/HLTrigger/Configuration/test/outputDY_lowEt_8tev50ns.root'
        #'file:/afs/cern.ch/work/t/treis/hlt/gsftracking/CMSSW_6_2_0_patch1/src/HLTrigger/Configuration/test/outputZskim_lowEt.root'
    ),
    secondaryFileNames = cms.untracked.vstring(),
)

process.HltGsfEleAna = cms.EDAnalyzer('HltGsfEleAnalyser',
    # track collections
    electronCollTag = cms.InputTag('gsfElectrons', '', 'RECO'),
    trgResultsTag = cms.InputTag('TriggerResults', '', 'GSFTEST'),
    trgEventTag = cms.InputTag('hltTriggerSummaryAOD', '', 'GSFTEST'),

    # triggers 
    triggers = cms.untracked.VPSet(
        cms.untracked.PSet(
            triggerName = cms.untracked.string('HLT_Ele25_CaloIdVT'),
            refTriggerName = cms.untracked.string('HLT_Ele25_CaloIdVT'),
            invertBit = cms.untracked.bool(False),
            minEle = cms.untracked.uint32(1),
            minEts = cms.untracked.vdouble(25.),
            tpTriggerName = cms.untracked.string('HLT_Ele27_WP80_v13'),
            tagFilterName = cms.untracked.string('hltEle27WP80TrackIsoFilter'),
        ),
        cms.untracked.PSet(
            triggerName = cms.untracked.string('HLT_Ele25_CaloIdVT_GsfTrkIdT_nC2'),
            refTriggerName = cms.untracked.string('HLT_Ele25_CaloIdVT'),
            invertBit = cms.untracked.bool(False),
            minEle = cms.untracked.uint32(1),
            minEts = cms.untracked.vdouble(25.),
            tpTriggerName = cms.untracked.string('HLT_Ele27_WP80_v13'),
            tagFilterName = cms.untracked.string('hltEle27WP80TrackIsoFilter'),
        ),
        cms.untracked.PSet(
            triggerName = cms.untracked.string('HLT_Ele25_CaloIdVT_GsfTrkIdT_nC3'),
            refTriggerName = cms.untracked.string('HLT_Ele25_CaloIdVT'),
            invertBit = cms.untracked.bool(False),
            minEle = cms.untracked.uint32(1),
            minEts = cms.untracked.vdouble(25.),
            tpTriggerName = cms.untracked.string('HLT_Ele27_WP80_v13'),
            tagFilterName = cms.untracked.string('hltEle27WP80TrackIsoFilter'),
        ),
        cms.untracked.PSet(
            triggerName = cms.untracked.string('HLT_Ele25_CaloIdVT_GsfTrkIdT_nC4'),
            refTriggerName = cms.untracked.string('HLT_Ele25_CaloIdVT'),
            invertBit = cms.untracked.bool(False),
            minEle = cms.untracked.uint32(1),
            minEts = cms.untracked.vdouble(25.),
            tpTriggerName = cms.untracked.string('HLT_Ele27_WP80_v13'),
            tagFilterName = cms.untracked.string('hltEle27WP80TrackIsoFilter'),
        ),
        cms.untracked.PSet(
            triggerName = cms.untracked.string('HLT_Ele25_CaloIdVT_GsfTrkIdT_nC5'),
            refTriggerName = cms.untracked.string('HLT_Ele25_CaloIdVT'),
            invertBit = cms.untracked.bool(False),
            minEle = cms.untracked.uint32(1),
            minEts = cms.untracked.vdouble(25.),
            tpTriggerName = cms.untracked.string('HLT_Ele27_WP80_v13'),
            tagFilterName = cms.untracked.string('hltEle27WP80TrackIsoFilter'),
        ),
        cms.untracked.PSet(
            triggerName = cms.untracked.string('HLT_Ele25_CaloIdVT_GsfTrkIdT_v2'),
            refTriggerName = cms.untracked.string('HLT_Ele25_CaloIdVT'),
            invertBit = cms.untracked.bool(False),
            minEle = cms.untracked.uint32(1),
            minEts = cms.untracked.vdouble(25.),
            tpTriggerName = cms.untracked.string('HLT_Ele27_WP80_v13'),
            tagFilterName = cms.untracked.string('hltEle27WP80TrackIsoFilter'),
        ),
        cms.untracked.PSet(
            triggerName = cms.untracked.string('HLT_Ele25_CaloIdVT_TrkIdT'),
            refTriggerName = cms.untracked.string('HLT_Ele25_CaloIdVT'),
            invertBit = cms.untracked.bool(False),
            minEle = cms.untracked.uint32(1),
            minEts = cms.untracked.vdouble(25.),
            tpTriggerName = cms.untracked.string('HLT_Ele27_WP80_v13'),
            tagFilterName = cms.untracked.string('hltEle27WP80TrackIsoFilter'),
        ),
    ),
)

