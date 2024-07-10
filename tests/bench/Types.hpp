#pragma once
#include <jsonifier/Index.hpp>
#include <filesystem>
#include <fstream>

class file_loader {
  public:
	file_loader(const std::string& filePathNew) {
		filePath = filePathNew;
		std::string directory{ filePathNew.substr(0, filePathNew.find_last_of("/") + 1) };
		if (!std::filesystem::exists(directory.operator std::basic_string_view<char, std::char_traits<char>>())) {
			std::filesystem::create_directories(directory.operator std::basic_string_view<char, std::char_traits<char>>());
		}

		if (!std::filesystem::exists(filePath.operator std::basic_string_view<char, std::char_traits<char>>())) {
			std::ofstream createFile(filePath.data());
			createFile.close();
		}

		std::ifstream theStream(filePath.data(), std::ios::binary | std::ios::in);
		std::stringstream inputStream{};
		inputStream << theStream.rdbuf();
		fileContents = inputStream.str();
		theStream.close();
	}

	void saveFile(const std::string& fileToSave) {
		std::ofstream theStream(filePath.data(), std::ios::binary | std::ios::out | std::ios::trunc);
		theStream.write(fileToSave.data(), static_cast<int64_t>(fileToSave.size()));
		theStream.close();
	}

	operator std::string&() {
		return fileContents;
	}

  protected:
	std::string fileContents{};
	std::string filePath{};
};

struct benchmark_result_final_parse {
	double medianAbsolutePercentageError{};
	jsonifier::string benchmarkColor{};
	jsonifier::string benchmarkName{};
	jsonifier::string libraryName{};
	jsonifier::string units{};
	uint64_t iterationCount{};
	double resultTime{};
};

struct benchmark_suite_results {
	inline benchmark_suite_results() noexcept = default;
	inline benchmark_suite_results(auto& values) {
		for (uint64_t x = 0; x < values.currentCount; ++x) {
			results.emplace_back(values.results[x]);
		}
		benchmarkSuiteName = values.benchmarkSuiteName;
	}
	jsonifier::vector<benchmark_result_final_parse> results{};
	jsonifier::string benchmarkSuiteName{};
};

namespace jsonifier {

	template<> struct core<benchmark_result_final_parse> {
		using value_type				 = benchmark_result_final_parse;
		static constexpr auto parseValue = createValue<&value_type::benchmarkName, &value_type::units, &value_type::medianAbsolutePercentageError, &value_type::resultTime,
			&value_type::benchmarkColor, &value_type::iterationCount, &value_type::libraryName>();
	};

	template<> struct core<benchmark_suite_results> {
		using value_type				 = benchmark_suite_results;
		static constexpr auto parseValue = createValue<&value_type::results, &value_type::benchmarkSuiteName>();
	};
}