# -*- makefile -*-
#  This is not a standalone makefile. It includes flags to control
#  the structure of the application in this directory
#
# --------
#  Command line arguments
# The type of computation being run (POSIT, FLOAT_POSIT, FLOAT)
TYPE ?= POSIT

# The width of the posit input data values. This is only relevant when TYPE = POSIT
PWIDTH ?= PWIDTH_8

# --------

# The benchmark being run
APP_FLAGS = -D$(TYPE) 
APP_FLAGS += -D$(PWIDTH)
APP_FLAGS += -DVSZ=$(VSZ)
APP_FLAGS += -D$(USE_FMADD)
APP_FLAGS += -D$(CACHES)

include ../Makefile
