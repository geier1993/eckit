/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include "eckit/container/LowLatencyQueue.h"

#include "eckit/log/Log.h"
#include "eckit/testing/Test.h"

#include <iostream>

using namespace eckit;
using namespace eckit::testing;
using namespace eckit::types;

namespace eckit {
namespace test {

using lowlatency::DefaultPushPopSpinFunctor;
using lowlatency::ConditionedPushPopSpinFunctor;
using lowlatency::LowLatencyQueue;


template <typename ElemT, typename Queue, typename DataGen>
void testQueue(Queue& queue, DataGen&& dataGen, unsigned int numRounds, unsigned int numEnqueues, unsigned int numThreads) {
    std::vector<std::thread> threads;
    
    std::cout << "Starting " << numThreads << std::endl;
    auto start = std::chrono::steady_clock::now();
    for (unsigned int i = 0; i < numThreads; ++i) {
        threads.push_back(std::thread([i, numEnqueues, &queue, &dataGen]() {
            for (unsigned long no = 0; no < 10; ++no) {
                // if (i==0) {
                std::cout << "Thread " << std::this_thread::get_id() << " round " << no << std::endl;
                std::cout << "Queue size: " << queue.size() << "/" << queue.capacity() << std::endl;

                // }
                // const auto nEnd = std::numeric_limits<INDT>::max()/8-2;
                for (unsigned long n = 0; n < numEnqueues; ++n) {
                    std::chrono::time_point<std::chrono::steady_clock> startSpin = std::chrono::steady_clock::now();

                    if (i % 2 == 0) {
                        queue.push(dataGen(i, n));
                    } else {
                        std::optional<ElemT> ret;
                        queue.pop([&ret](ElemT&& v) noexcept(noexcept(ret = std::move(v))) {
                            ret = std::move(v);  // Will call move contructor
                        });
                        ASSERT(ret);
                    }
                };
            }
            if (i % 2 == 0) {
                std::cout << "Thread " << i << " done pushing " << std::endl;
            }
            else {
                std::cout << "Thread " << i << " done pulling " << std::endl;
            }
        }));
    }
    for (auto& t : threads) {
        t.join();
    }
    std::cout << "Queue size: " << queue.size() << "/" << queue.capacity() << std::endl;
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in microseconds: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs" << std::endl;
}

decltype(auto) createStringFunctor() {
    return [](unsigned long i, unsigned long n){ return std::string("Elem ") + std::to_string(i) + std::string(" n ") + std::to_string(n); };
}

template<typename Int>
decltype(auto) createIntFunctor() {
    return [](unsigned long i, unsigned long n){ return static_cast<Int>(i + n); };
}


//----------------------------------------------------------------------------------------------------------------------

CASE("LowLatencyQueue<string, AlignedAllocator>(256)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>>;
    Queue queue(256); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, AlignedAllocator>(128)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>, DefaultPushPopSpinFunctor<>>;
    Queue queue(128); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, AlignedAllocator>(64)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>, DefaultPushPopSpinFunctor<>>;
    Queue queue(64); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, AlignedAllocator>(32)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>, DefaultPushPopSpinFunctor<>>;
    Queue queue(32); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, AlignedAllocator>(16)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>, DefaultPushPopSpinFunctor<>>;
    Queue queue(16); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, AlignedAllocator>(8)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>, DefaultPushPopSpinFunctor<>>;
    Queue queue(8); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, AlignedAllocator>(4)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>, DefaultPushPopSpinFunctor<>>;
    Queue queue(4); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, AlignedAllocator>(2)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>, DefaultPushPopSpinFunctor<>>;
    Queue queue(2); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, AlignedAllocator>(123)") {
    using Queue = LowLatencyQueue<std::string, AlignedAllocator<std::string>, DefaultPushPopSpinFunctor<>>;
    Queue queue(123); 
    testQueue<std::string>(queue, createStringFunctor(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}



CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(256)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(256); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(128)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(128); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(64)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(64); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(32)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(32); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(16)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(16); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(8)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(8); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(4)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(4); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(2)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(2); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, AlignedAllocator>(123)") {
    using Queue = LowLatencyQueue<unsigned long, AlignedAllocator<unsigned long>, DefaultPushPopSpinFunctor<>>;
    Queue queue(123); 
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}



CASE("LowLatencyQueue<int, AlignedAllocator>(256)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(256); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, AlignedAllocator>(128)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(128); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, AlignedAllocator>(64)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(64); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, AlignedAllocator>(32)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(32); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, AlignedAllocator>(16)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(16); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, AlignedAllocator>(8)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(8); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, AlignedAllocator>(4)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(4); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, AlignedAllocator>(2)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(2); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, AlignedAllocator>(123)") {
    using Queue = LowLatencyQueue<int, AlignedAllocator<int>, DefaultPushPopSpinFunctor<>>;
    Queue queue(123); 
    testQueue<int>(queue, createIntFunctor<int>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}


CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(256)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(256); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(128)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(128); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(64)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(64); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(32)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(32); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(16)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(16); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(8)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(8); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(4)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(4); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(2)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(2); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, AlignedAllocator>(123)") {
    using Queue = LowLatencyQueue<std::uint16_t, AlignedAllocator<std::uint16_t>, DefaultPushPopSpinFunctor<>>;
    Queue queue(123); 
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}


CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(256)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(256); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(128)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(128); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(64)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(64); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(32)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(32); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(16)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(16); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(8)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(8); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(4)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(4); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(2)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(2); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, AlignedAllocator>(123)") {
    using Queue = LowLatencyQueue<unsigned char, AlignedAllocator<unsigned char>, DefaultPushPopSpinFunctor<>>;
    Queue queue(123); 
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10, queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}

    
CASE("LowLatencyQueue<string, 256>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 256>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, 128>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 128>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, 64>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 64>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, 32>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 32>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, 16>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 16>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, 8>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 8>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, 4>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 4>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, 2>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 2>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<string, 123>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 123>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}



CASE("LowLatencyQueue<unsigned long, 256>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 256>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, 128>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 128>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, 64>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 64>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, 32>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 32>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, 16>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 16>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, 8>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 8>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, 4>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 4>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, 2>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 2>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned long, 123>()") {
    using Queue = LowLatencyQueue<unsigned long, std::integral_constant<std::size_t, 123>>;
    Queue queue{};
    testQueue<unsigned long>(queue, createIntFunctor<unsigned long>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}



CASE("LowLatencyQueue<int, 256>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 256>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, 128>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 128>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, 64>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 64>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, 32>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 32>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, 16>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 16>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, 8>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 8>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, 4>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 4>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, 2>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 2>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<int, 123>()") {
    using Queue = LowLatencyQueue<int, std::integral_constant<std::size_t, 123>>;
    Queue queue{};
    testQueue<int>(queue, createIntFunctor<int>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}



CASE("LowLatencyQueue<std::uint16_t, 256>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 256>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, 128>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 128>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, 64>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 64>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, 32>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 32>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, 16>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 16>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, 8>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 8>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, 4>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 4>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, 2>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 2>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<std::uint16_t, 123>()") {
    using Queue = LowLatencyQueue<std::uint16_t, std::integral_constant<std::size_t, 123>>;
    Queue queue{};
    testQueue<std::uint16_t>(queue, createIntFunctor<std::uint16_t>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}



CASE("LowLatencyQueue<unsigned char, 256>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 256>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, 128>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 128>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() / std::thread::hardware_concurrency() * 10, std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, 64>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 64>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() / std::thread::hardware_concurrency() * 10, std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, 32>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 32>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() / std::thread::hardware_concurrency() * 10, std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, 16>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 16>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() / std::thread::hardware_concurrency() * 10, std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, 8>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 8>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, 4>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 4>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, 2>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 2>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}
CASE("LowLatencyQueue<unsigned char, 123>()") {
    using Queue = LowLatencyQueue<unsigned char, std::integral_constant<std::size_t, 123>>;
    Queue queue{};
    testQueue<unsigned char>(queue, createIntFunctor<unsigned char>(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}



CASE("LowLatencyQueue<string, 64, uint16_t>() - test internal head/tail resetting by exceeding integer limits") {
    using INDT = uint16_t;
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 64>, DefaultPushPopSpinFunctor<>, INDT>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  std::numeric_limits<INDT>::max(), std::thread::hardware_concurrency());
}


CASE("LowLatencyQueue<string, 64, ConditionedPushPopSpinFunctor<>>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 64>, ConditionedPushPopSpinFunctor<>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}

CASE("LowLatencyQueue<string, 4, ConditionedPushPopSpinFunctor<>>()") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 4>, ConditionedPushPopSpinFunctor<>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}

CASE("LowLatencyQueue<string, 64, ConditionedPushPopSpinFunctor<0>>() (wait directly without spinning)") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 64>, ConditionedPushPopSpinFunctor<>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}

CASE("LowLatencyQueue<string, 4, ConditionedPushPopSpinFunctor<0>>() (wait directly without spinning)") {
    using Queue = LowLatencyQueue<std::string, std::integral_constant<std::size_t, 4>, ConditionedPushPopSpinFunctor<>>;
    Queue queue{};
    testQueue<std::string>(queue, createStringFunctor(), 10,  queue.capacity() * 100 / std::thread::hardware_concurrency(), std::thread::hardware_concurrency());
}

//----------------------------------------------------------------------------------------------------------------------

}  // namespace test
}  // namespace eckit

int main(int argc, char** argv) {
    return run_tests(argc, argv);
}
