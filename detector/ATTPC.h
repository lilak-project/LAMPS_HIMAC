#ifndef ATTPC_HH
#define ATTPC_HH

#include "LKDetector.h"
#include "LKPadPlane.h"

class ATTPC : public LKDetector
{
  public:
    ATTPC();
    virtual ~ATTPC() {};

    virtual bool Init();

    LKVector3::Axis GetEFieldAxis();

    virtual TVector3 GetEField(TVector3 pos);
    virtual LKPadPlane *GetDriftPlane(TVector3 pos);

  protected:
    virtual bool BuildGeometry();
    virtual bool BuildDetectorPlane();

    LKVector3::Axis fEFieldAxis = LKVector3::kNon;

  ClassDef(ATTPC, 1)
};

#endif
