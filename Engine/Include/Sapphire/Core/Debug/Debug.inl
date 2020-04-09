// Copyright 2020 Sapphire development team. All Rights Reserved.

namespace Sa
{
#if SA_ASSERTION

	template <ExceptCode code>
	void Debug::Assert(const Exception<code>& _exception)
	{
		// Assertion disabled on channel.
		if (!_exception.channel.enableAssert)
			return;

		if (_exception.level == LogLvlFlag::AssertFailed)
		{
			// Force channel and level.
			Log_Internal(_exception);

			throw _exception;
		}
		else
			Log(_exception);
	}

#endif
}
