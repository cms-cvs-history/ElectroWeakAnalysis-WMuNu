//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "TH1D.h"
#include "TH2D.h"
#include <map>

#include "DataFormats/Common/interface/View.h"
#include <string>
//
// class declaration
//

class WMuNuPATAnalyzer : public edm::EDAnalyzer {
   public:
      explicit WMuNuPATAnalyzer(const edm::ParameterSet&);
      ~WMuNuPATAnalyzer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      edm::InputTag muoLabel_;
      edm::InputTag jetLabel_;
      edm::InputTag metLabel_;

      bool useOnlyGlobalMuons_;
      double ptCut_;
      double etaCut_;
      bool isRelativeIso_;
      double isoCut03_;
      double massTMin_;
      double massTMax_;
      double ptThrForZCount_;
      double acopCut_;
      double eJetMin_;
      int nJetMax_;

      std::map<std::string,TH1D*> h1_;
      std::map<std::string,TH2D*> h2_;
  
      unsigned int numberOfEvents;
      unsigned int numberOfMuons;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
WMuNuPATAnalyzer::WMuNuPATAnalyzer(const edm::ParameterSet& iConfig):
  muoLabel_(iConfig.getUntrackedParameter<edm::InputTag>("muonTag")),
  jetLabel_(iConfig.getUntrackedParameter<edm::InputTag>("jetTag")),
  metLabel_(iConfig.getUntrackedParameter<edm::InputTag>("metTag")),
  useOnlyGlobalMuons_(iConfig.getUntrackedParameter<bool>("UseOnlyGlobalMuons", true)),
  ptCut_(iConfig.getUntrackedParameter<double>("PtCut", 25.)),
  etaCut_(iConfig.getUntrackedParameter<double>("EtaCut", 2.1)),
  isRelativeIso_(iConfig.getUntrackedParameter<bool>("IsRelativeIso", true)),
  isoCut03_(iConfig.getUntrackedParameter<double>("IsoCut03", 0.1)),
  massTMin_(iConfig.getUntrackedParameter<double>("MassTMin", 50.)),
  massTMax_(iConfig.getUntrackedParameter<double>("MassTMax", 200.)),
  ptThrForZCount_(iConfig.getUntrackedParameter<double>("PtThrForZCount", 20.)),
  acopCut_(iConfig.getUntrackedParameter<double>("AcopCut", 999999.)),
  eJetMin_(iConfig.getUntrackedParameter<double>("EJetMin", 999999.)),
  nJetMax_(iConfig.getUntrackedParameter<int>("NJetMax", 999999)),
  h1_(), h2_()
{
  LogDebug("WMuNuPATAnalyzer")<<" WMuNuPATAnalyzer constructor called";
}


WMuNuPATAnalyzer::~WMuNuPATAnalyzer()
{
}


void WMuNuPATAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace std;
   using namespace edm;
   using namespace pat;

///////////////////////////////

   numberOfEvents++;

   double pt_sel = -999999.;
   double eta_sel = 999999.;
   double acop_sel = 3.15;
   double massT_sel = 0.;
   double iso_sel = 999999.;
   double isoN_sel = 999999.;

   bool event_sel = false;
  
   double met_px = 0.;
   double met_py = 0.;

   // Get the Muon collection
   Handle<View<Muon> > muonCollection;
   if (!iEvent.getByLabel(muoLabel_,muonCollection)) {
      LogTrace("") << ">>> Muon collection does not exist !!!";
      return;
   }
   
   LogTrace("Analyzer")<<"Reconstructed Muon tracks: " << muonCollection->size() << endl;
   numberOfMuons += muonCollection->size();
   for(size_t im = 0; im<muonCollection->size(); ++im) {
      const Muon& mu = muonCollection->at(im);
      //if (useOnlyGlobalMuons_ && !mu.isGlobalMuon()) continue;
      met_px -= mu.px();
      met_py -= mu.py();
   }

  // Get the MET collection from the event
   Handle<View<MET> > metCollection;
   if (!iEvent.getByLabel(metLabel_,metCollection)) {
      LogTrace("") << ">>> MET collection does not exist !!!";
      return;
   }

   const MET& caloMET = metCollection->at(0);
   LogTrace("") << ">>> CaloMET_et, CaloMET_py, CaloMET_py= " << caloMET.et() << ", " << caloMET.px() << ", " << caloMET.py();;
   met_px += caloMET.px();
   met_py += caloMET.py();
   double met_et = sqrt(met_px*met_px+met_py*met_py);
   h1_["MET"]->Fill(met_et);

  // Get the Jet collection from the event
   Handle<View<Jet> > jetCollection;
   if (!iEvent.getByLabel(jetLabel_,jetCollection)) {
      LogTrace("") << ">>> JET collection does not exist !!!";
      return;
   }

   int njets = 0;
   for(size_t ij = 0; ij<jetCollection->size(); ++ij) {
         const Jet& jet = jetCollection->at(ij);
         if (jet.et()>eJetMin_) njets++;
   }
   h1_["NJETS"]->Fill(njets);
   LogTrace("") << ">>> Number of jets " << jetCollection->size() << "; above " << eJetMin_ << " GeV: " << njets;

   unsigned int nmuons = 0;
   unsigned int nmuonsForZ = 0;

   h1_["NMU"]->Fill(muonCollection->size());
   float max_pt = -9999.;
   for (unsigned int i=0; i<muonCollection->size(); i++) {
      bool muon_sel = true;

      const Muon& mu = muonCollection->at(i);
      //if (useOnlyGlobalMuons_ && !mu.isGlobalMuon()) continue;
      LogTrace("") << "> Processing (global) muon number " << i << "...";
// pt
      double pt = mu.pt();
      h1_["PTMU"]->Fill(pt);
      LogTrace("") << "\t... pt= " << pt << " GeV";

      if (pt>ptThrForZCount_) nmuonsForZ++;
      if (pt<ptCut_) muon_sel = false;
// eta
      double eta = mu.eta();
      h1_["ETAMU"]->Fill(eta);
      LogTrace("") << "\t... eta= " << eta;
      if (fabs(eta)>etaCut_) muon_sel = false;

// acoplanarity
      Geom::Phi<double> deltaphi(mu.phi()-atan2(met_py,met_px));
      double acop = deltaphi.value();
      if (acop<0) acop = - acop;
      acop = M_PI - acop;
      h1_["ACOP"]->Fill(acop);
      LogTrace("") << "\t... acop= " << acop;
      if (acop>acopCut_) muon_sel = false;

// transverse mass
      double w_et = mu.pt() + met_et;
      double w_px = mu.px() + met_px;
      double w_py = mu.py() + met_py;
      double massT = w_et*w_et - w_px*w_px - w_py*w_py;
      massT = (massT>0) ? sqrt(massT) : 0;
      h1_["TMASS"]->Fill(massT);
      LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
      LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";
      if (massT<massTMin_) muon_sel = false;
      if (massT>massTMax_) muon_sel = false;

// Isolation
      //double ptsum = mu.trackIso();
      double ptsum = mu.isolationR03().sumPt;
      h1_["PTSUM"]->Fill(ptsum);
      h1_["PTSUMN"]->Fill(ptsum/pt);
      h2_["TMASS_PTSUM"]->Fill(ptsum,massT);
      LogTrace("") << "\t... Isol, Track pt= " << mu.pt() << " GeV, " << " ptsum = " << ptsum;
      if (ptsum/pt > isoCut03_) muon_sel = false;

      if (muon_sel) {
        nmuons++;
        if (pt > max_pt) {  //and identify the highest pt muon
            max_pt = pt;
            pt_sel = pt;
            eta_sel = eta;
            acop_sel = acop;
            massT_sel = massT;
            iso_sel = ptsum;
            isoN_sel = ptsum/pt;
        }
      }
   }

   LogTrace("") << "> Number of muons for W= " << nmuons;
   LogTrace("") << "> Muon counts to reject Z= " << nmuonsForZ;
   LogTrace("") << "> Jets above threshold =" << njets;
   if (nmuons>0 && nmuonsForZ<2 && njets<=nJetMax_) {
            event_sel = true;
            LogTrace("") << ">>>> Event SELECTED!!!";
   } else {
            LogTrace("") << ">>>> Event REJECTED...";
   }

   if (event_sel == true) {
// Fill histograms for selected events
       h1_["NMU_SEL"]->Fill(nmuons);
       h1_["MET_SEL"]->Fill(met_et); 
       h1_["NJETS_SEL"]->Fill(njets);

// only the combination with highest pt
       h1_["PTMU_SEL"]->Fill(pt_sel);
       h1_["ETAMU_SEL"]->Fill(eta_sel);
       h1_["ACOP_SEL"]->Fill(acop_sel);
       h1_["TMASS_SEL"]->Fill(massT_sel);
       h1_["PTSUM_SEL"]->Fill(iso_sel);
       h1_["PTSUMN_SEL"]->Fill(isoN_sel);
       h2_["TMASS_PTSUM_SEL"]->Fill(iso_sel,massT_sel);
   }

}
// ------------ method called once each job just before starting event loop  ------------
void WMuNuPATAnalyzer::beginJob(const edm::EventSetup&) {
  // define some histograms using the framework tfileservice. Define the output file name in your .cfg.
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
  edm::Service<TFileService> fs;
  
  numberOfEvents = 0;
  numberOfMuons = 0;

  h1_["NMU"]  = fs->make<TH1D>("NMU","Nb. muons in the event",10,0.,10.);
  h1_["PTMU"] = fs->make<TH1D>("PTMU","Pt mu",100,0.,100.);
  h1_["ETAMU"] = fs->make<TH1D>("ETAMU","Eta mu",50,-2.5,2.5);
  h1_["MET"] = fs->make<TH1D>("MET","Missing Transverse Energy (GeV)", 100,0.,200.);
  h1_["TMASS"] = fs->make<TH1D>("TMASS","Rec. Transverse Mass (GeV)",150,0.,300.);
  h1_["ACOP"] = fs->make<TH1D>("ACOP","Mu-MET acoplanarity",50,0.,M_PI);
  h1_["NJETS"] = fs->make<TH1D>("NJETS","njets",25,0.,25.);
  h1_["PTSUM"] = fs->make<TH1D>("PTSUM","Sum pT (GeV)",100,0.,50.);
  h1_["PTSUMN"] = fs->make<TH1D>("PTSUMN","Sum pT/pT",100,0.,50.);
  h2_["TMASS_PTSUM"] = fs->make<TH2D>("TMASS_PTSUM","Rec. Transverse Mass (GeV) vs Sum pT (GeV)",100,0.,50.,150,0.,300.);

  h1_["NMU_SEL"] = fs->make<TH1D>("NMU_SEL","Nb. selected muons",10,0.,10.);
  h1_["PTMU_SEL"] = fs->make<TH1D>("PTMU_SEL","Pt mu",100,0.,100.);
  h1_["ETAMU_SEL"] = fs->make<TH1D>("ETAMU_SEL","Eta mu",50,-2.5,2.5);
  h1_["MET_SEL"] = fs->make<TH1D>("MET_SEL","Missing Transverse Energy (GeV)", 100,0.,200.);
  h1_["TMASS_SEL"] = fs->make<TH1D>("TMASS_SEL","Rec. Transverse Mass (GeV)",150,0.,300.);
  h1_["ACOP_SEL"] = fs->make<TH1D>("ACOP_SEL","Mu-MET acoplanarity",50,0.,M_PI);
  h1_["NJETS_SEL"] = fs->make<TH1D>("NJETS_SEL","njets",25,0.,25.);
  h1_["PTSUM_SEL"] = fs->make<TH1D>("PTSUM_SEL","Sum pT (GeV)",100,0.,50.);
  h1_["PTSUMN_SEL"] = fs->make<TH1D>("PTSUMN_SEL","Sum pT/pT ",100,0.,2.5);
  h2_["TMASS_PTSUM_SEL"] = fs->make<TH2D>("TMASS_PTSUM_SEL","Rec. Transverse Mass (GeV) vs Sum pT (GeV)",100,0.,50.,150,0.,300.);

}

// ------------ method called once each job just after ending the event loop  ------------
void WMuNuPATAnalyzer::endJob() {
  edm::LogVerbatim("") << "WMuNuPATAnalyzer>>> FINAL PRINTOUTS -> BEGIN";
  edm::LogVerbatim("") << "WMuNuPATAnalyzer>>> Number of analyzed events= " << numberOfEvents;
  edm::LogVerbatim("") << "WMuNuPATAnalyzer>>> Number of analyzed muons= " << numberOfMuons;
  edm::LogVerbatim("") << "WMuNuPATAnalyzer>>> FINAL PRINTOUTS -> END";
}

//define this as a plug-in
DEFINE_FWK_MODULE(WMuNuPATAnalyzer);
