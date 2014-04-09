// -*- C++ -*-
//
// Package:    NewTunePMuonsProducer
// Class:      NewTunePMuonsProducer
// 
/**\class NewTunePMuonsProducer NewTunePMuonsProducer.cc MuonAnalysis/NewTunePMuonsProducer/src/NewTunePMuonsProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Angelo De Souza Santos,,,
//         Created:  Wed Oct 30 23:58:13 CET 2013
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
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "DataFormats/MuonReco/interface/MuonSelectors.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <vector>

//
// class declaration
//

class NewTunePMuonsProducer : public edm::EDProducer {
   public:
      explicit NewTunePMuonsProducer(const edm::ParameterSet&);
      ~NewTunePMuonsProducer();

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
      edm::InputTag vtx_;

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
NewTunePMuonsProducer::NewTunePMuonsProducer(const edm::ParameterSet& iConfig) :
  src_( iConfig.getParameter<edm::InputTag>( "src" ) ),
  vtx_( iConfig.getParameter<edm::InputTag>( "vtx" ) )

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

  produces<std::vector<pat::Muon> >();
  
}


NewTunePMuonsProducer::~NewTunePMuonsProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
NewTunePMuonsProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<reco::VertexCollection> recVtxs;
   iEvent.getByLabel(vtx_, recVtxs);

   // From CMSSW/Validation/RecoMuon/src/RecoMuonValidator.cc
   // Look for the Primary Vertex (and use the BeamSpot instead, if you can't find it):
   reco::Vertex::Point posVtx;
   reco::Vertex::Error errVtx;
   //edm::Handle<reco::VertexCollection> recVtxs;
   //event.getByLabel(primvertexLabel_,recVtxs);
   unsigned int theIndexOfThePrimaryVertex = 999.;
   for (unsigned int ind=0; ind<recVtxs->size(); ++ind) {
     if ( (*recVtxs)[ind].isValid() && !((*recVtxs)[ind].isFake()) ) {
       theIndexOfThePrimaryVertex = ind;
       break;
     }
   }
   if (theIndexOfThePrimaryVertex<100) {
     posVtx = ((*recVtxs)[theIndexOfThePrimaryVertex]).position();
     errVtx = ((*recVtxs)[theIndexOfThePrimaryVertex]).error();
   }
   else {
     LogInfo("RecoMuonValidator") << "reco::PrimaryVertex not found, use BeamSpot position instead\n";
     //edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
     //event.getByLabel(beamspotLabel_,recoBeamSpotHandle);
     Handle<reco::BeamSpot> recoBeamSpotHandle;
     iEvent.getByLabel("offlineBeamSpot",recoBeamSpotHandle);
     reco::BeamSpot bs = *recoBeamSpotHandle;
     posVtx = bs.position();
     errVtx(0,0) = bs.BeamWidthX();
     errVtx(1,1) = bs.BeamWidthY();
     errVtx(2,2) = bs.sigmaZ();
   }
   const reco::Vertex thePrimaryVertex(posVtx,errVtx);

   Handle< std::vector<pat::Muon> > muons;
   iEvent.getByLabel(src_,muons);

   std::auto_ptr< std::vector<pat::Muon> > muonColl( new std::vector<pat::Muon> (*muons) );

   for(unsigned int i = 0; i< muonColl->size();++i){

     pat::Muon & m = (*muonColl)[i];

     //redefined track
     reco::TrackRef cktTrack;
     cktTrack = muon::improvedMuonBestTrack(m, muon::improvedTuneP);

     //pt, ptRelError, eta and phi
     float NewTunePpt = cktTrack->pt();
     float NewTunePptRelError = cktTrack->ptError()/cktTrack->pt();
     float NewTunePeta = cktTrack->eta();
     float NewTunePphi = cktTrack->phi();

     //is High Pt muon (using the newTuneP definition)
     float isNewHPTmuonGlobal = 0.0;
     if(muon::isHighPtMuon(m,thePrimaryVertex))isNewHPTmuonGlobal = 1.0;

     m.addUserFloat("IsNewHPTmuonGlobal", isNewHPTmuonGlobal);
     m.addUserFloat("NEWTUNEPpt", NewTunePpt);
     m.addUserFloat("NEWTUNEPptRelError",NewTunePptRelError);
     m.addUserFloat("NEWTUNEPeta",NewTunePeta);
     m.addUserFloat("NEWTUNEPphi",NewTunePphi);

   }
   iEvent.put(muonColl);

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
NewTunePMuonsProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
NewTunePMuonsProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
NewTunePMuonsProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
NewTunePMuonsProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
NewTunePMuonsProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
NewTunePMuonsProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
NewTunePMuonsProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(NewTunePMuonsProducer);
