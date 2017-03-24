#include <iostream>
#include <cstring>
#include <cassert>

template<class T,size_t MaxHeight=16> class SkipList
{
public:
	struct SkipListNode;

	class SkipListIter
	{
   public:
	   SkipListIter(SkipListNode* node) : m_curNode(node) {}

	   bool operator!=(SkipListIter const& rhs) 
	   {
		   return m_curNode != rhs.m_curNode;
	   }

	   SkipListIter* operator++(void) 
	   {
		   m_curNode = m_curNode->nodes[0];
		   return this;
	   }

	   SkipListNode& operator*(void) {return *m_curNode;}

   private:
	   SkipListNode* m_curNode = nullptr;
	};

	SkipListIter const begin() const {return SkipListIter(m_entryPoints[0]);}
	SkipListIter const end() const {return SkipListIter(nullptr);}

	struct SkipListNode
	{
		SkipListNode(size_t numNodes, T const& val) : 
			numNodes(numNodes), value(val)
		{
			nodes = new SkipListNode*[numNodes];
		}

		~SkipListNode()
		{
			if (nodes)
				delete [] nodes;
		}

		size_t const numNodes;
		SkipListNode** nodes = nullptr;
		T const value;
	};

	SkipList()
	{
		// zero-initialize entry-points
		std::memset(m_entryPoints, 0, sizeof(SkipListNode*)*MaxHeight);
	}

	// Insert value
	void insert(T const& val)
	{
		size_t curHeight = MaxHeight-1;
		while (!m_entryPoints[curHeight] && curHeight != 0)
			--curHeight;

		if (!m_entryPoints[curHeight])
		{
			// first entry into list. Create new node and connect bottom layer
			SkipListNode* newNode = createNode(val);
			m_entryPoints[0] = newNode;
		}
		else
		{
			SkipListNode* curNode = m_entryPoints[curHeight];

			while (curHeight != 0)
			{
				assert(curNode->numNodes >= curHeight);

				SkipListNode* nextNode = 
					curNode->nodes[curHeight];
				while (nextNode && nextNode->value < val)
					curNode = curNode->nodes[curHeight];
				--curHeight;
			}

			//reached base level
			SkipListNode* nextNode = 
				curNode->nodes[0];
			while (nextNode && nextNode->value < val)
				curNode = nextNode;

			
		}
	}

	// Remove ALL occurences of 'val'
	void erase(T const& val);

private:
	size_t const m_maxHeight = MaxHeight;
	SkipListNode* m_entryPoints[MaxHeight];

	// Insert value after node
	void insert(T const& val, SkipListNode const& node)
	{
	}

	// Creates a new node of random height
	SkipListNode* createNode(T const& val)
	{
		SkipListNode* newNode = new SkipListNode(rand() % MaxHeight, val);
		std::memset(newNode->nodes, 0, sizeof(SkipListNode*)*newNode->numNodes);

		return newNode;
	}
};

template<class T, size_t MaxHeight> 
std::ostream& operator<<(std::ostream& os, SkipList<T,MaxHeight> const& skipList)
{
	os << "{";
	for (auto iter = skipList.begin(); iter != skipList.end(); ++iter)
		os << (*iter).value << ", ";
	os << "}";
	return os;
}

int main(int argc, char* argv[])
{
	SkipList<int> testList;

	std::cout << "Initial SkipList: " << testList << std::endl;
	for (int i=0; i<5; ++i)
		testList.insert(i);
	std::cout << "Final SkipList: " << testList << std::endl;

	return 0;
}

