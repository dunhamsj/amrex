#include <AMReX_FPhysBC.H>
#include <AMReX_FillPatchUtil.H>

using namespace amrex;

namespace
{
    // THIS MUST BE CONSISTENT WITH amrex_interpolater_module in AMReX_interpolater_mod.F90!!!
    Vector<Interpolater*> interp = {
        &amrex::pc_interp,               // 0
        &amrex::node_bilinear_interp,    // 1
        &amrex::cell_bilinear_interp,    // 2
        &amrex::quadratic_interp,        // 3
        &amrex::lincc_interp,            // 4
        &amrex::cell_cons_interp,        // 5
        &amrex::protected_interp,        // 6
        &amrex::quartic_interp,          // 7
        &amrex::face_divfree_interp,     // 8
        &amrex::face_linear_interp,      // 9
        &amrex::dg_interp,               // 10
        &amrex::cg_interp                // 11
    };
}

namespace {

    using INTERP_HOOK = void (*)(const int* lo, const int*hi,
                                 Real* d, const int* dlo, const int* dhi, int nd,
                                 int icomp, int ncomp);

    using INTERP_HOOK_ARR = void (*)(const int* lo, const int*hi,
                                     Real* dx, const int* dxlo, const int* dxhi,
#if AMREX_SPACEDIM>=2
                                     Real* dy, const int* dylo, const int* dyhi,
#if AMREX_SPACEDIM==3
                                     Real* dz, const int* dzlo, const int* dzhi,
#endif
#endif
                                     int nd, int icomp, int ncomp);

    class FIInterpHook
    {
    public:
        explicit FIInterpHook (INTERP_HOOK a_f) : m_f(a_f) {}
        void operator() (FArrayBox& fab, const Box& bx, int icomp, int ncomp) const
        {
            if (m_f) {
                m_f(BL_TO_FORTRAN_BOX(bx),
                    BL_TO_FORTRAN_ANYD(fab), fab.nComp(),
                    icomp+1, ncomp);
                // m_f is a fortran function expecting 1-based index
            }
        }

    private:
        INTERP_HOOK m_f;
    };

    class FIArrInterpHook
    {
    public:
        explicit FIArrInterpHook (INTERP_HOOK_ARR a_f) : m_f(a_f) {}
        void operator() (Array<FArrayBox*, AMREX_SPACEDIM> fab, const Box& bx, int icomp, int ncomp) const
        {
            if (m_f) {
                m_f(BL_TO_FORTRAN_BOX(bx),
                    BL_TO_FORTRAN_ANYD(*fab[0]),
#if AMREX_SPACEDIM>=2
                    BL_TO_FORTRAN_ANYD(*fab[1]),
#if AMREX_SPACEDIM>=3
                    BL_TO_FORTRAN_ANYD(*fab[2]),
#endif
#endif
                    fab[0]->nComp(),
                    icomp+1, ncomp);
                // m_f is a fortran function expecting 1-based index
            }
        }

    private:
        INTERP_HOOK_ARR m_f;
    };

}

extern "C"
{
    void amrex_fi_fillpatch_single (MultiFab* mf, Real time, MultiFab* smf[], Real stime[], int ns,
                                    int scomp, int dcomp, int ncomp, const Geometry* geom,
                                    FPhysBC::fill_physbc_funptr_t fill)
    {
        FPhysBC pbc(fill, geom);
        amrex::FillPatchSingleLevel(*mf, time, Vector<MultiFab*>{smf, smf+ns},
                                    Vector<Real>{stime, stime+ns},
                                    scomp, dcomp, ncomp, *geom, pbc, 0);
    }

    void amrex_fi_fillpatch_two
           ( MultiFab * MF, Real Time,
             MultiFab * pCrseMF[], Real CrseTime[], int nCrse,
             MultiFab * pFineMF[], Real FineTime[], int nFine,
             int sComp, int dComp, int nComp,
             const Geometry * pCrseGeom, const Geometry * pFineGeom,
             FPhysBC::fill_physbc_funptr_t fpCrseFillPhysBC,
             FPhysBC::fill_physbc_funptr_t fpFineFillPhysBC,
             int RefRatio, int interp_id,
             int * pLoBC[], int * pHiBC[],
             INTERP_HOOK pre_interp, INTERP_HOOK post_interp )
    {
        Vector<BCRec> bcs;
        for ( int iComp = 0; iComp < nComp; ++iComp ) {
            bcs.emplace_back( pLoBC[iComp+sComp], pHiBC[iComp+sComp] );
        }

        FPhysBC CrseBC( fpCrseFillPhysBC, pCrseGeom );
        FPhysBC FineBC( fpFineFillPhysBC, pFineGeom );

        amrex::FillPatchTwoLevels
                 ( *MF, Time,
                   Vector<MultiFab*>{pCrseMF , pCrseMF+nCrse},
                   Vector<Real>     {CrseTime, CrseTime+nCrse},
                   Vector<MultiFab*>{pFineMF , pFineMF+nFine},
                   Vector<Real>     {FineTime, FineTime+nFine},
                   sComp, dComp, nComp,
                   *pCrseGeom, *pFineGeom,
                   CrseBC, 0, FineBC, 0,
                   IntVect{AMREX_D_DECL(RefRatio,RefRatio,RefRatio)},
                   interp[interp_id], bcs, 0,
                   FIInterpHook( pre_interp  ),
                   FIInterpHook( post_interp ) );
    }

    void amrex_fi_fillpatch_dgconservative_two
           ( MultiFab * MF, MultiFab * MF_G, Real Time,
             MultiFab * pCrseMF[], MultiFab * pCrseMF_G[],
             Real CrseTime[], int nCrse,
             MultiFab * pFineMF[], MultiFab * pFineMF_G[],
             Real FineTime[], int nFine,
             int sComp, int dComp, int nComp,
             const Geometry* pCrseGeom, const Geometry * pFineGeom,
             FPhysBC::fill_physbc_funptr_t fpCrseFillPhysBC,
             FPhysBC::fill_physbc_funptr_t fpFineFillPhysBC,
             int RefRatio, int interp_id,
             int * pLoBC[], int * pHiBC[],
             int nFineV, int nDOFX, void * vpCoarseToFineProjectionMatrix,
             INTERP_HOOK pre_interp, INTERP_HOOK post_interp )
    {
        Vector<BCRec> bcs;
        for ( int iComp = 0; iComp < nComp; ++iComp ) {
            bcs.emplace_back( pLoBC[iComp+sComp], pHiBC[iComp+sComp] );
        }

        FPhysBC CrseBC( fpCrseFillPhysBC, pCrseGeom );
        FPhysBC FineBC( fpFineFillPhysBC, pFineGeom );

        auto *pCoarseToFineProjectionMatrix
               = reinterpret_cast<Real*>(vpCoarseToFineProjectionMatrix);
        Array4<Real const> CoarseToFineProjectionMatrix
                             ( pCoarseToFineProjectionMatrix,
                               {0,0,0}, {1,nFineV,nDOFX}, nDOFX );

        amrex::FillPatchTwoLevels
                 ( *MF, *MF_G, Time,
                   Vector<MultiFab*>{pCrseMF  , pCrseMF  +nCrse},
                   Vector<MultiFab*>{pCrseMF_G, pCrseMF_G+nCrse},
                   Vector<Real>     {CrseTime , CrseTime +nCrse},
                   Vector<MultiFab*>{pFineMF  , pFineMF  +nFine},
                   Vector<MultiFab*>{pFineMF_G, pFineMF_G+nFine},
                   Vector<Real>     {FineTime , FineTime +nFine},
                   sComp, dComp, nComp,
                   *pCrseGeom, *pFineGeom,
                   CrseBC, 0, FineBC, 0,
                   IntVect{AMREX_D_DECL(RefRatio,RefRatio,RefRatio)},
                   interp[interp_id], bcs, 0,
                   nDOFX, CoarseToFineProjectionMatrix,
                   FIInterpHook(pre_interp),
                   FIInterpHook(post_interp));
    }

    void amrex_fi_fillpatch_dgpointwise_two
           ( MultiFab * MF, Real Time,
             MultiFab * pCrseMF[],
             Real CrseTime[], int nCrse,
             MultiFab * pFineMF[],
             Real FineTime[], int nFine,
             int sComp, int dComp, int nComp,
             const Geometry* pCrseGeom, const Geometry * pFineGeom,
             FPhysBC::fill_physbc_funptr_t fpCrseFillPhysBC,
             FPhysBC::fill_physbc_funptr_t fpFineFillPhysBC,
             int RefRatio, int interp_id,
             int * pLoBC[], int * pHiBC[],
             int nFineV, int nDOFX, void * vpCoarseToFineProjectionMatrix,
             INTERP_HOOK pre_interp, INTERP_HOOK post_interp )
    {
        Vector<BCRec> bcs;
        for ( int iComp = 0; iComp < nComp; ++iComp ) {
            bcs.emplace_back( pLoBC[iComp+sComp], pHiBC[iComp+sComp] );
        }

        FPhysBC CrseBC( fpCrseFillPhysBC, pCrseGeom );
        FPhysBC FineBC( fpFineFillPhysBC, pFineGeom );

        auto *pCoarseToFineProjectionMatrix
               = reinterpret_cast<Real*>(vpCoarseToFineProjectionMatrix);
        Array4<Real const> CoarseToFineProjectionMatrix
                             ( pCoarseToFineProjectionMatrix,
                               {0,0,0}, {1,nFineV,nDOFX}, nDOFX );

        amrex::FillPatchTwoLevels
                 ( *MF, Time,
                   Vector<MultiFab*>{pCrseMF  , pCrseMF  +nCrse},
                   Vector<Real>     {CrseTime , CrseTime +nCrse},
                   Vector<MultiFab*>{pFineMF  , pFineMF  +nFine},
                   Vector<Real>     {FineTime , FineTime +nFine},
                   sComp, dComp, nComp,
                   *pCrseGeom, *pFineGeom,
                   CrseBC, 0, FineBC, 0,
                   IntVect{AMREX_D_DECL(RefRatio,RefRatio,RefRatio)},
                   interp[interp_id], bcs, 0,
                   nDOFX, CoarseToFineProjectionMatrix,
                   FIInterpHook(pre_interp),
                   FIInterpHook(post_interp));
    }

    void amrex_fi_fillpatch_two_faces (MultiFab* mf[], Real time,
                                       MultiFab* cmf[], Real ct[], int nc,
                                       MultiFab* fmf[], Real ft[], int nf,
                                       int scomp, int dcomp, int ncomp,
                                       const Geometry* cgeom, const Geometry* fgeom,
                                       FPhysBC::fill_physbc_funptr_t cfill[],
                                       FPhysBC::fill_physbc_funptr_t ffill[],
                                       int rr, int interp_id,
                                       int* lo_bc[], int* hi_bc[],
                                       INTERP_HOOK_ARR pre_interp, INTERP_HOOK_ARR post_interp)
    {
        Array<Vector<BCRec>, AMREX_SPACEDIM> bcs;
        for (int d = 0; d < AMREX_SPACEDIM; ++d)
        {
            for (int i = 0; i < ncomp; ++i)
                { bcs[d].emplace_back(lo_bc[d*(scomp+ncomp)+i+scomp],
                                      hi_bc[d*(scomp+ncomp)+i+scomp]); }
        }

        Vector<Array<MultiFab*, AMREX_SPACEDIM> > va_cmf(nc);
        Vector<Array<MultiFab*, AMREX_SPACEDIM> > va_fmf(nf);
        for (int i = 0; i < nc; ++i) {
            for (int d = 0; d < AMREX_SPACEDIM; ++d)
                { va_cmf[i][d] = cmf[i*AMREX_SPACEDIM+d]; }
        }
        for (int i = 0; i < nf; ++i) {
            for (int d = 0; d < AMREX_SPACEDIM; ++d)
                { va_fmf[i][d] = fmf[i*AMREX_SPACEDIM+d]; }
        }

        Array<FPhysBC, AMREX_SPACEDIM> cbc{ AMREX_D_DECL( FPhysBC(cfill[0], cgeom),
                                                          FPhysBC(cfill[1], cgeom),
                                                          FPhysBC(cfill[2], cgeom)) };
        Array<FPhysBC, AMREX_SPACEDIM> fbc{ AMREX_D_DECL( FPhysBC(ffill[0], fgeom),
                                                          FPhysBC(ffill[1], fgeom),
                                                          FPhysBC(ffill[2], fgeom)) };

        amrex::FillPatchTwoLevels(Array<MultiFab*, AMREX_SPACEDIM>{AMREX_D_DECL(mf[0], mf[1], mf[2])},
                                  time,
                                  va_cmf, Vector<Real>{ct, ct+nc},
                                  va_fmf, Vector<Real>{ft, ft+nf},
                                  scomp, dcomp, ncomp,
                                  *cgeom, *fgeom,
                                  cbc, 0, fbc, 0,
                                  IntVect{AMREX_D_DECL(rr,rr,rr)},
                                  interp[interp_id], bcs, 0,
                                  FIArrInterpHook(pre_interp),
                                  FIArrInterpHook(post_interp));
    }

    void amrex_fi_fillcoarsepatch
           ( MultiFab * MF, Real Time, const MultiFab * pCrseMF,
             int sComp, int dComp, int nComp,
             const Geometry * pCrseGeom, const Geometry * pFineGeom,
             FPhysBC::fill_physbc_funptr_t fpFillPhysBCCrse,
             FPhysBC::fill_physbc_funptr_t fpFillPhysBCFine,
             int RefRatio, int interp_id,
             int * pLoBC[], int * pHiBC[],
             INTERP_HOOK pre_interp, INTERP_HOOK post_interp)
    {
        Vector<BCRec> bcs;
        for ( int iComp = 0; iComp < nComp; ++iComp) {
            bcs.emplace_back( pLoBC[iComp+sComp], pHiBC[iComp+sComp] );
        }

        FPhysBC CrseBC( fpFillPhysBCCrse, pCrseGeom );
        FPhysBC FineBC( fpFillPhysBCFine, pFineGeom );

        amrex::InterpFromCoarseLevel
                 ( *MF, Time, *pCrseMF,
                   sComp, dComp, nComp,
                   *pCrseGeom, *pFineGeom,
                   CrseBC, 0, FineBC, 0,
                   IntVect{AMREX_D_DECL(RefRatio,RefRatio,RefRatio)},
                   interp[interp_id], bcs, 0,
                   FIInterpHook( pre_interp  ),
                   FIInterpHook( post_interp ) );
    }

    void amrex_fi_fillcoarsepatch_dgconservative
           ( MultiFab * MF, MultiFab * MF_G, Real Time,
             const MultiFab * pCrseMF, const MultiFab * pCrseMF_G,
             int sComp, int dComp, int nComp,
             const Geometry * pCrseGeom, const Geometry * pFineGeom,
             FPhysBC::fill_physbc_funptr_t fpFillPhysBCCrse,
             FPhysBC::fill_physbc_funptr_t fpFillPhysBCFine,
             int RefRatio, int interp_id,
             int * pLoBC[], int * pHiBC[],
             int nFineV, int nDOFX, void * vpCoarseToFineProjectionMatrix,
             INTERP_HOOK pre_interp, INTERP_HOOK post_interp)
    {
        Vector<BCRec> bcs;
        for ( int iComp = 0; iComp < nComp; ++iComp) {
            bcs.emplace_back( pLoBC[iComp+sComp], pHiBC[iComp+sComp] );
        }

        FPhysBC CrseBC( fpFillPhysBCCrse, pCrseGeom );
        FPhysBC FineBC( fpFillPhysBCFine, pFineGeom );

        auto *pCoarseToFineProjectionMatrix
               = reinterpret_cast<Real*>(vpCoarseToFineProjectionMatrix);
        Array4<Real const> CoarseToFineProjectionMatrix
                             ( pCoarseToFineProjectionMatrix,
                               {0,0,0}, {1,nFineV,nDOFX}, nDOFX );

        amrex::InterpFromCoarseLevel
                 ( *MF, *MF_G, Time, *pCrseMF, *pCrseMF_G,
                   sComp, dComp, nComp,
                   *pCrseGeom, *pFineGeom,
                   CrseBC, 0, FineBC, 0,
                   IntVect{AMREX_D_DECL(RefRatio,RefRatio,RefRatio)},
                   interp[interp_id], bcs, 0,
                   nDOFX, CoarseToFineProjectionMatrix,
                   FIInterpHook( pre_interp  ),
                   FIInterpHook( post_interp ) );
    }

    void amrex_fi_fillcoarsepatch_dgpointwise
           ( MultiFab * MF, Real Time,
             const MultiFab * pCrseMF,
             int sComp, int dComp, int nComp,
             const Geometry * pCrseGeom, const Geometry * pFineGeom,
             FPhysBC::fill_physbc_funptr_t fpFillPhysBCCrse,
             FPhysBC::fill_physbc_funptr_t fpFillPhysBCFine,
             int RefRatio, int interp_id,
             int * pLoBC[], int * pHiBC[],
             int nFineV, int nDOFX, void * vpCoarseToFineProjectionMatrix,
             INTERP_HOOK pre_interp, INTERP_HOOK post_interp)
    {
        Vector<BCRec> bcs;
        for ( int iComp = 0; iComp < nComp; ++iComp) {
            bcs.emplace_back( pLoBC[iComp+sComp], pHiBC[iComp+sComp] );
        }

        FPhysBC CrseBC( fpFillPhysBCCrse, pCrseGeom );
        FPhysBC FineBC( fpFillPhysBCFine, pFineGeom );

        auto *pCoarseToFineProjectionMatrix
               = reinterpret_cast<Real*>(vpCoarseToFineProjectionMatrix);
        Array4<Real const> CoarseToFineProjectionMatrix
                             ( pCoarseToFineProjectionMatrix,
                               {0,0,0}, {1,nFineV,nDOFX}, nDOFX );

        amrex::InterpFromCoarseLevel
                 ( *MF, Time, *pCrseMF,
                   sComp, dComp, nComp,
                   *pCrseGeom, *pFineGeom,
                   CrseBC, 0, FineBC, 0,
                   IntVect{AMREX_D_DECL(RefRatio,RefRatio,RefRatio)},
                   interp[interp_id], bcs, 0,
                   nDOFX, CoarseToFineProjectionMatrix,
                   FIInterpHook( pre_interp  ),
                   FIInterpHook( post_interp ) );
    }

    void amrex_fi_fillcoarsepatch_faces (MultiFab* mf[], Real time, MultiFab* cmf[],
                                         int scomp, int dcomp, int ncomp,
                                         const Geometry* cgeom, const Geometry* fgeom,
                                         FPhysBC::fill_physbc_funptr_t cfill[],
                                         FPhysBC::fill_physbc_funptr_t ffill[],
                                         int rr, int interp_id,
                                         int* lo_bc[], int* hi_bc[],
                                         INTERP_HOOK_ARR pre_interp, INTERP_HOOK_ARR post_interp)
    {
        Array<Vector<BCRec>, AMREX_SPACEDIM> bcs;
        for (int d = 0; d < AMREX_SPACEDIM; ++d)
        {
            for (int i = 0; i < ncomp; ++i)
                { bcs[d].emplace_back(lo_bc[d*(scomp+ncomp)+i+scomp],
                                      hi_bc[d*(scomp+ncomp)+i+scomp]); }
        }

        Array<MultiFab*, AMREX_SPACEDIM> a_mf {AMREX_D_DECL( mf[0], mf[1], mf[2])};
        Array<MultiFab*, AMREX_SPACEDIM> a_cmf{AMREX_D_DECL(cmf[0],cmf[1],cmf[2])};

        Array<FPhysBC, AMREX_SPACEDIM> cbc{ AMREX_D_DECL( FPhysBC(cfill[0], cgeom),
                                                          FPhysBC(cfill[1], cgeom),
                                                          FPhysBC(cfill[2], cgeom)) };
        Array<FPhysBC, AMREX_SPACEDIM> fbc{ AMREX_D_DECL( FPhysBC(ffill[0], fgeom),
                                                          FPhysBC(ffill[1], fgeom),
                                                          FPhysBC(ffill[2], fgeom)) };

        amrex::InterpFromCoarseLevel(a_mf, time, a_cmf,
                                     scomp, dcomp, ncomp,
                                     *cgeom, *fgeom,
                                     cbc, 0, fbc, 0,
                                     IntVect{AMREX_D_DECL(rr,rr,rr)},
                                     interp[interp_id], bcs, 0,
                                     FIArrInterpHook(pre_interp),
                                     FIArrInterpHook(post_interp));
    }
}
