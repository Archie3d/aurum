#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include <complex>

namespace au {
namespace dsp {

/**
 * Abstract filter.
 */
class Filter
{
public:
    using Complex = std::complex<double>;

    /// Filter type.
    enum class Type
    {
        LowPass,
        HighPass,
        BandPass,
        Notch,
        AllPass,
        PeakingEq,
        LowShelf,
        HighShelf
    };

    Filter();
    Filter(const Filter&) = delete;
    Filter& operator =(const Filter&) = delete;
    virtual ~Filter() = default;

    double sampleRate() const noexcept { return m_sampleRate; }
    virtual void setSampleRate(double sr) { m_sampleRate = sr; }
    double lastOutput() const noexcept { return m_output; }

    virtual void reset();
    double process(double x);

    virtual double tick(double x) = 0;
    virtual Complex frequencyResponse(double f) const = 0;

private:
    double m_sampleRate;
    double m_output;
};

/**
 * @brief Filters chain defined at compilation time.
 */
template<class... F>
class StaticFilterChain;

template<>
class StaticFilterChain<> : public Filter
{
public:
    StaticFilterChain() {}

    double tick(double x) override { return x; }
    void reset() override { Filter::reset(); }
    Complex frequencyResponse(double) const override { return Complex(1.0); }

    void setSampleRate(double sr) override
    {
        Filter::setSampleRate(sr);
        setChainSampleRate(sr);
    }

    void setChainSampleRate(double) {}
};

template<class F, class... Fs>
class StaticFilterChain<F, Fs...> : public Filter
{
public:

    using Head = F;
    using Tail = StaticFilterChain<Fs...>;

    StaticFilterChain()
        : m_head(),
          m_tail()
    {
    }

    void setSampleRate(double sr) override
    {
        Filter::setSampleRate(sr);
        m_head.setSampleRate(sr);
        m_tail.setSampleRate(sr);
    }

    double tick(double x) override
    {
        return m_tail.tick(m_head.tick(x));
    }

    void reset() override
    {
        m_head.reset();
        m_tail.reset();
    }

    Complex frequencyResponse(double f) const override
    {
        return m_head.frequencyResponse(f) * m_tail.frequencyResponse(f);
    }

    template <int N, class T>
    struct Iterator;

    template <class T>
    struct Iterator<0, T>
    {
        static auto& get(T& t)
        {
            return t.head();
        }
    };

    template <int N, class T>
    struct Iterator
    {
        using Next = Iterator <N - 1, typename T::Tail>;
        static auto& get(T& t)
        {
            return Next::get(t.tail());
        }
    };

    template <int N>
    auto& get()
    {
        return Iterator<N, StaticFilterChain<F, Fs...> >::get(*this);
    }

    auto& head() { return m_head; }
    auto& tail() { return m_tail; }

private:
    Head m_head;
    Tail m_tail;
};

} // namespace dsp
} // namespace au

#endif // FILTER_H_INCLUDED
