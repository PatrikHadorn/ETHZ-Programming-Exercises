#include <iostream>

template<typename T> class binary_tree
{
public:
	binary_tree(){}

	void insert(T const& v)
	{
		entry* e = find(v);
		if (e == nullptr)
		{
			m_root = new entry{v, nullptr, nullptr};
			return;
		}
		
		// Already member of tree?
		if (e->v == v)
			return;

		entry* t = new entry{v, nullptr, nullptr};
		if (e->v < v)
			e->left = t;
		else
			e->right = t;
	}

	//TODO: 
	void remove(T const& v);

	bool lookup(T const& v)
	{
		entry* e = find(v);
		if (e == nullptr)
			return false;
		else if (e->v != v)
			return false;
		else 
			return true;
	}

private:
	struct entry
	{
		T v;
		entry *left = nullptr, *right = nullptr;
	} *m_root = nullptr;

	entry* find(T const& v)
	{
		entry* cur_entry = m_root;
		while (cur_entry != nullptr)
		{
			if (cur_entry->v < v)
			{
				if (cur_entry->left == nullptr)
					return cur_entry;
				cur_entry = cur_entry->left;
			}
			else if (cur_entry->v > v)
			{
				if (cur_entry->right == nullptr)
					return cur_entry;

				cur_entry = cur_entry->right;
			}
			else
				return cur_entry;
		}
		return cur_entry;
	}

};

int main(int argc, char* argv[])
{
	size_t const num_entries = 100000;
	size_t const num_lookups = 10000;

	binary_tree<int> btree;
	for (int i=0; i<num_entries; ++i)
		btree.insert(i);

	clock_t start = clock();

	volatile bool result;
	for (int i=0; i<num_lookups; ++i)
	{
		int rnd = rand()%num_entries;
		result = btree.lookup(rnd);
	}
	clock_t end = clock();
	double s = static_cast<double>(end-start)/CLOCKS_PER_SEC;
	std::cout << "Elapsed: " << s << "s" << std::endl;
	std::cout << "Time Per Lookup: " << s*1e9/num_lookups << "ns" << std::endl;

	return 0;
}

