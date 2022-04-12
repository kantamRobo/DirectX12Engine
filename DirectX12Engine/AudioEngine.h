#pragma once
#include "pch.h"
class AudioEngine
{
public:
	AudioEngine();

	std::unique_ptr<DirectX::AudioEngine> m_audEngine;
};

