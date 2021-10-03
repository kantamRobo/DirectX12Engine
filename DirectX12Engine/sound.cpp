
/*

	 Initialize XAudio2
	
CoInitializeEx(nullptr, COINIT_MULTITHREADED);

IXAudio2* pXAudio2 = nullptr;



*/
/*
UINT32 flags = 0;
//#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*///) && defined(_DEBUG)
//flags |= XAUDIO2_DEBUG_ENGINE;
//#endif
//HRESULT hr = XAudio2Create(&pXAudio2, flags);

/*
if (FAILED(hr))
{
	wprintf(L"Failed to init XAudio2 engine: %#X\n", hr);
	CoUninitialize();
	return 0;
}
*/
//#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/) && defined(_DEBUG)
// To see the trace output, you need to view ETW logs for this application:
//    Go to Control Panel, Administrative Tools, Event Viewer.
//    View->Show Analytic and Debug Logs.
//    Applications and Services Logs / Microsoft / Windows / XAudio2. 
//    Right click on Microsoft Windows XAudio2 debug logging, Properties, then Enable Logging, and hit OK 
//XAUDIO2_DEBUG_CONFIGURATION debug = { 0 };
//debug.TraceMask = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
//debug.BreakMask = XAUDIO2_LOG_ERRORS;
//pXAudio2->SetDebugConfiguration(&debug, 0);
//#endif

/*
 Create a mastering voice

IXAudio2MasteringVoice* pMasteringVoice = nullptr;

if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice)))
{
	wprintf(L"Failed creating mastering voice: %#X\n", hr);
	SAFE_RELEASE(pXAudio2);
	CoUninitialize();
	return 0;
}


 Play a PCM wave file

wprintf(L"Playing mono WAV PCM file...");
if (FAILED(hr = PlayWave(pXAudio2, L"Media\\Wavs\\MusicMono.wav")))
{
	wprintf(L"Failed creating source voice: %#X\n", hr);
	SAFE_RELEASE(pXAudio2);
	CoUninitialize();
	return 0;
}


 Play an ADPCM wave file

wprintf(L"\nPlaying mono WAV ADPCM file (loops twice)...");
if (FAILED(hr = PlayWave(pXAudio2, L"Media\\Wavs\\MusicMono_adpcm.wav")))
{
	wprintf(L"Failed creating source voice: %#X\n", hr);
	SAFE_RELEASE(pXAudio2);
	CoUninitialize();
	return 0;
}


 Play a 5.1 PCM wave extensible file

wprintf(L"\nPlaying 5.1 WAV PCM file...");
if (FAILED(hr = PlayWave(pXAudio2, L"Media\\Wavs\\MusicSurround.wav")))
{
	wprintf(L"Failed creating source voice: %#X\n", hr);
	SAFE_RELEASE(pXAudio2);
	CoUninitialize();
	return 0;
}
*/
//#if (_WIN32_WINNT < 0x0602 /*_WIN32_WINNT_WIN8*/)

/*
 Play a mono xWMA wave file


wprintf(L"\nPlaying mono xWMA file...");
if (FAILED(hr = PlayWave(pXAudio2, L"Media\\Wavs\\MusicMono_xwma.wav")))
{
	wprintf(L"Failed creating source voice: %#X\n", hr);
	SAFE_RELEASE(pXAudio2);
	CoUninitialize();
	return 0;
}


 Play a 5.1 xWMA wave file

wprintf(L"\nPlaying 5.1 xWMA file...");
if (FAILED(hr = PlayWave(pXAudio2, L"Media\\Wavs\\MusicSurround_xwma.wav")))
{
	wprintf(L"Failed creating source voice: %#X\n", hr);
	SAFE_RELEASE(pXAudio2);
	CoUninitialize();
	return 0;
}

#endif


 Cleanup XAudio2

wprintf(L"\nFinished playing\n");

// All XAudio2 interfaces are released when the engine is destroyed, but being tidy
pMasteringVoice->DestroyVoice();

SAFE_RELEASE(pXAudio2);
CoUninitialize();
*/
