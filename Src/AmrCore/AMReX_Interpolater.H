#ifndef AMREX_INTERPOLATER_H_
#define AMREX_INTERPOLATER_H_
#include <AMReX_Config.H>

#include <AMReX_Extension.H>
#include <AMReX_GpuControl.H>
#include <AMReX_InterpBase.H>

namespace amrex {

class Geometry;
class FArrayBox;
class IArrayBox;

/**
* \brief Virtual base class for interpolaters.
*
* Specifies interpolater interface for coarse-to-fine interpolation in space.
*/
class Interpolater
    : public InterpBase
{
public:

    /**
    * \brief Coarse to fine interpolation in space.
    * This is a pure virtual function and hence MUST
    * be implemented by derived classes.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    virtual void interp (const FArrayBox& crse,
                         int              crse_comp,
                         FArrayBox&       fine,
                         int              fine_comp,
                         int              ncomp,
                         const Box&       fine_region,
                         const IntVect&   ratio,
                         const Geometry&  crse_geom,
                         const Geometry&  fine_geom,
                         Vector<BCRec> const & bcr,
                         int              actual_comp,
                         int              actual_state,
                         RunOn            runon) = 0;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param CrseFab
    * \param CrseFab_G
    * \param FineFab
    * \param FineFab_G
    * \param nComp
    * \param fine_region
    * \param RefRatio
    * \param nDOFX
    * \param CoarseToFineProjectionMatrix
    * \param runon
    */
    virtual void interpConservative
      ( const FArrayBox &  /* CrseFab                      */ ,
        const FArrayBox &  /* CrseFab_G                    */ ,
        FArrayBox       &  /* FineFab                      */ ,
        const FArrayBox &  /* FineFab_G                    */ ,
        int                /* nComp                        */ ,
        const Box       &  /* fine_region                  */ ,
        const IntVect   &  /* RefRatio                     */ ,
        int                /* nDOFX                        */ ,
        Array4<Real const> /* CoarseToFineProjectionMatrix */ ,
        RunOn              /* runon                        */  ){}

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param CrseFab
    * \param CrseFab_G
    * \param FineFab
    * \param FineFab_G
    * \param nComp
    * \param fine_region
    * \param RefRatio
    * \param nDOFX
    * \param CoarseToFineProjectionMatrix
    * \param runon
    */
    virtual void interpPointWise
      ( const FArrayBox &  /* CrseFab                      */ ,
        FArrayBox       &  /* FineFab                      */ ,
        int                /* nComp                        */ ,
        const Box       &  /* fine_region                  */ ,
        const IntVect   &  /* RefRatio                     */ ,
        int                /* nDOFX                        */ ,
        Array4<Real const> /* CoarseToFineProjectionMatrix */ ,
        RunOn              /* runon                        */  ){}

    /**
    * \brief Coarse to fine interpolation in space for face-based data.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param fine_values
    * \param fine_known
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param bccomp
    * \param runon
    */
    virtual void interp_face (const FArrayBox& /*crse*/,
                              const int        /*crse_comp*/,
                              FArrayBox&       /*fine*/,
                              const int        /*fine_comp*/,
                              const int        /*ncomp*/,
                              const Box&       /*fine_region*/,
                              const IntVect&   /*ratio*/,
                              const IArrayBox& /*solve_mask*/,
                              const Geometry&  /*crse_geom*/,
                              const Geometry&  /*fine_geom*/,
                              Vector<BCRec> const & /*bcr*/,
                              const int        /*bccomp*/,
                              RunOn            /*runon*/)
    { amrex::Abort("The version of this Interpolater for face-based data is not implemented or does not apply. Call 'interp' instead."); }

    /**
    * \brief Coarse to fine interpolation in space.
    *        Takes an Array of FArrayBox*s
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param fine_values
    * \param fine_known
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    virtual void interp_arr (Array<FArrayBox*, AMREX_SPACEDIM> const& /*crse*/,
                             const int         /*crse_comp*/,
                             Array<FArrayBox*, AMREX_SPACEDIM> const& /*fine*/,
                             const int         /*fine_comp*/,
                             const int         /*ncomp*/,
                             const Box&        /*fine_region*/,
                             const IntVect&    /*ratio*/,
                             Array<IArrayBox*, AMREX_SPACEDIM> const& /*solve_mask*/,
                             const Geometry&   /*crse_geom*/,
                             const Geometry&   /*fine_geom*/,
                             Vector<Array<BCRec, AMREX_SPACEDIM> > const& /*bcr*/,
                             const int         /*actual_comp*/,
                             const int         /*actual_state*/,
                             const RunOn       /*runon*/)

    { amrex::Abort("The Array<FArrayBox*, AMREX_SPACEDIM> version of this Interpolater is not implemented or does not apply. Call 'interp' instead."); }

    /**
    * \brief Re-visit the interpolation to protect against under- or overshoots.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param fine_state
    * \param state_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    */
    virtual void protect (const FArrayBox& /*crse*/,
                          int              /*crse_comp*/,
                          FArrayBox&       /*fine*/,
                          int              /*fine_comp*/,
                          FArrayBox&       /*fine_state*/,
                          int              /*state_comp*/,
                          int              /*ncomp*/,
                          const Box&       /*fine_region*/,
                          const IntVect&   /*ratio*/,
                          const Geometry&  /*crse_geom*/,
                          const Geometry&  /*fine_geom*/,
                          Vector<BCRec>&   /*bcr*/,
                          RunOn            /*runon*/) {}
};


/**
* \brief Bilinear interpolation on node centered data.
*
* Bilinear interpolation on node centered data.
*/
class NodeBilinear
    :
    public Interpolater
{
public:
    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const& bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;
};


/**
* \brief Bilinear interpolation on cell centered data.
*
* Bilinear interpolation on cell centered data.
*/
class CellBilinear
    :
    public Interpolater
{
public:
    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const& bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;
};


/**
* \brief Linear conservative interpolation on cell centered data
*
*
* Linear conservative interpolation on cell centered data
* I.e, conservative interpolation with a limiting scheme that
* preserves the value of any linear combination  of the
* fab components; //!< e.g.,
*
* if sum_ivar a(ic,jc,ivar)*fab(ic,jc,ivar) = 0, then
* sum_ivar a(ic,jc,ivar)*fab(if,jf,ivar) = 0 is satisfied
* in all fine cells if,jf covering coarse cell ic,jc.
*/
class CellConservativeLinear
    :
    public Interpolater
{
public:

    /**
    * \brief The constructor.
    *
    * \param do_linear_limiting_
    */
    explicit CellConservativeLinear (bool do_linear_limiting_= true);

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;
    /**
    * \brief Coarse to fine interpolation in space.
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const& bcr,
                 int              /*actual_comp*/,
                 int              /*actual_state*/,
                 RunOn            runon) override;

protected:

    bool do_linear_limiting;
};


/**
* \brief Lin. cons. interp. on cc data with protection against under/over-shoots.
*
* Linear conservative interpolation on cell centered data
* but with protection against undershoots or overshoots.
*/
class CellConservativeProtected
    :
    public CellConservativeLinear
{
public:

    /**
    * \brief The constructor.
    */
    CellConservativeProtected ();

    /**
    * \brief Re-visit the interpolation to protect against under- or overshoots.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param fine_state
    * \param state_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    */
    void protect (const FArrayBox& crse,
                  int              crse_comp,
                  FArrayBox&       fine,
                  int              fine_comp,
                  FArrayBox&       fine_state,
                  int              state_comp,
                  int              ncomp,
                  const Box&       fine_region,
                  const IntVect&   ratio,
                  const Geometry&  crse_geom,
                  const Geometry&  fine_geom,
                  Vector<BCRec>&   bcr,
                  RunOn            runon) override;
};


/**
* \brief Quadratic interpolation on cell centered data.
*
* Quadratic interpolation on cell centered data.
*/
class CellQuadratic
    :
    public Interpolater
{
public:

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const& bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;
};


/**
* \brief Piecewise Constant interpolation on cell centered data.
*/
class PCInterp
    :
    public Interpolater
{
public:
    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const&  bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;
};


/**
* \brief Conservative quartic interpolation on cell averaged data.
*
* An order 4 polynomial is used to fit the data.  For each cell involved
* in constructing the polynomial, the average of the polynomial inside that
* cell is equal to the cell averaged value of the original data.
*/
class CellConservativeQuartic
    :
    public Interpolater
{
public:
    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const&  bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;
};

/**
* \brief Divergence-preserving interpolation on face centered data.
*
* Divergence-preserving interpolation on face centered data,
* i.e., it ensures the divergence of the fine ghost cells match the value
* of the divergence of the underlying crse cell. All fine cells overlying
* a given coarse cell will have the same divergence, even when the coarse
* grid divergence is spatially varying.
* Based on Vanella et. al. (doi:10.1016/j.jcp.2010.05.003, section 3.2),
* but solves the interior closure problem using least squares with an
* initial guess equal to the average of fine face values across the cell.
*/
class FaceDivFree
    :
    public Interpolater
{
public:
    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    * \param runon
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const& bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param solve_mask
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    * \param runon
    */
    void interp_arr (Array<FArrayBox*, AMREX_SPACEDIM> const& crse,
                     int         crse_comp,
                     Array<FArrayBox*, AMREX_SPACEDIM> const& fine,
                     int         fine_comp,
                     int         ncomp,
                     const Box&        fine_region,
                     const IntVect&    ratio,
                     Array<IArrayBox*, AMREX_SPACEDIM> const& solve_mask,
                     const Geometry&   crse_geom,
                     const Geometry&   fine_geom,
                     Vector<Array<BCRec, AMREX_SPACEDIM> > const& bcr,
                     int         actual_comp,
                     int         actual_state,
                     RunOn       runon) override;
};


/**
* \brief Piecewise constant tangential interpolation / linear normal interpolation of face data.
*
* Piecewise constant tangential interpolation / linear normal interpolation of face data.
*/
class FaceLinear
    :
    public Interpolater
{
public:
    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const& bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;

    /**
    * \brief Coarse to fine interpolation in space for face-based data.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param solve_mask
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param bccomp
    * \param runon
    */
    void interp_face (const FArrayBox& crse,
                      int        crse_comp,
                      FArrayBox&       fine,
                      int        fine_comp,
                      int        ncomp,
                      const Box&       fine_region,
                      const IntVect&   ratio,
                      const IArrayBox& solve_mask,
                      const Geometry&  crse_geom,
                      const Geometry&  fine_geom,
                      Vector<BCRec> const & bcr,
                      int        bccomp,
                      RunOn            runon) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp_arr (Array<FArrayBox*, AMREX_SPACEDIM> const& crse,
                     int         crse_comp,
                     Array<FArrayBox*, AMREX_SPACEDIM> const& fine,
                     int         fine_comp,
                     int         ncomp,
                     const Box&        fine_region,
                     const IntVect&    ratio,
                     Array<IArrayBox*, AMREX_SPACEDIM> const& solve_mask,
                     const Geometry&   /*crse_geom*/,
                     const Geometry&   /*fine_geom*/,
                     Vector<Array<BCRec, AMREX_SPACEDIM> > const& /*bcr*/,
                     int         /*actual_comp*/,
                     int         /*actual_state*/,
                     RunOn       runon) override;


};

/**
* \brief Bilinear tangential interpolation / linear normal interpolation of face data.
*
* Bilinear tangential interpolation / linear normal interpolation of face data.
*/
class FaceConservativeLinear
    :
    public Interpolater
{
public:
    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const& bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;

    /**
    * \brief Coarse to fine interpolation in space for face-based data.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param solve_mask
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param bccomp
    * \param runon
    */
    void interp_face (const FArrayBox& crse,
                      int        crse_comp,
                      FArrayBox&       fine,
                      int        fine_comp,
                      int        ncomp,
                      const Box&       fine_region,
                      const IntVect&   ratio,
                      const IArrayBox& solve_mask,
                      const Geometry&  crse_geom,
                      const Geometry&  fine_geom,
                      Vector<BCRec> const & bcr,
                      int        bccomp,
                      RunOn            runon) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp_arr (Array<FArrayBox*, AMREX_SPACEDIM> const& crse,
                     int         crse_comp,
                     Array<FArrayBox*, AMREX_SPACEDIM> const& fine,
                     int         fine_comp,
                     int         ncomp,
                     const Box&        fine_region,
                     const IntVect&    ratio,
                     Array<IArrayBox*, AMREX_SPACEDIM> const& solve_mask,
                     const Geometry&   /*crse_geom*/,
                     const Geometry&   /*fine_geom*/,
                     Vector<Array<BCRec, AMREX_SPACEDIM> > const& /*bcr*/,
                     int         /*actual_comp*/,
                     int         /*actual_state*/,
                     RunOn       runon) override;


};

/**
* \brief Quartic interpolation on cell centered data.
*
* Quartic interpolation on cell centered data.
*/
class CellQuartic
    :
    public Interpolater
{
public:
    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, int ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine, const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp (const FArrayBox& crse,
                 int              crse_comp,
                 FArrayBox&       fine,
                 int              fine_comp,
                 int              ncomp,
                 const Box&       fine_region,
                 const IntVect&   ratio,
                 const Geometry&  crse_geom,
                 const Geometry&  fine_geom,
                 Vector<BCRec> const& bcr,
                 int              actual_comp,
                 int              actual_state,
                 RunOn            runon) override;
};

/**
* \brief DG interpolation on nodal data with cell.
*/

class DGInterp
    :
    public Interpolater
{
public:

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine,
                   int        ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box&     fine,
                   const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp
           ( const FArrayBox     & crse        ,
             int                   crse_comp   ,
             FArrayBox           & fine        ,
             int                   fine_comp   ,
             int                   ncomp       ,
             const Box           & fine_region ,
             const IntVect       & ratio       ,
             const Geometry      & crse_geom   ,
             const Geometry      & fine_geom   ,
             Vector<BCRec> const & bcr         ,
             int                   actual_comp ,
             int                   actual_state,
             RunOn                 runon        ) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param CrseFab
    * \param CrseFab_G
    * \param FineFab
    * \param FineFab_G
    * \param nComp
    * \param fine_region
    * \param RefRatio
    * \param nDOFX
    * \param CoarseToFineProjectionMatrix
    * \param runon
    */
    void interpConservative
           ( const FArrayBox &  CrseFab                     ,
             const FArrayBox &  CrseFab_G                   ,
             FArrayBox       &  FineFab                     ,
             const FArrayBox &  FineFab_G                   ,
             int                nComp                       ,
             const Box       &  fine_region                 ,
             const IntVect   &  RefRatio                    ,
             int                nDOFX                       ,
             Array4<Real const> CoarseToFineProjectionMatrix,
             RunOn              runon                        ) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param CrseFab
    * \param FineFab
    * \param nComp
    * \param fine_region
    * \param RefRatio
    * \param nDOFX
    * \param CoarseToFineProjectionMatrix
    * \param runon
    */
    void interpPointWise
           ( const FArrayBox &  CrseFab                     ,
             FArrayBox       &  FineFab                     ,
             int                nComp                       ,
             const Box       &  fine_region                 ,
             const IntVect   &  RefRatio                    ,
             int                nDOFX                       ,
             Array4<Real const> CoarseToFineProjectionMatrix,
             RunOn              runon                        ) override;
};
/**
* \brief CG interpolation on nodal data with cell.
*/

class CGInterp
    :
    public Interpolater
{
public:

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box& fine,
                   int        ratio) override;

    /**
    * \brief Returns coarsened box given fine box and refinement ratio.
    *
    * \param fine
    * \param ratio
    */
    Box CoarseBox (const Box&     fine,
                   const IntVect& ratio) override;

    /**
    * \brief Coarse to fine interpolation in space.
    *
    * \param crse
    * \param crse_comp
    * \param fine
    * \param fine_comp
    * \param ncomp
    * \param fine_region
    * \param ratio
    * \param crse_geom
    * \param fine_geom
    * \param bcr
    * \param actual_comp
    * \param actual_state
    */
    void interp
           ( const FArrayBox     & crse        ,
             int                   crse_comp   ,
             FArrayBox           & fine        ,
             int                   fine_comp   ,
             int                   ncomp       ,
             const Box           & fine_region ,
             const IntVect       & ratio       ,
             const Geometry      & crse_geom   ,
             const Geometry      & fine_geom   ,
             Vector<BCRec> const & bcr         ,
             int                   actual_comp ,
             int                   actual_state,
             RunOn                 runon        ) override;
};

//! CONSTRUCT A GLOBAL OBJECT OF EACH VERSION.
extern AMREX_EXPORT PCInterp                  pc_interp;
extern AMREX_EXPORT NodeBilinear              node_bilinear_interp;
extern AMREX_EXPORT FaceDivFree               face_divfree_interp;
extern AMREX_EXPORT FaceLinear                face_linear_interp;
extern AMREX_EXPORT FaceConservativeLinear    face_cons_linear_interp;
extern AMREX_EXPORT CellConservativeLinear    lincc_interp;
extern AMREX_EXPORT CellConservativeLinear    cell_cons_interp;
extern AMREX_EXPORT CellBilinear              cell_bilinear_interp;
extern AMREX_EXPORT CellConservativeProtected protected_interp;
extern AMREX_EXPORT CellConservativeQuartic   quartic_interp;
extern AMREX_EXPORT CellQuadratic             quadratic_interp;
extern AMREX_EXPORT DGInterp                  dg_interp;
extern AMREX_EXPORT CGInterp                  cg_interp;
extern AMREX_EXPORT CellQuartic               cell_quartic_interp;

/**
* \brief Base class for DG interpolaters.
*
* Specifies interpolater interface for coarse-to-fine interpolation in space
* with nodal DG-based elements.
*
* @param nNodes1D The number of nodes per element in each active dimension.
* @param nDOF The total number of nodes per element.
* @param nFine The total number of fine elements covering a coarse element.
* @param Basis The polynomial basis on which the DG solution is based.
*/
enum struct DGBasis { Lagrange };
template <int nNodes1D, int nDOF, int nFine, DGBasis Basis>
class DGInterpolater
    : public InterpBase
{
    public:
        DGInterpolater ()
        {
            int nNodes[3];
            int nFineV[3];

            nNodes[0] = nNodes1D;
            nNodes[1] = 1;
            nNodes[2] = 1;

            nFineV[0] = 2;
            nFineV[1] = 1;
            nFineV[2] = 1;

#if AMREX_SPACEDIM > 1
            nNodes[1] = nNodes1D;
            nFineV[1] = 2;
#endif
#if AMREX_SPACEDIM > 2
            nNodes[2] = nNodes1D;
            nFineV[2] = 2;
#endif

            Real * x1q = new Real[nNodes[0]];
            Real * w1q = new Real[nNodes[0]];
            Real * x2q = new Real[nNodes[1]];
            Real * w2q = new Real[nNodes[1]];
            Real * x3q = new Real[nNodes[2]];
            Real * w3q = new Real[nNodes[2]];

            Real * xiX1 = new Real[nNodes[0]];
            Real * xiX2 = new Real[nNodes[1]];
            Real * xiX3 = new Real[nNodes[2]];

            int iFine, ii, jj;

            switch(Basis)
            {
                case amrex::DGBasis::Lagrange:
                    compute_quad_weights_and_points( nNodes[0], x1q, w1q );
                    compute_quad_weights_and_points( nNodes[1], x2q, w2q );
                    compute_quad_weights_and_points( nNodes[2], x3q, w3q );

                    iFine = 0;
                    for (int iFineX3 = 0; iFineX3 < nFineV[2]; iFineX3++) {
                    for (int iFineX2 = 0; iFineX2 < nFineV[1]; iFineX2++) {
                    for (int iFineX1 = 0; iFineX1 < nFineV[0]; iFineX1++) {

                        if (nFineV[0] > 1) {
                            for (int k = 0; k < nNodes[0]; k++) {
                                xiX1[k]
                                  = Half * ( x1q[k] + std::pow(-1,iFineX1+1)
                                                       * Half );
                            }
                        } else {
                            for (int k = 0; k < nNodes[0]; k++) {
                                xiX1[k] = Zero;
                            }
                        }

                        if (nFineV[1] > 1) {
                            for (int k = 0; k < nNodes[1]; k++) {
                                xiX2[k]
                                  = Half * ( x2q[k] + std::pow(-1,iFineX2+1)
                                                       * Half );
                            }
                        } else {
                            for (int k = 0; k < nNodes[1]; k++) {
                                xiX2[k] = Zero;
                            }
                        }

                        if (nFineV[2] > 1) {
                            for (int k = 0; k < nNodes[2]; k++) {
                                xiX3[k]
                                  = Half * ( x3q[k] + std::pow(-1,iFineX3+1)
                                                       * Half );
                            }
                        } else {
                            for (int k = 0; k < nNodes[2]; k++) {
                                xiX3[k] = Zero;
                            }
                        }

                        ii = 0;
                        for (int i3 = 0; i3 < nNodes[2]; i3++) {
                        for (int i2 = 0; i2 < nNodes[1]; i2++) {
                        for (int i1 = 0; i1 < nNodes[0]; i1++) {
                            jj = 0;
                            for (int j3 = 0; j3 < nNodes[2]; j3++) {
                            for (int j2 = 0; j2 < nNodes[1]; j2++) {
                            for (int j1 = 0; j1 < nNodes[0]; j1++) {
                                m_c2fmatrix(iFine,ii,jj)
                                  = Lag( Half * ( x1q[i1]
                                                   + std::pow(-1, iFineX1+1)
                                                       * Half ),
                                         j1, x1q, nNodes[0] )
                                  * Lag( Half * ( x1q[i2]
                                                   + std::pow(-1, iFineX2+1)
                                                       * Half ),
                                         j2, x2q, nNodes[1] )
                                  * Lag( Half * ( x1q[i3]
                                                   + std::pow(-1, iFineX3+1)
                                                       * Half ),
                                         j3, x3q, nNodes[2] );
                                jj++;
                            }}}
                            ii++;
                        }}}
                        iFine++;
                    }}}
                    break;
                default:
                    amrex::Abort("Unknown Basis");
            }

            delete[] xiX3;
            delete[] xiX2;
            delete[] xiX1;

            delete[] w3q;
            delete[] x3q;
            delete[] w2q;
            delete[] x2q;
            delete[] w1q;
            delete[] x1q;

        }

        /**
        * \brief Returns coarsened box given fine box and refinement ratio.
        *
        * \param fine
        * \param ratio
        */
        Box CoarseBox (const Box& fine, int ratio) override
            { return amrex::coarsen(fine,ratio); }

        /**
        * \brief Returns coarsened box given fine box and refinement ratio.
        *
        * \param fine
        * \param ratio
        */
        Box CoarseBox (const Box& fine, const IntVect& ratio) override
            { return amrex::coarsen(fine,ratio); }

    private:
        Real const Zero = 0.0;
        Real const Half = 0.5;
        Array3D<Real,0,nFine-1,0,nDOF-1,0,nDOF-1> m_c2fmatrix;
        void compute_quad_weights_and_points
          ( int N, Real *& xq, Real *& wq )
        {
            /* Computes the Gauss--Legendre quadrature points
             *
             * Adapted from Mark Newman's Python version:
             * https://public.websites.umich.edu/~mejn/cp/programs/gaussxw.py
            */

            Real const One   = 1.0;
            Real const Two   = 2.0;
            Real const Three = 3.0;
            Real const Four  = 4.0;
            Real const Eight = 8.0;

            if (N == 1){
                xq[0] = Zero;
                wq[0] = One;
                return;
            }

            // Initial approximation to roots of the Legendre polynomial
            Real * a = new Real[N];
            Real da
              = ( Four * (Real)N - One - Three ) / ( (Real)N - One );
            for (int i = 0; i < N; i++) {
                a[i] = ( Three + (Real)i * da ) / ( Four * (Real)N + Two );
            }

            Real * x = new Real[N];
            for (int i = 0; i < N; i++ ) {
                x[i] = cos( M_PI * a[i]
                         + One / ( Eight * (Real)N * (Real)N * tan( a[i] ) ) );
            }

            // Find roots using Newton's method
            Real const epsilon = 1.0e-15;
            Real delta = One;
            Real * p0 = new Real[N];
            Real * p1 = new Real[N];
            Real * dx = new Real[N];
            Real * dp = new Real[N];
            Real p00;
            while ( delta > epsilon ) {
                for (int i = 0; i < N; i++) {
                    p0[i] = One;
                    p1[i] = x[i];
                }
                for (int k = 1;  k < N; k++) {
                    for (int i = 0; i < N; i++) {
                        p00 = p1[i];
                        p1[i] = ( ( Two * (Real)k + One ) * x[i] * p1[i]
                                  - (Real)k * p0[i] ) / ( (Real)k + One );
                        p0[i] = p00;
                    }
                }
                delta = Zero;
                for (int i = 0; i < N; i++) {
                    dp[i] = ( (Real)N + One )
                               * ( p0[i] - x[i] * p1[i] )
                               / ( One - x[i] * x[i] );
                    dx[i] = p1[i] / dp[i];
                    x[i] -= dx[i];
                    delta = max(delta, std::abs(dx[i]) );
                }
            }

            // Calculate the weights

            Real * w = new Real[N];
            for (int i = 0; i < N; i++) {
                w[i]
                  = Two * ( (Real)N + One )
                      * ( (Real)N + One )
                      / ( (Real)N * (Real)N * ( One - x[i] * x[i] )
                          * dp[i] * dp[i] );
                w[i] /= Two;
                x[i] /= Two;
            }

            // Sort arrays in ascending order by quadrature point values
            //

            for (int i = 0; i < N; i++) {
                xq[i] = x[N-1-i];
                wq[i] = w[N-1-i];
            }

            delete[] w;
            delete[] dp;
            delete[] dx;
            delete[] p1;
            delete[] p0;
            delete[] x;
            delete[] a;

            // Force central quadrature point to be zero

            if (N % 2 == 1) {
                xq[N/2] = Zero;
            }
        } // end compute_quad_weights_and_points
        Real Lag( Real x, int i, Real const xq[], int N ) {
            Real L = 1.0;
            if (N == 1) {
                return L;
            }
            for (int j = 0; j < nNodes1D; j++) {
                if (i != j) {
                    L *= ( x - xq[j] ) / ( xq[i] - xq[j] );
                }
            }
            return L;
        }

};

}

#endif /*_INTERPOLATER_H_*/
