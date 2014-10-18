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
      edm::InputTag objects1_;
      edm::InputTag objects2_;
      edm::InputTag objects3_;
      edm::InputTag objects4_;
      edm::InputTag objects5_;
      edm::InputTag objects6_;
      edm::InputTag objects7_;
      edm::InputTag objects8_;
      edm::InputTag objects9_;
      edm::InputTag objects10_;
      edm::InputTag objects11_;

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
   objects1_(iConfig.getParameter<edm::InputTag>("objects1")),
   objects2_(iConfig.getParameter<edm::InputTag>("objects2")),
   objects3_(iConfig.getParameter<edm::InputTag>("objects3")),
   objects4_(iConfig.getParameter<edm::InputTag>("objects4")),
   objects5_(iConfig.getParameter<edm::InputTag>("objects5")),
   objects6_(iConfig.getParameter<edm::InputTag>("objects6")),
   objects7_(iConfig.getParameter<edm::InputTag>("objects7")),
   objects8_(iConfig.getParameter<edm::InputTag>("objects8")),
   objects9_(iConfig.getParameter<edm::InputTag>("objects9")),
   objects10_(iConfig.getParameter<edm::InputTag>("objects10")),
   objects11_(iConfig.getParameter<edm::InputTag>("objects11"))
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
///   produces<edm::ValueMap<float> >();  

   //pfMet
   produces<edm::ValueMap<float> >("MET1");
   produces<edm::ValueMap<float> >("METx1"); 
   produces<edm::ValueMap<float> >("METy1");
   produces<edm::ValueMap<float> >("METphi1");
 
   //pfMetT0rt
   produces<edm::ValueMap<float> >("MET2");
   produces<edm::ValueMap<float> >("METx2");
   produces<edm::ValueMap<float> >("METy2");
   produces<edm::ValueMap<float> >("METphi2");

   //pfMetT0rtT1
   produces<edm::ValueMap<float> >("MET3");
   produces<edm::ValueMap<float> >("METx3");
   produces<edm::ValueMap<float> >("METy3");
   produces<edm::ValueMap<float> >("METphi3");

   //pfMetT0pc
   produces<edm::ValueMap<float> >("MET4");
   produces<edm::ValueMap<float> >("METx4");
   produces<edm::ValueMap<float> >("METy4");
   produces<edm::ValueMap<float> >("METphi4");

   //pfMetT0pcT1
   produces<edm::ValueMap<float> >("MET5");
   produces<edm::ValueMap<float> >("METx5");
   produces<edm::ValueMap<float> >("METy5");
   produces<edm::ValueMap<float> >("METphi5");

   //pfMetT1
   produces<edm::ValueMap<float> >("MET6");
   produces<edm::ValueMap<float> >("METx6");
   produces<edm::ValueMap<float> >("METy6");
   produces<edm::ValueMap<float> >("METphi6");

   //pfMetT0rtTxy
   produces<edm::ValueMap<float> >("MET7");
   produces<edm::ValueMap<float> >("METx7");
   produces<edm::ValueMap<float> >("METy7");
   produces<edm::ValueMap<float> >("METphi7");

   //pfMetT0rtT1Txy
   produces<edm::ValueMap<float> >("MET8");
   produces<edm::ValueMap<float> >("METx8");
   produces<edm::ValueMap<float> >("METy8");
   produces<edm::ValueMap<float> >("METphi8");

   //pfMetT0pcTxy
   produces<edm::ValueMap<float> >("MET9");
   produces<edm::ValueMap<float> >("METx9");
   produces<edm::ValueMap<float> >("METy9");
   produces<edm::ValueMap<float> >("METphi9");
  
   //pfMetT0pcT1Txy
   produces<edm::ValueMap<float> >("MET10");
   produces<edm::ValueMap<float> >("METx10");
   produces<edm::ValueMap<float> >("METy10");
   produces<edm::ValueMap<float> >("METphi10");

   //pfMetT1Txy
   produces<edm::ValueMap<float> >("MET11");
   produces<edm::ValueMap<float> >("METx11");
   produces<edm::ValueMap<float> >("METy11");
   produces<edm::ValueMap<float> >("METphi11");

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
   iEvent.getByLabel(probes_,  probes);

   Handle<std::vector<reco::PFMET> > objects1;
   iEvent.getByLabel(objects1_, objects1);

   Handle<std::vector<reco::PFMET> > objects2;
   iEvent.getByLabel(objects2_, objects2);

   Handle<std::vector<reco::PFMET> > objects3;
   iEvent.getByLabel(objects3_, objects3);

   Handle<std::vector<reco::PFMET> > objects4;
   iEvent.getByLabel(objects4_, objects4);

   Handle<std::vector<reco::PFMET> > objects5;
   iEvent.getByLabel(objects5_, objects5);

   Handle<std::vector<reco::PFMET> > objects6;
   iEvent.getByLabel(objects6_, objects6);

   Handle<std::vector<reco::PFMET> > objects7;
   iEvent.getByLabel(objects7_, objects7);

   Handle<std::vector<reco::PFMET> > objects8;
   iEvent.getByLabel(objects8_, objects8);

   Handle<std::vector<reco::PFMET> > objects9;
   iEvent.getByLabel(objects9_, objects9);

   Handle<std::vector<reco::PFMET> > objects10;
   iEvent.getByLabel(objects10_, objects10);

   Handle<std::vector<reco::PFMET> > objects11;
   iEvent.getByLabel(objects11_, objects11);
   

   //pfMet 
   float met1    = objects1->begin()->pt();
   float metx1   = objects1->begin()->px();
   float mety1   = objects1->begin()->py();
   float metphi1 = objects1->begin()->phi();

   //pfMetT0rt
   float met2    = objects2->begin()->pt();
   float metx2   = objects2->begin()->px();
   float mety2   = objects2->begin()->py();
   float metphi2 = objects2->begin()->phi();
  
   //pfMetT0rtT1
   float met3    = objects3->begin()->pt();
   float metx3   = objects3->begin()->px();
   float mety3   = objects3->begin()->py();
   float metphi3 = objects3->begin()->phi();

   //pfMetT0pc
   float met4    = objects4->begin()->pt();
   float metx4   = objects4->begin()->px();
   float mety4   = objects4->begin()->py();
   float metphi4 = objects4->begin()->phi();

   //pfMetT0pcT1
   float met5    = objects5->begin()->pt();
   float metx5   = objects5->begin()->px();
   float mety5   = objects5->begin()->py();
   float metphi5 = objects5->begin()->phi();

   //pfMetT1
   float met6    = objects6->begin()->pt();
   float metx6   = objects6->begin()->px();
   float mety6   = objects6->begin()->py();
   float metphi6 = objects6->begin()->phi();

   //pfMetT0rtTxy
   float met7    = objects7->begin()->pt();
   float metx7   = objects7->begin()->px();
   float mety7   = objects7->begin()->py();
   float metphi7 = objects7->begin()->phi();

   //pfMetT0rtT1Txy
   float met8    = objects8->begin()->pt();
   float metx8   = objects8->begin()->px();
   float mety8   = objects8->begin()->py();
   float metphi8 = objects8->begin()->phi();

   //pfMetT0pcTxy
   float met9    = objects9->begin()->pt();
   float metx9   = objects9->begin()->px();
   float mety9   = objects9->begin()->py();
   float metphi9 = objects9->begin()->phi();

   //pfMetT0pcT1Txy
   float met10    = objects10->begin()->pt();
   float metx10   = objects10->begin()->px();
   float mety10   = objects10->begin()->py();
   float metphi10 = objects10->begin()->phi();

   //pfMetT1Txy
   float met11    = objects11->begin()->pt();
   float metx11   = objects11->begin()->px();
   float mety11   = objects11->begin()->py();
   float metphi11 = objects11->begin()->phi();




  
   /*std::cout<<"(MET Type, pt, px, py, phi)"<<std::endl;
   std::cout<<"( pfMet,           "<< met1 << ", "<< metx1 << ", "<< mety1 << ", "<< metphi1 <<" )" << std::endl;
   std::cout<<"( pfMetT0rt,       "<< met2 << ", "<< metx2 << ", "<< mety2 << ", "<< metphi2 <<" )" << std::endl;
   std::cout<<"( pfMetT0rtT1,     "<< met3 << ", "<< metx3 << ", "<< mety3 << ", "<< metphi3 <<" )" << std::endl;
   std::cout<<"( pfMetT0pc,       "<< met4 << ", "<< metx4 << ", "<< mety4 << ", "<< metphi4 <<" )" << std::endl;
   std::cout<<"( pfMetT0pcT1,     "<< met5 << ", "<< metx5 << ", "<< mety5 << ", "<< metphi5 <<" )" << std::endl;
   std::cout<<"( pfMetT1,         "<< met6 << ", "<< metx6 << ", "<< mety6 << ", "<< metphi6 <<" )" << std::endl;
   std::cout<<"( pfMetT0rtTxy,    "<< met7 << ", "<< metx7 << ", "<< mety7 << ", "<< metphi7 <<" )" << std::endl;
   std::cout<<"( pfMetT0rtT1Txy,  "<< met8 << ", "<< metx8 << ", "<< mety8 << ", "<< metphi8 <<" )" << std::endl;
   std::cout<<"( pfMetT0pcTxy,    "<< met9 << ", "<< metx9 << ", "<< mety9 << ", "<< metphi9 <<" )" << std::endl;
   std::cout<<"( pfMetT0pcT1Txy,  "<< met10 << ", "<< metx10 << ", "<< mety10 << ", "<< metphi10 <<" )" << std::endl;
   std::cout<<"( pfMetT1Txy,      "<< met11 << ", "<< metx11 << ", "<< mety11 << ", "<< metphi11 <<" )" << std::endl; */


   //pfMet   
   std::vector<float> MET1   (probes->size(), met1);
   std::vector<float> METx1  (probes->size(), metx1);
   std::vector<float> METy1  (probes->size(), mety1);
   std::vector<float> METphi1(probes->size(), metphi1);

   writeValueMap(iEvent, probes, MET1   , "MET1"); 
   writeValueMap(iEvent, probes, METx1  , "METx1");
   writeValueMap(iEvent, probes, METy1  , "METy1");                      
   writeValueMap(iEvent, probes, METphi1, "METphi1");

   //pfMetT0rt
   std::vector<float> MET2   (probes->size(), met2);
   std::vector<float> METx2  (probes->size(), metx2);
   std::vector<float> METy2  (probes->size(), mety2);
   std::vector<float> METphi2(probes->size(), metphi2);

   writeValueMap(iEvent, probes, MET2   , "MET2");
   writeValueMap(iEvent, probes, METx2  , "METx2");
   writeValueMap(iEvent, probes, METy2  , "METy2");
   writeValueMap(iEvent, probes, METphi2, "METphi2");

   //pfMetT0rtT1
   std::vector<float> MET3   (probes->size(), met3);
   std::vector<float> METx3  (probes->size(), metx3);
   std::vector<float> METy3  (probes->size(), mety3);
   std::vector<float> METphi3(probes->size(), metphi3);

   writeValueMap(iEvent, probes, MET3   , "MET3");
   writeValueMap(iEvent, probes, METx3  , "METx3");
   writeValueMap(iEvent, probes, METy3  , "METy3");
   writeValueMap(iEvent, probes, METphi3, "METphi3");

   //pfMetT0pc
   std::vector<float> MET4   (probes->size(), met4);
   std::vector<float> METx4  (probes->size(), metx4);
   std::vector<float> METy4  (probes->size(), mety4);
   std::vector<float> METphi4(probes->size(), metphi4);

   writeValueMap(iEvent, probes, MET4   , "MET4");
   writeValueMap(iEvent, probes, METx4  , "METx4");
   writeValueMap(iEvent, probes, METy4  , "METy4");
   writeValueMap(iEvent, probes, METphi4, "METphi4");

   //pfMetT0pcT1
   std::vector<float> MET5   (probes->size(), met5);
   std::vector<float> METx5  (probes->size(), metx5);
   std::vector<float> METy5  (probes->size(), mety5);
   std::vector<float> METphi5(probes->size(), metphi5);

   writeValueMap(iEvent, probes, MET5   , "MET5");
   writeValueMap(iEvent, probes, METx5  , "METx5");
   writeValueMap(iEvent, probes, METy5  , "METy5");
   writeValueMap(iEvent, probes, METphi5, "METphi5");

   //pfMetT1
   std::vector<float> MET6   (probes->size(), met6);
   std::vector<float> METx6  (probes->size(), metx6);
   std::vector<float> METy6  (probes->size(), mety6);
   std::vector<float> METphi6(probes->size(), metphi6);

   writeValueMap(iEvent, probes, MET6   , "MET6");
   writeValueMap(iEvent, probes, METx6  , "METx6");
   writeValueMap(iEvent, probes, METy6  , "METy6");
   writeValueMap(iEvent, probes, METphi6, "METphi6");

   //pfMetT0rtTxy
   std::vector<float> MET7   (probes->size(), met7);
   std::vector<float> METx7  (probes->size(), metx7);
   std::vector<float> METy7  (probes->size(), mety7);
   std::vector<float> METphi7(probes->size(), metphi7);

   writeValueMap(iEvent, probes, MET7   , "MET7");
   writeValueMap(iEvent, probes, METx7  , "METx7");
   writeValueMap(iEvent, probes, METy7  , "METy7");
   writeValueMap(iEvent, probes, METphi7, "METphi7");

   //pfMetT0rtT1Txy
   std::vector<float> MET8   (probes->size(), met8);
   std::vector<float> METx8  (probes->size(), metx8);
   std::vector<float> METy8  (probes->size(), mety8);
   std::vector<float> METphi8(probes->size(), metphi8);

   writeValueMap(iEvent, probes, MET8   , "MET8");
   writeValueMap(iEvent, probes, METx8  , "METx8");
   writeValueMap(iEvent, probes, METy8  , "METy8");
   writeValueMap(iEvent, probes, METphi8, "METphi8");

   //pfMetT0pcTxy
   std::vector<float> MET9   (probes->size(), met9);
   std::vector<float> METx9  (probes->size(), metx9);
   std::vector<float> METy9  (probes->size(), mety9);
   std::vector<float> METphi9(probes->size(), metphi9);

   writeValueMap(iEvent, probes, MET9   , "MET9");
   writeValueMap(iEvent, probes, METx9  , "METx9");
   writeValueMap(iEvent, probes, METy9  , "METy9");
   writeValueMap(iEvent, probes, METphi9, "METphi9");

   //pfMetT0pcT1Txy
   std::vector<float> MET10   (probes->size(), met10);
   std::vector<float> METx10  (probes->size(), metx10);
   std::vector<float> METy10  (probes->size(), mety10);
   std::vector<float> METphi10(probes->size(), metphi10);

   writeValueMap(iEvent, probes, MET10   , "MET10");
   writeValueMap(iEvent, probes, METx10  , "METx10");
   writeValueMap(iEvent, probes, METy10  , "METy10");
   writeValueMap(iEvent, probes, METphi10, "METphi10");    
   
   //pfMetT1Txy
   std::vector<float> MET11   (probes->size(), met11);
   std::vector<float> METx11  (probes->size(), metx11);
   std::vector<float> METy11  (probes->size(), mety11);
   std::vector<float> METphi11(probes->size(), metphi11);

   writeValueMap(iEvent, probes, MET11   , "MET11");
   writeValueMap(iEvent, probes, METx11  , "METx11");
   writeValueMap(iEvent, probes, METy11  , "METy11");
   writeValueMap(iEvent, probes, METphi11, "METphi11");


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
