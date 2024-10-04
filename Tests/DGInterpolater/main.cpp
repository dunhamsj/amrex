#include <AMReX.H>
#include <AMReX_Interpolater.H>

using namespace amrex;

int main(int argc, char* argv[])
{
    amrex::Initialize(argc, argv);

    const int nNodes1D = 2;
    const int nFineV   = std::pow(2, AMREX_SPACEDIM);

    DGInterpolater<nNodes1D, nFineV, amrex::DGBasis::Lagrange> myInterp;

    amrex::Finalize();

    return 0;
}
