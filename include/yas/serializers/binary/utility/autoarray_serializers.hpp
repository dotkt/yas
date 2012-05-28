
// Copyright (c) 2010-2012 niXman (i dot nixman dog gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef _yas__binary__autoarray_serializer_hpp__included_
#define _yas__binary__autoarray_serializer_hpp__included_

#include <stdexcept>

#include <yas/config/config.hpp>
#include <yas/tools/utf8conv.hpp>
#include <yas/tools/static_assert.hpp>
#include <yas/mpl/type_traits.hpp>
#include <yas/mpl/metafunctions.hpp>
#include <yas/serializers/detail/properties.hpp>
#include <yas/serializers/detail/selector.hpp>

namespace yas {
namespace detail {

/***************************************************************************/

template<typename T, size_t N>
struct serializer<
	e_type_type::e_type_type::is_array_of_pods,
	e_ser_method::use_internal_serializer,
	e_archive_type::binary,
	e_direction::out,
	T[N]
> {
	template<
		 typename Archive
		,typename U
	>
	static void apply(Archive& ar, const U(&v)[N], typename enable_if<is_any_of<U, char, signed char, unsigned char> >::type* = 0) {
		(ar & static_cast<yas::uint32_t>(N-1)).write(v, N-1);
	}

	template<
		 typename Archive
		,typename U
	>
	static void apply(Archive& ar, const U(&v)[N], typename disable_if<is_any_of<U, char, signed char, unsigned char> >::type* = 0) {
		(ar & static_cast<yas::uint32_t>(N*sizeof(T))).write(v, N*sizeof(T));
	}
};

template<typename T, size_t N>
struct serializer<
	e_type_type::e_type_type::is_array_of_pods,
	e_ser_method::use_internal_serializer,
	e_archive_type::binary,
	e_direction::in,
	T[N]
> {
	template<
		 typename Archive
		,typename U
	>
	static void apply(Archive& ar, U(&v)[N], typename enable_if<is_any_of<U, char, signed char, unsigned char> >::type* = 0) {
		yas::uint32_t size = 0;
		ar & size;
		if ( size != N-1 ) throw std::runtime_error("bad array size");
		ar.read(v, size);
		v[size] = 0;
	}

	template<
		 typename Archive
		,typename U
	>
	static void apply(Archive& ar, U(&v)[N], typename disable_if<is_any_of<U, char, signed char, unsigned char> >::type* = 0) {
		yas::uint32_t size = 0;
		ar & size;
		if ( size != (N*sizeof(T)) ) throw std::runtime_error("bad array size");
		ar.read(v, sizeof(T)*N);
	}
};

/***************************************************************************/

template<typename T, size_t N>
struct serializer<
	e_type_type::e_type_type::is_array,
	e_ser_method::use_internal_serializer,
	e_archive_type::binary,
	e_direction::out,
	T[N]
> {
	template<typename Archive>
	static void apply(Archive& ar, const T(&v)[N]) {
		ar & static_cast<yas::uint32_t>(N);
		for ( size_t idx = 0; idx < N; ++idx ) {
			ar & v[idx];
		}
	}
};

/***************************************************************************/

template<typename T, size_t N>
struct serializer<
	e_type_type::e_type_type::is_array,
	e_ser_method::use_internal_serializer,
	e_archive_type::binary,
	e_direction::in,
	T[N]
> {
	template<typename Archive>
	static void apply(Archive& ar, T(&v)[N]) {
		yas::uint32_t size = 0;
		ar & size;
		if ( size != N ) throw std::runtime_error("bad array size");
		for ( size_t idx = 0; idx < N; ++idx ) {
			ar & v[idx];
		}
	}
};

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // _yas__binary__autoarray_serializer_hpp__included_
