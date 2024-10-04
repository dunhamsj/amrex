# XXX_HOME defines the directory in which all the XXX source code is located
# If you set XXX_HOME as an environment variable, this line will be ignored
AMREX_HOME  ?= $(AMREX_DIR)

PRECISION = DOUBLE

DEBUG = TRUE

USE_MPI = TRUE
USE_OMP = FALSE

DIM = 1

EBASE = main

# Set pre-processor directives
# This is also where, e.g., USE_HDF5 and USE_PETSC are used
include $(AMREX_HOME)/Tools/GNUMake/Make.defs

# Include basic AMReX framework
include $(AMREX_HOME)/Src/Base/Make.package

include Make.package

# Include rules for compilation
include $(AMREX_HOME)/Tools/GNUMake/Make.rules
