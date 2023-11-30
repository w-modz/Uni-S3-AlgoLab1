// ALGO2 IS1 221A LAB01
// Wiktor Modzelewski
// mw53766@zut.edu.pl
#include <iostream>
#include <random>
#include <ctime>
#include <string>
#include <sstream>

std::default_random_engine gen(time(nullptr));
std::uniform_int_distribution<int> dist(-5, 5);

template<typename temp>
class linked_list
{
public:
	struct node
	{
		temp data;
		node* next;
		node* prev;

		node(const temp& data, node* next, node* prev)
			: data(data), next(next), prev(prev)
		{}

		node(temp&& data, node* next, node* prev)
			: data(data), next(next), prev(prev)
		{}

		~node()
		{
			if (next != nullptr)
			{
				next = nullptr;
			}
			if (prev != nullptr)
			{
				prev = nullptr;
			}
		}
	};

	linked_list() noexcept
		: head(nullptr), tail(nullptr), lenght(0)
	{}

	~linked_list()
	{
		clear();
	}

	void clear()
	{
		while (head != nullptr) delete_head();
	}

	unsigned int get_lenght() const noexcept
	{
		return lenght;
	}
	
	node* get_head() const noexcept
	{
		return head;
	}

	node* get_tail() const noexcept
	{
		return tail;
	}

	void add(const temp& data)
	{
		auto new_head = new node(data, head, nullptr);
		if (head)
		{
			head->prev = new_head;
		}
		else
		{
			tail = new_head;
		}
		head = new_head;
		lenght++;
	}

	void append(const temp& data)
	{
		auto new_tail = new node(data, nullptr, tail);
		if (tail != nullptr)
		{
			tail->next = new_tail;
		}
		else
		{
			head = new_tail;
		}
		tail = new_tail;
		lenght++;
	}

	void delete_head()
	{
		if (lenght == 1)
		{
			delete head;
			head = nullptr;
			tail = nullptr;
		}
		else if (head != nullptr)
		{
			node* old_head = head;
			head = head->next;
			old_head->next = nullptr;
			head->prev = nullptr;
			delete old_head;
		}
		else
		{
			std::cout << "No head in list";
			return;
		}
		lenght--;
	}

	void delete_tail()
	{
		if (lenght == 1)
		{
			delete tail;
			head = nullptr;
			tail = nullptr;
		}
		else if (tail)
		{
			node* old_tail = tail;
			tail = tail->prev;
			old_tail->prev = nullptr;
			tail->next = nullptr;
			delete old_tail;
		}
		else
		{
			std::cout << "No tail in list";
			return;
		}
		lenght--;
	}
	template<typename Comp>
	node* find_element(temp element, Comp comp)
	{
		for (auto pointer = head; pointer != nullptr; pointer = pointer->next)
		{
			if (comp(element,pointer->data))
			{
				return pointer;
			}
		}
		return nullptr;
	}

	template<typename Comp>
	bool delete_element(temp element, Comp comp)
	{
		auto to_be_deleted = find_element(element, comp);
		if(!to_be_deleted)
		{
			return false;
		}
		else if (to_be_deleted == head)
		{
			delete_head();
		}
		else if (to_be_deleted == tail)
		{
			delete_tail();
		}
		else
		{
			to_be_deleted->next->prev = to_be_deleted->prev;
			to_be_deleted->prev->next = to_be_deleted->next;
			to_be_deleted->next = nullptr;
			delete to_be_deleted;
			lenght--;
		}
		return true;
	}
	// function that inserts a new element of given data after an element matching the given comperator
	template<typename Comp>
	void insert_by_value(temp element, Comp comp)
	{
		if (!head || comp(element,head->data))
		{
			add(element);
		}
		else if (comp(tail->data,element))
		{
			append(element);
		}
		else
		{
			node* current_el = head;
			while (comp(current_el->data, element))
			{
				current_el = current_el->next;
			}
			node* new_el = new node(element, current_el, current_el->prev);
			current_el->prev = new_el;
			new_el->prev->next = new_el;
			lenght++;
		}
	}

	temp get_value(const int index)
	{
		if (index >= lenght)
		{
			std::cerr << "Index out of scope";
			abort();
		}
		node* curret_element = head;
		for (int i = 0; i < index; i++)
		{
			curret_element = curret_element->next;
		}
		return curret_element->data;
	}

	void set_data(const int index, temp new_data)
	{
		if (index >= lenght)
		{
			std::cerr << "Index out of scope";
			abort();
		}
		node* curret_element = head;
		for (int i = 0; i < index; i++)
		{
			curret_element = curret_element->next;
		}
		curret_element->data = new_data;
	}
	// function that outputs a formated string of given number of list elements starting from the head
	std::string to_string(unsigned int number_to_print = 1) const
	{
		if (lenght == 0)
			return "[]";
		else if (lenght < number_to_print)
		{
			return "Error; Index out of scope";
		}
		std::ostringstream output_stream;
		output_stream << "[" << head->data;
		for (auto current_el = head->next; number_to_print>1; current_el = current_el->next)
		{
			output_stream << ", " << current_el->data;
			number_to_print--;
		}
		output_stream << "]\n";
		return output_stream.str();
	}

private:
	node* head;
	node* tail;
	unsigned int lenght;
};

struct data_obj
{
	int field1;
	float field2;

	data_obj(int data1, float data2)
		: field1(data1), field2(data2)
	{}
	bool operator== (const data_obj &element) const
	{
		return (this->field1 == element.field1 && this->field2 == element.field2);
	}
	bool operator< (const data_obj& element) const
	{
		return (this->field1 < element.field1 || field1 == element.field1 && this->field2 < element.field2);
	}
};

std::ostream& operator<< (std::ostream& out, const data_obj& obj)
{
	out << "(" << obj.field1 << ", " << obj.field2 << ")";
	return out;
}
// comparator for data_obj type
bool compare(const data_obj& data1, const data_obj& data2)
{
	return data1 == data2;
}

bool less(const data_obj& data1, const data_obj& data2)
{
	return data1 < data2;
}

int main()
{
	srand(time(NULL));
	const int MAX_ORDER = 6; // maximum order of data input (in powers of 10)
	linked_list <data_obj> *ll = new linked_list <data_obj>();
	for (int o = 1; o <= MAX_ORDER; o++) // loop for data orders of magnitude
	{
		const int n = pow(10, o); // data sample size
		// addition to list (via appending)
		clock_t t1 = clock();
		for (int i = 0; i < n; i++) {
			data_obj so((rand() % 100 + 1), (rand() % 100 + 1)); // random data for appending
				ll -> append(so);
		}
		clock_t t2 = clock();
		// display of number of elements added, calculation of total time taken and average time per element
		std::cout << "List lenght: " << ll->get_lenght() << "\n";
		std::cout << "Total time of appending = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 << "ms\n";
		std::cout << "Average time of appending = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 / n << "ms\n";
		// searching and deleting elements from list
			const int m = pow(10, 4); // number of searches
		t1 = clock();
		for (int i = 0; i < m; i++) {
			data_obj so((rand() % 100 + 1), (rand() % 100 + 1)); // random data for searching
				ll -> delete_element(so, compare);
		}
		t2 = clock();
		// display of number of elements searched (not deleted!), calculation of total time taken and average time per element
		std::cout << "Elements searched: " << m << "\n";
		std::cout << "Total time of deleting = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 << "ms\n";
		std::cout << "Average time of deleting = " << ((t2 - t1) / (double)CLOCKS_PER_SEC) * 1000 / m << "ms\n";
		ll -> clear(); // clearing of list and freeing memory
	}
	delete ll;

	return 0;
}