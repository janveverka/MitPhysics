// $Id: MitPhysicsModsLinkDef.h,v 1.38 2012/11/14 20:05:37 sixie Exp $

#ifndef MITPHYSICS_MODS_LINKDEF_H
#define MITPHYSICS_MODS_LINKDEF_H
#include "MitPhysics/Mods/interface/CaloMetCorrectionMod.h"
#include "MitPhysics/Mods/interface/EffMod.h"
#include "MitPhysics/Mods/interface/ElectronCleaningMod.h"
#include "MitPhysics/Mods/interface/ElectronIDMod.h"
#include "MitPhysics/Mods/interface/ElectronIDMod_debug.h"
#include "MitPhysics/Mods/interface/ElectronCorrectionMod.h"
#include "MitPhysics/Mods/interface/GeneratorMod.h"
#include "MitPhysics/Mods/interface/GoodPVFilterMod.h"
#include "MitPhysics/Mods/interface/HFCoincidenceFilterMod.h"
#include "MitPhysics/Mods/interface/HKFactorProducer.h"
#include "MitPhysics/Mods/interface/JetCleaningMod.h"
#include "MitPhysics/Mods/interface/JetIDMod.h"
#include "MitPhysics/Mods/interface/MVAMetMod.h"
#include "MitPhysics/Mods/interface/JetCorrectionMod.h"
#include "MitPhysics/Mods/interface/MergeLeptonsMod.h"
#include "MitPhysics/Mods/interface/MuonIDMod.h"
#include "MitPhysics/Mods/interface/MuonIDMod_debug.h"
#include "MitPhysics/Mods/interface/PDFProducerMod.h"
#include "MitPhysics/Mods/interface/PartonFlavorHistoryMod.h"
#include "MitPhysics/Mods/interface/PhotonCleaningMod.h"
#include "MitPhysics/Mods/interface/PhotonIDMod.h"
#include "MitPhysics/Mods/interface/PhotonCiCMod.h"
#include "MitPhysics/Mods/interface/PhotonMvaMod.h"
#include "MitPhysics/Mods/interface/PhotonPairSelector.h"
#include "MitPhysics/Mods/interface/PhotonTreeWriter.h"
#include "MitPhysics/Mods/interface/PhotonPairSelector_debug.h"
#include "MitPhysics/Mods/interface/PhotonTreeWriter_debug.h"
#include "MitPhysics/Mods/interface/TauCleaningMod.h"
#include "MitPhysics/Mods/interface/PFTauCleaningMod.h"
#include "MitPhysics/Mods/interface/TauIDMod.h"
#include "MitPhysics/Mods/interface/PFTauIDMod.h"
#include "MitPhysics/Mods/interface/TrackingPurityFilterMod.h"
#include "MitPhysics/Mods/interface/MVASystematicsMod.h"
#include "MitPhysics/Mods/interface/SeparatePileUpMod.h"
#endif

#ifdef __CINT__
#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;
#pragma link C++ namespace mithep;

#pragma link C++ class mithep::CaloMetCorrectionMod+;
#pragma link C++ class mithep::EffMod+;
#pragma link C++ class mithep::ElectronCleaningMod+;
#pragma link C++ class mithep::ElectronIDMod+;
#pragma link C++ class mithep::ElectronIDMod_debug+;
#pragma link C++ class mithep::ElectronCorrectionMod+;
#pragma link C++ class mithep::GeneratorMod+;
#pragma link C++ class mithep::GoodPVFilterMod+;
#pragma link C++ class mithep::HFCoincidenceFilterMod+;
#pragma link C++ class mithep::HKFactorProducer+;
#pragma link C++ class mithep::JetCleaningMod+;
#pragma link C++ class mithep::JetIDMod+;
#pragma link C++ class mithep::MVAMetMod+;
#pragma link C++ class mithep::JetCorrectionMod+;
#pragma link C++ class mithep::MergeLeptonsMod+;
#pragma link C++ class mithep::MuonIDMod+;
#pragma link C++ enum mithep::MuonIDMod::EMuClassType;
#pragma link C++ enum mithep::MuonIDMod::EMuIdType;
#pragma link C++ enum mithep::MuonIDMod::EMuIsoType;
#pragma link C++ class mithep::MuonIDMod_debug+;
#pragma link C++ enum mithep::MuonIDMod_debug::EMuClassType;
#pragma link C++ enum mithep::MuonIDMod_debug::EMuIdType;
#pragma link C++ enum mithep::MuonIDMod_debug::EMuIsoType;
#pragma link C++ class mithep::PartonFlavorHistoryMod+;
#pragma link C++ class mithep::PDFProducerMod+;
#pragma link C++ class mithep::PhotonCleaningMod+;
#pragma link C++ class mithep::PhotonIDMod+;
#pragma link C++ class mithep::PhotonCiCMod+;
#pragma link C++ class mithep::PhotonPairSelector+;
#pragma link C++ class mithep::PhotonPairSelector_debug+;
#pragma link C++ class mithep::PhotonMvaMod+;
#pragma link C++ class mithep::PhotonTreeWriter+;
#pragma link C++ class mithep::PhotonTreeWriterPhoton<16>+;
#pragma link C++ class mithep::PhotonTreeWriterDiphotonEvent+;
#pragma link C++ class mithep::PhotonTreeWriterVtx+;
#pragma link C++ class mithep::PhotonTreeWriter_debug+;
#pragma link C++ class mithep::PhotonTreeWriter_debugPhoton<16>+;
#pragma link C++ class mithep::PhotonTreeWriter_debugDiphotonEvent+;
#pragma link C++ class mithep::PhotonTreeWriter_debugVtx+;
#pragma link C++ class mithep::TauCleaningMod+;
#pragma link C++ class mithep::PFTauCleaningMod+;
#pragma link C++ class mithep::TauIDMod+;
#pragma link C++ class mithep::PFTauIDMod+;
#pragma link C++ class mithep::TrackingPurityFilterMod+;
#pragma link C++ class mithep::MVASystematicsMod+;
#pragma link C++ class mithep::SeparatePileUpMod+;
#endif
