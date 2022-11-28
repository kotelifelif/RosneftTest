#include <vector>
#include <array>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <cmath>

using Column = std::vector<double>;
using Array = std::array<Column, 3>;

auto read_from_csv(const std::string& filename) -> Array {
	auto r = Array{};

	/// TODO: implementation of the reading ...
	std::ifstream input_file_stream(filename);
	std::string row;
	while (!input_file_stream.eof()) {
		input_file_stream >> row;
		std::stringstream input_string_stream(row);
		std::string value;
		auto index = 0;
		while (std::getline(input_string_stream, value, ',')) {
			r[index].push_back(stod(value));
			++index;
		}
	}

	return r;
}

auto calculate_coordinate(const size_t last_index, const size_t current_index,
	const std::vector<double>& md, const std::vector<double>& incl, const std::vector<double>& azim)->std::array<double, 3> {
	const auto radians_pi = std::acos(-1);
	const auto degrees_pi = 180.0;
	const auto degrees_to_radians_coefficient = radians_pi / degrees_pi;

	const auto first_incl = degrees_to_radians_coefficient * incl[last_index];
	const auto second_incl = degrees_to_radians_coefficient * incl[current_index];
	const auto first_azim = degrees_to_radians_coefficient * azim[last_index];
	const auto second_azim = degrees_to_radians_coefficient * azim[current_index];
	const auto delta_md = 0.5 * (md[current_index] - md[last_index]);

	const auto betta = acos(cos(second_incl - first_incl) - sin(first_incl) * sin(second_incl) * (1 - cos(second_azim - first_azim)));
	auto rf = 1.0;
	if (betta != 0)
		rf = 2 * tan(betta / 2) / (betta);
	const auto delta_x = delta_md * (sin(first_incl) * sin(first_azim) + sin(second_incl) * sin(second_azim)) * rf;
	const auto delta_y = delta_md * (sin(first_incl) * cos(first_azim) + sin(second_incl) * cos(second_azim)) * rf;
	const auto delta_z = delta_md * (cos(first_incl) + cos(second_incl)) * rf;

	std::array<double, 3> coordinate{ delta_x, delta_y, delta_z };
	return coordinate;
}

auto records_to_polyline(const Array& records) -> Array {
	auto& md = records[0]; /// в метрах
	auto& incl = records[1]; /// в градусах
	auto& azim = records[2]; /// в градусах
	auto r = Array{};

	/// TODO: implementation of the conversion...
	auto coordinate = calculate_coordinate(0, 0, md, incl, azim);
	r[0].push_back(coordinate[0]);
	r[1].push_back(coordinate[1]);
	r[2].push_back(coordinate[2]);
	for (auto i = 1; i < md.size(); ++i) {
		coordinate = calculate_coordinate(i - 1, i, md, incl, azim);
		r[0].push_back(r[0][i - 1] + coordinate[0]);
		r[1].push_back(r[1][i - 1] + coordinate[1]);
		r[2].push_back(r[2][i - 1] + coordinate[2]);
	}

	return r;
}

auto compare(const Array& lhs, const Array& rhs) {
	for (auto i = 0; i < lhs.size(); ++i) {
		auto c1 = lhs[i];
		auto c2 = rhs[i];
		assert(c1.size() == c2.size());
		assert(
			std::equal(c1.begin(), c1.end(), c2.begin(), [](auto v1, auto v2) {
				return std::abs(abs(v1) - abs(v2)) < 10e-6;
				})
		);
	}
}

int main(int argc, char* argv[]) {
	// файл исходных данных (md, incl, azim), разделитель - запятая
	auto records_filename = "records.csv";
	// файл исходных данных, пересчитанных в (x, y, z), разделитель - запятая
	auto polyline_filename = "polyline.csv";

	auto records = read_from_csv(records_filename);
	auto original_polyline = read_from_csv(polyline_filename);

	auto polyline = records_to_polyline(records);

	compare(polyline, original_polyline);

	return 0;
}