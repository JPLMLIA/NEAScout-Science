#
#   Copyright 2004-2008, by the California Institute of Technology.
#   ALL RIGHTS RESERVED. United States Government Sponsorship
#   acknowledged. Any commercial use must be negotiated with the Office
#   of Technology Transfer at the California Institute of Technology.
#
#   Information included herein is controlled under the International
#   Traffic in Arms Regulations ("ITAR") by the U.S. Department of State.
#   Export or transfer of this information to a Foreign Person or foreign
#   entity requires an export license issued by the U.S. State Department
#   or an ITAR exemption prior to the export or transfer.
#

# This is a template for the mod.mk file that goes in each module
# and each module's subdirectories.
# With a fresh checkout, "make gen_make" should be invoked. It should also be
# run if any of the variables are updated. Any unused variables can 
# be deleted from the file.

# There are some standard files that are included for reference

SRC = neascout_scienceComponentAi.xml \
      neascout_scienceComponentImpl.cpp \
      asp/asp_bbox.c \
      asp/asp_calib.c \
      asp/asp_comedi.c \
      asp/asp_common.c \
      asp/asp_coref.c \
      asp/asp_coshift.c \
      asp/asp_crop.c \
      asp/asp_error.c \
      asp/asp_gsmooth.c \
      asp/asp_image2d.c \
      icer/icer_compress-flushy.c \
      icer/icer_entropy_int-flushy.c \
      icer/icer_globals-flushy.c \
      icer/icer_transform-flushy.c

INCLUDE = asp icer
HDR = neascout_scienceComponentImpl.hpp
SUBDIRS = test
