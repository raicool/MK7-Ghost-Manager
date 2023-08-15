#pragma once 

#define CLOCK std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

struct frame
{
	int32_t before = 0; //< time before frame start
	int32_t lag    = 0; //< frame lag
	int32_t delta  = 0; //< frame delta

	void apply_delay(uint32_t fr)
	{
		if (before != 0)
		{
			delta = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - before;

			if (delta <= fr) // if process is ahead of framerate
			{
				lag = 0;
				delta = (delta - fr) * -1;

				Sleep(delta);
			}
			else // if process is behind or lagging
			{
				lag = delta - fr;
			}
		}
	}
};