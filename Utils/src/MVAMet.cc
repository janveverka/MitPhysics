#include "MitPhysics/Utils/interface/MVAMet.h"
#include "MitPhysics/Utils/interface/JetTools.h"
#include "MitPhysics/Utils/interface/RecoilTools.h"
#include "MitAna/DataTree/interface/StableData.h"
#include <TFile.h>
#include <TRandom3.h>
//#include "TMVA/Tools.h"
//#include "TMVA/Reader.h"


ClassImp(mithep::MVAMet)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
MVAMet::MVAMet() :
  fRecoilTools(0),
  fPhiMethodName("PhiCorrection"),
  fU1MethodName ("U1Correction"),
  fIsInitialized(kFALSE),
  fU      (0),
  fUPhi   (0),
  fTKSumEt(0),
  fTKU    (0),
  fTKUPhi (0),
  fNPSumEt(0),
  fNPU    (0),
  fNPUPhi (0),
  fPUSumEt(0),
  fPUMet  (0),
  fPUMetPhi(0),
  fPCSumEt(0),
  fPCU    (0),
  fPCUPhi (0),
  fJSPt1  (0),
  fJSEta1 (0),
  fJSPhi1 (0),
  fJSPt2  (0),
  fJSEta2 (0),
  fJSPhi2 (0),
  fNJet   (0),
  fNAllJet(0),
  fNPV    (0),
  fPhiReader(0),
  fU1Reader(0)
{    }
//--------------------------------------------------------------------------------------------------
MVAMet::~MVAMet()
{
  fPhiReader = 0;  
  fU1Reader  = 0;
}
//--------------------------------------------------------------------------------------------------
/*
void MVAMet::setVariables(TMVA::Reader *iReader,bool iScale) { 
  iReader->AddVariable( "npv"              , &fNPV      ); 
  iReader->AddVariable( "u"                , &fU       ); 
  iReader->AddVariable( "uphi"             , &fUPhi    );
  iReader->AddVariable( "chsumet/sumet"    , &fTKSumEt ); 
  iReader->AddVariable( "tku"              , &fTKU     );
  iReader->AddVariable( "tkuphi"           , &fTKUPhi  );
  iReader->AddVariable( "nopusumet/sumet"  , &fNPSumEt );
  iReader->AddVariable( "nopuu"            , &fNPU     );
  iReader->AddVariable( "nopuuphi"         , &fNPUPhi  );
  iReader->AddVariable( "pusumet/sumet"    , &fPUSumEt );
  iReader->AddVariable( "pumet"            , &fPUMet   );
  iReader->AddVariable( "pumetphi"         , &fPUMetPhi);
  iReader->AddVariable( "pucsumet/sumet"   , &fPCSumEt );
  iReader->AddVariable( "pucu"             , &fPCU     );
  iReader->AddVariable( "pucuphi"          , &fPCUPhi  );
  iReader->AddVariable( "jspt_1"           , &fJSPt1   );
  iReader->AddVariable( "jseta_1"          , &fJSEta1  );
  iReader->AddVariable( "jsphi_1"          , &fJSPhi1  );
  iReader->AddVariable( "jspt_2"           , &fJSPt2   );
  iReader->AddVariable( "jseta_2"          , &fJSEta2  );
  iReader->AddVariable( "jsphi_2"          , &fJSPhi2  );
  iReader->AddVariable( "nalljet"          , &fNAllJet );
  iReader->AddVariable( "njet"             , &fNJet    );
  if(iScale) iReader->AddVariable( "uphi_mva"         , &fUPhiMVA );
}
//--------------------------------------------------------------------------------------------------
void MVAMet::Initialize( TString iU1MethodName,
			 TString iPhiMethodName,
			 TString iJetMVAFile, 
			 TString iU1Weights, 
			 TString iPhiWeights, 
			 MVAMet::MVAType     iType) { 
  
  fIsInitialized = kTRUE;
  fRecoilTools = new RecoilTools(iJetMVAFile);
  
  fU1MethodName  = iU1MethodName;
  fPhiMethodName = iPhiMethodName;
  fType          = iType;
  fPhiReader     = new TMVA::Reader( "!Color:!Silent:Error" );  
  fU1Reader      = new TMVA::Reader( "!Color:!Silent:Error" );  
  if (fType == kBaseline) {
    setVariables(fPhiReader,false);
    setVariables(fU1Reader ,true);
  }
  fPhiReader->BookMVA(fPhiMethodName , iPhiWeights );
  fU1Reader ->BookMVA(fU1MethodName  , iU1Weights );

  std::cout << "Jet ID MVA Initialization\n";
  std::cout << "Phi Method Name : " << fPhiMethodName << " , type == " << iType << std::endl;
  std::cout << "U1  Method Name : " << fU1MethodName  << " , type == " << iType << std::endl;
  

}
*/
//--------------------------------------------------------------------------------------------------
void MVAMet::Initialize( TString iJetMVAFile, 
			 TString iU1Weights, 
			 TString iPhiWeights, 
			 MVAMet::MVAType     iType) { 
  
  fIsInitialized = kTRUE;
  fRecoilTools = new RecoilTools(iJetMVAFile);
  
  fType          = iType;

  TFile *lPhiForest = new TFile(iPhiWeights,"READ");
  fPhiReader = (GBRForest*)lPhiForest->Get(fPhiMethodName);
  lPhiForest->Close();

  TFile *lU1Forest = new TFile(iU1Weights,"READ");
  fU1Reader  = (GBRForest*)lU1Forest->Get(fU1MethodName);
  lU1Forest->Close();

  fPhiVals = new Float_t[23];
  fU1Vals  = new Float_t[24];
}
//--------------------------------------------------------------------------------------------------
Double_t MVAMet::evaluatePhi() { 
  fPhiVals[0]  =  fNPV     ;
  fPhiVals[1]  =  fU       ;
  fPhiVals[2]  =  fUPhi    ;
  fPhiVals[3]  =  fTKSumEt ;
  fPhiVals[4]  =  fTKU     ;
  fPhiVals[5]  =  fTKUPhi  ;
  fPhiVals[6]  =  fNPSumEt ;
  fPhiVals[7]  =  fNPU     ;
  fPhiVals[8]  =  fNPUPhi  ;
  fPhiVals[9]  =  fPUSumEt ;
  fPhiVals[10] =  fPUMet   ;
  fPhiVals[11] =  fPUMetPhi;
  fPhiVals[12] =  fPCSumEt ;
  fPhiVals[13] =  fPCU     ;
  fPhiVals[14] =  fPCUPhi  ;
  fPhiVals[15] =  fJSPt1   ;
  fPhiVals[16] =  fJSEta1  ;
  fPhiVals[17] =  fJSPhi1  ;
  fPhiVals[18] =  fJSPt2   ;
  fPhiVals[19] =  fJSEta2  ;
  fPhiVals[20] =  fJSPhi2  ;
  fPhiVals[21] =  fNAllJet ;
  fPhiVals[22] =  fNJet    ;
  return fPhiReader->GetResponse(fPhiVals);
}
//--------------------------------------------------------------------------------------------------
Double_t MVAMet::evaluateU1() { 
  fU1Vals[0]  =  fNPV     ;
  fU1Vals[1]  =  fU       ;
  fU1Vals[2]  =  fUPhi    ;
  fU1Vals[3]  =  fTKSumEt ;
  fU1Vals[4]  =  fTKU     ;
  fU1Vals[5]  =  fTKUPhi  ;
  fU1Vals[6]  =  fNPSumEt ;
  fU1Vals[7]  =  fNPU     ;
  fU1Vals[8]  =  fNPUPhi  ;
  fU1Vals[9]  =  fPUSumEt ;
  fU1Vals[10] =  fPUMet   ;
  fU1Vals[11] =  fPUMetPhi;
  fU1Vals[12] =  fPCSumEt ;
  fU1Vals[13] =  fPCU     ;
  fU1Vals[14] =  fPCUPhi  ;
  fU1Vals[15] =  fJSPt1   ;
  fU1Vals[16] =  fJSEta1  ;
  fU1Vals[17] =  fJSPhi1  ;
  fU1Vals[18] =  fJSPt2   ;
  fU1Vals[19] =  fJSEta2  ;
  fU1Vals[20] =  fJSPhi2  ;
  fU1Vals[21] =  fNAllJet ;
  fU1Vals[22] =  fNJet    ;
  fU1Vals[23] =  fUPhiMVA ;
  return fU1Reader->GetResponse(fU1Vals);
}
//--------------------------------------------------------------------------------------------------
Double_t MVAMet::MVAValue(  Bool_t iPhi, 
			    Float_t iPFSumEt, 
			    Float_t iU      ,
			    Float_t iUPhi   ,
			    Float_t iTKSumEt,
			    Float_t iTKU    ,
			    Float_t iTKUPhi ,
			    Float_t iNPSumEt,
			    Float_t iNPU    ,
			    Float_t iNPUPhi ,
			    Float_t iPUSumEt,
			    Float_t iPUMet  ,
			    Float_t iPUMetPhi,
			    Float_t iPCSumEt,
			    Float_t iPCU    ,
			    Float_t iPCUPhi ,
			    Float_t iJSPt1  ,
			    Float_t iJSEta1 ,
			    Float_t iJSPhi1 ,
			    Float_t iJSPt2  ,
			    Float_t iJSEta2 ,
			    Float_t iJSPhi2 ,
			    Float_t iNAllJet,
			    Float_t iNJet   ,
			    Float_t iNPV    
			    ){
  
  if (!fIsInitialized) { 
    std::cout << "Error: MVA Met not properly initialized.\n"; 
    return -9999;
  }
    
  fU        = iU      ;
  fUPhi     = iUPhi   ;
  fTKSumEt  = iTKSumEt/iPFSumEt;
  fTKU      = iTKU    ;
  fTKUPhi   = iTKUPhi ;
  fNPSumEt  = iNPSumEt/iPFSumEt;
  fNPU      = iNPU    ;
  fNPUPhi   = iNPUPhi ;
  fPUSumEt  = iPUSumEt/iPFSumEt;
  fPUMet    = iPUMet  ;
  fPUMetPhi = iPUMetPhi;
  fPCSumEt  = iPCSumEt/iPFSumEt;
  fPCU      = iPCU    ;
  fPCUPhi   = iPCUPhi ;
  fJSPt1    = iJSPt1  ;
  fJSEta1   = iJSEta1 ;
  fJSPhi1   = iJSPhi1 ;
  fJSPt2    = iJSPt2  ;
  fJSEta2   = iJSEta2 ;
  fJSPhi2   = iJSPhi2 ;
  fNAllJet  = iNAllJet;
  fNJet     = iNJet   ;
  fNPV      = iNPV    ;

  Double_t lMVA = -9999;  
  lMVA = evaluatePhi();
  if(!iPhi) fUPhiMVA = iUPhi+lMVA;
  //Not no nice feature of the training
  fTKSumEt  /= iPFSumEt;
  fNPSumEt  /= iPFSumEt;
  fPUSumEt  /= iPFSumEt;
  fPCSumEt  /= iPFSumEt; 
  if(!iPhi) lMVA  = evaluateU1();
  return lMVA;
}
//--------------------------------------------------------------------------------------------------
//====> Please not that the jet collection must be cleaned => all jets near leptons must be removed
Met MVAMet::GetMet(	Bool_t iPhi,Float_t iPtVis,Float_t iPhiVis,Float_t iSumEtVis,
			const PFMet            *iMet  ,
			const PFCandidateCol   *iCands,const Vertex *iVertex,
			const PFJetCol         *iJets ,
			FactorizedJetCorrector *iJetCorrector,
			const PileupEnergyDensityCol *iPUEnergyDensity,
			int iNPV,
			Bool_t printDebug) {
  
  Met lPFRec = fRecoilTools->pfRecoil   (iPtVis,iPhiVis,iSumEtVis,iMet);
  Met lTKRec = fRecoilTools->trackRecoil(iPtVis,iPhiVis,iSumEtVis,iCands,iVertex); 
  Met lNPRec = fRecoilTools->NoPURecoil (iPtVis,iPhiVis,iSumEtVis,iJets,iJetCorrector,iPUEnergyDensity,iCands,iVertex);  
  Met lPCRec = fRecoilTools->PUCRecoil  (iPtVis,iPhiVis,iSumEtVis,iJets,iJetCorrector,iPUEnergyDensity,iCands,iVertex);
  Met lPUMet = fRecoilTools->PUMet      (iJets,iJetCorrector,iPUEnergyDensity,iCands,iVertex);
  
  Double_t lPt0 = 0; const PFJet *lLead = 0; 
  Double_t lPt1 = 0; const PFJet *l2nd  = 0; 
  int lNAllJet  = 0;
  int lNJet     = 0;
  for(unsigned int i0 = 0; i0 < iJets->GetEntries(); i0++) {
    const PFJet *pJet = iJets->At(i0);
    Double_t pPt = fRecoilTools->fJetIDMVA->correctedPt(pJet,iJetCorrector,iPUEnergyDensity);
    if( pJet->TrackCountingHighEffBJetTagsDisc() == -100 && pPt < 10.) continue;
    if(!JetTools::passPFLooseId(pJet)) continue;
    lNAllJet++;
    if(pPt  > 30)  lNJet++;
    if(lPt0 < pPt) {lPt0 = pPt; lLead = pJet; continue;}    
    if(lPt1 < pPt) {lPt1 = pPt; l2nd  = pJet; continue;}
  }
  
  fU        = lPFRec.Pt();
  fUPhi     = lPFRec.Phi();
  fTKSumEt  = lTKRec.SumEt()/lPFRec.SumEt();
  fTKU      = lTKRec.Pt();
  fTKUPhi   = lTKRec.Phi();
  fNPSumEt  = lNPRec.SumEt()/lPFRec.SumEt();
  fNPU      = lNPRec.Pt();
  fNPUPhi   = lNPRec.Phi();
  fPUSumEt  = lPUMet.SumEt()/lPFRec.SumEt();
  fPUMet    = lPUMet.Pt();
  fPUMetPhi = lPUMet.Phi();
  fPCSumEt  = lPCRec.SumEt()/lPFRec.SumEt();
  fPCU      = lPCRec.Pt()    ;
  fPCUPhi   = lPCRec.Phi()   ;
  fJSPt1    = lPt0; 
  fJSEta1   = 0; if(lLead != 0) fJSEta1 = lLead->Eta();
  fJSPhi1   = 0; if(lLead != 0) fJSPhi1 = lLead->Phi();
  fJSPt2    = lPt1; 
  fJSEta2   = 0; if(l2nd  != 0) fJSEta2 = l2nd ->Eta();
  fJSPhi2   = 0; if(l2nd  != 0) fJSPhi2 = l2nd ->Phi();
  fNJet     = lNJet   ;
  fNAllJet  = lNAllJet;
  fNPV      = iNPV    ;

  Float_t lMVA = evaluatePhi();
  
  if(!iPhi) fUPhiMVA = fUPhi + lMVA; 
  //Not no nice feature of teh training
  fTKSumEt  /= lPFRec.SumEt();
  fNPSumEt  /= lPFRec.SumEt();
  fPUSumEt  /= lPFRec.SumEt();
  fPCSumEt  /= lPFRec.SumEt();
  if(!iPhi) lMVA     = evaluateU1();//fU1Reader    ->EvaluateMVA( fU1MethodName );  

  TLorentzVector lUVec(0,0,0,0);   lUVec .SetPtEtaPhiM(fU*lMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec(0,0,0,0);   lVVec .SetPtEtaPhiM(iPtVis ,0,iPhiVis ,0);
  if(lMVA < 0) lUVec .RotateZ(TMath::Pi());                                                   
  lUVec      -= lVVec;
  Met lMet(lUVec.Px(),lUVec.Py());

  if (printDebug == kTRUE) {
    std::cout << "Debug Jet MVA: "
	      <<  fU        << " : "
	      <<  fUPhi     << " : "
	      <<  fTKSumEt  << " : "
	      <<  fTKU      << " : "
	      <<  fTKUPhi   << " : "
	      <<  fNPSumEt  << " : "
	      <<  fNPU      << " : "
	      <<  fNPUPhi   << " : "
	      <<  fPUSumEt  << " : "
	      <<  fPUMet    << " : "
	      <<  fPUMetPhi << " : "
	      <<  fPCUPhi   << " : "
	      <<  fPCSumEt  << " : "
	      <<  fPCU     << " : "
	      <<  fPCUPhi  << " : "
	      <<  fJSPt1   << " : "
	      <<  fJSEta1  << " : "
	      <<  fJSPhi1  << " : "
	      <<  fJSPt2   << " : "
	      <<  fJSEta2  << " : "
	      <<  fJSPhi2  << " : "
	      <<  fNJet    << " : "
	      <<  fNAllJet << " : "
	      <<  fNPV     << " : "
              << " === : === "
              << std::endl;
  }

  return lMet;
}
//--------------------------------------------------------------------------------------------------
//====> Please not that the jet collection must be cleaned => all jets near leptons must be removed
//====> Corrected Jets
Met MVAMet::GetMet(	Bool_t iPhi,Float_t iPtVis,Float_t iPhiVis,Float_t iSumEtVis,
			const PFMet            *iMet  ,
			const PFCandidateCol   *iCands,const Vertex *iVertex,
			const PFJetCol         *iJets ,
			int iNPV,
			Bool_t printDebug) {
  
  Met lPFRec = fRecoilTools->pfRecoil   (iPtVis,iPhiVis,iSumEtVis,iMet);
  Met lTKRec = fRecoilTools->trackRecoil(iPtVis,iPhiVis,iSumEtVis,iCands,iVertex); 
  Met lNPRec = fRecoilTools->NoPURecoil (iPtVis,iPhiVis,iSumEtVis,iJets,iCands,iVertex);  
  Met lPCRec = fRecoilTools->PUCRecoil  (iPtVis,iPhiVis,iSumEtVis,iJets,iCands,iVertex);
  Met lPUMet = fRecoilTools->PUMet      (iJets,iCands,iVertex);
  
  Double_t lPt0 = 0; const PFJet *lLead = 0; 
  Double_t lPt1 = 0; const PFJet *l2nd  = 0; 
  int lNAllJet  = 0;
  int lNJet     = 0;
  for(unsigned int i0 = 0; i0 < iJets->GetEntries(); i0++) {
    const PFJet *pJet = iJets->At(i0);
    Double_t pPt = pJet->Pt();
    if( pJet->TrackCountingHighEffBJetTagsDisc() == -100 && pPt < 10.) continue;
    if(!JetTools::passPFLooseId(pJet)) continue;
    lNAllJet++;
    if(pPt  > 30)  lNJet++;
    if(lPt0 < pPt) {lPt0 = pPt; lLead = pJet; continue;}    
    if(lPt1 < pPt) {lPt1 = pPt; l2nd  = pJet; continue;}
  }
  
  fU        = lPFRec.Pt();
  fUPhi     = lPFRec.Phi();
  fTKSumEt  = lTKRec.SumEt()/lPFRec.SumEt();
  fTKU      = lTKRec.Pt();
  fTKUPhi   = lTKRec.Phi();
  fNPSumEt  = lNPRec.SumEt()/lPFRec.SumEt();
  fNPU      = lNPRec.Pt();
  fNPUPhi   = lNPRec.Phi();
  fPUSumEt  = lPUMet.SumEt()/lPFRec.SumEt();
  fPUMet    = lPUMet.Pt();
  fPUMetPhi = lPUMet.Phi();
  fPCSumEt  = lPCRec.SumEt()/lPFRec.SumEt();
  fPCU      = lPCRec.Pt()    ;
  fPCUPhi   = lPCRec.Phi()   ;
  fJSPt1    = lPt0; 
  fJSEta1   = 0; if(lLead != 0) fJSEta1 = lLead->Eta();
  fJSPhi1   = 0; if(lLead != 0) fJSPhi1 = lLead->Phi();
  fJSPt2    = lPt1; 
  fJSEta2   = 0; if(l2nd  != 0) fJSEta2 = l2nd ->Eta();
  fJSPhi2   = 0; if(l2nd  != 0) fJSPhi2 = l2nd ->Phi();
  fNJet     = lNJet   ;
  fNAllJet  = lNAllJet;
  fNPV      = iNPV    ;

  Float_t lMVA = evaluatePhi();
  
  if(!iPhi) fUPhiMVA = fUPhi + lMVA; 
  //Not no nice feature of teh training
  fTKSumEt  /= lPFRec.SumEt();
  fNPSumEt  /= lPFRec.SumEt();
  fPUSumEt  /= lPFRec.SumEt();
  fPCSumEt  /= lPFRec.SumEt();
  if(!iPhi) lMVA     = evaluateU1();//fU1Reader    ->EvaluateMVA( fU1MethodName );  

  TLorentzVector lUVec(0,0,0,0);   lUVec .SetPtEtaPhiM(fU*lMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec(0,0,0,0);   lVVec .SetPtEtaPhiM(iPtVis ,0,iPhiVis ,0);
  if(lMVA < 0) lUVec .RotateZ(TMath::Pi());                                                   
  lUVec      -= lVVec;
  Met lMet(lUVec.Px(),lUVec.Py());

  if (printDebug == kTRUE) {
    std::cout << "Debug Jet MVA: "
	      <<  fU        << " : "
	      <<  fUPhi     << " : "
	      <<  fTKSumEt  << " : "
	      <<  fTKU      << " : "
	      <<  fTKUPhi   << " : "
	      <<  fNPSumEt  << " : "
	      <<  fNPU      << " : "
	      <<  fNPUPhi   << " : "
	      <<  fPUSumEt  << " : "
	      <<  fPUMet    << " : "
	      <<  fPUMetPhi << " : "
	      <<  fPCUPhi   << " : "
	      <<  fPCSumEt  << " : "
	      <<  fPCU     << " : "
	      <<  fPCUPhi  << " : "
	      <<  fJSPt1   << " : "
	      <<  fJSEta1  << " : "
	      <<  fJSPhi1  << " : "
	      <<  fJSPt2   << " : "
	      <<  fJSEta2  << " : "
	      <<  fJSPhi2  << " : "
	      <<  fNJet    << " : "
	      <<  fNAllJet << " : "
	      <<  fNPV     << " : "
              << " === : === "
              << std::endl;
  }

  return lMet;
}
//--------------------------------------------------------------------------------------------------
//Interms of the two candidates
Met MVAMet::GetMet(	Bool_t iPhi,
			Float_t iPt1,Float_t iPhi1,Float_t iEta1,
			Float_t iPt2,Float_t iPhi2,Float_t iEta2,
			const PFMet            *iMet  ,
			const PFCandidateCol   *iCands,const Vertex *iVertex,
			const PFJetCol         *iJets ,
			FactorizedJetCorrector *iJetCorrector,
			const PileupEnergyDensityCol *iPUEnergyDensity,
			int iNPV,
			Bool_t printDebug) {
  
  TLorentzVector lVVec1(0,0,0,0);   lVVec1.SetPtEtaPhiM(iPt1,0,iPhi1 ,0);
  TLorentzVector lVVec2(0,0,0,0);   lVVec2.SetPtEtaPhiM(iPt2,0,iPhi2 ,0);
  lVVec1+=lVVec2;
  Float_t lPtVis    = lVVec1.Pt();
  Float_t lPhiVis   = lVVec1.Phi();
  Float_t lSumEtVis = iPt1 + iPt2;
  Met lPFRec = fRecoilTools->pfRecoil   (lPtVis,lPhiVis,lSumEtVis,iMet);
  Met lTKRec = fRecoilTools->trackRecoil(lPtVis,lPhiVis,lSumEtVis,iCands,iVertex); 
  Met lNPRec = fRecoilTools->NoPURecoil (lPtVis,lPhiVis,lSumEtVis,iJets,iJetCorrector,iPUEnergyDensity,iCands,iVertex,iPhi1,iEta1,iPhi2,iEta2);  
  Met lPCRec = fRecoilTools->PUCRecoil  (lPtVis,lPhiVis,lSumEtVis,iJets,iJetCorrector,iPUEnergyDensity,iCands,iVertex,iPhi1,iEta1,iPhi2,iEta2);
  Met lPUMet = fRecoilTools->PUMet      (                         iJets,iJetCorrector,iPUEnergyDensity,iCands,iVertex,iPhi1,iEta1,iPhi2,iEta2);

  Double_t lPt0 = 0; const PFJet *lLead = 0; 
  Double_t lPt1 = 0; const PFJet *l2nd  = 0; 
  int lNAllJet  = 0;
  int lNJet     = 0;
  for(unsigned int i0 = 0; i0 < iJets->GetEntries(); i0++) {
    const PFJet *pJet = iJets->At(i0);
    Double_t pPt = fRecoilTools->fJetIDMVA->correctedPt(pJet,iJetCorrector,iPUEnergyDensity);
    if( pJet->TrackCountingHighEffBJetTagsDisc() == -100 && pPt < 10.) continue;
    double pDEta1 = pJet->Eta() - iEta1;
    double pDPhi1 = fabs(pJet->Phi() - iPhi1); if(pDPhi1 > 2.*TMath::Pi()-pDPhi1) pDPhi1 = 2.*TMath::Pi()-pDPhi1;
    double pDR1   = sqrt(pDEta1*pDEta1 + pDPhi1*pDPhi1);
    if(pDR1 < 0.5) continue;
    double pDEta2 = pJet->Eta() - iEta2;
    double pDPhi2 = fabs(pJet->Phi() - iPhi2); if(pDPhi2 > 2.*TMath::Pi()-pDPhi2) pDPhi2 = 2.*TMath::Pi()-pDPhi2;
    double pDR2   = sqrt(pDEta2*pDEta2 + pDPhi2*pDPhi2);
    if(pDR2 < 0.5) continue;  
    if(!JetTools::passPFLooseId(pJet)) continue;
    lNAllJet++;
    if(pPt  > 30.)  lNJet++;
    if(lPt0 < pPt) {lPt0 = pPt; lLead = pJet; continue;}    
    if(lPt1 < pPt) {lPt1 = pPt; l2nd  = pJet; continue;}
  }
  
  fU       = lPFRec.Pt();
  fUPhi    = lPFRec.Phi();
  fTKSumEt = lTKRec.SumEt()/lPFRec.SumEt();
  fTKU     = lTKRec.Pt();
  fTKUPhi  = lTKRec.Phi();
  fNPSumEt = lNPRec.SumEt()/lPFRec.SumEt();
  fNPU     = lNPRec.Pt();
  fNPUPhi  = lNPRec.Phi();
  fPUSumEt = lPUMet.SumEt()/lPFRec.SumEt();
  fPUMet   = lPUMet.Pt();
  fPUMetPhi= lPUMet.Phi();
  fPCSumEt = lPCRec.SumEt()/lPFRec.SumEt();
  fPCU     = lPCRec.Pt()    ;
  fPCUPhi  = lPCRec.Phi()   ;
  fJSPt1   = lPt0; 
  fJSEta1  = 0; if(lLead != 0) fJSEta1 = lLead->Eta();
  fJSPhi1  = 0; if(lLead != 0) fJSPhi1 = lLead->Phi();
  fJSPt2   = lPt1; 
  fJSEta2  = 0; if(l2nd  != 0) fJSEta2 = l2nd ->Eta();
  fJSPhi2  = 0; if(l2nd  != 0) fJSPhi2 = l2nd ->Phi();
  fNJet    = lNJet   ;
  fNAllJet = lNAllJet;
  fNPV     = iNPV    ;

  Float_t lMVA = evaluatePhi();//fPhiReader                 ->EvaluateMVA( fPhiMethodName );
  
  if(!iPhi) fUPhiMVA = fUPhi + lMVA; 
  fTKSumEt  /= lPFRec.SumEt();
  fNPSumEt  /= lPFRec.SumEt();
  fPUSumEt  /= lPFRec.SumEt();
  fPCSumEt  /= lPFRec.SumEt();
  if(!iPhi) lMVA     = evaluateU1();//fU1Reader    ->EvaluateMVA( fU1MethodName );  

  TLorentzVector lUVec (0,0,0,0);   lUVec .SetPtEtaPhiM(fU*lMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec (0,0,0,0);   lVVec .SetPtEtaPhiM(lPtVis ,0,lPhiVis ,0);
  if(lMVA < 0) lUVec .RotateZ(TMath::Pi());                                                   
  lUVec      -= lVVec;
  Met lMet(lUVec.Px(),lUVec.Py());

  if (printDebug == kTRUE) {
    std::cout << "Debug Jet MVA: "
	      <<  fU        << " : "
	      <<  fUPhi     << " : "
	      <<  fTKSumEt  << " : "
	      <<  fTKU      << " : "
	      <<  fTKUPhi   << " : "
	      <<  fNPSumEt  << " : "
	      <<  fNPU      << " : "
	      <<  fNPUPhi   << " : "
	      <<  fPUSumEt  << " : "
	      <<  fPUMet    << " : "
	      <<  fPUMetPhi << " : "
	      <<  fPCSumEt  << " : "
	      <<  fPCU      << " : "
	      <<  fPCUPhi   << " : "
	      <<  fJSPt1    << " : "
	      <<  fJSEta1   << " : "
	      <<  fJSPhi1   << " : "
	      <<  fJSPt2    << " : "
	      <<  fJSEta2   << " : "
	      <<  fJSPhi2   << " : "
	      <<  fNJet     << " : "
	      <<  fNAllJet  << " : "
	      <<  fNPV      << " : "
              << " === : === "
              << std::endl;
  }

  return lMet;
}
//--------------------------------------------------------------------------------------------------
//Interms of the two candidates => corrected Jets
Met MVAMet::GetMet(	Bool_t iPhi,
			Float_t iPt1,Float_t iPhi1,Float_t iEta1,
			Float_t iPt2,Float_t iPhi2,Float_t iEta2,
			const PFMet            *iMet  ,
			const PFCandidateCol   *iCands,const Vertex *iVertex,
			const PFJetCol         *iJets ,
			int iNPV,
			Bool_t printDebug) {
  
  TLorentzVector lVVec1(0,0,0,0);   lVVec1.SetPtEtaPhiM(iPt1,0,iPhi1 ,0);
  TLorentzVector lVVec2(0,0,0,0);   lVVec2.SetPtEtaPhiM(iPt2,0,iPhi2 ,0);
  lVVec1+=lVVec2;
  Float_t lPtVis    = lVVec1.Pt();
  Float_t lPhiVis   = lVVec1.Phi();
  Float_t lSumEtVis = iPt1 + iPt2;
  Met lPFRec = fRecoilTools->pfRecoil   (lPtVis,lPhiVis,lSumEtVis,iMet);
  Met lTKRec = fRecoilTools->trackRecoil(lPtVis,lPhiVis,lSumEtVis,iCands,iVertex); 
  Met lNPRec = fRecoilTools->NoPURecoil (lPtVis,lPhiVis,lSumEtVis,iJets,iCands,iVertex,iPhi1,iEta1,iPhi2,iEta2);  
  Met lPCRec = fRecoilTools->PUCRecoil  (lPtVis,lPhiVis,lSumEtVis,iJets,iCands,iVertex,iPhi1,iEta1,iPhi2,iEta2);
  Met lPUMet = fRecoilTools->PUMet      (                         iJets,iCands,iVertex,iPhi1,iEta1,iPhi2,iEta2);

  Double_t lPt0 = 0; const PFJet *lLead = 0; 
  Double_t lPt1 = 0; const PFJet *l2nd  = 0; 
  int lNAllJet  = 0;
  int lNJet     = 0;
  for(unsigned int i0 = 0; i0 < iJets->GetEntries(); i0++) {
    const PFJet *pJet = iJets->At(i0);
    Double_t pPt = pJet->Pt();
    if( pJet->TrackCountingHighEffBJetTagsDisc() == -100 && pPt < 10.) continue;
    double pDEta1 = pJet->Eta() - iEta1;
    double pDPhi1 = fabs(pJet->Phi() - iPhi1); if(pDPhi1 > 2.*TMath::Pi()-pDPhi1) pDPhi1 = 2.*TMath::Pi()-pDPhi1;
    double pDR1   = sqrt(pDEta1*pDEta1 + pDPhi1*pDPhi1);
    if(pDR1 < 0.5) continue;
    double pDEta2 = pJet->Eta() - iEta2;
    double pDPhi2 = fabs(pJet->Phi() - iPhi2); if(pDPhi2 > 2.*TMath::Pi()-pDPhi2) pDPhi2 = 2.*TMath::Pi()-pDPhi2;
    double pDR2   = sqrt(pDEta2*pDEta2 + pDPhi2*pDPhi2);
    if(pDR2 < 0.5) continue;  
    if(!JetTools::passPFLooseId(pJet)) continue;
    lNAllJet++;
    if(pPt  > 30.)  lNJet++;
    if(lPt0 < pPt) {lPt0 = pPt; lLead = pJet; continue;}    
    if(lPt1 < pPt) {lPt1 = pPt; l2nd  = pJet; continue;}
  }
  
  fU       = lPFRec.Pt();
  fUPhi    = lPFRec.Phi();
  fTKSumEt = lTKRec.SumEt()/lPFRec.SumEt();
  fTKU     = lTKRec.Pt();
  fTKUPhi  = lTKRec.Phi();
  fNPSumEt = lNPRec.SumEt()/lPFRec.SumEt();
  fNPU     = lNPRec.Pt();
  fNPUPhi  = lNPRec.Phi();
  fPUSumEt = lPUMet.SumEt()/lPFRec.SumEt();
  fPUMet   = lPUMet.Pt();
  fPUMetPhi= lPUMet.Phi();
  fPCSumEt = lPCRec.SumEt()/lPFRec.SumEt();
  fPCU     = lPCRec.Pt()    ;
  fPCUPhi  = lPCRec.Phi()   ;
  fJSPt1   = lPt0; 
  fJSEta1  = 0; if(lLead != 0) fJSEta1 = lLead->Eta();
  fJSPhi1  = 0; if(lLead != 0) fJSPhi1 = lLead->Phi();
  fJSPt2   = lPt1; 
  fJSEta2  = 0; if(l2nd  != 0) fJSEta2 = l2nd ->Eta();
  fJSPhi2  = 0; if(l2nd  != 0) fJSPhi2 = l2nd ->Phi();
  fNJet    = lNJet   ;
  fNAllJet = lNAllJet;
  fNPV     = iNPV    ;

  Float_t lMVA = evaluatePhi();//fPhiReader                 ->EvaluateMVA( fPhiMethodName );
  
  if(!iPhi) fUPhiMVA = fUPhi + lMVA; 
  fTKSumEt  /= lPFRec.SumEt();
  fNPSumEt  /= lPFRec.SumEt();
  fPUSumEt  /= lPFRec.SumEt();
  fPCSumEt  /= lPFRec.SumEt();
  if(!iPhi) lMVA     = evaluateU1();//fU1Reader    ->EvaluateMVA( fU1MethodName );  

  TLorentzVector lUVec (0,0,0,0);   lUVec .SetPtEtaPhiM(fU*lMVA,0,fUPhiMVA,0);
  TLorentzVector lVVec (0,0,0,0);   lVVec .SetPtEtaPhiM(lPtVis ,0,lPhiVis ,0);
  if(lMVA < 0) lUVec .RotateZ(TMath::Pi());                                                   
  lUVec      -= lVVec;
  Met lMet(lUVec.Px(),lUVec.Py());

  if (printDebug == kTRUE) {
    std::cout << "Debug Jet MVA: "
	      <<  fU        << " : "
	      <<  fUPhi     << " : "
	      <<  fTKSumEt  << " : "
	      <<  fTKU      << " : "
	      <<  fTKUPhi   << " : "
	      <<  fNPSumEt  << " : "
	      <<  fNPU      << " : "
	      <<  fNPUPhi   << " : "
	      <<  fPUSumEt  << " : "
	      <<  fPUMet    << " : "
	      <<  fPUMetPhi << " : "
	      <<  fPCSumEt  << " : "
	      <<  fPCU      << " : "
	      <<  fPCUPhi   << " : "
	      <<  fJSPt1    << " : "
	      <<  fJSEta1   << " : "
	      <<  fJSPhi1   << " : "
	      <<  fJSPt2    << " : "
	      <<  fJSEta2   << " : "
	      <<  fJSPhi2   << " : "
	      <<  fNJet     << " : "
	      <<  fNAllJet  << " : "
	      <<  fNPV      << " : "
              << " === : === "
              << std::endl;
  }

  return lMet;
}
