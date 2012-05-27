// $Id: PhotonTools.cc,v 1.27 2012/05/25 19:41:11 bendavid Exp $

#include "MitPhysics/Utils/interface/PhotonTools.h"
#include "MitPhysics/Utils/interface/ElectronTools.h"
#include "MitPhysics/Utils/interface/IsolationTools.h"
#include "MitAna/DataTree/interface/StableData.h"
#include <TFile.h>
#include <TRandom3.h>

ClassImp(mithep::PhotonTools)

using namespace mithep;

//--------------------------------------------------------------------------------------------------
PhotonTools::PhotonTools()  
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
PhotonTools::eScaleCats PhotonTools::EScaleCat(const Photon *p)
{
  if (p->SCluster()->AbsEta()<1.0) {
    if (p->R9()>0.94) {
      Int_t ieta = p->SCluster()->Seed()->IEta();
      Int_t iphi = p->SCluster()->Seed()->IPhi();      
      Bool_t central = ( ((std::abs(ieta)-1)/5+1 >= 2 && (std::abs(ieta)-1)/5+1 < 5) || ((std::abs(ieta)-1)/5+1 >= 7 && (std::abs(ieta)-1)/5+1 < 9  ) || ((std::abs(ieta)-1)/5+1 >= 11 && (std::abs(ieta)-1)/5+1 < 13) || ((std::abs(ieta)-1)/5+1 >= 15 && (std::abs(ieta)-1)/5+1 < 17) ) && (iphi %20) > 5 && (iphi%20) <16;
      
      if (central) return kEBlowEtaGoldCenter;
      else return kEBlowEtaGoldGap;
    }
    else return kEBlowEtaBad;
  }
  else if (p->SCluster()->AbsEta()<1.5) {
    if (p->R9()>0.94) return kEBhighEtaGold;
    else return kEBhighEtaBad;    
  }
  else if (p->SCluster()->AbsEta()<2.0) {
    if (p->R9()>0.94) return kEElowEtaGold;
    else return kEElowEtaBad;    
  }
  else {
    if (p->R9()>0.94) return kEEhighEtaGold;
    else return kEEhighEtaBad;    
  }  
  
}

void PhotonTools::ScalePhoton(Photon* p, Double_t scale) {
  if( !p ) return;
  FourVectorM mom = p->Mom();
  p->SetMom(scale*mom.X(), scale*mom.Y(), scale*mom.Z(), scale*mom.E());
  
}

void PhotonTools::SmearPhoton(Photon* p, TRandom3* rng, Double_t width, UInt_t iSeed) {
  
  if( !p  )       return;
  if( !rng)       return;
  if( width < 0.) return;

  rng->SetSeed(iSeed);
  FourVectorM mom = p->Mom();
  Double_t scale = rng->Gaus(1.,width);

  if( scale > 0)
    p->SetMom(scale*mom.X(), scale*mom.Y(), scale*mom.Z(), scale*mom.E());
  
  return;
}

void PhotonTools::SmearPhotonError(Photon* p, Double_t width) {
  
  if( !p  )       return;
  if( width < 0.) return;  
  
  
  Double_t smear = p->EnergySmearing();
  p->SetEnergySmearing(TMath::Sqrt(smear*smear+width*width*p->E()*p->E()));
  
  Double_t err = p->EnergyErrSmeared();
  if (err>=0.0) {
    p->SetEnergyErrSmeared(TMath::Sqrt(err*err + width*width*p->E()*p->E()));
  }
  
}

void PhotonTools::ScalePhotonR9(Photon* p, Double_t scale) {
 p->SetR9(scale*p->R9()); 
}


void PhotonTools::ScalePhotonError(Photon* p, Double_t scale) {
 p->SetEnergyErrSmeared(scale*p->EnergyErrSmeared()); 
}

//--------------------------------------------------------------------------------------------------
Bool_t PhotonTools::PassConversionId(const Photon *p, const DecayParticle *c) {

  if (!c) return kTRUE;
  
  ThreeVector dirconvsc = ThreeVector(p->SCluster()->Point()) - c->Position();
  Double_t deta = c->Eta()-dirconvsc.Eta();
  Double_t dphi = MathUtils::DeltaPhi(c->Phi(),dirconvsc.Phi());
  Double_t eoverp = p->SCluster()->Energy()/c->P();
  
  if (p->IsEB() && eoverp>2.0) return kFALSE;
  if (p->IsEE() && eoverp>3.0) return kFALSE;
  
  if (p->IsEE() && TMath::Abs(deta)>0.01) return kFALSE;
  if (p->IsEE() && TMath::Abs(dphi)>0.01) return kFALSE;

  return kTRUE;
    
}

//--------------------------------------------------------------------------------------------------
Bool_t PhotonTools::PassElectronVeto(const Photon *p, const ElectronCol *els) {

  Bool_t pass = kTRUE;
  for (UInt_t i=0; i<els->GetEntries(); ++i) {
    const Electron *e = els->At(i);
    if (e->SCluster()==p->SCluster() && e->GsfTrk()->NExpectedHitsInner()==0) {
      pass = kFALSE;
    }
  }
  
  return pass;
}

//--------------------------------------------------------------------------------------------------
const Electron *PhotonTools::MatchedElectron(const Photon *p, const ElectronCol *els) {

  for (UInt_t i=0; i<els->GetEntries(); ++i) {
    const Electron *e = els->At(i);
    if ( e->SCluster()==p->SCluster() ) {
      return e;
    }
  }
  
  return 0;
}

//--------------------------------------------------------------------------------------------------
const Photon *PhotonTools::MatchedPhoton(const Electron *e, const PhotonCol *phs) {

  for (UInt_t i=0; i<phs->GetEntries(); ++i) {
    const Photon *p = phs->At(i);
    if ( p->SCluster()==e->SCluster() ) {
      return p;
    }
  }
  
  return 0;
}

//--------------------------------------------------------------------------------------------------
const SuperCluster *PhotonTools::MatchedSC(const SuperCluster *psc, const SuperClusterCol *scs, Double_t drMin) {

  Double_t drsmallest = 999.;
  const SuperCluster *match = 0;
  for (UInt_t i=0; i<scs->GetEntries(); ++i) {
    const SuperCluster *sc = scs->At(i);
    Double_t dr = MathUtils::DeltaR(*sc,*psc);
    if ( dr<drsmallest && dr<drMin ) {
      drsmallest = dr;
      match = sc;
    }
  }
  
  return match;
}

//--------------------------------------------------------------------------------------------------
const SuperCluster *PhotonTools::MatchedPFSC(const SuperCluster *psc, const PhotonCol *pfphos, const ElectronCol *eles, Double_t drMin) {

  
  if (pfphos) {
    for (UInt_t i=0; i<pfphos->GetEntries(); ++i) {
      const Photon *pfpho = pfphos->At(i);
      if (psc == pfpho->SCluster() && pfpho->PFSCluster()) return pfpho->PFSCluster();
    }
  }
  
  for (UInt_t i=0; i<eles->GetEntries(); ++i) {
    const Electron *ele = eles->At(i);
    if (psc == ele->SCluster() && ele->PFSCluster()) return ele->PFSCluster();
  }  

  Double_t drsmallest = 999.;
  const SuperCluster *match = 0;
  for (UInt_t i=0; i<eles->GetEntries(); ++i) {
    const Electron *ele = eles->At(i);
    //if (psc == ele->SCluster() && ele->HasPFSCluster()) return ele->PFSCluster();
    if (!ele->PFSCluster()) continue;
    
    Double_t dr = MathUtils::DeltaR(*ele->PFSCluster(),*psc);
    if ( dr<drsmallest && dr<drMin ) {
      drsmallest = dr;
      match = ele->PFSCluster();
    }    
  }  
    
  return match;
}

//--------------------------------------------------------------------------------------------------
Double_t PhotonTools::ElectronVetoCiC(const Photon *p, const ElectronCol *els) {
  
  for (UInt_t i=0; i<els->GetEntries(); ++i) {
    const Electron *e = els->At(i);
    if (e->SCluster()==p->SCluster() ) {
      //if( e->GsfTrk()->NExpectedHitsInner()==0 && e->GsfTrk()->Pt() > 2.5 ) {
      if( e->GsfTrk()->NExpectedHitsInner()==0 ) {
	double dEta = e->DeltaEtaSuperClusterTrackAtVtx();
	double dPhi = e->DeltaPhiSuperClusterTrackAtVtx();
	double dR = TMath::Sqrt(dEta*dEta+dPhi*dPhi);
	return dR;
      }
    }
  }  
  return 99.;
}

//--------------------------------------------------------------------------------------------------
Bool_t PhotonTools::PassElectronVetoConvRecovery(const Photon *p, const ElectronCol *els, const DecayParticleCol *conversions, const BaseVertex *v) {

  Bool_t pass = kTRUE;
  for (UInt_t i=0; i<els->GetEntries(); ++i) {
    const Electron *e = els->At(i);
    if (e->SCluster()==p->SCluster() && e->GsfTrk()->NExpectedHitsInner()==0 && ElectronTools::PassConversionFilter(e, conversions, 
                                                         v, 0, 1e-6, 2.0, kFALSE, kFALSE) ) {
      pass = kFALSE;
    }
  }
  
  return pass;
}

//--------------------------------------------------------------------------------------------------
Bool_t PhotonTools::PassTriggerMatching(const Photon *p, const TriggerObjectCol *trigobjs)
{
  
  for (UInt_t i=0; i<trigobjs->GetEntries(); ++i) {
    const TriggerObject *trigobj = trigobjs->At(i);
    if (trigobj->TriggerType()==TriggerObject::TriggerCluster || trigobj->TriggerType()==TriggerObject::TriggerElectron || trigobj->TriggerType()==TriggerObject::TriggerPhoton) {
      if (MathUtils::DeltaR(p->SCluster(),trigobj)<0.3) {
        return kTRUE;
      }
    }
  }
  
  return kFALSE;
  
  
}

//--------------------------------------------------------------------------------------------------
const DecayParticle *PhotonTools::MatchedConversion(const Photon *p, const DecayParticleCol *conversions, 
                                               const BaseVertex *vtx, Int_t nWrongHitsMax, Double_t probMin,
                                               Double_t lxyMin, Double_t dRMin) {
  
  return MatchedConversion(p->SCluster(), conversions, vtx, nWrongHitsMax, probMin, lxyMin, dRMin);
  
}

//--------------------------------------------------------------------------------------------------
const DecayParticle *PhotonTools::MatchedConversion(const SuperCluster *sc, const DecayParticleCol *conversions, 
                                               const BaseVertex *vtx, Int_t nWrongHitsMax, Double_t probMin,
                                               Double_t lxyMin, Double_t dRMin) {
  
  const DecayParticle *match = 0;
  Double_t rhosmallest = 999.;
  for (UInt_t i=0; i<conversions->GetEntries(); ++i) {
    const DecayParticle *c = conversions->At(i);
    ThreeVector dirconvsc = ThreeVector(sc->Point()) - c->Position();
    Double_t dr = MathUtils::DeltaR(*c,dirconvsc);
    Double_t rho = c->Position().Rho();
    if (dr<dRMin && rho<rhosmallest && c->Prob()>probMin && c->LxyCorrected(vtx)>lxyMin) {
      Int_t nhb1 = dynamic_cast<const StableData*>(c->DaughterDat(0))->NHitsBeforeVtx();
      Int_t nhb2 = dynamic_cast<const StableData*>(c->DaughterDat(1))->NHitsBeforeVtx();
      if (TMath::Max(nhb1,nhb2)<=nWrongHitsMax) {
        rhosmallest = rho;
        match = c;
      }
    }
    
  }
  
  return match;
  
}

//--------------------------------------------------------------------------------------------------
const DecayParticle *PhotonTools::MatchedConversion(const Track *t, const DecayParticleCol *conversions, 
                                               const BaseVertex *vtx, Int_t nWrongHitsMax, Double_t probMin,
                                               Double_t lxyMin) {
  
  for (UInt_t i=0; i<conversions->GetEntries(); ++i) {
    const DecayParticle *c = conversions->At(i);
    if (c->Prob()>probMin && c->LxyCorrected(vtx)>lxyMin) {
      Int_t nhb1 = dynamic_cast<const StableData*>(c->DaughterDat(0))->NHitsBeforeVtx();
      Int_t nhb2 = dynamic_cast<const StableData*>(c->DaughterDat(1))->NHitsBeforeVtx();
      if (TMath::Max(nhb1,nhb2)<=nWrongHitsMax) {
        const Track *ct1 = dynamic_cast<const ChargedParticle*>(c->Daughter(0))->Trk();
        const Track *ct2 = dynamic_cast<const ChargedParticle*>(c->Daughter(1))->Trk();
        if (t==ct1 || t==ct2) return c;
      }
    }
    
  }
  
  return 0;
  
}

PhotonTools::DiphotonR9EtaCats PhotonTools::DiphotonR9EtaCat(const Photon *p1, const Photon *p2) {
  
  if (p1->IsEB() && p2->IsEB()) {
    if (p1->R9()>0.93 && p2->R9()>0.93) return kCat1;
    else return kCat2;
    
  }
  else {
    if (p1->R9()>0.93 && p2->R9()>0.93) return kCat3;
    else return kCat4; 
  }
  
}

PhotonTools::DiphotonR9EtaConversionCats PhotonTools::DiphotonR9EtaConversionCat(const Photon *p1, const Photon *p2, const DecayParticleCol *conversions, const BaseVertex *v) {
  
  const DecayParticle *conv1 = MatchedConversion(p1, conversions, v);
  const DecayParticle *conv2 = MatchedConversion(p2, conversions, v);
    
  if (p1->IsEB() && p2->IsEB()) {
    if (p1->R9()>0.93 && p2->R9()>0.93) return kNewCat1;
    else if (conv1||conv2) return kNewCat2;
    else return kNewCat3;
    
  }
  else {
    if (p1->R9()>0.93 && p2->R9()>0.93) return kNewCat4;
    else if (conv1||conv2) return kNewCat5;
    else return kNewCat6;
  }
  
}

PhotonTools::CiCBaseLineCats PhotonTools::CiCBaseLineCat(const Photon *p) {
  if( p->SCluster()->AbsEta()<1.5 ) {
    if ( p->R9() > 0.94 ) return kCiCCat1;
    else return kCiCCat2;
  } else {
    if ( p->R9() > 0.94 ) return kCiCCat3;
    else return kCiCCat4;
  }

  // shoud NEVER happen, but you never know...
  return kCiCNoCat;
}

//--------------------------------------------------------------------------------------------------
const DecayParticle *PhotonTools::MatchedCiCConversion(const Photon *p, const DecayParticleCol *conversions, 
						       Double_t dPhiMin,
						       Double_t dEtaMin,
						       Double_t dRMin,
						       bool print) {
  
  // if there are no conversons, return
  if ( !p || !conversions)  return NULL;

  const DecayParticle *match = NULL;

  double minDeta = 999.;
  double minDphi = 999.;
  double minDR   = 999.;

  double phPhi = p->SCluster()->Phi();
  double phEta = p->SCluster()->Eta();

  if(print)
    std::cout<<"  --- conv match photon eta = "<<phEta<<"  phi = "<<phPhi<<std::endl;  


  for (UInt_t i=0; i<conversions->GetEntries(); ++i) {
    const DecayParticle *c = conversions->At(i);

    if(print)
      std::cout<< "   c "<<i+1<<"  pt = "<<c->Pt()<<std::endl;

    if(c->Pt()   < 1. )    continue; // is this refittedPirMomentum?

    if (c->Prob() < 1e-6) continue;

    //ThreeVector dirconvsc = ThreeVector(p->SCluster()->Point()) - c->Position();
    //ThreeVector dirconvsc = p->CaloPos() - c->Position();

    double convPhi = c->Phi();
    double convEta = c->Eta();
    
    const ThreeVector wrong(0,0,0);
    double Zvertex = c->DzCorrected(wrong);
    // ------------------ FROM GLOBE ----------------------
    //---Definitions for ECAL
    const float R_ECAL           = 136.5;
    const float Z_Endcap         = 328.0;
    const float etaBarrelEndcap  = 1.479; 
    
    //---ETA correction
    float Theta = 0.0  ; 
    float ZEcal = R_ECAL*sinh(convEta)+Zvertex;
    
    if(ZEcal != 0.0) Theta = TMath::ATan(R_ECAL/ZEcal);
    if(Theta<0.0) Theta = Theta+M_PI;
    double fEta = - TMath::Log(TMath::Tan(0.5*Theta));
    
    if( fabs(fEta) > etaBarrelEndcap ) {
      float Zend = Z_Endcap ;
      if(convEta<0.0 )  Zend = -Zend ;
      float Zlen = Zend - Zvertex ;
      float RR = Zlen/TMath::SinH(convEta); 
      Theta = TMath::ATan(RR/Zend);
      if(Theta<0.0) Theta = Theta+M_PI;
      fEta = -TMath::Log(TMath::Tan(0.5*Theta));	      
    } 
    // ---------------------------------------------------

    if(print) {
      std::cout<<"          eta bare = "<<convEta<<std::endl;
      std::cout<<"          eta new  = "<<fEta<<std::endl;
      std::cout<<"          phi      = "<<convPhi<<std::endl;
    }

    convEta = fEta;
    
    Double_t dphi = TMath::Abs(phPhi - convPhi);
    if(dphi > M_PI) dphi = 2.*M_PI-dphi;
    //Double_t deta = c->Eta()-dirconvsc.Eta();
    Double_t deta = TMath::Abs(convEta-phEta);
    Double_t dR = TMath::Sqrt(dphi*dphi+deta*deta);
    //if(dphi < minDphi && TMath::Abs(deta)<minDeta) {
    if(dR < minDR) {
      minDR = dR;
      minDphi = dphi;
      minDeta = TMath::Abs(deta);
      match = c;

      if(print)
	std::cout<<" conv "<<i+1<<" matches with dPhi = "<<minDphi<<"   dEta = "<<minDeta<<std::endl;

    } 
  }
  
  //if(minDphi < dPhiMin && minDeta < dEtaMin)
  if(minDR < dRMin)
    return match;
  else 
    return NULL;
}

bool PhotonTools::PassCiCSelection(const Photon* ph, const Vertex* vtx, 
				   const TrackCol* trackCol,
				   const ElectronCol* eleCol,
				   const VertexCol* vtxCol,
				   double rho, double ptmin, 
				   bool applyEleVeto,
				   bool print, float* kin) {

  
  // these values are taken from the H2GGlobe code... (actually from Marco/s mail)
  float cic4_allcuts_temp_sublead[] = { 
    3.8,         2.2,         1.77,        1.29,
    11.7,        3.4,         3.9,         1.84,
    3.5,         2.2,         2.3,         1.45,
    0.0106,      0.0097,      0.028,       0.027,
    0.082,       0.062,       0.065,       0.048,
    0.94,        0.36,        0.94,        0.32,
    1.,          0.062,       0.97,        0.97,
    1.5,         1.5,         1.5,         1.5 };  // the last line is PixelmatchVeto and un-used
  
  // cut on Et instead of Pt???    
  Bool_t isbarrel = ph->SCluster()->AbsEta()<1.5;
    
  // compute all relevant observables first
  double ecalIso3 = ph->EcalRecHitIsoDr03();
  double ecalIso4 = ph->EcalRecHitIsoDr04();
  double hcalIso4 = ph->HcalTowerSumEtDr04();

  unsigned int wVtxInd = 0;

  double trackIso1 = IsolationTools::CiCTrackIsolation(ph, vtx, 0.3, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol);

  // track iso only
  double trackIso3 = IsolationTools::CiCTrackIsolation(ph, vtx, 0.3, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol);

  // track iso worst vtx
  double trackIso2 = IsolationTools::CiCTrackIsolation(ph, vtx, 0.4, 0.02, 0.0, 0.0, 0.1, 1.0, trackCol, &wVtxInd, vtxCol);
  
  double combIso1 = ecalIso3+hcalIso4+trackIso1 - 0.17*rho;
  double combIso2 = ecalIso4+hcalIso4+trackIso2 - 0.52*rho;
  
  double tIso1 = (combIso1) *50./ph->Et();
  double tIso2 = (combIso2) *50./(ph->MomVtx(vtxCol->At(wVtxInd)->Position()).Pt());
  //double tIso2 = (combIso2) *50./ph->Et();
  double tIso3 = (trackIso3)*50./ph->Et();
  
  double covIEtaIEta  =ph->CoviEtaiEta();
  double HoE = ph->HadOverEm();
  
  double R9 = ph->R9();
  
  double dRTrack = PhotonTools::ElectronVetoCiC(ph, eleCol);

  // check which category it is ...
  int _tCat = 1;
  if ( !isbarrel ) _tCat = 3;
  if ( R9 < 0.94 ) _tCat++;
  
  if(print) {
    std::cout<<" -------------------------- "<<std::endl;

    std::cout<<" trackIso1  = "<<trackIso1<<std::endl;
    std::cout<<" trackIso2  = "<<trackIso2<<std::endl;
    std::cout<<" trackIso3  = "<<trackIso3<<std::endl;
    std::cout<<" ecalIso3   = "<<ecalIso3<<std::endl;
    std::cout<<" ecalIso4   = "<<ecalIso4<<std::endl;
    std::cout<<" hcalIso4   = "<<hcalIso4<<std::endl;

    std::cout<<" photon Et  = "<<ph->Et()<<std::endl;
   std::cout<<"        Eta = "<<ph->SCluster()->Eta()<<std::endl;
    std::cout<<"        HoE = "<<HoE<<"   ("<<cic4_allcuts_temp_sublead[_tCat-1+4*4]<<")"<<std::endl;
    std::cout<<"         R9 = "<<R9<<"   ("<<cic4_allcuts_temp_sublead[_tCat-1+5*4]<<")"<<std::endl;
    std::cout<<"         dR = "<<dRTrack<<"   ("<<cic4_allcuts_temp_sublead[_tCat-1+6*4]<<")"<<std::endl;
    std::cout<<"       iso1 = "<<tIso1<<"   ("<<cic4_allcuts_temp_sublead[_tCat-1+0*4]<<")"<<std::endl; 
    std::cout<<"       iso2 = "<<tIso2<<"   ("<<cic4_allcuts_temp_sublead[_tCat-1+1*4]<<")"<<std::endl; 
    std::cout<<"       iso3 = "<<tIso3<<"   ("<<cic4_allcuts_temp_sublead[_tCat-1+2*4]<<")"<<std::endl; 
  }

  if(kin) {
    kin[0] = tIso1;
    kin[1] = tIso2;
    kin[2] = tIso3;
    kin[3] = covIEtaIEta;
    kin[4] = HoE;
    kin[5] = R9;
    kin[6] = dRTrack;
    kin[7] = (float) ph->Pt();
    kin[8] = (float) ph->Eta();
    kin[9] = (float) ph->Phi();

    // iso quantities separate
    kin[10] = ecalIso3;
    kin[11] = ecalIso4;
    kin[12] = hcalIso4;
    kin[13] = trackIso1;
    kin[14] = trackIso2;
    kin[15] = trackIso3;

    kin[16] = (float) ph->Et();
    kin[17] = (float) ph->E();

    kin[18] = (float) ph->SCluster()->Eta();
    kin[19] = (float) ph->SCluster()->Phi();
  }

  float passCuts = 1.;

  if ( ph->Pt()     <= ptmin      ) passCuts = -1.;

  // not needed anymore, do in pre-selection...
  //if (  ph->SCluster()->AbsEta()>=2.5 || (ph->SCluster()->AbsEta()>=1.4442 && ph->SCluster()->AbsEta()<=1.566)) passCuts = -1.;
  
  if(   ! (    tIso1                          < cic4_allcuts_temp_sublead[_tCat-1+0*4]
	       && tIso2                       < cic4_allcuts_temp_sublead[_tCat-1+1*4]
	       && tIso3                       < cic4_allcuts_temp_sublead[_tCat-1+2*4]
	       && covIEtaIEta                 < cic4_allcuts_temp_sublead[_tCat-1+3*4]
	       && HoE                         < cic4_allcuts_temp_sublead[_tCat-1+4*4]
	       && R9                          > cic4_allcuts_temp_sublead[_tCat-1+5*4]
	       && ( dRTrack > cic4_allcuts_temp_sublead[_tCat-1+6*4] || !applyEleVeto ) ) )   passCuts = -1.;
  
  if(print) std::cout<<"   ---> "<<passCuts<<std::endl;

  if(kin) {    
    kin[20] = passCuts;
    kin[21] = (float) _tCat;
  }    

  if(passCuts > 0.) return true;
  return false;
}



bool PhotonTools::PassCiCPFIsoSelection(const Photon* ph, 
                                 const Vertex* vtx, 
                                 const PFCandidateCol*    pfCol,
                                 const VertexCol*   vtxCol,
                                 double rho, double ptmin)
{

  
  // these values are taken from the H2GGlobe code... (actually from Marco/s mail)
  float cic4_allcuts_temp_sublead[] = { 
    6.0,         4.7,         5.6,         3.6,
    10.0,        6.5,         5.6,         4.4,
    3.8,         2.5,         3.1,         2.2,
    0.0108,      0.0102,      0.028,       0.028,
    0.124,       0.092,       0.142,       0.063,
    0.94,        0.28,        0.94,        0.24 };  // the last line is PixelmatchVeto and un-used
  
  // cut on Et instead of Pt???    
  Bool_t isbarrel = ph->SCluster()->AbsEta()<1.5;
    
  
  
  // compute all relevant observables first
  double ecalIso3 = IsolationTools::PFGammaIsolation(ph, 0.3, 0.0, pfCol);
  double ecalIso4 = IsolationTools::PFGammaIsolation(ph, 0.4, 0.0, pfCol);

  unsigned int wVtxInd = 0;

  double trackIsoSel03 = IsolationTools::PFChargedIsolation(ph, vtx, 0.3, 0.0, pfCol);

  // track iso worst vtx
  double trackIsoWorst04 = IsolationTools::PFChargedIsolation(ph, vtx, 0.4, 0.00, pfCol, &wVtxInd, vtxCol);
  
  double combIso1 = ecalIso3+trackIsoSel03 - 0.09*rho;
  double combIso2 = ecalIso4+trackIsoWorst04 - 0.23*rho;
  
  double tIso1 = (combIso1) *50./ph->Et();
  double tIso2 = (combIso2) *50./(ph->MomVtx(vtxCol->At(wVtxInd)->Position()).Pt());
  //double tIso2 = (combIso2) *50./ph->Et();
  double tIso3 = (trackIsoSel03)*50./ph->Et();
  
  double covIEtaIEta  =ph->CoviEtaiEta();
  double HoE = ph->HadOverEm();
  
  double R9 = ph->R9();
  
  // check which category it is ...
  int _tCat = 1;
  if ( !isbarrel ) _tCat = 3;
  if ( R9 < 0.94 ) _tCat++;
  
  float passCuts = 1.;

  if ( ph->Pt()     <= ptmin      ) passCuts = -1.;

  // not needed anymore, do in pre-selection...
  //if (  ph->SCluster()->AbsEta()>=2.5 || (ph->SCluster()->AbsEta()>=1.4442 && ph->SCluster()->AbsEta()<=1.566)) passCuts = -1.;
  
  if(   ! (    tIso1                          < cic4_allcuts_temp_sublead[_tCat-1+0*4]
               && tIso2                       < cic4_allcuts_temp_sublead[_tCat-1+1*4]
               && tIso3                       < cic4_allcuts_temp_sublead[_tCat-1+2*4]
               && covIEtaIEta                 < cic4_allcuts_temp_sublead[_tCat-1+3*4]
               && HoE                         < cic4_allcuts_temp_sublead[_tCat-1+4*4]
               && R9                          > cic4_allcuts_temp_sublead[_tCat-1+5*4] ) )   passCuts = -1.;
  

  if(passCuts > 0.) return true;
  return false;
}





const MCParticle *PhotonTools::MatchMC(const Particle *ph, const MCParticleCol *c, Bool_t matchElectrons) {

//  printf("Start loop\n");
  for (UInt_t i=0; i<c->GetEntries(); ++i) {
    const MCParticle *p = c->At(i);
//     if (p->IsGenerated() && p->AbsPdgId()==11 && (p->DistinctMother()->AbsPdgId()==23|| p->DistinctMother()->AbsPdgId()==24)) {
//       printf("pdgid = %i, status = %i, pt = %5f, mother pdgid = %i\n",p->PdgId(),p->Status(),p->Pt(),p->Mother()->PdgId());
//     }
    if (matchElectrons && p->AbsPdgId()==11 && p->IsGenerated() && p->Status()==3 && MathUtils::DeltaR(*ph,*p) < 0.3 && p->Mother() && (p->Mother()->AbsPdgId()==23 || p->Mother()->AbsPdgId()==24 || p->Mother()->AbsPdgId()==22)) {
      return p;
    }
    if ( p->AbsPdgId()==22 && p->IsGenerated() && MathUtils::DeltaR(*ph,*p) < 0.3 && p->Mother() && (p->Mother()->AbsPdgId()==25 || p->Mother()->AbsPdgId()<=21) ) {
      return p;
    }
  }
  return 0;
}

PhotonTools::DiphotonR9EtaPtCats PhotonTools::DiphotonR9EtaPtCat(const Photon *p1, const Photon *p2) {

  PhotonTools::CiCBaseLineCats cat1 = CiCBaseLineCat(p1);
  PhotonTools::CiCBaseLineCats cat2 = CiCBaseLineCat(p2);
  
  PhotonTools::DiphotonR9EtaPtCats evtcat = PhotonTools::kOctCat0;
  
  bool ph1IsEB = (cat1 ==  PhotonTools::kCiCCat1 || cat1 == PhotonTools::kCiCCat2);
  bool ph2IsEB = (cat2 ==  PhotonTools::kCiCCat1 || cat2 == PhotonTools::kCiCCat2);

  bool ph1IsHR9 = (cat1 ==  PhotonTools::kCiCCat1 || cat1 == PhotonTools::kCiCCat3);
  bool ph2IsHR9 = (cat2 ==  PhotonTools::kCiCCat1 || cat2 == PhotonTools::kCiCCat3);
  
  if( ph1IsEB && ph2IsEB )
    evtcat = ( ph1IsHR9 && ph2IsHR9 ? PhotonTools::kOctCat0 : PhotonTools::kOctCat1);
  else
    evtcat = ( ph1IsHR9 && ph2IsHR9 ? PhotonTools::kOctCat2 : PhotonTools::kOctCat3);
  
  float ptgg = (p1->Mom()+p2->Mom()).Pt();
  if (ptgg<40.0) evtcat = PhotonTools::DiphotonR9EtaPtCats(evtcat + 4);
  
  
  return evtcat;
}

Bool_t  PhotonTools::PassSinglePhotonPresel(const Photon *p,const ElectronCol *els, const DecayParticleCol *conversions, const BaseVertex *bs, const TrackCol* trackCol,const Vertex *vtx, double rho, Bool_t applyElectronVeto, Bool_t invertElectronVeto) {
  float ScEta=p->SCluster()->Eta();
  float Et=p->Et();
  float R9=p->R9();
  float HoE=p->HadOverEm();
  float CovIEtaIEta=p->CoviEtaiEta();
  float EcalIsoDr03=p->EcalRecHitIsoDr03();
  float HcalIsoDr03=p->HcalTowerSumEtDr03();
  float TrkIsoHollowDr03=p->HollowConeTrkIsoDr03();
  float NewEcalIso=EcalIsoDr03-0.012*Et;
  float NewHcalIso=HcalIsoDr03-0.005*Et;
  float NewTrkIsoHollowDr03=TrkIsoHollowDr03-0.002*Et;
  Bool_t IsBarrel=kFALSE;
  Bool_t IsEndcap=kFALSE;
  Bool_t PassEleVetoRaw = PhotonTools::PassElectronVetoConvRecovery(p, els, conversions, bs);  
  Bool_t PassEleVeto = (!applyElectronVeto && !invertElectronVeto) || (applyElectronVeto && !invertElectronVeto && PassEleVetoRaw) || (!applyElectronVeto && invertElectronVeto && !PassEleVetoRaw);
  float AbsTrackIsoCIC=IsolationTools::CiCTrackIsolation(p,vtx, 0.3, 0.02, 0.0, 0.0, 0.1, 1.0,trackCol, NULL, NULL, (!applyElectronVeto ? els : NULL) );
  float HcalEcalPUCorr=EcalIsoDr03+HcalIsoDr03-0.17*rho;

  if(fabs(ScEta)<1.4442){IsBarrel=kTRUE;}
  if(fabs(ScEta)>1.566 && fabs(ScEta)<2.5){IsEndcap=kTRUE;}
  if((!IsBarrel) && (!IsEndcap)){
    return kFALSE;
  }
  if(!PassEleVeto){
    return kFALSE;
  }
  if(R9<=0.9){
    if(HoE<0.075 && ((IsBarrel && CovIEtaIEta<0.014) || (IsEndcap && CovIEtaIEta<0.034)) && NewEcalIso<4 && NewHcalIso<4 && NewTrkIsoHollowDr03<4 && HcalEcalPUCorr<3 && AbsTrackIsoCIC<2.8 && TrkIsoHollowDr03<4.0) {
      return kTRUE;
    }
  }
  if(R9>0.9){
    if(((IsBarrel && HoE<0.082 && CovIEtaIEta<0.014) || (IsEndcap && HoE <0.075 && CovIEtaIEta<0.034)) && NewEcalIso<50 && NewHcalIso<50 && NewTrkIsoHollowDr03<50 && HcalEcalPUCorr<3 && AbsTrackIsoCIC<2.8 && TrkIsoHollowDr03<4.0) {
      return kTRUE;  
    }
  }
  return kFALSE;
}

Bool_t  PhotonTools::PassSinglePhotonPreselPFISO(const Photon *p,const ElectronCol *els, const DecayParticleCol *conversions, const BaseVertex *bs, const TrackCol* trackCol,const Vertex *vtx, double rho, const PFCandidateCol *fPFCands, Bool_t applyElectronVeto, Bool_t invertElectronVeto) {
  float ScEta=p->SCluster()->Eta();
  float Et=p->Et();
  float R9=p->R9();
  float HoE=p->HadOverEm();
  float CovIEtaIEta=p->CoviEtaiEta();
  float EcalIsoDr03=p->EcalRecHitIsoDr03();
  float HcalIsoDr03=p->HcalTowerSumEtDr03();
  float TrkIsoHollowDr03=p->HollowConeTrkIsoDr03();
  float NewEcalIso=EcalIsoDr03-0.012*Et;
  float NewHcalIso=HcalIsoDr03-0.005*Et;
  float NewTrkIsoHollowDr03=TrkIsoHollowDr03-0.002*Et;
  Bool_t IsBarrel=kFALSE;
  Bool_t IsEndcap=kFALSE;
  Bool_t PassEleVetoRaw = PhotonTools::PassElectronVetoConvRecovery(p, els, conversions, bs);  
  Bool_t PassEleVeto = (!applyElectronVeto && !invertElectronVeto) || (applyElectronVeto && !invertElectronVeto && PassEleVetoRaw) || (!applyElectronVeto && invertElectronVeto && !PassEleVetoRaw);
 
  float ChargedIso_selvtx_DR002To0p02=IsolationTools::PFChargedIsolation(p,vtx, 0.2, 0.,fPFCands);

  if(fabs(ScEta)<1.4442){IsBarrel=kTRUE;}
  if(fabs(ScEta)>1.566 && fabs(ScEta)<2.5){IsEndcap=kTRUE;}
  if((!IsBarrel) && (!IsEndcap)){
    return kFALSE;
  }
  if(!PassEleVeto){
    return kFALSE;
  }
  if(R9<=0.9){
    if(HoE<0.075 && ((IsBarrel && CovIEtaIEta<0.014) || (IsEndcap && CovIEtaIEta<0.034)) && NewEcalIso<4 && NewHcalIso<4 && NewTrkIsoHollowDr03<4 && ChargedIso_selvtx_DR002To0p02<4) {
      return kTRUE;
    }
  }
  if(R9>0.9){
    if(((IsBarrel && HoE<0.082 && CovIEtaIEta<0.014) || (IsEndcap && HoE <0.075 && CovIEtaIEta<0.034)) && NewEcalIso<50 && NewHcalIso<50 && NewTrkIsoHollowDr03<50 && ChargedIso_selvtx_DR002To0p02<4) {
      return kTRUE;  
    }
  }
  return kFALSE;
}
