#include "RMIIAnalyzer.h"
#include "RMIIAnalyzerSettings.h"
#include <AnalyzerChannelData.h>


// Helper
#define RisingEdge( channel ) \
{ \
	if( channel->GetBitState() == BIT_HIGH ) \
		channel->AdvanceToNextEdge(); \
	channel->AdvanceToNextEdge(); \
}

#define AddFrame(type, data, start, stop) \
{ \
	Frame frame; \
	frame.mType = type; \
	frame.mData1 = data; \
	frame.mFlags = 0; \
	frame.mStartingSampleInclusive = start; \
	frame.mEndingSampleInclusive = stop; \
	mResults->AddFrame( frame ); \
	mResults->CommitResults(); \
	ReportProgress( frame.mEndingSampleInclusive ); \
}

RMIIAnalyzer::RMIIAnalyzer()
:	Analyzer2(),
	mSettings( new RMIIAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

RMIIAnalyzer::~RMIIAnalyzer()
{
	KillThread();
}

void RMIIAnalyzer::SetupResults()
{
	mResults.reset( new RMIIAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mD0Channel );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mD1Channel );
}

void RMIIAnalyzer::WorkerThread()
{
	mRefClk = GetAnalyzerChannelData( mSettings->mRefClkChannel );
	mD0 = GetAnalyzerChannelData( mSettings->mD0Channel );
	mD1 = GetAnalyzerChannelData( mSettings->mD1Channel );
	mEn = GetAnalyzerChannelData( mSettings->mEnChannel );

	// FSM state
	bool in_frame = false;
	U64 starting_sample;
	U8 rx;
	BitState crs_dv;
	U8 preamble = mSettings->mPreamble;

	for( ; ; )
	{
		U8 data = 0;
		U8 mask = 1 << 7;

		// Step 1: detect start of frame
		RisingEdge(mEn);
		U64 sample_number = mEn->GetSampleNumber();
		mRefClk->AdvanceToAbsPosition(sample_number);
		mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mRefClkChannel );

		in_frame = true;
		rx = 0;

		// Step 2: detect beginning of preamble
		while( true )
		{
			RisingEdge( mRefClk );
			U64 sample_number = mRefClk->GetSampleNumber();
			mEn->AdvanceToAbsPosition(sample_number);
			mD0->AdvanceToAbsPosition(sample_number);
			mD1->AdvanceToAbsPosition(sample_number);

			if( mEn->GetBitState() == BIT_LOW ) {
				break;
			}
			else {
				rx = mD0->GetBitState() == BIT_HIGH ? 1 << 6 : 0;
				rx = mD1->GetBitState() == BIT_HIGH ? rx + (2 << 6) : rx;
				if( rx != 0 ) {
					starting_sample = mRefClk->GetSampleNumber();
					in_frame = true;
					// Add Markers
					mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mEnChannel );
					mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mD0Channel );
					mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mD1Channel );
					break;
				}
			}
		}

		// Step 3: detect end of preamble (0xD5)
		while( in_frame )
		{
			RisingEdge( mRefClk );
			U64 sample_number = mRefClk->GetSampleNumber();
			mEn->AdvanceToAbsPosition(sample_number);
			mD0->AdvanceToAbsPosition(sample_number);
			mD1->AdvanceToAbsPosition(sample_number);
			// Add Markers
			mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mEnChannel );
			mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mD0Channel );
			mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mD1Channel );
			if( mEn->GetBitState() == BIT_LOW ) {
				in_frame = false;
				break;
			}
			rx = rx >> 2;
			rx = mD0->GetBitState() == BIT_HIGH ? rx + (1 << 6) : rx;
			rx = mD1->GetBitState() == BIT_HIGH ? rx + (2 << 6) : rx;
			if( rx == preamble ) {
				// add preamble end mark
				AddFrame(FRAME_TYPE_PREAMBLE, rx, starting_sample, mRefClk->GetSampleNumber());
				starting_sample = mRefClk->GetSampleNumber();
				break;
			}
		}

		// step 4: unpack bytes and detect frame end
		int bitcnt = 0;
		int crs_dv_zero_cnt = 0;
		while ( in_frame ) {
			// we have to be a bit smart to correctly detect the end of frame with RMII 1.2
			RisingEdge( mRefClk );
			U64 sample_number = mRefClk->GetSampleNumber();
			mEn->AdvanceToAbsPosition(sample_number);
			mD0->AdvanceToAbsPosition(sample_number);
			mD1->AdvanceToAbsPosition(sample_number);
			// Add Markers
			mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mEnChannel );
			mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mD0Channel );
			mResults->AddMarker( sample_number, AnalyzerResults::Dot, mSettings->mD1Channel );
			if( mEn->GetBitState() == BIT_LOW ) {
				crs_dv_zero_cnt++;
				if( crs_dv_zero_cnt == 2 ) {
					in_frame = false;
					break;
				}
			}
			else {
				crs_dv_zero_cnt = 0;
			}

			// receiving bytes, 2 bits at a time
			rx = rx >> 2;
			rx = mD0->GetBitState() == BIT_HIGH ? rx + (1 << 6) : rx;
			rx = mD1->GetBitState() == BIT_HIGH ? rx + (2 << 6) : rx;

			bitcnt += 2;
			if( bitcnt == 8 ) {
				// add byte mark
				AddFrame(FRAME_TYPE_DATA, rx, starting_sample, mRefClk->GetSampleNumber());
				starting_sample = mRefClk->GetSampleNumber();
				bitcnt = 0;
			}
		}

		if( !mEn->DoMoreTransitionsExistInCurrentData() ) {
			break;
		}
	}
}

bool RMIIAnalyzer::NeedsRerun()
{
	return false;
}

U32 RMIIAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 RMIIAnalyzer::GetMinimumSampleRateHz()
{
	return 100E6;
}

const char* RMIIAnalyzer::GetAnalyzerName() const
{
	return "RMII";
}

const char* GetAnalyzerName()
{
	return "RMII";
}

Analyzer* CreateAnalyzer()
{
	return new RMIIAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}