#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <future>
#include <numeric>

// is num a prime number?
bool IsPrime(int num)
{	
	if(num < 2)  return false;
	if(num == 2) return true;
	
	if(num % 2 == 0)
		return false;
	else
		for (int i = 3; i < num / 3; i += 2)
			if (num % i == 0)
				return false;
	
	return true;
}

std::string ToString(int val)
{
    std::stringstream stream;
    stream << val;
    return stream.str();
}

// the given hash function for a character, the parameter is the ASCII value
int Hash(const unsigned int c)
{
	unsigned int hash_code = 0x666;
	(c % 2 == 1) ? hash_code <<= 11 : hash_code <<= 6;
	hash_code ^= (c & 0xff);
	IsPrime(hash_code) ? (hash_code |= 0x12345678) : (hash_code &= 0x12345678);
	
	return hash_code;
}

// a word's hash value
int WordHash(const std::string &str)
{
	unsigned int hash_value = 0;
	for(auto &c: str) hash_value += Hash(c);
	return hash_value;
}

// a line's hash value
std::string LineHash(const std::vector<std::string> &v)
{
	return std::accumulate(v.begin(), v.end(),
							ToString(WordHash(v[0])),
							[](std::string a, std::string b)
							{
								return a + ' ' + ToString(WordHash(b));
							});
}

int main()
{
	std::ifstream f("input.txt");
	unsigned int N;
	f >> N;

	std::vector<std::vector<std::string>> data(N);
	std::string input_line;

	getline(f, input_line);

	//reading the input file
	for (unsigned int i = 0; i < N; i++) {
		getline(f, input_line);
		std::stringstream curr_line(input_line);
		std::vector<std::string> tmp;
		std::string str;

		while (curr_line >> str) tmp.push_back(str);
		data[i] = tmp;
	}
	f.close();
	
	// Start calculating hash value for every vector.
	std::vector<std::future<std::string>> results(N);
	
	auto start = std::chrono::steady_clock::now();
	
	for (size_t i = 0; i < N; i++)
		results[i] = std::async(std::launch::async, LineHash, data[i]);

	// Wait for the function to return, write the result into a file
	std::ofstream output("output.txt");
	for (auto& f : results)
		output << f.get() << std::endl;
	
	auto end = std::chrono::steady_clock::now();
	auto diff = end - start;
	std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
	
	output.close();

    return 0;
}

