#pragma once

#include <cstdint>
#include <cmath>
#include <numbers>
#include <algorithm>
#include <limits>

namespace YOBA {
	class EMAFilter {
		public:
			constexpr static uint16_t maxUint16Factor = std::numeric_limits<uint16_t>::max();

			static float getDeltaTimeSFactor(const float factorPerSecond, const float deltaTimeS) {
				return factorPerSecond * deltaTimeS;
			}

			static float getDeltaTimeUsFactor(const float factorPerSecond, const uint32_t deltaTimeUs) {
				return factorPerSecond * static_cast<float>(deltaTimeUs) / 1'000'000.f;
			}

			static float apply(const float oldValue, const float newValue, const float factor) {
				return oldValue * (1.f - factor) + newValue * factor;
			}

			static float apply(const uint32_t oldValue, const uint32_t newValue, const uint16_t factor) {
				return (oldValue * (maxUint16Factor - factor) + newValue * factor) / maxUint16Factor;
			}

			static float applyToAngle(float oldValue, const float newValue, const float factor) {
				auto delta = newValue - oldValue;
				
				// -170 to 170
				// delta = 170 - -170 = 340
				if (delta > std::numbers::pi_v<float>) {
					// delta = 340 - 360 = -20
					delta -= 2.0f * std::numbers::pi_v<float>;
				}
				else if (delta < -std::numbers::pi_v<float>) {
					delta += 2.0f * std::numbers::pi_v<float>;
				}
				
				// oldValue = 170 - -20 = 190;
				oldValue = newValue - delta;
				oldValue = apply(oldValue, newValue, factor);
				
				if (oldValue > std::numbers::pi_v<float>) {
					oldValue -= 2.0f * std::numbers::pi_v<float>;
				}
				else if (oldValue < -std::numbers::pi_v<float>) {
					oldValue += 2.0f * std::numbers::pi_v<float>;
				}
				
				return oldValue;
			}
	};
}
