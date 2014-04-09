// -*- C++ -*-
//
// Package:    VetoCosmicMuonsProducer
// Class:      VetoCosmicMuonsProducer
// 
/**\class VetoCosmicMuonsProducer VetoCosmicMuonsProducer.cc MuonAnalysis/VetoCosmicMuonsProducer/src/VetoCosmicMuonsProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Angelo De Souza Santos,,,
//         Created:  Wed Oct 30 21:19:27 CET 2013
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
#include "RecoMuon/MuonIdentification/interface/MuonCosmicsId.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"

#include "TVector3.h"

//
// class declaration
//

class VetoCosmicMuonsProducer : public edm::EDProducer {
   public:
      explicit VetoCosmicMuonsProducer(const edm::ParameterSet&);
      ~VetoCosmicMuonsProducer();

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
VetoCosmicMuonsProducer::VetoCosmicMuonsProducer(const edm::ParameterSet& iConfig) :
  src_(iConfig.getParameter<edm::InputTag>("src"))
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
   produces<edm::ValueMap<float> >("collinearity1"); 

}


VetoCosmicMuonsProducer::~VetoCosmicMuonsProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
VetoCosmicMuonsProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<View<reco::Candidate> > src;
   iEvent.getByLabel(src_, src);

   size_t n = src->size();
   std::vector<float> collinearity1(n,-999);

   for(size_t i = 0; i < n; ++i) {

     const reco::Candidate & ci = (*src)[i];
     if (ci.numberOfDaughters() != 2) throw cms::Exception("CorruptData") <<
       "DimuonVtxProducer should be used on composite candidates with two daughters, this one has " << ci.numberOfDaughters() << "\n";
     const reco::Candidate &d1 = *ci.daughter(0), &d2 = *ci.daughter(1);

     const reco::Muon *mu1 = dynamic_cast<const reco::Muon *>(&*d1.masterClone());
     if (mu1 == 0) throw cms::Exception("CorruptData") << "First daughter of candidate is not a ShallowClone of a reco::Muon\n";
     reco::Muon::MuonTrackTypePair tuneP1 = muon::tevOptimized(*mu1, 200, 40., 17., 0.25);

     const reco::Muon *mu2 = dynamic_cast<const reco::Muon *>(&*d2.masterClone());
     if (mu2 == 0) throw cms::Exception("CorruptData") << "Second daughter of candidate is not a ShallowClone of a reco::Muon\n";
     reco::Muon::MuonTrackTypePair tuneP2 = muon::tevOptimized(*mu2, 200, 40., 17., 0.25);

     float tag_pX = tuneP1.first->px();
     float tag_pY = tuneP1.first->py();
     float tag_pZ = tuneP1.first->pz();
     TVector3 tag_Pxyz(tag_pX,tag_pY,tag_pZ);

     float probe_pX = tuneP2.first->px();
     float probe_pY = tuneP2.first->py();
     float probe_pZ = tuneP2.first->pz();
     TVector3 probe_Pxyz(probe_pX,probe_pY,probe_pZ);

     //Angle between two vectors                                                                                                                                                                       
     collinearity1[i] = tag_Pxyz.Angle(probe_Pxyz);

   }
   writeValueMap(iEvent, src, collinearity1, "collinearity1");

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
VetoCosmicMuonsProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
VetoCosmicMuonsProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
VetoCosmicMuonsProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
VetoCosmicMuonsProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
VetoCosmicMuonsProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
VetoCosmicMuonsProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
VetoCosmicMuonsProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

void
VetoCosmicMuonsProducer::writeValueMap(edm::Event &iEvent,
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
DEFINE_FWK_MODULE(VetoCosmicMuonsProducer);
