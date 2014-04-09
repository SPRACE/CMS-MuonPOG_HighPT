// -*- C++ -*-
//
// Package:    GenPVariablesProducer
// Class:      GenPVariablesProducer
// 
/**\class GenPVariablesProducer GenPVariablesProducer.cc MuonAnalysis/GenPVariablesProducer/src/GenPVariablesProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Angelo De Souza Santos,,,
//         Created:  Wed Oct 30 23:30:23 CET 2013
// $Id$
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

//my includes
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

//
// class declaration
//

class GenPVariablesProducer : public edm::EDProducer {
   public:
      explicit GenPVariablesProducer(const edm::ParameterSet&);
      ~GenPVariablesProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      virtual void beginRun(edm::Run&, edm::EventSetup const&);
      virtual void endRun(edm::Run&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);

      // ----------member data ---------------------------
      edm::InputTag src_;
      edm::InputTag obj_;

     /// Write a ValueMap<float> in the event                                                                                                                                                          
     void writeValueMap(edm::Event &iEvent,
			const edm::Handle<edm::View<reco::Candidate> > & handle,
			const std::vector<float> & values,
			const std::string    & label) const ;
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
GenPVariablesProducer::GenPVariablesProducer(const edm::ParameterSet& iConfig) :
  src_(iConfig.getParameter<edm::InputTag>("src")),
  obj_(iConfig.getParameter<edm::InputTag>("obj"))
{
  produces<edm::ValueMap<float> >("vMuPluspt");
  produces<edm::ValueMap<float> >("vMuPluseta");
  produces<edm::ValueMap<float> >("vMuPlusphi");
  produces<edm::ValueMap<float> >("vMuMinuspt");
  produces<edm::ValueMap<float> >("vMuMinuseta");
  produces<edm::ValueMap<float> >("vMuMinusphi");
  produces<edm::ValueMap<float> >("vDimuMass");

   //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
 
   //if you want to put into the Run
   produces<ExampleData2,InRun>();
*/
   //now do what ever other initialization is needed
  
}


GenPVariablesProducer::~GenPVariablesProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
GenPVariablesProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<View<reco::Candidate> > src;
   iEvent.getByLabel(src_, src);

   Handle<std::vector<reco::GenParticle> > objects;
   iEvent.getByLabel(obj_, objects);

   size_t n = src->size();
   float MuPlus_pt(-999.0),MuPlus_eta(-999.0),MuPlus_phi(-999.0),MuMinus_pt(-999.0),MuMinus_eta(-999.0),MuMinus_phi(-999.0),DimuMass(-999.0);

   int nMuPlus=0;
   int nMuMinus=0;
   const reco::GenParticle * MPlus = 0;
   const reco::GenParticle * MMinus = 0;
   for(size_t ngenPart=0; ngenPart<objects->size(); ngenPart++){
     const reco::GenParticle & genPart = (*objects)[ngenPart];
     if(genPart.pdgId()==13 && genPart.status()==1){
       nMuMinus++;
       MMinus = &genPart;
     }
     if(genPart.pdgId()==-13 && genPart.status()==1){
       nMuPlus++;
       MPlus = &genPart;
     }
   }
   if( (nMuMinus==1) && (nMuPlus==1) ){
     MuPlus_pt = MPlus->pt();
     MuPlus_eta = MPlus->eta();
     MuPlus_phi = MPlus->phi();
     MuMinus_pt = MMinus->pt();
     MuMinus_eta = MMinus->eta();
     MuMinus_phi = MMinus->phi();
     DimuMass = (MPlus->p4() + MMinus->p4()).mass();
   }
   std::vector<float> vMuPluspt(n,MuPlus_pt),vMuPluseta(n,MuPlus_eta),vMuPlusphi(n,MuPlus_phi),vMuMinuspt(n,MuMinus_pt),vMuMinuseta(n,MuMinus_eta),vMuMinusphi(n,MuMinus_phi),vDimuMass(n,DimuMass);

   writeValueMap(iEvent, src,vMuPluspt , "vMuPluspt");
   writeValueMap(iEvent, src,vMuPluseta , "vMuPluseta");
   writeValueMap(iEvent, src,vMuPlusphi , "vMuPlusphi");
   writeValueMap(iEvent, src,vMuMinuspt , "vMuMinuspt");
   writeValueMap(iEvent, src,vMuMinuseta,  "vMuMinuseta");
   writeValueMap(iEvent, src,vMuMinusphi , "vMuMinusphi");
   writeValueMap(iEvent, src,vDimuMass , "vDimuMass");

/* This is an event example
   //Read 'ExampleData' from the Event
   Handle<ExampleData> pIn;
   iEvent.getByLabel("example",pIn);

   //Use the ExampleData to create an ExampleData2 which 
   // is put into the Event
   std::auto_ptr<ExampleData2> pOut(new ExampleData2(*pIn));
   iEvent.put(pOut);
*/

/* this is an EventSetup example
   //Read SetupData from the SetupRecord in the EventSetup
   ESHandle<SetupData> pSetup;
   iSetup.get<SetupRecord>().get(pSetup);
*/
 
}

// ------------ method called once each job just before starting event loop  ------------
void 
GenPVariablesProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GenPVariablesProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
GenPVariablesProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
GenPVariablesProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
GenPVariablesProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
GenPVariablesProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenPVariablesProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
GenPVariablesProducer::writeValueMap(edm::Event &iEvent,
				     const edm::Handle<edm::View<reco::Candidate> > & handle,
				     const std::vector<float> & values,
				     const std::string & label) const
{
  using namespace edm;
  using namespace std;
  auto_ptr<ValueMap<float> > valMap(new ValueMap<float>());
  edm::ValueMap<float>::Filler filler(*valMap);
  filler.insert(handle, values.begin(), values.end());
  filler.fill();
  iEvent.put(valMap, label);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenPVariablesProducer);
