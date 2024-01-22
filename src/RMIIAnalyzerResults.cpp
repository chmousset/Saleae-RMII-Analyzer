#include "RMIIAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "RMIIAnalyzer.h"
#include "RMIIAnalyzerSettings.h"
#include <iostream>
#include <fstream>

RMIIAnalyzerResults::RMIIAnalyzerResults( RMIIAnalyzer* analyzer, RMIIAnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

RMIIAnalyzerResults::~RMIIAnalyzerResults()
{
}

void RMIIAnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	switch(frame.mType) {
	case 1: // Preamble
		AddResultString("Preamble (", number_str, ")");
		break;
	case 2: // Data
		AddResultString( number_str );
		break;
	}
}

void RMIIAnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();
	U64 num_frames = GetNumFrames();

	std::ofstream file_stream( file, std::ios::out );
	file_stream << "Time [s],packet";

	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		switch( frame.mType ) {
		case FRAME_TYPE_PREAMBLE:
			char time_str[128];
			AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );
			file_stream << std::endl << time_str << ",";
			break;
		case FRAME_TYPE_DATA:
			char number_str[128];
			AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
			file_stream << " " << number_str;
			break;
		}
		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void RMIIAnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	AddTabularText( number_str );
#endif
}

void RMIIAnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void RMIIAnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}