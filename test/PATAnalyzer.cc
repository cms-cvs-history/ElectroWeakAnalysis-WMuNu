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
//#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/PFMET.h"

#include "TH1D.h"
#include <map>

#include "DataFormats/Common/interface/View.h"
#include <string>
//
// class declaration
//

class PATAnalyzer : public edm::EDAnalyzer {
   public:
      explicit PATAnalyzer(const edm::ParameterSet&);
      ~PATAnalyzer();


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      // ----------member data ---------------------------

  
  std::map<std::string,TH1D*> histocontainer_; // simple map to contain all histograms. Histograms are booked in the beginJob() method
  
  edm::InputTag eleLabel_;
  edm::InputTag muoLabel_;
  edm::InputTag jetLabel_;
  //edm::InputTag tauLabel_;
  edm::InputTag metLabel_;
  edm::InputTag phoLabel_;
  edm::InputTag pfjetLabel_;
  edm::InputTag pfmetLabel_;
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
PATAnalyzer::PATAnalyzer(const edm::ParameterSet& iConfig):
  histocontainer_(),
  eleLabel_(iConfig.getUntrackedParameter<edm::InputTag>("electronTag")),
  muoLabel_(iConfig.getUntrackedParameter<edm::InputTag>("muonTag")),
  jetLabel_(iConfig.getUntrackedParameter<edm::InputTag>("jetTag")),
  //tauLabel_(iConfig.getUntrackedParameter<edm::InputTag>("tauTag")),
  metLabel_(iConfig.getUntrackedParameter<edm::InputTag>("metTag")),
  phoLabel_(iConfig.getUntrackedParameter<edm::InputTag>("photonTag")),
  pfjetLabel_(iConfig.getUntrackedParameter<edm::InputTag>("pfJetTag")),
  pfmetLabel_(iConfig.getUntrackedParameter<edm::InputTag>("pfMetTag"))

{
   //now do whatever initialization is needed

}


PATAnalyzer::~PATAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
PATAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;


   //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
   // first: get all objects from the event.
   //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}

   edm::Handle<edm::View<pat::Muon> > muonHandle;
   iEvent.getByLabel(muoLabel_,muonHandle);
   edm::View<pat::Muon> muons = *muonHandle;
   
   edm::Handle<edm::View<pat::Jet> > jetHandle;
   iEvent.getByLabel(jetLabel_,jetHandle);
   edm::View<pat::Jet> jets = *jetHandle;

   edm::Handle<edm::View<pat::Electron> > electronHandle;
   iEvent.getByLabel(eleLabel_,electronHandle);
   edm::View<pat::Electron> electrons = *electronHandle;

   edm::Handle<edm::View<pat::MET> > metHandle;
   iEvent.getByLabel(metLabel_,metHandle);
   edm::View<pat::MET> met = *metHandle;

   edm::Handle<edm::View<pat::Photon> > phoHandle;
   iEvent.getByLabel(phoLabel_,phoHandle);
   edm::View<pat::Photon> photons = *phoHandle;

   //edm::Handle<edm::View<pat::Tau> > tauHandle;
   //iEvent.getByLabel(tauLabel_,tauHandle);
   //edm::View<pat::Tau> taus = *tauHandle;

   edm::Handle<edm::View<reco::PFJet> > pfjetHandle;
   iEvent.getByLabel(pfjetLabel_,pfjetHandle);
   edm::View<reco::PFJet> pfjets = *pfjetHandle;

   edm::Handle<edm::View<reco::PFMET> > pfmetHandle;
   iEvent.getByLabel(pfmetLabel_,pfmetHandle);
   edm::View<reco::PFMET> pfmet = *pfmetHandle;

   //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
   // example of a loop over objects... this works identical for all vectors defined above
   //   once you have a jet object you can use all methods defined in the header file 
   // (DataFormats/PatCandidates/interface/Jet.h and equivalent for other objects)
   //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
   size_t njetscounter=0;
   //edm::View<pat::Jet>::const_iterator firstjet;
   //edm::View<pat::Jet>::const_iterator lastjet;
   //edm::View<pat::Jet>::const_iterator jet_iter;
   //for(jet_iter = firstjet; jet_iter!=lastjet; ++jet_iter){
   for(size_t ij = 0; ij<jets.size(); ++ij) {
     const pat::Jet& jet = jets[ij];
     //if(jet_iter->pt()>50)
     if(jet.pt()>15) njetscounter++;
   }

   size_t npfjetscounter=0;
   for(size_t ij = 0; ij<pfjets.size(); ++ij) {
     const reco::PFJet& jet = pfjets[ij];
     if(jet.pt()>15) npfjetscounter++;
   }

   //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
   //histocontainer_ is of type std::map<edm::View, TH1D*>. This means you can use it with this syntax:
   // histocontainer_["histname"]->Fill(x); 
   // histocontainer_["histname"]->Draw(); 
   // etc, etc. Essentially you use the histname string to look up a pointer to a TH1D* 
   // which you can do everything to you would normally do in ROOT.
   //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
   
   histocontainer_["njets"]->Fill(njetscounter);
   histocontainer_["npfjets"]->Fill(npfjetscounter);
   //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
   // for the other objects just quickly book the multiplicity. Again, just use the same infrastructure as for jets if you want to loop over them.
   //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
   histocontainer_["nelectrons"]->Fill(electrons.size());
   histocontainer_["nphotons"]->Fill(photons.size());
   histocontainer_["nmuons"]->Fill(muons.size());
   histocontainer_["met"]->Fill(met[0].et());
   histocontainer_["pfmet"]->Fill(pfmet[0].et());
   //histocontainer_["ntaus"]->Fill(taus.size());
}
// ------------ method called once each job just before starting event loop  ------------
void 
PATAnalyzer::beginJob(const edm::EventSetup&)
{
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
  // define some histograms using the framework tfileservice. Define the output file name in your .cfg.
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
  edm::Service<TFileService> fs;
  
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
  //histocontainer_ is of type std::map<std::string, TH1D*>. This means you can use it with this syntax:
  // histocontainer_["histname"]->Fill(x); 
  // histocontainer_["histname"]->Draw(); 
  // etc, etc. Essentially you use the histname string to look up a pointer to a TH1D* 
  // which you can do everything to you would normally do in ROOT.
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}

  
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}
  // here we book new histograms:
  //{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}

  histocontainer_["njets"]=fs->make<TH1D>("njets","jet multiplicity for jets with p_{T} > 15 GeV/c",10,-0.5,9.5);
  histocontainer_["npfjets"]=fs->make<TH1D>("npfjets","PFlow jet multiplicity for jets with p_{T} > 15 GeV/c",10,-0.5,9.5);

  histocontainer_["nelectrons"]=fs->make<TH1D>("nelectrons","electron multiplicity",10,-0.5,9.5);
  //histocontainer_["ntaus"]=fs->make<TH1D>("ntaus","tau multiplicity",10,-0.5,9.5);
  histocontainer_["nphotons"]=fs->make<TH1D>("nphotons","photon multiplicity",10,-0.5,9.5);
  histocontainer_["nmuons"]=fs->make<TH1D>("nmuons","muon multiplicity",10,-0.5,9.5);
  histocontainer_["met"]=fs->make<TH1D>("met","MET [GeV]",100,0.,200.);
  histocontainer_["pfmet"]=fs->make<TH1D>("pfmet","PFlow MET [GeV]",100,0.,200.);

}

// ------------ method called once each job just after ending the event loop  ------------
void 
PATAnalyzer::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(PATAnalyzer);
