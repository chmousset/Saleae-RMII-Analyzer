#ifndef RMII_ANALYZER_H
#define RMII_ANALYZER_H

#include <Analyzer.h>
#include "RMIIAnalyzerResults.h"
#include "RMIISimulationDataGenerator.h"

#define FRAME_TYPE_PREAMBLE 1
#define FRAME_TYPE_DATA 2

class RMIIAnalyzerSettings;
class ANALYZER_EXPORT RMIIAnalyzer : public Analyzer2
{
public:
	RMIIAnalyzer();
	virtual ~RMIIAnalyzer();


	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< RMIIAnalyzerSettings > mSettings;
	std::auto_ptr< RMIIAnalyzerResults > mResults;
	AnalyzerChannelData* mRefClk;
	AnalyzerChannelData* mD0;
	AnalyzerChannelData* mD1;
	AnalyzerChannelData* mEn;
	RMIISimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	// U32 mSampleRateHz;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //RMII_ANALYZER_H
