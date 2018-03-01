#pragma once
#include <gmock/gmock-matchers.h>
#include <gmock/gmock-more-actions.h>

namespace
{
class DoubleNear : public testing::MatcherInterface<std::tuple<const double&, const double&>>
{
public:
    explicit DoubleNear(double max_abs_error) : m_maxAbsError(max_abs_error) { }

    bool MatchAndExplain(std::tuple<const double&, const double&> x, testing::MatchResultListener* listener) const override
    {
        const auto first = std::get<0>(x);
        const auto second = std::get<1>(x);
        const auto absoluteDifference = abs(first - second);
        if (absoluteDifference > m_maxAbsError && listener->IsInterested())
        {
            *listener->stream() << "absolute difference is " << absoluteDifference;
        }
        return absoluteDifference < m_maxAbsError;
    }

    void DescribeTo(::std::ostream* os) const override
    {
        *os << "absolute difference is lower than " << m_maxAbsError;
    }

private:
    const double m_maxAbsError;
};

testing::Matcher<std::tuple<const double&, const double&>> double_near(double max_abs_error)
{
    static auto matcher = testing::MakeMatcher(new DoubleNear(max_abs_error));
    return matcher;
}
}