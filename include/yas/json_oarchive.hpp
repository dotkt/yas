
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

#ifndef _yas__json_oarchive_hpp__included_
#define _yas__json_oarchive_hpp__included_

#include <yas/serializers/detail/properties.hpp>
#include <yas/serializers/detail/has_method_serialize.hpp>
#include <yas/serializers/detail/has_function_serialize.hpp>

#include <yas/serializers/detail/selector.hpp>
#include <yas/serializers/detail/memstream.hpp>

//#include <yas/serializers/json/utility/pod_serializers.hpp>
//#include <yas/serializers/json/utility/usertype_serializers.hpp>
//#include <yas/serializers/json/utility/autoarray_serializers.hpp>
//#include <yas/serializers/json/utility/buffer_serializers.hpp>

#include <yas/tools/buffer.hpp>
#include <yas/tools/noncopyable.hpp>

namespace yas {

/***************************************************************************/

struct json_mem_oarchive
	:detail::omemstream
	,detail::archive_information<e_archive_type::json, e_direction::out, json_mem_oarchive>
	,private detail::noncopyable
{
	json_mem_oarchive(header_t::type op = header_t::with_header)
		:detail::omemstream()
		,detail::archive_information<e_archive_type::json, e_direction::out, json_mem_oarchive>(this, op)
	{}
	json_mem_oarchive(char* ptr, size_t size, header_t::type op = header_t::with_header)
		:detail::omemstream(ptr, size)
		,detail::archive_information<e_archive_type::json, e_direction::out, json_mem_oarchive>(this, op)
	{}

	template<typename T>
	json_mem_oarchive& operator& (const T& v) {
		using namespace detail;
		serializer<
			type_propertyes<T>::value,
			serialization_method<T, json_mem_oarchive>::value,
			e_archive_type::json,
			e_direction::out,
			T
		>::apply(*this, v);

		return *this;
	}
};

/***************************************************************************/

} // namespace yas

#endif // _yas__json_oarchive_hpp__included_
