// 
// Class: L1ITMuTextDumper
//
// Info: Just a simple text dumper for manual debugging
//
// Author: L. Gray (FNAL)
//

#include <memory>
#include <map>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/GeometryTranslator.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"

#include "L1Trigger/L1IntegratedMuonTrigger/interface/SubsystemCollectorFactory.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

using namespace L1ITMu;

typedef edm::ParameterSet PSet;

class L1ITMuTextDumper : public edm::EDAnalyzer {
public:
  L1ITMuTextDumper(const PSet&);
  ~L1ITMuTextDumper() {}

  void analyze(const edm::Event&, const edm::EventSetup&);  
private:
  bool _dogen;
  edm::InputTag _geninput;
  std::vector<edm::InputTag> _tpinputs;
};

L1ITMuTextDumper::L1ITMuTextDumper(const PSet& p) {
  if( (_dogen = p.getUntrackedParameter<bool>("doGen",false)) ) {
    _geninput = p.getUntrackedParameter<edm::InputTag>("genSrc");
  }
  _tpinputs = p.getParameter<std::vector<edm::InputTag> >("primitiveSrcs");
}

void L1ITMuTextDumper::analyze(const edm::Event& ev, 
			       const edm::EventSetup& es) {
  //dump the generated muons in the event (if requested)
  if( _dogen ) {
    edm::Handle<reco::GenParticleCollection> genps;
    ev.getByLabel(_geninput,genps);
    auto bgen = genps->cbegin();
    auto egen = genps->cend();
    for( ; bgen != egen; ++bgen ) {
      if( std::abs(bgen->pdgId()) == 13 ) {
	std::cout << "Particle Type: " << bgen->pdgId() 
		  << " pt: " << bgen->pt()
		  << " eta: " << bgen->eta() 
		  << " phi: " << bgen->phi() << std::endl;
      }
    }
  }
  // dump the trigger primitive digis
  auto src = _tpinputs.cbegin();
  auto end = _tpinputs.cend();
  for( ; src != end; ++src ) {
    edm::Handle<TriggerPrimitiveCollection> tps;
    ev.getByLabel(*src,tps);
    auto tp = tps->cbegin();
    auto tpend = tps->cend();
    
    std::cout << "Dumping trigger primitives from : " << *src << std::endl;
    for( ; tp != tpend; ++tp ) {
      tp->print(std::cout);
    }    
  }
}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(L1ITMuTextDumper);
