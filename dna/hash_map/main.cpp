#include <iostream>
#include <cstring>
#include <vector>

template<typename T, size_t N> class hash
{
public:
	hash() 
	{
		// prevent bad choices of a,b?
		a = rand()%N;
		b = rand()%N;
	}

	T operator() (T key) const
	{ 
		return ((a*key + b) % N); 
	}

private:
	T a, b;
};

template<typename T, size_t N> class hash_map
{
public:
	hash_map()
	{
		std::memset(m_map, 0, sizeof(hash_map_entry*)*N);
	}

	T* find(T key) const
	{
		hash_map_entry* e = m_map[m_hash(key)];
		if (e == nullptr)
			return nullptr;

		while (e != nullptr && e->m_entry != key)
			e = e->m_next;

		if (e == nullptr)
			return nullptr;
		else 
			return &e->m_entry;
	}

	void insert(T key)
	{
		// check if key is already in the map
		if (find(key) != nullptr)
			return;

		hash_map_entry* e = m_map[m_hash(key)];
		if (e == nullptr)
			m_map[m_hash(key)] = new hash_map_entry{key, nullptr};
		else
		{
			while (e->m_next != nullptr)
				e = e->m_next;
			e->m_next = new hash_map_entry{key, nullptr};
		}
	}

	void print(void) const
	{
		for (size_t i=0; i<N; ++i)
		{
			std::cout << i << ": {";
			hash_map_entry const* e = m_map[i];
			while (e != nullptr)
			{
				std::cout << e->m_entry;
				if (e->m_next != nullptr)
					std::cout << ",";
				e = e->m_next;
			}
			std::cout << "}\n";
		}
	}

	void clear(void)
	{
		for (size_t i=0; i<N; ++i)
		{
			hash_map_entry* e = m_map[i];
			while (e != nullptr)
			{
				hash_map_entry* tmp = e;
				e = e->m_next;
				delete tmp;
			}
			m_map[i] = nullptr;
		}
	}

private:
	struct hash_map_entry
	{
		T m_entry;
		hash_map_entry* m_next = nullptr;
	};

	hash_map_entry* m_map[N];
	hash<T,N> m_hash;
};

int main(int argc, char* argv[])
{
	size_t const hash_map_size = 1000000;
	size_t const hash_map_entries = 1000000;
	hash_map<int, hash_map_size> hash_map;

	for (int i=0; i<hash_map_entries; ++i)
		hash_map.insert(rand());

	clock_t start = clock();
	int* volatile e;
	for (int i=0; i<10000; ++i)
	{
		 e = hash_map.find(rand());
	}
	clock_t end = clock();

	double ms = static_cast<double>(end-start)/CLOCKS_PER_SEC*1e3;
	std::cout << "Time: " << ms << "ms" << std::endl;
	std::cout << "Per Lookup: " << 1e6*ms/10000 << "ns" << std::endl;

	//hash_map.print();
	hash_map.clear();
	//hash_map.print();

	return 0;
}

