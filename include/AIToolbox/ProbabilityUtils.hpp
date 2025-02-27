#ifndef AI_TOOLBOX_PROBABILITY_UTILS_HEADER_FILE
#define AI_TOOLBOX_PROBABILITY_UTILS_HEADER_FILE

#include <cstddef>
#include <cmath>
#include <limits>
#include <random>
#include <algorithm>

namespace AIToolbox {

    /**
     * @brief This function checks if two doubles near [0,1] are reasonably equal.
     *
     * The order of the parameter is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are close enough, false otherwise.
     */
    inline bool checkEqualSmall(double a, double b) {
        return ( std::fabs(a - b) < std::numeric_limits<double>::epsilon() );
    }

    /**
     * @brief This function checks if two doubles near [0,1] are reasonably different.
     *
     * The order of the parameter is not important.
     *
     * @param a The first number to compare.
     * @param b The second number to compare.
     *
     * @return True if the two numbers are far away enough, false otherwise.
     */
    inline bool checkDifferentSmall(double a, double b) {
        return !checkEqualSmall(a,b);
    }

    /**
     * @brief This function checks whether the supplied vector is a correct probability vector.
     *
     * This function verifies basic probability conditions on the
     * supplied container. The sum of all elements must be 1, and
     * all elements must be >= 0 and <= 1.
     *
     * The container needs to support data access through
     * operator[]. In addition, the dimension of the
     * container must match the one provided as argument.
     *
     * This is important, as this function DOES NOT perform
     * any size checks on the external container.
     *
     * Internal values of the container will be converted to double,
     * so the conversion T to double must be possible.
     *
     * @tparam T The external transition container type.
     * @param in The external transitions container.
     * @param d The size of the supplied container.
     *
     * @return True if the container satisfies probability constraints,
     *         and false otherwise.
     */
    template <typename T>
    bool isProbability(size_t d, const T & in) {
        double p = 0.0;
        for ( size_t i = 0; i < d; ++i ) {
            double value = static_cast<double>(in[i]);
            if ( value < 0.0 || value > 1.0 ) return false;
            p += value;
        }
        if ( checkDifferentSmall(p, 1.0) )
            return false;

        return true;
    }

    /**
     * @brief This function samples an index from a probability vector.
     *
     * This function randomly samples an index between 0 and d, given a
     * vector containing the probabilities of sampling each of the indexes.
     *
     * For performance reasons this function does not verify that the input
     * container is effectively a probability.
     *
     * The generator has to be supplied to the function, so that different
     * objects are able to maintain different generators, to reduce correlations
     * between different samples. The generator has to be compatible with
     * std::uniform_real_distribution<double>, since that is what is used
     * to obtain the random sample.
     *
     * @tparam T The type of the container vector to sample.
     * @tparam G The type of the generator used.
     * @param in The external probability container.
     * @param d The size of the supplied container.
     * @param generator The generator used to sample.
     *
     * @return An index in range [0,d-1].
     */
    template <typename T, typename G>
    size_t sampleProbability(size_t d, const T& in, G& generator) {
        static std::uniform_real_distribution<double> sampleDistribution(0.0, 1.0);
        double p = sampleDistribution(generator);

        for ( size_t i = 0; i < d; ++i ) {
            if ( in[i] > p ) return i;
            p -= in[i];
        }
        return d-1;
    }

    template <typename InputIterator, typename OutputIterator>
    void normalizeProbability(InputIterator begin, InputIterator end, OutputIterator out) {
        double norm = static_cast<double>(std::accumulate(begin, end, 0.0));
        std::transform(begin, end, out, [norm](decltype(*begin) t){ return t/norm; });
    }
}

#endif
