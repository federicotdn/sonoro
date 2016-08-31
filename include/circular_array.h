#ifndef CIRCULAR_ARRAY_H
#define CIRCULAR_ARRAY_H

namespace so
{
	template<typename T> class CircularArray
	{
	public:
		CircularArray(T *buffer, size_t capacity) :
			m_cursor(0),
			m_buffer(buffer),
			m_capacity(capacity),
			m_count(0)
		{ }
		
		T *m_buffer;

		void addValue(T val)
		{
			if ((size_t)m_count < m_capacity)
			{
				m_count++;
			}

			m_buffer[m_cursor++] = val;
			if (m_cursor >= m_capacity)
			{
				m_cursor = 0;
			}
		}

		int getCount() { return m_count; }

	private:
		size_t m_capacity;
		size_t m_cursor;
		int m_count;
	};
}

#endif /* CIRCULAR_ARRAY_H */