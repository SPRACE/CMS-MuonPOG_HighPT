// -*- C++ -*-
//
// Package:    DimuonVtxProducer
// Class:      DimuonVtxProducer
// 
/**\class DimuonVtxProducer DimuonVtxProducer.cc MuonAnalysis/DimuonVtxProducer/src/DimuonVtxProducer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Angelo De Souza Santos,,,
//         Created:  Thu Oct 10 03:04:59 CEST 2013
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
#include "DataFormats/Candidate/interface/CompositeCandidate.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/MuonReco/interface/MuonCocktails.h"

//for tracks
#include "DataFormats/TrackReco/interface/Track.h"

// for vertexing                                                                                                                                                                                        
#include "FWCore/Framework/interface/ESHandle.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"


#include <math.h>
#include <TMath.h>

//
// class declaration
//

class DimuonVtxProducer : public edm::EDProducer {
   public:
      explicit DimuonVtxProducer(const edm::ParameterSet&);
      ~DimuonVtxProducer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
      reco::TrackRef mu_track(const reco::Muon & mu);
      //TransientVertex dimuon_vtxfit(const reco::Muon & mu1, const reco::Muon & mu2, edm::ESHandle<TransientTrackBuilder> trkBuild);

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
DimuonVtxProducer::DimuonVtxProducer(const edm::ParameterSet& iConfig) :
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

  produces<edm::ValueMap<float> >("vtxNormQui2");
  produces<edm::ValueMap<float> >("vtxZcoordinate");
  produces<edm::ValueMap<float> >("vtxZcoordinateFromBS");
  produces<edm::ValueMap<float> >("vtxRdistance");
  produces<edm::ValueMap<float> >("vtxRdistanceFromBS");
  produces<edm::ValueMap<float> >("tagPtAtTheVtx");
  produces<edm::ValueMap<float> >("probePtAtTheVtx");
  produces<edm::ValueMap<float> >("tagPtBefore");
  produces<edm::ValueMap<float> >("probePtBefore");

}


DimuonVtxProducer::~DimuonVtxProducer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
void
DimuonVtxProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

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

   edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
   iEvent.getByLabel("offlineBeamSpot" ,recoBeamSpotHandle);
   reco::BeamSpot bs = *recoBeamSpotHandle;
   math::XYZPoint BSPosition;
   BSPosition = bs.position();

   Handle<View<reco::Candidate> > src;
   iEvent.getByLabel(src_, src);

   size_t n = src->size();
   std::vector<float> vtxNormQui2(n,-999), vtxZcoordinate(n,-999), vtxZcoordinateFromBS(n,-999), vtxRdistance(n,-999), vtxRdistanceFromBS(n,-999), tagPtAtTheVtx(n,0), probePtAtTheVtx(n,0), tagPtBefore(n,0), probePtBefore(n,0);

   edm::ESHandle<TransientTrackBuilder> trkBuild;
   iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",trkBuild);

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
     //     if( mu1->globalTrack().isNull() || mu2->globalTrack().isNull() ) continue;

     if( (mu1->charge() == mu2->charge()) && (tuneP1.first->pt() == tuneP2.first->pt()) ) continue;
     //TransientVertex dimu_vtx = dimuon_vtxfit(*mu1, *mu2, trkBuild);
     reco::TrackRef muon1; //reco::TrackRef muon1 = mu_track(*mu1);
     reco::TrackRef muon2; //reco::TrackRef muon2 = mu_track(*mu2);
     if( (!(mu1->innerTrack().isNull())) && (!(mu2->innerTrack().isNull())) ){
       muon1 = mu1->innerTrack();
       muon2 = mu2->innerTrack();
     }else{ continue; }

     std::vector<reco::TransientTrack> t_tks;
     t_tks.clear();
     t_tks.push_back( (*trkBuild).build(muon1.get()));
     t_tks.push_back( (*trkBuild).build(muon2.get()));
     TransientVertex dimu_vtx;
     if(t_tks.size() > 1){
       KalmanVertexFitter kvf(true);
       dimu_vtx = kvf.vertex(t_tks);
     }

     if(dimu_vtx.isValid() && dimu_vtx.totalChiSquared() >= 0. && dimu_vtx.degreesOfFreedom() > 0){

       vtxNormQui2[i] = dimu_vtx.totalChiSquared()/dimu_vtx.degreesOfFreedom();

       vtxZcoordinate[i] = dimu_vtx.position().z();
       //z distance with respect to the beam spot
       double dimu_vtx_Z;
       dimu_vtx_Z = dimu_vtx.position().z();
       double bsz;
       bsz = BSPosition.z();
       vtxZcoordinateFromBS[i] = (dimu_vtx_Z - bsz);

       vtxRdistance[i] = dimu_vtx.position().perp();

       //Dimuon Vertex x and y coordinate
       double dimu_vtx_X,dimu_vtx_Y;
       dimu_vtx_X = dimu_vtx.position().x();
       dimu_vtx_Y = dimu_vtx.position().y();

       //r distance with respect to the beam spot
       double bsx, bsy;
       bsx = BSPosition.x();
       bsy = BSPosition.y();
       //std::cout << "BSPosition.x()  =  " << bsx << "  BSPosition.y()  =  " << bsy << std::endl;
       double rxy_vtx_bs;
       rxy_vtx_bs = std::sqrt( (bsx-dimu_vtx_X)*(bsx-dimu_vtx_X) + (bsy-dimu_vtx_Y)*(bsy-dimu_vtx_Y) );
       vtxRdistanceFromBS[i] = rxy_vtx_bs;

       // important! evaluate momentum vectors AT THE VERTEX
       TrajectoryStateClosestToPoint tag_TSCP = t_tks[0].trajectoryStateClosestToPoint(dimu_vtx.position());
       TrajectoryStateClosestToPoint probe_TSCP = t_tks[1].trajectoryStateClosestToPoint(dimu_vtx.position());
       GlobalVector tag_momentum = tag_TSCP.momentum();
       GlobalVector probe_momentum = probe_TSCP.momentum();

       //Tag pt
       double tag_px = tag_momentum.x();
       double tag_py = tag_momentum.y();
       //std::cout << "tag px:  " << tag_px << "  tag py:  "  << tag_py << std::endl;
       tagPtAtTheVtx[i] = TMath::Sqrt(TMath::Power(tag_px,2) + TMath::Power(tag_py,2));
       tagPtBefore[i] = mu1->pt();

       //Probe pt
       double probe_px = probe_momentum.x();
       double probe_py = probe_momentum.y();
       //std::cout << "probe px:  " << probe_px << "  probe py:  "  << probe_py << std::endl;
       probePtAtTheVtx[i] = TMath::Sqrt(TMath::Power(probe_px,2) + TMath::Power(probe_py,2));
       probePtBefore[i] = mu2->pt();

     }else{
       //std::cout << "Qui2 do Vertice ----- VERTEX IS NOT VALID!!!" << std::endl;
     }

   }

   writeValueMap(iEvent, src, vtxNormQui2, "vtxNormQui2");
   writeValueMap(iEvent, src, vtxZcoordinate, "vtxZcoordinate");
   writeValueMap(iEvent, src, vtxZcoordinateFromBS, "vtxZcoordinateFromBS");
   writeValueMap(iEvent, src, vtxRdistance, "vtxRdistance");
   writeValueMap(iEvent, src, vtxRdistanceFromBS, "vtxRdistanceFromBS");
   writeValueMap(iEvent, src, tagPtAtTheVtx, "tagPtAtTheVtx");
   writeValueMap(iEvent, src, probePtAtTheVtx, "probePtAtTheVtx");
   writeValueMap(iEvent, src, tagPtBefore, "tagPtBefore");
   writeValueMap(iEvent, src, probePtBefore, "probePtBefore");

}

// ------------ method called once each job just before starting event loop  ------------
void 
DimuonVtxProducer::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
DimuonVtxProducer::endJob() {
}

// ------------ method called when starting to processes a run  ------------
void 
DimuonVtxProducer::beginRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a run  ------------
void 
DimuonVtxProducer::endRun(edm::Run&, edm::EventSetup const&)
{
}

// ------------ method called when starting to processes a luminosity block  ------------
void 
DimuonVtxProducer::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method called when ending the processing of a luminosity block  ------------
void 
DimuonVtxProducer::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
DimuonVtxProducer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}



void
DimuonVtxProducer::writeValueMap(edm::Event &iEvent,
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

//This function is not recommended to be used for our case
//since the muon collection is merged with the general tracks
//then in case the probe is a fake general track, probably
//this function will return a null reco::TrackRef
reco::TrackRef DimuonVtxProducer::mu_track(const reco::Muon & mu){
  reco::TrackRef to_return;
  if(mu.isStandAloneMuon()) to_return = mu.outerTrack();
  if(mu.isTrackerMuon()) to_return = mu.innerTrack();
  if(mu.isGlobalMuon()) to_return = mu.globalTrack();

  return to_return;
}

//define this as a plug-in
DEFINE_FWK_MODULE(DimuonVtxProducer);
