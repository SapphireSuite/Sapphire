// Copyright 2020 Sapphire development team. All Rights Reserved.

#include <iostream>

#include <Core/Debug/Debug.hpp>

namespace Sa
{
#if SA_LOGGING || SA_ASSERTION

	std::vector<LogChannel> Debug::channels{ LogChannel(L"Default") };
	LogLvlFlags Debug::levelMask = LogLvlFlags(LogLvlFlag::Normal | LogLvlFlag::Infos | LogLvlFlag::Warning | LogLvlFlag::Error | LogLvlFlag::AssertFailed);

	const wchar* Debug::GetFileNameFromPath(const wchar* _filePath) noexcept
	{
		// Remove characters until last backslash.
		const wchar* fileName = wcsrchr(_filePath, '\\');

		if (!fileName) // No backslash found.
			fileName = _filePath;
		else
			fileName += 1; // Remove last '\\' found.


		// Remove characters until last slash.
		if (const wchar* filePathNoSlash = wcsrchr(fileName, '/'))
		{
			// Remove last '/' found.
			fileName = filePathNoSlash + 1;
		}

		return fileName;
	}

	LogChannel& Debug::GetChannel(const wchar* _name)
	{
		SA_ASSERT(_name, InvalidParam, Default, L"Param _name is nullptr!")

		for (auto it = channels.begin(); it != channels.end(); ++it)
		{
			if (wcscmp(it->name, _name) == 0)
				return *it;
		}

		SA_ASSERT(_name, InvalidParam, Default, L"No channel with name found!")

		return channels[0];
	}

	LogChannel& Debug::RegisterChannel(const wchar* _chan) noexcept
	{
		if (!_chan)
			return channels[0];

		return channels.emplace_back(_chan);
	}

	void Debug::UnRegisterChannel(const wchar* _chan) noexcept
	{
		if (!_chan)
			return;

		for (auto it = channels.begin(); it != channels.end(); ++it)
		{
			if (wcscmp(it->name, _chan) == 0)
			{
				channels.erase(it);
				return;
			}
		}
	}


	void Debug::Log_Internal(const Sa::Log& _log) noexcept
	{
#if SA_LOGGING

		std::wcout << _log << std::endl;

#endif
	}

	void Debug::Log(const Sa::Log& _log) noexcept
	{
#if SA_LOGGING

		if (levelMask.IsSet(_log.level) && _log.channel.enableLog)
			Log_Internal(_log);

#endif
	}

#endif
}
