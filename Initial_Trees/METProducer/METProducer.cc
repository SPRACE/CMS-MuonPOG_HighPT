// -*- C++ -*-
//
// Package:    METProducer
// Class:      METProducer
// 
/**\class METProducer METProducer.cc MuonAnalysis/METProducer/src/METProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Cesar Bernardes,40 2-B15,+41227671625,
//         Created:  Mon Sep 30 12:16:05 CEST 2013
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


#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/METReco/interface/PFMET.h"

//
// class declaration
//

class METProducer : public edm::EDProducer {
   public:
      explicit METProducer(const edm::ParameterSet&);
      ~METProducer();

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

      edm::InputTag probes_;
      edm::InputTag objects_;

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
METProducer::METProducer(const edm::ParameterSet& iConfig) : 
   probes_(iConfig.getParameter<edm::InputTag>("probes")),
   objects_(iConfig.getParameter<edm::InputTag>("objects"))
{
   //register your products
/* Examples
   produces<ExampleData2>();

   //if do put with a label
   produces<ExampleData2>("label");
 
   //if you want to put into the Run
   produces<ExampleData2,InRun>();
*/
   //now do what ever other initialization is needed
  //   produces<edm::ValueMap<float> >();
   produces<edm::ValueMap<float> >("MET");
   produces<edm::ValueMap<float> >("METx");
   produces<edm::ValueMap<float> >("METy");
   produces<edm::ValueMap<float> >("METphi");
}


METProducer::~METProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
METProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   // read input
   Handle<View<reco::Candidate> > probes;
   Handle<std::vector<reco::PFMET> > objects;
   iEvent.getByLabel(probes_,  probes);
   iEvent.getByLabel(objects_, objects);

   // get the PFMET
   float met    = objects->begin()->et();
   float metx   = objects->begin()->px();
   float mety   = objects->begin()->py();
   float metphi = objects->begin()->phi();

   // prepare vector for output    
   //   std::vector<float> values(probes->size(), met);
   std::vector<float> MET   (probes->size(), met);
   std::vector<float> METx  (probes->size(), metx);
   std::vector<float> METy  (probes->size(), mety);
   std::vector<float> METphi(probes->size(), metphi);

   writeValueMap(iEvent, probes, MET   , "MET");
   writeValueMap(iEvent, probes, METx  , "METx");
   writeValueMap(iEvent, probes, METy  , "METy");
   writeValueMap(iEvent, probes, METphi, "METphi");

   /*
   // convert into ValueMap and store
   std::auto_ptr<ValueMap<float> > valMap(new ValueMap<float>());
   ValueMap<float>::Filler filler(*valMap);
   filler.insert(probes, values.begin(), values.end());
   filler.fill();
   iEvent.put(valMap);
   */

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
METProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
METProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
METProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
METProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
METProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
METProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
METProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
METProducer::writeValueMap(edm::Event &iEvent,
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
DEFINE_FWK_MODULE(METProducer);
