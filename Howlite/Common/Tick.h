#pragma once

namespace Howlite {

	class HTick {
	public:
		HTick(float Time = 0.0f);
		~HTick() = default;

		/**
		 * Get Seconds
		 * @return 
		 */
		float GetSeconds() const noexcept;

		/**
		 * Get Milliseconds
		 * @return 
		 */
		float GetMilliseconds() const noexcept;

	private:
		float mTime;
	};

}
