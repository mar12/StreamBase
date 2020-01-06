/**
 * Raw data container
 *
 * Implemented internally with `std::vector`, allows the storage of raw data
 * Unlike `std::vector` has more restricted interface and can be serialized
 * to `Utility::OutputBuffer` and deserialized from `Utility::InputBuffer`.
 */
#ifndef UTILITY_RAWDATA_H_
#define UTILITY_RAWDATA_H_
#include <vector>
#include <string>

namespace Utility {

class RawData {
public:
	RawData() : m_data() {}

	RawData(std::size_t length, const char* data) :
		m_data(data, data + length) {
	}

	template <typename T> static RawData fromValue(const T& value);

	template <typename T> bool toValue(T& value) const;

	const char* data() const noexcept {
		return m_data.data();
	}

	std::size_t size() const noexcept {
		return m_data.size();
	}


private:
	std::vector<char> m_data;

};

/// Conversions for `int`
template<> RawData RawData::fromValue<int>(const int& value);
template<> bool RawData::toValue<int>(int& value) const;

/// Conversions for `std::string`
template<> RawData RawData::fromValue<std::string>(const std::string& value);
template<> bool RawData::toValue<std::string>(std::string& value) const;

}

#endif // UTILITY_RAWDATA_H_