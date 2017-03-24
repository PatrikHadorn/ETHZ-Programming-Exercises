#include <iostream>
#include <cstring>
#include <cassert>
#include <cmath>

// TODO: Code Deduplication by adding find(...) method used by insert and erase
// TODO: Overhead of structure updates is very large, should add functionality 
//       update once after structure has been filled with data

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
			std::cout << "Added initial node" << std::endl;
			update();
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
				{
					curNode = curNode->nodes[curHeight];
					nextNode = curNode->nodes[curHeight];
				}
				--curHeight;
			}

			//reached base level
			SkipListNode* nextNode = curNode->nodes[0];
			while (nextNode && nextNode->value < val)
			{
				curNode = nextNode;
				nextNode = curNode->nodes[0];
			}

			insert(val, curNode);
			update();
		}
	}

	// Remove ALL occurences of 'val'
	void erase(T const& val)
	{
		size_t curHeight = MaxHeight-1;
		while (!m_entryPoints[curHeight] && curHeight != 0)
			--curHeight;

		if (!m_entryPoints[curHeight])
		{
			// we're empty so trivially returning
			return;
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
				{
					curNode = curNode->nodes[curHeight];
					nextNode = curNode->nodes[curHeight];
				}
				--curHeight;
			}

			//reached base level
			SkipListNode* nextNode = curNode->nodes[0];
			while (nextNode && nextNode->value < val)
			{
				curNode = nextNode;
				nextNode = curNode->nodes[0];
				update();
			}

			if (nextNode && nextNode->value == val)
			{
				// we have matches
				while (nextNode && nextNode->value == val)
					nextNode = nextNode->nodes[0];

				SkipListNode* tmp = curNode->nodes[0];
				while (tmp != nextNode)
				{
					SkipListNode* nextTmp = tmp->nodes[0];
					delete tmp;
					tmp = nextTmp;
				}
				curNode->nodes[0] = nextNode;
				update();
			}
			else
			{
				// no matches, trivially returning;
				return;
			}
		}
	}

private:
	size_t const m_maxHeight = MaxHeight;
	SkipListNode* m_entryPoints[MaxHeight];

	// Insert value after node
	void insert(T const& val, SkipListNode* node)
	{
		SkipListNode* newNode = createNode(val);
		newNode->nodes[0] = node->nodes[0];
		node->nodes[0] = newNode;
	}

	// Creates a new node of random height
	SkipListNode* createNode(T const& val)
	{
		size_t numNodes = std::max(1UL, rand() % MaxHeight);
		SkipListNode* newNode = new SkipListNode(numNodes, val);
		std::memset(newNode->nodes, 0, sizeof(SkipListNode*)*newNode->numNodes);

		return newNode;
	}

	// update SkipList structure
	void update(void)
	{
		// Update entry points first
		SkipListNode* curNode = m_entryPoints[0];
		std::memset(m_entryPoints, 0, sizeof(SkipListNode*)*MaxHeight);

		size_t targetHeight = 0;
		while (curNode && targetHeight < MaxHeight)
		{
			if (curNode->numNodes >= targetHeight)
			{
				for (size_t i=targetHeight; i<curNode->numNodes; ++i)
					m_entryPoints[i] = curNode;
				targetHeight = curNode->numNodes+1;
			}

			curNode = curNode->nodes[0];
		}

		SkipListNode* ptrs[MaxHeight];
		std::copy(m_entryPoints, m_entryPoints+MaxHeight, ptrs);
		curNode = m_entryPoints[0];
		while (curNode)
		{
			size_t numNodes = curNode->numNodes;
			for (size_t i=0; i<numNodes; ++i)
				if (ptrs[i] && ptrs[i] != curNode)
					ptrs[i]->nodes[i] = curNode;

			for (size_t i=0; i<numNodes; ++i)
				ptrs[i] = curNode;

			curNode = curNode->nodes[0];
		}
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
	//srand(time(0));

	SkipList<int> testList;

	std::cout << "Initial SkipList: " << testList << std::endl;
	for (int i=0; i<1000; ++i)
	{
		int rnd = rand() % 1000;
		testList.insert(rnd);
	}

	for (int i=0; i<1000; ++i)
	{
		int rnd = rand() % 1000;
		testList.erase(rnd);
	}

	std::cout << "Final SkipList: " << testList << std::endl;

	return 0;
}

