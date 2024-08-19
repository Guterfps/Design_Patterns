/*-------------------------------------------*/
/* DEVELOPER: Itay Guter                    */
/* LAST MODIFIED: 16/5/2023	               */
/* WORKSHEET NAME: bit array      	      */
/* REVIEWER: Noy		                 */
/*--------------------------------------*/

#ifndef ILRD_RD136_7_BIT_ARRAY_HPP
#define ILRD_RD136_7_BIT_ARRAY_HPP

#include <cstddef> // size_t
#include <algorithm> // fill
#include <string> // string
#include <cstring> // memmove

namespace ilrd
{
static const unsigned char NUM_OF_BITS_IN_BYTE = 8;
static const unsigned int NUM_OF_VALUES_IN_BYTE = 1 << NUM_OF_BITS_IN_BYTE;

template <std::size_t SIZE>
class BitArray
{
private:
    class ProxyBit;

public:
    explicit BitArray();

    bool operator[](std::size_t index) const;
    ProxyBit operator[](std:: size_t index);

    bool operator==(const BitArray& other) const;
    bool operator!=(const BitArray& other) const;
    BitArray& operator|=(const BitArray& other);
    BitArray& operator&=(const BitArray& other);
    BitArray& operator^=(const BitArray& other);
    BitArray& operator>>=(std::size_t n);
    BitArray& operator<<=(std::size_t n);

    std::size_t Count() const;
    std::string ToString() const;
    bool Get(std::size_t index) const;
    BitArray& Set();
    BitArray& Set(std::size_t index, bool val = true);
    BitArray& Flip();
    BitArray& Flip(std::size_t index);

private:
    static const unsigned int WORD_SIZE = sizeof(std::size_t) * 
                                        NUM_OF_BITS_IN_BYTE;
    static const std::size_t ARR_SIZE = 
    ((SIZE + (WORD_SIZE - 1)) & ~(WORD_SIZE - 1)) / WORD_SIZE;
    static const std::size_t NUM_OF_EXTRA_BITS = ARR_SIZE * WORD_SIZE - SIZE;

    class ProxyBit
    {
    public:
        explicit ProxyBit(std::size_t& arr, std::size_t index);

        operator bool() const;
        ProxyBit& operator=(bool value);
        ProxyBit& operator=(ProxyBit& value);

    private:
        std::size_t& m_ref;
        std::size_t m_index;
    };

    class FunctorOr
    {
    public:
        std::size_t operator()(const std::size_t& arr, const std::size_t& other);
    };

    class FunctorAnd
    {
    public:
        std::size_t operator()(const std::size_t& arr, const std::size_t& other);
    };

    class FunctorXor
    {
    public:
        std::size_t operator()(const std::size_t& arr, const std::size_t& other);
    };

    class FunctorCount
    {
    public:
        FunctorCount();
        void operator()(const unsigned char& arr);
        std::size_t GetCount() const;
    
    private:
        std::size_t m_count;
    };

    class FunctorToString
    {
    public:
        FunctorToString();
        void operator()(const std::size_t& arr);
        std::string GetString() const;

    private:
        std::string m_str;
    };

    class FunctorSet
    {
    public:
        void operator()(std::size_t& arr);
    };

    class FunctorFlip
    {
    public:
        void operator()(std::size_t& arr);
    };

    class FunctorShiftRight
    {
    public:
        FunctorShiftRight(std::size_t shift);
        void operator()(std::size_t& arr);
    private:
        std::size_t m_temp1;
        std::size_t m_temp2;
        std::size_t m_shift;
    };

    class FunctorShiftLeft
    {
    public:
        FunctorShiftLeft(std::size_t shift);
        void operator()(std::size_t& arr);
    private:
        std::size_t m_temp1;
        std::size_t m_temp2;
        std::size_t m_shift;
    };

    std::size_t m_arr[ARR_SIZE];

    static std::size_t GetIndx(std::size_t index);
    static std::size_t GetBitIndx(std::size_t index);
    static void ZeroExtraBits(std::size_t& arr);
    void StringWithoutExtraBits(std::string *str) const;
    void ShiftMoreThenWordSize(std::size_t jumper);
};

static const unsigned char BitsSetTable[NUM_OF_VALUES_IN_BYTE] = 
{
    // macros to create the LUT for BitArrCountSetBitsLUT
    #   define B2(n) n,     n+1,     n+1,     n+2
    #   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
    #   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
    
    B6(0), B6(1), B6(1), B6(2)
};

template <std::size_t SIZE>
BitArray<SIZE>::BitArray()
{
    std::fill_n(m_arr, ARR_SIZE, std::size_t(0));
}

template <std::size_t SIZE>
typename BitArray<SIZE>::ProxyBit BitArray<SIZE>::operator[](std::size_t index)
{
    if (index >= SIZE)
    {
        throw std::out_of_range("index out of range");
    }
    
    return ProxyBit(m_arr[GetIndx(index)], GetBitIndx(index));
}

template <std::size_t SIZE>
bool BitArray<SIZE>::operator[](std::size_t index) const
{
    if (index >= SIZE)
    {
        throw std::out_of_range("index out of range");
    }

    return (m_arr[GetIndx(index)] & (std::size_t(1) << GetBitIndx(index)));
}

template <std::size_t SIZE>
bool BitArray<SIZE>::operator==(const BitArray& other) const
{
    return (std::equal(m_arr, m_arr + ARR_SIZE, other.m_arr));
}

template <std::size_t SIZE>
bool BitArray<SIZE>::operator!=(const BitArray& other) const
{
    return !(*this == other);
}

template <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator|=(const BitArray& other)
{
    std::transform(m_arr, m_arr + ARR_SIZE, other.m_arr, m_arr, FunctorOr());

    return *this;
}

template <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator&=(const BitArray& other)
{
    std::transform(m_arr, m_arr + ARR_SIZE, other.m_arr, m_arr, FunctorOr());

    return *this;
}

template <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator^=(const BitArray& other)
{
    std::transform(m_arr, m_arr + ARR_SIZE, other.m_arr, m_arr, FunctorXor());

    return *this;
}

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::Count() const
{
    const unsigned char *arr = reinterpret_cast<const unsigned char*>(m_arr);
    std::size_t size = ARR_SIZE * sizeof(std::size_t);
    
    return (std::for_each(arr, arr + size, FunctorCount()).GetCount());
}

template <std::size_t SIZE>
std::string BitArray<SIZE>::ToString() const
{
    std::string res = (std::for_each(m_arr, m_arr + ARR_SIZE - 1, 
                        FunctorToString()).GetString());
    StringWithoutExtraBits(&res);

    return res;
}

template <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::Set()
{
    std::for_each(m_arr, m_arr + ARR_SIZE, FunctorSet());
    ZeroExtraBits(m_arr[ARR_SIZE - 1]);

    return *this;
}

template <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::Set(std::size_t index, bool value)
{
    if (index >= SIZE)
    {
        throw  std::out_of_range("index out of range");
    }

    (*this)[index] = value;

    return *this;
}

template <std::size_t SIZE>
bool BitArray<SIZE>::Get(std::size_t index) const
{
    if (index >= SIZE)
    {
        throw  std::out_of_range("index out of range");
    }

    return ((*this)[index]);
}

template  <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::Flip()
{
    std::for_each(m_arr, m_arr + ARR_SIZE, FunctorFlip());
    ZeroExtraBits(m_arr[ARR_SIZE - 1]);

    return *this;
}

template <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::Flip(std::size_t index)
{
    if (index >= SIZE)
    {
        throw  std::out_of_range("index out of range");
    }

    (*this)[index] = !(*this)[index];

    return *this;
}

template <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator>>=(std::size_t shift)
{
    std::size_t jumper = (shift / WORD_SIZE) % ARR_SIZE;

    shift &= (WORD_SIZE - 1);
    std::reverse(m_arr, m_arr + ARR_SIZE);
    ShiftMoreThenWordSize(jumper);
    std::for_each(m_arr + jumper, m_arr + ARR_SIZE, FunctorShiftRight(shift));
    std::reverse(m_arr, m_arr + ARR_SIZE);
    
    return *this;
}

template <std::size_t SIZE>
BitArray<SIZE>& BitArray<SIZE>::operator<<=(std::size_t shift)
{
    std::size_t jumper = (shift / WORD_SIZE) % ARR_SIZE;

    shift &= (WORD_SIZE - 1);
    ShiftMoreThenWordSize(jumper);
    std::for_each(m_arr + jumper, m_arr + ARR_SIZE, FunctorShiftLeft(shift));
    ZeroExtraBits(m_arr[ARR_SIZE - 1]);

    return *this;
}

template <std::size_t SIZE>
void BitArray<SIZE>::ShiftMoreThenWordSize(std::size_t jumper)
{   
    if (jumper > 0)
    {
        std::memmove(m_arr + jumper, m_arr, 
                    (ARR_SIZE - jumper) * sizeof(std::size_t));
        std::fill(m_arr, m_arr + jumper, std::size_t(0));
    }
}

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::GetIndx(std::size_t index)
{
    if (index >= SIZE)
    {
        throw  std::out_of_range("index out of range");
    }

    return (index / WORD_SIZE);
}

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::GetBitIndx(std::size_t index)
{
    if (index >= SIZE)
    {
        throw  std::out_of_range("index out of range");
    }

    return (index & (WORD_SIZE - std::size_t(1)));
}

template <std::size_t SIZE>
void BitArray<SIZE>::ZeroExtraBits(std::size_t& arr)
{
    arr &= (~std::size_t(0) >> (NUM_OF_EXTRA_BITS));
}

template <std::size_t SIZE>
void BitArray<SIZE>::StringWithoutExtraBits(std::string *str) const
{
    for (std::size_t i = (ARR_SIZE - 1) * WORD_SIZE; 
        i < SIZE; ++i)
    {
        *str += (*this)[i] ? "1" : "0";
    }
}

// ProxyBit class
template <std::size_t SIZE>
BitArray<SIZE>::ProxyBit::ProxyBit(std::size_t& arr, std::size_t index) : 
m_ref(arr), m_index(index)
{}

template <std::size_t SIZE>
BitArray<SIZE>::ProxyBit::operator bool() const
{
    return (m_ref & (std::size_t(1) << m_index));
}

template <std::size_t SIZE>
typename BitArray<SIZE>::ProxyBit& 
BitArray<SIZE>::ProxyBit::operator=(bool value)
{
    m_ref = (m_ref & ~(std::size_t(1) << m_index)) | 
            (std::size_t(value) << m_index);
    
    return *this;
}

// FunctorOr class
template <std::size_t SIZE>
std::size_t BitArray<SIZE>::FunctorOr::operator()(const std::size_t&  arr ,
                                                    const std::size_t& other)
{
    return (arr | other);
}

// FunctorAnd class
template <std::size_t SIZE>
std::size_t BitArray<SIZE>::FunctorAnd::operator()(const std::size_t& arr,  
                                                    const std::size_t& other)
{
    return (arr & other);
}

// FunctorXor class
template <std::size_t SIZE>
std::size_t BitArray<SIZE>::FunctorXor::operator()(const std::size_t& arr,
                                                    const std::size_t& other)
{
    return (arr ^ other);
}

// FunctorCount class
template <std::size_t SIZE>
BitArray<SIZE>::FunctorCount::FunctorCount() : m_count(0)
{}

template <std::size_t SIZE>
void BitArray<SIZE>::FunctorCount::operator()(const unsigned char& arr)
{
    m_count += (BitsSetTable[arr]);
}

template <std::size_t SIZE>
std::size_t BitArray<SIZE>::FunctorCount::GetCount() const
{
    return m_count;
}

// FunctorToString class
template <std::size_t SIZE>
BitArray<SIZE>::FunctorToString::FunctorToString() : m_str("")
{}

template <std::size_t SIZE>
void BitArray<SIZE>::FunctorToString::operator()(const std::size_t& arr)
{
    for (std::size_t i = 0; i < BitArray<SIZE>::WORD_SIZE; ++i)
    {
        m_str += (std::size_t(1) & (arr >> i)) ? "1" : "0";
    }
}

template <std::size_t SIZE>
std::string BitArray<SIZE>::FunctorToString::GetString() const
{
    return m_str;
}

// FunctorSet class
template <std::size_t SIZE>
void BitArray<SIZE>::FunctorSet::operator()(std::size_t& arr)
{
    arr = ~(std::size_t(0));
}

// FunctorFlip class
template <std::size_t SIZE>
void BitArray<SIZE>::FunctorFlip::operator()(std::size_t& arr)
{
    arr = ~(arr);
}

// FunctorShiftRight class
template <std::size_t SIZE>
BitArray<SIZE>::FunctorShiftRight::FunctorShiftRight(std::size_t shift) :
m_temp1(0), m_temp2(0), m_shift(shift)
{}

template <std::size_t SIZE>
void BitArray<SIZE>::FunctorShiftRight::operator()(std::size_t& arr)
{
    m_temp2 = arr & (~std::size_t(0) >> (WORD_SIZE - m_shift));
    arr >>= m_shift;
    arr |= m_temp1 << (WORD_SIZE - m_shift);
    m_temp1 = m_temp2;
}

// FunctorShiftLeft class
template <std::size_t SIZE>
BitArray<SIZE>::FunctorShiftLeft::FunctorShiftLeft(std::size_t shift) :
m_temp1(0), m_temp2(0) , m_shift(shift)
{}

template <std::size_t SIZE>
void BitArray<SIZE>::FunctorShiftLeft::operator()(std::size_t& arr)
{
    m_temp2 = arr & (~std::size_t(0) << (WORD_SIZE - m_shift));
    arr <<= m_shift;
    arr |= m_temp1 >> (WORD_SIZE - m_shift);
    m_temp1 = m_temp2;
    
}

}

#endif  // ILRD_RD136_7_BIT_ARRAY_HPP