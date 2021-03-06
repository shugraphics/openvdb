# Copyright (c) 2012-2013 DreamWorks Animation LLC
#
# All rights reserved. This software is distributed under the
# Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
#
# Redistributions of source code must retain the above copyright
# and license notice and the following restrictions and disclaimer.
#
# *     Neither the name of DreamWorks Animation nor the names of
# its contributors may be used to endorse or promote products derived
# from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# IN NO EVENT SHALL THE COPYRIGHT HOLDERS' AND CONTRIBUTORS' AGGREGATE
# LIABILITY FOR ALL CLAIMS REGARDLESS OF THEIR BASIS EXCEED US$250.00.
#
# Makefile for the OpenVDB library

# See INSTALL for a list of requirements.
#
# Targets:
#   lib                 the OpenVDB library
#
#   doc                 HTML documentation (doc/html/index.html)
#   pdfdoc              PDF documentation (doc/latex/refman.pdf;
#                       requires LaTeX and ghostscript)
#   vdb_print           command-line tool to inspect OpenVDB files
#   vdb_view            command-line tool to view OpenVDB files
#   vdb_test            unit tests for the OpenVDB library
#
#   all                 [default target] all of the above
#   install             install all of the above except vdb_test
#                       into subdirectories of INSTALL_DIR
#   depend              recompute source file header dependencies
#   clean               delete generated files from the local directory
#   test                run tests
#
# Options:             
#   shared=no           link executables against static OpenVDB libraries
#                       (default: link against shared libraries)
#   debug=yes           build with debugging symbols and without optimization
#   verbose=yes         run commands (e.g., doxygen) in verbose mode


#
# The following variables must be defined, either here or on the command line
# (e.g., "make install INSTALL_DIR=/usr/local"):
#
# Note that if you plan to build the Houdini OpenVDB tools (distributed
# separately), you must build the OpenVDB library and the Houdini tools
# against compatible versions of the Boost, OpenEXR and TBB libraries.
# Fortunately, all three are included in the Houdini HDK, so the relevant
# variables below point by default to the HDK library and header directories:
# $(HDSO) and $(HT)/include, respectively.  (Source the houdini_setup script
# to set those two environment variables.)
#

# The directory into which to install libraries, executables and header files
INSTALL_DIR := /tmp/OpenVDB

# The parent directory of the boost/ header directory
BOOST_INCL_DIR := $(HT)/include

# The parent directory of the OpenEXR/ header directory (which contains half.h)
HALF_INCL_DIR := $(HT)/include
# The directory containing libHalf
HALF_LIB_DIR := $(HDSO)
HALF_LIB := -lHalf

# The parent directory of the tbb/ header directory
TBB_INCL_DIR := $(HT)/include
# The directory containing libtbb
TBB_LIB_DIR := $(HDSO)
TBB_LIB := -ltbb

# The parent directory of the cppunit/ header directory
# (leave blank if CppUnit is unavailable)
CPPUNIT_INCL_DIR := /rel/map/generic-2011.15/sys_include
# The directory containing libcppunit
CPPUNIT_LIB_DIR := /rel/depot/third_party_build/cppunit/1.10.2-7/opt-ws5-x86_64-gccWS5_64/lib/
CPPUNIT_LIB := -lcppunit

# The directory containing glfw.h
# (leave blank if GLFW is unavailable)
GLFW_INCL_DIR := /usr/local/include/GL
# The directory containing libglfw
GLFW_LIB_DIR := /usr/local/lib
GLFW_LIB := -lglfw

# The Doxygen executable
# (leave blank if Doxygen is unavailable)
DOXYGEN := doxygen


#
# Ideally, users shouldn't need to change anything below this line.
#

SHELL = /bin/bash

PWD ?= $(abspath .)

# Turn off implicit rules for speed.
.SUFFIXES:

# Determine the platform.
ifeq ("$(OS)","Windows_NT")
    WINDOWS_NT := 1
else
    UNAME_S := $(shell uname -s)
    ifeq ("$(UNAME_S)","Linux")
        LINUX := 1
    else
        ifeq ("$(UNAME_S)","Darwin")
            MBSD := 1
        endif
    endif
endif

ifeq (yes,$(strip $(debug)))
    OPTIMIZE := -g
else
    OPTIMIZE := -O3 -DNDEBUG
endif

ifeq (yes,$(strip $(verbose)))
    QUIET :=
else
    QUIET := > /dev/null
endif

INCLDIRS := -I . -I .. -I $(BOOST_INCL_DIR) -I $(HALF_INCL_DIR) -I $(TBB_INCL_DIR)

CXXFLAGS += -pthread $(OPTIMIZE) $(INCLDIRS)

LIBS := \
    -ldl -lm -lz \
    -L$(HALF_LIB_DIR) $(HALF_LIB) \
    -L$(TBB_LIB_DIR) $(TBB_LIB) \
#
LIBS_RPATH := \
    -ldl -lm -lz \
    -Wl,-rpath,$(HALF_LIB_DIR) -L$(HALF_LIB_DIR) $(HALF_LIB) \
    -Wl,-rpath,$(TBB_LIB_DIR) -L$(TBB_LIB_DIR) $(TBB_LIB) \
#
ifdef LINUX
  LIBS += -lrt
  LIBS_RPATH += -lrt
endif

INCLUDE_NAMES := \
    Exceptions.h \
    Grid.h \
    io/Archive.h \
    io/Compression.h \
    io/File.h \
    io/GridDescriptor.h \
    io/Stream.h \
    math/BBox.h \
    math/Coord.h \
    math/FiniteDifference.h \
    math/Hermite.h \
    math/LegacyFrustum.h \
    math/Maps.h \
    math/Mat.h \
    math/Mat3.h \
    math/Mat4.h \
    math/Math.h \
    math/Operators.h \
    math/Proximity.h \
    math/QuantizedUnitVec.h \
    math/Quat.h \
    math/Stencils.h \
    math/Transform.h\
    math/Tuple.h\
    math/Vec2.h \
    math/Vec3.h \
    math/Vec4.h \
    Metadata.h \
    metadata/Metadata.h \
    metadata/MetaMap.h \
    metadata/StringMetadata.h \
    openvdb.h \
    Platform.h \
    tools/Composite.h \
    tools/Filter.h \
    tools/GridOperators.h \
    tools/GridTransformer.h \
    tools/Interpolation.h \
    tools/LevelSetAdvect.h \
    tools/LevelSetFilter.h \
    tools/LevelSetRebuild.h \
    tools/LevelSetSphere.h \
    tools/LevelSetTracker.h \
    tools/LevelSetUtil.h \
    tools/MeshToVolume.h \
    tools/Morphology.h \
    tools/ParticlesToLevelSet.h \
    tools/PointAdvect.h \
    tools/PointScatter.h \
    tools/ValueTransformer.h \
    tools/VolumeToMesh.h \
    tree/InternalNode.h \
    tree/Iterator.h \
    tree/LeafManager.h \
    tree/LeafNode.h \
    tree/LeafNodeBool.h \
    tree/NodeUnion.h \
    tree/RootNode.h \
    tree/Tree.h \
    tree/TreeIterator.h \
    tree/Util.h \
    tree/ValueAccessor.h \
    Types.h \
    util/Formats.h \
    util/logging.h \
    util/MapsUtil.h \
    util/Name.h \
    util/NodeMasks.h \
    util/NullInterrupter.h \
    util/Util.h \
    version.h \
#

SRC_NAMES := \
    Grid.cc \
    io/Archive.cc \
    io/Compression.cc \
    io/File.cc \
    io/GridDescriptor.cc \
    io/Stream.cc \
    math/Hermite.cc \
    math/Maps.cc \
    math/Proximity.cc \
    math/QuantizedUnitVec.cc \
    math/Transform.cc \
    metadata/Metadata.cc \
    metadata/MetaMap.cc \
    Platform.cc \
    util/Formats.cc \
    util/Util.cc \
#

UNITTEST_INCLUDE_NAMES := \
    unittest/util.h \
#

UNITTEST_SRC_NAMES := \
    unittest/main.cc \
    unittest/TestBBox.cc \
    unittest/TestCoord.cc \
    unittest/TestCpt.cc \
    unittest/TestCurl.cc \
    unittest/TestDivergence.cc \
    unittest/TestDoubleMetadata.cc \
    unittest/TestExceptions.cc \
    unittest/TestFile.cc \
    unittest/TestFloatMetadata.cc \
    unittest/TestGradient.cc \
    unittest/TestGrid.cc \
    unittest/TestGridBbox.cc \
    unittest/TestGridDescriptor.cc \
    unittest/TestGridIO.cc \
    unittest/TestGridTransformer.cc \
    unittest/TestHermite.cc \
    unittest/TestInit.cc \
    unittest/TestInt32Metadata.cc \
    unittest/TestInt64Metadata.cc \
    unittest/TestInternalOrigin.cc \
    unittest/TestLaplacian.cc \
    unittest/TestLeaf.cc \
    unittest/TestLeafBool.cc \
    unittest/TestLeafIO.cc \
    unittest/TestLeafOrigin.cc \
    unittest/TestLevelSetUtil.cc \
    unittest/TestLinearInterp.cc \
    unittest/TestMaps.cc \
    unittest/TestMat4Metadata.cc \
    unittest/TestMeanCurvature.cc \
    unittest/TestMeshToVolume.cc \
    unittest/TestMetadata.cc \
    unittest/TestMetadataIO.cc \
    unittest/TestMetaMap.cc \
    unittest/TestName.cc \
    unittest/TestNodeIterator.cc \
    unittest/TestParticlesToLevelSet.cc \
    unittest/TestPrePostAPI.cc \
    unittest/TestQuadraticInterp.cc \
    unittest/TestQuantizedUnitVec.cc \
    unittest/TestQuat.cc \
    unittest/TestStream.cc \
    unittest/TestStringMetadata.cc \
    unittest/TestTools.cc \
    unittest/TestTransform.cc \
    unittest/TestTree.cc \
    unittest/TestTreeCombine.cc \
    unittest/TestTreeGetSetValues.cc \
    unittest/TestTreeIterators.cc \
    unittest/TestTreeVisitor.cc \
    unittest/TestValueAccessor.cc \
    unittest/TestVec2Metadata.cc \
    unittest/TestVec3Metadata.cc \
    unittest/TestVolumeToMesh.cc \
#

DOC_FILES := doc/doc.txt doc/faq.txt doc/changes.txt doc/examplecode.txt doc/api_0_98_0.txt
DOC_INDEX := doc/html/index.html
DOC_PDF := doc/latex/refman.pdf

VIEWER_INCLUDE_NAMES := \
    cmd/openvdb_view/MCTable.h \
    cmd/openvdb_view/RenderModules.h \
    cmd/openvdb_view/Viewer.h \
#

VIEWER_SRC_NAMES := \
    cmd/openvdb_view/main.cc \
    cmd/openvdb_view/MCTable.cc \
    cmd/openvdb_view/RenderModules.cc \
    cmd/openvdb_view/Viewer.cc \
#

CMD_INCLUDE_NAMES := \
    $(VIEWER_INCLUDE_NAMES) \
#

CMD_SRC_NAMES := \
    cmd/openvdb_print/main.cc \
    $(VIEWER_SRC_NAMES) \
#

HEADER_SUBDIRS := $(dir $(INCLUDE_NAMES))

ALL_INCLUDE_FILES := \
    $(INCLUDE_NAMES) \
    $(UNITTEST_INCLUDE_NAMES) \
    $(CMD_INCLUDE_NAMES) \
#
SRC_FILES := \
    $(SRC_NAMES) \
    $(UNITTEST_SRC_NAMES) \
    $(CMD_SRC_NAMES) \
#
ALL_SRC_FILES := $(SRC_FILES)

OBJ_NAMES := $(SRC_NAMES:.cc=.o)
UNITTEST_OBJ_NAMES := $(UNITTEST_SRC_NAMES:.cc=.o)
VIEWER_OBJ_NAMES := $(VIEWER_SRC_NAMES:.cc=.o)

LIB_MAJOR_VERSION=$(shell grep 'OPENVDB_LIBRARY_MAJOR_VERSION = ' \
    version.h | sed 's/[^0-9]*//g')
LIB_MINOR_VERSION=$(shell grep 'OPENVDB_LIBRARY_MINOR_VERSION = ' \
    version.h | sed 's/[^0-9]*//g')
LIB_PATCH_VERSION=$(shell grep 'OPENVDB_LIBRARY_PATCH_VERSION = ' \
    version.h | sed 's/[^0-9]*//g')

LIB_VERSION=$(LIB_MINOR_VERSION).$(LIB_PATCH_VERSION)

LIBOPENVDB_NAME=libopenvdb
LIBOPENVDB_STATIC := $(LIBOPENVDB_NAME).a
LIBOPENVDB_SHARED := $(LIBOPENVDB_NAME).so.$(LIB_VERSION)

ifeq (no,$(strip $(shared)))
    LIBOPENVDB := $(LIBOPENVDB_STATIC)
else
    LIBOPENVDB := $(LIBOPENVDB_SHARED)

    # For debugging, pass "rpath=yes" to encode the path to libopenvdb.so
    # into commands.  With "rpath=no" (the default setting), the directory
    # containing libopenvdb.so will need to be added
    # to LD_LIBRARY_PATH.
    ifeq (yes,$(strip $(rpath)))
        LIBOPENVDB_RPATH := -Wl,-rpath,$(PWD)
    else
        LIBOPENVDB_RPATH :=
    endif # rpath
endif # shared

DEPEND := dependencies

# Get the list of dependencies that are newer than the current target,
# but limit the list to at most three entries.
list_deps = $(if $(wordlist 4,5,$(?F)),$(firstword $(?F)) and others,$(wordlist 1,3,$(?F)))

ALL_TARGETS := \
    $(LIBOPENVDB) \
    vdb_test \
    vdb_print \
    vdb_view \
    $(DEPEND) \
    $(LIBOPENVDB_NAME).so \
#

.SUFFIXES: .o .cc

.PHONY: all clean depend doc install lib pdfdoc test

.cc.o:
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) -c $(CXXFLAGS) -fPIC -o $@ $<

all: lib vdb_print vdb_test depend

$(OBJ_NAMES): %.o: %.cc
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) -c -DOPENVDB_PRIVATE $(CXXFLAGS) -fPIC -o $@ $<

ifneq (no,$(strip $(shared)))

# Build shared library
lib: $(LIBOPENVDB_NAME).so

$(LIBOPENVDB_NAME).so: $(LIBOPENVDB_SHARED)
	ln -f -s $< $@

$(LIBOPENVDB_SHARED): $(OBJ_NAMES)
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) $(CXXFLAGS) -shared -o $@ $^ $(LIBS_RPATH)

else

# Build static library
lib: $(LIBOPENVDB)

$(LIBOPENVDB_STATIC): $(OBJ_NAMES)
	@echo "Building $@ because of $(call list_deps)"
	$(AR) cr $@ $^

endif # shared

$(DOC_INDEX): doxygen-config $(INCLUDE_NAMES) $(SRC_NAMES) $(DOC_FILES)
	@echo "Generating documentation because of $(call list_deps)"
	echo 'OUTPUT_DIRECTORY=./doc' | cat doxygen-config - | $(DOXYGEN) - $(QUIET)

$(DOC_PDF): doxygen-config $(INCLUDE_NAMES) $(SRC_NAMES) $(DOC_FILES)
	@echo "Generating documentation because of $(call list_deps)"
	echo -e 'OUTPUT_DIRECTORY=./doc\nGENERATE_LATEX=YES\nGENERATE_HTML=NO' \
	    | cat doxygen-config - | $(DOXYGEN) - $(QUIET) \
	    && cd ./doc/latex && make refman.pdf $(QUIET) \
	    && echo 'Created doc/latex/refman.pdf'

ifneq ($(strip $(DOXYGEN)),)
doc: $(DOC_INDEX)
pdfdoc: $(DOC_PDF)
else
doc:
	@echo "$@"': $$DOXYGEN is undefined'
pdfdoc:
	@echo "$@"': $$DOXYGEN is undefined'
endif

vdb_print: $(LIBOPENVDB) cmd/openvdb_print/main.cc
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) $(CXXFLAGS) -o $@ cmd/openvdb_print/main.cc -I . \
	    $(LIBS_RPATH) $(LIBOPENVDB_RPATH) -L$(PWD) $(LIBOPENVDB)

$(VIEWER_OBJ_NAMES): $(VIEWER_INCLUDE_NAMES)
$(VIEWER_OBJ_NAMES): %.o: %.cc
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) -c $(CXXFLAGS) -I . -I $(GLFW_INCL_DIR) -DGL_GLEXT_PROTOTYPES=1 -fPIC -o $@ $<

ifneq (3,$(words $(strip $(GLFW_LIB_DIR) $(GLFW_INCL_DIR) $(GLFW_LIB))))
vdb_view:
	@echo "$@"': $$GLFW_INCL_DIR is undefined'
else
ifdef MBSD
vdb_view: $(LIBOPENVDB) $(VIEWER_OBJ_NAMES)
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) $(CXXFLAGS) -o $@ $(VIEWER_OBJ_NAMES) -I . \
	    -Wl,-rpath,$(GLFW_LIB_DIR) -L$(GLFW_LIB_DIR) $(GLFW_LIB) \
	    -framework Cocoa -framework OpenGL -framework IOKit \
	    $(LIBS_RPATH) $(LIBOPENVDB_RPATH) -L$(PWD) $(LIBOPENVDB)
else
vdb_view: $(LIBOPENVDB) $(VIEWER_OBJ_NAMES)
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) $(CXXFLAGS) -o $@ $(VIEWER_OBJ_NAMES) -I . \
	    -Wl,-rpath,$(GLFW_LIB_DIR) -L$(GLFW_LIB_DIR) $(GLFW_LIB) -lGL -lGLU \
	    $(LIBS_RPATH) $(LIBOPENVDB_RPATH) -L$(PWD) $(LIBOPENVDB)
endif
endif

$(UNITTEST_OBJ_NAMES): %.o: %.cc
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) -c $(CXXFLAGS) -I $(CPPUNIT_INCL_DIR) -fPIC -o $@ $<

ifneq ($(strip $(CPPUNIT_INCL_DIR)),)
vdb_test: $(LIBOPENVDB) $(UNITTEST_OBJ_NAMES)
	@echo "Building $@ because of $(call list_deps)"
	$(CXX) $(CXXFLAGS) -o $@ $(UNITTEST_OBJ_NAMES) $(LIBS_RPATH) \
	    -Wl,-rpath,$(CPPUNIT_LIB_DIR) -L$(CPPUNIT_LIB_DIR) $(CPPUNIT_LIB) \
	    $(LIBOPENVDB_RPATH) -L$(PWD) $(LIBOPENVDB)

test: vdb_test
	export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$(PWD); ./vdb_test
else
vdb_test:
	@echo "$@"': $$(CPPUNIT_INCL_DIR) is undefined'
test:                                            
	@echo "$@"': $$(CPPUNIT_INCL_DIR) is undefined'
endif

install: lib vdb_print vdb_view doc
	mkdir -p $(INSTALL_DIR)/include/openvdb
	@echo "Created $(INSTALL_DIR)/include/openvdb"
	pushd $(INSTALL_DIR)/include/openvdb > /dev/null; \
	    mkdir -p $(HEADER_SUBDIRS); popd > /dev/null
	for f in $(INCLUDE_NAMES); \
	    do cp -f $$f $(INSTALL_DIR)/include/openvdb/$$f; done
	@#
	mkdir -p $(INSTALL_DIR)/lib
	@echo "Created $(INSTALL_DIR)/lib/"
	cp -f $(LIBOPENVDB) $(INSTALL_DIR)/lib
	pushd $(INSTALL_DIR)/lib > /dev/null; \
	    if [ -f $(LIBOPENVDB_SHARED) ]; then \
	        ln -f -s $(LIBOPENVDB_SHARED) $(LIBOPENVDB_NAME).so; fi; \
	    popd > /dev/null
	@echo "Copied libopenvdb to $(INSTALL_DIR)/lib/"
	@#
	mkdir -p $(INSTALL_DIR)/bin
	@echo "Created $(INSTALL_DIR)/bin/"
	cp -f vdb_print $(INSTALL_DIR)/bin
	@echo "Copied vdb_print to $(INSTALL_DIR)/bin/"
	if [ -f vdb_view ]; \
	then \
	    cp -f vdb_view $(INSTALL_DIR)/bin; \
	    echo "Copied vdb_view to $(INSTALL_DIR)/bin/"; \
	fi
	@#
	if [ -d doc/html ]; \
	then \
	    mkdir -p $(INSTALL_DIR)/share/doc/openvdb; \
	    echo "Created $(INSTALL_DIR)/share/doc/openvdb/"; \
	    cp -r -f doc/html $(INSTALL_DIR)/share/doc/openvdb; \
	    echo "Copied documentation to $(INSTALL_DIR)/share/doc/openvdb/"; \
	fi

# TODO: This accumulates all source file dependencies into a single file
# containing a rule for each *.o file.  Consider generating a separate
# dependency file for each *.o file instead.
$(DEPEND): $(ALL_INCLUDE_FILES) $(ALL_SRC_FILES)
	@echo "Generating dependencies because of $(call list_deps)"
	$(RM) $(DEPEND)
	for f in $(SRC_NAMES) $(CMD_SRC_NAMES); \
	    do $(CXX) $(CXXFLAGS) -O0 \
	        -MM $$f -MT `echo $$f | sed 's%\.[^.]*%.o%'` >> $(DEPEND); \
	done
	if [ -d "$(CPPUNIT_INCL_DIR)" ]; \
	then \
	    for f in $(UNITTEST_SRC_NAMES); \
	        do $(CXX) $(CXXFLAGS) -O0 \
	            -MM $$f -MT `echo $$f | sed 's%\.[^.]*%.o%'` \
	            -I $(CPPUNIT_INCL_DIR) >> $(DEPEND); \
	    done; \
	fi

depend: $(DEPEND)

clean:
	$(RM) $(OBJ_NAMES) $(ALL_TARGETS) $(DEPEND)
	$(RM) $(LIBOPENVDB_STATIC)
	$(RM) $(LIBOPENVDB_SHARED)
	$(RM) $(VIEWER_OBJ_NAMES)
	$(RM) $(UNITTEST_OBJ_NAMES)
	$(RM) -r ./doc/html ./doc/latex

ifneq ($(strip $(wildcard $(DEPEND))),)
    include $(DEPEND)
endif

# Copyright (c) 2012-2013 DreamWorks Animation LLC
# All rights reserved. This software is distributed under the
# Mozilla Public License 2.0 ( http://www.mozilla.org/MPL/2.0/ )
