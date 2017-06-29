#include <iostream>
#include <cstring>
#include <vector>
#include <exception>

template<typename T, size_t N> class hash
{
public:
	T operator()(T key, int j) const
	{
		return ((key%N) - j*((1+key)%(N-2)))%N;
	}
};

template<typename T, size_t N> class hash_map
{
public:
	hash_map()
	{
		clear();
	}

	void insert(T const& key)
	{
		for (int j=0; j<N; ++j)
		{
			size_t i = m_hash(key, j);
			if (m_deleted[i])
			{
				m_deleted[i] = false;
				m_map[i] = key;
				++m_num_entries;
				return;
			}
		}

		// TODO: we're full
		throw(std::runtime_error("Container full"));
	}

	T* find(T const& key)
	{
		for (int j=0; j<N; ++j)
		{
			size_t const i = m_hash(key, j);
			if (m_deleted[i])
				return nullptr;
			else if (m_map[i] == key)
				return &m_map[i];
		}
		return nullptr;
	}

	void clear(void)
	{
		m_num_entries = 0;
		std::memset(m_map, 0, sizeof(T)*N);
		std::memset(m_deleted, true, sizeof(bool)*N);
	}

private:
	size_t m_num_entries = 0;
	T m_map[N];
	bool m_deleted[N];
	hash<T,N> m_hash;
};

int main(int argc, char* argv[])
try {
	size_t const hash_map_entries 	= 100000;
	size_t const hash_map_size 		= 104743; // needs to be prime!
	hash_map<int, hash_map_size> hash_map;

	for (int i=0; i<hash_map_entries; ++i)
		hash_map.insert(rand());

	clock_t start = clock();
	int* volatile e;
	size_t const num_lookups = 100000;
	for (int i=0; i<num_lookups; ++i)
	{
		 e = hash_map.find(rand());
	}
	clock_t end = clock();

	double ms = static_cast<double>(end-start)/CLOCKS_PER_SEC*1e3;
	std::cout << "Time: " << ms << "ms" << std::endl;
	std::cout << "Per Lookup: " << 1e6*ms/num_lookups << "ns" << std::endl;

	//hash_map.print();
	hash_map.clear();
	//hash_map.print();

	return 0;
}
catch (std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return 1;
}

