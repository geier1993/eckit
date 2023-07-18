/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation
 * nor does it submit to any jurisdiction.
 */

/// @author Philipp Geier

/// @date May 2023

#pragma once


#if (defined __cplusplus) && (__cplusplus >= 201703L)

#include "eckit/utils/CompilerAbstractions.h"

// Aligned allocator - C++17...
// Modified from https://stackoverflow.com/questions/60169819/modern-approach-to-making-stdvector-allocate-aligned-memory/70994249#70994249
#include <limits>
#include <new>

namespace eckit {

template<typename T, std::size_t Alignment = CACHELINE_SIZE_DESTRUCTIVE>
class AlignedAllocator
{
public:
    static_assert(Alignment >= alignof(T), "Alignment is too small for this type");

    using value_type = T;

    // Rebind allocator with different type whil preserving Alignment
    template<class OtherT>
    struct rebind
    {
        using other = AlignedAllocator<OtherT, Alignment>;
    };

public:
    constexpr AlignedAllocator() noexcept = default;

    constexpr AlignedAllocator( const AlignedAllocator& ) noexcept = default;

    template<typename U>
    constexpr AlignedAllocator( AlignedAllocator<U, Alignment> const& ) noexcept
    {}

    [[nodiscard]] T* allocate(std::size_t numElem) {
        if (numElem > std::numeric_limits<std::size_t>::max()/sizeof(T)) {
            throw std::bad_array_new_length();
        }
        return reinterpret_cast<T*>(::operator new[](numElem * sizeof(T), std::align_val_t{Alignment}));
    }

    void deallocate(T* allocatedPointer, [[maybe_unused]] std::size_t numElemAllocated) noexcept {
        ::operator delete[](allocatedPointer, std::align_val_t{Alignment});
    }
};

}

#endif
