//--------------------------------------------------------------------------------------------------
// CosmicCleaningMod
//
// This Module performs cleaning of cosmics, ie. it removes duplicate objects and good muons 
// from the cosmic muons.
//
// Authors: C. Ferko
//--------------------------------------------------------------------------------------------------

#ifndef MITPHYSICS_MODS_COSMICCLEANINGMOD_H
#define MITPHYSICS_MODS_COSMICCLEANINGMOD_H

#include "MitAna/TreeMod/interface/BaseMod.h" 
#include "MitAna/DataTree/interface/MuonCol.h"

namespace mithep 
{
  class CosmicCleaningMod : public BaseMod
  {
    public:
      CosmicCleaningMod(const char *name="CosmicCleaningMod", 
                          const char *title="Cosmic cleaning module");

      const char        *GetCleanCosmicsName() const { return fCleanCosmicsName;       }
      const char        *GetCleanName()          const { return GetCleanCosmicsName(); }
      const char        *GetCleanMuonsName()     const { return fCleanMuonsName;       }
      const char        *GetCosmicsName()  const { return fCosmicsName;                }
      const char        *GetOutputName()         const { return GetCleanCosmicsName(); }
      void               SetCleanCosmicsName(const char *name)   { fCleanCosmicsName = name;  }
      void               SetCleanName(const char *name)          { SetCleanCosmicsName(name); }
      void               SetCleanMuonsName(const char *name)     { fCleanMuonsName     = name;}
      void               SetCosmicsName(const char *name)        { fCosmicsName  = name;      }
      void               SetOutputName(const char *name)         { SetCleanCosmicsName(name); }

    protected:
      void               Process();
      void               SlaveBegin();

      TString            fCosmicsName;  //name of cosmics (input)
      TString            fCleanMuonsName;     //name of clean muons (input)
      TString            fCleanCosmicsName; //name of clean cosmics (output)
      const MuonCol     *fCosmics;            //muon cosmics collection (input)
    
      ClassDef(CosmicCleaningMod, 1) // Cosmic cleaning module
  };
}
#endif
