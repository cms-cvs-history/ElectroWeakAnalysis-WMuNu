//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

#include "FWCore/Framework/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
  
#include "DataFormats/Common/interface/View.h"
#include <string>
//
// class declaration
//

class WMuNuPATSelector : public edm::EDFilter {
   public:
      WMuNuPATSelector (const edm::ParameterSet &);
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void beginJob(const edm::EventSetup&);
      virtual void endJob();
      double my_weight(double eta, const std::string mode);

   private:
      edm::InputTag genLabel_;
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

      unsigned int nall;
      unsigned int ngen;
      unsigned int nrec;
      unsigned int niso;
      unsigned int nhlt;
      unsigned int nmt;
      unsigned int ntop;
      unsigned int nsel;
      double wiso;
      double whlt;
      double wrec;
      double wall;
};

using namespace edm;
using namespace std;
using namespace pat;

WMuNuPATSelector::WMuNuPATSelector(const edm::ParameterSet& iConfig):
  genLabel_(iConfig.getUntrackedParameter<edm::InputTag>("genTag")),
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
  nJetMax_(iConfig.getUntrackedParameter<int>("NJetMax", 999999))
{}


void WMuNuPATSelector::beginJob(const edm::EventSetup &) {
      nall = 0;
      ngen = 0;
      nrec = 0;
      niso = 0;
      nhlt = 0;
      nmt = 0;
      ntop = 0;
      nsel = 0;
      wiso = 0.;
      whlt = 0.;
      wrec = 0.;
      wall = 0.;
}

void WMuNuPATSelector::endJob() {
      LogError("") << "nall= " << nall << endl;
      double all = nall;
      double egen = ngen/all;
      double erec = nrec/all;
      double eiso = niso/all;
      double ehlt = nhlt/all;
      double emt  = nmt /all;
      double etop = ntop/all;
      double esel = nsel/all;
      LogError("") << "ngen= " << ngen << ", " << egen*100 <<" +/- "<<sqrt(egen*(1-egen)/all)*100<<" %, to previous step: " << egen*100 <<"%";
      LogError("") << "nrec= " << nrec << ", " << erec*100 <<" +/- "<<sqrt(erec*(1-erec)/all)*100<<" %, to previous step: " << erec/egen*100 <<"%";
      LogError("") << "niso= " << niso << ", " << eiso*100 <<" +/- "<<sqrt(eiso*(1-eiso)/all)*100<<" %, to previous step: " << eiso/erec*100 <<"%";
      LogError("") << "nhlt= " << nhlt << ", " << ehlt*100 <<" +/- "<<sqrt(ehlt*(1-ehlt)/all)*100<< " %, to previous step: " << ehlt/eiso*100 <<"%";
      LogError("") << "nmt = " << nmt  << ", " << emt*100 <<" +/- "<<sqrt(emt*(1-emt)/all)*100<< "%, to previous step: " << emt/ehlt*100 <<"%";
      LogError("") << "ntop= " << ntop << ", " << etop*100 <<" +/- "<<sqrt(etop*(1-etop)/all)*100<<  "%, to previous step: " << etop/emt*100 <<"%";
      LogError("") << "nsel= " << nsel << ", " << esel*100 <<" +/- "<<sqrt(esel*(1-esel)/all)*100<< "%, to previous step: " << esel/etop*100 <<"%";
      LogError("") << "hlt/iso= " << ehlt/eiso*100<<" +/- "<<sqrt(ehlt/eiso*(1-ehlt/eiso)/niso)*100<< "%, from data: " << whlt/niso*100<<" +/- "<<sqrt(whlt/niso*(1-whlt/niso)/niso)*100<<"%";
      LogError("") << "iso/rec= " << eiso/erec*100 <<" +/- "<<sqrt(eiso/erec*(1-eiso/erec)/nrec)*100<< "%, from data: " << wiso/nrec*100 <<" +/- "<<sqrt(wiso/nrec*(1-wiso/nrec)/nrec)*100<<"%";
      LogError("") << "rec/gen= " << erec/egen*100 <<" +/- "<<sqrt(erec/egen*(1-erec/egen)/ngen)*100<< "%, from data: " << wrec/ngen*100 <<" +/- "<<sqrt(wrec/ngen*(1-wrec/ngen)/ngen)*100<<"%";
      LogError("") << "all(rec-iso-hlt)/gen= " << ehlt/egen*100 <<" +/- "<<sqrt(ehlt/egen*(1-ehlt/egen)/ngen)*100<<  "%, from data: " << wall/ngen*100<< " +/- "<<sqrt(wall/ngen*(1-wall/ngen)/ngen)*100<<"%";
}

bool WMuNuPATSelector::filter (edm::Event & ev, const edm::EventSetup &) {
      double met_px = 0.;
      double met_py = 0.;

      // Get the Muon collection
      Handle<View<Muon> > muonCollection;
      if (!ev.getByLabel(muoLabel_,muonCollection)) {
            LogError("") << ">>> Muon collection does not exist !!!";
            return false;
      }
   
      /* Already accounted for in MET
      for(size_t im = 0; im<muonCollection->size(); ++im) {
            const Muon& mu = muonCollection->at(im);
            //if (useOnlyGlobalMuons_ && !mu.isGlobalMuon()) continue;
            met_px -= mu.px();
            met_py -= mu.py();
      }
      */

      // Get the MET collection from the event
      Handle<View<MET> > metCollection;
      if (!ev.getByLabel(metLabel_,metCollection)) {
            LogError("") << ">>> MET collection does not exist !!!";
            return false;
      }

      const MET& caloMET = metCollection->at(0);
      LogTrace("") << ">>> CaloMET_et, CaloMET_py, CaloMET_py= " << caloMET.et() << ", " << caloMET.px() << ", " << caloMET.py();;
      met_px += caloMET.px();
      met_py += caloMET.py();
      double met_et = sqrt(met_px*met_px+met_py*met_py);

      // Get the Jet collection from the event
      Handle<View<Jet> > jetCollection;
      if (!ev.getByLabel(jetLabel_,jetCollection)) {
            LogError("") << ">>> JET collection does not exist !!!";
            return false;
      }

      int njets = 0;
      for(size_t ij = 0; ij<jetCollection->size(); ++ij) {
            const Jet& jet = jetCollection->at(ij);
            if (jet.et()>eJetMin_) njets++;
      }

      float etagen, ptgen;
      bool genfound = false;

      Handle<reco::GenParticleCollection> genParticles;
      if (!ev.getByLabel(genLabel_, genParticles)) {
            cout<<"genParticles dont exist!!!"<<endl;
            return false;
      }
      nall++;

      for(size_t i = 0; i < genParticles->size(); ++ i) {
            const reco::GenParticle& p = (*genParticles)[i];
            int id = p.pdgId();      
            int st = p.status();

            if (st==3) continue;
            if (abs(id)!=13) continue;

            int nmothers = p.numberOfMothers();
            //printf("Boson Id %d numberOfMothers %d\n", id, nmothers);
            if (nmothers!=1) continue;
            const reco::Candidate * mom = p.mother();
            int momId=mom->pdgId();

            while(abs(momId)==13 && mom->numberOfMothers()==1){
                  mom = mom->mother();  
                  momId=mom->pdgId();
            }

            if(abs(momId)!=24) continue;
	      //To select only W+ or W-: 
	      // if(momId!=-24) continue;

	      ptgen = p.pt();     
            etagen =p.eta(); 

            ptgen = p.pt();     
            etagen =p.eta();     
            if (ptgen<25 || fabs(etagen)>2.0) continue;    
            genfound = true;               

            break;
      }

      if (!genfound) return false;
      
      ngen++;
      double rec_weight = my_weight(etagen,"REC");
      if (rec_weight>0) wrec += rec_weight;
      double all_weight = my_weight(etagen,"");
      if (all_weight>0) wall += all_weight;


      Handle<edm::TriggerResults> triggerResults;
      edm::TriggerNames trigNames;
      if (!ev.getByLabel(InputTag("TriggerResults::HLT"), triggerResults)) {
            LogError("") << ">>> TRIGGER collection does not exist !!!";
            return false;
      }
      trigNames.init(*triggerResults);
      bool fired = false;

      /*
      for (unsigned int i=0; i<triggerResults->size(); i++) {
            if (triggerResults->accept(i)) {
                  LogError("") << "Accept by: " << i << ", Trigger: " << trigNames.triggerName(i);
            }
      }
      */

      int itrig1 = trigNames.triggerIndex("HLT_Mu15");
      if (triggerResults->accept(itrig1)) fired = true;

      unsigned int nmuons = 0;
      unsigned int nmuonsForZ = 0;
      for (unsigned int i=0; i<muonCollection->size(); i++) {
            const Muon& mu = muonCollection->at(i);
            //if (useOnlyGlobalMuons_ && !mu.isGlobalMuon()) continue;
            if (mu.innerTrack().isNull()) continue;
            const reco::Track& tk = *(mu.innerTrack());
            LogTrace("") << "> Processing muon number " << i << "...";
            double pt = tk.pt();
            if (pt>ptThrForZCount_) nmuonsForZ++;
            LogTrace("") << "\t... pt= " << pt << " GeV";
  //          if (pt<ptCut_) continue;
            double eta = tk.eta();
            LogTrace("") << "\t... eta= " << eta;
//            if (fabs(eta)>etaCut_) continue;


            //if(tk.charge()!=1) continue;

            if (pt<ptCut_-2*tk.ptError()) continue;
            if (fabs(eta)>etaCut_+2*tk.ptError()) continue;
            LogTrace("") << "\t... pt Resolution = " << tk.ptError() << ", etaResolution = " << tk.etaError();
            nrec++;

            double iso_weight = my_weight(eta,"ISO");
            if (iso_weight>0) wiso += iso_weight;

            bool iso = false;
            double ptsum = mu.isolationR03().sumPt;
            if (ptsum/pt<isoCut03_){iso=true;};
 
            LogTrace("") << "\t... isolated? " << iso;
            if (!iso) continue;
            niso++;

            double hlt_weight = my_weight(eta,"HLT");
            if (hlt_weight>0) whlt += hlt_weight;

            if (!fired) continue;
            nhlt++;

            double w_et = tk.pt() + met_et;
            double w_px = tk.px() + met_px;
            double w_py = tk.py() + met_py;
            double massT = w_et*w_et - w_px*w_px - w_py*w_py;
            massT = (massT>0) ? sqrt(massT) : 0;
            LogTrace("") << "\t... W_et, W_px, W_py= " << w_et << ", " << w_px << ", " << w_py << " GeV";
            LogTrace("") << "\t... Invariant transverse mass= " << massT << " GeV";
            if (massT<massTMin_) continue;
            if (massT>massTMax_) continue;
            nmt++;

            LogTrace("") << ">>> Number of jets " << jetCollection->size() << "; above " << eJetMin_ << " GeV: " << njets;
            LogTrace("") << ">>> Number of jets above " << eJetMin_ << " GeV: " << njets;
            if (njets>nJetMax_) return false;

            Geom::Phi<double> deltaphi(tk.phi()-atan2(met_py,met_px));
            double acop = deltaphi.value();
            if (acop<0) acop = - acop;
            acop = M_PI - acop;
            LogTrace("") << "\t... acop= " << acop;
            if (acop>acopCut_) continue;
            ntop++;
            nmuons++;
      }
      LogTrace("") << "> Muon counts to reject Z= " << nmuonsForZ;
      if (nmuonsForZ>=2) {
            LogTrace("") << ">>>> Event REJECTED";
            return false;
      }
      LogTrace("") << "> Number of muons for W= " << nmuons;
      if (nmuons<1) {
            LogTrace("") << ">>>> Event REJECTED";
            return false;
      }
      LogTrace("") << ">>>> Event SELECTED!!!";
      nsel++;

      return true;

}

double WMuNuPATSelector::my_weight(double eta, const std::string mode) {
      // ISO
      const double iso_min = -2.0; 
      const double iso_max = +2.0; 
      const int iso_bins = 20;


// New Isolation!!!!!!! (ptsum/pt)
const double eff_iso[iso_bins] ={ 0.984649 ,  0.969325 ,  0.974406 ,  0.986864 ,  0.961353 ,  0.964286 ,  0.967647 ,  0.974965 ,  0.98329 ,  0.972603 ,  0.965007 ,  0.956989 ,  0.978231 ,  0.968839 ,  0.971125 ,  0.968 ,  0.976271 ,  0.985428 ,  0.984344 ,  0.971963 };



      // HLT
      const double hlt_min = -2.1;
      const double hlt_max = +2.1;
      const int hlt_bins = 50;

 const double eff_hlt[hlt_bins] = { 0.740458012, 0.900709212, 0.918918908, 0.947368443, 0.915032685, 0.909090936, 0.901639342, 0.958333313, 0.939226508, 0.982758641, 0.866666675, 0.849056602, 0.896551728, 0.882113814, 0.868778288, 0.918552041, 0.916334689, 0.895161271, 0.93065691 , 0.914979756, 0.893700778, 0.913513541, 0.903669715, 0.946043193, 0.903100789, 0.921146929, 0.944852948, 0.941908717, 0.919831216, 0.92125982 , 0.954166651, 0.920000017, 0.88429755 , 0.929906547, 0.907834113, 0.876712322, 0.936440706, 0.864000022, 0.842857122, 0.85446012 , 0.945652187, 0.934343457, 0.949748755, 0.908045948, 0.942708313, 0.932584286, 0.886486471, 0.890173435, 0.90322578 , 0.744827569};





     // Fake values for REC as of today
      const double rec_min = -2.1;
      const double rec_max = +2.1;
      const int rec_bins = 50;
  


      const double eff_rec[rec_bins] = { 0.976272, 0.979553, 0.982398, 0.977702, 0.968152, 0.980511, 0.98003, 0.979629, 0.985716, 0.989284, 0.980755, 0.983586, 0.988282, 0.986041, 0.991525, 0.982173, 0.992052, 0.993635, 0.992547, 0.992847, 0.992071, 0.961635, 0.982511, 0.991947, 0.970049, 0.97124, 0.992011, 0.979631, 0.9573, 0.990328, 0.992836, 0.991493, 0.991128, 0.988691, 0.974209, 0.990908, 0.985435, 0.985698, 0.983657, 0.980375, 0.991139, 0.986679, 0.98006, 0.977969, 0.979052, 0.968776, 0.975388, 0.982799, 0.980274, 0.976611};
      const double eff_sta[rec_bins] = { 0.983005, 0.985753, 0.988546, 0.987125, 0.978763, 0.990595, 0.991229, 0.992798, 0.996607, 0.999549, 0.994675, 0.993563, 0.99636 , 0.995803, 0.996025, 0.986317, 0.995953, 0.996802, 0.996158, 0.99696 , 0.997018, 0.96867 , 0.986946, 0.996645, 0.996869, 0.996446, 0.996218, 0.984129, 0.964356, 0.995571, 0.99659 , 0.995412, 0.995965, 0.994154, 0.979378, 0.995491, 0.994966, 0.995404, 0.992672, 0.993381, 0.999444, 0.995852, 0.990442, 0.990121, 0.987173, 0.977185, 0.982735, 0.988271, 0.985356, 0.982167};
      const double eff_trk[rec_bins] = { 0.991292, 0.992328, 0.992554, 0.989253, 0.988361, 0.98955 , 0.98833 , 0.98674 , 0.989074, 0.989765, 0.985971, 0.989958, 0.991892, 0.990197, 0.995483, 0.995798, 0.996083, 0.996821, 0.996375, 0.995874, 0.995038, 0.992738, 0.995477, 0.995312, 0.973095, 0.974705, 0.995749, 0.99543 , 0.992653, 0.994732, 0.996261, 0.996034, 0.995143, 0.994505, 0.994721, 0.995396, 0.990389, 0.990217, 0.990919, 0.986907, 0.991761, 0.990754, 0.989526, 0.987541, 0.991423, 0.989434, 0.990578, 0.993657, 0.993631, 0.992817};
      const double eff_mat[rec_bins] = { 0.998088, 0.998979, 0.999314, 0.998753, 0.998534, 0.99977 , 0.999843, 0.999809, 0.999927, 0.999964, 1       , 1   , 1       , 1       , 1       , 1       , 1       , 0.999972, 1       , 1       , 1       , 1       , 1       , 0.999973, 0.999972, 1       , 1       , 1      , 1       , 0.999973, 0.999972, 1       , 1       , 1       , 0.99997 , 1       , 1       , 0.999968, 1       , 1       , 0.999928, 0.999964, 0.999771, 0.999807, 0.999479, 0.997169, 0.99702 , 0.999179, 0.998106, 0.997866};



      double eff = 1.;



 if (mode=="ISO") {

            // Iso
            int iso_bin = int((eta-iso_min)/(iso_max-iso_min)*iso_bins);
            if (iso_bin<0) iso_bin = 0;
            if (iso_bin>=iso_bins) iso_bin = iso_bins-1;
            eff = eff_iso[iso_bin];

      } else if (mode=="HLT") {

            // HLT
            int hlt_bin = int((eta-hlt_min)/(hlt_max-hlt_min)*hlt_bins);
            if (hlt_bin<0) hlt_bin = 0;
            if (hlt_bin>=hlt_bins) hlt_bin = hlt_bins-1;
            eff = eff_hlt[hlt_bin];

      } else if (mode=="REC") {

            // Rec
            int rec_bin = int((eta-rec_min)/(rec_max-rec_min)*rec_bins);
            if (rec_bin<0) rec_bin = 0;
            if (rec_bin>=rec_bins) rec_bin = rec_bins-1;
            //eff = eff_rec[rec_bin];
            eff = eff_sta[rec_bin]*eff_trk[rec_bin]*eff_mat[rec_bin];

      } else {

            // All
            int iso_bin = int((eta-iso_min)/(iso_max-iso_min)*iso_bins);
            if (iso_bin<0) iso_bin = 0;
            if (iso_bin>=iso_bins) iso_bin = iso_bins-1;
            int hlt_bin = int((eta-hlt_min)/(hlt_max-hlt_min)*hlt_bins);
            if (hlt_bin<0) hlt_bin = 0;
            if (hlt_bin>=hlt_bins) hlt_bin = hlt_bins-1;
            int rec_bin = int((eta-rec_min)/(rec_max-rec_min)*rec_bins);
            if (rec_bin<0) rec_bin = 0;
            if (rec_bin>=rec_bins) rec_bin = rec_bins-1;
            eff = eff_iso[iso_bin] * eff_hlt[hlt_bin] * eff_rec[rec_bin];

      }

      return eff;
      
}

//define this as a plug-in
DEFINE_FWK_MODULE(WMuNuPATSelector);
