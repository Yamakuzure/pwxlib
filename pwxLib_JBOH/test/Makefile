.PHONY: all clean

CXX       = g++
CXXFLAGS += -I. -I..
VPATH     = . ../pwxLib ../pwxLib/internal ../pwxLib/tools
LDFLAGS  +=
RNGOUT    = testRNG
RNGMOD    = testRandom.o
SCTOUT    = testSCT
SCTMOD    = testSinCosTables.o
MRFOUT    = testMRF
MRFMOD    = testMemRing.o
CFHOUT    = testCFH
CFHMOD    = testCfHandler.o
ARGOUT    = testArgs
ARGMOD    = testArgs.o
CWCOUT    = testColor
CWCMOD    = testColor.o
TESTPROG  = $(RNGOUT) $(SCTOUT) $(MRFOUT) $(CFHOUT) $(ARGOUT) $(CWCOUT)
TESTMODS  = $(RNGMOD) $(SCTMOD) $(MRFMOD) $(CFHMOD) $(ARGMOD) $(CWCMOD)

all: $(TESTPROG)

clean:
	@echo "Cleaning test programs"
	@rm -rf $(TESTPROG) $(TESTMODS)

$(RNGOUT): $(RNGMOD)
	@echo "Linking $(RNGOUT) ..."
	@$(CXX) -o $(RNGOUT) $(LDFLAGS) $(RNGMOD)

$(SCTOUT): $(SCTMOD)
	@echo "Linking $(SCTOUT) ..."
	@$(CXX) -o $(SCTOUT) $(LDFLAGS) $(SCTMOD)

$(MRFOUT): $(MRFMOD)
	@echo "Linking $(MRFOUT) ..."
	@$(CXX) -o $(MRFOUT) $(LDFLAGS) $(MRFMOD)

$(CFHOUT): $(CFHMOD)
	@echo "Linking $(CFHOUT) ..."
	@$(CXX) -o $(CFHOUT) $(LDFLAGS) $(CFHMOD)

$(ARGOUT): $(ARGMOD)
	@echo "Linking $(ARGOUT) ..."
	@$(CXX) -o $(ARGOUT) $(LDFLAGS) $(ARGMOD)

$(CWCOUT): $(CWCMOD)
	@echo "Linking $(CWCOUT) ..."
	@$(CXX) -o $(CWCOUT) $(LDFLAGS) $(CWCMOD)

%.o: %.cpp
	@echo "Generating $@ ..."
	@$(CXX) -c $< -o $@ $(CXXFLAGS)

# --- Dependencies ---
testMemRing.cpp: CMemRingFactory.h
testRandom.cpp: CRandom.h
testSinCosTables.cpp: CSinCosTables.h
testCfHandler.cpp: CCnfFileHandler.h
testArgs.cpp: Args.h
testColor.cpp: CWaveColor.h
Args.h: DefaultDefines.h ArgTypesBase.h ArgTypes.h ArgImpl.h StreamHelpers.h CMemRingFactory.h
CCnfFileHandler.h: DefaultDefines.h StreamHelpers.h CMemRingFactory.h CfhTypes.h CfhImpl.h CConfigDataImpl.h CConfigGroupImpl.h CConfigFileImpl.h
CRandom.h: DefaultDefines.h CRNGImpl.h
DefaultDefines.h: DefaultTempTools.h MSVCwrap.h
MSVCwrap.h: MSVCwarnPop.h MSVCwarnPush.h
CRNGImpl.h: CRandomStatics.h CRandomHash.h CRandomWordConstants.h CRandomName.h CRandomNoise.h CRandomRandom.h CRandomSimplex.h
CSinCosTables.h: DefaultDefines.h CSCTImpl.h
CSCTImpl.h: MathHelpers.h
CMemRingFactory.h: DefaultDefines.h CMRFImpl.h TMemRing.h
TMemRing.h: MRFexceptions.h CMemRing.h TItem.h
TItem.h: CItem.h TMRIImpl.h
CItem.h: CMRIImpl.h
CWaveColor.h: sWave.h CWaveColorImpl.h CWCexceptions.h
CWCexceptions.h: Exception.h
MRFexceptions.h: Exception.h
