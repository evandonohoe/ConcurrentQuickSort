#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_COLOUR_NONE
#include "catch.hpp"

#include <string>
#include <sstream>

#include "cqsort.hpp"


template <typename T>
bool is_sorted(const T& vec) {
	for (auto i = 1; i < vec.size(); ++i) {
		if (vec[i - 1] > vec[i]) {
			return false;
		}
	}
	return true;
}

TEST_CASE("Sequential sort of a vector of integers", "[cqsort]") {

	std::vector<int> v = { 2, 4, 1, 3 };

	CQSort<std::vector<int>> cqsort(0);

	cqsort(v);

	REQUIRE(is_sorted(v) == true);
}

TEST_CASE("Sequential sort of a vector of strings of varying length", "[cqsort]") {

	std::vector<std::string> v = { "dasd", "afas", "casdf", "basdf", "daasdfsd", "afssas", "cassdf", "bdafdsasdf" };

	CQSort<std::vector<std::string>> cqsort(0);

	cqsort(v);

	REQUIRE(is_sorted(v) == true);
}

TEST_CASE("Sequential sort of a vector of chars", "[cqsort]") {

	std::vector<char> v = { 'c', 'a', 't', '.'};

	CQSort<std::vector<char>> cqsort(0);

	cqsort(v);

	REQUIRE(is_sorted(v) == true);
}

TEST_CASE("concurrent sort of vectors of integers, strings, chars", "[cqsort]") {

	for (int n = 0; n <= 8; n++) {
		std::vector<char> v1 = { 'c', 'a', 't', '.' };
		std::vector<std::string> v2 = { "dasd", "afas", "casdf", "basdf", "daasdfsd", "afssas", "cassdf", "bdafdsasdf" };
		std::vector<int> v3 = { 2, 4, 1, 3 };

		CQSort<std::vector<char>> cqsort1(n);
		CQSort<std::vector<std::string>> cqsort2(n);
		CQSort<std::vector<int>> cqsort3(n);

		cqsort1(v1);
		cqsort2(v2);
		cqsort3(v3);


		REQUIRE(is_sorted(v1) == true);
		REQUIRE(is_sorted(v2) == true);
		REQUIRE(is_sorted(v3) == true);
	}


}

