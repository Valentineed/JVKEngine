#pragma once

//FNV1Hash https://blog.molecular-matters.com/2011/06/24/hashed-strings/
class HashString
{
public:
	static constexpr size_t Hash(const char* cstr)
	{
        size_t hash = m_offset;

        while (*cstr != '\0')
        {
            hash ^= *cstr++;
            hash *= m_prime;
        }
        return hash;
	}
private:
    static const size_t m_offset = 2166136261u;
    static const size_t m_prime = 16777619u;
};