#include "RMIISimulationDataGenerator.h"
#include "RMIIAnalyzerSettings.h"

#include <AnalyzerHelpers.h>

RMIISimulationDataGenerator::RMIISimulationDataGenerator()
:	mSerialText( "My first analyzer, woo hoo!" ),
	mStringIndex( 0 )
{
}

RMIISimulationDataGenerator::~RMIISimulationDataGenerator()
{
}

void RMIISimulationDataGenerator::Initialize( U32 simulation_sample_rate, RMIIAnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mSerialSimulationData.SetChannel( mSettings->mD0Channel );
	mSerialSimulationData.SetSampleRate( simulation_sample_rate );
	mSerialSimulationData.SetInitialBitState( BIT_HIGH );
}

U32 RMIISimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	// U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	// while( mSerialSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	// {
	// 	CreateSerialByte();
	// }

	*simulation_channel = &mSerialSimulationData;
	return 1;
}
