#include <AMReX.H>
#include <AMReX_Interpolater.H>

using namespace amrex;

int main(int argc, char* argv[])
{
    amrex::Initialize(argc, argv);

    const int nNodes1D = 2;
    const int nDOFX    = std::pow(nNodes1D, AMREX_SPACEDIM);
    const int nFineV   = 4;

    DGInterpolater<nNodes1D, nDOFX, nFineV, amrex::DGBasis::Lagrange> myInterp;

    amrex::Finalize();

    return 0;
}
